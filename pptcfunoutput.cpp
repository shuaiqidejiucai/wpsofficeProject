#include "pptcfunoutput.h"

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QElapsedTimer>
#include "utilitytool.h"
#include "wppcomment.h"
#include <QApplication>
#include <QDir>
#include <QStack>
#include <QMutex>
#include <QTextStream>
#include <QDateTime>
#include <QStringList>
#include <log_global.h>
#include <QtEndian>
#include "etcomment.h"
#ifdef __cplusplus
extern "C" {
#endif

QString tmpOlePath;
QString tmpImagePath;
int gloabalIndex = 0;
static bool TestOleFile(ST_VarantFile varInFile, ST_VarantFile& varOutFile, EU_OperateType& operateType)
{
    operateType = NoOperateType;
    QByteArray ba = varInFile.fileData;
    if(ba.isEmpty())
    {
        return true;
    }

    QFile file(tmpOlePath + "/" +QString::number(gloabalIndex) + varInFile.qsFileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(ba);
        file.close();
        gloabalIndex++;
    }

    return true;
}

static bool TestPicture(ST_VarantFile varInFile, ST_VarantFile& varOutFile, EU_OperateType& operateType)
{
    operateType = NoOperateType;
    QByteArray ba = varInFile.fileData;
    QFile file(tmpImagePath + QString::number(gloabalIndex) + ".png");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(ba);
        file.close();
        gloabalIndex++;
    }
    //    QImage image = QImage::fromData(ba);
    //    QLabel *label = new QLabel;
    //    label->setPixmap(QPixmap::fromImage(image));
    //    label->show();
    return true;
}

bool isNeedFreeAppplication = false;
static QApplication* app = nullptr;

void initWPP(WPPHANDLE *wppObj)
{
    static int argc = 0;
    static char* argv[] = {nullptr};
    QDir dir("logs");
    dir.mkpath(dir.absolutePath());
    spdlog::init_thread_pool(512, 2); // 队列8192条，1个后台线程
    std::shared_ptr<spdlog::logger> loger = spdlog::rotating_logger_mt<spdlog::async_factory>("com_loger", "logs/app.log", 1048576 *5 , 3);
    loger->flush_on(spdlog::level::trace);
    loger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
    spdlog::set_default_logger(loger);
    //spdlog::set_level(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::set_level(spdlog::level::trace);
    if (!QApplication::instance())
    {
        // 第一次调用时自动创建 QGuiApplication
        app = new QApplication(argc, argv);
        isNeedFreeAppplication = true;
        SPDLOG_WARN("QApplication create");
    }
    WppComment *wpp =new WppComment;
    wpp->initWPPRpcClient();
    SPDLOG_INFO("initWPPRpcClient ok");
    wpp->initWppApplication();
    *wppObj = wpp;
    SPDLOG_INFO("initWppApplication ok");
}

void closeWPP(WPPHANDLE wppObj)
{
    if(wppObj)
    {
        WppComment *wpp = (WppComment*)wppObj;
        wpp->closeApp();
        SPDLOG_INFO("wpp close ok");
        delete wpp;
        SPDLOG_INFO("delete wppCommnt");

        if(isNeedFreeAppplication)
        {
            app->deleteLater();
            SPDLOG_WARN("QApplication release");
        }
        SPDLOG_INFO("log release...");
        spdlog::shutdown();
    }
}

int initExtractFloder(const char *inputfilepath, const char *rootpath, ST_OutFilePath * pOutFilePath, ExtratorElementType elementType)
{
    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
    QFileInfo inputPathInfo(qsInputfilepath);
    if(!inputPathInfo.exists())
    {
        SPDLOG_ERROR("file is No exists");
        return 0;
    }
    QString qsRootpath = QString::fromUtf8(rootpath);
    if(qsRootpath.right(1) == "/")
    {
        qsRootpath = qsRootpath.left(qsRootpath.length() - 1);
    }
    QDir dir(qsRootpath);
    if(!dir.exists())
    {
        SPDLOG_INFO("floder no exists try create floder");
        //qDebug()<<"floder no exists try create floder";
        bool isSuccessful = dir.mkpath(qsRootpath);
        if(!isSuccessful)
        {
            QString qslog = "path:" + qsRootpath +" create failed";
            //qDebug()<<"create failed";
            SPDLOG_ERROR(qslog.toUtf8().data());
            return 0;
        }
    }

    if(elementType & TextElementType)
    {
        if(!pOutFilePath->textFilePath)
        {
            SPDLOG_ERROR("outfilepath is nullptr");
            return 0;
        }
        QString qsFileTextOutDir = qsRootpath + "/outText/" + inputPathInfo.fileName();
        QString qsFileTextOutFile = qsFileTextOutDir + "/content.txt";
        QDir fileTextOutDir(qsFileTextOutDir);
        if(fileTextOutDir.exists())
        {
            QString qslog = "path:" + qsFileTextOutDir +" existed ready delete";
            SPDLOG_INFO(qslog.toUtf8().data());
            if(!fileTextOutDir.removeRecursively())
            {
                SPDLOG_ERROR("delete failed");
                return 0;
            }
            else
            {
                SPDLOG_INFO("delete successful");
            }
        }

        if(!fileTextOutDir.mkpath(qsFileTextOutDir))
        {
            QString qslog = "path:" + qsFileTextOutDir +" create failed";
            SPDLOG_ERROR(qslog.toUtf8().data());
            return 0;
        }

        SPDLOG_INFO(QString("path:" + qsFileTextOutDir +" create successful").toUtf8().data());
        memcpy(pOutFilePath->textFilePath, qsFileTextOutFile.toUtf8().data(), qsFileTextOutFile.toUtf8().size());
    }
    if(elementType & ImageElementType)
    {
        if(!pOutFilePath->imageDir)
        {
            SPDLOG_ERROR("imagedir is nullptr");
            return 0;
        }
        QString qsFileImageOutDir = qsRootpath + "/outImage/" + inputPathInfo.fileName();
        QDir fileImageOutDir(qsFileImageOutDir);
        if(fileImageOutDir.exists())
        {
            SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" existed ready delete").toUtf8().data());
            if(!fileImageOutDir.removeRecursively())
            {
                SPDLOG_ERROR(QString("path:" + qsFileImageOutDir +"  delete failed").toUtf8().data());
                return 0;
            }
            else
            {
                SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" delete successful").toUtf8().data());
            }
        }
        if(!fileImageOutDir.mkpath(qsFileImageOutDir))
        {
            SPDLOG_ERROR(QString("path:" + qsFileImageOutDir +" create failed").toUtf8().data());
            return 0;
        }
        SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" create successful").toUtf8().data());

        //qDebug()<<qsFileImageOutDir<<": create successful";


        memcpy(pOutFilePath->imageDir, qsFileImageOutDir.toUtf8().data(), qsFileImageOutDir.toUtf8().size());
    }
    if(elementType & AttachmentElementType)
    {
        if(!pOutFilePath->attachmentDir)
        {
            SPDLOG_ERROR("attachmentdir is nullptr");
            return 0;
        }
        QString qsAttachmentDir = qsRootpath + "/outAttachment/" + inputPathInfo.fileName();
        QDir fileOutDir(qsAttachmentDir);
        if(fileOutDir.exists())
        {
            SPDLOG_INFO(QString("path:" + qsAttachmentDir +" existed ready delete").toUtf8().data());
            if(!fileOutDir.removeRecursively())
            {
                SPDLOG_ERROR(QString("path:" + qsAttachmentDir +"  delete failed").toUtf8().data());
                return 0;
            }
            else
            {
                SPDLOG_INFO(QString("path:" + qsAttachmentDir +" delete successful").toUtf8().data());
            }
        }
        if(!fileOutDir.mkpath(qsAttachmentDir))
        {
            SPDLOG_ERROR(QString("path:" + qsAttachmentDir +" create failed").toUtf8().data());
            return 0;
        }
        SPDLOG_INFO(QString("path:" + qsAttachmentDir +" create successful").toUtf8().data());

        memcpy(pOutFilePath->attachmentDir, qsAttachmentDir.toUtf8().data(), qsAttachmentDir.toUtf8().size());
    }


    return 1;
}

int extractElement(const char *inputfilepath, const char *rootpath, ST_OutFilePath*pSTOutFilePath, WPPHANDLE wppObj, ExtratorElementType elementType)
{
    if(!wppObj)
    {
        SPDLOG_ERROR("HANDLE NO INIT");
        return 0;
    }
    if(!pSTOutFilePath)
    {
        SPDLOG_ERROR("ST_OutFilePath Is NULL");
        return 0;
    }
    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
    QFileInfo inputPathInfo(qsInputfilepath);
    QStringList filterStrList;
    filterStrList.append("dps");
    filterStrList.append("ppt");
    filterStrList.append("pptx");

    if(filterStrList.indexOf(inputPathInfo.suffix()) < 0)
    {
        //qDebug()<<inputPathInfo.suffix();
        //qDebug()<<"file type is error";
        SPDLOG_ERROR("file type is error");
        return 0;
    }
    int result = initExtractFloder(inputfilepath, rootpath, pSTOutFilePath, elementType);
    if(result == 0)
    {
        SPDLOG_INFO("floder init failed");
        return 0;
    }
    WppComment* wpp = (WppComment*)wppObj;
    if(wpp->openWPPDoc(qsInputfilepath))
    {
        SPDLOG_INFO(QString("file:" + qsInputfilepath + "open successful").toUtf8().data());
        if(elementType & TextElementType)
        {
            QStringList qsTextList = wpp->GetWPPText();
            QStringList qsrearksStringList = wpp->getRearksText();
            QStringList qsMasterList = wpp->getMasterText();
            QStringList qsLayoutList = wpp->getPPLayoutText();
            QString qsFileTextOutFile = QString::fromUtf8(pSTOutFilePath->textFilePath);
            QFile file(qsFileTextOutFile);
            if(file.open(QIODevice::WriteOnly))
            {
                QString qsLayoutText = qsLayoutList.join("\r\n");
                QString qsText = "#main body#" + qsTextList.join("\r\n") + "#main body#";
                QString qsTextReark = qsrearksStringList.join("\r\n");
                QString qsMaterText = qsMasterList.join("\r\n");
                qsText = qsLayoutText + qsMaterText + qsText + qsTextReark + qsMaterText;
                file.write(qsText.toUtf8());
                file.close();
                SPDLOG_INFO(QString(qsFileTextOutFile + ":text exported").toUtf8().data());
            }
        }

        if(elementType & ImageElementType)
        {
            QString qsFileImageOutDir = QString::fromUtf8(pSTOutFilePath->imageDir);
            wpp->extractPictureNomemery(qsFileImageOutDir);
            SPDLOG_INFO(QString(qsFileImageOutDir +":image exported").toUtf8().data());
        }

        if(elementType & AttachmentElementType)
        {
            QString qsAttachmentDir = QString::fromUtf8(pSTOutFilePath->attachmentDir);
            tmpOlePath = qsAttachmentDir;
            gloabalIndex = 0;
            wpp->getOleFileData(TestOleFile);
            tmpOlePath.clear();
            SPDLOG_INFO(QString(qsAttachmentDir +":attachment exported").toUtf8().data());
        }

        //wpp.extractPicture(TestPicture);
        wpp->closeWPPDoc();
    }
    else
    {
        SPDLOG_ERROR("file open fatal");
    }
    return 1;
}

void initEt(ETHANDLE *etObj)
{
    static int argc = 0;
    static char* argv[] = {nullptr};
    QDir dir("logs");
    dir.mkpath(dir.absolutePath());
    spdlog::init_thread_pool(512, 2); // 队列8192条，1个后台线程
    std::shared_ptr<spdlog::logger> loger = spdlog::rotating_logger_mt<spdlog::async_factory>("com_loger", "logs/app.log", 1048576 *5 , 3);
    loger->flush_on(spdlog::level::trace);
    loger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
    spdlog::set_default_logger(loger);
    //spdlog::set_level(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::set_level(spdlog::level::trace);
    if (!QApplication::instance())
    {
        // 第一次调用时自动创建 QGuiApplication
        app = new QApplication(argc, argv);
        isNeedFreeAppplication = true;
        SPDLOG_WARN("QApplication create");
    }
    EtComment *wpp =new EtComment;
    wpp->initEtRpcClient();
    SPDLOG_INFO("iniEtRpcClient ok");
    wpp->initEtApplication();
    *etObj = wpp;
    SPDLOG_INFO("initEtApplication ok");
}

void closeEt(ETHANDLE etObj)
{
    if(etObj)
    {
        EtComment *wpp = (EtComment*)etObj;
        wpp->closeApp();
        SPDLOG_INFO("wpp close ok");
        delete wpp;
        SPDLOG_INFO("delete wppCommnt");

        if(isNeedFreeAppplication)
        {
            app->deleteLater();
            SPDLOG_WARN("QApplication release");
        }
        SPDLOG_INFO("log release...");
        spdlog::shutdown();
    }
}

int extractEtElement(const char *inputfilepath, const char *rootpath, ST_OutFilePath *pSTOutFilePath, ETHANDLE etObj, ExtratorElementType elementType)
{
    if(!etObj)
    {
        SPDLOG_ERROR("HANDLE NO INIT");
        return 0;
    }
    if(!pSTOutFilePath)
    {
        SPDLOG_ERROR("ST_OutFilePath Is NULL");
        return 0;
    }
    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
    QFileInfo inputPathInfo(qsInputfilepath);
    QStringList filterStrList;
    filterStrList.append("et");
    filterStrList.append("xlsx");
    filterStrList.append("xls");

    if(filterStrList.indexOf(inputPathInfo.suffix()) < 0)
    {
        //qDebug()<<inputPathInfo.suffix();
        //qDebug()<<"file type is error";
        SPDLOG_ERROR("file type is error");
        return 0;
    }
    int result = initExtractFloder(inputfilepath, rootpath, pSTOutFilePath, elementType);
    if(result == 0)
    {
        SPDLOG_INFO("floder init failed");
        return 0;
    }
    EtComment* wpp = (EtComment*)etObj;
    if(wpp->openEtDoc(qsInputfilepath))
    {
        SPDLOG_INFO(QString("file:" + qsInputfilepath + "open successful").toUtf8().data());

        if(true/*elementType & AttachmentElementType*/)
        {
            QString qsAttachmentDir = QString::fromUtf8(pSTOutFilePath->attachmentDir);
            tmpOlePath = qsAttachmentDir;
            gloabalIndex = 0;
            int sheetCount = wpp->getSheetCount();
            for(int i = 1; i <= sheetCount; ++i)
            {
                wpp->getOleFileData(i, TestOleFile);
                tmpOlePath.clear();
            }


            SPDLOG_INFO(QString(qsAttachmentDir +":attachment exported").toUtf8().data());
        }

        //wpp.extractPicture(TestPicture);
        wpp->closeEtDoc();
    }
    else
    {
        SPDLOG_ERROR("file open fatal");
    }
    return 1;
}

#ifdef __cplusplus
}
#endif



