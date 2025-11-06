#ifndef WPSCOMMENT_H
#define WPSCOMMENT_H

#include <QObject>
#include "wpsapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"
#include "utilitytool.h"
#include "officegeneralabilities.h"

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

    void getOleFileData(GetNextOleDataFun oleDataPtr);

    bool delFile(long rangeStart, long rangeEnd);

    QList<kfc::ks_stdptr<wpsapi::Shape> > GetShapeGroupList(kfc::ks_stdptr<wpsapi::Shape> shapePtr, EU_FileType fileterFileType);
private:
    void extractFile(EU_FileType fileType, GetNextOleDataFun fileFunPtr);

    void getOldFileDataForShapes(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    void getOldFileDataForInlineShapes(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr,GetNextOleDataFun oldDataFunPtr,bool& isContinue);

    void getPictureForInlineShapes(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool& isContinue);

    void getPictureForShapes(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue);

    bool getOldFileDataForShape(kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    bool getOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr,GetNextOleDataFun oldDataFunPtr,bool& isContinue);

    bool getPictureForInlineShape(kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

    bool getPictureForShape( kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);


private:
    IKRpcClient * m_rpcClient;
    kfc::ks_stdptr<wpsapi::_Application> m_spApplication;
    kfc::ks_stdptr<wpsapi::Documents> m_spDocs;
    kfc::ks_stdptr<wpsapi::_Document> m_spDocument;
};

#endif // WPSCOMMENT_H
