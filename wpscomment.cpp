#include "wpscomment.h"
#include <QPixmap>
#include "variant.h"
#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <quazipfile.h>
#include <QBuffer>
#include <libolecf.h>
#include <libbfio_handle.h>
#include <libbfio_memory_range.h>
#include "officegeneralabilities.h"

using namespace wpsapi;
using namespace kfc;
using namespace wpsapiex;

extern QString GetBSTRText(BSTR str);

const QString MimeDataKey = "Kingsoft WPS 9.0 Format";
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
        qDebug() <<"get WpsRpcClient erro";
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
        qDebug() << "open ok";
        return true;
    }
    else
    {
        qDebug() << "open fail";
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

    bool isContinue = true;
    ks_stdptr<Shapes> shapesPtr;
    m_spDocument->get_Shapes(&shapesPtr);
    if(!shapesPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);
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
        MsoShapeType type;
        spShape->get_Type(&type);
        if(type == msoPicture || type == msoLinkedPicture)
        {
            GetPictureForShape(shapesPtr, spShape, imageFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }

    if(!isContinue)
    {
        return;
    }
    ks_stdptr<InlineShapes> inlineShapes;
    m_spDocument->get_InlineShapes(&inlineShapes);
    if(!inlineShapes)
    {
        return;
    }
    long count = 0;
    inlineShapes->get_Count(&count);
    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        inlineShapes->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);
        if(inlineShapeType == wdInlineShapePicture)
        {
            GetPictureForInlineShape(inlineShapes, inlineShapeType, imageFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

// bool WpsComment::GetOleFileData(QList<ST_OleFile>& stOleList)
// {
//     bool successful = false;
//     ks_stdptr<InlineShapes> inlineShapes;
//     m_spDocument->get_InlineShapes(&inlineShapes);
//     if(!inlineShapes)
//     {
//         return successful;
//     }
//     long count = 0;
//     inlineShapes->get_Count(&count);
//     for(int i = 1; i <= count; ++i)
//     {
//         ks_stdptr<InlineShape> shapePtr;
//         inlineShapes->Item(i, &shapePtr);
//         if(!shapePtr)
//         {
//             continue;
//         }
//         WdInlineShapeType inlineShapeType;
//         shapePtr->get_Type(&inlineShapeType);
//         if(inlineShapeType == wdInlineShapeEmbeddedOLEObject
//             || inlineShapeType == wdInlineShapeEmbeddedOLEObject
//             || inlineShapeType == wdInlineShapeOLEControlObject)
//         {
//             shapePtr->Select();
//             ks_stdptr<Selection> spSelection;
//             m_spApplication->get_Selection(&spSelection);
//             if(!spSelection)
//             {
//                 continue;
//             }
//             spSelection->Copy();
//             const QMimeData *  mdata = qApp->clipboard()->mimeData();
//             if(mdata)
//             {
//                 QByteArray data = mdata->data(MimeDataKey);
//                 QByteArray srcData;
//                 if(UtilityTool::findOleDataFromZipMemory(data, srcData))
//                 {
//                     if(srcData.isEmpty())
//                     {
//                         continue;
//                     }
//                     successful = UtilityTool::GetOleFileData(srcData, stOleList);
//                 }
//             }
//         }
//     }
//     return successful;
// }

void WpsComment::GetOleFileData(GetNextOleDataFun oleDataPtr)
{
    if(!oleDataPtr)
    {
        return;
    }
    if(!m_spDocument)
    {
        return;
    }

    bool isContinue = true;
    ks_stdptr<Shapes> shapesPtr;
    m_spDocument->get_Shapes(&shapesPtr);
    if(!shapesPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);
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
        MsoShapeType type;
        spShape->get_Type(&type);
        if(type == msoEmbeddedOLEObject || type == msoLinkedOLEObject)
        {
            GetOldFileDataForShape(shapesPtr, spShape, oleDataPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }

    if(!isContinue)
    {
        return;
    }
    ks_stdptr<InlineShapes> inlineShapes;
    m_spDocument->get_InlineShapes(&inlineShapes);
    if(!inlineShapes)
    {
        return;
    }
    long count = 0;
    inlineShapes->get_Count(&count);
    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        inlineShapes->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);
        if(inlineShapeType == wdInlineShapeEmbeddedOLEObject
            || inlineShapeType == wdInlineShapeLinkedOLEObject)
        {
            GetOldFileDataForInlineShape(inlineShapes, inlineShapeType, oleDataPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
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

void WpsComment::GetOldFileDataForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return;
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
        return;
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
                return;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                shapePtr->Delete();
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
                if(SUCCEEDED(hr))
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
                    shapesPtr->AddOLEObject(&missing,&fileNameVar,&missing
                                            ,&missing,&missing,&missing,&missing
                                            ,&leftVar,&topVar,&widthVar,&heightVar
                                            ,&anchorVar, &outShapePtr);
                    //日至
                }
            }
            else
            {

            }
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void WpsComment::GetOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return;
    }
    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return;
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
                return;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                shapePtr->Delete();
                //日至
            }
            else if(operaTye == ReplaceType)
            {
                shapePtr->Delete();
                ks_stdptr<Range> oleFileRange;
                shapePtr->get_Range(&oleFileRange);
                if(oleFileRange)
                {

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
                    shapesPtr->AddOLEObject(&missing, &fileNameVar, &missing
                                               ,&missing,&missing,&missing
                                               ,&missing, &rangeVar, &inlineShapePtr);
                    if(inlineShapePtr)
                    {
                        qDebug()<<"sucessful";
                    }
                }
            }
            else
            {

            }
        }
    }
}

void WpsComment::GetPictureForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return;
    }

    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    m_spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return;
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
        shapePtr->Delete();
        //日至
    }
    else if(euOperateType == ReplaceType)
    {
        ks_stdptr<Range> inlineShapeRange;
        shapePtr->get_Range(&inlineShapeRange);
        if(!inlineShapeRange)
        {
            return;
        }
        VARIANT unitTypeVar;
        V_VT(&unitTypeVar) = VT_I4;
        V_I4(&unitTypeVar) = wdCollapseStart;

        inlineShapeRange->Collapse(&unitTypeVar);
        HRESULT hr = shapePtr->Delete();

        VARIANT anchorVar;
        V_VT(&anchorVar) = VT_DISPATCH;
        V_DISPATCH(&anchorVar) = inlineShapeRange;

        if(SUCCEEDED(hr))
        {
            VARIANT missing;
            VariantInit(&missing);
            V_VT(&missing) = VT_ERROR;
            V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

            ks_bstr fileNameBstr(outFileInfo.qsFilePath.utf16());
            ks_stdptr<InlineShape> outShape;
            shapesPtr->AddPicture(fileNameBstr, &missing, &missing, &anchorVar, &outShape);
        }
    }
    else
    {

    }
}

void WpsComment::GetPictureForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue)
{
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return;
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
        return;
    }
    ks_stdptr<Range> selectionRange;
    spSelection->get_Range(&selectionRange);
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
        shapePtr->Delete();
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
        if(SUCCEEDED(hr))
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
            shapesPtr->AddPicture(filePathBstr, &missing, &missing
                                  ,&leftVar, &topVar, &widthVar,&heightVar
                                  ,&anchorVar,&outShape);
            //日至
        }
    }
    else
    {
        qDebug()<<"Unknown Type";
    }
}




// bool WpsComment::insertFile(long rangeStart, long rangeEnd, const QString &qsFileName)
// {
//     ks_stdptr<Selection> spSelection;
//     m_spApplication->get_Selection(&spSelection);
//     if(!spSelection)
//     {
//         return false;
//     }
//     HRESULT hr = spSelection->SetRange(rangeStart, rangeEnd);
//     if(!SUCCEEDED(hr))
//     {
//         return false;
//     }
//     spSelection->Paste();
//     ks_stdptr<Range> range;
//     spSelection->get_Range(&range);
//     if(!range)
//     {
//         return false;
//     }
//     ks_stdptr<InlineShapes> inlineShapesPtr;
//     range->get_InlineShapes(&inlineShapesPtr);
//     if(!inlineShapesPtr)
//     {
//         return false;
//     }
//     inlineShapesPtr->AddPicture()
// }

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
