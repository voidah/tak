#ifndef UNICODE_H__
#define UNICODE_H__

#include <string>

class Unicode
{
    public:
        static std::string FromUtf8ToAnsi(const std::string& utf8String, const std::string& ansiCodePage = "ISO−8859−1");
        static std::string FromAnsiToUtf8(const std::string& ansiString, const std::string& ansiCodePage = "ISO−8859−1");
        static std::wstring FromAnsiToWstring(const std::string& ansiString, const std::string& ansiCodePage = "ISO−8859−1");
        static std::string FromWstringToUtf8(const std::wstring& wString);
};


#endif // UNICODE_H__
