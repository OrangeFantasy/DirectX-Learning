#pragma once

#include <string>
#include <sstream>

#ifdef UNICODE
using FChar   = wchar_t;
using FString = std::wstring;

using FStringStream  = std::wstringstream;
using FOStringStream = std::wostringstream;
using FIStringStream = std::wistringstream;
#elif
using FChar   = char;
using FString = std::string;

using FStringStream  = std::stringstream;
using FOStringStream = std::ostringstream;
using FIStringStream = std::istringstream;
#endif // !UNICODE

#ifdef UNICODE
#define TEXT_PASTE(Quote) L##Quote
#elif
#define TEXT_PASTE(Quote) Quote
#endif // !UNICODE

#ifndef TEXT
#define TEXT(Quote) TEXT_PASTE(Quote)
#endif // !TEXT
