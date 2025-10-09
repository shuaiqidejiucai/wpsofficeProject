#ifndef OFFICEGENERALABILITIES_H
#define OFFICEGENERALABILITIES_H
#include <QString>
#include "wpsapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"
#include "utilitytool.h"
#include <QDebug>
enum EU_FileType
{
    ImageType,
    OleFileType
};
const QString MimeDataKey = "Kingsoft WPS 9.0 Format";

inline bool OFFICE_HRESULT(HRESULT hr, const QString& msg) {
    if (SUCCEEDED(hr)) {
        qDebug() << msg + " SUCCEEDED";
        return true;
    } else {
        qDebug() << msg + " FAILED";
        return false;
    }
}

class OfficeGeneralAbilities
{
public:
    OfficeGeneralAbilities();

    static void extractFile(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::_Document> documentPtr, EU_FileType fileType, GetNextOleDataFun fileFunPtr);

    static void GetOldFileDataForShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    static void GetOldFileDataForInlineShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr,GetNextOleDataFun oldDataFunPtr,bool& isContinue);

    static void GetPictureForInlineShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool& isContinue);

    static void GetPictureForShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue);
private:
    static bool GetOldFileDataForShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool& isContinue);

    static bool GetOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr,GetNextOleDataFun oldDataFunPtr,bool& isContinue);

    static bool GetPictureForInlineShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

    static bool GetPictureForShape(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr,bool& isContinue);

};

#endif // OFFICEGENERALABILITIES_H
