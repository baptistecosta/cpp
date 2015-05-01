#include "filesys.h"
#include "string.h"

using namespace BCosta::Core;

FileSystem::~FileSystem()
{ Close(); }

const bool FileSystem::Open(const String &path)
{
    if (path.IsEmpty()) {
        return false;
    }
    Close();
    f = fopen(path.cStr(), "r");
    return f != nullptr;
}

const bool FileSystem::Word(String &w) const
{
    char buff[128] = {0};
    if (fgets(buff, 128, f) != NULL) {
        w = buff;
        return true;
    }
    return false;
}

const bool FileSystem::Scan(String &line) const
{
    char buffer[128] = {0};
    if (fscanf(f, "%s", buffer) == EOF) {
        return false;
    }
    line = buffer;
    return true;
}

const String FileSystem::Read()
{
    String s(Size());
    char b[1024] = {0};
    while (fgets(b, 1024, f) != NULL) {
        s += b;
    }
    return s;
}

const int FileSystem::Size() const
{
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

void FileSystem::Close()
{
    if (f) {
        fclose(f);
    }
}