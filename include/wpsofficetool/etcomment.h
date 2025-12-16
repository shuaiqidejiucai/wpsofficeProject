#ifndef ETCOMMENT_H
#define ETCOMMENT_H

#include <QObject>
#include "etapi.h"
#include "kfc/comsptr.h"
#include "utilitytool.h"
#include "officegeneralabilities.h"

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

    QString getText(kfc::ks_stdptr<etapi::IRange> cellRane);

    QList<QStringList> getSheetText(long sheetIndex);

    long getSheetCount();

    void replaceTextForEtDoc(kfc::ks_stdptr<etapi::IRange> srcRange , const QString& replaceText);

    QList<QList<kfc::ks_stdptr<etapi::IRange>>> getTextRange(long sheetIndex);

    void extractPicture(long sheetIndex, GetNextOleDataFun imageFunPtr);

    static void extractPictureNomemery(const QString &qsETFilePath, const QString &imageOutDir);

    static void extractDataNomemery(const QString &qsETFilePath, const QString &imageOutDir);

    void getOleFileData(long sheetIndex, GetNextOleDataFun oleDataPtr);
private:
    void extractFile(long sheetIndex, EU_FileType fileType, GetNextOleDataFun fileFunPtr);

    void getPictureForShapes(kfc::ks_stdptr<etapi::IShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue);

    bool getPictureForShape( kfc::ks_stdptr<etapi::IShapes> shapesPtr, kfc::ks_stdptr<etapi::IShape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

    void getOldFileDataForShapes(kfc::ks_stdptr<etapi::IShapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    bool getOldFileDataForShape(kfc::ks_stdptr<etapi::IShapes> shapesPtr, kfc::ks_stdptr<etapi::IShape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

signals:

private:
    IKRpcClient *m_pEtRpcClient;
    kfc::ks_stdptr<etapi::_Application> m_spApplication;
    kfc::ks_stdptr<etapi::Workbooks> m_spWorkbooks;
    kfc::ks_stdptr<etapi::_Workbook> m_spWorkbook;
};

#endif // ETCOMMENT_H
