#include "officegeneralabilities.h"
#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <QDebug>
#include <QBuffer>

using namespace wpsapi;
using namespace kfc;
using namespace wpsapiex;
OfficeGeneralAbilities::OfficeGeneralAbilities() {}

void OfficeGeneralAbilities::extractFile(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::_Document> documentPtr, EU_FileType fileType, GetNextOleDataFun fileFunPtr)
{
    if(!fileFunPtr)
    {
        return;
    }
    if(!documentPtr)
    {
        return;
    }

    bool isContine = true;
    if(fileType == ImageType)
    {
        ks_stdptr<Shapes> shapesPtr;
        documentPtr->get_Shapes(&shapesPtr);
        if(shapesPtr)
        {
            GetPictureForShapes(spApplication, shapesPtr, fileFunPtr,isContine);
        }
        if(isContine)
        {
            ks_stdptr<InlineShapes> inlineShapesPtr;
            documentPtr->get_InlineShapes(&inlineShapesPtr);
            if(inlineShapesPtr)
            {
                GetPictureForInlineShapes(spApplication, inlineShapesPtr, fileFunPtr,isContine);
            }
        }
    }
    else if(fileType == OleFileType)
    {
        ks_stdptr<InlineShapes> inlineShapesPtr;
        documentPtr->get_InlineShapes(&inlineShapesPtr);
        if(inlineShapesPtr)
        {
            GetOldFileDataForInlineShapes(spApplication, inlineShapesPtr, fileFunPtr,isContine);
        }

        if(isContine)
        {
            ks_stdptr<Shapes> shapesPtr;
            documentPtr->get_Shapes(&shapesPtr);
            if(shapesPtr)
            {
                GetOldFileDataForShapes(spApplication, shapesPtr, fileFunPtr,isContine);
            }
        }
    }
    else
    {
        qDebug()<<"Unknown file Type";
    }
}

void OfficeGeneralAbilities::GetOldFileDataForShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!shapesPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<Shape>> shapeList;
    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<Shape> spShape;
        shapesPtr->Item(&shapeIndex, &spShape);
        if(spShape)
        {
            shapeList.append(spShape);
        }
    }


    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isOleFile = type == msoEmbeddedOLEObject || type == msoLinkedOLEObject;
        if(isOleFile)
        {
            GetPictureForShape(spApplication, shapesPtr, spShape, oldDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void OfficeGeneralAbilities::GetOldFileDataForInlineShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!shapesPtr)
    {
        return;
    }
    long count = 0;
    shapesPtr->get_Count(&count);

    QList<ks_stdptr<InlineShape>> inlineShapeList;

    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        shapesPtr->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        inlineShapeList.append(shapePtr);
    }

    for(int i = 0; i < inlineShapeList.count(); ++i)
    {
        ks_stdptr<InlineShape> shapePtr = inlineShapeList.at(i);
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);

        bool isOleFile = inlineShapeType == wdInlineShapeEmbeddedOLEObject || inlineShapeType == wdInlineShapeLinkedOLEObject;

        if(isOleFile)
        {
            GetOldFileDataForInlineShape(spApplication, shapesPtr, shapePtr, oldDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void OfficeGeneralAbilities::GetPictureForInlineShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{    if(!shapesPtr)
    {
        return;
    }
    QList<ks_stdptr<InlineShape>> inlineShapeList;
    long count = 0;
    shapesPtr->get_Count(&count);
    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        shapesPtr->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        inlineShapeList.append(shapePtr);
    }

    for(int i = 0; i < inlineShapeList.count(); ++i)
    {
        ks_stdptr<InlineShape> shapePtr = inlineShapeList.at(i);
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);

        bool isImage = inlineShapeType == wdInlineShapeLinkedPicture || inlineShapeType == wdInlineShapePicture;
        if(isImage)
        {
            GetPictureForInlineShape(spApplication, shapesPtr, inlineShapeType, imageDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

void OfficeGeneralAbilities::GetPictureForShapes(kfc::ks_stdptr<wpsapi::_Application> spApplication, kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    if(!shapesPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<Shape>> shapeList;

    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<Shape> spShape;
        shapesPtr->Item(&shapeIndex, &spShape);
        if(!spShape)
        {
            continue;
        }
        shapeList.append(spShape);
    }

    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isImage = type == msoPicture || type == msoLinkedPicture;
        if(isImage)
        {
            GetPictureForShape(spApplication, shapesPtr, spShape, imageDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }

        }
    }
}

bool OfficeGeneralAbilities::GetOldFileDataForShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{

    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

    shapePtr->Select(&missing);
    ks_stdptr<Selection> spSelection;
    spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    ks_stdptr<Range> selectionRange;
    spSelection->get_Range(&selectionRange);
    spSelection->Copy();
    const QMimeData *  mdata = qApp->clipboard()->mimeData();
    if(mdata)
    {
        QByteArray data = mdata->data(MimeDataKey);
        QByteArray srcData;
        if(UtilityTool::findOleDataFromZipMemory(data, srcData))
        {
            if(srcData.isEmpty())
            {
                return result;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                //日至
            }
            else if(operaTye == ReplaceType)
            {
                float left = 0.0f;
                float top = 0.0f;
                float width = 0.0f;
                float height = 0.0f;
                shapePtr->get_Left(&left);
                shapePtr->get_Top(&top);
                shapePtr->get_Width(&width);
                shapePtr->get_Height(&height);
                ks_stdptr<Range> anchorRangePtr;
                shapePtr->get_Anchor(&anchorRangePtr);

                VARIANT unitTypeVar;
                V_VT(&unitTypeVar) = VT_I4;
                V_I4(&unitTypeVar) = wdCollapseStart;
                anchorRangePtr->Collapse(&unitTypeVar);

                VARIANT anchorVar;
                V_VT(&anchorVar) = VT_DISPATCH;
                V_DISPATCH(&anchorVar) = anchorRangePtr;

                HRESULT hr = shapePtr->Delete();
                result = OFFICE_HRESULT(hr, "delete");
                if(result)
                {
                    VARIANT leftVar;
                    VariantInit(&leftVar);
                    V_R4(&leftVar) = left;

                    VARIANT topVar;
                    VariantInit(&topVar);
                    V_R4(&topVar) = top;

                    VARIANT widthVar;
                    VariantInit(&widthVar);
                    V_R4(&widthVar) = width;

                    VARIANT heightVar;
                    VariantInit(&heightVar);
                    V_R4(&heightVar) = height;


                    ks_bstr fileNameStr(outFileInfo.qsFilePath.utf16());
                    VARIANT fileNameVar;
                    V_VT(&fileNameVar) = VT_BSTR;
                    V_BSTR(&fileNameVar) = fileNameStr;
                    ks_stdptr<Shape> outShapePtr;
                    HRESULT addOleHr = shapesPtr->AddOLEObject(&missing,&fileNameVar,&missing
                                            ,&missing,&missing,&missing,&missing
                                            ,&leftVar,&topVar,&widthVar,&heightVar
                                            ,&anchorVar, &outShapePtr);
                    result = OFFICE_HRESULT(addOleHr, "AddOLEObject");
                    //日至
                }
            }
            else
            {
                qDebug()<<"No Operate Type";
            }
        }
    }
    return result;
}

bool OfficeGeneralAbilities::GetOldFileDataForInlineShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    spSelection->Copy();
    const QMimeData *  mdata = qApp->clipboard()->mimeData();
    if(mdata)
    {
        QByteArray data = mdata->data(MimeDataKey);
        QByteArray srcData;
        if(UtilityTool::findOleDataFromZipMemory(data, srcData))
        {
            if(srcData.isEmpty())
            {
                return result;
            }
            ST_VarantFile stOleFile;
            UtilityTool::GetOleFileData(srcData, stOleFile);
            EU_OperateType operaTye;
            ST_VarantFile outFileInfo;
            isContinue = oldDataFunPtr(stOleFile, outFileInfo, operaTye);
            if(operaTye == DeleteType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                //日至
            }
            else if(operaTye == ReplaceType)
            {
                HRESULT delHr = shapePtr->Delete();
                result = OFFICE_HRESULT(delHr, "delete");
                if(result)
                {
                    ks_stdptr<Range> oleFileRange;
                    shapePtr->get_Range(&oleFileRange);
                    if(oleFileRange)
                    {
                        result = true;
                        ks_bstr fileNameStr(outFileInfo.qsFilePath.utf16());
                        VARIANT fileNameVar;
                        V_VT(&fileNameVar) = VT_BSTR;
                        V_BSTR(&fileNameVar) = fileNameStr;

                        VARIANT missing;
                        VariantInit(&missing);
                        V_VT(&missing) = VT_ERROR;
                        V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;


                        VARIANT unitTypeVar;
                        V_VT(&unitTypeVar) = VT_I4;
                        V_I4(&unitTypeVar) = wdCollapseStart;

                        oleFileRange->Collapse(&unitTypeVar);

                        VARIANT rangeVar;
                        V_VT(&rangeVar) = VT_DISPATCH;
                        V_DISPATCH(&rangeVar) = oleFileRange;

                        ks_stdptr<InlineShape> inlineShapePtr;
                        HRESULT addOLEHr = shapesPtr->AddOLEObject(&missing, &fileNameVar, &missing
                                                ,&missing,&missing,&missing
                                                ,&missing, &rangeVar, &inlineShapePtr);
                        result = OFFICE_HRESULT(addOLEHr, "Add OLEObject");
                    }
                }
            }
            else
            {
                qDebug()<<"No opreate type";
            }
        }
    }
    return result;
}

bool OfficeGeneralAbilities::GetPictureForInlineShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::InlineShapes> shapesPtr, kfc::ks_stdptr<wpsapi::InlineShape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result  = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    shapePtr->Select();
    ks_stdptr<Selection> spSelection;
    spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    spSelection->CopyAsPicture();
    QImage image = qApp->clipboard()->image();

    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "JPG");

    ST_VarantFile varantFile;
    varantFile.fileData = buf.data();

    EU_OperateType euOperateType;
    ST_VarantFile outFileInfo;
    isContinue = imageDataFunPtr(varantFile,outFileInfo ,euOperateType);
    if(euOperateType == DeleteType)
    {
        HRESULT delHr = shapePtr->Delete();
        result = OFFICE_HRESULT(delHr, "delete picture");
        //日至
    }
    else if(euOperateType == ReplaceType)
    {
        ks_stdptr<Range> inlineShapeRange;
        shapePtr->get_Range(&inlineShapeRange);
        if(!inlineShapeRange)
        {
            return result;
        }
        VARIANT unitTypeVar;
        V_VT(&unitTypeVar) = VT_I4;
        V_I4(&unitTypeVar) = wdCollapseStart;

        inlineShapeRange->Collapse(&unitTypeVar);
        HRESULT delHr = shapePtr->Delete();

        VARIANT anchorVar;
        V_VT(&anchorVar) = VT_DISPATCH;
        V_DISPATCH(&anchorVar) = inlineShapeRange;
        result = OFFICE_HRESULT(delHr, "delete picture");
        if(result)
        {
            VARIANT missing;
            VariantInit(&missing);
            V_VT(&missing) = VT_ERROR;
            V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

            ks_bstr fileNameBstr(outFileInfo.qsFilePath.utf16());
            ks_stdptr<InlineShape> outShape;
            HRESULT AddHr = shapesPtr->AddPicture(fileNameBstr, &missing, &missing, &anchorVar, &outShape);
            result = OFFICE_HRESULT(AddHr, "Add picture");
        }
    }
    else
    {
        qDebug()<< "No opreate type";
    }
    return result;
}

bool OfficeGeneralAbilities::GetPictureForShape(kfc::ks_stdptr<wpsapi::_Application> spApplication,kfc::ks_stdptr<wpsapi::Shapes> shapesPtr, kfc::ks_stdptr<wpsapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;
    shapePtr->Select(&missing);
    ks_stdptr<Selection> spSelection;
    spApplication->get_Selection(&spSelection);
    if(!spSelection)
    {
        return result;
    }
    ks_stdptr<Range> selectionRange;
    spSelection->get_Range(&selectionRange);
    spSelection->CopyAsPicture();
    QImage image = qApp->clipboard()->image();

    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "JPG");

    ST_VarantFile varantFile;
    varantFile.fileData = buf.data();

    EU_OperateType euOperateType;
    ST_VarantFile outFileInfo;
    isContinue = imageDataFunPtr(varantFile,outFileInfo ,euOperateType);
    if(euOperateType == DeleteType)
    {
        HRESULT hr = shapePtr->Delete();
        result = OFFICE_HRESULT(hr, "delete");
        //日至
    }
    else if(euOperateType == ReplaceType)
    {
        float left = 0.0f;
        float top = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        shapePtr->get_Left(&left);
        shapePtr->get_Top(&top);
        shapePtr->get_Width(&width);
        shapePtr->get_Height(&height);
        ks_stdptr<Range> anchorRangePtr;
        shapePtr->get_Anchor(&anchorRangePtr);

        VARIANT unitTypeVar;
        V_VT(&unitTypeVar) = VT_I4;
        V_I4(&unitTypeVar) = wdCollapseStart;

        anchorRangePtr->Collapse(&unitTypeVar);

        VARIANT anchorVar;
        V_VT(&anchorVar) = VT_DISPATCH;
        V_DISPATCH(&anchorVar) = anchorRangePtr;
        HRESULT hr = shapePtr->Delete();
        result = OFFICE_HRESULT(hr,"delete");
        if(result)
        {
            VARIANT leftVar;
            VariantInit(&leftVar);
            V_R4(&leftVar) = left;

            VARIANT topVar;
            VariantInit(&topVar);
            V_R4(&topVar) = top;

            VARIANT widthVar;
            VariantInit(&widthVar);
            V_R4(&widthVar) = width;

            VARIANT heightVar;
            VariantInit(&heightVar);
            V_R4(&heightVar) = height;


            ks_stdptr<Shape> outShape;
            ks_bstr filePathBstr (outFileInfo.qsFilePath.utf16());
            HRESULT AddPictureHr = shapesPtr->AddPicture(filePathBstr, &missing, &missing
                                  ,&leftVar, &topVar, &widthVar,&heightVar
                                  ,&anchorVar,&outShape);
            result = OFFICE_HRESULT(AddPictureHr, QString("AddPicture"));
            //日至
        }
    }
    else
    {
        qDebug()<<"Unknown Type";
    }
    return result;
}


