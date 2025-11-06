#include "etcomment.h"
#include "officegeneralabilities.h"
#include "qmimedata.h"
#include "variant.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QBuffer>

using namespace etapi;
using namespace kfc;
#define ET_LCID ((long)2052)
EtComment::EtComment(QObject *parent)
    : QObject{parent},m_pEtRpcClient(NULL)
{}

bool EtComment::initEtApplication()
{
    if(m_pEtRpcClient && m_pEtRpcClient->getEtApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        VARIANT_BOOL visibleVar = VARIANT_FALSE;
        m_spApplication->put_Visible(ET_LCID, visibleVar);
        m_spApplication->get_Workbooks(&m_spWorkbooks);
        return true;
    }
    return false;
}

bool EtComment::initEtRpcClient()
{
    HRESULT hr = createEtRpcInstance(&m_pEtRpcClient);
    if (hr != S_OK)
    {
        qDebug() <<"get ETRpcClient erro";
        return false;
    }

    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/et"));
    m_pEtRpcClient->setProcessPath(StrWpsAddress);
    SysFreeString(StrWpsAddress);

    QVector<BSTR> vArgs;
    vArgs.resize(2);
    vArgs[0] =  SysAllocString(__X("-shield"));
    vArgs[1] =  SysAllocString(__X("-multiply"));
    m_pEtRpcClient->setProcessArgs(vArgs.size(), vArgs.data());
    for (int i = 0; i < vArgs.size(); i++)
    {
        SysFreeString(vArgs.at(i));
    }
    return true;
}

bool EtComment::openEtDoc(const QString &fileName)
{
    ks_bstr fileNameBstr (fileName.utf16());

    KComVariant vars[12];
    HRESULT hr = m_spWorkbooks->_Open(fileNameBstr, vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7]
                                      , vars[8], vars[9], vars[10], vars[11], ET_LCID, (Workbook**)&m_spWorkbook);

    if(OFFICE_HRESULT(hr, "openEt"))
    {
        m_spWorkbook->Activate(ET_LCID);
        return true;
    }
    return false;
}

bool EtComment::closeEtDoc()
{
    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;
    HRESULT hr = m_spWorkbook->Close(missing, missing, missing, ET_LCID);
    return OFFICE_HRESULT(hr, "close workbook");
}

void EtComment::closeApp()
{
    if(m_spApplication)
    {
        m_spApplication->Quit();
        m_pEtRpcClient = nullptr;
        m_spWorkbooks->Close(ET_LCID);
        m_spWorkbooks.clear();
        m_spApplication.clear();
    }
}

bool EtComment::saveEtDoc()
{
    HRESULT hr = m_spWorkbook->Save(ET_LCID);
    return OFFICE_HRESULT(hr, "saveET");
}

QString EtComment::getText(kfc::ks_stdptr<etapi::IRange> cellRane)
{
    QString text;
    if(cellRane)
    {
        VARIANT bstrVar;
        VariantInit(&bstrVar);
        cellRane->get_Text(&bstrVar);
        BSTR textBstr = V_BSTR(&bstrVar);
        text = GetBSTRText(textBstr);
    }
    return text;
}

QList<QStringList> EtComment::getSheetText(long sheetIndex)
{
    QList< QList<kfc::ks_stdptr<IRange> > > tableRangeList = getTextRange(sheetIndex);
    QList<QStringList> tableTextList;
    for(int i = 0; i < tableRangeList.count(); ++i)
    {
        QList<kfc::ks_stdptr<IRange> > rowRangeList = tableRangeList.at(i);
        QStringList rowTextList;
        for(int j = 0; j < rowRangeList.count(); ++j)
        {
            QString qsText;
            ks_stdptr<IRange> cellRange = rowRangeList.at(j);
            if(cellRange)
            {
                qsText = getText(cellRange);
            }
            rowTextList.append(qsText);
        }
        tableTextList.append(rowTextList);
    }
    return tableTextList;
}

long EtComment::getSheetCount()
{
    if(m_spWorkbook)
    {
        ks_stdptr<Sheets> sheetsPtr;
        m_spWorkbook->get_Sheets(&sheetsPtr);
        if(!sheetsPtr)
        {
            return 0;
        }
        long sheetCount = 0;
        sheetsPtr->get_Count(&sheetCount);
        return sheetCount;
    }

    return 0;
}

void EtComment::replaceTextForEtDoc(kfc::ks_stdptr<etapi::IRange> srcRange, const QString &replaceText)
{
    ks_bstr replaceTextBSTR(replaceText.utf16());

    VARIANT bstrVar;
    VariantInit(&bstrVar);
    V_VT(&bstrVar) = VT_BSTR;
    V_BSTR(&bstrVar) = replaceTextBSTR;

    srcRange->put_Value2(ET_LCID, bstrVar);
}

void EtComment::extractPicture(long sheetIndex, GetNextOleDataFun imageFunPtr)
{
    if(!imageFunPtr)
    {
        return;
    }
    if(!m_spWorkbook)
    {
        return;
    }
    extractFile(sheetIndex, ImageType, imageFunPtr);
}

void EtComment::getOleFileData(long sheetIndex, GetNextOleDataFun oleDataPtr)
{
    if(!oleDataPtr)
    {
        return;
    }
    if(!m_spWorkbook)
    {
        return;
    }

    extractFile(sheetIndex, OleFileType, oleDataPtr);
}

void EtComment::extractFile(long sheetIndex,EU_FileType fileType, GetNextOleDataFun fileFunPtr)
{
    if(!fileFunPtr)
    {
        return;
    }
    if(!m_spWorkbook)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }

    bool isContinue = false;
    long sheetCount = getSheetCount();
    if(sheetIndex <= sheetCount && sheetCount > 0)
    {
        if(!m_spWorkbook)
        {
            return;
        }
        ks_stdptr<Sheets> sheetsPtr;
        m_spWorkbook->get_Sheets(&sheetsPtr);
        if(!sheetsPtr)
        {
            return;
        }

        VARIANT sheetIndexVar;
        VariantInit(&sheetIndexVar);
        V_VT(&sheetIndexVar) = VT_I8;
        V_I4(&sheetIndexVar) = sheetIndex;

        ks_stdptr<_Worksheet> spWorksheet;
        sheetsPtr->get_Item(sheetIndexVar, (IDispatch**)&spWorksheet);

        if(!spWorksheet)
        {
            return;
        }
        ks_stdptr<IShapes> spShapes;
        spWorksheet->get_Shapes((Shapes**)&spShapes);
        if(!spShapes)
        {
            return;
        }
        if(fileType == ImageType)
        {
            getPictureForShapes(spShapes, fileFunPtr, isContinue);
        }
        else if(fileType == OleFileType)
        {
            getOldFileDataForShapes(spShapes, fileFunPtr,isContinue);
        }
        else
        {
            qDebug()<<"Unknown Type";
        }
    }
}

void EtComment::getPictureForShapes(kfc::ks_stdptr<etapi::IShapes> shapesPtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    if(!shapesPtr)
    {
        return;
    }
    if(!m_spApplication || !imageDataFunPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<IShape>> shapeList;

    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<IShape> spShape;
        shapesPtr->Item(shapeIndex, (Shape**)&spShape);
        if(!spShape)
        {
            continue;
        }
        shapeList.append(spShape);
    }

    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<IShape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isImage = type == msoPicture || type == msoLinkedPicture;
        if(isImage)
        {
            getPictureForShape(shapesPtr, spShape, imageDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }

        }
    }
}

bool EtComment::getPictureForShape(kfc::ks_stdptr<etapi::IShapes> shapesPtr, kfc::ks_stdptr<etapi::IShape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
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


    shapePtr->CopyPicture(missing,missing);

    QImage image = qApp->clipboard()->image();

    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "png");

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


            ks_stdptr<IShape> outShape;
            ks_bstr filePathBstr (outFileInfo.qsFilePath.utf16());
            HRESULT AddPictureHr = shapesPtr->AddPicture(filePathBstr, msoFalse, msoFalse
                                                         ,left, top, width, height
                                                         ,(Shape**)&outShape);
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

void EtComment::getOldFileDataForShapes(kfc::ks_stdptr<IShapes> shapesPtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    if(!shapesPtr || !oldDataFunPtr)
    {
        return;
    }
    if(!m_spApplication)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);

    QList<ks_stdptr<IShape>> shapeList;
    for(int i = 1; i <= shapeCount; ++i)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = i;

        ks_stdptr<IShape> spShape;
        shapesPtr->Item(shapeIndex, (Shape**)&spShape);
        if(spShape)
        {
            shapeList.append(spShape);
        }
    }


    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<IShape> spShape = shapeList.at(i);
        MsoShapeType type;
        spShape->get_Type(&type);

        bool isOleFile = type == msoEmbeddedOLEObject || type == msoLinkedOLEObject;
        if(isOleFile)
        {
            getOldFileDataForShape(shapesPtr, spShape, oldDataFunPtr, isContinue);
            if(!isContinue)
            {
                return;
            }
        }
    }
}

bool EtComment::getOldFileDataForShape(kfc::ks_stdptr<etapi::IShapes> shapesPtr, kfc::ks_stdptr<etapi::IShape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
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
    shapePtr->Copy();
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
                    HRESULT addOleHr = shapesPtr->AddOLEObject(missing,fileNameVar,missing
                                                               ,missing,missing,missing,missing
                                                               ,leftVar,topVar,widthVar,heightVar
                                                               ,&outShapePtr);
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

QList< QList<kfc::ks_stdptr<IRange> > > EtComment::getTextRange(long sheetIndex)
{
    QList<QList<ks_stdptr<IRange> > > tableList;
    ks_stdptr<Sheets> sheetsPtr;

    if(!m_spWorkbook)
    {
        return tableList;
    }
    long sheetCount = getSheetCount();
    if(sheetIndex <= sheetCount && sheetCount > 0)
    {
        VARIANT sheetIndexVar;
        VariantInit(&sheetIndexVar);
        V_VT(&sheetIndexVar) = VT_I8;
        V_I4(&sheetIndexVar) = sheetIndex;

        ks_stdptr<_Worksheet> spWorksheet;
        sheetsPtr->get_Item(sheetIndexVar, (IDispatch**)&spWorksheet);

        if(spWorksheet)
        {
            ks_stdptr<IRange> spCellsRange;
            spWorksheet->get_Cells((Range**)&spCellsRange);
            if(spCellsRange)
            {
                long rowConut = 0;
                spCellsRange->get_Row(&rowConut);
                long columnConut = 0;
                spCellsRange->get_Column(&columnConut);
                for(long r = 1; r <= rowConut; ++r)
                {
                    QList<ks_stdptr<IRange>> rowRangeList;
                    for(long c = 1; c <= columnConut; ++c)
                    {
                        VARIANT rowVar;
                        VariantInit(&rowVar);
                        V_VT(&rowVar) = VT_UI4;
                        V_I4(&rowVar) = 0;

                        VARIANT columnVar;
                        VariantInit(&columnVar);
                        V_VT(&columnVar) = VT_UI4;
                        V_I4(&columnVar) = 0;

                        VARIANT rangeVar;
                        VariantInit(&rangeVar);
                        spCellsRange->get_Item(rowVar, columnVar, ET_LCID, &rangeVar);

                        if (V_VT(&rangeVar) == VT_DISPATCH && V_DISPATCH(&rangeVar))
                        {
                            ks_stdptr<IRange> cellRange((IRange*)V_DISPATCH(&rangeVar));
                            //IDispatch
                            //V_DISPATCH(&rangeVar)->QueryInterface(__uuidof(IRange), (void**)&cellRange);
                            rowRangeList.append(cellRange);
                        }
                    }
                    tableList.append(rowRangeList);
                }
            }
        }
    }
    return tableList;
}
