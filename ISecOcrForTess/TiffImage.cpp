#include "TiffImage.h"
#include <opencv2/opencv.hpp>
#include <tiff/tiffio.h>

using namespace std;
using namespace cv;

//���洦��
static int grfmt_tiff_err_handler_init = 0;
static void GrFmtSilentTIFFErrorHandler(const char*, const char*, va_list) {}


CTiffImage::CTiffImage() 
    : CMultiPageImageBase(), m_pTiff(nullptr, [](TIFF* ptr) {if (ptr) TIFFClose(ptr); })
{
    //���洦��:��ֹ����unknown field with tag  33500 encountered����
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
    //��ͼ��
    //��Tiff�ļ����õ�ָ�룬�Ժ����еĲ�����ͨ��ָ�����
    m_pTiff.reset(TIFFOpen(imgURI.c_str(), "r"));
    if (!m_pTiff)
        return false;
    m_pageCount = TIFFNumberOfDirectories(m_pTiff.get()); // ��ȡҳ��
    return true;
}
bool CTiffImage::loadFromMemory(const uchar* data, size_t len) noexcept
{
    return false;
}

cv::Mat CTiffImage::getMat(int pageIndex) noexcept
{
    // ����ָ����ҳ��, ���pageIndex ����ҳ��, ��Ĭ�ϴ򿪵�һҳ
    TIFFSetDirectory(m_pTiff.get(), pageIndex); 

    //��ȡͼ�����
    int width, height;
    TIFFGetField(m_pTiff.get(), TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(m_pTiff.get(), TIFFTAG_IMAGELENGTH, &height);

    //��ȡͼ��
    //ע�⣺TIFFReadRGBAImage��ȡ��ͨ��˳��Ϊ��ABGR
    uint32* image;
    int pixelCount = width*height;
    image = (uint32*)malloc(pixelCount * sizeof(uint32));
    TIFFReadRGBAImage(m_pTiff.get(), width, height, image, 1);

    //��ȡRͨ��
    //����tiff��ʽ��ͼ��������bmpͼ�洢��ʽһ�£��Ǵ��µ��ϣ����Զ���ʱ����Ҫ�������϶�
    uchar* RImage = new uchar[pixelCount];    //Ϊ������ݷ����ڴ�ռ�
    uchar* GImage = new uchar[pixelCount];    //Ϊ������ݷ����ڴ�ռ�
    uchar* BImage = new uchar[pixelCount];    //Ϊ������ݷ����ڴ�ռ�
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
            //��ȡRͨ��
            *colPointerToR = (uchar)TIFFGetR(*colPointerToSrc);
            *colPointerToG = (uchar)TIFFGetG(*colPointerToSrc);//��ȡGͨ��
            *colPointerToB = (uchar)TIFFGetB(*colPointerToSrc);//��ȡBͨ��
            

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
    // ˳���������, opencv �е�˳���� BGR
    channels.push_back(bMat);
    channels.push_back(gMat);
    channels.push_back(rMat);

    Mat resultImage;
    cv::merge(channels, resultImage);
    //imshow("test tiff", resultImage);

    //�ͷſռ�
    _TIFFfree(image);
    _TIFFfree(RImage);
    _TIFFfree(GImage);
    _TIFFfree(BImage);

    return resultImage;
}