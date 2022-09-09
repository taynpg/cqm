#include <calendar.h>
#include <data.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

void FreeSolar(calSolar* pSolar);                                                  // 释放公历结构体内存
void InitLunar(calLunar* pLunar);                                                  // 初始化农历结构体
void FreeLunar(calLunar* pLunar);                                                  // 释放农历结构体内存
void InitGanZhi(calGanZhi* pGanZhi);                                               // 初始化干支结构体
void FreeGanZhi(calGanZhi* pGanZhi);                                               // 释放农历四干支内存
void InitMonthJieQi(calMonthJieQi* pMonJieQi);                                     // 初始化节气结构体
void FreeMonthJieQi(calMonthJieQi* pMonJieQi);                                     // 释放当月节气内存s
/*
int IsSolarLegal(const calSolar* pSolar);                                          // 判断公历日期是否合法
*/
int IsLeapYear(int nYear);                                                         // 判断某一年是否是闰年
void StartBefore();                                                                // 计算之前的准备工作
int GetLunarYearDays(int nYear);                                                   // 返回农历 year 年的总天数
int GetLunarLeapDays(int nYear);                                                   // 返回农历 year 年闰月的天数
int GetLunarLeapMonth(int nYear);                                                  // 返回农历 year 年润哪一个月
int GetLunarMonthDays(int nYear, int nMonth);                                      // 返回农历 year 年 month 月的总天
int GetDayDiffFromBase(const calSolar* pSolar);                                    // 返回距离 1900.1.1 的天数
void AllocResult();                                                                // 为计算结果分配内存并初始化
void FreeResult( calResult* pResult);                                              // 释放为计算结果分配的内存
int GetLunarSTerm(int year, int n,  calSolar* pSolar);                             // 某年的第 n 个节气为几日 (从 0 小寒起算)
void GetYearMonGanZhi(const calSolar* pSolar);                                     // 计算农历的年干支，月干支(以立春为新年)
int GetSecondDiviZero(const calSolar* pSolar);                                     // 返回一个时间到 00:00:00 的秒数
/*
void GetNowTime(calSolar* pSolar);                                                 // 获取系统时间
*/
// void GetDateByDiffSecond(const calSolar* pSolar, calSolar* pResult, INT64 nSec);
void GetNextYearMonthGanZhi(const char* pYearGanzhi, char* pNextYearGanZhi, char* pStartMonthGanZhi);
void GetPreMonthGZ(int nYear, int nMon, const char* pYearGanZhi, const char* pMonGanZhi, char* pPreYearGZ, char* pPreMonGZ, int bIsLiChun);
void GetNextJieQiInfo(int* year, int* month, char* pJieQi, char* monzhu, char* yearzhu);
int GetSecondDiviTwoTime(const calSolar* pSolarOne, const calSolar* pSolarTwo);

// ===  基础数据区
dataIndex* pGlobalWuHu = NULL;
dataIndex* pGlobalQiShi = NULL;
dataIndex* pGlobalLunarTable = NULL;
dataIndex* pGlobalJieQiTime = NULL;
dataHans* pGlobalJieQiTable = NULL;
dataHans* pGlobalTianGan = NULL;
dataHans* pGlobalDiZhi = NULL;
dataHans* pGlobalSixtyJiaZi = NULL;

// ===  结果数据区
calResult* pGlobalResult = NULL;

// 拿到计算结果
void calendarGetResult(calResult** pResult)    /* NOLINT */
{
    *pResult = pGlobalResult;
}
void InitSolar(calSolar* pSolar)
{
    pSolar->nSolarYear = -1;
    pSolar->nSolarMonth = -1;
    pSolar->nSolarDay = -1;
    pSolar->nSolarHour = -1;
    pSolar->nSolarMin = -1;
    pSolar->nSolarSecond = -1;
}
// 初始化农历结构体
void InitLunar(calLunar* pLunar)
{
    pLunar->nLunarYear = -1;
    pLunar->nLunarMonth = -1;
    pLunar->nLunarDay = -1;
    pLunar->bBigMonth = 1;
    pLunar->bLeapMonth = 1;
};
// 释放农历结构体内存
void FreeLunar(calLunar* pLunar)
{
    if (pLunar == NULL)
        return;
    free(pLunar);
}
// 释放公历结构体内存
void FreeSolar(calSolar* pSolar)
{
    if (pSolar == NULL)
        return;
    free(pSolar);
}
// 初始化农历四干支
void InitGanZhi(calGanZhi* pGanZhi)
{
    memset(pGanZhi->szLunarYearGZ, 0x0, sizeof(pGanZhi->szLunarYearGZ));
    memset(pGanZhi->szLunarMonthGZ, 0x0, sizeof(pGanZhi->szLunarMonthGZ));
    memset(pGanZhi->szLunarDayGZ, 0x0, sizeof(pGanZhi->szLunarDayGZ));
    memset(pGanZhi->szLunarHourGZ, 0x0, sizeof(pGanZhi->szLunarHourGZ));
}
// 释放农历四干支内存
void FreeGanZhi(calGanZhi* pGanZhi)
{
    if (pGanZhi == NULL)
        return;
    free(pGanZhi);
}
// 初始化结果的当月节气
void InitMonthJieQi(calMonthJieQi* pMonJieQi)
{
    pMonJieQi->nYear = -1;
    pMonJieQi->nFirstHour = -1;
    pMonJieQi->nFirstMin = -1;
    pMonJieQi->nFirstSec = -1;
    pMonJieQi->nFirstDay = -1;
    pMonJieQi->nMonth = -1;
    pMonJieQi->nSecHour = -1;
    pMonJieQi->nSecMin = -1;
    pMonJieQi->nSecSec = -1;
    pMonJieQi->nSecondDay = -1;
    memset(pMonJieQi->szFirstJieQi, 0x0, sizeof(pMonJieQi->szFirstJieQi));
    memset(pMonJieQi->szSecondJieQi, 0x0, sizeof(pMonJieQi->szSecondJieQi));
}
// 释放当月节气内存
void FreeMonthJieQi(calMonthJieQi* pMonJieQi)
{
    if (pMonJieQi == NULL)
        return;
    free(pMonJieQi);
}
// 为计算结果分配内存并初始化
void AllocResult()
{
    // 1. 分配内存
    if (pGlobalResult == NULL)
    {
        pGlobalResult = (calResult *)malloc(sizeof (calResult));
        pGlobalResult->pSolar = NULL;
        pGlobalResult->pLunar = NULL;
        pGlobalResult->pMonJieQi = NULL;
        pGlobalResult->pGanzi = NULL;
    }
        
    if (pGlobalResult->pSolar == NULL)
        pGlobalResult->pSolar = (calSolar *) malloc(sizeof (calSolar));
    if (pGlobalResult->pLunar == NULL)
        pGlobalResult->pLunar = (calLunar *) malloc(sizeof (calLunar));
    if (pGlobalResult->pMonJieQi == NULL)
        pGlobalResult->pMonJieQi = (calMonthJieQi *) malloc(sizeof (calMonthJieQi));
    if (pGlobalResult->pGanzi == NULL)
        pGlobalResult->pGanzi = (calGanZhi *) malloc(sizeof (calGanZhi));

    InitGanZhi(pGlobalResult->pGanzi);
    InitLunar(pGlobalResult->pLunar);
    InitSolar(pGlobalResult->pSolar);
    InitMonthJieQi(pGlobalResult->pMonJieQi);
}
// 释放为计算结果分配的内存
void FreeResult(calResult* pResult)
{
    if (pResult == NULL)
        return;

    if (pGlobalResult->pSolar != NULL)
    {
        FreeSolar(pGlobalResult->pSolar);
        pGlobalResult->pSolar = NULL;
    }
        
    if (pGlobalResult->pLunar != NULL)
    {
        FreeLunar(pGlobalResult->pLunar);
        pGlobalResult->pLunar = NULL;
    }
        
    if (pGlobalResult->pMonJieQi != NULL)
    {
        FreeMonthJieQi(pGlobalResult->pMonJieQi);
        pGlobalResult->pMonJieQi = NULL;
    }
        
    if (pGlobalResult->pGanzi != NULL)
    {
        FreeGanZhi(pGlobalResult->pGanzi);
        pGlobalResult->pGanzi = NULL;
    }

    free(pResult);
}
// 计算之前的准备工作
void StartBefore()
{
    // 1. 为农历表分配内存
    if (pGlobalLunarTable == NULL)
        dataAllocLunarTable(&pGlobalLunarTable);
    // 2. 为节气时间表分配内存
    if (pGlobalJieQiTime == NULL)
        dataAllocJieQiTime(&pGlobalJieQiTime);
    // 3. 为节气表分配内存
    if (pGlobalJieQiTable == NULL)
        dataAllocJieQiTable(&pGlobalJieQiTable);
    // 4. 为十天干分配内存
    if (pGlobalTianGan == NULL)
        dataAllocTianGan(&pGlobalTianGan);
    // 5. 为十二地支分配内存
    if (pGlobalDiZhi == NULL)
        dataAllocDiZhi(&pGlobalDiZhi);
    // 6. 为六十甲子分配内存
    if (pGlobalSixtyJiaZi == NULL)
        dataAllocSixtyJiaZi(&pGlobalSixtyJiaZi);
    // 7. 五虎遁年起月表
    if (pGlobalWuHu == NULL)
        dataAllocIndexCounterWuHu(&pGlobalWuHu);
    // 8. 日上起时法表
    if (pGlobalQiShi == NULL)
        dataAllocIndexCounterGetDay(&pGlobalQiShi);
}
// 计算完成后的工作()
void calendarFree()
{
    // 释放全局农历表
    if (pGlobalLunarTable)
    {
        dataFreeIndexCounter(pGlobalLunarTable);
        pGlobalLunarTable = NULL;
    }
    // 释放全局农历时间表
    if (pGlobalJieQiTime)
    {
        dataFreeIndexCounter(pGlobalJieQiTime);
        pGlobalJieQiTime = NULL;
    }
    // 释放全局节气表
    if (pGlobalJieQiTable)
    {
        dataFreeDataHans(pGlobalJieQiTable);
        pGlobalJieQiTable = NULL;
    }
    // 释放十天干表
    if (pGlobalTianGan)
    {
        dataFreeDataHans(pGlobalTianGan);
        pGlobalTianGan = NULL;
    }
    // 释放十二地支表
    if (pGlobalDiZhi)
    {
        dataFreeDataHans(pGlobalDiZhi);
        pGlobalDiZhi = NULL;
    }
    // 释放六十甲子内存
    if (pGlobalSixtyJiaZi)
    {
        dataFreeDataHans(pGlobalSixtyJiaZi);
        pGlobalSixtyJiaZi = NULL;
    }
    // 五虎遁年起月表
    if (pGlobalWuHu)
    {
        dataFreeIndexCounter(pGlobalWuHu);
        pGlobalWuHu = NULL;
    }
    // 日上起时法表
    if (pGlobalQiShi)
    {
        dataFreeIndexCounter(pGlobalQiShi);
        pGlobalQiShi = NULL;
    }
    // 释放结结果表
    if (pGlobalResult)
    {
        FreeResult(pGlobalResult);
        pGlobalResult = NULL;
    }
}
// 对数进行取余数(支持负数)
int GetRemainder(int nNum, int nBasic)
{
    int temp = nNum % nBasic;
    if (temp >= 0)
        return temp;
    temp += nBasic;
    return temp;
}
// 计算时辰干支(该函数要在计算完年月干支后进行)
void GetHourGanZhi(const calSolar* pSolar)
{
    char temp[STR_LEN_08];
    memset(temp, 0x0, sizeof(temp));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarDayGZ, temp, 0, 1);
    int tianindex = dataFindHanIndex(pGlobalTianGan, temp);
    int ntianindex = pGlobalQiShi->pIndex[tianindex];
    memset(temp, 0x0, sizeof(temp));
    strcpy(temp, &pGlobalTianGan->pHan->szData[STRINDEX(ntianindex)]);
    strcat(temp, "子");

    int nJiazi = dataFindHanIndex(pGlobalSixtyJiaZi, temp);
    int ttime = pSolar->nSolarHour + 1;
    if (ttime == 24)
        ttime = 0;
    int anum = ttime / 2;
    strcpy(pGlobalResult->pGanzi->szLunarHourGZ, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(GetRemainder(nJiazi+anum, 60))]);
}
// 给定年月和干支，修改为上一个月年月干支，最后参数为传入月是否是立春
void GetPreMonthGZ(int nYear, int nMon, const char* pYearGanZhi, const char* pMonGanZhi, char* pPreYearGZ, char* pPreMonGZ, int bIsLiChun)
{
    if (bIsLiChun)
    {
        // 1. 查看当前的年干支索引
        int yearindex = dataFindHanIndex(pGlobalSixtyJiaZi, pYearGanZhi);
        int nPre = GetRemainder(yearindex-1, 60);
        strcpy(pPreYearGZ, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(nPre)]);
        // 2. 拿到五虎起月表的天干索引
        char data[STR_LEN_08];
        memset(data, 0x0, sizeof(data));
        dataGetSubStr(pPreYearGZ, data, 0, 1);
        int tianindex = dataFindHanIndex(pGlobalTianGan, data);
        int ntianindex = pGlobalWuHu->pIndex[tianindex];
        // 3. 拿到上一个月的月干支
        memset(data, 0x0, sizeof(data));
        strcpy(data, &pGlobalTianGan->pHan->szData[STRINDEX(ntianindex)]);
        strcat(data, "寅");
        strcpy(pPreMonGZ, data);
    }
    else
    {
        strcpy(pPreYearGZ, pYearGanZhi);
        int monindex = dataFindHanIndex(pGlobalSixtyJiaZi, pMonGanZhi);
        int nPre = GetRemainder(monindex-1, 60);
        strcpy(pPreMonGZ, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(nPre)]);
    }
}
// 计算农历的年干支，月干支(以立春为新年)
void GetYearMonGanZhi(const calSolar* pSolar)
{
    // 以公历 1900 年 01 月 6 日时为起始日期 （小寒）
    // 对应农历日期为 1900 年 己亥年，丁丑月 (以立春为新年)
    char yearzhu[] = "己亥";
    char monzhu[] = "丁丑";
    char sjieqi[] = "小寒";
    int year = 1900;
    int mon = 1;

    // 某年 02-04-立春-时间13:12:44  02-19-雨水-时间20:01:14
    int diviyear = (pSolar->nSolarYear - 1900) * 24 + dataFindHanIndex(pGlobalJieQiTable,
                                                                       pGlobalResult->pMonJieQi->szFirstJieQi);
    for (int i = 0; i < diviyear; ++i)
        GetNextJieQiInfo(&year, &mon, sjieqi, monzhu, yearzhu);

    calSolar solar;
    solar.nSolarYear = pGlobalResult->pMonJieQi->nYear;
    solar.nSolarMonth = pGlobalResult->pMonJieQi->nMonth;
    solar.nSolarDay = pGlobalResult->pMonJieQi->nFirstDay;
    solar.nSolarHour = pGlobalResult->pMonJieQi->nFirstHour;
    solar.nSolarMin = pGlobalResult->pMonJieQi->nFirstMin;
    solar.nSolarSecond = pGlobalResult->pMonJieQi->nFirstSec;

    INT64 res = GetSecondDiviTwoDate(&solar, pSolar);
    if (res < 0)
    {
        strcpy(pGlobalResult->pGanzi->szLunarYearGZ, yearzhu);
        strcpy(pGlobalResult->pGanzi->szLunarMonthGZ, monzhu);
    }
    else 
    {
        if (strcmp(pGlobalResult->pMonJieQi->szFirstJieQi, "立春") == 0)
            GetPreMonthGZ(year, mon, yearzhu, monzhu, pGlobalResult->pGanzi->szLunarYearGZ, 
            pGlobalResult->pGanzi->szLunarMonthGZ, 1);
        else
            GetPreMonthGZ(year, mon, yearzhu, monzhu, pGlobalResult->pGanzi->szLunarYearGZ, 
            pGlobalResult->pGanzi->szLunarMonthGZ, 0);
    }

}
// 返回两个日期之间相差的秒数
INT64 GetSecondDiviTwoDate(const calSolar* pSolarOne, const calSolar* pSolarTwo)
{
    INT64 a = GetSecondDiviTwoTime(pSolarOne, pSolarTwo);
    INT64 b = GetDayDiffTwo(pSolarOne, pSolarTwo);
    return (a + b * 86400);
}
// 返回两个时间点的秒数之差
int GetSecondDiviTwoTime(const calSolar* pSolarOne, const calSolar* pSolarTwo)
{
    return (GetSecondDiviZero(pSolarOne) - GetSecondDiviZero(pSolarTwo));
}
// 返回一个时间到 00:00:00 的秒数
int GetSecondDiviZero(const calSolar* pSolar)
{
    return pSolar->nSolarHour * 3600 + pSolar->nSolarMin * 60 + pSolar->nSolarSecond;
}
// 给定年干支，返回下一年的年干支和起始月干支
void GetNextYearMonthGanZhi(const char* pYearGanzhi, char* pNextYearGanZhi, char* pStartMonthGanZhi)
{
    int yindex = dataFindHanIndex(pGlobalSixtyJiaZi, pYearGanzhi);
    strcpy(pNextYearGanZhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(GetRemainder(yindex+1, 60))]);
    char temp[STR_LEN_08];
    memset(temp, 0x0, sizeof (temp));

    dataGetSubStr(pNextYearGanZhi, temp, 0, 1);
    int windex = dataFindHanIndex(pGlobalTianGan, temp);
    memset(temp, 0x0, sizeof (temp));
    strcpy(temp, &pGlobalTianGan->pHan->szData[STRINDEX(pGlobalWuHu->pIndex[windex])]);
    strcat(temp, "寅");
    strcpy(pStartMonthGanZhi, temp);
}
// 返回下一个月的年干支月干支信息
void GetNextJieQiInfo(int* year, int* month, char* pJieQi, char* monzhu, char* yearzhu)
{
    // 1900 1 小寒 丁丑
    int startindex = dataFindHanIndex(pGlobalJieQiTable, pJieQi);
    // 下一个节气还是当月
    if ((startindex % 2) == 0)
        strcpy(pJieQi, &pGlobalJieQiTable->pHan->szData[STRINDEX(GetRemainder(startindex+1, 24))]);
    else
    {
        int nmont = *month + 1;
        if (nmont > 12)
        {
            *year = *year + 1;
            *month = 1;
        }
        else
            *month = nmont;
        strcpy(pJieQi, &pGlobalJieQiTable->pHan->szData[STRINDEX((startindex + 1) % 24)]);
        char temp[STR_LEN_08];
        if (strcmp(pJieQi, "立春") == 0)
        {
            memset(temp, 0x0, sizeof (temp));
            GetNextYearMonthGanZhi(yearzhu, temp, monzhu);
            strcpy(yearzhu, temp);
        }
        else
        {
            int index = dataFindHanIndex(pGlobalSixtyJiaZi, monzhu);
            strcpy(monzhu, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(GetRemainder(index+1, 60))]);
        }
    }
}
// 传入公历进行结算
int calendarRun(const calSolar* pSolar)
{
    int rtn = -1;
    // 1. 检查日期是否合法
    if (IsSolarLegal(pSolar) != 0)
        return rtn;

    // 2. 准备各种需要的基础数据
    StartBefore();
    AllocResult();   // 准备好存数据的位置

    pGlobalResult->pSolar->nSolarYear = pSolar->nSolarYear;
    pGlobalResult->pSolar->nSolarMonth = pSolar->nSolarMonth;
    pGlobalResult->pSolar->nSolarDay = pSolar->nSolarDay;
    pGlobalResult->pSolar->nSolarHour = pSolar->nSolarHour;
    pGlobalResult->pSolar->nSolarMin = pSolar->nSolarMin;
    pGlobalResult->pSolar->nSolarSecond = pSolar->nSolarSecond;

    // 3. 计算农历相关
    calSolar solar;
    solar.nSolarYear = 1900;
    solar.nSolarMonth = 1;
    solar.nSolarDay = 31;
    solar.nSolarHour = 0;
    solar.nSolarMin = 0;
    solar.nSolarSecond = 0;

    int offset = GetDayDiffTwo(pSolar, &solar);

    int i = 0, leap = 0, temp = 0;

    for (i = 1900; i < 2100 && offset > 0; i++)
    {
        temp = GetLunarYearDays(i);
        offset -= temp;
    }
    if (offset < 0)
    {
        offset += temp;
        i--;
    }
    pGlobalResult->pLunar->nLunarYear = i;
    leap = GetLunarLeapMonth(i); // 闰哪个月
    pGlobalResult->pLunar->bLeapMonth = 0;

    for (i = 1; i < 13 && offset > 0; i++)
    {
        //闰月
        if (leap > 0 && i == (leap + 1) && (!pGlobalResult->pLunar->bLeapMonth))
        {
            --i;
            pGlobalResult->pLunar->bLeapMonth = 1;
            temp = GetLunarLeapDays(pGlobalResult->pLunar->nLunarYear);
        }
        else
        {
            temp = GetLunarMonthDays(pGlobalResult->pLunar->nLunarYear, i);
        }

        //解除闰月
        if (pGlobalResult->pLunar->bLeapMonth == 1 && i == (leap + 1))
            pGlobalResult->pLunar->bLeapMonth = 0;

        offset -= temp;
    }
    if (offset == 0 && leap > 0 && i == leap + 1)
    {
        if (pGlobalResult->pLunar->bLeapMonth)
        {
            pGlobalResult->pLunar->bLeapMonth = 0;
        }
        else
        {
            pGlobalResult->pLunar->bLeapMonth = 1;
            --i;
        }
    }
    if (offset < 0)
    {
        offset += temp;
        --i;
    }
    pGlobalResult->pLunar->nLunarMonth = i;
    pGlobalResult->pLunar->nLunarDay = offset + 1;
    pGlobalResult->pLunar->bBigMonth = (pGlobalResult->pLunar->bLeapMonth ?
            GetLunarLeapDays(pGlobalResult->pLunar->nLunarYear) :
            GetLunarMonthDays(pGlobalResult->pLunar->nLunarYear, pGlobalResult->pLunar->nLunarMonth)) > 29 ? 1 : 0;

    // 4. 计算节气相关
    calSolar timeone;
    calSolar timetwo;
    GetLunarSTerm(pSolar->nSolarYear, (pSolar->nSolarMonth - 1) * 2, &timeone);
    GetLunarSTerm(pSolar->nSolarYear, (pSolar->nSolarMonth - 1) * 2 + 1, &timetwo);

    // 4.1 将计算结果保存起来
    pGlobalResult->pMonJieQi->nYear = timeone.nSolarYear;
    pGlobalResult->pMonJieQi->nMonth = timeone.nSolarMonth;
    pGlobalResult->pMonJieQi->nFirstDay  = timeone.nSolarDay;
    pGlobalResult->pMonJieQi->nFirstHour = timeone.nSolarHour;
    pGlobalResult->pMonJieQi->nFirstMin = timeone.nSolarMin;
    pGlobalResult->pMonJieQi->nFirstSec = timeone.nSolarSecond;
    pGlobalResult->pMonJieQi->nSecondDay = timetwo.nSolarDay;
    pGlobalResult->pMonJieQi->nSecHour = timetwo.nSolarHour;
    pGlobalResult->pMonJieQi->nSecMin = timetwo.nSolarMin;
    pGlobalResult->pMonJieQi->nSecSec = timetwo.nSolarSecond;

    // 4.2 添加二十四节气
    strcpy(pGlobalResult->pMonJieQi->szFirstJieQi, &pGlobalJieQiTable->pHan->szData[STRINDEX((pSolar->nSolarMonth - 1) * 2)]);
    strcpy(pGlobalResult->pMonJieQi->szSecondJieQi, &pGlobalJieQiTable->pHan->szData[STRINDEX(((pSolar->nSolarMonth - 1) * 2 + 1))]);

    // 5. 计算日柱
    int r = GetDayDiffFromBase(pSolar);
    // 5.1 夜晚11点以后算第二天
    if (pSolar->nSolarHour == 23)
        r++;
    
    char szTemp[STR_LEN_08];
    memset(szTemp, 0x0, sizeof(szTemp));
    int tianindex = r % 10;
    strcpy(szTemp, &pGlobalTianGan->pHan->szData[STRINDEX(tianindex)]);
    int diindex = (r + 10) % 12;
    strcat(szTemp, &pGlobalDiZhi->pHan->szData[STRINDEX(diindex)]);
    strcpy(pGlobalResult->pGanzi->szLunarDayGZ, szTemp);
    
    GetYearMonGanZhi(pSolar);
    GetHourGanZhi(pSolar);
    return 0;
}
// 某年的第 n 个节气为几日 (从 0 小寒起算)
int GetLunarSTerm(int year, int n, calSolar* pSolar)
{
    int dyear = year - 1900;
    int up = dyear * 24 + n;

    INT64 rec = 0;
    for (int i = 0; i < up; i++)
        rec = rec + pGlobalJieQiTime->pIndex[i];

    calSolar solar;
    solar.nSolarYear = 1900;
    solar.nSolarMonth = 1;
    solar.nSolarDay = 6;
    solar.nSolarHour = 2;
    solar.nSolarMin = 3;
    solar.nSolarSecond = 57;
    GetDateByDiffSecond(&solar, pSolar, rec);
    return pSolar->nSolarDay;
}
// 返回农历 year 年的总天数
int GetLunarYearDays(int nYear)
{
    int i = 0, sum = 348, temp = 0;
    for (i = 0x8000; i > 0x8; i >>= 1)
    {
        temp = pGlobalLunarTable->pIndex[nYear - 1900];
        sum += (temp & i) ? 1 : 0;
    }
    return sum + GetLunarLeapDays(nYear);
}
// 返回农历 year 年闰月的天数
int GetLunarLeapDays(int nYear)
{
    if (GetLunarLeapMonth(nYear))
        return (pGlobalLunarTable->pIndex[nYear - 1899] & 0xf) == 0xf ? 30 : 29;
    else
        return 0;
}
// 返回农历 year 年润哪一个月，没有返回 0
int GetLunarLeapMonth(int nYear)
{
    int result = pGlobalLunarTable->pIndex[nYear - 1900] & 0xf;
    return result == 0xf ? 0 : result;
}
// 返回农历 year 年 month 月的总天
int GetLunarMonthDays(int nYear, int nMonth)
{
    int re = (pGlobalLunarTable->pIndex[nYear - 1900] & (0x10000 >> nMonth)) ? 30 : 29;
    return re;
}
// 根据基准日期和相差的秒钟计算另一个日期, 正向前，负向后 (Note: 请确认日期 solar 的正确性)
void GetDateByDiffSecond(const calSolar* pSolar, calSolar* pResult, INT64 nSec)
{
    int year = pSolar->nSolarYear, month = pSolar->nSolarMonth, day = pSolar->nSolarDay, i = 0;
    int hour = pSolar->nSolarHour, minute = pSolar->nSolarMin, second = pSolar->nSolarSecond;
    char c;
    INT64 n = 0;
    INT64 num = nSec / (24 * 3600);

    int remain = (int)(nSec % (24 * 3600));
    if (remain < 0)
        remain = -remain;

    int bsec = hour * 3600 + minute * 60 + second;

    if (num < 0)
    {
        c = '-';
        n = -num;
    }
    else
    {
        c = '+';
        n = num;
    }
    for (i = 0; i < n; i++)
    {
        if (c == '-')
        {
            day--;
            if (day == 0)
            {
                month--;
                if (month == 0)
                {
                    month = 12;
                    year--;
                }
                switch (month)
                {
                    case 4:
                    case 6:
                    case 9:
                    case 11: {day = 30; break; }
                    case 2:
                    {
                        if (IsLeapYear(year)) day = 29;
                        else day = 28;
                        break;
                    }
                    default:
                    {
                        day = 31;
                        break;
                    }
                };
            }
        }
        else
        {
            day++;
            switch (month)
            {
                case 4:
                case 6:
                case 9:
                case 11:
                {
                    if (day == 31)
                    {
                        day = 1;
                        month++;
                    }
                    break;
                }
                case 2: {
                    if (IsLeapYear(year))
                    {
                        if (day == 30)
                        {
                            day = 1;
                            month++;
                        }
                    }
                    else
                    {
                        if (day == 29)
                        {
                            day = 1;
                            month++;
                        }
                    }
                    break;
                }
                default:
                {
                    if (day == 32)
                    {
                        day = 1;
                        month++;
                    }
                    break;
                }
            };
            if (month == 13)
            {
                month = 1;
                year++;
            }
        }
    }

    if (c == '-')
    {
        int divi = bsec - remain;
        if (divi >= 0)
        {
            pResult->nSolarHour = divi / 3600;
            pResult->nSolarMin = (divi % 3600) / 60;
            pResult->nSolarSecond = divi - pResult->nSolarHour * 3600 - pResult->nSolarMin * 60;
        }
        else
        {
            GetPreDayFromDate(pSolar, pResult);
            int t = 24 * 3600 + divi;
            pResult->nSolarHour = t / 3600;
            pResult->nSolarMin = (t % 3600) / 60;
            pResult->nSolarSecond = t - pResult->nSolarHour * 3600 - pResult->nSolarMin * 60;
            return;
        }
    }
    else
    {
        int divi = bsec + remain;
        if (divi > (24 * 3600))
        {
            calSolar calTem;
            calTem.nSolarYear = year;
            calTem.nSolarMonth = month;
            calTem.nSolarDay = day;
            GetAfterDayFromDateOwn(&calTem);

            pResult->nSolarYear = calTem.nSolarYear;
            pResult->nSolarMonth = calTem.nSolarMonth;
            pResult->nSolarDay = calTem.nSolarDay;

            divi = divi - 24 * 3600;

            pResult->nSolarHour = divi / 3600;
            pResult->nSolarMin = (divi % 3600) / 60;
            pResult->nSolarSecond = divi - pResult->nSolarHour * 3600 - pResult->nSolarMin * 60;
            return;
        }
        pResult->nSolarHour = divi / 3600;
        pResult->nSolarMin = (divi % 3600) / 60;
        pResult->nSolarSecond = divi - pResult->nSolarHour * 3600 - pResult->nSolarMin * 60;
    }
    pResult->nSolarYear = year;
    pResult->nSolarMonth = month;
    pResult->nSolarDay = day;
}
// 返回一个公历日期的前一天的日期 (返回 0 成功，其他失败) (Note: 请确认日期 solar 的正确性)
void GetPreDayFromDate(const calSolar* pSolar, calSolar* pResult)
{
    int year = pSolar->nSolarYear;
    int month = pSolar->nSolarMonth;
    int day = pSolar->nSolarDay;

    pResult->nSolarHour = pSolar->nSolarHour;
    pResult->nSolarMin = pSolar->nSolarMin;
    pResult->nSolarSecond = pSolar->nSolarSecond;

    if (day > 1)
    {
        pResult->nSolarYear = pSolar->nSolarYear;
        pResult->nSolarMonth = pSolar->nSolarMonth;
        pResult->nSolarDay = pSolar->nSolarDay - 1;
        return ;
    }
    if (month > 1)
    {
        pResult->nSolarYear = year;
        if ((month - 1) == 2)
        {
            pResult->nSolarMonth = 2;
            if (IsLeapYear(year))
                pResult->nSolarDay = 29;
            else
                pResult->nSolarDay = 28;
            return ;
        }
        pResult->nSolarMonth = month - 1;
        switch (pResult->nSolarMonth) {
            case 3:
            case 5:
            case 7:
            case 8:
            case 10: {
                pResult->nSolarDay = 31;
                break;
            }
            default: {
                pResult->nSolarDay = 30;
                break;
            }
        }
        return;
    }
    pResult->nSolarYear = year - 1;
    pResult->nSolarMonth = 12;
    pResult->nSolarDay = 31;
}
// 复制 solar 结构体内容
void DupSolar(const calSolar* pSourceSolar, calSolar* pDesSolar)
{
    if (pSourceSolar == NULL || pDesSolar == NULL)
        return;
    pDesSolar->nSolarYear = pSourceSolar->nSolarYear;
    pDesSolar->nSolarMonth = pSourceSolar->nSolarMonth;
    pDesSolar->nSolarDay = pSourceSolar->nSolarDay;
    pDesSolar->nSolarHour = pSourceSolar->nSolarHour;
    pDesSolar->nSolarMin = pSourceSolar->nSolarMin;
    pDesSolar->nSolarSecond = pSourceSolar->nSolarSecond;
}
// 返回一个公历日期的后一天的日期(改变自己)
void GetAfterDayFromDateOwn(calSolar* pSolar)
{
    if (pSolar == NULL)
        return;
    calSolar temp;
    DupSolar(pSolar, &temp);
    GetAfterDayFromDate(&temp, pSolar);
}
// 返回一个公历日期的后一天的日期
void GetAfterDayFromDate(const calSolar* pSolar, calSolar* pResult)
{
    int year = pSolar->nSolarYear;
    int month = pSolar->nSolarMonth;
    int day = pSolar->nSolarDay;

    pResult->nSolarHour = pSolar->nSolarHour;
    pResult->nSolarMin = pSolar->nSolarMin;
    pResult->nSolarSecond = pSolar->nSolarSecond;

    int upDay = 0;

    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            upDay = 31;
            break;
        }
        case 2:
        {
            if (IsLeapYear(year))
                upDay = 29;
            else
                upDay = 28;
            break;
        }
        default:
        {
            upDay = 30;
            break;
        }
    }

    if (month <= 11)
    {
        pResult->nSolarYear = year;
        if (day == upDay)
        {
            pResult->nSolarMonth = month + 1;
            pResult->nSolarDay = 1;
        }
        else
        {
            pResult->nSolarMonth = month;
            pResult->nSolarDay = day + 1;
        }
    }
    else if (month == 12)
    {
        if (day == upDay)
        {
            pResult->nSolarYear = year + 1;
            pResult->nSolarMonth = 1;
            pResult->nSolarDay = 1;
        }
        else
        {
            pResult->nSolarYear = year;
            pResult->nSolarMonth = 12;
            pResult->nSolarDay = day + 1;
        }
    }
}
// 判断某一年是否是闰年。(返回 0 表示是闰年，其他不是)
int IsLeapYear(int nYear)
{
    if (((nYear % 4 == 0) && (nYear % 100 != 0)) || (nYear % 400 == 0)) 
        return 1;
    return 0;
}
// 判断公历日期是否合法。(返回 0 表示合法，其他不合法)
int IsSolarLegal(const calSolar* pSolar)
{
    int rtn = -1;
    if (pSolar->nSolarYear < 1901 || pSolar->nSolarYear > 2099)
        return rtn;
    if (pSolar->nSolarMonth < 1 || pSolar->nSolarMonth > 12)
        return rtn;
    
    int y = pSolar->nSolarYear;
    int m = pSolar->nSolarMonth;
    int d = pSolar->nSolarDay;

    if (IsLeapYear(y))
    {
        if (m == 2)
        {
            if ((d > 29) || (d < 1))
                return rtn;
        }
    }
    else
    {
        if (m == 2)
        {
            if ((d > 28) || (d < 1))
                return rtn;
        }
    }
    if ((m == 1) || (m == 3) || (m == 5) || (m == 7) || (m == 8) || (m == 10) || (m == 12))
    {
        if ((d > 31) || (d < 1))
            return rtn;
    }
    if ((m == 4) || (m == 6) || (m == 9) || (m == 11))
    {
        if ((d > 30) || (d < 1))
            return rtn;
    }

    int hour = pSolar->nSolarHour;
    int min = pSolar->nSolarMin;
    int sec = pSolar->nSolarSecond;
    
    if ((hour > 23) || (hour < 0))
        return rtn;
    if ((min > 59) || (min < 0))
        return rtn;
    if ((sec > 59) || (sec < 0))
        return rtn;

    return 0;
}
// 返回距离 1900.1.1 的天数 (Note: 请确保日期有效)
int GetDayDiffFromBase(const calSolar* pSolar)
{
    int y = pSolar->nSolarYear;
    int m = pSolar->nSolarMonth;
    int d = pSolar->nSolarDay;

    int dyear = 0;
    int dmonth = 0;
    int dday = 0;

    int sum = 0;

    if (y >= 1900)
    {
        for (int i = 1900; i < y; i++)
        {
            if (IsLeapYear(i))
                dyear = dyear + 366;
            else
                dyear = dyear + 365;
        }
        for (int i = 1; i < m; i++)
        {
            switch (i)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12: dmonth = dmonth + 31; break;
                case 4:
                case 6:
                case 9:
                case 11:
                    dmonth = dmonth + 30;
                    break;
                case 2: {
                    if (IsLeapYear(y))
                        dmonth = dmonth + 29;
                    else
                        dmonth = dmonth + 28;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        dday = d - 1;
        sum = dyear + dmonth + dday;
    }
    else
    {
        for (int i = y; i < 1900; i++)
        {
            if (IsLeapYear(i))
                dyear = dyear + 366;
            else
                dyear = dyear + 365;
        }
        for (int i = 1; i < m; i++)
        {
            switch (i)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12: dmonth = dmonth + 31; break;
                case 4:
                case 6:
                case 9:
                case 11: dmonth = dmonth + 30; break;
                case 2:
                {
                if (IsLeapYear(y))
                    dmonth = dmonth + 29;
                else
                    dmonth = dmonth + 28;
                break;
                }
                default:
                {
                    break;
                }
            }
        }
        dday = d - 1;
        sum = dyear - dmonth - dday;
        sum = -sum;
    }
    return sum;
}
// 返回两个日期之间的天数差 (Note: 请确保日期有效)
int GetDayDiffTwo(const calSolar* pSolarOne, const calSolar* pSolarTwo)
{
    int a = GetDayDiffFromBase(pSolarOne);
    int b = GetDayDiffFromBase(pSolarTwo);
    return a - b;
}
#ifdef _WIN32
// 获取系统时间
void GetNowTime(calSolar* pSolar)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    pSolar->nSolarYear = time.wYear;
    pSolar->nSolarMonth = time.wMonth;
    pSolar->nSolarDay = time.wDay;
    pSolar->nSolarHour = time.wHour;
    pSolar->nSolarMin = time.wMinute;
    pSolar->nSolarSecond = time.wSecond;
}
#else
// 获取系统时间
void GetNowTime(calSolar* pSolar)
{
    struct tm* nowtime = NULL;
    time_t t;
    time(&t);
    nowtime = localtime(&t);
    pSolar->nSolarYear = nowtime->tm_year + 1900; 
    pSolar->nSolarMonth = nowtime->tm_mon + 1;
    pSolar->nSolarDay = nowtime->tm_mday;
    pSolar->nSolarHour = nowtime->tm_hour;
    pSolar->nSolarMin = nowtime->tm_min;
    pSolar->nSolarSecond = nowtime->tm_sec;
}
#endif

 // 判断公历日期范围是否合法(0 合法)
int IsSolarAreaLegal(const calSolar* pSolar)
{
    calSolar lower;
    lower.nSolarYear = 1901;
    lower.nSolarMonth = 1;
    lower.nSolarDay = 1;
    lower.nSolarHour = 0;
    lower.nSolarMin = 0;
    lower.nSolarSecond = 0;

    calSolar higher;
    higher.nSolarYear = 2098;
    higher.nSolarMonth = 12;
    higher.nSolarDay = 31;
    higher.nSolarHour = 23;
    higher.nSolarMin = 59;
    higher.nSolarSecond = 59;

    if (GetSecondDiviTwoDate(pSolar, &lower) < 0 ||
        GetSecondDiviTwoDate(&higher, pSolar) < 0)
        return -1;
    else
        return 0;
}                                            
