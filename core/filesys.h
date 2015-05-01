#ifndef BCOSTA_CORE_FILE_SYSTEM_H
#define BCOSTA_CORE_FILE_SYSTEM_H

#include <stdio.h>

namespace BCosta {
namespace Core {

class String;

class FileSystem
{
private:

    FILE *f;

public:

    FileSystem() : f(0)
    { }

    ~FileSystem();

    const bool Open(const String &path);

    const bool Word(String &w) const;

    const bool Scan(String &line) const;

    const String Read();

    const int Size() const;

    void Close();

    inline FILE *GetFile() const
    { return f; }
};

}
}
#endif // BCOSTA_CORE_FILE_SYSTEM_H