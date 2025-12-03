#include "mainwindow.h"

#include <QApplication>
#include "wppcomment.h"
#include "wpscomment.h"
#include "etcomment.h"
#include <QDebug>
#include "utilitytool.h"
#include <log_global.h>
#include <QLabel>
#include <QDir>
#include <QStack>

#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QElapsedTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSettings>

QString tmpAllPath;
int gloabalIndex = 0;
static bool TestPicture(ST_VarantFile varInFile, ST_VarantFile& varOutFile, EU_OperateType& operateType)
{
    operateType = NoOperateType;
    QByteArray ba = varInFile.fileData;
    QFile file(tmpAllPath + QString::number(gloabalIndex) + ".png");
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

// 检查并杀死名为 "wpp" 的进程
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

int extractImageAndeText(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir, WppComment& wpp)
{
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
    if(wpp.openWPPDoc(qsInputfilepath))
    {
        SPDLOG_INFO(QString("file:" + qsInputfilepath + "open successful").toUtf8().data());
        //qDebug()<<"file open successful";
        QStringList qsTextList = wpp.GetWPPText();
        QStringList qsrearksStringList = wpp.getRearksText();
        QStringList qsMasterList = wpp.getMasterText();
        QString qsFileTextOutFile = QString::fromUtf8(outfilepath);
        QFile file(qsFileTextOutFile);
        if(file.open(QIODevice::WriteOnly))
        {
            QString qsText = qsTextList.join("\r\n");
            QString qsTextReark = qsrearksStringList.join("\r\n");
            QString qsMaterText = qsMasterList.join("\r\n");
            qsText = qsText + qsTextReark + qsMaterText;
            file.write(qsText.toUtf8());
            file.close();
            SPDLOG_INFO(QString(qsFileTextOutFile + ":text exported").toUtf8().data());
        }

        QString qsFileImageOutDir = QString::fromUtf8(imagedir);
        wpp.extractPictureNomemery(qsFileImageOutDir);
        SPDLOG_INFO(QString(qsFileImageOutDir +":image exported").toUtf8().data());
        //wpp.extractPicture(TestPicture);
        wpp.closeWPPDoc();
    }
    else
    {
        SPDLOG_ERROR("file open fatal");
    }
    return 1;
}


void killEtProcess()
{
    // 查找所有名为 wpp 的进程
    QProcess psProcess;
    psProcess.start("pgrep", QStringList() << "et");
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //EtComment::extractPictureNomemery("/home/user/mjc/dps-ppt/bugwenjian/et2.et", "");
    killEtProcess();
    // EtComment etCom;
    // etCom.initEtRpcClient();
    // etCom.initEtApplication();
    //etCom.openEtDoc("/home/user/mjc/dps-ppt/bugwenjian/et2.et");
    //etCom.extractPictureNomemery("/home/user/mjc/dps-ppt/bugwenjian/et2.et");
    // etCom.closeEtDoc();
    // etCom.closeApp();
    EtComment::extractPictureNomemery("/home/user/mjc/dps-ppt/bugwenjian/et1.xls","");
    a.exec();

    QSettings settings("netconfig.ini", QSettings::IniFormat);
    QString host = settings.value("network/host", "localhost").toString();
    int port = settings.value("network/port", 12345).toInt();

    QTcpServer tcpSever;
   if (tcpSever.listen(QHostAddress::Any, port))
   {
       qDebug()<<"connect";
   }
    QObject::connect(&tcpSever, &QTcpServer::newConnection, [&](){
        QTcpSocket *client = tcpSever.nextPendingConnection();
        QObject::connect(client, &QTcpSocket::readyRead, [client]() {
            QByteArray data = client->readAll();
            QString allPath = QString::fromUtf8(data);
            QStringList textGroupList = allPath.split("||");
            if(textGroupList.count() == 2)
            {
                QString qsInputFilePath = textGroupList.first();
                QString qsRootpath = textGroupList.last();
                WppComment wpp;
                wpp.initWPPRpcClient();
                wpp.initWppApplication();
                spdlog::init_thread_pool(8192, 1); // 队列8192条，1个后台线程
                std::shared_ptr<spdlog::logger> loger = spdlog::rotating_logger_mt<spdlog::async_factory>("com_loger", "logs/app.log", 1048576 *5 , 3);
                spdlog::set_default_logger(loger);

                loger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
                spdlog::set_default_logger(loger);
                //spdlog::set_level(spdlog::level::info);
                spdlog::flush_every(std::chrono::seconds(3));
                spdlog::set_level(spdlog::level::trace);

                char textCh[2048]= {0};
                char imageCh[2048] = {0};
                extractImageAndeText(qsInputFilePath.toUtf8().constData(), qsRootpath.toUtf8().constData(),textCh, imageCh, wpp);

                QString qsClientPath = QString::fromUtf8(textCh) + "||" + QString::fromUtf8(imageCh);
                client->write(qsClientPath.toUtf8());
                client->flush();

                wpp.closeApp();
                spdlog::shutdown();
            }
            //client->write("Echo: " + data); // 回显
        });
        QObject::connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
    });
    a.exec();

    QDir dir("/home/ft2000/mjcenv/dps-ppt");

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
        for(int i = 0; i < wppFileInfoList.count(); ++i)
        {
            QFileInfo fileInfo = wppFileInfoList.at(i);
            QString qsAbsoluteFilePath = fileInfo.absoluteFilePath();
            QString destPath = qsAbsoluteFilePath + "_out";
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
//            QStringList qsTextList = wpp.GetWPPText();
//            QFile file(destPath + "/content" + ".txt");
//            if(file.open(QIODevice::WriteOnly))
//            {
//                QString qsText = qsTextList.join("\r\n");
//                file.write(qsText.toUtf8());
//                file.close();
//            }
//            tmpAllPath = destPath + "/content";

            wpp.extractPictureNomemery(destPath);

            //wpp.extractPicture(TestPicture);
            gloabalIndex = 0;
            wpp.closeWPPDoc();


        }
        wpp.closeApp();

        //qDebug()<<"run time:<<<<" << userd<<" ms";
    }

    // WppComment wpp;
    // wpp.initWPPRpcClient();
    // wpp.initWppApplication();
    // wpp.openWPPDoc("/mnt/hgfs/vmshare/8.dps");
    // wpp.extractPicture(TestPicture);



   // //qDebug()<< wpp.GetWPPText();
    //wpp.replaceTextForWPPDoc(u8"神马玩意儿", "");
    //wpp.saveWPPDoc();
    //wpp.closeApp();

    // WpsComment wpscom;
    // wpscom.initWpsRpcClient();
    // wpscom.initWpsApplication();
    // wpscom.openWPSDoc("/mnt/hgfs/vmshare/9.doc");
    // //wpscom.testChangeImage();
    // wpscom.saveWPSDoc();
    // wpscom.closeApp();

    // EtComment etCom;
    // etCom.initEtRpcClient();
    // etCom.initEtApplication();

   // return a.exec();
    return 0;
}
