#include <comline.h>
#include <stdio.h>
#include <string.h>
#include <data.h>

// 计算解结果区
int                    nStyle = 0;
extern int             nReDiPan[9];
extern int             nReTianPan[9];
extern int             nReJiuXing[9];
extern int             nReBamen[9];
extern int             nReBashen[9];
extern int             nReOther[9];                         // 内容为 0 表示没有，1 为空，2为马星
extern int             nReZhifu;
extern int             nReZhishi;
char                   nReZhiFuHan[STR_LEN_08];
char                   nReZhiShiHan[STR_LEN_08];
char                   nReTianGanHan[STR_LEN_08];
extern int             nGlobalAutoJushu;
extern char            szReNullOne[STR_LEN_08];            // 空亡一
extern char            szReNullTwo[STR_LEN_08];            // 空亡二

extern dataHans*       pGlobalJiuxing;
extern dataHans*       pGlobalBamen;
extern dataHans*       pGlobalBashenR;
extern dataHans*       pGlobalJiuxingR;
extern dataHans*       pGlobalBamenR;
extern dataHans*       pGlobalTianGan;
extern dataHans*       pGlobalSixtyJiaZi;
extern dataIndex*      pGlobalLiuJia;
extern dataHans*       pGlobalDiZhi;

// 日历结果数据区
extern calResult*      pGlobalResult;
extern char            szGlobalCurJieqi[STR_LEN_08];       // 传入日期当天的节气
extern char            szGlobalYuan[STR_LEN_08];           // 上中下三元
extern int             nGlobalJushu;                   // 0 自动计算, 其他是几局就是几局

dataHans* pGlobalYearCh = NULL;
dataHans* pGlobalMonCh = NULL;
dataHans* pGlobalDayCh = NULL;

void comlineInit()
{
    // 1. 数字大写内存分配
    if (pGlobalYearCh == NULL)
        dataAllocNumChinese(&pGlobalYearCh);
    // 2. 月份中文内存分配
    if (pGlobalMonCh == NULL)
        dataAllocLunarMonChinese(&pGlobalMonCh);
    // 3. 日子中文内存分配
    if (pGlobalDayCh == NULL)
        dataAllocLunarDayChinese(&pGlobalDayCh);
}

void comlineFree()
{
    // 1. 数字大写内存去分配
    if (pGlobalYearCh)
    {
        dataFreeDataHans(pGlobalYearCh);
        pGlobalYearCh = NULL;
    }
    // 2. 月份中文内存去分配
    if (pGlobalMonCh)
    {
        dataFreeDataHans(pGlobalMonCh);
        pGlobalMonCh = NULL;
    }
    // 3. 日子中文内存去分配
    if (pGlobalDayCh)
    {
        dataFreeDataHans(pGlobalDayCh);
        pGlobalDayCh = NULL;
    }
}
// 打印每宫格的第一行的内容
void PrintFirst(int nGongeIndex)
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    strcpy(szTem, &pGlobalBashenR->pHan->szData[STRINDEX(nReBashen[nGongeIndex])]);

    if (nReOther[nGongeIndex] == 0)
        printf(" %s        |", szTem);
    else
    {
        if (nReOther[nGongeIndex] > 10)
        {
            printf(" %s  (马空)|", szTem);
        }
        else
        {
            if (nReOther[nGongeIndex] == 1)
                printf(" %s   (空) |", szTem);
            else
                printf(" %s   (马) |", szTem);
        }
    } 
}
// 打印每宫格的第二行的内容
void PrintSecond(int nGongeIndex)
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    if (nReJiuXing[nGongeIndex] == 5)
    {
        if ((strcmp("天禽", nReZhiFuHan) == 0) || (strcmp(nReZhiFuHan, "天芮") == 0))
            printf("|*芮禽  ");
        else
            printf("| 芮禽  ");

        int nTe = nReTianPan[nGongeIndex];
        int nOne = nTe / 10;   // 天芮
        int nTwo = nTe % 10;   // 天禽

        char szTemTwo[STR_LEN_08];
        memset(szTemTwo, 0x0, sizeof(szTemTwo));
        strcpy(szTem, &pGlobalTianGan->pHan->szData[STRINDEX(nOne)]);
        strcpy(szTemTwo, &pGlobalTianGan->pHan->szData[STRINDEX(nTwo)]);

        if ((strcmp(szTem, nReTianGanHan) == 0) || (strcmp(szTemTwo, nReTianGanHan) == 0))
        {
            printf("[%s", szTem);
            printf("%s]", szTemTwo);
        }
        else
        {
            printf("  %s", szTem);
            printf("%s", szTemTwo);
        } 
    }
    else
    {
        strcpy(szTem, &pGlobalJiuxingR->pHan->szData[STRINDEX(nReJiuXing[nGongeIndex])]);
        if (strcmp(szTem, nReZhiFuHan) == 0)
            printf("|*%s   ", szTem);
        else
            printf("| %s   ", szTem);

        strcpy(szTem, &pGlobalTianGan->pHan->szData[STRINDEX(nReTianPan[nGongeIndex])]);
        if (strcmp(nReTianGanHan, szTem) != 0)
            printf(" %s  ", szTem);
        else
            printf("[%s] ", szTem);
    }
}
// 打印每宫格的第三行的内容
void PrintThird(int nGongeIndex)
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    strcpy(szTem, &pGlobalBamenR->pHan->szData[STRINDEX(nReBamen[nGongeIndex])]);

    if (strcmp(szTem, nReZhiShiHan) == 0)
        printf("|*%s    ", szTem);
    else
        printf("| %s    ", szTem);
    strcpy(szTem, &pGlobalTianGan->pHan->szData[STRINDEX(nReDiPan[nGongeIndex])]);
    printf("%s  ", szTem);
}
// 打印奇门盘面
void PrintQiMen()
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));

    memset(nReZhiFuHan, 0x0, sizeof(nReZhifu));
    memset(nReZhiShiHan, 0x0, sizeof(nReZhiShiHan));
    memset(nReTianGanHan, 0x0, sizeof(nReTianGanHan));
    strcpy(nReZhiFuHan, &pGlobalJiuxing->pHan->szData[STRINDEX(nReZhifu)]);
    strcpy(nReZhiShiHan, &pGlobalBamen->pHan->szData[STRINDEX(nReZhishi)]);
    int dindex = dataFindHanIndex(pGlobalSixtyJiaZi, pGlobalResult->pGanzi->szLunarDayGZ);
    if ((dindex % 10) == 0)
    {
        int tindex = pGlobalLiuJia->pIndex[dindex];
        strcpy(nReTianGanHan, &pGlobalTianGan->pHan->szData[STRINDEX(tindex)]);
    }
    else
        dataGetSubStr(pGlobalResult->pGanzi->szLunarDayGZ, nReTianGanHan, 0, 1);

    printf("|");
    PrintFirst(3);
    PrintFirst(8);
    PrintFirst(1);
    printf("\n");

    PrintSecond(3);
    PrintSecond(8);
    PrintSecond(1);
    printf("|\n");

    PrintThird(3);
    PrintThird(8);
    PrintThird(1);
    printf("|\n");

    LineLn();

    printf("|");
    PrintFirst(2);
    printf("             |");
    PrintFirst(6);
    printf("\n");

    PrintSecond(2);
    memset(szTem, 0x0, sizeof(szTem));
    strcpy(szTem, &pGlobalTianGan->pHan->szData[STRINDEX(nReDiPan[4])]);
    printf("|      %s     ", szTem);
    PrintSecond(6);
    printf("|\n");

    PrintThird(2);
    printf("|             ");
    PrintThird(6);
    printf("|\n");

    LineLn();

    printf("|");
    PrintFirst(7);
    PrintFirst(0);
    PrintFirst(5);
    printf("\n");

    PrintSecond(7);
    PrintSecond(0);
    PrintSecond(5);
    printf("|\n");

    PrintThird(7);
    PrintThird(0);
    PrintThird(5);
    printf("|\n");
}

// 打印结果
void PrintResult()
{
    comlineInit();
    LineLn();
    printf("公元:%04d年%02d月%02d日%02d时%02d分%02d秒 遁甲排盘cqm\n", 
                                            pGlobalResult->pSolar->nSolarYear,
                                            pGlobalResult->pSolar->nSolarMonth,
                                            pGlobalResult->pSolar->nSolarDay,
                                            pGlobalResult->pSolar->nSolarHour,
                                            pGlobalResult->pSolar->nSolarMin,
                                            pGlobalResult->pSolar->nSolarSecond);
    char strTemp[STR_LEN_08];
    LineLn();
    printf("农历:");

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalYearCh->pHan->szData[STRINDEX((pGlobalResult->pLunar->nLunarYear / 1000))]);
    printf("%s", strTemp);

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalYearCh->pHan->szData[STRINDEX(((pGlobalResult->pLunar->nLunarYear / 100) % 10))]); 
    printf("%s", strTemp);

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalYearCh->pHan->szData[STRINDEX(((pGlobalResult->pLunar->nLunarYear / 10) % 10))]);
    printf("%s", strTemp);

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalYearCh->pHan->szData[STRINDEX((pGlobalResult->pLunar->nLunarYear % 10))]);
    printf("%s", strTemp);
    printf("年 ");

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalMonCh->pHan->szData[STRINDEX(pGlobalResult->pLunar->nLunarMonth - 1)]);
    printf("%s", strTemp);

    memset(strTemp, 0x0, sizeof (strTemp));
    strcpy(strTemp, &pGlobalDayCh->pHan->szData[STRINDEX(pGlobalResult->pLunar->nLunarDay - 1)]);
    printf("%s", strTemp);
    printf("日 ");

    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 1, 1);
    printf("%s时\n", szTem);
    
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarYearGZ, szTem, 0, 1);
    printf("干支:%s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarMonthGZ, szTem, 0, 1);
    printf("%s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarDayGZ, szTem, 0, 1);
    printf("%s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 0, 1);
    printf("%s\n", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarYearGZ, szTem, 1, 1);
    printf("     %s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarMonthGZ, szTem, 1, 1);
    printf("%s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarDayGZ, szTem, 1, 1);
    printf("%s ", szTem);

    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 1, 1);

    switch (nStyle)
    {
    // 转盘超接法
    case 0:
        printf("%s             (转盘超接置润)\n", szTem);
        break;
    // 时家阴盘
    case 1:
    {
        // pGlobalTianGan->pHan->szData[STRINDEX(liuJiaIndex)]
        int nTemIndex = pGlobalResult->pLunar->nLunarMonth;
        printf("%s        (月将:%s)(时家阴盘)\n", szTem, &pGlobalDiZhi->pHan->szData[STRINDEX((12 - nTemIndex))]);
        break;
    }
    default:
        break;
    }
        
    LineLn();
    strcpy(szTem, &pGlobalJiuxing->pHan->szData[STRINDEX(nReZhifu)]);
    printf("值符:%s  ", szTem);
    strcpy(szTem, &pGlobalBamen->pHan->szData[STRINDEX(nReZhishi)]);
    printf("值使:%s   ", szTem);

    switch (nStyle)
    {
    case 0:
    {
        if (nGlobalAutoJushu == 1)
            printf("[%s%s]", szGlobalCurJieqi, szGlobalYuan);
        else
            printf("[手动定局]");
        break;
    }
    case 1:
    {
        printf("[%s%s空亡]", szReNullOne, szReNullTwo);
        break;
    }
    default:
        break;
    }
 
    if (nGlobalJushu < 0)
    {
        memset(szTem, 0x0, sizeof(szTem));
        strcpy(szTem, &pGlobalYearCh->pHan->szData[STRINDEX(-nGlobalJushu)]);
        printf("[阴遁%s局]", szTem);
    }
    else 
    {
        memset(szTem, 0x0, sizeof(szTem));
        strcpy(szTem, &pGlobalYearCh->pHan->szData[STRINDEX(nGlobalJushu)]);
        printf("[阳遁%s局]", szTem);
    }

    printf("\n");
    LineLn();
    
    //   ****************************  这里打印结果 ***********************************
    PrintQiMen();
    //   ****************************  这里打印结果 ***********************************

    LineLn();
    printf("[%02d-%02d %s %02d:%02d:%02d] ", pGlobalResult->pMonJieQi->nMonth,
                                            pGlobalResult->pMonJieQi->nFirstDay,
                                            pGlobalResult->pMonJieQi->szFirstJieQi,
                                            pGlobalResult->pMonJieQi->nFirstHour,
                                            pGlobalResult->pMonJieQi->nFirstMin,
                                            pGlobalResult->pMonJieQi->nFirstSec);
    printf("[%02d-%02d %s %02d:%02d:%02d]\n", pGlobalResult->pMonJieQi->nMonth,
                                            pGlobalResult->pMonJieQi->nSecondDay,
                                            pGlobalResult->pMonJieQi->szSecondJieQi,
                                            pGlobalResult->pMonJieQi->nSecHour,
                                            pGlobalResult->pMonJieQi->nSecMin,
                                            pGlobalResult->pMonJieQi->nSecSec);
    LineLn();
}

void BaseInfomation(const char* pErrorInfo)
{
    printf("==========================================================\n");
    if (pErrorInfo)
        printf("%s", pErrorInfo);
    printf("cqm %s 使用介绍:\n", CQM_VERSION);
    printf("cqm 第三个参数表明排盘方式(当前支持以下排盘方式):\n", CQM_VERSION);
    printf("        0 时家转盘超接置润盘\n");
    printf("        1 时家阴盘\n", CQM_VERSION);
    printf("\n");
    printf("以其中一个排盘方式为例=>\n");
    printf("示例1: cqm 0 0 0 (取当前时间，自动算局数，超接排盘)\n");
    printf("示例2: cqm 1 0 0 (取当前时间+8h，自动算局数，超接排盘)\n");
    printf("示例3: cqm 1992-6-15-18-31-12 0 0  (指定时间，自动算局数)\n");
    printf("示例4: cqm 1992-6-16-18-31-12 6 0  (指定时间，指定阳六局)\n");
    printf("示例5: cqm 1992-6-17-18-31-12 -6 0 (指定时间，指定阴六局)\n");
    printf("\n说明:  cqm 使用时间范围:\n");
    printf("           起始时间: 1901年01月01日00时00分00秒\n");
    printf("           结束时间: 2098年12月31日23时59分59秒\n");
    printf("==========================================================\n");
}

// 日期不合法打印通知
void InlegalTime()
{
    char szTem[STR_LEN_128];
    memset(szTem, 0x0, sizeof(szTem));
    sprintf(szTem, "时间范围不合法或者日期格式不正确。\n\n");
    BaseInfomation(szTem);
}

// 传入命令行相关参数，当返回值为 0 时，pSolar 中保存解析出来的日期
// 其他失败，命令行格式: xx 0 0 或者 xx 1995-4-1-4-5-34 9
int  ParseCommand(int nArgc, char** pArgv, calSolar* pSolar, int* pnJushu, int* pbIsAutoTime, int* nStyle)
{
    char szTem[STR_LEN_128];
    memset(szTem, 0x0, sizeof(szTem));
    if (nArgc == 1)
    {
        sprintf(szTem, "奇门遁甲本地离线起盘工具      https://gitee.com/taynpg/cqm\n\n");
        BaseInfomation(szTem);
        return -1;
    }
    if (nArgc != 4)
    {
        sprintf(szTem, "参数的个数不正确，应为3个。\n\n");
        BaseInfomation(szTem);
        return -1;
    }
    int nParseRe = 0;
    int nTem = -1;
    // 解析第一个参数
    nParseRe = sscanf((const char*)pArgv[1], "%d", &nTem);
    if (nParseRe == 1)
    {
        if (nTem == 0)
            (*pbIsAutoTime) = 1;
        else if (nTem == 1)
            (*pbIsAutoTime) = 2;
        else
        {
            (*pbIsAutoTime) = 0;
            nParseRe = 0;
            nParseRe = sscanf((const char*)pArgv[1], "%d-%d-%d-%d-%d-%d",
                                &(pSolar->nSolarYear),
                                &(pSolar->nSolarMonth),
                                &(pSolar->nSolarDay),
                                &(pSolar->nSolarHour),
                                &(pSolar->nSolarMin),
                                &(pSolar->nSolarSecond));
            if (nParseRe != 6)
            {
                sprintf(szTem, "参数1的格式不正确。\n\n");
                BaseInfomation(szTem);
                return -2;
            }
        }
    }
    else
    {
        sprintf(szTem, "参数1的格式不正确。\n\n");
        BaseInfomation(szTem);
        return -2;
    }

    nParseRe = 0;
    // 解析第二个参数
    nParseRe = sscanf((const char*)pArgv[2], "%d", pnJushu);
    if (nParseRe != 1)
    {
        sprintf(szTem, "参数2格式不正确，范围 -9 ~ 9。\n\n");
        BaseInfomation(szTem);
        return -2;
    }
    if (*pnJushu > 9 || *pnJushu < -9)
    {
        sprintf(szTem, "参数2值不正确，范围 -9 ~ 9。\n\n");
        BaseInfomation(szTem);
        return -2;
    }

    nParseRe = 0;
    // 解析第三个参数
    nParseRe = sscanf((const char*)pArgv[3], "%d", nStyle);
    if (nParseRe != 1)
    {
        sprintf(szTem, "参数3格式不正确，目前支持 0、1。\n\n");
        BaseInfomation(szTem);
        return -2;
    }
    if (*nStyle != 0 && *nStyle != 1)
    {
        sprintf(szTem, "参数3值不正确，目前支持 0、1。\n\n");
        BaseInfomation(szTem);
        return -2;
    }
    return 0;
}
 

