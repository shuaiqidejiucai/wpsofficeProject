#include "etcomment.h"
#include "officegeneralabilities.h"
#include "qmimedata.h"
#include "variant.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QBuffer>
#include "libolecf.h"
#include <QtEndian>
#include <QJsonDocument>
#include <QJsonObject>
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
        //qDebug() <<"get ETRpcClient erro";
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
#include <QLabel>
#include <QFile>
static QList<QByteArray> extractPNG(const QByteArray& data) {
    QList<QByteArray> images;

    QByteArray header = QByteArray::fromHex("89504E470D0A1A0A");
    QByteArray footer = QByteArray::fromHex("49454E44AE426082");

    int pos = 0;
    while (data.indexOf(header, pos) != -1)
    {
        pos = data.indexOf(header, pos);
        int end = data.indexOf(footer, pos + header.size());
        if (end != -1)
        {
            end += footer.size();
            QByteArray candidate = data.mid(pos, end - pos);
            QByteArray rmData = QByteArray::fromHex("3C00");
            int rmIndex = candidate.indexOf(rmData);
            // while (rmIndex != -1)
            // {
            //     candidate = candidate.remove(rmIndex, 4);
            //     rmIndex = candidate.indexOf(rmData);
            // }

            int ssss = footer.size();
            QByteArray ba = candidate.right(ssss);
            // 严格验证
            QFile file("/home/user/mjc/dps-ppt/bugwenjian/bin");
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(candidate);
                file.close();
            }
            images.append(candidate);
            qDebug() << "✓ PNG" << candidate.size() << "bytes";
            pos = end;  // 跳过整个文件
            continue;

        }
        pos += header.size();
    }

    return images;
}

enum oldImageType
{
    JPGType,
    PNGType
    //Other:BMP,GIF
};

static bool IsContainsImageHeardData(const QByteArray &srcData, int &srcDataPos, oldImageType &imageType)
{
    //实际数据开始
    QByteArray jpgHeaderData = QByteArray::fromHex("FFD8FF");
    int imagePos = 0;
    imagePos = srcData.indexOf(jpgHeaderData, imagePos);
    if(imagePos != -1)
    {
        srcDataPos = imagePos;
        imageType = JPGType;
        return true;
    }
    imagePos = 0;
    QByteArray pngHeaderData = QByteArray::fromHex("89504E470D0A1A0A");
    imagePos = srcData.indexOf(pngHeaderData, imagePos);
    if(imagePos != -1)
    {
        srcDataPos = imagePos;
        imageType = PNGType;
        return true;
    }
    //...other
    return false;
}

static bool IsContainsImageFooterData(const QByteArray &srcData, oldImageType imageType, int &srcEndDataPos)
{
    QByteArray footer;
    if(imageType == JPGType)
    {
        footer = QByteArray::fromHex("FFD9");
    }
    else if(imageType == PNGType)
    {
        footer = QByteArray::fromHex("49454E44AE426082");
    }
    else
    {
        return false;
    }
    int pos = srcData.indexOf(footer);
    if(pos != -1)
    {
        pos += footer.size();
        srcEndDataPos = pos;
        return true;
    }
    return false;
}

static QList<QByteArray> extractJPEG(const QByteArray& data)
{
    QList<QByteArray> images;

    // 多种 JPEG 开头
    QList<QByteArray> headers = {
        QByteArray::fromHex("FFD8FFE0"),  // JFIF
        QByteArray::fromHex("FFD8FFE1"),  // EXIF
        QByteArray::fromHex("FFD8FFE2"),  // ICC
        QByteArray::fromHex("FFD8FFDB"),  // 量化表
        QByteArray::fromHex("FFD8FFC0"),  // SOF0
    };

    QByteArray header = QByteArray::fromHex("FFD8FF");
    QByteArray footer = QByteArray::fromHex("FFD9");

    int pos = 0;
    while ((pos = data.indexOf(header, pos)) != -1)
    {
        int end = data.indexOf(footer, pos + header.size());
        if (end != -1) {
            end += footer.size();
            QByteArray candidate = data.mid(pos, end - pos);

            QFile file("/home/user/mjc/dps-ppt/bugwenjian/bin");
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(candidate);
                file.close();
            }
            images.append(candidate);
            qDebug() << "✓ JPEG" << candidate.size() << "bytes";
            pos = end;
            continue;
        }
        pos += header.size();
    }


    return images;
}

static QList<QByteArray> extractBMP(const QByteArray& data) {
    QList<QByteArray> images;

    int pos = 0;
    while (pos <= data.size() - 14) {
        if (data[pos] == 'B' && data[pos + 1] == 'M') {
            quint32 fileSize = qFromLittleEndian<quint32>(
                reinterpret_cast<const uchar*>(data.data() + pos + 2)
                );

            if (fileSize >= 14 && fileSize < 50 * 1024 * 1024 &&
                pos + fileSize <= data.size()) {

                QByteArray candidate = data.mid(pos, fileSize);

                images.append(candidate);
                qDebug() << "✓ BMP" << candidate.size() << "bytes";
                pos += fileSize;
                continue;

            }
        }
        pos++;
    }

    return images;
}

void EtComment::extractPictureNomemery(const QString& qsETFilePath, const QString& imageOutDir)
{
    QByteArray data;

    libolecf_file_t* file = nullptr;
    libolecf_item_t* item = nullptr;
    libolecf_error_t* error = nullptr;

    // 打开文件
    libolecf_file_initialize(&file, &error);
    libolecf_file_open(file, qsETFilePath.toUtf8().constData(),
                       LIBOLECF_OPEN_READ, &error);

    // 根据路径获取项
    // streamPath 格式如 "Workbook" 或 "Root Entry/Workbook"
    const QString streamPath = "Workbook";
    if (libolecf_file_get_item_by_utf8_path(
            file,
            (const uint8_t *)streamPath.toUtf8().constData(),
            streamPath.toUtf8().size(),
            &item,
            &error) == 1) {

        // 读取流数据
        uint32_t stream_size = 0;
        libolecf_item_get_size(item, &stream_size, nullptr);

        //限制图片大小不超过100M防止内存过大
        if (stream_size > 0 && stream_size < 100 * 1024 * 1024) {
            data.resize(stream_size);

            libolecf_stream_read_buffer(
                item,
                (uint8_t*)data.data(),
                stream_size,
                nullptr
                );
        }

        libolecf_item_free(&item, nullptr);
    }

    libolecf_file_close(file, nullptr);
    libolecf_file_free(&file, nullptr);

    auto readZ = [&](int& pos)->QByteArray {
        int start = pos;
        while(data[pos] == '\0')
        {
            ++pos;
        }
        while (pos < data.size() && data[pos] != '\0') ++pos;
        if (pos >= data.size()) return {};
        QByteArray s = data.mid(start, pos - start);
        ++pos;
        return s;
    };

    QByteArray allImageHeader = QByteArray::fromHex("8C00040056005600C1010800C101000035EA0200EB00");
    QByteArray footer = QByteArray::fromHex("FFD9");

    int pos = 0;
    pos = data.indexOf(allImageHeader, pos);
    pos = pos + allImageHeader.size();
    QByteArray dataSegment = data.mid(pos, 2);
    pos += 2;
    QByteArray dataSegmentSize = qToLittleEndian(dataSegment);
    quint16 value = qFromLittleEndian<quint16>(reinterpret_cast<const uchar*>(dataSegmentSize.constData()));
    //去标记数据
    QList<int> posIndexList;
    pos += value;
    while(pos  < data.size())
    {
        if(pos + 4 < data.size())
        {
            dataSegment = data.mid(pos, 4);
            if(dataSegment.left(2) == QByteArray::fromHex("3C00"))
            {
                posIndexList.append(pos);
                dataSegment = dataSegment.right(2);
                dataSegmentSize = qToLittleEndian(dataSegment);
                value = qFromLittleEndian<quint16>(reinterpret_cast<const uchar*>(dataSegmentSize.constData()));
                pos += value;
                pos += 4;
            }
            else
            {
                break;
            }
        }
        else
        {
            pos = data.size() - 1;
            break;
        }
    }

    for (int i = posIndexList.count() - 1; i >= 0 ; --i)
    {
        data.remove(posIndexList.at(i), 4);
    }

    QList<QByteArray> imageDatas;
    imageDatas.append(extractJPEG(data));
    imageDatas.append(extractPNG(data));
    imageDatas.append(extractBMP(data));

    // QFile file2("/home/user/mjc/dps-ppt/bugwenjian/bin");
    // if(file2.open(QIODevice::WriteOnly))
    // {
    //     file2.write(data);
    //     file2.close();
    // }



    //imageDatas.append( extractPNG(data));

    //imageDatas.append(extractBMP(data));

    for(QByteArray &ba : imageDatas)
    {
        QImage image = QImage::fromData(ba);
        if(!image.isNull())
        {
            QLabel * tmpLabel = new QLabel;
            tmpLabel->setPixmap(QPixmap::fromImage(image));
            tmpLabel->show();
        }
    }
    //return data;
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
            //qDebug()<<"Unknown Type";
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
        //qDebug()<<"Unknown Type";
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
        ST_VarantFile stOleFile;
        QStringList qsMimeDataKeyList = mdata->formats();
        QString qsMimeData;
        QByteArray tmpData = mdata->data("Kingsoft Shapes Tag");

        QJsonDocument jsonDoc = QJsonDocument::fromJson(tmpData);
        QJsonObject jsonObj = jsonDoc.object();
        QString qsType = jsonObj.value("objectsTag").toString();

        for(const QString& qsTmp : qsMimeDataKeyList)
        {
            if(qsTmp.contains(ETMimeDataKey) && qsTmp.contains("Format"))
            {
                qsMimeData = qsTmp;
                break;
            }
        }
        if(qsMimeData.isEmpty())
        {
            return result;
        }
        QByteArray data = mdata->data(qsMimeData);
        QByteArray srcData;
        if(UtilityTool::findOleDataFromZipMemory(data, srcData, qsType))
        {
            if(srcData.isEmpty())
            {
                return result;
            }
            if(srcData.at(0) == 0x02)
            {
                stOleFile.fileData = srcData;
            }
            else
            {
                srcData.remove(0,1);

                UtilityTool::GetOleFileData(srcData, stOleFile);
                if(qsType.contains("PowerPoint.Show.12") && stOleFile.qsFileName.isEmpty())
                {
                    stOleFile.qsFileName = "tmp.pptx";
                    if(stOleFile.fileData.isEmpty())
                    {
                        stOleFile.fileData = srcData;
                    }
                }
                if(qsType.contains("PowerPoint.Show.8") && stOleFile.fileData.isEmpty())
                {
                    stOleFile.qsFileName = "tmp.ppt";
                    stOleFile.fileData = srcData;
                }
                if(qsType.contains("Word.Document.8"))
                {
                    stOleFile.qsFileName = "tmp.doc";
                    stOleFile.fileData = srcData;
                }
                if(qsType.contains("Excel.Sheet.8"))
                {
                    stOleFile.qsFileName = "tmp.et";
                    stOleFile.fileData = srcData;
                }
                if(qsType.contains("Excel.Sheet.12"))
                {
                    stOleFile.qsFileName = "tmp.xlsx";
                    if(stOleFile.fileData.isEmpty())
                    {
                        stOleFile.fileData = srcData;
                    }
                }
                if(qsType.contains("Word.Document.12"))
                {
                    stOleFile.qsFileName = "tmp.docx";
                    if(stOleFile.fileData.isEmpty())
                    {
                        stOleFile.fileData = srcData;
                    }
                }
//                if(stOleFile.qsTmpFilePath == "WordDocument")
//                {
//                    stOleFile.fileData = srcData;
//                }
            }
            //UtilityTool::GetOleFileData(srcData, stOleFile);
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
                //qDebug()<<"No Operate Type";
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
