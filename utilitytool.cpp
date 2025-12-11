#include "utilitytool.h"
#include <libolecf.h>
#include <libbfio_handle.h>
#include <libolecf/libolecf_file.h>
#include <libbfio_memory_range.h>
#include <QDebug>
#include <QBuffer>
#include <quazipfile.h>
#include <QTextCodec>
#include <QDomDocument>
static QByteArray readItemData(libolecf_item_t* item)
{
    uint32_t size = 0;
    if (libolecf_item_get_size(item, &size, NULL) != 1 || size == 0)
        return {};

    QByteArray buf;
    buf.resize((int)size);

    ssize_t read_count = libolecf_stream_read_buffer(
                item,
                reinterpret_cast<uint8_t*>(buf.data()),
                (size_t)size,
                NULL
                );

    if (read_count < 0) return {};
    buf.resize((int)read_count);
    return buf;
}

static bool parseOle10Native(const QByteArray& src, ST_VarantFile & stOleFile)
{
    if (src.size() < 12) return false;

    auto rdLE32 = [&](int off)->quint32 {
        if (off + 4 > src.size()) return 0;
        const uchar* p = reinterpret_cast<const uchar*>(src.constData() + off);
        return (quint32)p[0] | ((quint32)p[1] << 8) | ((quint32)p[2] << 16) | ((quint32)p[3] << 24);
    };

    auto rdLE16 = [&](int off)->quint32 {
        if (off + 2 > src.size()) return 0;
        const uchar* p = reinterpret_cast<const uchar*>(src.constData() + off);
        return (quint32)p[0] | ((quint32)p[1] << 8);
    };

    quint32 totalSize = rdLE32(0);
    int off = 4;

    auto readZ = [&](int& pos)->QByteArray {
        int start = pos;
        while(src[pos] == '\0')
        {
            ++pos;
        }
        while (pos < src.size() && src[pos] != '\0') ++pos;
        if (pos >= src.size()) return {};
        QByteArray s = src.mid(start, pos - start);
        ++pos;
        return s;
    };

    bool ok = false;
    for (int tryOff : {4}) {
        if (tryOff >= src.size()) break;
        int p = tryOff;


        quint32 filePrexLength = rdLE16(p);

        QTextCodec * code = QTextCodec::codecForName("GBK");

        QByteArray label = readZ(p);
        if (label.isEmpty()) continue;

        QByteArray file = readZ(p);
        if (file.isEmpty()) continue;
        stOleFile.qsFileName = code->toUnicode(file);

        QByteArray cmd = readZ(p);
        if (cmd.isEmpty()) continue;
        stOleFile.qsFilePath = code->toUnicode(cmd);

        quint32 mark = rdLE32(p);
        p = p + 4;
        quint32 tempPathLen = rdLE32(p);
        p = p + 4;
        QByteArray tempPathBa = readZ(p);
        if (tempPathBa.isEmpty()) continue;
        stOleFile.qsTmpFilePath = code->toUnicode(tempPathBa);

        quint32 datasize = rdLE32(p);
        p = p + 4;

        QByteArray srcData = src.mid(p, datasize);
        stOleFile.fileData = srcData;
        ok = true;
    }

    return ok;
}


static void getOle10NativeData(libolecf_item_t* root_item, QList<ST_VarantFile>& stOleFileList)
{
    int number_of_sub_items = 0;
    libolecf_item_get_number_of_sub_items(root_item, &number_of_sub_items, NULL);
    for (int i = 0; i < number_of_sub_items; ++i)
    {
        libolecf_item_t* sub_item = NULL;
        libolecf_item_get_sub_item(root_item, i, &sub_item, NULL);
        int childItemCount = 0;
        libolecf_item_get_number_of_sub_items(sub_item, &childItemCount, NULL);
        size_t name_size = 0;
        libolecf_item_get_utf8_name_size(sub_item, &name_size, NULL);
        char* name = new char[name_size];
        libolecf_item_get_utf8_name(sub_item, (uint8_t*)name, name_size, NULL);
        QString itemName(name);
        delete[] name;
        if (itemName.endsWith("Ole10Native"))
        {
            QByteArray ole10 = readItemData(sub_item);
            ST_VarantFile tmpSTOleFile;
            if (parseOle10Native(ole10, tmpSTOleFile))
            {
                stOleFileList.append(tmpSTOleFile);

                libolecf_item_free(&sub_item, nullptr);
                continue;
            }
        }
        if (childItemCount > 0 /*&& outData.isEmpty()*/)
        {
            getOle10NativeData(sub_item, stOleFileList);
        }
        libolecf_item_free(&sub_item, nullptr);
    }
    return;
}

static void getOle10NativeData(libolecf_item_t* root_item ,ST_VarantFile & stOleFile)
{
    int number_of_sub_items = 0;
    libolecf_item_get_number_of_sub_items(root_item, &number_of_sub_items, NULL);
    for (int i = 0; i < number_of_sub_items; ++i)
    {
        libolecf_item_t* sub_item = NULL;
        libolecf_item_get_sub_item(root_item, i, &sub_item, NULL);
        int childItemCount = 0;
        libolecf_item_get_number_of_sub_items(sub_item, &childItemCount, NULL);
        size_t name_size = 0;
        libolecf_item_get_utf8_name_size(sub_item, &name_size, NULL);
        char* name = new char[name_size];
        libolecf_item_get_utf8_name(sub_item, (uint8_t*)name, name_size, NULL);
        QString itemName(name);
        delete[] name;

        if (itemName.endsWith("Ole10Native"))
        {
            QByteArray ole10 = readItemData(sub_item);
            if (parseOle10Native(ole10, stOleFile))
            {
                //libolecf_item_free(&sub_item, nullptr);
            }
        }
        else if (itemName.endsWith("Package", Qt::CaseInsensitive))
        {
            stOleFile.fileData = readItemData(sub_item);
            //libolecf_item_free(&sub_item, nullptr);
        }
//        else if (itemName.contains("WordDocument", Qt::CaseInsensitive))
//        {
//            stOleFile.qsTmpFilePath = "WordDocument";
//            stOleFile.qsFileName = "tmp.doc";
//            //stOleFile.fileData = readItemData(sub_item);
//            //libolecf_item_free(&sub_item, nullptr);
//        }

        if (childItemCount > 0 /*&& outData.isEmpty()*/)
        {
            getOle10NativeData(sub_item, stOleFile);
        }
        libolecf_item_free(&sub_item, nullptr);
    }
    return;
}

bool UtilityTool::GetOleFileData(const QByteArray &srcData, ST_VarantFile &stOleFile)
{
    bool successful = false;
    QByteArray buffer = srcData;
    libbfio_handle_t* bfio_handle = nullptr;
    libcerror_error_t* bfio_error = nullptr;

    libcerror_error_t* rangeBfio_error = nullptr;
    if (libbfio_memory_range_initialize(&bfio_handle, &rangeBfio_error) != 1)
    {
        return successful;
    }

    // 2. 设置内存数据
    if (libbfio_memory_range_set(
                bfio_handle,
                reinterpret_cast<uint8_t*>(buffer.data()),
                buffer.size(),
                &bfio_error) != 1) {
        libbfio_handle_free(&bfio_handle, nullptr);
        return successful;
    }

    // 初始化 libolecf 对象
    libolecf_error_t* error = nullptr;
    libolecf_file_t* olecf2_file = nullptr;
    if (libolecf_file_initialize(&olecf2_file, nullptr) != 1)
    {
        qCritical() << "Unable to initialize libolecf.";
        libbfio_handle_free(&bfio_handle, nullptr);
        return successful;
    }

    // 使用内存句柄打开 OLECF
    if (libolecf_file_open_file_io_handle(
                olecf2_file,
                bfio_handle,
                LIBOLECF_OPEN_READ,
                &error) != 1)
    {
        qCritical() << "Unable to open OLECF from memory.";
        libolecf_file_free(&olecf2_file, nullptr);
        libbfio_handle_free(&bfio_handle, nullptr);
        return successful;
    }
    // 获取根项并解析
    libolecf_item_t* root_item = nullptr;
    if (libolecf_file_get_root_item(olecf2_file, &root_item, nullptr) == 1)
    {
        getOle10NativeData(root_item, stOleFile);
        if (!stOleFile.fileData.isEmpty())
        {
            successful = true;
        }

        libolecf_item_free(&root_item, nullptr);
    }

    // 清理资源
    libolecf_file_free(&olecf2_file, nullptr);
    libbfio_handle_free(&bfio_handle, nullptr);
    return successful;
}
#include <QtEndian>
bool checkOleHeader(const QByteArray& srcData)
{
    // OLECF 魔数 D0 CF 11 E0 A1 B1 1A E1
    QByteArray oleMagic = QByteArray::fromHex("D0CF11E0A1B11AE1");
    if(srcData.startsWith(oleMagic))
    {
        return true;
    }
    return false;
}
#include <QTextCodec>
#include <QDomElement>
QString GetAttachmentDocumentType(QByteArray xmlData)
{
    //    "clipboard/worksheets/worksheet1.xml"
    //QTextCodec *code = QTextCodec::codecForName("GBK");
    //QString xmlString = code->toUnicode(xmlData);
    QDomDocument xmlDoc;
    xmlDoc.setContent(xmlData);
    QDomElement mainEle = xmlDoc.documentElement();
    if(!mainEle.isNull())
    {
        QDomElement oldObjsNode = mainEle.firstChildElement("oleObjects");
        if(!oldObjsNode.isNull())
        {
            QDomElement altEle = oldObjsNode.firstChildElement("mc:AlternateContent");
            if(!altEle.isNull())
            {
                QDomElement backEle = altEle.firstChildElement("mc:Fallback");
                if(!backEle.isNull())
                {
                    QDomElement oleEle = backEle.firstChildElement("oleObject");
                    if(!oleEle.isNull())
                    {
                        QString qsID = oleEle.attribute("progId");
                        return qsID;
                    }
                }
            }
        }
    }
    return QString();
}

bool UtilityTool::findOleDataFromZipMemory(const QByteArray &zipBytes, QByteArray &outData, QString &qsDocType)
{
    bool findOleData = false;
    QBuffer buffer;
    buffer.setData(zipBytes);
    QByteArray dstData;
    if (!buffer.open(QIODevice::ReadOnly))
    {
        qWarning() << "QBuffer open fail";
        return findOleData;
    }

    QuaZip zip(&buffer);

    if (!zip.open(QuaZip::mdUnzip))
    {
        qWarning() << "QuaZip open fail, code =" << zip.getZipError();
        return findOleData;
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
    {
        QString name = zip.getCurrentFileName();
        // 目录条目
        if (name.endsWith('/'))
            continue;


        if(name.contains("/worksheets/worksheet"))
        {
            if(name.split("/").last().endsWith(".xml"))
            {
                QuaZipFile zaf(&zip);
                if (zaf.open(QIODevice::ReadOnly))
                {
                    QByteArray tmpData = zaf.readAll();
                    qsDocType = GetAttachmentDocumentType(tmpData);
//                    QString nameT = name.split("/").last();
//                    QFile file("/home/ft2000/mjcenv/dps-ppt/bugwenjian/" + nameT);
//                    if(file.open(QIODevice::WriteOnly))
//                    {
//                        file.write(tmpData);
//                        file.close();
//                    }
                    zaf.close();
                }
            }
        }

        if(!name.contains("oleObject") && !name.contains("embeddings"))
        {
            continue;
        }
        //01:OLE. 02:Embeddings
        QString qsFileName;
        QByteArray heard = QByteArray::fromHex("01");
        if(name.contains("embeddings"))
        {
            QStringList qsFloderLevelList = name.split("/");
            int currentPositionIndex = qsFloderLevelList.count() - 2;
            if(currentPositionIndex < 0)
            {
                continue;
            }
            QString qsParentFloder = qsFloderLevelList.at(currentPositionIndex);
            if(qsParentFloder != "embeddings")
            {
                continue;
            }
            heard = QByteArray::fromHex("02");
            qsFileName = name.split("/").last();
            QuaZipFile zf(&zip);
            if (!zf.open(QIODevice::ReadOnly))
            {
                qWarning() << "open file fail:" << name;
                continue;
            }
            findOleData = true;

            if(qsFileName.contains("oleobject",Qt::CaseSensitivity::CaseInsensitive))
            {
                QByteArray oleObjectData = zf.readAll();
                //                QFile file("/home/ft2000/mjcenv/dps-ppt/bugwenjian/ole.bin");
                //                if(file.open(QIODevice::WriteOnly))
                //                {
                //                    file.write(oleObjectData);
                //                    file.close();
                //                }
                QByteArray ba = QByteArray::fromHex("01") + oleObjectData;
                dstData = ba;
                zf.close();
                continue;
            }

            QByteArray ba = zf.readAll();
            QByteArray sizeBytes;
            sizeBytes.resize(4);
            int baSize = ba.size();

            QByteArray fileNameData=qsFileName.toUtf8().leftJustified(128, '\0', true);

            qToLittleEndian((quint32)baSize,(uchar*)sizeBytes.data());

            dstData = dstData + heard + QByteArray::fromHex("80") +
                    fileNameData + sizeBytes + ba;
            zf.close();
            break;
        }
        else
        {
            QuaZipFile zf(&zip);
            if (!zf.open(QIODevice::ReadOnly))
            {
                qWarning() << "open file fail:" << name;
                continue;
            }
            findOleData = true;
            dstData = heard + zf.readAll();
            zf.close();
            break;
        }
    }
    zip.close();
    outData = dstData;
    return findOleData;
}

UtilityTool::UtilityTool() {}
