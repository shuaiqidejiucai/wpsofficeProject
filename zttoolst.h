#ifndef ZTTOOLST_H
#define ZTTOOLST_H


#include <QSharedPointer>
#include "enumtype.h"
#include "structdefine.h"
extern "C"
{
 #include "libolecf.h"
}
namespace ZT_Libolecf {
    void oleItemDeleter(libolecf_item_t* item);

    void oleFileDeleter(libolecf_file_t* file);

    void oleErrorDeleter(libolecf_error_t* oleError);

    int ZT_libolecf_item_get_sub_item(const QSharedPointer<libolecf_item_t>& parentItem, int num, QSharedPointer<libolecf_item_t>& subItem, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_item_get_sub_item_by_utf8_path(const QSharedPointer<libolecf_item_t>& item,
        const uint8_t* utf8_string,
        size_t utf8_string_length,
        QSharedPointer<libolecf_item_t>& subItem,
        QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_file_initialize(QSharedPointer<libolecf_file_t>& file, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_file_open(const QSharedPointer<libolecf_file_t>& file, const char* filename, int access_flags, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_item_get_number_of_sub_items(const QSharedPointer<libolecf_item_t>& oleItem, int& count, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_item_get_utf8_name_size(const QSharedPointer<libolecf_item_t>& oleItem, size_t* utf8_string_size, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_item_get_utf8_name(const QSharedPointer<libolecf_item_t>& oleItem, uint8_t* utf8_string, const size_t& utf8_string_size, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_item_get_size(const QSharedPointer<libolecf_item_t>& item, uint32_t* size, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_stream_read_buffer(const QSharedPointer<libolecf_item_t>& intputItem, uint8_t* buffer, const size_t& size, QSharedPointer<libolecf_error_t>* error = nullptr);

    int ZT_libolecf_file_get_root_item(const QSharedPointer<libolecf_file_t>& file, QSharedPointer<libolecf_item_t>& pRootIem, QSharedPointer<libolecf_error_t>* error = nullptr);

}

class ZTTools
{
public:

    static bool parseOle10Native(const QByteArray& ole10NativeData, ST_VarantFile& stOleFile);

    static EU_DocumentType getOleFileFormat(const QSharedPointer<libolecf_item_t>& intputItem, bool& haveOutput, QSharedPointer<libolecf_item_t>& outputItem);

    static EU_DocumentType getZipPackage(const QByteArray& zipBytes);//查看内部结构是不是docx等

    static bool findOleTreeItem(const QSharedPointer<libolecf_item_t>& intputItem, const QString& nodeName, QSharedPointer<libolecf_item_t>& subItem, bool isRegex = false, bool isOutPut = false);

    static QString getOleItemName(const QSharedPointer<libolecf_item_t>& pItem);

    static QByteArray getOleItemData(const QSharedPointer<libolecf_item_t>& intputItem);


    ZTTools();
};

template<class T>
inline T GetFlagData(const char* srcData, quint32 &pos)
{
    T data = qFromLittleEndian<T>(reinterpret_cast<const uchar*>(srcData + pos));
    pos += sizeof(T);
    return data;
}

inline bool physicalStruct(quint32 pos, const QByteArray& srcData, ST_Variable& stVar)
{
    if (pos + 8 < srcData.size())
    {
        stVar.originPos = pos;
        quint16 head = GetFlagData<quint16>(srcData.constData(), pos);
        ST_TP(stVar) = GetFlagData<quint16>(srcData.constData(), pos);
        ST_SZ(stVar) = GetFlagData<quint32>(srcData.constData(), pos);

        ST_RV(stVar) = head & 0xF;
        ST_RI(stVar) = head >> 4;
        if (pos + ST_SZ(stVar) <= srcData.size())
        {
            ST_SP(stVar) = pos;
            ST_EP(stVar) = pos + ST_SZ(stVar);
            return true;
        }
    }
    return false;
}

inline QString GetQString(const char* dataPtr, quint32 length)
{
    QTextDecoder* decoder = QTextCodec::codecForName("UTF-16LE")->makeDecoder();
    QString qsContent = decoder->toUnicode(dataPtr, length);
    delete decoder;
    //移除终止符（如果有）
    if (qsContent.endsWith(QChar::Null)) {
        qsContent.chop(1);
    }
    return qsContent;
}
#endif // ZTTOOLST_H
