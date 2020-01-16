/// 该文件是与项目公用
///
/// @author zhoumanman888@126.com
/// @file TextHelper.h
/// @date 2017.07.17

#pragma once

#include <string>
#define XAB_NAMESPACE_START namespace XAB {
#define XAB_NAMESPACE_END };


XAB_NAMESPACE_START

struct CTextHelper
{
    /// 由gbk编码, 转换为utf8编码
    ///
    /// @author zhoumanman888@126.com
    static std::string UTF2GBK(const std::string& src) noexcept;

    /// 由utf8编码, 转换为gbk编码
    ///
    /// @author zhoumanman888@126.com
    std::string GBK2UTF(const std::string& src) noexcept;
};

XAB_NAMESPACE_END