#ifndef BCOSTA_CORE_TIME_H
#define BCOSTA_CORE_TIME_H

#include "windows.h"

namespace BCosta {
namespace Core {

// Convert FILETIME to ULONGLONG (casting won't work on 64-bit platforms, due to alignment of FILETIME members)
void ToUnsignedLongLong(const FILETIME &ft, ULONGLONG &uft);

// Convert ULONGLONG to FILETIME (casting won't work on 64-bit platforms, due to alignment of FILETIME members)
void ToFiletime(const ULONGLONG &uft, FILETIME &ft);

// Convert ULONGLONG to time-components
bool UnsignedLongLongToSystemTime(const ULONGLONG time, WORD &year, WORD &month, WORD &day, WORD &hour, WORD &min,
                                  WORD &sec, WORD &msec);

void TimeToDate(const ULONGLONG time, char *date);

}
}
#endif // BCOSTA_CORE_TIME_H
