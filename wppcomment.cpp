#include "wppcomment.h"
#include <QDebug>
#include "qlabel.h"
#include "variant.h"
#include "wpsapiex.h"
#include <QApplication>
#include <QClipboard>
#include <QBuffer>
#include <QMimeData>
#include <QStack>
#include <utilitytool.h>
#include <log_global.h>
#include<QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
//#include <quazip.h>

using namespace wppapi;
using namespace kfc;
using namespace wpsapiex;

extern QString GetBSTRText(BSTR str);
WppComment::WppComment(QObject *parent)
    : QObject{parent},m_rpcClient(nullptr),m_containWidget(nullptr)
{}

bool WppComment::initWppApplication()
{
    if(m_rpcClient && m_rpcClient->getWppApplication((IUnknown**)&m_spApplication) == S_OK)
    {
        MsoTriState state = msoFalse;
        m_spApplication->get_Presentations(&m_spDocs);
        m_spApplication->put_DisplayAlerts(PpAlertLevel::ppAlertsNone);
        return true;
    }
    return false;
}
#include <libolecf.h>
#include <libbfio_handle.h>
#include <libolecf/libolecf_file.h>
#include <libbfio_memory_range.h>
bool WppComment::initWPPRpcClient()
{
    HRESULT hr = createWppRpcInstance(&m_rpcClient);
    if (hr != S_OK)
    {
        //qDebug() <<"get WpsRpcClient erro";
        return false;
    }

    BSTR StrWpsAddress = SysAllocString(__X("/opt/kingsoft/wps-office/office6/wpp"));
    m_rpcClient->setProcessPath(StrWpsAddress);
    SysFreeString(StrWpsAddress);

    m_containWidget = new QWidget;
    m_containWidget->setFixedSize(200,100);
    m_containWidget->show();
    QVector<BSTR> vArgs;
    vArgs.resize(6);
    vArgs[0] =  SysAllocString(__X("-shield"));
    vArgs[1] =  SysAllocString(__X("-multiply"));
    vArgs[2] =  SysAllocString(__X("-x11embed"));
    vArgs[3] =  SysAllocString(QString::number(m_containWidget->winId()).utf16());
    vArgs[4] =  SysAllocString(QString::number(m_containWidget->width()).utf16());
    vArgs[5] =  SysAllocString(QString::number(m_containWidget->height()).utf16());
    m_rpcClient->setProcessArgs(vArgs.size(), vArgs.data());
    for (int i = 0; i < vArgs.size(); i++)
    {
        SysFreeString(vArgs.at(i));
    }
    m_containWidget->hide();
    return true;
}
#include <QString>
#include <QtEndian>
bool isPptEncrypted(const QByteArray &currentUserStream) {
    if (currentUserStream.size() < 16) return false;

    const uchar* p = reinterpret_cast<const uchar*>(currentUserStream.constData());
    quint16 recType = qFromLittleEndian<quint16>(p + 2); // 0x0FF6
    quint32 len = qFromLittleEndian<quint32>(p + 4);
    quint16 docVer = qFromLittleEndian<quint16>(p + 14);

    // 加密特征：docFileVersion=0 且长度异常短
    return (docVer == 0x0000 && len < 30);
}

bool WppComment::openWPPDoc(const QString &fileName)
{
    if(isFileLocked(fileName))
    {
        SPDLOG_ERROR(QString("%1 has been opened").arg(fileName).toUtf8().data());
    }
    ks_bstr filename(fileName.utf16());
    wppapi::MsoTriState readOnly = msoFalse;
    wppapi::MsoTriState untitled = msoFalse;
    wppapi::MsoTriState withWindow = msoFalse;
    ks_stdptr<_Presentation> spPresentation;


    bool ok = UtilityTool::findNameOleBinFromFile(fileName);

    if(ok)
    {
        return false;
    }
    HRESULT hr = m_spDocs->Open(filename, readOnly, untitled, withWindow, (Presentation**)&spPresentation);


    if (SUCCEEDED(hr))
    {
        m_spPresentation = spPresentation;
        //qDebug() << "open ok";
        return true;
    }
    else
    {
        qDebug()<< HRESULT_CODE(hr);
        qDebug() << "open fail";
    }

    return false;
}

bool WppComment::closeWPPDoc()
{
    HRESULT hr = m_spPresentation->Close();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    return false;
}

#include <ksoapi/ksoapi.h>
QStringList GetSmartArtText(ks_stdptr<Shape> tmpShape)
{
    QStringList qsTextList;
    MsoShapeType tmpShapeType;
    tmpShape->get_Type(&tmpShapeType);

    if(tmpShapeType == MsoShapeType::msoSmartArt)
    {
        ks_stdptr<SmartArt> smartArtPtr;
        tmpShape->get_SmartArt(&smartArtPtr);
        if(smartArtPtr)
        {
            ks_stdptr<ksoapi::SmartArtNodes> nodesPtr;
            smartArtPtr->get_AllNodes(&nodesPtr);
            if(nodesPtr)
            {
                int nodeCount = 0;
                nodesPtr->get_Count(&nodeCount);
                for(int nodeIndex = 1; nodeIndex <= nodeCount; ++nodeIndex)
                {
                    VARIANT nodeIndexVar;
                    VariantInit(&nodeIndexVar);
                    V_VT(&nodeIndexVar) = VT_I4;
                    V_I4(&nodeIndexVar) = nodeIndex;
                    ks_stdptr<ksoapi::SmartArtNode> nodePtr;
                    nodesPtr->get_Item(nodeIndexVar, &nodePtr);
                    if(nodePtr)
                    {
                        ks_stdptr<ksoapi::TextFrame2> frame2Ptr;
                        nodePtr->get_TextFrame2(&frame2Ptr);
                        if(frame2Ptr)
                        {
                            ks_stdptr<ksoapi::TextRange2> textRange2Ptr;
                            frame2Ptr->get_TextRange(&textRange2Ptr);
                            if(textRange2Ptr)
                            {
                               ks_bstr textPtr;
                               textRange2Ptr->get_Text(&textPtr);
                               QString qsText = GetBSTRText(textPtr);
                               qsTextList.append(qsText);
                            }
                        }
                    }
                }
            }

            //

        }
    }
    return qsTextList;
}

QStringList WppComment::GetWPPText()
{
    QStringList qsStrList;

    if(!m_spPresentation)
    {
        return qsStrList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return qsStrList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    for(long j = 1; j <= slidRangeCount; ++j)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = j;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int q = 1; q <= shapeCount; ++q)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = q;
            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            if(!shapePtr)
            {
                continue;
            }

            MsoShapeType soShape;
            shapePtr->get_Type(&soShape);
            if(soShape == msoTable)
            {
                qsStrList.append(getTableTextList(shapePtr));
            }

            QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList<WppApiTypes>(shapePtr);
            for(int i = 0; i < shapePtrList.count(); ++i)
            {
                ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
                QStringList smartTextList = GetSmartArtText(tmpShape);
                qsStrList.append(smartTextList);
                ks_stdptr<TextFrame> textFramePtr;
                tmpShape->get_TextFrame(&textFramePtr);
                ks_stdptr<TextEffectFormat> textEffectFramePtr;
                tmpShape->get_TextEffect(&textEffectFramePtr);

                if(textFramePtr)
                {
                    ks_stdptr<TextRange> textRangePtr;
                    textFramePtr->get_TextRange(&textRangePtr);
                    if(textRangePtr)
                    {
                        BSTR frameText;
                        textRangePtr->get_Text(&frameText);
                        qsStrList.append(GetBSTRText(frameText));
                    }
                }
                if(textEffectFramePtr)
                {
                    BSTR effectText;
                    textEffectFramePtr->get_Text(&effectText);
                    qsStrList.append(GetBSTRText(effectText));
                }
            }
        }
    }

    return qsStrList;
}

long WppComment::getPageCount()
{
    if(!m_spPresentation)
    {
        return 0;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return 0;
    }

    long slideCount = 0;
    slidesPtr->get_Count(&slideCount);
    return slideCount;
}

QList<kfc::ks_stdptr<TextRange> > WppComment::GetTextRangeForShapes(kfc::ks_stdptr<wppapi::Shapes> shapesPtr)
{
    QList<ks_stdptr<TextRange>> textRangeList;
    if(!shapesPtr)
    {
        return textRangeList;
    }
    int shapeCount = 0;
    shapesPtr->get_Count(&shapeCount);
    for(int q = 1; q <= shapeCount + 1; ++q)
    {
        VARIANT shapeIndex;
        VariantInit(&shapeIndex);
        V_VT(&shapeIndex) = VT_I4;
        V_I4(&shapeIndex) = q;
        ks_stdptr<Shape> shapePtr;
        shapesPtr->Item(shapeIndex, &shapePtr);
        if(!shapePtr)
        {
            continue;
        }

        QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList<WppApiTypes>(shapePtr);
        for(int i = 0; i < shapePtrList.count(); ++i)
        {
            ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
            ks_stdptr<TextFrame> textFramePtr;
            tmpShape->get_TextFrame(&textFramePtr);
            if(!textFramePtr)
            {
                continue;
            }
            ks_stdptr<TextRange> textRangePtr;
            textFramePtr->get_TextRange(&textRangePtr);
            if(textRangePtr)
            {
                textRangeList.append(textRangePtr);
            }
        }
    }
    return textRangeList;
}

QList<kfc::ks_stdptr<wppapi::TextRange>> WppComment::getRearks(long pageIndex)
{
    QList<ks_stdptr<TextRange>> qsRearkRangeList;
    if(!m_spPresentation)
    {
        return qsRearkRangeList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return qsRearkRangeList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);

    VARIANT rangeIndex;
    VariantInit(&rangeIndex);
    V_VT(&rangeIndex) = VT_I4;
    V_I4(&rangeIndex) = pageIndex;

    ks_stdptr<_Slide> slide;
    slidesPtr->Item(rangeIndex, (Slide**)&slide);
    if(slide)
    {
        ks_stdptr<SlideRange> slideRange;
        slide->get_NotesPage(&slideRange);
        if(slideRange)
        {
            ks_stdptr<Shapes> shapesPtr;
            slideRange->get_Shapes(&shapesPtr);
            QList<ks_stdptr<TextRange>> beizhuRangeList = GetTextRangeForShapes(shapesPtr);
            qsRearkRangeList = beizhuRangeList;
        }
    }
    return qsRearkRangeList;
}

QList<kfc::ks_stdptr<TextRange> > WppComment::getMaster()
{
    QList<ks_stdptr<TextRange>> textRangeList;
    if(!m_spPresentation)
    {
        return textRangeList;
    }

    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return textRangeList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);

    for(int pageIndex = 1; pageIndex <= slidRangeCount; ++pageIndex)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = pageIndex;

        ks_stdptr<_Slide> slide;
        slidesPtr->Item(rangeIndex, (Slide**)&slide);
        if(slide)
        {
            ks_stdptr<_Master> sliderMaster;
            slide->get_Master(&sliderMaster);
            if(sliderMaster)
            {
                ks_stdptr<Shapes> shapesPtr;
                sliderMaster->get_Shapes(&shapesPtr);
                if(!shapesPtr)
                {
                    return textRangeList;
                }
                int shapeCount = 0;
                shapesPtr->get_Count(&shapeCount);
                for(int q = 1; q <= shapeCount; ++q)
                {
                    VARIANT shapeIndex;
                    VariantInit(&shapeIndex);
                    V_VT(&shapeIndex) = VT_I4;
                    V_I4(&shapeIndex) = q;
                    ks_stdptr<Shape> shapePtr;
                    shapesPtr->Item(shapeIndex, &shapePtr);
                    if(!shapePtr)
                    {
                        continue;
                    }

                    QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList<WppApiTypes>(shapePtr);
                    for(int i = 0; i < shapePtrList.count(); ++i)
                    {
                        ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
                        ks_stdptr<TextFrame> textFramePtr;
                        tmpShape->get_TextFrame(&textFramePtr);
                        if(!textFramePtr)
                        {
                            continue;
                        }
                        ks_stdptr<TextRange> textRangePtr;
                        textFramePtr->get_TextRange(&textRangePtr);
                        if(textRangePtr)
                        {
                            textRangeList.append(textRangePtr);
                        }
                    }
                }
            }
        }

    }
    return textRangeList;
}

QStringList WppComment::getPPLayout(long pageIndex)
{
    QStringList qsTextList;
    if(!m_spPresentation)
    {
        return qsTextList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return qsTextList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);

    VARIANT rangeIndex;
    VariantInit(&rangeIndex);
    V_VT(&rangeIndex) = VT_I4;
    V_I4(&rangeIndex) = pageIndex;

    ks_stdptr<_Slide> slide;
    slidesPtr->Item(rangeIndex, (Slide**)&slide);
    if(slide)
    {
        ks_stdptr<CustomLayout> custPtr;
        slide->get_CustomLayout(&custPtr);
        if(custPtr)
        {
            ks_stdptr<Shapes> shapesPtr;
            custPtr->get_Shapes(&shapesPtr);
            if(!shapesPtr)
            {
                return qsTextList;
            }
            int shapeCount = 0;
            shapesPtr->get_Count(&shapeCount);
            for(int q = 1; q <= shapeCount; ++q)
            {
                VARIANT shapeIndex;
                VariantInit(&shapeIndex);
                V_VT(&shapeIndex) = VT_I4;
                V_I4(&shapeIndex) = q;
                ks_stdptr<Shape> shapePtr;
                shapesPtr->Item(shapeIndex, &shapePtr);
                if(!shapePtr)
                {
                    continue;
                }
                QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList<WppApiTypes>(shapePtr);
                for(int i = 0; i < shapePtrList.count(); ++i)
                {
                    ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
                    ks_stdptr<TextFrame> textFramePtr;
                    MsoShapeType shapeType;
                    tmpShape->get_Type(&shapeType);
                    tmpShape->get_TextFrame(&textFramePtr);
                    ks_stdptr<TextEffectFormat> textEffectFramePtr;
                    tmpShape->get_TextEffect(&textEffectFramePtr);

                    if(textFramePtr)
                    {
                        ks_stdptr<TextRange> textRangePtr;
                        textFramePtr->get_TextRange(&textRangePtr);
                        if(textRangePtr)
                        {
                            BSTR frameText;
                            textRangePtr->get_Text(&frameText);
                            qsTextList.append(GetBSTRText(frameText));
                        }
                    }
                    else if(textEffectFramePtr)
                    {
                        BSTR effectText;
                        textEffectFramePtr->get_Text(&effectText);
                        qsTextList.append(GetBSTRText(effectText));
                    }
                }
            }
        }
    }
    return qsTextList;
}

QStringList WppComment::getPPLayoutText()
{
    QStringList qsLayoutStringList;
    int count = getPageCount();
    for(int i = 1; i <= count; ++i)
    {
        QStringList qsTextList = getPPLayout(i);
        qsLayoutStringList.append(qsTextList);
    }
    return qsLayoutStringList;
}

QStringList WppComment::getMasterText()
{
    QList<ks_stdptr<TextRange>> masterRangeList = getMaster();
    return TextRnageToQString(masterRangeList);
}

QStringList WppComment::TextRnageToQString(QList<kfc::ks_stdptr<wppapi::TextRange>> textRangePtrList)
{
    QStringList qsTextList;
    for(int j = 0; j < textRangePtrList.count(); ++j)
    {
        ks_stdptr<TextRange> textRangePtr = textRangePtrList.at(j);
        BSTR bText;
        textRangePtr->get_Text(&bText);
        QString qsStr = GetBSTRText(bText);
        qsTextList.append(qsStr);
    }
    return qsTextList;
}

QStringList WppComment::getRearksText()
{
    QStringList rearlksStringList;
    int count = getPageCount();
    for(int i = 1; i <= count; ++i)
    {
        QList<ks_stdptr<TextRange>> qsRearkRangeList = getRearks(i);
        rearlksStringList.append(TextRnageToQString(qsRearkRangeList));
    }
    return rearlksStringList;
}

void WppComment::replaceTextForWPPDoc(const QString &findText, const QString &replaceText)
{
    QList<ks_stdptr<TextRange>> textRangList = GetTextRange();
    for(int i = 0; i < textRangList.count(); ++i)
    {
        ks_stdptr<TextRange> textRangePtr = textRangList.at(i);
        if(!textRangePtr)
        {
            continue;
        }
        ks_bstr findTextBSTR(findText.utf16());
        ks_bstr replaceTextBSTR(replaceText.utf16());
        ks_stdptr<TextRange> replaceTextRange;
        textRangePtr->Replace(findTextBSTR,replaceTextBSTR, -1, msoFalse, msoFalse,&replaceTextRange);
    }
}

bool WppComment::insertTextForTextRange(kfc::ks_stdptr<wppapi::TextRange> range, int start, int length, const QString &qsTexts, bool after)
{
    if(!range)
    {
        return false;
    }
    ks_stdptr<TextRange> midTextRange;
    range->Characters(start, length, &midTextRange);
    if(midTextRange)
    {
        ks_bstr newBstr(qsTexts.utf16());
        ks_stdptr<TextRange> newTextRange;
        HRESULT hr;
        if(after)
        {
            hr = midTextRange->InsertAfter(newBstr, &newTextRange);
        }
        else
        {
            hr = midTextRange->InsertBefore(newBstr, &newTextRange);
        }
        if (SUCCEEDED(hr))
        {
            return true;
        }
    }
    return false;
}

void WppComment::getOleFileData(GetNextOleDataFun oleDataPtr)
{
    extractFile(OleFileType, oleDataPtr);
}

void WppComment::extractPicture(GetNextOleDataFun imageFunPtr)
{
    extractFile(ImageType, imageFunPtr);
}

void WppComment::extractPictureNomemery(const QString &qsImageDir)
{
    if(!m_spPresentation || qsImageDir.isEmpty())
    {
        return;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return;
    }

    QList<ks_stdptr<Shape>> shapeList;
    ks_stdptr<Shapes> belongShapesPtr;
    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    SPDLOG_INFO(QString("slider count is %1").arg(slidRangeCount).toUtf8().data());
    for(long i = 1; i <= slidRangeCount; ++i)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = i;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        belongShapesPtr = shapesPtr;
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int j = 1; j <= shapeCount; ++j)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = j;

            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            MsoShapeType tmpShapgeType;
            shapePtr->get_Type(&tmpShapgeType);
            //if(tmpShapgeType == msoGroup)
            //{
            //ks_stdptr<ShapeRange> shapeRangePtr;
            //shapePtr->Ungroup(&shapeRangePtr);
            //long llcount = 0;
            //shapeRangePtr->get_Count(&llcount);
            //shapesPtr->get_Count(&shapeCount);
            //j--;
            //continue;
            //}

            QList<kfc::ks_stdptr<wppapi::Shape>> tmpShapeList = GetShapeGroupList<WppApiTypes>(shapePtr, ImageType);
            shapeList.append(tmpShapeList);
        }
    }

    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> shapePtr = shapeList.at(i);
        QString outPAth = QString(qsImageDir + "/" + QString::number(i) + ".png");
        ks_bstr filePathBstr (outPAth.utf16());
        HRESULT opHr = shapePtr->Export(filePathBstr, ppShapeFormatPNG);

        if(SUCCEEDED(opHr))
        {
            //qDebug()<<"export successful";
        }
        else
        {
            SPDLOG_ERROR(QString("%1 export failed").arg(outPAth).toUtf8().data());
        }
    }
    return;
}

void WppComment::extractFile(EU_FileType fileType, GetNextOleDataFun fileFunPtr)
{
    if(!m_spPresentation || !fileFunPtr)
    {
        return;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return;
    }

    QList<ks_stdptr<Shape>> shapeList;
    ks_stdptr<Shapes> belongShapesPtr;
    long count = 0;
    slidesPtr->get_Count(&count);
    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    for(long i = 1; i <= slidRangeCount; ++i)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = i;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        belongShapesPtr = shapesPtr;
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int j = 1; j <= shapeCount; ++j)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = j;

            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            MsoShapeType tmpShapgeType;
            shapePtr->get_Type(&tmpShapgeType);

            //            if(tmpShapgeType == msoGroup)
            //            {
            //                ks_stdptr<ShapeRange> shapeRangePtr;
            //                shapePtr->Ungroup(&shapeRangePtr);
            //                long llcount = 0;
            //                shapeRangePtr->get_Count(&llcount);
            //                shapesPtr->get_Count(&shapeCount);
            //                j--;
            //                continue;
            //            }

            QList<kfc::ks_stdptr<wppapi::Shape>> tmpShapeList = GetShapeGroupList<WppApiTypes>(shapePtr, fileType);
            shapeList.append(tmpShapeList);
        }
    }

    bool isContinue = false;
    for(int i = 0; i < shapeList.count(); ++i)
    {
        ks_stdptr<Shape> shapePtr = shapeList.at(i);
        if(fileType == ImageType)
        {
            getPictureForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else if(fileType == OleFileType)
        {
            getOldFileDataForShape(belongShapesPtr, shapePtr, fileFunPtr, isContinue);
        }
        else
        {
            //qDebug()<<"UnKnown Type";
        }
        if(!isContinue)
        {
            return;
        }
    }
    return;
}
int pppp = 0;

bool WppComment::getPictureForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun imageDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!imageDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    float fleft = 0.0;
    float ftop = 0.0;
    float fwidth = 0.0;
    float fheight = 0.0;

    shapePtr->get_Left(&fleft);
    shapePtr->get_Top(&ftop);
    shapePtr->get_Width(&fwidth);
    shapePtr->get_Height(&fheight);

    ks_bstr filePathBstr (QString("/home/ft2000/mjcenv/dps-ppt/outImage/" + QString::number(pppp) + "ceshi.png").utf16());
    pppp++;
    HRESULT opHr = shapePtr->Export(filePathBstr, ppShapeFormatPNG);
    if(SUCCEEDED(opHr))
    {
        qDebug()<<"success;";
    }
    else {

        qDebug()<<"faile;";
    }
    // isContinue = true;
    // return false;
    //MsoAutoShapeType autoShapeType;
    //shapePtr->get_AutoShapeType()
    //shapePtr->Copy();
    // const QMimeData * tmpMimeData = qApp->clipboard()->mimeData();
    //tmpShapePtr->Delete();
    // QStringList formatList =  qApp->clipboard()->mimeData()->formats();
    // if(formatList.indexOf("image/png") != -1)
    // {
    //     //qDebug()<<"shape;";
    // }
    // for(int i = 0; i < formatList.count(); ++i)
    // {
    //     QString qsFormat = formatList.at(i);
    //     QByteArray datda = tmpMimeData->data(qsFormat);
    //     //qDebug()<<"data size:" << datda.size();
    // }
    isContinue = true;
    return true;
    QImage image = qApp->clipboard()->image();
    if(image.isNull())
    {
        isContinue = true;
        return result;
    }
    QByteArray encoded = QByteArray::fromRawData((char*)image.bits(), image.sizeInBytes());
    QBuffer buf(&encoded);
    buf.open(QIODevice::WriteOnly);

    image.save(&buf, "PNG");

    ST_VarantFile varantFile;
    varantFile.fileData = buf.data();

    EU_OperateType euOperateType;
    ST_VarantFile outFileInfo;
    isContinue = imageDataFunPtr(varantFile,outFileInfo ,euOperateType);
    if(euOperateType == DeleteType)
    {
        HRESULT hr = shapePtr->Delete();
        result= OFFICE_HRESULT(hr, "delete");
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


            ks_stdptr<Shape> outShape;
            ks_bstr filePathBstr (outFileInfo.qsFilePath.utf16());
            HRESULT AddPictureHr = shapesPtr->AddPicture(filePathBstr, msoFalse, msoTrue, left, top, width, height, &outShape);
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

bool WppComment::getOldFileDataForShape(kfc::ks_stdptr<wppapi::Shapes> shapesPtr, kfc::ks_stdptr<wppapi::Shape> shapePtr, GetNextOleDataFun oldDataFunPtr, bool &isContinue)
{
    bool result = false;
    if(!oldDataFunPtr || !shapePtr || !shapesPtr)
    {
        return result;
    }

    MsoShapeType shapeType;
    shapePtr->get_Type(&shapeType);
    shapePtr->Copy();
    const QMimeData *  mdata = qApp->clipboard()->mimeData();
    if(mdata)
    {
        ST_VarantFile stOleFile;
        QStringList qsMimeDataKeyList = mdata->formats();
        QString qsMimeData;

        for(const QString& qsTmp : qsMimeDataKeyList)
        {
            if(qsTmp.contains(WPPMimeDataKey) && qsTmp.contains("Format"))
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
        ST_VarantFile outFileInfo;
        UtilityTool::GetAttachmentData(data, stOleFile, WPPFileType);

        if(!stOleFile.fileData.isEmpty())
        {
            EU_OperateType operaTye;
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

                    VARIANT missing;
                    VariantInit(&missing);
                    V_VT(&missing) = VT_ERROR;
                    V_ERROR(&missing) = DISP_E_PARAMNOTFOUND;

                    ks_bstr missBstr;
                    HRESULT addOleHr = shapesPtr->AddOLEObject(left, top, width, height, missBstr, fileNameStr, msoFalse, missBstr, 0, missBstr, msoFalse, &outShapePtr);
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

QStringList WppComment::getTableTextList(kfc::ks_stdptr<wppapi::Shape> tableShape)
{
    QStringList tableStringList;
    ks_stdptr<Table> tablePtr;
    tableShape->get_Table(&tablePtr);
    if(!tablePtr)
    {
        return tableStringList;
    }
    ks_stdptr<Rows> rowsPtr;
    ks_stdptr<Columns> columnsPtr;
    tablePtr->get_Rows(&rowsPtr);
    tablePtr->get_Columns(&columnsPtr);
    if(!rowsPtr || !columnsPtr)
    {
        return tableStringList;
    }

    long columnCount = 0;
    columnsPtr->get_Count(&columnCount);

    for(long i = 1; i <= columnCount; ++i)
    {
        ks_stdptr<Column> columnPtr;
        columnsPtr->Item(i, &columnPtr);
        if(!columnPtr)
        {
            continue;
        }
        ks_stdptr<CellRange> cellRangePtr;
        columnPtr->get_Cells(&cellRangePtr);
        if(!cellRangePtr)
        {
            continue;
        }
        long count = 0;
        cellRangePtr->get_Count(&count);
        for(int j = 1; j <= count; ++j)
        {
            ks_stdptr<Cell> cellPtr;
            cellRangePtr->Item(j, &cellPtr);
            if(!cellPtr)
            {
                continue;
            }
            ks_stdptr<Shape> shapePtr;
            cellPtr->get_Shape(&shapePtr);
            if(!shapePtr)
            {
                continue;
            }
            //暂时不考虑表中表
            ks_stdptr<TextFrame> textFramePtr;
            shapePtr->get_TextFrame(&textFramePtr);
            if(!textFramePtr)
            {
                continue;
            }
            ks_stdptr<TextRange> textRangePtr;
            textFramePtr->get_TextRange(&textRangePtr);
            if(textRangePtr)
            {
                ks_bstr text;
                textRangePtr->get_Text(&text);
                QString qsText = GetBSTRText(text);
                tableStringList.append(qsText);
            }
        }
    }

    return tableStringList;
}

QList<kfc::ks_stdptr<TextRange> > WppComment::GetTextRange()
{
    QList<kfc::ks_stdptr<wppapi::TextRange> > textRangeList;
    if(!m_spPresentation)
    {
        return textRangeList;
    }
    ks_stdptr<Slides> slidesPtr;
    m_spPresentation->get_Slides(&slidesPtr);
    if(!slidesPtr)
    {
        return textRangeList;
    }

    long slidRangeCount = 0;
    slidesPtr->get_Count(&slidRangeCount);
    for(long j = 1; j <= slidRangeCount; ++j)
    {
        VARIANT rangeIndex;
        VariantInit(&rangeIndex);
        V_VT(&rangeIndex) = VT_I4;
        V_I4(&rangeIndex) = j;

        ks_stdptr<SlideRange> range;
        slidesPtr->Range(rangeIndex, &range);
        if(!range)
        {
            continue;
        }
        ks_stdptr<Shapes> shapesPtr;
        range->get_Shapes(&shapesPtr);
        if(!shapesPtr)
        {
            continue;
        }
        int shapeCount = 0;
        shapesPtr->get_Count(&shapeCount);
        for(int q = 1; q <= shapeCount + 1; ++q)
        {
            VARIANT shapeIndex;
            VariantInit(&shapeIndex);
            V_VT(&shapeIndex) = VT_I4;
            V_I4(&shapeIndex) = q;
            ks_stdptr<Shape> shapePtr;
            shapesPtr->Item(shapeIndex, &shapePtr);
            if(!shapePtr)
            {
                continue;
            }
            QList<ks_stdptr<Shape>> shapePtrList = GetShapeGroupList<WppApiTypes>(shapePtr);
            for(int i = 0; i < shapePtrList.count(); ++i)
            {
                ks_stdptr<Shape> tmpShape = shapePtrList.at(i);
                ks_stdptr<TextFrame> textFramePtr;
                tmpShape->get_TextFrame(&textFramePtr);
                ks_stdptr<TextEffectFormat> textEffectFramePtr;
                tmpShape->get_TextEffect(&textEffectFramePtr);
                //textEffectFramePtr->get_Text()
                if(!textFramePtr)
                {
                    continue;
                }
                ks_stdptr<TextRange> textRangePtr;
                textFramePtr->get_TextRange(&textRangePtr);
                if(textRangePtr)
                {
                    textRangeList.append(textRangePtr);
                }
            }
        }
    }

    return textRangeList;
}

void WppComment::closeApp()
{
    if (m_spApplication != NULL)
    {
        SPDLOG_INFO("m_spPresentation ready close");
        if(m_spPresentation)
        {
            m_spPresentation->Close();
        }
        SPDLOG_INFO("m_spPresentation close end");
        SPDLOG_INFO("m_spApplication ready quit");
        m_spApplication->Quit();
        SPDLOG_INFO("m_spApplication quit end");
        SPDLOG_INFO("m_spDocs ready clear");
        if(m_spDocs)
        {
            m_spDocs.clear();
        }
        SPDLOG_INFO("m_spDocs clear end");
        //m_spApplicationEx.clear();
        SPDLOG_INFO("m_spApplication ready clear");
        m_spApplication.clear();
        SPDLOG_INFO("m_spApplication clear end ");
        m_rpcClient = nullptr;
        SPDLOG_INFO("m_rpcClient compare null ");
        if(m_containWidget)
        {
            delete m_containWidget;
            m_containWidget = nullptr;
        }
    }
}

bool WppComment::saveWPPDoc()
{
    HRESULT hr = m_spPresentation->Save();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    return true;
}
