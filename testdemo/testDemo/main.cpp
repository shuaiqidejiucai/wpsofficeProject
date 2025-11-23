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


int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MyTcpServer server;
//    server.start(12345);
//    return a.exec();

    Q_UNUSED(argc)
    Q_UNUSED(argv)

    typedef void (*initWPP)(WPPHANDLE*);
    typedef void (*closeWPP)(WPPHANDLE);
    typedef int (*extractImageAndeText)(const char*, const char*, char*, char*,WPPHANDLE);
    QString qsLibPath;
#ifdef PPTCFUNLIBPATH
    qsLibPath = QString(PPTCFUNLIBPATH) + "/pptcfunoutout";
#endif
    //killWppProcess();
    QLibrary lib(qsLibPath);
    if (lib.load())
    {
        initWPP initWPPFun = (initWPP)lib.resolve("initWPP");
        if (initWPPFun)
        {

            WPPHANDLE wppObj = nullptr;
            initWPPFun(&wppObj);

            extractImageAndeText pptCFunOutput = (extractImageAndeText)lib.resolve("extractImageAndeText");
            if (pptCFunOutput)
            {
                QElapsedTimer time;
                time.start();
                for(int i = 0; i < 1; ++i)
                {
                    qDebug()<<"outIndex====================:"<<QString::number(i);
                    char textCh[2048]= {0};
                    char imageCh[2048] = {0};
                    pptCFunOutput(u8"/home/ft2000/mjcenv/dps-ppt/bugwenjian/演示-SM (5).dps", "/home/ft2000/mjcenv/dps-ppt/bugwenjian",textCh, imageCh, wppObj);
                }
                qint64 userd = time.elapsed();
                qDebug()<<"run time:<<<<" << userd<<" ms";
            }

            closeWPP closeWPPFun = (closeWPP)lib.resolve("closeWPP");
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
