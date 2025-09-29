#include "mainwindow.h"

#include <QApplication>
#include "wppcomment.h"
#include "wpscomment.h"
#include <QDebug>

QString GetBSTRText(BSTR str)
{
    QString qsStr;
    qsStr = QString::fromUtf16(str);
    return qsStr;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // WppComment wpp;
    // wpp.initWPPRpcClient();
    // wpp.initWppApplication();
    // wpp.openWPPDoc("/mnt/hgfs/vmshare/9.ppt");
    // wpp.replaceTextForWPPDoc(u8"神马玩意儿", "");
    // wpp.saveWPPDoc();
    // wpp.closeApp();

    WpsComment wpscom;
    wpscom.initWpsRpcClient();
    wpscom.initWpsApplication();
    wpscom.openWPSDoc("/mnt/hgfs/vmshare/9.doc");
    //wpscom.testChangeImage();
    wpscom.saveWPSDoc();
    wpscom.closeApp();
    return a.exec();
}
