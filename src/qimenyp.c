#include <qimenyp.h>
#include <stdlib.h>
#include <string.h>

extern calResult* pGlobalResult;            // 日历结算结果区

// ===  基础数据区
extern dataHans*    pGlobalJieQiTable;
extern dataHans*    pGlobalTianGan;
extern dataHans*    pGlobalDiZhi;
extern dataHans*    pGlobalSixtyJiaZi;
extern dataIndex*   pGlobalLunarTabl;
extern dataIndex*   pGlobalJieQiTime;
extern dataIndex*   pGlobalWuHu;
extern dataIndex*   pGlobalQiShi;

extern int          nGlobalAutoJushu;
extern dataHans*    pGlobalJiuxing;
extern dataHans*    pGlobalBamen;
extern dataHans*    pGlobalJiuxingR;
extern dataHans*    pGlobalBamenR;
extern dataHans*    pGlobalBashenR;
extern dataIndex*   pGlobalJuAndQi;
extern dataIndex*   pGlobalLiuJia;
extern dataIndex*   pGlobalDiZhiGong;
extern dataIndex*   pGlobalDiZhiChong;
extern dataIndex*   pGlobalDiZhiSanhe;

// 计算解结果区
extern int          nReDiPan[9];                        // 原始下标
extern int          nReTianPan[9];
extern int          nReJiuXing[9];                      // 旋转下标
extern int          nReBamen[9];                        // 旋转下标           
extern int          nReBashen[9];                       // 旋转下标
extern int          nReOther[9];                        // 内容为 0 表示没有，1 为空，2为马星
extern int          nReZhifu;
extern int          nReZhishi;

extern int          bGlobalZhirun;                      // 0 不置润，其他置润
extern int          bGlobalAutoTime;                    // 0 手动输入时间，其他自动获取时间
extern int          nGlobalJushu;                       // 0 自动计算, 其他是几局就是几局
extern char         szGlobalYuan[STR_LEN_08];           // 上中下三元
extern char         szGlobalCurJieqi[STR_LEN_08];       // 传入日期当天的节气
extern char         szGlobalCurGanzi[STR_LEN_08];       // 传入日期当天的日干支
extern int          nOneCircle[8];                      // 九宫的顺时针旋转下标顺序(从坤2宫开始)

int qimenYpRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu)
{
    qimenInit();

    int nRet = qimenParmCheck(pSolar, bIsAutoTime, qimenJuShu);
    if (nRet != 0)
        return nRet;

    // 定局数
    generateJushuYp(pSolar);
    // 地盘排布
    GenerateZpZrDiPan();
    // 算当日日期
    calendarRun(pSolar);
    // 查找值符值使
    FindZhiShiFuZpZr();
    // 九星
    GenerateXingZpZr();
    // 八门
    GenerateMenZpZr();
    // 计算八神
    GenerateBaShenZpZr();
    // 计算天盘
    GenerateTianPanZpZr();
    // 计算旬空、马星
    GenerateOtherZpZr();

    return 0;
}

// 计算局数
void generateJushuYp(const calSolar* pSolar)
{
    int nJu = 0;
    // 计算当日日历
    calendarRun(pSolar);
    // 1. 查找年干支的地支
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarYearGZ, szTem, 1, 1);
    int nReA = dataFindHanIndex(pGlobalDiZhi, szTem) + 1;
    // 2. 阴历月份 日子
    int nReB = pGlobalResult->pLunar->nLunarMonth;
    int nReC = pGlobalResult->pLunar->nLunarDay;
    // 3. 时辰数
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 1, 1);
    int nReD = dataFindHanIndex(pGlobalDiZhi, szTem) + 1;

    int nSum = nReA + nReB + nReC + nReD;
    nJu = nSum % 9;
    if (nJu == 0)
        nJu = 9;

    // 完成定局
    nGlobalJushu = nJu;

    // 判断阴阳遁
    // 1. 先查找当年的冬至时间
    calSolar dongzhiTime;
    dongzhiTime.nSolarYear = pSolar->nSolarYear;
    dongzhiTime.nSolarMonth = 12;
    dongzhiTime.nSolarDay = 15;
    dongzhiTime.nSolarHour = 12;
    dongzhiTime.nSolarMin = 12;
    dongzhiTime.nSolarSecond = 12;
    calendarRun(&dongzhiTime);
    dongzhiTime.nSolarDay = pGlobalResult->pMonJieQi->nSecondDay;
    dongzhiTime.nSolarHour = pGlobalResult->pMonJieQi->nSecHour;
    dongzhiTime.nSolarMin = pGlobalResult->pMonJieQi->nSecMin;
    dongzhiTime.nSolarSecond = pGlobalResult->pMonJieQi->nSecSec;
    // 2. 查找当年的夏至时间
    calSolar xiazhiTime;
    xiazhiTime.nSolarYear = pSolar->nSolarYear;
    xiazhiTime.nSolarMonth = 6;
    xiazhiTime.nSolarDay = 15;
    xiazhiTime.nSolarHour = 12;
    xiazhiTime.nSolarMin = 12;
    xiazhiTime.nSolarSecond = 12;
    calendarRun(&xiazhiTime);
    xiazhiTime.nSolarDay = pGlobalResult->pMonJieQi->nSecondDay;
    xiazhiTime.nSolarHour = pGlobalResult->pMonJieQi->nSecHour;
    xiazhiTime.nSolarMin = pGlobalResult->pMonJieQi->nSecMin;
    xiazhiTime.nSolarSecond = pGlobalResult->pMonJieQi->nSecSec;

    // ------- 阳遁 ----- 夏至 ---- 阴遁 -------- 冬至 ----- 阳遁
    // 根据当日日期和夏至冬至日期判断阴阳遁
    if (GetSecondDiviTwoDate(pSolar, &xiazhiTime) > 0 &&
    GetSecondDiviTwoDate(pSolar, &dongzhiTime) <= 0)
    {
        // 当日日期在夏至之后冬至之前则为阴遁
        nGlobalJushu = -nGlobalJushu;
    }
}