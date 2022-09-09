#ifndef _QIMEN_ZG_H_
#define _QIMEN_ZG_H_

#include <qimenbasic.h>
#include <qimenyp.h>
#include <qimenzpzr.h>

// 执行诸葛盘排盘
int qimenZgRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu);

// 诸葛盘八门排盘
void GenerateMenZg();

#endif