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
    AllFileType,
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

inline QString GetBSTRText(BSTR str)
{
    QString qsStr;
    qsStr = QString::fromUtf16(str);
    return qsStr;
}
class OfficeGeneralAbilities
{
public:
    OfficeGeneralAbilities();

};

#endif // OFFICEGENERALABILITIES_H
