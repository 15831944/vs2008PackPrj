#pragma once

#include <string>

// �����Ͳ��ô� <opencv2/opencv.hpp> ��ͷ�ļ���
namespace cv {
    class Mat;
}
typedef unsigned char uchar;

///  ���ɷ��ʶ�ҳͼƬ(��tiff, pdf, gif) ��ÿһҳ
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

