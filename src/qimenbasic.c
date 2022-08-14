#include <qimenbasic.h>

int                 nGlobalAutoJushu = 1;
dataHans*           pGlobalJiuxing = NULL;
dataHans*           pGlobalBamen = NULL;
dataHans*           pGlobalJiuxingR = NULL;
dataHans*           pGlobalBamenR = NULL;
dataHans*           pGlobalBashenR = NULL;
dataIndex*          pGlobalJuAndQi = NULL;
dataIndex*          pGlobalLiuJia = NULL;
dataIndex*          pGlobalDiZhiGong = NULL;
dataIndex*          pGlobalDiZhiChong = NULL;
dataIndex*          pGlobalDiZhiSanhe = NULL;

int                 bGlobalZhirun = 0;                  // 0 不置润，其他置润
int                 bGlobalAutoTime = 1;                // 0 手动输入时间，其他自动获取时间
int                 nGlobalJushu = 0;                   // 0 自动计算, 其他是几局就是几局
char                szGlobalYuan[STR_LEN_08];           // 上中下三元
char                szGlobalCurJieqi[STR_LEN_08];       // 传入日期当天的节气
char                szGlobalCurGanzi[STR_LEN_08];       // 传入日期当天的日干支
int                 nOneCircle[8];                      // 九宫的顺时针旋转下标顺序(从坤2宫开始)

char                szReNullOne[STR_LEN_08];            // 空亡一
char                szReNullTwo[STR_LEN_08];            // 空亡二

// 计算解结果区
int                 nReDiPan[9];                        // 原始下标
int                 nReTianPan[9];
int                 nReJiuXing[9];                      // 旋转下标
int                 nReBamen[9];                        // 旋转下标           
int                 nReBashen[9];                       // 旋转下标
int                 nReOther[9];                        // 内容为 0 表示没有，1 为空，2为马星
int                 nReZhifu;
int                 nReZhishi;

void qimenInit()
{
    nReZhifu = -1;
    nReZhishi = -1;

    bGlobalZhirun = 0;
    bGlobalAutoTime = 1;
    nGlobalJushu = 0;

    nOneCircle[0] = 1; nOneCircle[1] = 6; nOneCircle[2] = 5;
    nOneCircle[3] = 0; nOneCircle[4] = 7; nOneCircle[5] = 2;
    nOneCircle[6] = 3; nOneCircle[7] = 8; 

    for (int i = 0; i < 9; ++i)
    {
        nReJiuXing[i] = -1;
        nReBamen[i] = -1;
        nReBashen[i] = -1;
    }
    // 置三元为空
    memset(szGlobalYuan, 0x0, sizeof(szGlobalYuan));
    memset(szGlobalCurJieqi, 0x0, sizeof(szGlobalCurJieqi));
    memset(szGlobalCurGanzi, 0x0, sizeof(szGlobalCurGanzi));
    // 1. 初始化九星
    if (pGlobalJiuxing == NULL)
        dataAllocJiuxing(&pGlobalJiuxing);
    // 2. 初始化旋转九星
    if (pGlobalJiuxingR == NULL)
        dataAllocJiuXingR(&pGlobalJiuxingR);
    // 3. 初始化八门
    if (pGlobalBamen == NULL)
        dataAllocBamen(&pGlobalBamen);
    // 4. 初始化旋转八门
    if (pGlobalBamenR == NULL)
        dataAllocBamenR(&pGlobalBamenR);
    // 5. 初始化八神
    if (pGlobalBashenR == NULL)
        dataAllocBashenR(&pGlobalBashenR);
    // 6. 初始化六甲
    if (pGlobalLiuJia == NULL)
        dataAllocLiuJia(&pGlobalLiuJia);
    // 7. 初始化局和节气
    if (pGlobalJuAndQi == NULL)
        dataAllocJuAndQi(&pGlobalJuAndQi);
    // 8. 初始化地支所在宫位
    if (pGlobalDiZhiGong == NULL)
        dataAllocDizhiGong(&pGlobalDiZhiGong);
    // 9. 初始化地支相冲
    if (pGlobalDiZhiChong == NULL)
        dataAllocDizhiChong(&pGlobalDiZhiChong);
    //10. 初始化地支三和
    if (pGlobalDiZhiSanhe == NULL)
        dataAllocDizhiSanhe(&pGlobalDiZhiSanhe);
}

void qimenFree()
{
    // 1. 去初始化九星表
    if (pGlobalJiuxing)
    {
        dataFreeDataHans(pGlobalJiuxing);
        pGlobalJiuxing = NULL;
    }
    // 2. 去初始化九星旋转
    if (pGlobalJiuxingR)
    {
        dataFreeDataHans(pGlobalJiuxingR);
        pGlobalJiuxingR = NULL;
    }
    // 3. 去初始化八门
    if (pGlobalBamen)
    {
        dataFreeDataHans(pGlobalBamen);
        pGlobalBamen = NULL;
    }
    // 4. 去初始化八门旋转
    if (pGlobalBamenR)
    {
        dataFreeDataHans(pGlobalBamenR);
        pGlobalBamenR = NULL;
    }
    // 5. 去初始化八神
    if (pGlobalBashenR)
    {
        dataFreeDataHans(pGlobalBashenR);
        pGlobalBashenR = NULL;
    }
    // 6. 去初始化六甲
    if (pGlobalLiuJia)
    {
        dataFreeIndexCounter(pGlobalLiuJia);
        pGlobalLiuJia = NULL;
    }
    // 7. 去初始化局和节气
    if (pGlobalJuAndQi)
    {
        dataFreeIndexCounter(pGlobalJuAndQi);
        pGlobalJuAndQi = NULL;
    }
    // 8. 去初始化地支所在宫位
    if (pGlobalDiZhiGong)
    {
        dataFreeIndexCounter(pGlobalDiZhiGong);
        pGlobalDiZhiGong = NULL;
    }
    // 8. 去初始化地支相冲
    if (pGlobalDiZhiChong)
    {
        dataFreeIndexCounter(pGlobalDiZhiChong);
        pGlobalDiZhiChong = NULL;
    }
    // 9. 去初始化地支三和
    if (pGlobalDiZhiSanhe)
    {
        dataFreeIndexCounter(pGlobalDiZhiSanhe);
        pGlobalDiZhiSanhe = NULL;
    }
}