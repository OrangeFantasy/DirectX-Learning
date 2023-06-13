#include "OraeException.h"

#include <sstream>

OraeException::OraeException(int line, const char_t* file) noexcept : line(line), file(file) {}

const char_t* OraeException::What() const noexcept
{
#ifdef UNICODE
    std::wostringstream oss;
#elif
    std::ostringstream oss;
#endif
    oss << GetType() << "\n" << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char_t* OraeException::GetType() const noexcept
{
#ifdef UNICODE
    return L"Orae Exception";
#elif
    return "Orae Exception";
#endif
}

int OraeException::GetLine() const noexcept
{
    return line;
}

const string_t OraeException::GetFile() const noexcept
{
    return file;
}

string_t OraeException::GetOriginString() const noexcept
{
#ifdef UNICODE
    std::wostringstream oss;
    oss << L"[File] " << file << L"\n[Line] " + std::to_wstring(line);
#elif
    std::ostringstream oss;
    oss << "[File] " << file << "\n[Line] " + std::to_string(line);
#endif
    return oss.str();
}
