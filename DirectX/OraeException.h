#pragma once

#include <exception>
#include <string>

#ifdef UNICODE
using char_t   = wchar_t;
using string_t = std::wstring;
#elif
using char_t   = char;
using string_t = std::string;
#endif

class OraeException : public std::exception
{
public:
    OraeException(int line, const char_t* file) noexcept;

    virtual const char_t* What() const noexcept;
    virtual const char_t* GetType() const noexcept;
    int                   GetLine() const noexcept;
    const string_t        GetFile() const noexcept;
    string_t              GetOriginString() const noexcept;

private:
    virtual char const* what() const { return ""; }

protected:
    mutable string_t whatBuffer;

private:
    int      line;
    string_t file;
};
