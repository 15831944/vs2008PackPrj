#pragma once

#include <string>

// 这样就不用带 <opencv2/opencv.hpp> 的头文件啦
namespace cv {
    class Mat;
}
typedef unsigned char uchar;

///  轻松访问多页图片(如tiff, pdf, gif) 的每一页
///
/// @author zhoumanman888@126.com
/// @date 2017.7.27
class CMultiPageImageBase
{
public:
    CMultiPageImageBase() : m_pageCount(0){};
    virtual ~CMultiPageImageBase() {};

    virtual bool loadFromFile(const std::string imgURI) noexcept = 0;
    virtual bool loadFromMemory(const uchar* data, size_t len) noexcept = 0;
    int getPageCount() noexcept
    {
        return m_pageCount;
    }
    virtual cv::Mat getMat(int pageIndex = 0) noexcept = 0;
protected:
    int m_pageCount;
};

