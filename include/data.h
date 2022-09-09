#ifndef _CQM_DATA_H_
#define _CQM_DATA_H_

#include <define.h>

// 保存两个或两个字以内的汉字
typedef struct dataHan
{
    char szData[STR_LEN_08];
}dataHan;
// 汉字数组
typedef struct dataHans
{
    dataHan* pHan;
    int nLen;
}dataHans;
// 索引对应表
typedef struct dataIndex
{
    int nLen;
    int* pIndex;
}dataIndex;

// 初始化六十甲子信息 
void dataAllocSixtyJiaZi(dataHans** pData);
// 初始化十天干
void dataAllocTianGan(dataHans** pData);
// 初始化二十四节气表
void dataAllocJieQiTable(dataHans** pData);
// 初始化十二地支
void dataAllocDiZhi(dataHans** pData);
// 查找词在 dataHans 数据的索引，-1 表示查找失败
int dataFindHanIndex(dataHans* pData, const char* pStr);
// 初始化 五虎遁年起月表 对应索引表
void dataAllocIndexCounterWuHu(dataIndex** pIndex);
// 初始化 日上起时法 对应索引表
void dataAllocIndexCounterGetDay(dataIndex** pIndex);
// 初始化 九星
void dataAllocJiuxing(dataHans** pData);
// 九星的旋转位置
void dataAllocJiuXingR(dataHans** pData);
// 初始化 八门
void dataAllocBamen(dataHans** pData);
// 初始化 八门的旋转位置
void dataAllocBamenR(dataHans** pData);
// 初始化 八神
void dataAllocBashenR(dataHans** pData);
// 初始化奇门局数与节气关系对应表
void dataAllocJuAndQi(dataIndex** pIndex);
// 初始化农历日子大写表
void dataAllocLunarDayChinese(dataHans** pData);
// 初始化农历月份大写表
void dataAllocLunarMonChinese(dataHans** pData);
// 初始化数字大写表
void dataAllocNumChinese(dataHans** pData);
// 初始化六甲对应关系
void dataAllocLiuJia(dataIndex** pIndex);
// 初始化十二地支三合首个地支
void dataAllocDizhiSanhe(dataIndex** pIndex);
// 初始化地支相冲表
void dataAllocDizhiChong(dataIndex** pIndex);
// 初始化地支所在的宫位
void dataAllocDizhiGong(dataIndex** pIndex);
// 初始化节气时间
void dataAllocJieQiTime(dataIndex** pData);
// 初始化农历日期对照表
void dataAllocLunarTable(dataIndex** pData);
// 初始化三合局
void dataAllocSanHeJu(dataIndex** pIndex);

// ***********************************************************************************************
// ******************                                                *****************************
// ******************   上面的内存申请统一使用下面两个函数进行内存释放   *****************************
// ******************                                                *****************************
// ***********************************************************************************************
// 去初始化 dataHans 结构体
void dataFreeDataHans(dataHans* pData);
// 去初始化对应索引表
void dataFreeIndexCounter(dataIndex* pIndex);
// ***********************************************************************************************

// 取 char 数组中的某个中文下标开始的几个字(仅数组全中文可用)(如：你好啊，取下标1开始的2个字，好啊)
// [Note: 非安全函数，请自行保证所有的内存可用，即数据一定可取]
void dataGetSubStr(const char* pSource, char* pDestination, int nIndex, int nNum);
// 将下标转换成 Hans 下标用于取 Hans 中的数据
int STRINDEX(int nIndex);
// 查询一个定长数组的指定值的下标
int GetIndexFromArray(const int nArray[], int nLen, int nValue);
// 打印分隔符
void LineLn();

#endif

