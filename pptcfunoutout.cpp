#include "pptcfunoutout.h"

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

int fun_singlefile(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir)
{
    QString qsInputfilepath = QString::fromUtf8(inputfilepath);
    QFileInfo inputPathInfo(qsInputfilepath);
    QStringList filterStrList;
    filterStrList.append("dps");
    filterStrList.append("ppt");
    filterStrList.append("pptx");

    if(!inputPathInfo.exists())
    {
        qDebug()<<"file is No exists";
        return 1;
    }
    if(filterStrList.indexOf(inputPathInfo.suffix()) < 0)
    {
        qDebug()<<inputPathInfo.suffix();
        qDebug()<<"file type is error";
        return 1;
    }
    if(!outfilepath)
    {
        return 1;
    }
    if(!imagedir)
    {
        return 1;
    }

    QString qsRootpath = QString::fromUtf8(rootpath);
    QDir dir(qsRootpath);
    if(!dir.exists())
    {
        qDebug()<<"floder no exists try create floder";
        bool isSuccessful = dir.mkpath(qsRootpath);
        if(!isSuccessful)
        {
            qDebug()<<"create failed";
            return 1;
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
    tmpImagePath = qsFileImageOutDir + "/";

    QDir fileTextOutDir(qsFileTextOutDir);
    if(fileTextOutDir.exists())
    {
        qDebug()<<qsFileTextOutDir<<": existed ready delete";
        if(fileTextOutDir.removeRecursively())
        {
            qDebug()<<qsFileTextOutDir<<": delete successful";
        }
    }

    if(fileTextOutDir.mkpath(qsFileTextOutDir))
    {
        qDebug()<<qsFileTextOutDir<<": create successful";
    }

    QDir fileImageOutDir(qsFileImageOutDir);
    if(fileImageOutDir.exists())
    {
        qDebug()<<qsFileImageOutDir<<": existed ready delete";
        if(fileImageOutDir.removeRecursively())
        {
            qDebug()<<qsFileImageOutDir<<": delete successful";
        }
    }
    if(fileImageOutDir.mkpath(qsFileImageOutDir))
    {
        qDebug()<<qsFileImageOutDir<<": create successful";
    }

    if(!outfilepath)
    {
        return 1;
    }
    if(!imagedir)
    {
        return 1;
    }

//    qsFileTextOutFile.toUtf8().size();
    memcpy(outfilepath, qsFileTextOutFile.toUtf8().data(), qsFileTextOutFile.toUtf8().size());
    memcpy(imagedir, qsFileImageOutDir.toUtf8().data(), qsFileImageOutDir.toUtf8().size());


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
    WppComment wpp;
    wpp.initWPPRpcClient();
    wpp.initWppApplication();
    if(wpp.openWPPDoc(qsInputfilepath))
    {
        QStringList qsTextList = wpp.GetWPPText();
        QFile file(qsFileTextOutFile);
        if(file.open(QIODevice::WriteOnly))
        {
            QString qsText = qsTextList.join("\r\n");
            file.write(qsText.toUtf8());
            file.close();
        }
        gloabalIndex = 0;
        wpp.extractPicture(TestPicture);
        wpp.closeWPPDoc();
    }
    wpp.closeApp();
    if(isNeedFreeAppplication)
    {
        app->deleteLater();
    }
}
