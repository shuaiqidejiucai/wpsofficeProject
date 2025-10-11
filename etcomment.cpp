#include "etcomment.h"
#include "officegeneralabilities.h"
#include "variant.h"
#include <QDebug>

using namespace etapi;
using namespace kfc;
#define ET_LCID ((long)2052)
EtComment::EtComment(QObject *parent)
    : QObject{parent},m_pEtRpcClient(NULL)
{}

bool EtComment::initEtApplication()
{
    if(m_pEtRpcClient && m_pEtRpcClient->getEtApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        VARIANT_BOOL visibleVar = VARIANT_FALSE;
        m_spApplication->put_Visible(ET_LCID, visibleVar);
        m_spApplication->get_Workbooks(&m_spWorkbooks);
        return true;
    }
    return false;
}

bool EtComment::initEtRpcClient()
{
    HRESULT hr = createEtRpcInstance(&m_pEtRpcClient);
    if (hr != S_OK)
    {
        qDebug() <<"get ETRpcClient erro";
        return false;
    }

    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/et"));
    m_pEtRpcClient->setProcessPath(StrWpsAddress);
    SysFreeString(StrWpsAddress);

    QVector<BSTR> vArgs;
    vArgs.resize(2);
    vArgs[0] =  SysAllocString(__X("-shield"));
    vArgs[1] =  SysAllocString(__X("-multiply"));
    m_pEtRpcClient->setProcessArgs(vArgs.size(), vArgs.data());
    for (int i = 0; i < vArgs.size(); i++)
    {
        SysFreeString(vArgs.at(i));
    }
    return true;
}

bool EtComment::openEtDoc(const QString &fileName)
{
    ks_bstr fileNameBstr (fileName.utf16());

    KComVariant vars[12];
    HRESULT hr = m_spWorkbooks->_Open(fileNameBstr, vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7]
                                      , vars[8], vars[9], vars[10], vars[11], ET_LCID, (Workbook**)&spWorkbook);

    if(OFFICE_HRESULT(hr, "openEt"))
    {
        m_spWorkbook->Activate(ET_LCID);
        return true;
    }
    return false;
}

bool EtComment::closeEtDoc()
{
    KComVariant varSaveOptions(wdDoNotSaveChanges);
    KComVariant varOriginalFormat(wdWordDocument);
    KComVariant varRouteDocument;
    m_spWorkbook->Close();
}
