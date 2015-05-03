#include <stdio.h>
#include "time.h"

using namespace BCosta::Core;

void BCosta::Core::ToUnsignedLongLong(const FILETIME &ft, ULONGLONG &uft)
{
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    uft = uli.QuadPart;
}

void BCosta::Core::ToFiletime(const ULONGLONG &uft, FILETIME &ft)
{
    ULARGE_INTEGER uli = {0};
    uli.QuadPart = uft;
    ft.dwLowDateTime = uli.LowPart;
    ft.dwHighDateTime = uli.HighPart;
}

bool BCosta::Core::UnsignedLongLongToSystemTime(const ULONGLONG time, WORD &year, WORD &month, WORD &day, WORD &hour,
                                                WORD &min, WORD &sec, WORD &msec)
{
    SYSTEMTIME sys_time;
    FILETIME ft;
    ToFiletime(time, ft);

    // The wDayOfWeek member of the SYSTEMTIME structure is ignored
    if (FileTimeToSystemTime(&ft, &sys_time) == 0) {
        return false;
    }

    year = sys_time.wYear;
    month = sys_time.wMonth;
    day = sys_time.wDay;
    hour = sys_time.wHour;
    min = sys_time.wMinute;
    sec = sys_time.wSecond;
    msec = sys_time.wMilliseconds;

    return true;
}

void BCosta::Core::TimeToDate(const ULONGLONG time, char *date)
{
    WORD yr, mth, day, hr, min, sec, msec;
    UnsignedLongLongToSystemTime(time, yr, mth, day, hr, min, sec, msec);
    sprintf_s(date, 24, "%02u-%02u-%04u %02u:%02u:%02u.%03u", day, mth, yr, hr, min, sec, msec);
}
