#pragma once

#include "../ISecOcrService/OcrRegco.h"

#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/api/baseapi.h>
#include <chrono>
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

struct MyOCRWord
{
    std::string word;
    cv::Rect rect; // 包围 word 的矩形框
};
struct MyOCRChar
{
    std::string chr;
    cv::Rect rect; // 包围 word 的矩形框
};

struct CMyClock
{
public:
    void clockBeg() noexcept;
    float clockEnd() noexcept; // 返回秒
private:
    std::chrono::time_point<std::chrono::system_clock> m_tmpTime;
};


class COcrByTesseract : public COcrRegco
{
public:
    COcrByTesseract() noexcept;
    ~COcrByTesseract() noexcept;

    void EnableLog(bool val) noexcept override;
    // 重载基类函数
    void SetPrivilLevel(EN_OcrPrivilgeLevel privilgeLevel) noexcept override;
    bool Init(unsigned dwTimeOutMS) noexcept override;
    void SetKeywords(char* keyWords[], int keyWordCount) noexcept override;
    bool RegcoFile(const char* strSrcFilePath, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) noexcept override;
    bool RegcoMem(const unsigned char *pSourceFileMem, unsigned dwSourceFileSize, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) noexcept override;
    void Release() noexcept override;

    void setThresh(float thresh) noexcept;
private:
    void releaseResult(RegcoResult& result) noexcept;

    /// 传入的根据源图片, 传回识别后的文本
    ///
    /// @outStr 识别后的文本
    bool regcoMat(const cv::Mat& srcImage, std::string& outStr, bool canRotate = true) noexcept;
    void myRotate(cv::Mat& srcImage, double degree) noexcept;
    void improveImg(cv::Mat& srcImg) noexcept;
    void adjustImageOrient(cv::Mat& srcImage);
    double CalcDegree(const cv::Mat &srcImage);
    double DegreeTrans(double theta);
    void COcrByTesseract::gammaCorrection(const cv::Mat &img, cv::Mat& dst);
    void removeRt(std::string& srcStr);
    bool checkAndHandle(cv::Mat& srcImage, std::string& hitKeyword, int pageNo = 1); // 中标检测并处理
    cv::Rect mergeRect(const cv::Rect& box1, const cv::Rect& box2);
    bool isSameLine(const cv::Rect& box1, const cv::Rect& box2);
    void makePages(std::vector<std::string>& pageList, RegcoResult& result) noexcept;
private:
    tesseract::TessBaseAPI m_apiEng; // 英语 OCR 引擎
    tesseract::TessBaseAPI m_apiChi; // 汉语 OCR 引擎
    cv::Mat m_srcImg;
    cv::Mat m_grayImg;
    cv::Mat m_lookUpTable;
    double m_thresh; // 以后可以改成从配置里面读取, 如果用户设置为-1, 则由程序自己决定; 默认-1
    std::vector<std::string> m_keywords;
    const std::locale m_loc;
    std::string m_strTmpPath; // 临时目录
    std::vector<MyOCRChar> m_charList;
    std::vector<MyOCRWord> m_wordList;
    EN_OcrPrivilgeLevel m_privilgeLevel;
};



// 导出变量
extern "C" BOOST_SYMBOL_EXPORT COcrByTesseract ocr_engine;