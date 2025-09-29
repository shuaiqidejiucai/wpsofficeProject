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

    void extractPicture(GetNextOleDataFun imageFunPtr);

    //bool GetOleFileData(QList<ST_OleFile>& stOleList);

    void GetOleFileData(GetNextOleDataFun oleDataPtr);

    bool delFile(long rangeStart, long rangeEnd);

    //bool insertFile(long rangeStart, long rangeEnd, const QString& qsFileName);

    //bool insertFile(long rangeStart, long rangeEnd, const QString& qsFileName);

private:
    void GetOldFileDataForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    void GetOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr,GetNextOleDataFun oldDataFunPtr,bool& isContinue);

    void GetPictureForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

    void GetPictureForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

private:
    IKRpcClient * m_rpcClient;
    kfc::ks_stdptr<wpsapi::_Application> m_spApplication;
    kfc::ks_stdptr<wpsapi::Documents> m_spDocs;
    kfc::ks_stdptr<wpsapi::_Document> m_spDocument;
};

#endif // WPSCOMMENT_H
