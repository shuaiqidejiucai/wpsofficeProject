#ifndef OFFICEGENERALABILITIES_H
#define OFFICEGENERALABILITIES_H
#include <QString>
#include "wpsapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"
#include "utilitytool.h"

enum EU_FileType
{
    ImageType,
    OleFileType
};

class OfficeGeneralAbilities
{
public:
    OfficeGeneralAbilities();

    static void extractFile(kfc::ks_stdptr<wpsapi::_Document> documentPtr, EU_FileType fileType, GetNextOleDataFun imageFunPtr);
};

#endif // OFFICEGENERALABILITIES_H
