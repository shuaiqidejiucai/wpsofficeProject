#ifndef UTILITYTOOL_H
#define UTILITYTOOL_H
#include <QByteArray>
#include <QString>
#include <QList>
#include <QImage>
#include <QScopedPointer>


//#ifdef _DEBUG
//int i = 0;
//#else

//#endif
enum EU_OperateType
{
    NoOperateType,
    DeleteType,
    ReplaceType
};

struct ST_VarantFile
{
    QString qsFileName;//AS:temp.zip
    QString qsTmpFilePath;//AS:C:/user/temp/temp.zip
    QString qsFilePath;//AS:D:/test/temp.zip
    QByteArray fileData;//AS:zip data
};
/*
 * ST_VarantFile[in]
 * ST_VarantFile&[out]
 * EU_OperateType&[out]
 */
typedef bool(*GetNextOleDataFun)(ST_VarantFile, ST_VarantFile&, EU_OperateType&);
//typedef bool(*GetNextImageDataFun)(ST_OleFile);
class UtilityTool
{
public:
    /*
     * in[srcData]:传入的OLE文件数据
     * out[stOleFile]:传出ole文件的结构体
     * bool[return]:返回是否成功
     */
    //out:srcData
    static bool GetOleFileData(const QByteArray& srcData, ST_VarantFile &stOleFile);
    /*
     * in[zipBytes]:zip数据流
     * out[outData]:传出ole文件的二进制流
     * bool[return]:返回是否成功
     */
    //out:srcData
    static bool findOleDataFromZipMemory(const QByteArray& zipBytes, QByteArray& outData, QString& qsDocType);
private:
    UtilityTool();
};

#endif // UTILITYTOOL_H
