#ifndef WPPCOMMENT_H
#define WPPCOMMENT_H

#include <QObject>
#include "wppapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"

class WppComment : public QObject
{
    Q_OBJECT
public:
    explicit WppComment(QObject *parent = nullptr);

    bool initWppApplication();

    bool initWPPRpcClient();

    bool openWPPDoc(const QString& fileName);

    bool closeWPPDoc();

    void closeApp();

    bool saveWPPDoc();

    QStringList GetWPPText();

    QList<kfc::ks_stdptr<wppapi::TextRange>> GetTextRange();

    void replaceTextForWPPDoc(const QString& findText, const QString& replaceText);

    bool insertTextForTextRange(kfc::ks_stdptr<wppapi::TextRange> range, int start, int length, const QString& qsTexts, bool after = true);

    bool insertAttachment();

    bool extractAttachments();

    bool extractPicture();
private:
    IKRpcClient * m_rpcClient;
    kfc::ks_stdptr<wppapi::_Application> m_spApplication;
    kfc::ks_stdptr<wppapi::Presentations> m_spDocs;
    kfc::ks_stdptr<wppapi::_Presentation> m_spPresentation;
    //kfc::ks_stdptr<wpsapiex::_ApplicationEx> m_spApplicationEx;
};

#endif // WPPCOMMENT_H
