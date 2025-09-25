#include "wppcomment.h"
#include <QDebug>
#include "variant.h"
#include "wpsapiex.h"
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

bool WppComment::insertAttachment()
{
    return false;
}

bool WppComment::extractAttachments()
{
    if(!m_spPresentation)
    {
        return false;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return false;
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
            for(int q = 1; q <= shapeCount; ++q)
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
                //shapePtr->get_OLEFormat()
                MsoShapeType shapeType;
                shapePtr->get_Type(&shapeType);
                if(shapeType == msoEmbeddedOLEObject)
                {
                    ks_stdptr<OLEFormat> oleFormatPtr;
                    //oleFormatPtr->
                    shapePtr->get_OLEFormat(&oleFormatPtr);
                    IDispatch* obj = nullptr;
                    oleFormatPtr->get_Object(&obj);
                }
                else if(shapeType == msoLinkedOLEObject)
                {

                }
                else if(shapeType == msoPicture)
                {

                }
                else if(shapeType == msoMedia)
                {

                }
            }
        }
    }
    return false;
}

bool WppComment::extractPicture()
{
    if(!m_spPresentation)
    {
        return false;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return false;
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
                ks_stdptr<PictureFormat> picturePtr;
                shapePtr->get_PictureFormat(&picturePtr);
                if(!picturePtr)
                {
                    continue;
                }
            }
        }
    }
    return false;
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
