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
#include <QFile>
#include <QIODevice>
#include <QString>
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

#include "pptcfunoutput.h"
#include <QLibrary>
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    typedef void (*fun_singlefile)(const char*, const char*, char*, char*);
    QString qsLibPath;
#ifdef PPTCFUNLIBPATH
    qsLibPath = QString(PPTCFUNLIBPATH) + "/pptcfunoutout";
#endif
    QLibrary lib(qsLibPath);
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

return 0;

}
