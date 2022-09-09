#ifndef CQM_CALENDAR_H_
#define CQM_CALENDAR_H_

#include <define.h>

typedef long long int INT64;
// 字典结构体
typedef struct calSolar
{
    int nSolarYear;       // 公历年
    int nSolarMonth;      // 公历月
    int nSolarDay;        // 公历日
    int nSolarHour;       // 公历时
    int nSolarMin;        // 公历分钟
    int nSolarSecond;     // 公历秒钟
}calSolar;
// 农历
typedef struct calLunar
{
    int nLunarYear;       // 农历年
    int nLunarMonth;      // 农历月
    int nLunarDay;        // 农历日
    int bBigMonth;        // 月大还是月小
    int bLeapMonth;       // 是不是闰月
}calLunar;
// 农历四干支
typedef struct calGanZhi
{
    char szLunarYearGZ[STR_LEN_08];    // 年干支(两个字)
    char szLunarMonthGZ[STR_LEN_08];   // 月干支(两个字)
    char szLunarDayGZ[STR_LEN_08];     // 日干支(两个字)
    char szLunarHourGZ[STR_LEN_08];    // 时干支(两个字)
}calGanZhi;
// 当月节气
typedef struct calMonthJieQi
{
    int nYear;
    int nMonth;
    int nFirstDay;
    int nFirstHour;
    int nFirstMin;
    int nFirstSec;
    char szFirstJieQi[STR_LEN_08];
    int nSecondDay;
    int nSecHour;
    int nSecMin;
    int nSecSec;
    char szSecondJieQi[STR_LEN_08];
}calMonthJieQi;
// 计算结果结构
typedef struct calResult
{
    calSolar*       pSolar;
    calLunar*       pLunar;
    calGanZhi*      pGanzi;
    calMonthJieQi*  pMonJieQi;
}calResult;

int calendarRun(const calSolar* pSolar);                                                    // 传入公历进行结算
void calendarGetResult(calResult** pResult);                                                // 拿到计算结果
void calendarFree();                                                                     // 计算完成后的释放所有资源

// 处理时间相关的函数
void GetNowTime(calSolar* pSolar);                                                          // 获取系统时间
int IsSolarLegal(const calSolar* pSolar);                                                   // 判断公历日期是否合法(0 合法)
int IsSolarAreaLegal(const calSolar* pSolar);                                               // 判断公历日期范围是否合法(0 合法)
void GetPreDayFromDate(const calSolar* pSolar, calSolar* pResult);                          // 返回一个公历日期的前一天的日期
void GetAfterDayFromDate(const calSolar* pSolar, calSolar* pResult);                        // 返回一个公历日期的后一天的日期
void GetAfterDayFromDateOwn(calSolar* pSolar);                                              // 返回一个公历日期的后一天的日期(改变自己)
void DupSolar(const calSolar* pSourceSolar, calSolar* pDesSolar);                           // 复制 solar 结构体内容
int GetDayDiffTwo(const calSolar* pSolarOne, const calSolar* pSolarTwo);                    // 返回两个日期之间的天数差
int GetRemainder(int nNum, int nBasic);                                                     // 对数进行取余数(支持负数)
INT64 GetSecondDiviTwoDate(const calSolar* pSolarOne, const calSolar* pSolarTwo);           // 返回两个日期之间相差的秒数
// 根据基准日期和相差的秒钟计算另一个日期, 正向前，负向后 (Note: 请确认日期 solar 的正确性)
void GetDateByDiffSecond(const calSolar* pSolar, calSolar* pResult, INT64 nSec);

#endif

