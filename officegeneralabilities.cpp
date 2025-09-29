#include "officegeneralabilities.h"

OfficeGeneralAbilities::OfficeGeneralAbilities() {}

void OfficeGeneralAbilities::extractFile(kfc::ks_stdptr<wpsapi::_Document> documentPtr, EU_FileType fileType, GetNextOleDataFun imageFunPtr)
{
    if(!documentPtr)
    {
        return;
    }
}
