#ifndef PPTCFUNOUTPUT_H
#define PPTCFUNOUTPUT_H


#ifdef __cplusplus
extern "C" {
#endif

typedef void*  WPPHANDLE ;

void initWPP(WPPHANDLE *wppObj);

void closeWPP(WPPHANDLE wppObj);

//void pptCFunOutput(const char* inputPPTFileDir, const char* outputPPTFileDir);

// 入参 char *inputfilepath 输入文件的绝对路径
// 入参  char *rootpath 输出信息的根路径
// 出参 char *outfilepath 文档内容的utf8的txt文档绝对路径
// 出参 char * imagedir   文档内容中的图片路径
//int fun_singlefile(const char * inputfilepath, const char *rootpath, char * outfilepath,char * imagedir);

int extractImageAndeText(const char *inputfilepath, const char *rootpath, char *outfilepath, char *imagedir, WPPHANDLE wppObj);

#ifdef __cplusplus
}
#endif

#endif // PPTCFUNOUTPUT_H
