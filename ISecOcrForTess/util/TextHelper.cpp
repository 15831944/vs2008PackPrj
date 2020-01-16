#include "TextHelper.h"
#include <codecvt>
#include <regex>
#include <iostream>

XAB_NAMESPACE_START


std::string CTextHelper::GBK2UTF(const std::string& src) noexcept
{
#ifdef _MSC_VER
    const std::string localeName = "CHS"; // ".936";
#else
    const string localeName = "zh_CN.GBK";
#endif
    try
    {
        std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv1(new std::codecvt_byname<wchar_t, char, mbstate_t>(localeName));
        const std::wstring ws = cv1.from_bytes(src);

        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.to_bytes(ws);
    }
    catch (std::exception& e)
    {
        std::cerr<<"GBK2UTF: " << e.what();
        return src;
    }
}

std::string CTextHelper::UTF2GBK(const std::string& src) noexcept
{
#ifdef _MSC_VER
    const std::string localeName = "CHS"; //".936";
#else
    const std::string localeName = "zh_CN.GBK";
#endif
    try
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        const std::wstring ws = myconv.from_bytes(src);

        std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv(new std::codecvt_byname<wchar_t, char, mbstate_t>(localeName));
        return cv.to_bytes(ws);
    }
    catch (std::exception& e)
    {
        std::cerr << "UTF2GBK: " << e.what();
        return src;
    }
}


XAB_NAMESPACE_END