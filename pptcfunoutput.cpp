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
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifdef __cplusplus
extern "C" {
#endif
QString tmpImagePath;
int gloabalIndex = 0;
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

void killWppProcess()
{
    // 查找所有名为 wpp 的进程
    QProcess psProcess;
    psProcess.start("pgrep", QStringList() << "wpp");
    psProcess.waitForFinished();
    QByteArray output = psProcess.readAllStandardOutput();
    QList<QByteArray> pidList = output.split('\n');

    for (const QByteArray& pid : pidList) {
        bool ok = false;
        int pidInt = pid.trimmed().toInt(&ok);
        if (ok && pidInt > 0) {
            // 杀死进程
            QProcess::execute("kill", QStringList() << "-9" << QString::number(pidInt));
        }
    }
}

void pptCFunOutput(const char* inputPPTFileDir, const char* outputPPTFileDir)
{
    static QGuiApplication* app = nullptr;
    static int argc = 0;
    static char* argv[] = {nullptr};

    bool isNeedFreeAppplication = false;
    if (!QCoreApplication::instance())
    {
        // 第一次调用时自动创建 QGuiApplication
        app = new QGuiApplication(argc, argv);
        isNeedFreeAppplication = true;
    }
    QString str = QString::fromUtf8(inputPPTFileDir);
    QString outstr = QString::fromUtf8(outputPPTFileDir);
    QDir dir(str);
    if(dir.exists())
    {
        killWppProcess();
        WppComment wpp;
        wpp.initWPPRpcClient();
        wpp.initWppApplication();
        QStringList filterStrList;
        filterStrList.append("*.dps");
        filterStrList.append("*.ppt");
        filterStrList.append("*.pptx");

        QFileInfoList wppFileInfoList = dir.entryInfoList(filterStrList ,QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        QElapsedTimer time;
        time.start();
        for(int i = 0; i < wppFileInfoList.count(); ++i)
        {
            QFileInfo fileInfo = wppFileInfoList.at(i);
            QString qsAbsoluteFilePath = fileInfo.absoluteFilePath();
            QString qsOutputFileName = fileInfo.fileName();
            QString destPath = outstr + "/" + qsOutputFileName + "_out";
            QDir destDir(destPath);
            if(destDir.exists())
            {
                destDir.removeRecursively();
            }

            if(!destDir.exists())
            {
                destDir.mkpath(destPath);
            }
            if(!destDir.exists())
            {
                break;
            }

            wpp.openWPPDoc(qsAbsoluteFilePath);
            //wpp.GetBeiZhu();
            QStringList qsTextList = wpp.GetWPPText();
            QFile file(destPath + "/content" + ".txt");
            if(file.open(QIODevice::WriteOnly))
            {
                QString qsText = qsTextList.join("\r\n");
                file.write(qsText.toUtf8());
                file.close();
            }
            tmpImagePath = destPath + "/content";
            wpp.extractPicture(TestPicture);
            gloabalIndex = 0;
            wpp.closeWPPDoc();
        }
        wpp.closeApp();
        qint64 userd = time.elapsed();
        qDebug()<<"run time:<<<<" << userd<<" ms";
    }
    if(isNeedFreeAppplication)
    {
        app->deleteLater();
    }
}

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QFile file("officePPT.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("[%1] [%2] Debug: %3").arg(time, context.function, msg);
        break;
    case QtWarningMsg:
        text = QString("[%1] [%2] Warning: %3").arg(time, context.function, msg);
        break;
    case QtCriticalMsg:
        text = QString("[%1] [%2] Critical: %3").arg(time, context.function, msg);
        break;
    case QtFatalMsg:
        text = QString("[%1] [%2] Fatal: %3").arg(time, context.function, msg);
        break;
    default:
        text = QString("[%1] [%2] Info: %3").arg(time, context.function, msg);
        break;
    }
    out<< text <<endl;
    file.close();
}

//int fun_singlefile(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir)
//{
//    //    //qInstallMessageHandler(myMessageOutput);
//    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
//    QFileInfo inputPathInfo(qsInputfilepath);
//    QStringList filterStrList;
//    filterStrList.append("dps");
//    filterStrList.append("ppt");
//    filterStrList.append("pptx");

//    if(!inputPathInfo.exists())
//    {
//        //qDebug()<<"file is No exists";
//        return 1;
//    }
//    if(filterStrList.indexOf(inputPathInfo.suffix()) < 0)
//    {
//        //qDebug()<<inputPathInfo.suffix();
//        //qDebug()<<"file type is error";
//        return 1;
//    }
//    if(!outfilepath)
//    {
//        return 1;
//    }
//    if(!imagedir)
//    {
//        return 1;
//    }

//    QString qsRootpath = QString::fromUtf8(rootpath);
//    QDir dir(qsRootpath);
//    if(!dir.exists())
//    {
//        //qDebug()<<"floder no exists try create floder";
//        bool isSuccessful = dir.mkpath(qsRootpath);
//        if(!isSuccessful)
//        {
//            //qDebug()<<"create failed";
//            return 1;
//        }
//    }
//    if(qsRootpath.right(1) == "/")
//    {
//        qsRootpath = qsRootpath.left(qsRootpath.length() - 1);
//    }
//    QString qsFileTextOutDir = qsRootpath + "/outText/" + inputPathInfo.fileName();
//    QString qsFileTextOutFile = qsFileTextOutDir + "/content.txt";
//    QString qsFileImageOutDir = qsRootpath + "/outImage/" + inputPathInfo.fileName();
//    //QString qsFileImOutFilePrefix = qsRootpath + "/outImage/" + inputPathInfo.fileName();
//    tmpImagePath = qsFileImageOutDir + "/";

//    QDir fileTextOutDir(qsFileTextOutDir);
//    if(fileTextOutDir.exists())
//    {
//        //qDebug()<<qsFileTextOutDir<<": existed ready delete";
//        if(fileTextOutDir.removeRecursively())
//        {
//            //qDebug()<<qsFileTextOutDir<<": delete successful";
//        }
//    }

//    if(fileTextOutDir.mkpath(qsFileTextOutDir))
//    {
//        //qDebug()<<qsFileTextOutDir<<": create successful";
//    }

//    QDir fileImageOutDir(qsFileImageOutDir);
//    if(fileImageOutDir.exists())
//    {
//        //qDebug()<<qsFileImageOutDir<<": existed ready delete";
//        if(fileImageOutDir.removeRecursively())
//        {
//            //qDebug()<<qsFileImageOutDir<<": delete successful";
//        }
//    }
//    if(fileImageOutDir.mkpath(qsFileImageOutDir))
//    {
//        //qDebug()<<qsFileImageOutDir<<": create successful";
//    }
//    qsFileTextOutFile.toUtf8().size();
//    memcpy(outfilepath, qsFileTextOutFile.toUtf8().data(), qsFileTextOutFile.toUtf8().size());
//    memcpy(imagedir, qsFileImageOutDir.toUtf8().data(), qsFileImageOutDir.toUtf8().size());

//    static QGuiApplication* app = nullptr;
//    static int argc = 0;
//    static char* argv[] = {nullptr};

//    bool isNeedFreeAppplication = false;
//    if (!QCoreApplication::instance())
//    {
//        // 第一次调用时自动创建 QGuiApplication
//        app = new QGuiApplication(argc, argv);
//        isNeedFreeAppplication = true;
//    }

//    WppComment wpp;
//    wpp.initWPPRpcClient();
//    wpp.initWppApplication();

////    QElapsedTimer time;
////    time.start();
////    for(int i = 0; i < 1500; ++i)
////    {
//       // qDebug()<< "outPut==================:"<<QString::number(i);
//        if(wpp.openWPPDoc(qsInputfilepath))
//        {
//            //qDebug()<<"file open successful";
//            QStringList qsTextList = wpp.GetWPPText();
//            QStringList qsrearksStringList = wpp.getRearksText();

//            QFile file(qsFileTextOutFile);
//            if(file.open(QIODevice::WriteOnly))
//            {
//                QString qsText = qsTextList.join("\r\n");
//                QString qsTextReark = qsrearksStringList.join("\r\n");
//                qsText = qsText + qsTextReark;
//                file.write(qsText.toUtf8());
//                file.close();
//            }

//            gloabalIndex = 0;

//            wpp.extractPictureNomemery(qsFileImageOutDir);
//            //wpp.extractPicture(TestPicture);
//            wpp.closeWPPDoc();
//        }
//        else
//        {
//            qFatal("file open fatal");
//        }
////    }

////    qint64 userd = time.elapsed();
////    qDebug()<<"run time:<<<<" << userd<<" ms";
//    wpp.closeApp();

//    //qint64 userd = time.elapsed();
//    // qDebug()<<"run time:<<<<" << userd<<" ms";
//    if(isNeedFreeAppplication)
//    {
//        app->deleteLater();
//    }
//    return 0;
//}

bool isNeedFreeAppplication = false;
static QGuiApplication* app = nullptr;

void initWPP(WPPHANDLE *wppObj)
{
    WppComment *wpp =new WppComment;
    wpp->initWPPRpcClient();
    wpp->initWppApplication();
    *wppObj = wpp;
    spdlog::init_thread_pool(8192, 1); // 队列8192条，1个后台线程
    std::shared_ptr<spdlog::logger> loger = spdlog::rotating_logger_mt<spdlog::async_factory>("com_loger", "logs/app.log", 1048576 *5 , 3);
//    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("/home/ft2000/hmg0815/app.log", 1048576 * 5, 3);

    // 创建 logger 并保存到变量
//    spdlog::sinks_init_list sinks{console_sink, file_sink};
//    auto loger = std::make_shared<spdlog::logger>(
//        "multi_sink",sinks
//    );
    //spdlog::logger loger("multi_sink", {console_sink, file_sink});
    spdlog::set_default_logger(loger);

    loger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
    spdlog::set_default_logger(loger);
    //spdlog::set_level(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(3));
    spdlog::set_level(spdlog::level::trace);

    static int argc = 0;
    static char* argv[] = {nullptr};


    if (!QCoreApplication::instance())
    {
        // 第一次调用时自动创建 QGuiApplication
        app = new QGuiApplication(argc, argv);
        isNeedFreeAppplication = true;
    }
}

void closeWPP(WPPHANDLE wppObj)
{
    if(wppObj)
    {
        WppComment *wpp = (WppComment*)wppObj;
        wpp->closeApp();
        delete wpp;
        spdlog::shutdown();
        if(isNeedFreeAppplication)
        {
            app->deleteLater();
        }
    }
}

int initExtractFloder(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir)
{
    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
    QFileInfo inputPathInfo(qsInputfilepath);
    if(!inputPathInfo.exists())
    {
        SPDLOG_ERROR("file is No exists");
        return 0;
    }
    if(!outfilepath)
    {
        return 0;
    }
    if(!imagedir)
    {

        return 0;
    }
    QString qsRootpath = QString::fromUtf8(rootpath);
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

    if(qsRootpath.right(1) == "/")
    {
        qsRootpath = qsRootpath.left(qsRootpath.length() - 1);
    }
    QString qsFileTextOutDir = qsRootpath + "/outText/" + inputPathInfo.fileName();
    QString qsFileTextOutFile = qsFileTextOutDir + "/content.txt";
    QString qsFileImageOutDir = qsRootpath + "/outImage/" + inputPathInfo.fileName();
    //QString qsFileImOutFilePrefix = qsRootpath + "/outImage/" + inputPathInfo.fileName();

    QDir fileTextOutDir(qsFileTextOutDir);
    if(fileTextOutDir.exists())
    {
        QString qslog = "path:" + qsFileTextOutDir +" existed ready delete";
        SPDLOG_INFO(qslog.toUtf8().data());
        //qDebug()<<qsFileTextOutDir<<": existed ready delete";
        if(!fileTextOutDir.removeRecursively())
        {
            SPDLOG_ERROR("delete failed");
            //qDebug()<<qsFileTextOutDir<<": delete failed";
            return 0;
        }
        else
        {
            SPDLOG_INFO("delete successful");
            //qDebug()<<qsFileTextOutDir<<": delete successful";
        }
    }

    if(!fileTextOutDir.mkpath(qsFileTextOutDir))
    {
        QString qslog = "path:" + qsFileTextOutDir +" create failed";
        SPDLOG_ERROR(qslog.toUtf8().data());
        //qDebug()<<qsFileTextOutDir<<": create failed";
        return 0;
    }

    SPDLOG_INFO(QString("path:" + qsFileTextOutDir +" create successful").toUtf8().data());
    //qDebug()<<qsFileTextOutDir<<": create successful";
    QDir fileImageOutDir(qsFileImageOutDir);
    if(fileImageOutDir.exists())
    {
        SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" existed ready delete").toUtf8().data());
        //qDebug()<<qsFileImageOutDir<<": existed ready delete";
        if(!fileImageOutDir.removeRecursively())
        {
            SPDLOG_ERROR(QString("path:" + qsFileImageOutDir +"  delete failed").toUtf8().data());
            //qDebug()<<qsFileImageOutDir<<": delete failed";
            return 0;
        }
        else
        {
            SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" delete successful").toUtf8().data());

            //qDebug()<<qsFileImageOutDir<<": delete successful";
        }
    }
    if(!fileImageOutDir.mkpath(qsFileImageOutDir))
    {
        SPDLOG_ERROR(QString("path:" + qsFileImageOutDir +" create failed").toUtf8().data());
        return 0;
        //qDebug()<<qsFileImageOutDir<<": create failed";
    }
    SPDLOG_INFO(QString("path:" + qsFileImageOutDir +" create successful").toUtf8().data());

    //qDebug()<<qsFileImageOutDir<<": create successful";

    memcpy(outfilepath, qsFileTextOutFile.toUtf8().data(), qsFileTextOutFile.toUtf8().size());
    memcpy(imagedir, qsFileImageOutDir.toUtf8().data(), qsFileImageOutDir.toUtf8().size());
    return 1;
}

int extractImageAndeText(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir, WPPHANDLE wppObj)
{
    if(!wppObj)
    {
        SPDLOG_ERROR("HANDLE NO INIT");
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
    int result = initExtractFloder(inputfilepath, rootpath, outfilepath, imagedir);
    if(result == 0)
    {
        SPDLOG_INFO("floder init failed");
        return 0;
    }
    WppComment* wpp = (WppComment*)wppObj;
    if(wpp->openWPPDoc(qsInputfilepath))
    {
        SPDLOG_INFO(QString("file:" + qsInputfilepath + "open successful").toUtf8().data());
        //qDebug()<<"file open successful";
        QStringList qsTextList = wpp->GetWPPText();
        QStringList qsrearksStringList = wpp->getRearksText();
        QString qsFileTextOutFile = QString::fromUtf8(outfilepath);
        QFile file(qsFileTextOutFile);
        if(file.open(QIODevice::WriteOnly))
        {
            QString qsText = qsTextList.join("\r\n");
            QString qsTextReark = qsrearksStringList.join("\r\n");
            qsText = qsText + qsTextReark;
            file.write(qsText.toUtf8());
            file.close();
            SPDLOG_INFO(QString(qsFileTextOutFile + ":text exported").toUtf8().data());
        }

        QString qsFileImageOutDir = QString::fromUtf8(imagedir);
        wpp->extractPictureNomemery(qsFileImageOutDir);
        SPDLOG_INFO(QString(qsFileImageOutDir +":image exported").toUtf8().data());
        //wpp.extractPicture(TestPicture);
        wpp->closeWPPDoc();
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
