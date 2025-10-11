#ifndef ETCOMMENT_H
#define ETCOMMENT_H

#include <QObject>
#include "etapi.h"
#include "kfc/comsptr.h"

class EtComment : public QObject
{
    Q_OBJECT
public:
    explicit EtComment(QObject *parent = nullptr);

    bool initEtApplication();

    bool initEtRpcClient();

    bool openEtDoc(const QString& fileName);

    bool closeEtDoc();

    void closeApp();

    bool saveEtDoc();

signals:

private:
    IKRpcClient *m_pEtRpcClient;
    kfc::ks_stdptr<etapi::_Application> m_spApplication;
    kfc::ks_stdptr<etapi::Workbooks> m_spWorkbooks;
    kfc::ks_stdptr<etapi::_Workbook> spWorkbook;
};

#endif // ETCOMMENT_H
