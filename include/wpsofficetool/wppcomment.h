#ifndef WPPCOMMENT_H
#define WPPCOMMENT_H

#include <QObject>
#include "wppapi.h"
#include "kfc/comsptr.h"
#include "officegeneralabilities.h"


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

    QList<kfc::ks_stdptr<wppapi::TextRange>> GetTextRangeForShapes(kfc::ks_stdptr<wppapi::Shapes> shapesPtr);

    long getPageCount();

    QList<kfc::ks_stdptr<wppapi::TextRange>> getRearks(long pageIndex);

    QList<kfc::ks_stdptr<wppapi::TextRange>> getMaster();

    QStringList getPPLayout(long pageIndex);

    QStringList getPPLayoutText();

    QStringList getMasterText();

    QStringList getRearksText();

    void replaceTextForWPPDoc(const QString& findText, const QString& replaceText);

    bool insertTextForTextRange(kfc::ks_stdptr<wppapi::TextRange> range, int start, int length, const QString& qsTexts, bool after = true);

    void getOleFileData(GetNextOleDataFun oleDataPtr);

    void extractPicture(GetNextOleDataFun imageFunPtr);

    void extractPictureNomemery(const QString& qsImageDir);
private:
    void extractFile(EU_FileType fileType, GetNextOleDataFun fileFunPtr);

    bool getPictureForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

    bool getOldFileDataForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);
private:
    QStringList getTableTextList(kfc::ks_stdptr<wppapi::Shape> tableShape);
private:
    IKRpcClient * m_rpcClient;
    kfc::ks_stdptr<wppapi::_Application> m_spApplication;
    kfc::ks_stdptr<wppapi::Presentations> m_spDocs;
    kfc::ks_stdptr<wppapi::_Presentation> m_spPresentation;
    QWidget * m_containWidget;
    //kfc::ks_stdptr<wpsapiex::_ApplicationEx> m_spApplicationEx;
    QStringList TextRnageToQString(QList<kfc::ks_stdptr<wppapi::TextRange>> textRangePtrList);
};

#endif // WPPCOMMENT_H
