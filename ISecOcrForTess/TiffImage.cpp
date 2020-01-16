#include "TiffImage.h"
#include <opencv2/opencv.hpp>
#include <tiff/tiffio.h>

using namespace std;
using namespace cv;

//警告处理
static int grfmt_tiff_err_handler_init = 0;
static void GrFmtSilentTIFFErrorHandler(const char*, const char*, va_list) {}


CTiffImage::CTiffImage() 
    : CMultiPageImageBase(), m_pTiff(nullptr, [](TIFF* ptr) {if (ptr) TIFFClose(ptr); })
{
    //警告处理:防止出现unknown field with tag  33500 encountered警告
    if (!grfmt_tiff_err_handler_init)
    {
        grfmt_tiff_err_handler_init = 1;

        TIFFSetErrorHandler(GrFmtSilentTIFFErrorHandler);
        TIFFSetWarningHandler(GrFmtSilentTIFFErrorHandler);
    }
}


CTiffImage::~CTiffImage()
{
}

bool CTiffImage::loadFromFile(const std::string imgURI) noexcept
{
    //打开图像
    //打开Tiff文件，得到指针，以后所有的操作都通过指针进行
    m_pTiff.reset(TIFFOpen(imgURI.c_str(), "r"));
    if (!m_pTiff)
        return false;
    m_pageCount = TIFFNumberOfDirectories(m_pTiff.get()); // 读取页数
    return true;
}
bool CTiffImage::loadFromMemory(const uchar* data, size_t len) noexcept
{
    return false;
}

cv::Mat CTiffImage::getMat(int pageIndex) noexcept
{
    // 跳到指定的页数, 如果pageIndex 超过页数, 则默认打开第一页
    TIFFSetDirectory(m_pTiff.get(), pageIndex); 

    //获取图像参数
    int width, height;
    TIFFGetField(m_pTiff.get(), TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(m_pTiff.get(), TIFFTAG_IMAGELENGTH, &height);

    //读取图像
    //注意：TIFFReadRGBAImage读取的通道顺序为：ABGR
    uint32* image;
    int pixelCount = width*height;
    image = (uint32*)malloc(pixelCount * sizeof(uint32));
    TIFFReadRGBAImage(m_pTiff.get(), width, height, image, 1);

    //读取R通道
    //由于tiff格式的图像数据与bmp图存储方式一致，是从下到上，所以读的时候，需要从下往上读
    uchar* RImage = new uchar[pixelCount];    //为存放数据分配内存空间
    uchar* GImage = new uchar[pixelCount];    //为存放数据分配内存空间
    uchar* BImage = new uchar[pixelCount];    //为存放数据分配内存空间
    uint32 *rowPointerToSrc = image + (height - 1)*width;
    uchar *rowPointerToR = RImage;
    uchar *rowPointerToG = GImage;
    uchar *rowPointerToB = BImage;
    for (int y = height - 1; y >= 0; --y)
    {
        uint32 *colPointerToSrc = rowPointerToSrc;
        uchar *colPointerToR = rowPointerToR;
        uchar *colPointerToG = rowPointerToG;
        uchar *colPointerToB = rowPointerToB;
        for (int x = 0; x <= width - 1; ++x)
        {
            //获取R通道
            *colPointerToR = (uchar)TIFFGetR(*colPointerToSrc);
            *colPointerToG = (uchar)TIFFGetG(*colPointerToSrc);//获取G通道
            *colPointerToB = (uchar)TIFFGetB(*colPointerToSrc);//获取B通道
            

            colPointerToR++;
            colPointerToG++;
            colPointerToB++;

            colPointerToSrc++;
        }
        rowPointerToSrc -= width;
        rowPointerToR += width;
        rowPointerToG += width;
        rowPointerToB += width;
    }

    vector<Mat> channels;
    Mat rMat(height, width, CV_8UC1, RImage, width);
    Mat gMat(height, width, CV_8UC1, GImage, width);
    Mat bMat(height, width, CV_8UC1, BImage, width);
    // 顺序别搞错误了, opencv 中的顺序是 BGR
    channels.push_back(bMat);
    channels.push_back(gMat);
    channels.push_back(rMat);

    Mat resultImage;
    cv::merge(channels, resultImage);
    //imshow("test tiff", resultImage);

    //释放空间
    _TIFFfree(image);
    _TIFFfree(RImage);
    _TIFFfree(GImage);
    _TIFFfree(BImage);

    return resultImage;
}