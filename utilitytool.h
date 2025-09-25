#ifndef UTILITYTOOL_H
#define UTILITYTOOL_H
#include <QByteArray>
#include <QString>
#include <QList>

struct ST_OleFile
{
    QString qsFileName;
    QString qsTmpFilePath;
    QString qsFilePath;
    QByteArray fileData;
};
typedef bool(*GetNextOleDataFun)(ST_OleFile&);
class UtilityTool
{
public:
    static bool GetOleFileData(const QByteArray& srcData, QList<ST_OleFile> & stOleFileList, GetNextOleDataFun funPtr= nullptr);
    static bool findOleDataFromZipMemory(const QByteArray& zipBytes, QByteArray& outData);
private:
    UtilityTool();
};

#endif // UTILITYTOOL_H
