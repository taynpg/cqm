#ifndef _QIMEN_BASIC_H_
#define _QIMEN_BASIC_H_

#include <define.h>
#include <data.h>
#include <stdio.h>
#include <string.h>
#include <calendar.h>
#include <comline.h>

void qimenInit();
void qimenFree();

// 奇门输入是否正确
int qimenParmCheck(calSolar* pSolar, int bIsAutoTime, int qimenJuShu);

// 旋转基础定义 (用于旋转宫的数字定义正顺时针旋转，负值逆时针旋转，0不旋转)
void basicNumberTurned(int nTurnedNum);

#endif