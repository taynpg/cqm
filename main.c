#include <stdio.h>
#include <calendar.h>
#include <qimenzpzr.h>
#include <comline.h>
#include <qimenyp.h>
/*
*   奇门遁甲排盘
*   CQM (cqm)       纯C语言(C99标准)实现。 
*                   version:     1.0.1
*                   designed by: TAYNPG
*
*          https://gitee.com/taynpg/cqm  
*/

int main(int argc, char** argv)
{
    int nJushu = 0;
    int bIsAutoTime = 0;
    extern int nStyle;
    calSolar solar;

    int re = ParseCommand(argc, argv, &solar, &nJushu, &bIsAutoTime, &nStyle);
    if (re != 0)
        return -1;
    
    switch (nStyle)
    {
    case 0:
    {
        int qimenRet = qimenZpZrRun(&solar, bIsAutoTime, nJushu);
        if (qimenRet == 0)
            PrintResult();
        break;
    }
    case 1:
    {
        int qimenRet = qimenYpRun(&solar, bIsAutoTime, nJushu);
        if (qimenRet == 0)
            PrintResult();
        break;
    }
    default:
        break;
    }

    // 释放命令行解析、奇门内存、日历内存。
    comlineFree();
    qimenFree();
    calendarFree();

    return 0;
}

