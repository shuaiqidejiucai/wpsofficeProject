#include "wppcomment.h"
#include <QDebug>
#include "variant.h"
#include "wpsapiex.h"
#include <QApplication>
#include <QClipboard>
#include <QBuffer>
#include <QMimeData>
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
        qDebug() <<"get WpsRpcClient erro";
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
        qDebug() << "open ok";
        return true;
    }
    else
    {
        qDebug() << "open fail";
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
            if(!qsStr.isEmpty())
            {
                qsStrList.append(qsStr);
            }
        }
    }
    return qsStrList;
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
            if(shapePtr)
            {
                MsoShapeType type;
                shapePtr->get_Type(&type);
                if(fileType == ImageType && (type == msoPicture || type == msoLinkedPicture))
                {
                    shapeList.append(shapePtr);
                }
                else if(fileType == OleFileType && (type == msoEmbeddedOLEObject || type == msoLinkedOLEObject))
                {
                    shapeList.append(shapePtr);
                }
                else
                {
                    qDebug()<<"other type";
                }
            }
        }
    }

    bool isContinue = false;
    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> shapePtr = shapeList.at(i);
        if(fileType == ImageType)
        {
            WppComment::getPictureForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else if(fileType == OleFileType)
        {
            WppComment::getOldFileDataForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else
        {
            qDebug()<<"UnKnown Type";
        }
        if(!isContinue)
        {
            return;
        }
    }
    return;
}

bool WppComment::getPictureForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    shapePtr->Copy();

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
        qDebug()<<"Unknown Type";
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
                qDebug()<<"No Operate Type";
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

    long count = 0;
    slidesPtr->get_Count(&count);
    for (long i = 1; i <= count; ++i)
    {
        VARIANT v;
        VariantInit(&v);
        V_VT(&v) = VT_I4;
        V_I4(&v) = i;

        long slidRangeCount = 0;
        slidesPtr->get_Count(&slidRangeCount);
        for(long j = 1; j <= slidRangeCount; ++j)
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
                ks_stdptr<TextFrame> textFramePtr;
                shapePtr->get_TextFrame(&textFramePtr);
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
