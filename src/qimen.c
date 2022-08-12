#include <qimen.h>
#include <stdlib.h>
#include <string.h>

// 日期和节气日干支结构体
typedef struct _dateJieQiGan
{
    calSolar                    pSolar;
    char                        szJieQi[STR_LEN_08];
    char                        szDayGanzhi[STR_LEN_08];
    struct _dateJieQiGan*       pNext;
}dateJieQiGan;

// 初始化日期和节气日干支结构体链表头
void InitJieQiGan(dateJieQiGan** pHeader)
{
    (*pHeader) = (dateJieQiGan *)malloc(sizeof(dateJieQiGan));
    (*pHeader)->pSolar.nSolarYear = 2021;
    (*pHeader)->pSolar.nSolarMonth = 12;
    (*pHeader)->pSolar.nSolarDay = 26;
    (*pHeader)->pSolar.nSolarHour = 17;
    (*pHeader)->pSolar.nSolarMin = 0;
    (*pHeader)->pSolar.nSolarSecond = 0;
    memset((*pHeader)->szDayGanzhi, 0x0, sizeof((*pHeader)->szDayGanzhi));
    memset((*pHeader)->szJieQi, 0x0, sizeof((*pHeader)->szJieQi));
    (*pHeader)->pNext = NULL;
}
// 向日期和节气日干支结构体中插入一条记录
void InsertJieQiGan(dateJieQiGan** pJieQiGanHeader, const calSolar* pSolar, const char* szJieqi, const char* szDayGanzhi)
{
    dateJieQiGan* h = (*pJieQiGanHeader);
    dateJieQiGan* d = (dateJieQiGan *)malloc(sizeof(dateJieQiGan));

    d->pSolar.nSolarYear = pSolar->nSolarYear;
    d->pSolar.nSolarMonth = pSolar->nSolarMonth;
    d->pSolar.nSolarDay = pSolar->nSolarDay;
    d->pSolar.nSolarHour = pSolar->nSolarHour;
    d->pSolar.nSolarMin = pSolar->nSolarMin;
    d->pSolar.nSolarSecond = pSolar->nSolarSecond;
    strcpy(d->szJieQi, szJieqi);
    strcpy(d->szDayGanzhi, szDayGanzhi);
    d->pNext = NULL;

    // 找到链表尾
    while (h->pNext != NULL)
        h = h->pNext;
    
    h->pNext = d;
}
// 打印 JieQiGan
void  PrintJieQiGan(dateJieQiGan* pDateJieQiGan)
{
    if (pDateJieQiGan == NULL)
        return;
    dateJieQiGan* p = pDateJieQiGan;
    while (p->pNext != NULL)
    {
        printf("%04d-%02d-%02d %s%s\n",p->pSolar.nSolarYear,
                                    p->pSolar.nSolarMonth,
                                    p->pSolar.nSolarDay, p->szJieQi, p->szDayGanzhi);
        p = p->pNext;
    }
}
// 根据日期(不计算时分秒)查找当天的节气范围和当日干支
int SearchJieQiGan(dateJieQiGan* pDateJieQiGan, const calSolar* pSolar, char* pszJieqi, char* pszGanzhi)
{
    if (!pDateJieQiGan)
        return -1;
    dateJieQiGan* p = pDateJieQiGan->pNext;

    int rtn = -1;

    if (p == NULL)
        return rtn;

    while (p)
    {
        if (p->pSolar.nSolarYear == pSolar->nSolarYear &&
            p->pSolar.nSolarMonth == pSolar->nSolarMonth &&
            p->pSolar.nSolarDay == pSolar->nSolarDay)
        {
            memset(pszJieqi, 0x0, strlen(pszJieqi));
            memset(pszGanzhi, 0x0, strlen(pszGanzhi));
            strcpy(pszJieqi, p->szJieQi);
            strcpy(pszGanzhi, p->szDayGanzhi);
            rtn = 0;
            break;
        }
        p = p->pNext;
    }
    return rtn;
}
// 释放日期和节气日干支结构体链表内存
void FreeJieQiGan(dateJieQiGan* pDateJieQiGan)
{
    if (pDateJieQiGan == NULL)
        return;
    if (pDateJieQiGan->pNext != NULL)
        FreeJieQiGan(pDateJieQiGan->pNext);
    free(pDateJieQiGan);
}
// 查找一个整数在某个整数数组中的下标
int GetNumIndex(int* pData, int nLen, int nKey)
{
    int rtn = -1;
    for (int i = 0; i < nLen; ++i)
    {
        if (pData[i] == nKey)
        {
            rtn = i;
            break;
        }
    }
    return rtn;
}
// ===  基础数据区
extern dataHans* pGlobalJieQiTable;
extern dataHans* pGlobalTianGan;
extern dataHans* pGlobalDiZhi;
extern dataHans* pGlobalSixtyJiaZi;
extern dataIndex* pGlobalLunarTabl;
extern dataIndex* pGlobalJieQiTime;
extern dataIndex* pGlobalWuHu;
extern dataIndex* pGlobalQiShi;

int             nGlobalAutoJushu = 1;
dateJieQiGan*   pGlobalJieQiGan = NULL;             // 日期和节气日干支表
dataHans*       pGlobalJiuxing = NULL;
dataHans*       pGlobalBamen = NULL;
dataHans*       pGlobalJiuxingR = NULL;
dataHans*       pGlobalBamenR = NULL;
dataHans*       pGlobalBashenR = NULL;
dataIndex*      pGlobalJuAndQi = NULL;
dataIndex*      pGlobalLiuJia = NULL;
dataIndex*      pGlobalDiZhiGong = NULL;
dataIndex*      pGlobalDiZhiChong = NULL;
dataIndex*      pGlobalDiZhiSanhe = NULL;

// ===  结果数据区
extern calResult* pGlobalResult;

int             bGlobalZhirun = 0;                  // 0 不置润，其他置润
int             bGlobalAutoTime = 1;                // 0 手动输入时间，其他自动获取时间
int             nGlobalJushu = 0;                   // 0 自动计算, 其他是几局就是几局
char            szGlobalYuan[STR_LEN_08];           // 上中下三元
char            szGlobalCurJieqi[STR_LEN_08];       // 传入日期当天的节气
char            szGlobalCurGanzi[STR_LEN_08];       // 传入日期当天的日干支
int             nOneCircle[8];                      // 九宫的顺时针旋转下标顺序(从坤2宫开始)

// 计算解结果区
int             nReDiPan[9];                        // 原始下标
int             nReTianPan[9];
int             nReJiuXing[9];                      // 旋转下标
int             nReBamen[9];                        // 旋转下标           
int             nReBashen[9];                       // 旋转下标
int             nReOther[9];                        // 内容为 0 表示没有，1 为空，2为马星
int             nReZhifu;
int             nReZhishi;

void GenerateJieQiGan(const calSolar* pSolar);      // 生成日期和节气日干支表
int GenerateYuan(const calSolar* pSolar);           // 确定阴阳遁和三元

void QimenInit()
{
    nReZhifu = -1;
    nReZhishi = -1;

    nOneCircle[0] = 1; nOneCircle[1] = 6; nOneCircle[2] = 5;
    nOneCircle[3] = 0; nOneCircle[4] = 7; nOneCircle[5] = 2;
    nOneCircle[6] = 3; nOneCircle[7] = 8; 

    for (int i = 0; i < 9; ++i)
    {
        nReJiuXing[i] = -1;
        nReBamen[i] = -1;
        nReBashen[i] = -1;
    }

    memset(szGlobalYuan, 0x0, sizeof(szGlobalYuan));
    memset(szGlobalCurJieqi, 0x0, sizeof(szGlobalCurJieqi));
    memset(szGlobalCurGanzi, 0x0, sizeof(szGlobalCurGanzi));
    // 1. 初始化九星表
    if (pGlobalJiuxing == NULL)
        dataAllocJiuxing(&pGlobalJiuxing);
    if (pGlobalJiuxingR == NULL)
        dataAllocJiuXingR(&pGlobalJiuxingR);
    // 2. 初始化八门
    if (pGlobalBamen == NULL)
        dataAllocBamen(&pGlobalBamen);
    if (pGlobalBamenR == NULL)
        dataAllocBamenR(&pGlobalBamenR);
    // 3. 初始化八神
    if (pGlobalBashenR == NULL)
        dataAllocBashenR(&pGlobalBashenR);
    // 4. 初始化六甲
    if (pGlobalLiuJia == NULL)
        dataAllocLiuJia(&pGlobalLiuJia);
    // 5. 初始化局和节气
    if (pGlobalJuAndQi == NULL)
        dataAllocJuAndQi(&pGlobalJuAndQi);
    if (pGlobalDiZhiGong == NULL)
        dataAllocDizhiGong(&pGlobalDiZhiGong);
    if (pGlobalDiZhiChong == NULL)
        dataAllocDizhiChong(&pGlobalDiZhiChong);
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
    if (pGlobalJiuxingR)
    {
        dataFreeDataHans(pGlobalJiuxingR);
        pGlobalJiuxingR = NULL;
    }
    // 2. 去初始化八门
    if (pGlobalBamen)
    {
        dataFreeDataHans(pGlobalBamen);
        pGlobalBamen = NULL;
    }
    if (pGlobalBamenR)
    {
        dataFreeDataHans(pGlobalBamenR);
        pGlobalBamenR = NULL;
    }
    // 3. 去初始化八神
    if (pGlobalBashenR)
    {
        dataFreeDataHans(pGlobalBashenR);
        pGlobalBashenR = NULL;
    }
    // 4. 去初始化六甲
    if (pGlobalLiuJia)
    {
        dataFreeIndexCounter(pGlobalLiuJia);
        pGlobalLiuJia = NULL;
    }
    // 5. 去初始化局和节气
    if (pGlobalJuAndQi)
    {
        dataFreeIndexCounter(pGlobalJuAndQi);
        pGlobalJuAndQi = NULL;
    }
    if (pGlobalDiZhiGong)
    {
        dataFreeIndexCounter(pGlobalDiZhiGong);
        pGlobalDiZhiGong = NULL;
    }
    if (pGlobalDiZhiChong)
    {
        dataFreeIndexCounter(pGlobalDiZhiChong);
        pGlobalDiZhiChong = NULL;
    }
    if (pGlobalDiZhiSanhe)
    {
        dataFreeIndexCounter(pGlobalDiZhiSanhe);
        pGlobalDiZhiSanhe = NULL;
    }
    calendarFreeAll();
    FreeJieQiGan(pGlobalJieQiGan);
    pGlobalJieQiGan = NULL;
}
// 计算地盘
void GenerateDiPan()
{
    memset(nReDiPan, 0x0, sizeof(nReDiPan));
    int nT = -1;
    if (nGlobalJushu < 0)
        nT = -nGlobalJushu;
    else
        nT = nGlobalJushu;
    int si = GetRemainder(nT - 1, 9);  
    nReDiPan[si] = 4;   // 4 - 戊
    if (nGlobalJushu < 0)
    {
        for (int i = 0; i < 5; ++i)
        {
            --si;
            nReDiPan[GetRemainder(si, 9)] = 5 + i;
        }
        for (int i = 0; i < 3; ++i)
        {
            --si;
            nReDiPan[GetRemainder(si, 9)] = 3 - i;
        }
    }
    else
    {
        for (int i = 0; i < 5; ++i)
        {
            ++si;
            nReDiPan[GetRemainder(si, 9)] = 5 + i;
        }
        for (int i = 0; i < 3; ++i)
        {
            ++si;
            nReDiPan[GetRemainder(si, 9)] = 3 - i;
        }
    }
}
// 找值符、直使
void FindZhiShiFu()
{
    int hindex = dataFindHanIndex(pGlobalSixtyJiaZi, pGlobalResult->pGanzi->szLunarHourGZ);
    int futou = (hindex / 10) * 10;

    // 查看符头对应的六仪
    int nLiuyi = pGlobalLiuJia->pIndex[futou];
    int reIndex = GetNumIndex(nReDiPan, 9, nLiuyi);
    // 值符、值使得宫位(0~8), 此宫位原九星八门的星门
    nReZhifu = reIndex;
    nReZhishi = reIndex;
}
// 计算八神
void GenerateBaShen()
{
    // "值符", "腾蛇", "太阴", "六合", "白虎", "玄武", "九地", "九天"
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 0, 1);
    int liuJiaIndex = 0;
    if (strcmp(szTem, "甲") == 0)
    {
        int hourIndex = dataFindHanIndex(pGlobalSixtyJiaZi, pGlobalResult->pGanzi->szLunarHourGZ);
        liuJiaIndex = pGlobalLiuJia->pIndex[hourIndex];
        strcpy(szTem, &pGlobalTianGan->pHan->szData[STRINDEX(liuJiaIndex)]);
    }
    // 1. 看天干在数据中的索引
    int sindex = dataFindHanIndex(pGlobalTianGan, szTem);
    int dindex = GetNumIndex(nReDiPan, 9, sindex);

    int tindex = dindex;
    if (tindex == 4)
        tindex = 1;
    int nstart = 0;
    for (int i = 0; i < 8; ++i, ++nstart)
    {
        if (tindex == nOneCircle[i])
            break;
    }
    int pindex = 0;
    if (nGlobalJushu > 0)
    {
        for (int i = 0; i < 8; ++i, ++nstart)
            nReBashen[nOneCircle[nstart % 8]] = i;
    }
    else
    {
        for (int i = 0; i < 8; ++i, --nstart)
            nReBashen[nOneCircle[GetRemainder(nstart, 8)]] = i;
    }
}
// 转动九星八门
void GenerateXingMen()
{
    char szTem[STR_LEN_08];
    memset(szTem, 0x0, sizeof(szTem));

    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTem, 0, 1);
    // 如果时辰天干为甲，则伏吟
    if (strcmp(szTem, "甲") == 0)
    {
        char szTemTwo[STR_LEN_08];
        int nTem = -1;
        for (int i = 0; i < 9; ++i)
        {
            if (i == 4)
                continue;
            memset(szTemTwo, 0x0, sizeof(szTemTwo));
            strcpy(szTemTwo, &pGlobalJiuxing->pHan->szData[STRINDEX(i)]);
            nTem = dataFindHanIndex(pGlobalJiuxingR, szTemTwo);
            nReJiuXing[i] = nTem;
        }
        for (int i = 0; i < 9; ++i)
        {
            if (i == 4)
                continue;
            memset(szTemTwo, 0x0, sizeof(szTemTwo));
            strcpy(szTemTwo, &pGlobalBamen->pHan->szData[STRINDEX(i)]);
            nTem = dataFindHanIndex(pGlobalBamenR, szTemTwo);
            nReBamen[i] = nTem;
        }
        return;
    }
    // 查看该时辰天干的存储索引
    int hi = dataFindHanIndex(pGlobalTianGan, szTem);
    // 查看该时辰天干的地盘索引 (hid 是值符落宫的位置)
    int hid = GetNumIndex(nReDiPan, 9, hi);
    int nTempIndex = hid;
    if (nTempIndex == 4)
        nTempIndex = 1;
    
    strcpy(szTem, &pGlobalJiuxing->pHan->szData[STRINDEX(nReZhifu)]);
    if (strcmp("天禽", szTem) == 0)
        strcpy(szTem, "天芮");
    int rsindex = dataFindHanIndex(pGlobalJiuxingR, szTem);
    int nZhifu = rsindex;
    
    nReJiuXing[nTempIndex] = nZhifu;
    int nStart = 0;
    for (int i = 0; i < 8; ++i, ++nStart)
    {
        if (nTempIndex == nOneCircle[i])
            break;
    }
    int nNextXingIndex = -1;
    strcpy(szTem, &pGlobalJiuxing->pHan->szData[STRINDEX(nZhifu)]);
    nNextXingIndex = dataFindHanIndex(pGlobalJiuxingR, szTem);
    // 蓬、任、冲、辅、英、芮、柱、心
    for (int i = 0; i < 8; ++i)
    {
        nNextXingIndex = (++nZhifu) % 8;
        ++nStart;
        nReJiuXing[nOneCircle[nStart % 8]] = nNextXingIndex;
    }

    // 九星排列完成，接下来是八门
    int hindex = dataFindHanIndex(pGlobalSixtyJiaZi, pGlobalResult->pGanzi->szLunarHourGZ);
    int futou = (hindex / 10) * 10;
    // 旬头对应的六仪
    int nLiuYi = pGlobalLiuJia->pIndex[futou];
    int nGongwei = GetNumIndex(nReDiPan, 9, nLiuYi);
    // 从 nLiuYi 开始向后排列
    int nTemp = 0;
    if (nGlobalJushu < 0)
    {
        // 阴遁
        for (int i = 0; i < 9; ++i)
        {
            if (futou == hindex)
                break;        
            --nGongwei;
            ++futou;
            nTemp = GetRemainder(nGongwei, 9);
        }
    }
    else
    {
        // 阳遁
        for (int i = 0; i < 9; ++i)
        {
            if (futou == hindex)
                break;        
            ++nGongwei;
            ++futou;
            nTemp = GetRemainder(nGongwei, 9);
        }
    }
    if (nTemp == 4)
        nTemp = 1;

    nStart = 0;
    for (int i = 0; i < 8; ++i, ++nStart)
    {
        if (nTemp == nOneCircle[i])
            break;
    }

    int menIndex = 0;
    // 找出门的下标
    strcpy(szTem, &pGlobalBamen->pHan->szData[STRINDEX(nReZhishi)]);
    char szTem2[STR_LEN_08];
    for (int i = 0; i < 8; ++i, ++menIndex)
    {
        memset(szTem2, 0x0, sizeof(szTem2));
        strcpy(szTem2, &pGlobalBamenR->pHan->szData[STRINDEX(i)]);
        if (strcmp(szTem, szTem2) == 0)
            break;
    }
    // 休、生、伤、杜、景、死、惊、开
    for (int i = 0; i < 8; ++i, ++nStart)
        nReBamen[nOneCircle[nStart % 8]] = (menIndex++) % 8;
}
// 计算天盘
void GenerateTianPan()
{
    char szTem[STR_LEN_08];
    memset(nReTianPan, 0x0, sizeof(nReTianPan));
    int diPanIndex = -1;
    int k = 0;
    for (int i = 0; i < 9; ++i)
    {
        k = i;
        if (k == 4)
            k = 1;
        diPanIndex = nReDiPan[i];
        // 原 九星 在旋转 九星中的位置
        memset(szTem, 0x0, sizeof(szTem));
        strcpy(szTem, &pGlobalJiuxing->pHan->szData[STRINDEX(k)]);
        int zindex = dataFindHanIndex(pGlobalJiuxingR, szTem);
        int rindex = GetNumIndex(nReJiuXing, 9, zindex);
        if (nReTianPan[rindex] != 0)
            nReTianPan[rindex] = diPanIndex + (nReTianPan[rindex] * 10);
        else
            nReTianPan[rindex] = diPanIndex;
    }
}
    // 计算旬空、马星
void  GenerateOther()
{
    int hindex = dataFindHanIndex(pGlobalSixtyJiaZi, pGlobalResult->pGanzi->szLunarHourGZ);
    int xtou = (hindex / 10) * 10;
    
    char szTemOne[STR_LEN_08];
    char szTemTwo[STR_LEN_08];
    char szReOne[STR_LEN_08];
    char szReTwo[STR_LEN_08];
    memset(szTemOne, 0x0, sizeof(szTemOne));
    memset(szTemTwo, 0x0, sizeof(szTemTwo));
    memset(szReOne, 0x0, sizeof(szReOne));
    memset(szReTwo, 0x0, sizeof(szReTwo));

    strcpy(szTemOne, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX((xtou + 10) % 60)]);
    strcpy(szTemTwo, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX((xtou + 11) % 60)]);
    dataGetSubStr(szTemOne, szReOne, 1, 1);
    dataGetSubStr(szTemTwo, szReTwo, 1, 1);

    int aindex = dataFindHanIndex(pGlobalDiZhi, szReOne);
    int bindex = dataFindHanIndex(pGlobalDiZhi, szReTwo);

    nReOther[pGlobalDiZhiGong->pIndex[aindex]] = 1;
    nReOther[pGlobalDiZhiGong->pIndex[bindex]] = 1;

    memset(szTemOne, 0x0, sizeof(szTemOne));
    dataGetSubStr(pGlobalResult->pGanzi->szLunarHourGZ, szTemOne, 1, 1);
    int dindex = dataFindHanIndex(pGlobalDiZhi, szTemOne);
    int xcindex = pGlobalDiZhiSanhe->pIndex[dindex];
    int rindex = pGlobalDiZhiChong->pIndex[xcindex];
    int position = pGlobalDiZhiGong->pIndex[rindex];

    nReOther[position] = 2 + (nReOther[position] * 10);
}
/*
*   pSolar:             公里日期时间
*   bIsAutoTIme:        是否自动获取时间(1--是，其他不是)
*   qimenJuShu:         奇门局数(阴9局到阳9局，-9 ~ 9, 0 表示自动计算局数，其他失败)
*   return:             返回 0 成功，其他失败。
*/
int  qimenRun(calSolar* pSolar, int bIsAutoTime, int qimenJuShu)
{
    QimenInit();
    
    // 处理日期相关
    if (bIsAutoTime == 1)
        GetNowTime(pSolar);

    if (bIsAutoTime == 2)
    {
        GetNowTime(pSolar);
        calSolar sTem;
        DupSolar(pSolar, &sTem);
        GetDateByDiffSecond(&sTem, pSolar, 8*60*60);
    }

    if (qimenJuShu < -9 || qimenJuShu > 9)
        return -1;

    // 检查日期是否合法
    if (IsSolarLegal(pSolar) != 0)
    {
        InlegalTime();
        return -2;
    }

    // 检查日期是否在范围内
    if (IsSolarAreaLegal(pSolar) != 0)
        return -3;

    InitJieQiGan(&pGlobalJieQiGan);
    // 生成日期节气干支表(该步骤一定是使用 calendar 的所有函数的最优先的函数)
    GenerateJieQiGan(pSolar);
    //PrintJieQiGan(pGlobalJieQiGan);
    // 计算当日的全部信息(注: 要在生成日期节气干支表完成后计算，因为 calendar 仅保存最后一次计算结果)
    calendarRun(pSolar);

    if (SearchJieQiGan(pGlobalJieQiGan, pSolar, szGlobalCurJieqi, szGlobalCurGanzi) != 0)
        return -1;

    if (qimenJuShu == 0)
    {
    // 确定阴阳遁和三元
        GenerateYuan(pSolar);
        // (1) 查看当前所处的节气
        int cindex = dataFindHanIndex(pGlobalJieQiTable, szGlobalCurJieqi);
        // (2) 根据 阴阳 和节气和三元选取局数存储值
        int ts = pGlobalJuAndQi->pIndex[cindex];
        // (3) 根据三元取出对应的起始宫位数 5671
        int sp = -1;
        if (strcmp(szGlobalYuan, "上元") == 0)
            sp = ts / 1000;
        if (strcmp(szGlobalYuan, "中元") == 0)
            sp = (ts / 100) % 10;
        if (strcmp(szGlobalYuan, "下元") == 0)
            sp = (ts / 10) % 10;
        // (4) 取出局数
        if ((ts % 10) == 1)
            nGlobalJushu = sp;
        else
            nGlobalJushu = -sp;
    }
    else
    {
        nGlobalJushu = qimenJuShu;
        nGlobalAutoJushu = 0;
    }
    
    // 计算地盘
    GenerateDiPan();
    // 查找值符、值使
    FindZhiShiFu();
    // 计算九星八门
    GenerateXingMen();
    // 计算八神
    GenerateBaShen();
    // 计算天盘
    GenerateTianPan();
    // 计算旬空、马星
    GenerateOther();
    return 0;
}

// 确定阴阳遁和三元
int GenerateYuan(const calSolar* pSolar)
{
    int cindex = dataFindHanIndex(pGlobalSixtyJiaZi, szGlobalCurGanzi);
    int futou = (cindex / 15) * 15;
    int select = (cindex - futou) / 5;
    
    switch (select)
    {
    case 0:
        strcpy(szGlobalYuan, "上元");
        break;
    case 1:
        strcpy(szGlobalYuan, "中元");
        break;
    case 2:
        strcpy(szGlobalYuan, "下元");
        break;
    default:
        break;
    }
    return 0;
}

// 生成日期和节气日干支表
void GenerateJieQiGan(const calSolar* pSolar)
{
    // 1. 获取上一年的大雪节日的日干支
    calSolar temp;
    calResult* tresult = NULL;

    temp.nSolarYear = pSolar->nSolarYear -1;
    temp.nSolarMonth = 12;
    temp.nSolarDay = 15;
    temp.nSolarHour = 12;
    temp.nSolarMin = 0;
    temp.nSolarSecond = 0;

    char daxueGanzhi[STR_LEN_08];
    memset(daxueGanzhi, 0x0, sizeof(daxueGanzhi));

    // 1.1 临时存储大雪当日信息
    calSolar daxue;

    // 找到大雪的日期
    calendarRun(&temp);
    calendarGetResult(&tresult);

    daxue.nSolarYear = tresult->pMonJieQi->nYear;
    daxue.nSolarMonth = tresult->pMonJieQi->nMonth;
    daxue.nSolarDay = tresult->pMonJieQi->nFirstDay;
    daxue.nSolarHour = 12;
    daxue.nSolarMin = 0;
    daxue.nSolarSecond = 0;

    // 拿到大学节气当日的信息
    calendarRun(&daxue);
    calendarGetResult(&tresult);

    // 2. 查找大学节气的符头
    int dxindex = dataFindHanIndex(pGlobalSixtyJiaZi, tresult->pGanzi->szLunarDayGZ);
    int futou = (dxindex / 15) * 15;

    if ((dxindex - futou) >= 9)
        bGlobalZhirun = 1;
    else
        bGlobalZhirun = 0;

    int aindex = dxindex;
    char ntemp[STR_LEN_08];

    // 复制一份当月大雪节气的日期
    calSolar cntemp;  // cntemp 用于临时保存下一天的日期信息
    DupSolar(tresult->pSolar, &cntemp);

    // 保存当前的三元
    for (int i = 0; i < futou + 15 - dxindex; ++i, ++aindex)
    {
        memset(ntemp, 0x0, sizeof(ntemp));
        strcpy(ntemp, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex)]);
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, "大雪", ntemp);
        GetAfterDayFromDateOwn(&cntemp);
    }
    // 
    char startJieName[STR_LEN_08];
    memset(startJieName, 0x0, sizeof(startJieName));
    if (bGlobalZhirun)
        strcpy(startJieName, "大雪");
    else
        strcpy(startJieName, "冬至");

    int nCircle = 0;
    char mzGanzhi[STR_LEN_08];
    char nextGanzhi[STR_LEN_08];
    memset(mzGanzhi, 0x0, sizeof(mzGanzhi));

    for (int i = 0; i < 300; ++i, ++nCircle, ++aindex)
    {
        memset(nextGanzhi, 0x0, sizeof(nextGanzhi));
        strcpy(nextGanzhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex % 60)]);
        if (nCircle == 15)
        {
            int currentJieIndex = dataFindHanIndex(pGlobalJieQiTable, startJieName);
            memset(startJieName, 0x0, sizeof(startJieName));
            strcpy(startJieName, &pGlobalJieQiTable->pHan->szData[STRINDEX((currentJieIndex + 1) % 24)]);
            if (strcmp(startJieName, "芒种") == 0)
            {
                InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
                strcpy(mzGanzhi, nextGanzhi);
                break;
            }
            nCircle = 0;
        }
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
        GetAfterDayFromDateOwn(&cntemp);
    }
    // 此时 cntemp 保存的是芒种上元的第一天，在这里看是否需要置润
    // 先找一下芒种的具体日期
    calSolar mzDate;

    // memset(temp, 0x0, sizeof (temp));
    temp.nSolarYear = pSolar->nSolarYear;
    temp.nSolarMonth = 6;
    temp.nSolarDay = 15;
    temp.nSolarHour = 12;
    temp.nSolarMin = 0;
    temp.nSolarSecond = 0;
    // 找到芒种的日期
    // 2022-05-06 BUG 日期会被在这里置成 1900
    calendarRun(&temp);
    calendarGetResult(&tresult);

    mzDate.nSolarYear = tresult->pMonJieQi->nYear;
    mzDate.nSolarMonth = tresult->pMonJieQi->nMonth;
    mzDate.nSolarDay = tresult->pMonJieQi->nFirstDay;
    mzDate.nSolarHour = 12;
    mzDate.nSolarMin = 0;
    mzDate.nSolarSecond = 0;

    // 拿到芒种节气当日的信息
    calendarRun(&mzDate);
    calendarGetResult(&tresult);
    DupSolar(tresult->pSolar, &mzDate);

    if (GetDayDiffTwo(&mzDate, &cntemp) >= 9)
        bGlobalZhirun = 1;
    else
        bGlobalZhirun = 0;
    
    // 把当前的三元保存进去
    for (int i = 0; i < 14; ++i)
    {
        aindex++;
        GetAfterDayFromDateOwn(&cntemp);
        memset(nextGanzhi, 0x0, sizeof(nextGanzhi));
        // strcpy(nextGanzhi, &pGlobalJieQiTable->pHan[STRINDEX(aindex) % 24]);
        strcpy(nextGanzhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex % 60)]);
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, "芒种", nextGanzhi);
    }
    if (bGlobalZhirun)
        strcpy(startJieName, "芒种");
    else
        strcpy(startJieName, "夏至");
        
    nCircle = 0;
    char dxStartGazhi[STR_LEN_08];
    memset(dxStartGazhi, 0x0, sizeof(dxStartGazhi));
    
    for (int i = 0; i < 300; ++i, ++nCircle)
    {
        aindex++;
        GetAfterDayFromDateOwn(&cntemp);
        memset(nextGanzhi, 0x0, sizeof(nextGanzhi));
        int cindex = dataFindHanIndex(pGlobalSixtyJiaZi, startJieName);
        strcpy(nextGanzhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex % 60)]);
        if (nCircle == 15)
        {
            int curJieQiIndex = dataFindHanIndex(pGlobalJieQiTable, startJieName);
            strcpy(startJieName, &pGlobalJieQiTable->pHan->szData[STRINDEX((curJieQiIndex + 1) % 24)]);
            if (strcmp(startJieName, "大雪") == 0)
            {
                InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
                strcpy(dxStartGazhi, nextGanzhi);
                break;
            }
            nCircle = 0;
        }
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
    }
    // 此时 cntem 是大雪上元的第一天，在这里看要不要置润
    temp.nSolarYear = pSolar->nSolarYear;
    temp.nSolarMonth = 12;
    temp.nSolarDay = 15;
    temp.nSolarHour = 12;
    temp.nSolarMin = 0;
    temp.nSolarSecond = 0;

    calendarRun(&temp);    
    calendarGetResult(&tresult);

    calSolar ndxdate;    
    
    ndxdate.nSolarYear = tresult->pMonJieQi->nYear;
    ndxdate.nSolarMonth = tresult->pMonJieQi->nMonth;
    ndxdate.nSolarDay = tresult->pMonJieQi->nFirstDay;
    ndxdate.nSolarHour = 12;
    ndxdate.nSolarMin = 0;
    ndxdate.nSolarSecond = 0;

    if (GetDayDiffTwo(&ndxdate, &cntemp) >= 9)
        bGlobalZhirun = 1;
    else
        bGlobalZhirun = 0;

    // 先把当前的大雪节气排进去
    for (int i = 0; i < 14; ++i)
    {
        aindex++;
        GetAfterDayFromDateOwn(&cntemp);
        strcpy(nextGanzhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex % 60)]);
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, "大雪", nextGanzhi);
    }
    if (bGlobalZhirun)
        strcpy(startJieName, "大雪");
    else
        strcpy(startJieName, "冬至");
    
    nCircle = 0;
    memset(dxStartGazhi, 0x0, sizeof(dxStartGazhi));
    for (int i = 0; i < 60; ++i, ++nCircle)
    {
        aindex++;
        GetAfterDayFromDateOwn(&cntemp);
        strcpy(nextGanzhi, &pGlobalSixtyJiaZi->pHan->szData[STRINDEX(aindex % 60)]);
        if (nCircle == 15)
        {
            int cindex = dataFindHanIndex(pGlobalJieQiTable, startJieName);
            strcpy(startJieName, &pGlobalJieQiTable->pHan->szData[STRINDEX((cindex + 1) % 24)]);
            if (strcmp(startJieName, "立春") == 0)
            {
                InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
                strcpy(dxStartGazhi, nextGanzhi);
                break;
            }
            nCircle = 0;
        }
        InsertJieQiGan(&pGlobalJieQiGan, &cntemp, startJieName, nextGanzhi);
    }
}





