#include "wpscomment.h"
#include <QPixmap>
#include "variant.h"
#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <quazipfile.h>
#include <QBuffer>
#include <libolecf.h>
#include <libbfio_handle.h>
#include <libbfio_memory_range.h>
using namespace wpsapi;
using namespace kfc;
using namespace wpsapiex;

extern QString GetBSTRText(BSTR str);
WpsComment::WpsComment(QObject *parent)
    : QObject{parent}
    ,m_rpcClient(nullptr)
{}

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

static bool parseOle10Native(const QByteArray& src, QString& outFileName, QByteArray& outData)
{
    outFileName.clear();
    outData.clear();
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

        QByteArray label = readZ(p);
        if (label.isEmpty()) continue;

        QByteArray file = readZ(p);
        outFileName = file;
        if (file.isEmpty()) continue;

        QByteArray cmd = readZ(p);
        if (cmd.isEmpty()) continue;

        quint32 mark = rdLE32(p);
        p = p + 4;
        quint32 tempPathLen = rdLE32(p);
        p = p + 4;
        QByteArray tempPathBa = readZ(p);
        if (tempPathBa.isEmpty()) continue;

        quint32 datasize = rdLE32(p);
        p = p + 4;


        QByteArray srcData = src.mid(p, datasize);
        outData = srcData;
        ok = true;
    }

    return ok;
}


void getOle10NativeData(libolecf_item_t* root_item, QString& rootName, QByteArray & outData)
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
            QString fileName;
            if (parseOle10Native(ole10, fileName, outData))
            {
                rootName = fileName;
                libolecf_item_free(&sub_item, nullptr);
                return;
            }
        }
        if (childItemCount > 0 && outData.isEmpty())
        {
            getOle10NativeData(sub_item, rootName, outData);
        }
        libolecf_item_free(&sub_item, nullptr);
    }
    return;
}


bool attachmentName(const QByteArray& srcData, QString &fileName)
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
        QByteArray data;
        getOle10NativeData(root_item, fileName, data);
        if (!fileName.isEmpty())
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

void listAndReadZipFromMemory(const QByteArray& zipBytes) {
    QBuffer buffer;
    buffer.setData(zipBytes);
    if (!buffer.open(QIODevice::ReadOnly)) {
        qWarning() << "QBuffer open fail";
        return;
    }

    QuaZip zip(&buffer);
    if (!zip.open(QuaZip::mdUnzip)) {
        qWarning() << "QuaZip open fail, code =" << zip.getZipError();
        return;
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        QString name = zip.getCurrentFileName();
        qDebug() << "Entry:" << name;

        // 目录条目
        if (name.endsWith('/'))
            continue;

        if(!name.startsWith("oleObject"))
        {
            continue;
        }
        QuaZipFile zf(&zip);
        if (!zf.open(QIODevice::ReadOnly)) {
            qWarning() << "open file fail:" << name;
            continue;
        }
        QByteArray data = zf.readAll();
        zf.close();

        qDebug() << "  size =" << data.size();
        // TODO: 处理 data
    }

    zip.close();
}

bool WpsComment::initWpsApplication()
{
    if(m_rpcClient && m_rpcClient->getWpsApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        m_spApplication->put_Visible(false);
        m_spApplication->get_Documents(&m_spDocs);
        return true;
    }
    return false;
}

bool WpsComment::initWpsRpcClient()
{
    HRESULT hr = createWpsRpcInstance(&m_rpcClient);
    if (hr != S_OK)
    {
        qDebug() <<"get WpsRpcClient erro";
        return false;
    }
    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/wps"));
    m_rpcClient->setProcessPath(StrWpsAddress);
    SysFreeString(StrWpsAddress);

    QVector<BSTR> vArgs;
    vArgs.resize(2);
    vArgs[0] =  SysAllocString(__X("-shield"));
    vArgs[1] =  SysAllocString(__X("-multiply"));
    m_rpcClient->setProcessArgs(vArgs.size(), vArgs.data());
    for (int i = 0; i < vArgs.size(); i++)
    {
        SysFreeString(vArgs.at(i));
    }
    return true;
}

bool WpsComment::openWPSDoc(const QString &fileName)
{
    ks_bstr fileNameBstr (fileName.utf16());

    VARIANT fileNameVar;
    VariantInit(&fileNameVar);
    V_VT(&fileNameVar) = VT_BSTR;
    V_BSTR(&fileNameVar) = fileNameBstr;

    VARIANT visibleVar;
    VariantInit(&visibleVar);
    V_VT(&visibleVar) = VT_BOOL;
    V_BOOL(&visibleVar) = false;

    VARIANT argWps[15];
    for(int i = 0; i < 15; ++i)
    {
        VariantInit(&argWps[i]);
    }

    VARIANT missing;
    VariantInit(&missing);
    V_VT(&missing) = VT_ERROR;
    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

    HRESULT hr = m_spDocs->Open(&fileNameVar, &missing, &missing, &missing, &missing, &missing, &missing, &missing, &missing
                   , &missing, &missing, &missing, &missing, &missing, &missing, &missing,(Document**)&m_spDocument);
    // HRESULT hr = m_spDocs->Open(&fileNameVar, &argWps[0], &argWps[1],&argWps[2],&argWps[3],
    //                &argWps[4],&argWps[5],&argWps[6],&argWps[7],&argWps[8],&argWps[9],
    //                &argWps[10],&argWps[11],&argWps[12],&argWps[13],&argWps[14], (Document**)&m_spDocument);

    if (SUCCEEDED(hr))
    {
        qDebug() << "open ok";
        return true;
    }
    else
    {
        qDebug() << "open fail";
    }

    return false;
}

bool WpsComment::closeWPSDoc()
{
    KComVariant varSaveOptions(wdDoNotSaveChanges);
    KComVariant varOriginalFormat(wdWordDocument);
    KComVariant varRouteDocument;
    HRESULT hr = m_spDocument->Close(&varSaveOptions, &varOriginalFormat, &varRouteDocument);
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

void WpsComment::closeApp()
{
    KComVariant vars[3];
    if (m_spApplication != NULL)
    {
        m_spApplication->Quit(vars, vars+1, vars+2);
        m_rpcClient = nullptr;
        m_spDocs = NULL;
        m_spApplication = NULL;
    }
}

bool WpsComment::saveWPSDoc()
{
    HRESULT hr = m_spDocument->Save();
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

QStringList WpsComment::GetWPSText()
{
    QStringList qsTextList;
    QList<kfc::ks_stdptr<Range> > rangeList = GetTextRange();
    for(int i = 0; i < rangeList.count(); ++i)
    {
        kfc::ks_stdptr<Range> range = rangeList.at(i);
        if(!range)
        {
            continue;
        }
        BSTR bstr;
        range->get_Text(&bstr);
        QString text = GetBSTRText(bstr);
        text.remove("\u0001");
        text.remove("\r");
        if(!text.isEmpty())
        {
            qsTextList.append(text);
        }
    }
    return qsTextList;
}

void WpsComment::replaceTextForWPSDoc(const QString &findText, const QString &replaceText)
{
    QList<ks_stdptr<Range>> rangList = GetTextRange();
    for(int i = 0; i < rangList.count(); ++i)
    {
        ks_stdptr<Range> textRangePtr = rangList.at(i);
        if(!textRangePtr)
        {
            continue;
        }
        //ks_bstr findTextBSTR(findText.utf16());
        ks_bstr replaceTextBSTR(replaceText.utf16());
        //ks_stdptr<Range> replaceTextRange;
        BSTR bstr;
        textRangePtr->get_Text(&bstr);
        QString qsText = GetBSTRText(bstr);
        int index = qsText.indexOf(findText);
        if(index != -1)
        {
            int length = findText.count();
            ks_stdptr<Range> tmpRange = textRangePtr;
            tmpRange->SetRange(index+1, index + length+1);
            tmpRange->put_Text(replaceTextBSTR);
        }
    }
}

bool WpsComment::insertTextForTextRange(kfc::ks_stdptr<wpsapi::Range> range, int start, int length, const QString &qsTexts, bool after)
{
    if(!range)
    {
        return false;
    }
    range->SetRange(start + 1, start + 1 + length);
    HRESULT hr;
    ks_bstr bstr(qsTexts.utf16());
    if(after)
    {
        hr = range->InsertAfter(bstr);
    }
    else
    {
        hr = range->InsertBefore(bstr);
    }
    if (hr != S_OK)
    {
        return false;
    }
    return true;
}

void WpsComment::extractPicture()
{
    if(!m_spDocument)
    {
        return;
    }
    ks_stdptr<InlineShapes> inlineShapes;
    m_spDocument->get_InlineShapes(&inlineShapes);
    if(!inlineShapes)
    {
        return;
    }
    long count = 0;
    inlineShapes->get_Count(&count);
    for(int i = 1; i <= count; ++i)
    {
        ks_stdptr<InlineShape> shapePtr;
        inlineShapes->Item(i, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }
        WdInlineShapeType inlineShapeType;
        shapePtr->get_Type(&inlineShapeType);
        if(inlineShapeType == wdInlineShapeEmbeddedOLEObject
            || inlineShapeType == wdInlineShapeEmbeddedOLEObject
            || inlineShapeType == wdInlineShapeOLEControlObject)
        {
            shapePtr->Select();
            ks_stdptr<Selection> spSelection;
            m_spApplication->get_Selection(&spSelection);
            if(!spSelection)
            {
                continue;
            }
            //listAndReadZipFromMemory
            spSelection->Copy();
            const QMimeData *  mdata = qApp->clipboard()->mimeData();
            if(mdata)
            {
                QByteArray data = mdata->data("Kingsoft WPS 9.0 Format");

                listAndReadZipFromMemory(data);
            }
        }


        ks_stdptr<Range> range;
        shapePtr->get_Range(&range);
        if(!range)
        {
            range->CopyAsPicture();
            QPixmap map = qApp->clipboard()->pixmap();
            QLabel * label = new QLabel;
            label->setPixmap(map);
            label->show();
        }
    }

    ks_stdptr<Shapes> shapesPtr;
    m_spDocument->get_Shapes(&shapesPtr);
    if(!shapesPtr)
    {
        return;
    }
    long shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);
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
        MsoShapeType type;
        spShape->get_Type(&type);
        if(type == msoPicture || type == msoLinkedPicture)
        {
            VARIANT missing;
            VariantInit(&missing);
            V_VT(&missing) = VT_ERROR;
            V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;
            spShape->Select(&missing);
            ks_stdptr<Selection> spSelection;
            m_spApplication->get_Selection(&spSelection);
            if(!spSelection)
            {
                continue;
            }
            spSelection->CopyAsPicture();
            QPixmap map = qApp->clipboard()->pixmap();
            QLabel * label = new QLabel;
            label->setPixmap(map);
            label->show();
        }

    }
}

QList<kfc::ks_stdptr<Range> > WpsComment::GetTextRange()
{
    QList<ks_stdptr<Range> > rangeList;
    if(!m_spDocument)
    {
        return rangeList;
    }
    ks_stdptr<StoryRanges> storyRanges;
    m_spDocument->get_StoryRanges(&storyRanges);
    if(!storyRanges)
    {
        return rangeList;
    }

    ks_stdptr<Range> allRange;

    VARIANT start;
    VariantInit(&start);
    V_VT(&start) = VT_I4;
    V_I4(&start) = 0;

    VARIANT end;
    VariantInit(&end);
    V_VT(&end) = VT_I4;
    V_I4(&end) = 0;

    ks_stdptr<Range> tmpRange;
    m_spDocument->Range(&start,&end, &tmpRange);

    VARIANT unitVar;
    V_VT(&unitVar) = VT_UINT;
    V_UINT(&unitVar) = wdParagraph;

    long prop = 0;
    tmpRange->Expand(&unitVar, &prop);


    VARIANT count;
    VariantInit(&count);
    V_VT(&count) = VT_I4;
    V_I4(&count) = 1;

    while (tmpRange)
    {
        rangeList.append(tmpRange);
        ks_stdptr<Range> tmpRange2;
        tmpRange->Next(&unitVar, &count, &tmpRange2);
        tmpRange = tmpRange2;
    }

    return rangeList;
}
