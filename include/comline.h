#include <calendar.h>

#define CQM_VERSION "v1.0.6"

void InlegalTime();
// 传入命令行相关参数，当返回值为 0 时，pSolar 中保存解析出来的日期。
int ParseCommand(int nArgc, char** pArgv, calSolar* pSolar, int* pnJushu, int* pbIsAutoTime);

// 打印结果
void PrintResult();

void comlineFree();

