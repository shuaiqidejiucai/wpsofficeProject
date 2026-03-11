#include "zttoolst.h"
#include "quazip.h"
#include "libolecf.h"
namespace ZT_Libolecf
{
    int ZT_libolecf_item_get_sub_item(const QSharedPointer<libolecf_item_t>& parentItem, int num, QSharedPointer<libolecf_item_t>& subItem, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_item_t* rawItem = nullptr;
        libolecf_error_t* rawError = nullptr;
        int errorCode = libolecf_item_get_sub_item(parentItem.data(), num, &rawItem, &rawError);
        if (rawItem) {
            subItem = QSharedPointer<libolecf_item_t>(rawItem, oleItemDeleter);
        }

        if (rawError && error) {
            *error = QSharedPointer<libolecf_error_t>(rawError, oleErrorDeleter);
        }

        return errorCode;
    }

    void oleItemDeleter(libolecf_item_t* item)
    {
        if (item)
        {
            libolecf_item_free(&item, nullptr);
        }
    }

    void oleFileDeleter(libolecf_file_t* file)
    {
        if (file)
        {
            libolecf_file_close(file, nullptr);
            libolecf_file_free(&file, nullptr);
        }
    }

    void oleErrorDeleter(libolecf_error_t* oleError)
    {
        if (oleError)
        {
            libolecf_error_free(&oleError);
        }
    }

    int ZT_libolecf_item_get_sub_item_by_utf8_path(const QSharedPointer<libolecf_item_t>& item, const uint8_t* utf8_string, size_t utf8_string_length, QSharedPointer<libolecf_item_t>& subItem, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_item_t* rawItem = nullptr;
        libolecf_error_t* oleError = nullptr;
        int errorCode = libolecf_item_get_sub_item_by_utf8_path(item.data(), utf8_string, utf8_string_length, &rawItem, &oleError);
        if (rawItem)
        {
            subItem = QSharedPointer<libolecf_item_t>(rawItem, oleItemDeleter);
        }
        if (oleError && error)
        {
            *error = QSharedPointer<libolecf_error_t>(oleError, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_file_initialize(QSharedPointer<libolecf_file_t>& file, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_file_t* filePtr = nullptr;
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_file_initialize(&filePtr, &errorPtr);
        if (filePtr)
        {
            file = QSharedPointer<libolecf_file_t>(filePtr, oleFileDeleter);
        }
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_file_open(const QSharedPointer<libolecf_file_t>& file, const char* filename, int access_flags, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_file_open(file.data(), filename, LIBOLECF_OPEN_READ, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_item_get_number_of_sub_items(const QSharedPointer<libolecf_item_t>& oleItem, int& count, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_item_get_number_of_sub_items(oleItem.data(), &count, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }

        return errorCode;
    }

    int ZT_libolecf_item_get_utf8_name_size(const QSharedPointer<libolecf_item_t>& oleItem, size_t* utf8_string_size, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;

        int errorCode = libolecf_item_get_utf8_name_size(oleItem.data(), utf8_string_size, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_item_get_utf8_name(const QSharedPointer<libolecf_item_t>& oleItem, uint8_t* utf8_string, const size_t& utf8_string_size, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_item_get_utf8_name(oleItem.data(), utf8_string, utf8_string_size, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_item_get_size(const QSharedPointer<libolecf_item_t>& item, uint32_t* size, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_item_get_size(item.data(), size, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_stream_read_buffer(const QSharedPointer<libolecf_item_t>& intputItem, uint8_t* buffer, const size_t& size, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_stream_read_buffer(intputItem.data(), buffer, size, &errorPtr);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

    int ZT_libolecf_file_get_root_item(const QSharedPointer<libolecf_file_t>& file, QSharedPointer<libolecf_item_t>& pRootIem, QSharedPointer<libolecf_error_t>* error)
    {
        libolecf_item_t* pItem = nullptr;
        libolecf_error_t* errorPtr = nullptr;
        int errorCode = libolecf_file_get_root_item(file.data(), &pItem, &errorPtr);
        pRootIem = QSharedPointer<libolecf_item_t>(pItem, oleItemDeleter);
        if (errorPtr && error)
        {
            *error = QSharedPointer<libolecf_error_t>(errorPtr, oleErrorDeleter);
        }
        return errorCode;
    }

};

using namespace ZT_Libolecf;
bool ZTTools::parseOle10Native(const QByteArray& ole10NativeData, ST_VarantFile& stOleFile)
{
    if (ole10NativeData.size() < 12) return false;

    int offset = 0;

    // 1. totalSize
    quint32 totalSize = qFromLittleEndian<quint32>(
        reinterpret_cast<const uchar*>(ole10NativeData.constData() + offset)
        );
    offset += 4;

    // 2. signature/flags1
    quint16 signature = qFromLittleEndian<quint16>(
        reinterpret_cast<const uchar*>(ole10NativeData.constData() + offset)
        );
    offset += 2;

    QTextCodec* gbkCodec = QTextCodec::codecForName("GBK");

    if (signature == 0x0002) {
        // ===== Apache POI 的启发式判断 =====
        uchar firstByte = (offset < ole10NativeData.size()) ? static_cast<uchar>(ole10NativeData[offset]) : 0;

        if (firstByte > 0 && firstByte < 32) {
            // Compact 模式：label 的第一个字节是控制字符
            int dataSize = totalSize - 2;  // 减去 signature
            if (offset + dataSize <= ole10NativeData.size()) {
                stOleFile.fileData = ole10NativeData.mid(offset, dataSize);
                return true;
            }
        }
        else {
            // Parsed 模式：标准格式

            // ===== LibreOffice 的字符串跳过策略 =====
            auto skipNullTerm = [&]() -> QByteArray {
                int start = offset;
                while (offset < ole10NativeData.size() && ole10NativeData[offset] != '\0') ++offset;
                QByteArray result = ole10NativeData.mid(start, offset - start);
                if (offset < ole10NativeData.size()) ++offset;  // 跳过 '\0'
                return result;
            };

            // label
            QByteArray label = skipNullTerm();
            stOleFile.qsFileName = gbkCodec->toUnicode(label);

            // fileName
            QByteArray fileName = skipNullTerm();
            stOleFile.qsFilePath = gbkCodec->toUnicode(fileName);

            // 尝试读取可选的 flags2 + unknown1 (4 bytes)
            if (offset + 4 <= ole10NativeData.size()) {
                offset += 4;
            }

            // 尝试读取可选的 command（带长度前缀）
            if (offset + 4 <= ole10NativeData.size()) {
                quint32 commandLen = qFromLittleEndian<quint32>(
                    reinterpret_cast<const uchar*>(ole10NativeData.constData() + offset)
                    );
                if (commandLen > 0 && commandLen < 1024 &&
                    offset + 4 + commandLen <= ole10NativeData.size()) {
                    offset += 4 + commandLen;
                }
            }

            // dataSize
            if (offset + 4 <= ole10NativeData.size()) {
                quint32 dataSize = qFromLittleEndian<quint32>(
                    reinterpret_cast<const uchar*>(ole10NativeData.constData() + offset)
                    );
                offset += 4;

                if (offset + dataSize <= ole10NativeData.size()) {
                    stOleFile.fileData = ole10NativeData.mid(offset, dataSize);
                    return true;
                }
            }
        }
    }

    // ===== Unparsed 模式（兜底）=====
    // 从当前位置读取到 totalSize 指定的末尾
    int expectedEnd = 4 + totalSize;
    int remainingSize = expectedEnd - offset;
    if (remainingSize > 0 && offset + remainingSize <= ole10NativeData.size()) {
        stOleFile.fileData = ole10NativeData.mid(offset, remainingSize);
        return true;
    }

    return false;
}

EU_DocumentType ZTTools::getOleFileFormat(const QSharedPointer<libolecf_item_t>& intputItem, bool& haveOutput, QSharedPointer<libolecf_item_t>& outputItem)
{
    bool isDoc = true;
    QSharedPointer<libolecf_item_t> outPutItem;
    isDoc = isDoc && findOleTreeItem(intputItem, "WordDocument", outPutItem);

    bool isXls = true;
    isXls = isXls && findOleTreeItem(intputItem, "Workbook", outPutItem);

    bool isPpt = true;
    isPpt = isPpt && findOleTreeItem(intputItem, "PowerPoint Document", outPutItem);
    isPpt = isPpt && findOleTreeItem(intputItem, "Current User", outPutItem);
    EU_DocumentType documentType = EU_DocumentType::EU_NoType;
    if (isDoc)
    {
        if (findOleTreeItem(intputItem, "WpsCustomData", outPutItem))
        {
            documentType = EU_WPSType;
        }
        else
        {
            documentType = EU_DOCType;
        }
    }
    else if (isXls)
    {
        documentType = EU_XLSType;
    }
    else if (isPpt)
    {
        documentType = EU_PPTType;
    }
    else
    {
        QSharedPointer<libolecf_item_t> packageItem;
        if (findOleTreeItem(intputItem, "Package", packageItem, false, true))
        {
            if (packageItem)
            {
                QByteArray zipData = getOleItemData(packageItem);
                documentType = getZipPackage(zipData);
                haveOutput = true;
                outputItem = packageItem;
            }
        }
    }

    if (documentType == EU_DocumentType::EU_NoType)
    {
        QSharedPointer<libolecf_item_t> ole10NativeItem;
        if (findOleTreeItem(intputItem, "Ole10Native", ole10NativeItem, true, true))
        {
            if (ole10NativeItem)
            {
                documentType = EU_DocumentType::EU_BinType;
                haveOutput = true;
                outputItem = ole10NativeItem;
            }
        }
    }

    return documentType;
}

EU_DocumentType ZTTools::getZipPackage(const QByteArray& zipBytes)
{
    QBuffer buffer;
    buffer.setData(zipBytes);
    if (!buffer.open(QIODevice::ReadOnly))
    {
        qWarning() << "QBuffer open fail";
        return EU_DocumentType::EU_NoType;
    }

    QuaZip zip(&buffer);
    if (!zip.open(QuaZip::mdUnzip))
    {
        qWarning() << "QuaZip open fail, code =" << zip.getZipError();
        return EU_DocumentType::EU_NoType;
    }

    bool isDocx = true;
    isDocx = isDocx && zip.setCurrentFile("[Content_Types].xml");
    isDocx = isDocx && zip.setCurrentFile("_rels/.rels");
    isDocx = isDocx && zip.setCurrentFile("word/document.xml");

    bool isXlsx = true;
    isXlsx = isXlsx && zip.setCurrentFile("[Content_Types].xml");
    isXlsx = isXlsx && zip.setCurrentFile("_rels/.rels");
    isXlsx = isXlsx && zip.setCurrentFile("xl/workbook.xml");
    isXlsx = isXlsx && zip.setCurrentFile("xl/_rels/workbook.xml.rels");
    isXlsx = isXlsx && zip.setCurrentFile("xl/worksheets/");

    bool isPptx = true;
    isPptx = isPptx && zip.setCurrentFile("[Content_Types].xml");
    isPptx = isPptx && zip.setCurrentFile("_rels/.rels");
    isPptx = isPptx && zip.setCurrentFile("ppt/presentation.xml");
    isPptx = isPptx && zip.setCurrentFile("ppt/_rels/presentation.xml.rels");
    isPptx = isPptx && zip.setCurrentFile("ppt/slides/");

    if (isDocx)
    {
        return EU_DocumentType::EU_DOCXType;
    }
    else if (isXlsx)
    {
        return EU_DocumentType::EU_XLSXType;
    }
    else if (isPptx)
    {
        return EU_DocumentType::EU_PPTXType;
    }
    return EU_DocumentType::EU_NoType;
}

bool ZTTools::findOleTreeItem(const QSharedPointer<libolecf_item_t>& intputItem, const QString& nodeName, QSharedPointer<libolecf_item_t>& subItem, bool isRegex, bool isOutPut)
{
    if (isRegex)
    {
        int nItemCount = 0;
        ZT_libolecf_item_get_number_of_sub_items(intputItem, nItemCount, nullptr);

        for (int i = 0; i < nItemCount; ++i)
        {
            QSharedPointer<libolecf_item_t> subTmpItem;
            ZT_libolecf_item_get_sub_item(intputItem, i, subTmpItem, nullptr);

            if (!subTmpItem)
            {
                continue;
            }
            QString nodeItemName = getOleItemName(subTmpItem);
            if (nodeItemName.contains(nodeName))
            {
                if (isOutPut)
                {
                    subItem = subTmpItem;
                    return true;
                }
            }
        }
    }
    else
    {
        QSharedPointer<libolecf_item_t> subTmpItem;
        ZT_libolecf_item_get_sub_item_by_utf8_path(intputItem, (uint8_t*)nodeName.toUtf8().data(), nodeName.toUtf8().size(), subTmpItem, nullptr);
        if (subTmpItem)
        {
            if (isOutPut)
            {
                subItem = subTmpItem;
                return true;
            }
        }
    }
    return false;
}

QString ZTTools::getOleItemName(const QSharedPointer<libolecf_item_t>& pItem)
{
    size_t stringSize = 0;
    ZT_libolecf_item_get_utf8_name_size(pItem, &stringSize, nullptr);
    QByteArray nameData;
    nameData.resize(stringSize);
    ZT_libolecf_item_get_utf8_name(pItem, (uint8_t*)nameData.data(), stringSize, nullptr);
    QString nodeName = QString::fromUtf8(nameData);
    return nodeName;
}

QByteArray ZTTools::getOleItemData(const QSharedPointer<libolecf_item_t>& intputItem)
{
    QByteArray data;
    if (intputItem)
    {
        // 读取流数据
        uint32_t stream_size = 0;
        ZT_libolecf_item_get_size(intputItem, &stream_size, nullptr);
        if (stream_size > 0)
        {
            data.resize(stream_size);
            ZT_libolecf_stream_read_buffer(intputItem, (uint8_t*)data.data(), stream_size, nullptr);
        }
    }
    return data;
}

ZTTools::ZTTools() {}
