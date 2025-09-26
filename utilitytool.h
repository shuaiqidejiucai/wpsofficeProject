#ifndef UTILITYTOOL_H
#define UTILITYTOOL_H
#include <QByteArray>
#include <QString>
#include <QList>
#include <QImage>
struct ST_OleFile
{
    QString qsFileName;
    QString qsTmpFilePath;
    QString qsFilePath;
    QByteArray fileData;
};

enum EU_FileOperate
{
    Unknown_OP,
    Delete_OP,
    Replace_OP
};

struct ST_FileOPerate
{
    EU_FileOperate euOperate = Unknown_OP;
    QString fileAbsolutePath;
};

typedef bool(*GetNextOleDataFun)(ST_OleFile, ST_FileOPerate&);
typedef bool(*GetNextImageDataFun)(QImage, ST_FileOPerate&);
class UtilityTool
{
public:
    /*
     * in[srcData]:传入的OLE文件数据
     * out[stOleFile]:传出ole文件的结构体
     * bool[return]:返回是否成功
     */
    //out:srcData
    static bool GetOleFileData(const QByteArray& srcData, ST_OleFile &stOleFile);
    /*
     * in[zipBytes]:zip数据流
     * out[outData]:传出ole文件的二进制流
     * bool[return]:返回是否成功
     */
    //out:srcData
    static bool findOleDataFromZipMemory(const QByteArray& zipBytes, QByteArray& outData);
private:
    UtilityTool();
};

#endif // UTILITYTOOL_H
