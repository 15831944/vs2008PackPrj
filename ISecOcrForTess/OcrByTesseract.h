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
    cv::Rect rect; // ��Χ word �ľ��ο�
};
struct MyOCRChar
{
    std::string chr;
    cv::Rect rect; // ��Χ word �ľ��ο�
};

struct CMyClock
{
public:
    void clockBeg() noexcept;
    float clockEnd() noexcept; // ������
private:
    std::chrono::time_point<std::chrono::system_clock> m_tmpTime;
};


class COcrByTesseract : public COcrRegco
{
public:
    COcrByTesseract() noexcept;
    ~COcrByTesseract() noexcept;

    void EnableLog(bool val) noexcept override;
    // ���ػ��ຯ��
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

    /// ����ĸ���ԴͼƬ, ����ʶ�����ı�
    ///
    /// @outStr ʶ�����ı�
    bool regcoMat(const cv::Mat& srcImage, std::string& outStr, bool canRotate = true) noexcept;
    void myRotate(cv::Mat& srcImage, double degree) noexcept;
    void improveImg(cv::Mat& srcImg) noexcept;
    void adjustImageOrient(cv::Mat& srcImage);
    double CalcDegree(const cv::Mat &srcImage);
    double DegreeTrans(double theta);
    void COcrByTesseract::gammaCorrection(const cv::Mat &img, cv::Mat& dst);
    void removeRt(std::string& srcStr);
    bool checkAndHandle(cv::Mat& srcImage, std::string& hitKeyword, int pageNo = 1); // �б��Ⲣ����
    cv::Rect mergeRect(const cv::Rect& box1, const cv::Rect& box2);
    bool isSameLine(const cv::Rect& box1, const cv::Rect& box2);
    void makePages(std::vector<std::string>& pageList, RegcoResult& result) noexcept;
private:
    tesseract::TessBaseAPI m_apiEng; // Ӣ�� OCR ����
    tesseract::TessBaseAPI m_apiChi; // ���� OCR ����
    cv::Mat m_srcImg;
    cv::Mat m_grayImg;
    cv::Mat m_lookUpTable;
    double m_thresh; // �Ժ���Ըĳɴ����������ȡ, ����û�����Ϊ-1, ���ɳ����Լ�����; Ĭ��-1
    std::vector<std::string> m_keywords;
    const std::locale m_loc;
    std::string m_strTmpPath; // ��ʱĿ¼
    std::vector<MyOCRChar> m_charList;
    std::vector<MyOCRWord> m_wordList;
    EN_OcrPrivilgeLevel m_privilgeLevel;
};



// ��������
extern "C" BOOST_SYMBOL_EXPORT COcrByTesseract ocr_engine;