#include <calendar.h>

#define CQM_VERSION "v1.1.2"

void InlegalTime();
// 传入命令行相关参数，当返回值为 0 时，pSolar 中保存解析出来的日期。
int ParseCommand(int nArgc, char** pArgv, calSolar* pSolar, int* pnJushu, int* pbIsAutoTime, int* nStyle);

// 打印结果
void PrintResult();

void comlineFree();

