#include "wppcomment.h"
#include <QDebug>
#include "qlabel.h"
#include "variant.h"
#include "wpsapiex.h"
#include <QApplication>
#include <QClipboard>
#include <QBuffer>
#include <QMimeData>
#include <QStack>
//#include <quazip.h>

using namespace wppapi;
using namespace kfc;
using namespace wpsapiex;

extern QString GetBSTRText(BSTR str);
WppComment::WppComment(QObject *parent)
    : QObject{parent},m_rpcClient(nullptr)
{}




bool WppComment::initWppApplication()
{
    if(m_rpcClient && m_rpcClient->getWppApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        MsoTriState state = msoFalse;
        m_spApplication->get_Presentations(&m_spDocs);
        return true;
    }
    return false;
}

bool WppComment::initWPPRpcClient()
{
    HRESULT hr = createWppRpcInstance(&m_rpcClient);
    if (hr != S_OK)
    {
        //qDebug() <<"get WpsRpcClient erro";
        return false;
    }

    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/wpp"));
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

bool WppComment::openWPPDoc(const QString &fileName)
{
    ks_bstr filename(fileName.utf16());
    wppapi::MsoTriState readOnly = msoFalse;
    wppapi::MsoTriState untitled = msoFalse;
    wppapi::MsoTriState withWindow = msoFalse;
    ks_stdptr<_Presentation> spPresentation;
    HRESULT hr = m_spDocs->Open(filename, readOnly, untitled, withWindow, (Presentation**)&spPresentation);
    if (SUCCEEDED(hr))
    {
        m_spPresentation = spPresentation;
        //qDebug() << "open ok";
        return true;
    }
    else
    {
        //qDebug() << "open fail";
    }

    return false;
}

bool WppComment::closeWPPDoc()
{
    HRESULT hr = m_spPresentation->Close();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    return false;
}


QStringList WppComment::GetWPPText()
{
    QStringList qsStrList;

    QList<kfc::ks_stdptr<wppapi::TextRange> > textRangeList = GetTextRange();
    for(int i = 0; i < textRangeList.count(); ++i)
    {
        ks_stdptr<TextRange> textRangePtr = textRangeList.at(i);
        if(textRangePtr)
        {
            BSTR  textPtr4;
            textRangePtr->get_Text(&textPtr4);
            QString qsStr = GetBSTRText(textPtr4);
            //qDebug()<<"text:"<<qsStr;
            if(!qsStr.isEmpty())
            {
                qsStrList.append(qsStr);
            }
        }
    }
    return qsStrList;
}

long WppComment::getPageCount()
{
    if(!m_spPresentation)
    {
        return 0;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return 0;
    }

    long slideCount = 0;
    slidesPtr->get_Count(&slideCount);
    return slideCount;
}

QList<kfc::ks_stdptr<TextRange> > WppComment::GetTextRangeForShapes(kfc::ks_stdptr<wppapi::Shapes> shapesPtr)
{
    QList<ks_stdptr<TextRange>> textRangeList;
    if(!shapesPtr)
    {
        return textRangeList;
    }
    int shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);
    for(int q = 1; q <= shapeCount + 1; ++q)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = q;
        ks_stdptr<Shape> shapePtr;
        shapesPtr->Item(shapeIndex, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }

        QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList(shapePtr);
        for(int i = 0; i < shapePtrList.count(); ++i)
        {
            ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
            ks_stdptr<TextFrame> textFramePtr;
            tmpShape->get_TextFrame(&textFramePtr);
            if(!textFramePtr)
            {
                continue;
            }
            ks_stdptr<TextRange> textRangePtr;
            textFramePtr->get_TextRange(&textRangePtr);
            if(textRangePtr)
            {
                textRangeList.append(textRangePtr);
            }
        }
    }
    return textRangeList;
}

QList<kfc::ks_stdptr<wppapi::TextRange>> WppComment::getRearks(long pageIndex)
{
    QList<ks_stdptr<TextRange>> qsRearkRangeList;
    if(!m_spPresentation)
    {
        return qsRearkRangeList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return qsRearkRangeList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);

    VARIANT rangeIndex;
    VariantInit(&rangeIndex);
    V_VT(&rangeIndex) = VT_I4;
    V_I4(&rangeIndex) = pageIndex;

    ks_stdptr<_Slide> slide;
    slidesPtr->Item(rangeIndex, (Slide**)&slide);
    if(slide)
    {
        ks_stdptr<SlideRange> slideRange;
        slide->get_NotesPage(&slideRange);
        if(slideRange)
        {
            ks_stdptr<Shapes> shapesPtr;
            slideRange->get_Shapes(&shapesPtr);
            QList<ks_stdptr<TextRange>> beizhuRangeList = GetTextRangeForShapes(shapesPtr);
            qsRearkRangeList = beizhuRangeList;
        }
    }
    return qsRearkRangeList;
}

QStringList WppComment::getRearksText()
{
    QStringList rearlksStringList;
    int count = getPageCount();
    for(int i = 1; i <= count; ++i)
    {
         QList<ks_stdptr<TextRange>> qsRearkRangeList = getRearks(i);
         for(int j = 0; j < qsRearkRangeList.count(); ++j)
         {
             ks_stdptr<TextRange> textRangePtr = qsRearkRangeList.at(j);
             BSTR bText;
             textRangePtr->get_Text(&bText);
             QString qsStr = GetBSTRText(bText);
             rearlksStringList.append(qsStr);
         }
    }
    return rearlksStringList;
}

void WppComment::replaceTextForWPPDoc(const QString &findText, const QString &replaceText)
{
    QList<ks_stdptr<TextRange>> textRangList = GetTextRange();
    for(int i = 0; i < textRangList.count(); ++i)
    {
        ks_stdptr<TextRange> textRangePtr = textRangList.at(i);
        if(!textRangePtr)
        {
            continue;
        }
        ks_bstr findTextBSTR(findText.utf16());
        ks_bstr replaceTextBSTR(replaceText.utf16());
        ks_stdptr<TextRange> replaceTextRange;
        textRangePtr->Replace(findTextBSTR,replaceTextBSTR, -1, msoFalse, msoFalse,&replaceTextRange);
    }
}

bool WppComment::insertTextForTextRange(kfc::ks_stdptr<wppapi::TextRange> range, int start, int length, const QString &qsTexts, bool after)
{
    if(!range)
    {
        return false;
    }
    ks_stdptr<TextRange> midTextRange;
    range->Characters(start, length, &midTextRange);
    if(midTextRange)
    {
        ks_bstr newBstr(qsTexts.utf16());
        ks_stdptr<TextRange> newTextRange;
        HRESULT hr;
        if(after)
        {
            hr = midTextRange->InsertAfter(newBstr, &newTextRange);
        }
        else
        {
            hr = midTextRange->InsertBefore(newBstr, &newTextRange);
        }
        if (SUCCEEDED(hr))
        {
            return true;
        }
    }
    return false;
}

void WppComment::getOleFileData(GetNextOleDataFun oleDataPtr)
{
    extractFile(OleFileType, oleDataPtr);
}

void WppComment::extractPicture(GetNextOleDataFun imageFunPtr)
{
    extractFile(ImageType, imageFunPtr);
}

void WppComment::extractPictureNomemery(const QString &qsImageDir)
{
    if(!m_spPresentation || qsImageDir.isEmpty())
    {
        return;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return;
    }

    QList<ks_stdptr<Shape>> shapeList;
    ks_stdptr<Shapes> belongShapesPtr;
    long count = 0;
    slidesPtr->get_Count(&count);
    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    for(long i = 1; i <= slidRangeCount; ++i)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = i;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        belongShapesPtr = shapesPtr;
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int j = 1; j <= shapeCount; ++j)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = j;

            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            MsoShapeType tmpShapgeType;
            shapePtr->get_Type(&tmpShapgeType);

            //if(tmpShapgeType == msoGroup)
            //{
                //ks_stdptr<ShapeRange> shapeRangePtr;
                //shapePtr->Ungroup(&shapeRangePtr);
                //long llcount = 0;
                //shapeRangePtr->get_Count(&llcount);
                //shapesPtr->get_Count(&shapeCount);
                //j--;
                //continue;
            //}

            QList<kfc::ks_stdptr<wppapi::Shape>> tmpShapeList = GetShapeGroupList(shapePtr, ImageType);
            shapeList.append(tmpShapeList);
        }
    }

    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> shapePtr = shapeList.at(i);
        QString outPAth = QString(qsImageDir + "/" + QString::number(i) + ".png");
        ks_bstr filePathBstr (outPAth.utf16());
        HRESULT opHr = shapePtr->Export(filePathBstr, ppShapeFormatPNG);
        if(SUCCEEDED(opHr))
        {
            //qDebug()<<"export successful";
        }
    }
    return;
}

void WppComment::extractFile(EU_FileType fileType, GetNextOleDataFun fileFunPtr)
{
    if(!m_spPresentation || !fileFunPtr)
    {
        return;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return;
    }

    QList<ks_stdptr<Shape>> shapeList;
    ks_stdptr<Shapes> belongShapesPtr;
    long count = 0;
    slidesPtr->get_Count(&count);
    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    for(long i = 1; i <= slidRangeCount; ++i)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = i;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        belongShapesPtr = shapesPtr;
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int j = 1; j <= shapeCount; ++j)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = j;

            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            MsoShapeType tmpShapgeType;
            shapePtr->get_Type(&tmpShapgeType);

//            if(tmpShapgeType == msoGroup)
//            {
//                ks_stdptr<ShapeRange> shapeRangePtr;
//                shapePtr->Ungroup(&shapeRangePtr);
//                long llcount = 0;
//                shapeRangePtr->get_Count(&llcount);
//                shapesPtr->get_Count(&shapeCount);
//                j--;
//                continue;
//            }

            QList<kfc::ks_stdptr<wppapi::Shape>> tmpShapeList = GetShapeGroupList(shapePtr, fileType);
            shapeList.append(tmpShapeList);
        }
    }

    bool isContinue = false;
    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> shapePtr = shapeList.at(i);
        if(fileType == ImageType)
        {
            getPictureForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else if(fileType == OleFileType)
        {
            getOldFileDataForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else
        {
            //qDebug()<<"UnKnown Type";
        }
        if(!isContinue)
        {
            return;
        }
    }
    return;
}
int pppp = 0;

bool WppComment::getPictureForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    float fleft = 0.0;
    float ftop = 0.0;
    float fwidth = 0.0;
    float fheight = 0.0;

    shapePtr->get_Left(&fleft);
    shapePtr->get_Top(&ftop);
    shapePtr->get_Width(&fwidth);
    shapePtr->get_Height(&fheight);

     ks_bstr filePathBstr (QString("/home/ft2000/mjcenv/dps-ppt/outImage/" + QString::number(pppp) + "ceshi.png").utf16());
     pppp++;
     HRESULT opHr = shapePtr->Export(filePathBstr, ppShapeFormatPNG);
     if(SUCCEEDED(opHr))
     {
         qDebug()<<"success;";
     }
     else {

         qDebug()<<"faile;";
     }
    // isContinue = true;
    // return false;
    //MsoAutoShapeType autoShapeType;
    //shapePtr->get_AutoShapeType()
    //shapePtr->Copy();
   // const QMimeData * tmpMimeData = qApp->clipboard()->mimeData();
    //tmpShapePtr->Delete();
    // QStringList formatList =  qApp->clipboard()->mimeData()->formats();
    // if(formatList.indexOf("image/png") != -1)
    // {
    //     //qDebug()<<"shape;";
    // }
    // for(int i = 0; i < formatList.count(); ++i)
    // {
    //     QString qsFormat = formatList.at(i);
    //     QByteArray datda = tmpMimeData->data(qsFormat);
    //     //qDebug()<<"data size:" << datda.size();
    // }
    isContinue = true;
    return true;
    QImage image = qApp->clipboard()->image();
    if(image.isNull())
    {
        isContinue = true;
        return result;
    }
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
        result= OFFICE_HRESULT(hr, "delete");
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
            HRESULT AddPictureHr = shapesPtr->AddPicture(filePathBstr, msoFalse, msoTrue, left, top, width, height, &outShape);
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

bool WppComment::getOldFileDataForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    shapePtr->Copy();
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

                    VARIANT missing;
                    VariantInit(&missing);
                    V_VT(&missing) = VT_ERROR;
                    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

                    ks_bstr missBstr;
                    HRESULT addOleHr = shapesPtr->AddOLEObject(left, top, width, height, missBstr, fileNameStr, msoFalse, missBstr, 0, missBstr, msoFalse, &outShapePtr);
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

QList<kfc::ks_stdptr<TextRange> > WppComment::GetTextRange()
{
    QList<kfc::ks_stdptr<wppapi::TextRange> > textRangeList;
    if(!m_spPresentation)
    {
        return textRangeList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return textRangeList;
    }

        long slidRangeCount = 0;
        slidesPtr->get_Count(&slidRangeCount);
        for(long j = 1; j <= slidRangeCount; ++j)
        {
            VARIANT rangeIndex;
            VariantInit(&rangeIndex);
            V_VT(&rangeIndex) = VT_I4;
            V_I4(&rangeIndex) = j;

            ks_stdptr<SlideRange> range;
            slidesPtr->Range(rangeIndex, &range);
            if(!range)
            {
                continue;
            }
            ks_stdptr<Shapes> shapesPtr;
            range->get_Shapes(&shapesPtr);
            if(!shapesPtr)
            {
                continue;
            }
            int shapeCount = 0;
            shapesPtr->get_Count(&shapeCount);
            for(int q = 1; q <= shapeCount + 1; ++q)
            {
                VARIANT shapeIndex;
                VariantInit(&shapeIndex);
                V_VT(&shapeIndex) = VT_I4;
                V_I4(&shapeIndex) = q;
                ks_stdptr<Shape> shapePtr;
                shapesPtr->Item(shapeIndex, &shapePtr);
                if(!shapePtr)
                {
                    continue;
                }
                QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList(shapePtr);
                for(int i = 0; i < shapePtrList.count(); ++i)
                {
                    ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
                    ks_stdptr<TextFrame> textFramePtr;
                    tmpShape->get_TextFrame(&textFramePtr);
                    if(!textFramePtr)
                    {
                        continue;
                    }
                    ks_stdptr<TextRange> textRangePtr;
                    textFramePtr->get_TextRange(&textRangePtr);
                    if(textRangePtr)
                    {
                        textRangeList.append(textRangePtr);
                    }
                }
            }
        }

    return textRangeList;
}

void WppComment::closeApp()
{
    if (m_spApplication != NULL)
    {
        m_spPresentation->Close();
        m_spApplication->Quit();
        m_spDocs.clear();
        //m_spApplicationEx.clear();
        m_spApplication.clear();
        m_rpcClient = nullptr;
    }
}

bool WppComment::saveWPPDoc()
{
    HRESULT hr = m_spPresentation->Save();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    return true;
}

QList<kfc::ks_stdptr<Shape> > WppComment::GetShapeGroupList(kfc::ks_stdptr<wppapi::Shape> shapePtr, EU_FileType fileterFileType)
{
    QStack<ks_stdptr<Shape> > shapePtrStack;
    shapePtrStack.push(shapePtr);
    QList<ks_stdptr<Shape> > shapeList;
    while (!shapePtrStack.empty())
    {
        ks_stdptr<Shape> popShapePtr = shapePtrStack.pop();
        if(popShapePtr)
        {
            MsoShapeType shapeType;
            popShapePtr->get_Type(&shapeType);
            if(shapeType == msoGroup)
            {
                ks_stdptr<ShapeRange> shapeRangePtr;
                ks_stdptr<GroupShapes> groupShapesPtr;
                shapePtr->get_GroupItems(&groupShapesPtr);
                if(groupShapesPtr)
                {
                    int groupShapeCount = 0;
                    groupShapesPtr->get_Count(&groupShapeCount);
                    for(int i = 1; i <= groupShapeCount; ++i)
                    {
                        VARIANT index;
                        VariantInit(&index);
                        V_VT(&index) = VT_I4;
                        V_I4(&index) = i;
                        ks_stdptr<Shape> groupShapePtr;
                        groupShapesPtr->Item(index, &groupShapePtr);
                        shapePtrStack.push(groupShapePtr);
                    }
                }
                continue;
            }
            bool isImage = fileterFileType == ImageType && (shapeType == msoPicture || shapeType == msoLinkedPicture || shapeType == msoAutoShape);
            bool isOleFile = fileterFileType == OleFileType && (shapeType == msoEmbeddedOLEObject || shapeType == msoLinkedOLEObject);
            bool isAllfileType = fileterFileType == AllFileType;
            if(isImage || isOleFile || isAllfileType)
            {
                shapeList.append(popShapePtr);
            }
            else
            {
                //qDebug()<<"shape type:"<<shapeType;
            }
        }
    }
    return shapeList;
}
