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
    OfficeGeneralAbilities::extractFile(m_spApplication, m_spDocument, ImageType, imageFunPtr);
}

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

    OfficeGeneralAbilities::extractFile(m_spApplication, m_spDocument, OleFileType, oleDataPtr);
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
