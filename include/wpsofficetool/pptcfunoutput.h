#ifndef PPTCFUNOUTPUT_H
#define PPTCFUNOUTPUT_H


#ifdef __cplusplus
extern "C" {
#endif
struct ST_OutFilePath
{
    char textFilePath[2048];
    char imageDir[2048];
    char attachmentDir[2048];
};
enum ExtratorElementType
{
    TextElementType = 1,
    ImageElementType = 2,
    AttachmentElementType = 4,
    AllElementType = TextElementType | ImageElementType | AttachmentElementType
};

typedef void*  WPPHANDLE ;

//output:wppObj
void initWPP(WPPHANDLE *wppObj);
//input:wppObj
void closeWPP(WPPHANDLE wppObj);
// 入参 char *inputfilepath 输入文件的绝对路径
// 入参  char *rootpath 输出信息的根路径
// 出参 char *outfilepath 文档内容的utf8的txt文档绝对路径
// 出参 char * imagedir   文档内容中的图片路径
//input:wppObj

int extractElement(const char *inputfilepath, const char *rootpath, ST_OutFilePath* pSTOutFilePath, WPPHANDLE wppObj, ExtratorElementType elementType = AllElementType);

#ifdef __cplusplus
}
#endif

#endif // PPTCFUNOUTPUT_H
