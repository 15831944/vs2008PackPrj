#pragma once
#include "MultiPageImageBase.h"
#include <memory>

struct tiff;
typedef struct tiff TIFF;


class CTiffImage :
    public CMultiPageImageBase
{
public:
    CTiffImage();
    ~CTiffImage() override;

    bool loadFromFile(const std::string imgURI) noexcept override;
    bool loadFromMemory(const uchar* data, size_t len) noexcept override;

    cv::Mat getMat(int pageIndex = 0) noexcept override;
private:
    std::unique_ptr<TIFF, void(*)(TIFF*)> m_pTiff;
};

