#ifndef BCOSTA_CORE_LOG_H
#define BCOSTA_CORE_LOG_H

#include <stdio.h>
#include "core/string.h"
#include "platform/debug.h"
#include "platform/io.h"

namespace BCosta {
namespace Core {

class String;

struct Log
{
    static const bool __log__;

    static void i(const String &msg);

    static void i(const char *format, ...);

    static void e(const String &msg);

    static void e(const char *format, ...);

    static void w(const String &msg);

    static void w(const char *format, ...);

    static void Flat(const String &msg);

    static void Flat(const char *format, ...);

    static void Tee(FILE *f, const char *format, ...);

    static void Hex(const unsigned char *data, int len, FILE * = 0);

    static void Binary(const void *const ptr, size_t const size);

    // Horizontal line
    static void HorizontalLine();

    // New line
    static void NewLine();
};

}
}
#endif // BCOSTA_CORE_LOG_H