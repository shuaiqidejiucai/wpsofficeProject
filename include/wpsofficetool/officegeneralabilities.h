#ifndef OFFICEGENERALABILITIES_H
#define OFFICEGENERALABILITIES_H
#include <QString>
#include "wpsapi.h"
#include "kfc/comsptr.h"
#include "wpsapiex.h"
#include "utilitytool.h"
#include <QDebug>
#include <wppapi.h>
#include <common.h>
#include <mso_enum.h>
#include<QStack>
#include <QProcess>

enum EU_FileType
{
    AllFileType,
    ImageType,
    OleFileType
};
const QString MimeDataKey = "Kingsoft WPS 9.0 Format";
struct WppApiTypes {
    using Shape = wppapi::Shape;
    using MsoShapeType = wppapi::MsoShapeType;
    using MsoAutoShapeType = wppapi::MsoAutoShapeType;
    using MsoFillType = wppapi::MsoFillType;
    using FillFormat = wppapi::FillFormat;
    using ShapeRange = wppapi::ShapeRange;
    using GroupShapes = wppapi::GroupShapes;
    using GroupShapeCountType = int;
    static VARIANT adapt(VARIANT& v) { return v; }
};

struct WpsApiTypes {
    using Shape = wpsapi::Shape;
    using MsoShapeType = wpsapi::MsoShapeType;
    using MsoAutoShapeType = wpsapi::MsoAutoShapeType;
    using MsoFillType = wpsapi::MsoFillType;
    using FillFormat = wpsapi::FillFormat;
    using ShapeRange = wpsapi::ShapeRange;
    using GroupShapes = wpsapi::GroupShapes;
    using GroupShapeCountType = long;
    static VARIANT* adapt(VARIANT& v) { return &v; }
};
inline bool OFFICE_HRESULT(HRESULT hr, const QString& msg) {
    if (SUCCEEDED(hr)) {
        //qDebug() << msg + " SUCCEEDED";
        return true;
    } else {
        //qDebug() << msg + " FAILED";
        return false;
    }
}

inline QString GetBSTRText(BSTR str)
{
    QString qsStr;
    qsStr = QString::fromUtf16(str);
    return qsStr;
}


template<typename T>
inline QList<kfc::ks_stdptr<typename T::Shape> > GetShapeGroupList(kfc::ks_stdptr<typename T::Shape> shapePtr, EU_FileType fileterFileType = AllFileType)
{
    using namespace kfc;
    QStack<ks_stdptr<typename T::Shape> > shapePtrStack;
    shapePtrStack.push(shapePtr);
    QList<ks_stdptr<typename T::Shape> > shapeList;
    while (!shapePtrStack.empty())
    {
        ks_stdptr<typename T::Shape> popShapePtr = shapePtrStack.pop();
        if(popShapePtr)
        {
            typename T::MsoShapeType shapeType;
            popShapePtr->get_Type(&shapeType);
            typename T::MsoAutoShapeType autoShapeType;
            ks_stdptr<typename T::FillFormat> format;
            popShapePtr->get_Fill(&format);
            typename T::MsoFillType fileType;
            format->get_Type(&fileType);
            popShapePtr->get_AutoShapeType(&autoShapeType);

            if(shapeType == T::MsoShapeType::msoGroup)
            {
                ks_stdptr<typename T::ShapeRange> shapeRangePtr;
                ks_stdptr<typename T::GroupShapes> groupShapesPtr;
                shapePtr->get_GroupItems(&groupShapesPtr);
                if(groupShapesPtr)
                {
                    typename T::GroupShapeCountType groupShapeCount = 0;
                    groupShapesPtr->get_Count(&groupShapeCount);
                    for(int i = 1; i <= groupShapeCount; ++i)
                    {
                        VARIANT index;
                        VariantInit(&index);
                        V_VT(&index) = VT_I4;
                        V_I4(&index) = i;

                        ks_stdptr<typename T::Shape> groupShapePtr;
                        groupShapesPtr->Item(T::adapt(index), &groupShapePtr);
                        shapePtrStack.push(groupShapePtr);
                    }
                }
                continue;
            }

            bool isImage = fileterFileType == ImageType && (shapeType == T::MsoShapeType::msoPicture ||
                                        shapeType == T::MsoShapeType::msoLinkedPicture ||
                                        (shapeType == T::MsoShapeType::msoAutoShape && (fileType == T::MsoFillType::msoFillPicture || fileType == T::MsoFillType::msoFillPatterned)));
            bool isOleFile = fileterFileType == OleFileType && (shapeType == T::MsoShapeType::msoEmbeddedOLEObject || shapeType == T::MsoShapeType::msoLinkedOLEObject);
            bool isAllfileType = fileterFileType == AllFileType;
            if(isImage || isOleFile || isAllfileType)
            {
                shapeList.append(popShapePtr);
            }
            else
            {
                //qDebug()<<"shape type:"<<shapeType;
            }
        }
    }
    return shapeList;
}


inline bool isFileLocked(const QString& filePath)
{
    QProcess process;
    process.start("lsof", QStringList() << filePath);
    process.waitForFinished();

    return process.exitCode() == 0 && !process.readAllStandardOutput().isEmpty();
}

class OfficeGeneralAbilities
{
public:
    OfficeGeneralAbilities();

};

#endif // OFFICEGENERALABILITIES_H
