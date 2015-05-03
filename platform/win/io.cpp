#include <Windows.h>
#include "../io.h"

using namespace BCosta::Platform;

void IO::Print(const char *out)
{ OutputDebugStringA(out); }