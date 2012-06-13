#include "unicode.h"
#include <boost/locale.hpp>

// http://www.boost.org/doc/libs/1_49_0/libs/locale/doc/html/charset_handling.html

std::string Unicode::FromUtf8ToAnsi(const std::string& utf8String, const std::string& ansiCodePage)
{
    return boost::locale::conv::from_utf(utf8String, ansiCodePage, boost::locale::conv::skip);
}

std::string Unicode::FromAnsiToUtf8(const std::string& ansiString, const std::string& ansiCodePage)
{
    return boost::locale::conv::to_utf<char>(ansiString, ansiCodePage, boost::locale::conv::skip);
}

std::wstring Unicode::FromAnsiToWstring(const std::string& ansiString, const std::string& ansiCodePage)
{
    return boost::locale::conv::to_utf<wchar_t>(ansiString, ansiCodePage, boost::locale::conv::skip);
}

std::string Unicode::FromWstringToUtf8(const std::wstring& wString)
{
    return boost::locale::conv::utf_to_utf<char>(wString, boost::locale::conv::skip);
}
