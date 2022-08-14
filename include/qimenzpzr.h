#include <stdio.h>
#include <calendar.h>
#include <data.h>
#include <comline.h>
#include <qimenbasic.h>

/*
*   《转盘超接置润法》           
*
*
*   pSolar:             公里日期时间
*   bIsAutoTIme:        是否自动获取时间(1--是，其他不是)
*   qimenJuShu:         气门局数(阴9局到阳9局，-9 ~ 9, 0 表示自动计算局数，其他失败)
*   return:             返回 0 成功，其他失败。
                        -1: 输入的局数不正确
                        -2: 日期数据不合法
                        -3: 日期不在有效范围内
*/
int qimenZpZrRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu);
void qimenZpZrFree();


// 如下部分是给有相同计算逻辑的的其他排盘方式使用
// 计算地盘
void GenerateZpZrDiPan();
// 找值符、直使
void FindZhiShiFuZpZr();
// 转动九星八门
void GenerateXingMenZpZr();
// 计算八神
void GenerateBaShenZpZr();
// 计算天盘
void GenerateTianPanZpZr();
// 计算旬空、马星
void GenerateOtherZpZr();

