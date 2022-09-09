#ifndef _QIMEN_YP_H_
#define _QIMEN_YP_H_

#include <stdio.h>
#include <calendar.h>
#include <data.h>
#include <comline.h>
#include <qimenbasic.h>
#include <qimenzpzr.h>

/*
*   《时家阴盘》           
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
int qimenYpRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu);


// *************************** 对外接口  ********************************
// 计算局数
void generateJushuYp(const calSolar* pSolar);

#endif