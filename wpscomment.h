#ifndef WPSCOMMENT_H
#define WPSCOMMENT_H

#include <QObject>
#include "wpsapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"
#include "utilitytool.h"

class WpsComment : public QObject
{
    Q_OBJECT
public:
    explicit WpsComment(QObject *parent = nullptr);

    bool initWpsApplication();

    bool initWpsRpcClient();

    bool openWPSDoc(const QString& fileName);

    bool closeWPSDoc();

    void closeApp();

    bool saveWPSDoc();

    QStringList GetWPSText();

    QList<kfc::ks_stdptr<wpsapi::Range>> GetTextRange();

    void replaceTextForWPSDoc(const QString& findText, const QString& replaceText);

    bool insertTextForTextRange(kfc::ks_stdptr<wpsapi::Range> range, int start, int length, const QString& qsTexts, bool after = true);

    void extractPicture(GetNextImageDataFun imageFunPtr);

    //bool GetOleFileData(QList<ST_OleFile>& stOleList);

    void GetOleFileData(GetNextOleDataFun oleDataPtr);

private:
    IKRpcClient * m_rpcClient;
    kfc::ks_stdptr<wpsapi::_Application> m_spApplication;
    kfc::ks_stdptr<wpsapi::Documents> m_spDocs;
    kfc::ks_stdptr<wpsapi::_Document> m_spDocument;
};

#endif // WPSCOMMENT_H
