#include "wpscomment.h"
#include <QPixmap>
#include "variant.h"
#include <QDebug>
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <quazipfile.h>
#include <QBuffer>
#include <libolecf.h>
#include <libbfio_handle.h>
#include <libbfio_memory_range.h>
#include <QStack>
#include <QApplication>
using namespace wpsapi;
using namespace kfc;
using namespace wpsapiex;

WpsComment::WpsComment(QObject *parent)
    : QObject{parent}
    ,m_rpcClient(nullptr)
{}

bool WpsComment::initWpsApplication()
{
    if(m_rpcClient && m_rpcClient->getWpsApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        m_spApplication->put_Visible(false);
        m_spApplication->get_Documents(&m_spDocs);
        return true;
    }
    return false;
}

bool WpsComment::initWpsRpcClient()
{
    HRESULT hr = createWpsRpcInstance(&m_rpcClient);
    if (hr != S_OK)
    {
        //qDebug() <<"get WpsRpcClient erro";
        return false;
    }
    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/wps"));
    m_rpcClient->setProcessPath(StrWpsAddress);
    SysFreeString(StrWpsAddress);

    QVector<BSTR> vArgs;
    vArgs.resize(2);
    vArgs[0] =  SysAllocString(__X("-shield"));
    vArgs[1] =  SysAllocString(__X("-multiply"));
    m_rpcClient->setProcessArgs(vArgs.size(), vArgs.data());
    for (int i = 0; i < vArgs.size(); i++)
    {
        SysFreeString(vArgs.at(i));
    }
    return true;
}

bool WpsComment::openWPSDoc(const QString &fileName)
{
    ks_bstr fileNameBstr (fileName.utf16());

    VARIANT fileNameVar;
    VariantInit(&fileNameVar);
    V_VT(&fileNameVar) = VT_BSTR;
    V_BSTR(&fileNameVar) = fileNameBstr;

    VARIANT visibleVar;
    VariantInit(&visibleVar);
    V_VT(&visibleVar) = VT_BOOL;
    V_BOOL(&visibleVar) = false;

    VARIANT argWps[15];
    for(int i = 0; i < 15; ++i)
    {
        VariantInit(&argWps[i]);
    }

    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

    HRESULT hr = m_spDocs->Open(&fileNameVar, &missing, &missing, &missing, &missing, &missing, &missing, &missing, &missing
                   , &missing, &missing, &missing, &missing, &missing, &missing, &missing,(Document**)&m_spDocument);
    // HRESULT hr = m_spDocs->Open(&fileNameVar, &argWps[0], &argWps[1],&argWps[2],&argWps[3],
    //                &argWps[4],&argWps[5],&argWps[6],&argWps[7],&argWps[8],&argWps[9],
    //                &argWps[10],&argWps[11],&argWps[12],&argWps[13],&argWps[14], (Document**)&m_spDocument);

    if (SUCCEEDED(hr))
    {
        //qDebug() << "open ok";
        return true;
    }
    else
    {
        //qDebug() << "open fail";
    }

    return false;
}

bool WpsComment::closeWPSDoc()
{
    KComVariant varSaveOptions(wdDoNotSaveChanges);
    KComVariant varOriginalFormat(wdWordDocument);
    KComVariant varRouteDocument;
    HRESULT hr = m_spDocument->Close(&varSaveOptions, &varOriginalFormat, &varRouteDocument);
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

void WpsComment::closeApp()
{
    KComVariant vars[3];
    if (m_spApplication != NULL)
    {
        m_spApplication->Quit(vars, vars+1, vars+2);
        m_rpcClient = nullptr;
        m_spDocs = NULL;
        m_spApplication = NULL;
    }
}

bool WpsComment::saveWPSDoc()
{
    HRESULT hr = m_spDocument->Save();
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

QStringList WpsComment::GetWPSText()
{
    QStringList qsTextList;
    QList<kfc::ks_stdptr<Range> > rangeList = GetTextRange();
    for(int i = 0; i < rangeList.count(); ++i)
    {
        kfc::ks_stdptr<Range> range = rangeList.at(i);
        if(!range)
        {
            continue;
        }
        BSTR bstr;
        range->get_Text(&bstr);
        QString text = GetBSTRText(bstr);
        text.remove("\u0001");
        text.remove("\r");
        if(!text.isEmpty())
        {
            qsTextList.append(text);
        }
    }
    return qsTextList;
}

void WpsComment::replaceTextForWPSDoc(const QString &findText, const QString &replaceText)
{
    QList<ks_stdptr<Range>> rangList = GetTextRange();
    for(int i = 0; i < rangList.count(); ++i)
    {
        ks_stdptr<Range> textRangePtr = rangList.at(i);
        if(!textRangePtr)
        {
            continue;
        }
        //ks_bstr findTextBSTR(findText.utf16());
        ks_bstr replaceTextBSTR(replaceText.utf16());
        //ks_stdptr<Range> replaceTextRange;
        BSTR bstr;
        textRangePtr->get_Text(&bstr);
        QString qsText = GetBSTRText(bstr);
        int index = qsText.indexOf(findText);
        if(index != -1)
        {
            int length = findText.count();
            ks_stdptr<Range> tmpRange = textRangePtr;
            tmpRange->SetRange(index+1, index + length+1);
            tmpRange->put_Text(replaceTextBSTR);
        }
    }
}

bool WpsComment::insertTextForTextRange(kfc::ks_stdptr<wpsapi::Range> range, int start, int length, const QString &qsTexts, bool after)
{
    if(!range)
    {
        return false;
    }
    range->SetRange(start + 1, start + 1 + length);
    HRESULT hr;
    ks_bstr bstr(qsTexts.utf16());
    if(after)
    {
        hr = range->InsertAfter(bstr);
    }
    else
    {
        hr = range->InsertBefore(bstr);
    }
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

void WpsComment::extractPicture(GetNextOleDataFun imageFunPtr)
{
    if(!imageFunPtr)
    {
        return;
    }
    if(!m_spDocument)
    {
        return;
    }
    extractFile(ImageType, imageFunPtr);
}

void WpsComment::getOleFileData(GetNextOleDataFun oleDataPtr)
{
    if(!oleDataPtr)
    {
        return;
    }
    if(!m_spDocument)
    {
        return;
    }

    extractFile(OleFileType, oleDataPtr);
}

bool WpsComment::delFile(long rangeStart, long rangeEnd)
{
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return false;
    }
    HRESULT hr = spSelection->SetRange(rangeStart, rangeEnd);
    if(!SUCCEEDED(hr))
    {
        return false;
    }
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;
    long prop = 0;
    spSelection->Delete(&missing, &missing, &prop);
    if(prop != 0)
    {
        return true;
    }
    return false;
}

QList<kfc::ks_stdptr<Range> > WpsComment::GetTextRange()
{
    QList<ks_stdptr<Range> > rangeList;
    if(!m_spDocument)
    {
        return rangeList;
    }
    ks_stdptr<StoryRanges> storyRanges;
    m_spDocument->get_StoryRanges(&storyRanges);
    if(!storyRanges)
    {
        return rangeList;
    }

    ks_stdptr<Range> allRange;

    VARIANT start;
    VariantInit(&start);
    V_VT(&start) = VT_I4;
    V_I4(&start) = 0;

    VARIANT end;
    VariantInit(&end);
    V_VT(&end) = VT_I4;
    V_I4(&end) = 0;

    ks_stdptr<Range> tmpRange;
    m_spDocument->Range(&start,&end, &tmpRange);

    VARIANT unitVar;
    V_VT(&unitVar) = VT_UINT;
    V_UINT(&unitVar) = wdParagraph;

    long prop = 0;
    tmpRange->Expand(&unitVar, &prop);


    VARIANT count;
    VariantInit(&count);
    V_VT(&count) = VT_I4;
    V_I4(&count) = 1;

    while (tmpRange)
    {
        rangeList.append(tmpRange);
        ks_stdptr<Range> tmpRange2;
        tmpRange->Next(&unitVar, &count, &tmpRange2);
        tmpRange = tmpRange2;
    }

    return rangeList;
}


void WpsComment::extractFile(EU_FileType fileType,GetNextOleDataFun fileFunPtr)
{
    if(!fileFunPtr)
    {
        return;
    }
    if(!m_spDocument)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }

    bool isContine = true;
    if(fileType == ImageType)
    {
        ks_stdptr<Shapes> shapesPtr;
        m_spDocument->get_Shapes(&shapesPtr);
        if(shapesPtr)
        {
            getPictureForShapes(shapesPtr, fileFunPtr,isContine);
        }
        if(isContine)
        {
            ks_stdptr<InlineShapes> inlineShapesPtr;
            m_spDocument->get_InlineShapes(&inlineShapesPtr);
            if(inlineShapesPtr)
            {
                getPictureForInlineShapes(inlineShapesPtr, fileFunPtr,isContine);
            }
        }
    }
    else if(fileType == OleFileType)
    {
        ks_stdptr<InlineShapes> inlineShapesPtr;
        m_spDocument->get_InlineShapes(&inlineShapesPtr);
        if(inlineShapesPtr)
        {
            getOldFileDataForInlineShapes(inlineShapesPtr, fileFunPtr,isContine);
        }

        if(isContine)
        {
            ks_stdptr<Shapes> shapesPtr;
            m_spDocument->get_Shapes(&shapesPtr);
            if(shapesPtr)
            {
                getOldFileDataForShapes(shapesPtr, fileFunPtr,isContine);
            }
        }
    }
    else
    {
        //qDebug()<<"Unknown file Type";
    }
}

void WpsComment::getOldFileDataForShapes(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!shapesPtr || !oldDataFunPtr)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<Shape>> shapeList;
    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<Shape> spShape;
        shapesPtr->Item(&shapeIndex, &spShape);
        if(spShape)
        {
            QList<kfc::ks_stdptr<wpsapi::Shape> > tmpShapeList = GetShapeGroupList<WpsApiTypes>(spShape, OleFileType);
            shapeList.append(tmpShapeList);
        }
    }


    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isOleFile = type == msoEmbeddedOLEObject || type == msoLinkedOLEObject;
        if(isOleFile)
        {
            getOldFileDataForShape(shapesPtr, spShape, oldDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void WpsComment::getOldFileDataForInlineShapes(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!shapesPtr || !oldDataFunPtr)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }
    long count = 0;
    shapesPtr->get_Count(&count);

    QList<ks_stdptr<InlineShape>> inlineShapeList;

    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        shapesPtr->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        inlineShapeList.append(shapePtr);
    }

    for(int i = 0; i < inlineShapeList.count(); ++i)
    {
        ks_stdptr<InlineShape> shapePtr = inlineShapeList.at(i);
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);

        bool isOleFile = inlineShapeType == wdInlineShapeEmbeddedOLEObject || inlineShapeType == wdInlineShapeLinkedOLEObject;

        if(isOleFile)
        {
            getOldFileDataForInlineShape(shapesPtr, shapePtr, oldDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void WpsComment::getPictureForInlineShapes(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    if(!shapesPtr || !imageDataFunPtr)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }
    QList<ks_stdptr<InlineShape>> inlineShapeList;
    long count = 0;
    shapesPtr->get_Count(&count);
    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        shapesPtr->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        inlineShapeList.append(shapePtr);
    }

    for(int i = 0; i < inlineShapeList.count(); ++i)
    {
        ks_stdptr<InlineShape> shapePtr = inlineShapeList.at(i);
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);

        bool isImage = inlineShapeType == wdInlineShapeLinkedPicture || inlineShapeType == wdInlineShapePicture;
        if(isImage)
        {
            getPictureForInlineShape(shapesPtr, inlineShapeType, imageDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void WpsComment::getPictureForShapes(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    if(!shapesPtr)
    {
        return;
    }
    if(!m_spApplication || !imageDataFunPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<Shape>> shapeList;

    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<Shape> spShape;
        shapesPtr->Item(&shapeIndex, &spShape);
        if(!spShape)
        {
            continue;
        }
        MsoShapeType wpsShapeType;
        spShape->get_Type(&wpsShapeType);
        if(wpsShapeType == msoGroup)
        {
            ks_stdptr<ShapeRange> shapeRangePtr;
            spShape->Ungroup(&shapeRangePtr);
            shapesPtr->get_Count(&shapeCount);
            i--;
            continue;
        }
        QList<kfc::ks_stdptr<Shape> > tmpShapeList = GetShapeGroupList<WpsApiTypes>(spShape, ImageType);
        shapeList.append(tmpShapeList);
    }

    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isImage = type == msoPicture || type == msoLinkedPicture;
        if(isImage)
        {
            getPictureForShape(shapesPtr, spShape, imageDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }

        }
    }
}

bool WpsComment::getOldFileDataForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{

    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

    shapePtr->Select(&missing);
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    ks_stdptr<Range> selectionRange;
    spSelection->get_Range(&selectionRange);
    spSelection->Copy();
    const QMimeData *  mdata = qApp->clipboard()->mimeData();
    if(mdata)
    {
        QByteArray data = mdata->data(MimeDataKey);
        QByteArray srcData;
        if(UtilityTool::findOleDataFromZipMemory(data, srcData))
        {
            if(srcData.isEmpty())
            {
                return result;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                //日至
            }
            else if(operaTye == ReplaceType)
            {
                float left = 0.0f;
                float top = 0.0f;
                float width = 0.0f;
                float height = 0.0f;
                shapePtr->get_Left(&left);
                shapePtr->get_Top(&top);
                shapePtr->get_Width(&width);
                shapePtr->get_Height(&height);
                ks_stdptr<Range> anchorRangePtr;
                shapePtr->get_Anchor(&anchorRangePtr);

                VARIANT unitTypeVar;
                V_VT(&unitTypeVar) = VT_I4;
                V_I4(&unitTypeVar) = wdCollapseStart;
                anchorRangePtr->Collapse(&unitTypeVar);

                VARIANT anchorVar;
                V_VT(&anchorVar) = VT_DISPATCH;
                V_DISPATCH(&anchorVar) = anchorRangePtr;

                HRESULT hr = shapePtr->Delete();
                result = OFFICE_HRESULT(hr, "delete");
                if(result)
                {
                    VARIANT leftVar;
                    VariantInit(&leftVar);
                    V_R4(&leftVar) = left;

                    VARIANT topVar;
                    VariantInit(&topVar);
                    V_R4(&topVar) = top;

                    VARIANT widthVar;
                    VariantInit(&widthVar);
                    V_R4(&widthVar) = width;

                    VARIANT heightVar;
                    VariantInit(&heightVar);
                    V_R4(&heightVar) = height;


                    ks_bstr fileNameStr(outFileInfo.qsFilePath.utf16());
                    VARIANT fileNameVar;
                    V_VT(&fileNameVar) = VT_BSTR;
                    V_BSTR(&fileNameVar) = fileNameStr;
                    ks_stdptr<Shape> outShapePtr;
                    HRESULT addOleHr = shapesPtr->AddOLEObject(&missing,&fileNameVar,&missing
                                                               ,&missing,&missing,&missing,&missing
                                                               ,&leftVar,&topVar,&widthVar,&heightVar
                                                               ,&anchorVar, &outShapePtr);
                    result = OFFICE_HRESULT(addOleHr, "AddOLEObject");
                    //日至
                }
            }
            else
            {
                //qDebug()<<"No Operate Type";
            }
        }
    }
    return result;
}

bool WpsComment::getOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    spSelection->Copy();
    const QMimeData *  mdata = qApp->clipboard()->mimeData();
    if(mdata)
    {
        QByteArray data = mdata->data(MimeDataKey);
        QByteArray srcData;
        if(UtilityTool::findOleDataFromZipMemory(data, srcData))
        {
            if(srcData.isEmpty())
            {
                return result;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                //日至
            }
            else if(operaTye == ReplaceType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                if(result)
                {
                    ks_stdptr<Range> oleFileRange;
                    shapePtr->get_Range(&oleFileRange);
                    if(oleFileRange)
                    {
                        result = true;
                        ks_bstr fileNameStr(outFileInfo.qsFilePath.utf16());
                        VARIANT fileNameVar;
                        V_VT(&fileNameVar) = VT_BSTR;
                        V_BSTR(&fileNameVar) = fileNameStr;

                        VARIANT missing;
                        VariantInit(&missing);
                        V_VT(&missing) = VT_ERROR;
                        V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;


                        VARIANT unitTypeVar;
                        V_VT(&unitTypeVar) = VT_I4;
                        V_I4(&unitTypeVar) = wdCollapseStart;

                        oleFileRange->Collapse(&unitTypeVar);

                        VARIANT rangeVar;
                        V_VT(&rangeVar) = VT_DISPATCH;
                        V_DISPATCH(&rangeVar) = oleFileRange;

                        ks_stdptr<InlineShape> inlineShapePtr;
                        HRESULT addOLEHr = shapesPtr->AddOLEObject(&missing, &fileNameVar, &missing
                                                                   ,&missing,&missing,&missing
                                                                   ,&missing, &rangeVar, &inlineShapePtr);
                        result = OFFICE_HRESULT(addOLEHr, "Add OLEObject");
                    }
                }
            }
            else
            {
                //qDebug()<<"No opreate type";
            }
        }
    }
    return result;
}

bool WpsComment::getPictureForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result  = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    spSelection->CopyAsPicture();
    QImage image = qApp->clipboard()->image();

    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "JPG");

    ST_VarantFile varantFile;
    varantFile.fileData = buf.data();

    EU_OperateType euOperateType;
    ST_VarantFile outFileInfo;
    isContinue = imageDataFunPtr(varantFile,outFileInfo ,euOperateType);
    if(euOperateType == DeleteType)
    {
        HRESULT delHr = shapePtr->Delete();
        result = OFFICE_HRESULT(delHr, "delete picture");
        //日至
    }
    else if(euOperateType == ReplaceType)
    {
        ks_stdptr<Range> inlineShapeRange;
        shapePtr->get_Range(&inlineShapeRange);
        if(!inlineShapeRange)
        {
            return result;
        }
        VARIANT unitTypeVar;
        V_VT(&unitTypeVar) = VT_I4;
        V_I4(&unitTypeVar) = wdCollapseStart;

        inlineShapeRange->Collapse(&unitTypeVar);
        HRESULT delHr = shapePtr->Delete();

        VARIANT anchorVar;
        V_VT(&anchorVar) = VT_DISPATCH;
        V_DISPATCH(&anchorVar) = inlineShapeRange;
        result = OFFICE_HRESULT(delHr, "delete picture");
        if(result)
        {
            VARIANT missing;
            VariantInit(&missing);
            V_VT(&missing) = VT_ERROR;
            V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

            ks_bstr fileNameBstr(outFileInfo.qsFilePath.utf16());
            ks_stdptr<InlineShape> outShape;
            HRESULT AddHr = shapesPtr->AddPicture(fileNameBstr, &missing, &missing, &anchorVar, &outShape);
            result = OFFICE_HRESULT(AddHr, "Add picture");
        }
    }
    else
    {
        //qDebug()<< "No opreate type";
    }
    return result;
}

bool WpsComment::getPictureForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;
    shapePtr->Select(&missing);
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    ks_stdptr<Range> selectionRange;
    spSelection->get_Range(&selectionRange);
    spSelection->CopyAsPicture();
    QImage image = qApp->clipboard()->image();

    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "PNG");

    ST_VarantFile varantFile;
    varantFile.fileData = buf.data();

    EU_OperateType euOperateType;
    ST_VarantFile outFileInfo;
    isContinue = imageDataFunPtr(varantFile,outFileInfo ,euOperateType);
    if(euOperateType == DeleteType)
    {
        HRESULT hr = shapePtr->Delete();
        result = OFFICE_HRESULT(hr, "delete");
        //日至
    }
    else if(euOperateType == ReplaceType)
    {
        float left = 0.0f;
        float top = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        shapePtr->get_Left(&left);
        shapePtr->get_Top(&top);
        shapePtr->get_Width(&width);
        shapePtr->get_Height(&height);
        ks_stdptr<Range> anchorRangePtr;
        shapePtr->get_Anchor(&anchorRangePtr);

        VARIANT unitTypeVar;
        V_VT(&unitTypeVar) = VT_I4;
        V_I4(&unitTypeVar) = wdCollapseStart;

        anchorRangePtr->Collapse(&unitTypeVar);

        VARIANT anchorVar;
        V_VT(&anchorVar) = VT_DISPATCH;
        V_DISPATCH(&anchorVar) = anchorRangePtr;
        HRESULT hr = shapePtr->Delete();
        result = OFFICE_HRESULT(hr,"delete");
        if(result)
        {
            VARIANT leftVar;
            VariantInit(&leftVar);
            V_R4(&leftVar) = left;

            VARIANT topVar;
            VariantInit(&topVar);
            V_R4(&topVar) = top;

            VARIANT widthVar;
            VariantInit(&widthVar);
            V_R4(&widthVar) = width;

            VARIANT heightVar;
            VariantInit(&heightVar);
            V_R4(&heightVar) = height;


            ks_stdptr<Shape> outShape;
            ks_bstr filePathBstr (outFileInfo.qsFilePath.utf16());
            HRESULT AddPictureHr = shapesPtr->AddPicture(filePathBstr, &missing, &missing
                                                         ,&leftVar, &topVar, &widthVar,&heightVar
                                                         ,&anchorVar,&outShape);
            result = OFFICE_HRESULT(AddPictureHr, QString("AddPicture"));
            //日至
        }
    }
    else
    {
        //qDebug()<<"Unknown Type";
    }
    return result;
}

