#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include "utilitytool.h"
#include <QLabel>
#include <QDir>
#include <QStack>
#include <QFile>
#include <QIODevice>
#include <QString>
#include "pptcfunoutput.h"
#include <QLibrary>
#include "mytcpserver.h"
#include <QFileInfo>
#include <QProcess>
#include <QElapsedTimer>
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
//    QApplication a(argc, argv);
//    MyTcpServer server;
//    server.start(12345);
//    return a.exec();

    Q_UNUSED(argc)
    Q_UNUSED(argv)

    typedef void (*initEt)(ETHANDLE*);
    typedef void (*closeEt)(ETHANDLE);
    typedef int (*extractEtElement)(const char*, const char*, ST_OutFilePath * ,ETHANDLE, ExtratorElementType);
    QString qsLibPath;
#ifdef PPTCFUNLIBPATH
    qsLibPath = QString(PPTCFUNLIBPATH) + "/pptcfunoutout";
#endif
    killEtProcess();
    QLibrary lib(qsLibPath);
    if (lib.load())
    {
        initEt initWPPFun = (initEt)lib.resolve("initEt");
        if (initWPPFun)
        {
            ETHANDLE wppObj = nullptr;
            initWPPFun(&wppObj);

            extractEtElement pptCFunOutput = (extractEtElement)lib.resolve("extractEtElement");
            if (pptCFunOutput)
            {
                QElapsedTimer time;
                time.start();
                for(int i = 0; i < 1; ++i)
                {
                    qDebug()<<"outIndex====================:"<<QString::number(i);
                    ST_OutFilePath pSTOutFilePath = {0};
                    pptCFunOutput(u8"/home/ft2000/mjcenv/dps-ppt/bugwenjian/indoc.et", "/home/ft2000/mjcenv/dps-ppt/bugwenjian",&pSTOutFilePath, wppObj, AllElementType);
                }
                qint64 userd = time.elapsed();
                qDebug()<<"run time:<<<<" << userd<<" ms";
            }

            closeEt closeWPPFun = (closeEt)lib.resolve("closeEt");
            if(closeWPPFun)
            {
                closeWPPFun(wppObj);
            }
        }
        //qDebug()<<"textCh:"<<QString(textCh) <<"======imageCh:"<< QString(imageCh);
    }
    else {
        qDebug()<< lib.errorString();
    }
    return 0;
}
