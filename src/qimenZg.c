#include <qimenZg.h>

extern calResult* pGlobalResult;            // 日历结算结果区

// ===  基础数据区
extern dataHans*    pGlobalJieQiTable;
extern dataHans*    pGlobalTianGan;
extern dataHans*    pGlobalDiZhi;
extern dataHans*    pGlobalSixtyJiaZi;
// extern dataIndex*   pGlobalLunarTabl;
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
extern dataIndex*   pGlobalSanheJu;

// 计算解结果区
extern int          nReDiPan[9];                        // 原始下标
extern int          nReTianPan[9];
extern int          nReJiuXing[9];                      // 旋转下标
extern int          nReBamen[9];                        // 旋转下标           
extern int          nReBashen[9];                       // 旋转下标
extern int          nReOther[9];                        // 内容为 0 表示没有，1 为空，2为马星
extern int          nReZhifu;
extern int          nReZhishi;
extern int          nGongNum[9];                        // 九宫各宫位代表的数字

extern int          bGlobalZhirun;                      // 0 不置润，其他置润
extern int          bGlobalAutoTime;                    // 0 手动输入时间，其他自动获取时间
extern int          nGlobalJushu;                       // 0 自动计算, 其他是几局就是几局
extern char         szGlobalYuan[STR_LEN_08];           // 上中下三元
extern char         szGlobalCurJieqi[STR_LEN_08];       // 传入日期当天的节气
extern char         szGlobalCurGanzi[STR_LEN_08];       // 传入日期当天的日干支
extern int          nOneCircle[8];                      // 九宫的顺时针旋转下标顺序(从坤2宫开始)

int qimenZgRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu)
{
    qimenInit();

    int nRet = qimenParmCheck(pSolar, bIsAutoTime, qimenJuShu);
    if (nRet != 0)
        return nRet;

    // 定局数
    generateJushuYp(pSolar);
    // 旋转原始宫的数字定义
    basicNumberTurned(-1);
    // 地盘排布
    GenerateZpZrDiPan();
    // 算当日日期
    calendarRun(pSolar);
    // 查找值符值使
    FindZhiShiFuZpZr();
    // 九星
    GenerateXingZpZr();
    // 计算天盘
    GenerateTianPanZpZr();
    // 转动八门
    GenerateMenZg();
    // 计算八神
    GenerateBaShenZpZr();
    // 计算旬空、马星
    GenerateOtherZpZr();

    return 0;
}

// 诸葛盘八门排盘
void GenerateMenZg()
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 1, 1);

    // 查看地支的数字
    int nDizhi = dataFindHanIndex(pGlobalDiZhi, szTem);
    int num = pGlobalSanheJu->pIndex[nDizhi];

    if (nGlobalJushu < 0)
        num = num % 10;
    else
        num = num / 10;

    // 拿到数字后，查看此数字在哪一个宫位
    int nGong = GetIndexFromArray(nGongNum, 9, num - 1);
    // 从此宫位开始 排景门
    // 先查看景门的位置
    int startMen = dataFindHanIndex(pGlobalBamenR, "景门");
    int nCircleIndex = GetIndexFromArray(nOneCircle, 9, nGong);

    for (int i = 0; i < 8; ++i, ++startMen, ++nCircleIndex)
        nReBamen[nOneCircle[GetRemainder(nCircleIndex, 8)]] = GetRemainder(startMen, 8);

    return;
}
