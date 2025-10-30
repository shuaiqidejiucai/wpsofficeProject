#include "mainwindow.h"

#include <QApplication>
#include "wppcomment.h"
#include "wpscomment.h"
#include "etcomment.h"
#include <QDebug>
#include "utilitytool.h"
#include <QLabel>
#include <QDir>
#include <QStack>
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

#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QElapsedTimer>
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

#include "pptcfunoutout.h"
#include <QLibrary>
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    typedef void (*fun_singlefile)(const char*, const char*, char*, char*);

    QLibrary lib("/home/ft2000/mjcenv/wpsofficeProject/lib/pptcfunoutout");
    if (lib.load()) {
        fun_singlefile pptCFunOutput = (fun_singlefile)lib.resolve("fun_singlefile");
        if (pptCFunOutput) {
            char textCh[2048]= {0};
            char imageCh[2048] = {0};
            pptCFunOutput(u8"/home/ft2000/mjcenv/dps-ppt/演示文件-SM- (9).ppt", "/home/ft2000/mjcenv/dps-ppt",textCh, imageCh);
            qDebug()<<"textCh:"<<QString(textCh) <<"======imageCh:"<< QString(imageCh);
        } else {
            qDebug() << "函数加载失败:" << lib.errorString();
        }
    } else {
        qDebug() << "库加载失败:" << lib.errorString();
    }

    //pptCFunOutput("/home/ft2000/mjcenv/dps-ppt", "/home/ft2000/mjcenv/dps-ppt");
return 0;
    //return a.exec();
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
        QElapsedTimer time;
        time.start();
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
            QStringList qsTextList = wpp.GetWPPText();
            QFile file(destPath + "/content" + ".txt");
            if(file.open(QIODevice::WriteOnly))
            {
                QString qsText = qsTextList.join("\r\n");
                file.write(qsText.toUtf8());
                file.close();
            }
            tmpAllPath = destPath + "/content";
            wpp.extractPicture(TestPicture);
            gloabalIndex = 0;
            wpp.closeWPPDoc();

        }
        wpp.closeApp();
        qint64 userd = time.elapsed();
        qDebug()<<"run time:<<<<" << userd<<" ms";
    }

    // WppComment wpp;
    // wpp.initWPPRpcClient();
    // wpp.initWppApplication();
    // wpp.openWPPDoc("/mnt/hgfs/vmshare/8.dps");
    // wpp.extractPicture(TestPicture);



   // qDebug()<< wpp.GetWPPText();
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
}
