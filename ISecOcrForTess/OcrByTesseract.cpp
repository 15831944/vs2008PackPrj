#include "TextAndImgExtractor.h"
#include "OcrByTesseract.h"
#include <codecvt>
#include <cstdlib>
#include "util/log4z.h"
#include "util/TextHelper.h"
#include "TiffImage.h"
#include "GsHelper.h"
#include <regex>
#include <stdlib.h>     /* at_quick_exit, quick_exit, EXIT_SUCCESS, EXIT_FAILURE */

using namespace std;
using namespace cv;
using namespace zsummer::log4z;
using namespace PoDoFo;
// #define MY_DEBUG


COcrByTesseract ocr_engine;

COcrByTesseract::COcrByTesseract() noexcept
    : COcrRegco(), m_thresh(-1), m_lookUpTable(1, 256, CV_8U)
    , m_strTmpPath(std::getenv("temp"))
    , m_privilgeLevel(en_OcrPrivilgeNormal)
{
    PdfError::EnableDebug(false);

    const double gamma_ = 3;
    uchar* p = m_lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
}


COcrByTesseract::~COcrByTesseract() noexcept
{
}

void COcrByTesseract::SetPrivilLevel(EN_OcrPrivilgeLevel privilgeLevel) noexcept
{
    m_privilgeLevel = privilgeLevel;
}
void COcrByTesseract::EnableLog(bool val) noexcept
{
    ILog4zManager& lm = ILog4zManager::getRef();
    lm.enableLogger(LOG4Z_MAIN_LOGGER_ID, val); 
    lm.start();
}

bool COcrByTesseract::Init(unsigned dwTimeOutMS)  noexcept
{
    CGsHelper& helper = CGsHelper::getInstance();
    if (!helper.isLoadOk())
    {
        LOGD("����gsdll.dll");
        // ���ص�ǰĿ¼�� dll
        if (!helper.loadDll("gsdll32.dll"))
        {
            LOGE("���� gsdll32.dll ʧ��, ��dll ȱʧ�ᵼ��pdfתͼƬʧ��!");
            //std::exit(EXIT_FAILURE); // �˳������� static ����, ��ֹǰ�ȵ��þ��� atexit() ע��ĺ���
            //std::quick_exit(EXIT_FAILURE); // �˳��������κζ���, �˳�ǰ���� at_quick_exit ע��ĺ���
        }
    }
    if (m_bInit)
        return true;

    CMyClock clk;
    clk.clockBeg();

    m_bInit = false;
    std::string strlang = "eng";
    const std::string langPath(".\\tessdata"); // ���ص�ǰ������·��
    LOGI("��ʼ����������: " << strlang);
    if (m_apiEng.Init(langPath.c_str(), strlang.c_str(), tesseract::OEM_DEFAULT))//OEM_TESSERACT_ONLY OEM_DEFAULT OEM_CUBE_ONLY
    {
        LOGE("��ʼ��" << strlang << " OCR ����ʧ��!");
        return false;
    }
    float timeLen = clk.clockEnd();
    LOGD("��ʼ��" << strlang << " OCR �����ʱ: " << std::move(timeLen));

    clk.clockBeg();
    strlang = "chi_sim";
    LOGI("��ʼ����������: " << strlang);
    if (m_apiChi.Init(langPath.c_str(), strlang.c_str(), tesseract::OEM_DEFAULT))//OEM_TESSERACT_ONLY OEM_DEFAULT OEM_CUBE_ONLY
    {
        LOGE("��ʼ��" << strlang << " OCR ����ʧ��! ");
        return false;
    }
    timeLen = clk.clockEnd();
    LOGD("��ʼ��" << strlang << " OCR �����ʱ: " << std::move(timeLen));

    m_bInit = true;
    return m_bInit;
}

void COcrByTesseract::SetKeywords(char* keyWords[], int keyWordCount) noexcept
{
    m_keywords.clear();
    string sss = "keywords: ";
    for (int i = 0; i < keyWordCount; ++i)
    {
        sss += string(keyWords[i]) + " ";
        m_keywords.push_back(keyWords[i]);
    }
    LOGD(sss.data());
}

void COcrByTesseract::Release() noexcept
{
    if (!m_bInit)
        return;
    m_apiEng.End();
    m_apiChi.End();

    m_bInit = false;
}

bool COcrByTesseract::RegcoFile(const char* strSrcFilePath, FNC_OnFinished callback, void* pUserData, bool bQuik
    , EN_OcrSourceType enOcrSourceType) noexcept
{
    LOGI("ʶ��ʼ...");
    if (nullptr == callback)
    {
        LOGD("�����ûص�����!");
        return false;
    }

    cv::Mat resultImg;
    RegcoResult res;
    std::string hitKeyword;

    vector<string> pageList;
    bool bOk = false;
    if (tag_EN_OcrSourceType::en_OcrSourcePdf == enOcrSourceType)
    {
        // ȡ�� ҳ��--�ı�map
        CTextAndImgExtractor extractor;
        try
        {
            extractor.Init(strSrcFilePath);

            std::string strTmp;
            int pgCnt = extractor.getPagesCount();
            for (int pgIndex = 0; pgIndex < pgCnt; ++pgIndex)
            {
                LOGI("����ʶ pdf ��� " << pgIndex + 1 << " ҳ");
                std::string strCurPageContent;

                auto page = extractor.getPage(pgIndex + 1);
                strCurPageContent = page.strText;

                if (!res.isHit)
                {// ֻ����һ���б��������
                    for (const auto& keyword : m_keywords)
                    {
                        const int keywordLen = keyword.length();
                        const size_t pos = strCurPageContent.find(keyword);
                        if (string::npos == pos)
                            continue;
                        else
                        {
                            LOGI("pdf �ı��б�keyword: " << keyword << ", λ��: " << pos);
                            res.pageNo = pgIndex + 1;
                            hitKeyword = keyword;
                            res.isHit = true;
                            bOk = true;
                            break;
                        }
                    }
                    if (res.isHit)
                    {
                        if (m_privilgeLevel == en_OcrPrivilgeAdvanced)
                        {
                            // �ѵ�ǰҳת��ͼƬ
                            CGsHelper& helper = CGsHelper::getInstance();
                            const string outImgPath = m_strTmpPath + "/out.jpg";
                            if (helper.isLoadOk() && !helper.Convert2Img(strSrcFilePath, outImgPath, pgIndex + 1, pgIndex + 1))
                            {
                                LOGE("pdf תͼƬʧ��");
                                break;
                            }
                            resultImg = imread(outImgPath);
                            bOk = regcoMat(resultImg, strTmp, false);
                            if (!bOk)
                                break;
                            // �ٴν���orc
                            checkAndHandle(resultImg, hitKeyword, pgIndex + 1);
                            strCurPageContent = strTmp;

                            pageList.push_back(strCurPageContent);
                            // �Ƿ��������ҳ��ʶ��
                            if (bQuik)
                                break;
                            else
                                continue;
                        }
                        else
                        {
                            // �Ƿ��������ҳ��ʶ��
                            if (bQuik)
                            {
                                pageList.push_back(strCurPageContent);
                                break;
                            }
                        }
                    }
                }            

                // ͼƬ
                for (int imgIndex = 0; imgIndex < page.imgURIs.size(); ++imgIndex)
                {
                    if (page.imgURIs[imgIndex].empty())
                        continue;
                    LOGI("����ʶ pdf ��� " << pgIndex + 1 << " ҳ, �� " << imgIndex + 1 << " ��ͼƬ...");

                    Mat srcImage = imread(page.imgURIs[imgIndex]);
                    bOk = regcoMat(srcImage, strTmp);
                    if (!bOk)
                        break;
                    if (!res.isHit)
                    {
                        res.isHit = checkAndHandle(srcImage, hitKeyword, pgIndex + 1);
                        if (res.isHit)
                        {
                            LOGI("pdf ͼƬ�б�keyword: " << hitKeyword);
                            srcImage.copyTo(resultImg);
                            res.pageNo = pgIndex + 1;
                        }
                    }
                    strCurPageContent += " " + strTmp;
                    // �Ƿ������ǰҳ����һ��ͼƬʶ��
                    if (res.isHit && bQuik)
                        break;
                }
                pageList.push_back(strCurPageContent);
                
                // �Ƿ��������ҳ��ʶ��
                if (res.isHit && bQuik)
                    break;
            }
        }
        catch (PdfError & e)
        {
            LOGE("Error: An error " << e.GetError() << " ocurred during processing the pdf file. msg: " << e.what());
            fprintf(stderr, "Error: An error %i ocurred during processing the pdf file.\n", e.GetError());
            return false;
        }
    }
    else if (EN_OcrSourceType::en_OcrSourceTif == enOcrSourceType)
    {
        std::string outStr;
        CMultiPageImageBase& imagePage = CTiffImage();
        if (!imagePage.loadFromFile(strSrcFilePath))
        {
            LOGE("ͼƬ����ʧ��!");
            return false;
        }
        const int pageCount = imagePage.getPageCount();
        for (int i = 0; i < pageCount; ++i)
        {
            LOGD("����ʶ�� tif �� " << i + 1 << " ��ͼƬ...");

            Mat srcImage = imagePage.getMat(i);
            bOk = regcoMat(srcImage, outStr);
            if (!bOk)
                break;
            pageList.push_back(outStr);

            if (!res.isHit)
            {
                res.isHit = checkAndHandle(srcImage, hitKeyword, i + 1);
                if (res.isHit)
                {
                    LOGI("ͼƬ�б�keyword: " << hitKeyword);
                    srcImage.copyTo(resultImg);
                    res.pageNo = i + 1;
                }
            }
            // �Ƿ��������ҳ��ʶ��
            if (res.isHit && bQuik)
                break;
        }
    }
    else
    {
        Mat srcImage = imread(strSrcFilePath);

        std::string outStr;
        bOk = regcoMat(srcImage, outStr);
        if (bOk)
        {
            pageList.push_back(outStr);

            res.isHit = checkAndHandle(srcImage, hitKeyword, 1);
            if (res.isHit)
            {
                LOGI("ͼƬ�б�keyword: " << hitKeyword);
                srcImage.copyTo(resultImg);
                res.pageNo = 1;
            }
        }
    }

    
    if (bOk)
    {
        makePages(pageList, res);

        vector<uchar> imgBuff;//buffer for coding 
        if (res.isHit)
        {
            res.keyword = hitKeyword.data();

            if (m_privilgeLevel == en_OcrPrivilgeAdvanced)
            {// ��Ȩ��, �Ż����ͼƬ����, ���ظ�����ʾ��ͼƬ
                vector<int> param = vector<int>(2);
                param[0] = CV_IMWRITE_JPEG_QUALITY;
                param[1] = 95;//default(95) 0-100 
                cv::imencode(".jpg", resultImg, imgBuff, param);

                res.pImgData = imgBuff.data();
                res.imgLen = imgBuff.size();
            }
        }
        callback(res, pUserData);
        releaseResult(res);
    }

    LOGI("ʶ�����");
    return bOk;
}

void COcrByTesseract::makePages(std::vector<std::string>& pageList, RegcoResult& result) noexcept
{
    result.pageCount = pageList.size();
    result.pages = new char*[result.pageCount];
    std::string strTmp;
    for (int i = 0; i < result.pageCount; ++i)
    {
        strTmp = pageList[i];
        result.pages[i] = new char[strTmp.size() + 1];
        std::strcpy(result.pages[i], strTmp.data());
    }
}

void COcrByTesseract::releaseResult(RegcoResult& result) noexcept
{
    for (int i = 0; i < result.pageCount; ++i)
        delete[] result.pages[i];
    delete[] result.pages;
}

bool COcrByTesseract::RegcoMem(const unsigned char *pSourceFileMem, unsigned dwSourceFileSize, FNC_OnFinished callback, void* pUserData, bool bQuik
    , EN_OcrSourceType enOcrSourceType) noexcept
{
    LOGI("ʶ��ʼ...");
    if (nullptr == callback)
    {
        LOGD("�����ûص�����!");
        return false;
    }

    std::vector<uchar> data(pSourceFileMem, pSourceFileMem + dwSourceFileSize);
    Mat srcImage = imdecode(data, CV_LOAD_IMAGE_COLOR);

    std::vector<std::string> pageList;
    std::string outStr;
    bool bOk = regcoMat(srcImage, outStr);
    pageList.push_back(outStr);

    if (bOk)
    {
        RegcoResult res;
        makePages(pageList, res);

        vector<uchar> imgBuff;//buffer for coding 
        std::string hitKeyword;
        res.isHit = checkAndHandle(srcImage, hitKeyword, 1);
        res.pageNo = 1;
        res.pageCount = 1;
        if (res.isHit)
        {
            res.keyword = hitKeyword.data();

            //(1) jpeg compression 
            vector<int> param = vector<int>(2);
            param[0] = CV_IMWRITE_JPEG_QUALITY;
            param[1] = 95;//default(95) 0-100 
            cv::imencode(".jpg", srcImage, imgBuff, param);

            res.pImgData = imgBuff.data();
            res.imgLen = imgBuff.size();
        }
        callback(res, pUserData);
        releaseResult(res);
    }

    LOGI("ʶ�����");
    return bOk;
}

bool COcrByTesseract::regcoMat(const cv::Mat& srcImage, std::string& outStr, bool canRotate) noexcept
{
    if (!m_bInit)
        return false;

    LOGI("bengin...");
    CMyClock myClock;

    
    if (srcImage.empty())
    {
        LOGE("��ͼƬʧ��!");
        return false;
    }
#ifdef MY_DEBUG
    namedWindow("ԭͼ", WINDOW_AUTOSIZE);
    imshow("ԭͼ", srcImage);
#endif

    // 1> �ҶȻ�
    Mat outImage;
    cvtColor(srcImage, outImage, CV_BGR2GRAY);
#ifdef MY_DEBUG
    namedWindow("�ҶȻ�", WINDOW_AUTOSIZE);
    imshow("�ҶȻ�", outImage);
#endif

    float timeLen;
    if (canRotate && m_privilgeLevel == en_OcrPrivilgeAdvanced)
    {
        myClock.clockBeg();
        adjustImageOrient(outImage);
        timeLen = myClock.clockEnd();
        LOGD("����ı����� ��ʱ: " << timeLen << " ��");
#ifdef MY_DEBUG
        namedWindow("��ת��", WINDOW_AUTOSIZE);
        imshow("��ת��", outImage);
#endif
    }

    improveImg(outImage);


#ifdef MY_DEBUG
    waitKey();
#endif
    // ʶ��
    std::string strResult = "";
    //char* pBuffer;

    LOGI("��ʼʶ��...");
    myClock.clockBeg();
    m_apiChi.SetImage(outImage.data, outImage.cols, outImage.rows, outImage.channels(), outImage.step);
    m_apiChi.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO_ONLY);
    
    
    cv::Rect rtTmp;
    MyOCRWord wdTmp;
    // ��ʼʶ��
    m_apiChi.Recognize(0);

    // ���ش���ʽ���ı�
    char* pBuffer = m_apiChi.GetUTF8Text();
    strResult = pBuffer;
    delete[] pBuffer;
    strResult = XAB::CTextHelper::UTF2GBK(strResult);
    outStr = strResult;

    LOGD("ʶ����ı�: \n" << outStr);
    timeLen = myClock.clockEnd();
    LOGD("ʶ���ʱ: " << timeLen << " ��");

    // ��¼ÿ���ַ���λ����Ϣ, ������ͼƬ�Ͻ��и�����ʾ
    tesseract::ResultIterator* ri = m_apiChi.GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
    string sss;
    if (ri != 0) 
    {
        MyOCRChar chTmp;
        m_charList.clear();
        do 
        {
            const char* chr = ri->GetUTF8Text(level);
            if (chr)
            {// ���ͼƬ����б��, ��ָ��Ϊ��
                const float conf = ri->Confidence(level);
                int x1, y1, x2, y2;
                ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                //wdTmp.word = word;
                // ע��: Ӣ�ĵ���֮��Ӧ���Կո����ֿ�, �����ĵ�word����Ҫ�ո�
                if (std::isalpha(chr[0], m_loc))
                    chTmp.chr = string(chr);
                else
                    chTmp.chr = XAB::CTextHelper::UTF2GBK(chr);
                chTmp.rect.x = x1;
                chTmp.rect.y = y1;
                chTmp.rect.width = x2 - x1;
                chTmp.rect.height = y2 - y1;
                m_charList.push_back(chTmp);

                delete[] chr;
            }
        } while (ri->Next(level));
    }

    LOGI("end...");

    return true;
}

bool COcrByTesseract::checkAndHandle(cv::Mat& srcImage, std::string& hitKeyword, int pageNo)
{
    if (m_charList.empty())
    {
        LOGD("�ַ��б�Ϊ��!");
        return false;
    }

    LOGD("�б���begin");
    string symbContent = ""; // û�пհ��ַ����ַ���
    for (const auto& chrStr : m_charList)
        symbContent += chrStr.chr;
    LOGD("symbContent = " << symbContent);


    bool bFind = false;
    size_t pos;
    size_t totalPos;
    int index;
    string strKeywordNoBlank;
    vector<int> indexList;
    for (const auto& keyword : m_keywords)
    {
        // ȥ��keyword �е����пո�
        static std::regex pattern("[[:blank:]]*");
        // ����ȥ�� keyword �е����пհ��ַ�
        strKeywordNoBlank = std::regex_replace(keyword, pattern, "");
        LOGD("keyword no blank: " << strKeywordNoBlank);

        // �Ȳ��ҵ�λ��
        pos = symbContent.find(keyword);
        if (string::npos == pos)
        {
            continue;
        }
        else
        {
            bFind = true;
            hitKeyword = keyword;
            LOGD("�б�keyword: " << keyword << ", λ��: " << pos);
            break;
        }        
    }
    if (bFind && m_privilgeLevel == en_OcrPrivilgeAdvanced)
    {
        const int keywordLen = strKeywordNoBlank.length();
        // Ȼ���ٶ�λ�������ַ�Ԫ��
        totalPos = 0;
        index = 0;
        for (const auto& e : m_charList)
        {
            totalPos += e.chr.length();
            if (totalPos > pos)
            {// �ҵ�λ��
                break;
            }
            ++index;
        }
        do
        {
            indexList.push_back(index);
                
            if (totalPos < pos + keywordLen)
                totalPos += m_charList[++index].chr.length();
            else
                break;

        } while (true);

        LOGD("rect count: " << indexList.size());
        vector<cv::Rect> rtList;
        // �ϲ�ͬ�о���
        index = indexList[0];
        cv::Rect rtTmp = m_charList[index].rect;
        if (indexList.size() > 1)
        {
            for (int i = 1; i < indexList.size(); ++i)
            {
                index = indexList[i];
                if (isSameLine(rtTmp, m_charList[index].rect))
                    rtTmp = mergeRect(rtTmp, m_charList[index].rect);
                else
                {
                    rtList.push_back(rtTmp);
                    rtTmp = m_charList[index].rect;
                }
            }
        }            
        rtList.push_back(rtTmp);
            

        // ���ƾ���
        for (const auto& rt : rtList)
            cv::rectangle(srcImage, rt, Scalar(0, 0, 255));
                
        const string ss = "page number: " + std::to_string(pageNo);
        cv::putText(srcImage, ss, Point(0, srcImage.rows - 10), HersheyFonts::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255)); // ����ҳ��
    }
    LOGD("�б���end");

    return bFind;
}

bool COcrByTesseract::isSameLine(const cv::Rect& box1, const cv::Rect& box2)
{
    if (box1.y + box1.height < box2.y || box1.y > box2.y + box2.height)
        return false;
    else
        return true;
}
cv::Rect COcrByTesseract::mergeRect(const cv::Rect& box1, const cv::Rect& box2)
{
    Rect rtTmp;
    if (box1.x < box2.x)
        rtTmp.x = box1.x;
    else
        rtTmp.x = box2.x;

    if (box1.y < box2.y)
        rtTmp.y = box1.y;
    else
        rtTmp.y = box2.y;

    if (box1.x + box1.width > box2.x + box2.width)
        rtTmp.width = box1.x + box1.width - rtTmp.x;
    else
        rtTmp.width = box2.x + box2.width - rtTmp.x;

    if (box1.y + box1.height > box2.y + box2.height)
        rtTmp.height = box1.y + box1.height - rtTmp.y;
    else
        rtTmp.height = box2.y + box2.height - rtTmp.y;

    return rtTmp;
}

void COcrByTesseract::removeRt(std::string& srcStr)
{
    static std::regex pattern("[\n|\r]");
    // ����ȥ�����лس�
    srcStr = std::regex_replace(srcStr, pattern, "");
}

void COcrByTesseract::adjustImageOrient(cv::Mat& srcImage)
{
    if (!m_bInit)
        return;

// eng, chi_sim
    //���� 620 * 1119 �ֱ��ʵ�ͼƬ, �׵׺��ִ������ı�:
    //�����������ѡ�� eng, ���ʱ2�������, ���Ŷ� : 1.41327;
    //���ѡ chi_sim, ���ʱ28 ��������, ���Ŷ�: 46.2038
    //    ���ѡ eng+chi_sim, ���ʱ5 ������, ���Ŷ� : 1.41327
    //    ���ѡ chi_sim+eng, ���ʱ28 ������, ���Ŷ�: 46.2038
    //    ���л��� : win7 �����, ˫��, 2G�ڴ�
    // 1> ������ͼƬ, ��ת�Ƕ�С��90��
    double degree = CalcDegree(srcImage);
    LOGD("degree:" << degree);
    if (std::abs(degree) - 0.1 > 0)
    {
        myRotate(srcImage, -degree);
#ifdef MY_DEBUG
        imshow("������ͼƬ", srcImage);
#endif
    }


    // 2> �ٻ��ҳ���������ת��, �ýǶ���90�ȱ���
    m_apiEng.InitForAnalysePage();
    m_apiEng.SetPageSegMode(tesseract::PageSegMode::PSM_OSD_ONLY);
    m_apiEng.SetImage(srcImage.data, srcImage.cols, srcImage.rows, srcImage.channels(), srcImage.step);
    int orient_deg;
    float orient_conf;
    const char* script_name;
    float script_conf;
    if (m_apiEng.DetectOrientationScript(&orient_deg, &orient_conf, &script_name, &script_conf))
    {
        /*
        ֻ֧����ת��Ϊ90�ȵı���
        confidence (15.0 is reasonably confident)
        ���ͼƬ������, �����: deg: 0, confidence: 3.76825, script_name: Latin, script_conf: 5.33333
        ���˳ʱ����ת90��, ���: deg: 90, confidence: 2.96749, script_name: Latin, script_conf: 5.33333
        180��: deg: 180, confidence: 3.46679, script_name: Latin, script_conf: 5.33333
        270��: deg: 270, confidence: 3.01704, script_name: Latin, script_conf: 6.2963
        */
        ;
    }
    else
    {
        orient_deg = 0;
    }
    LOGD("degree: " << orient_deg << ", confidence: " << orient_conf << ", script_name: " << script_name << ", script_conf: " << script_conf);
    if (orient_deg > 0.1)
        myRotate(srcImage, -orient_deg);
}

// ͨ������任����Ƕ�, ����˳ʱ��Ƕ�
// 1> ֻȡ�����ҳ���ֱ����, �Ƕ�����(���С��1��)�������߶�,
// 2> ����ĽǶȲ���90 �ȱ���, ��Ϊ�����90 �ȱ���, ˵��ͼƬ�ǲ���Ҫ��ת
double COcrByTesseract::CalcDegree(const cv::Mat &srcImage)
{
    Mat dstImage;
    Canny(srcImage, dstImage, 50, 200, 3);

    //ͨ������任���ֱ��
    vector<Vec2f> lines;
    HoughLines(dstImage, lines, 1, CV_PI / 180, 300, 0, 0);//��5������������ֵ����ֵԽ�󣬼�⾫��Խ��

    //����ͼ��ͬ����ֵ�����趨����Ϊ��ֵ�趨���ߵ����޷����ֱ�ߣ���ֵ����ֱ��̫�࣬�ٶȺ���
    //���Ը�����ֵ�ɴ�С������������ֵ�����������������󣬿��Թ̶�һ���ʺϵ���ֵ��
    if (!lines.size())
    {
        HoughLines(dstImage, lines, 1, CV_PI / 180, 200, 0, 0);
    }

    if (!lines.size())
    {
        HoughLines(dstImage, lines, 1, CV_PI / 180, 150, 0, 0);
    }
    if (!lines.size())
    {
        LOGE("û�м�⵽ֱ�ߣ�");
        return 0;
    }

    LOGD("line size = " << lines.size());
    //Vec4f  0: �߶γ�, 1: ����, 2: �Ƕ�, 3: �������߶νǶ����Ƶĸ���
    list<Vec4f> lineList(lines.size());
    auto pLine1 = lineList.begin();
    auto pLine2 = lines.cbegin();
    for (; pLine1 != lineList.end(); ++pLine1, ++pLine2)
    {
        (*pLine1)[0] = (*pLine2)[0];
        (*pLine1)[1] = (*pLine2)[1];
        (*pLine1)[2] = DegreeTrans((*pLine2)[1]);;
        (*pLine1)[3] = 0;
    }

    // 1> �ҳ��Ƕ�����С��, �������������߶�
    int maxCount = 0;
    for (auto& line : lineList)
    {
        for (const auto& lineTmp : lineList)
        {
            if (std::abs(line[2] - lineTmp[2]) - 1 < 0)
                ++line[3];
        }
        if (line[3] > maxCount)
            maxCount = line[3];
    }
    // 2> ���˳����Ƹ���Ϊ maxCount ���߶�
    for (auto it = lineList.begin(); it != lineList.end();)
    {
        if ((*it)[3] == maxCount)
            ++it;
        else
            it = lineList.erase(it);
    }
    LOGD("������ƽǵ��߶�: line size = " << lineList.size() << ", maxCount = " << maxCount);

    // 3> ת������Ϊ�Ƕ�, ����ȥ������ 90�� ������line
    for (auto it = lineList.begin(); it != lineList.end();)
    {
        const float mod = std::fmod((*it)[2], 90);
        if (std::abs(mod) - 0.1 < 0)
            it = lineList.erase(it);
        else
            ++it;
    }
    LOGD("ȥ��90�ȱ������߶�: line size = " << lineList.size());


    float sum = 0;
    //���λ���ÿ���߶�, ������ƽ���Ƕ�
    Point pt1, pt2;
    for (const auto item : lineList)
    {
        const float theta = item[1];
        sum += item[2];
#ifdef MY_DEBUG
        const float rho = item[0];
        const double a = cos(theta), b = sin(theta);
        const double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dstImage, pt1, pt2, Scalar(255), 1, LINE_AA); //Scalar�������ڵ����߶���ɫ
        imshow("ֱ��̽��Ч��ͼ", dstImage);
#endif
    }
    float average = 0;
    if (lineList.size() > 0)
        average = sum / lineList.size(); //�����нǶ���ƽ������������תЧ������� lines.size()

    LOGD("line size: " << lineList.size());
    if (std::abs(average) - 0.1 > 0)
        return std::round(average * 10) / 10 - 90;
    else
        return 0;
}

// ����ת��
double COcrByTesseract::DegreeTrans(double theta)
{
    const double res = theta / CV_PI * 180;
    return res;
}

// ˳ʱ����ת
void COcrByTesseract::myRotate(cv::Mat& srcImage, double degree) noexcept
{
    degree = -degree;
    const double angle = degree  * CV_PI / 180.; // ����
    const double a = sin(angle), b = cos(angle);
    const int width = srcImage.cols;
    const int height = srcImage.rows;
    const int width_rotate = int(height * fabs(a) + width * fabs(b));
    const int height_rotate = int(width * fabs(a) + height * fabs(b));
    //��ת����map
    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

    cv::Mat rotMat(2, 3, CV_32F);
    cv::Point center = cv::Point(srcImage.cols / 2, srcImage.rows / 2);
    double scale = 1;
    // ͨ���������תϸ����Ϣ�����ת����
    rotMat = getRotationMatrix2D(center, degree, scale);

    // ƽ�Ƶ���������������
    rotMat.ptr<double>(0)[2] += (width_rotate - width) / 2;
    rotMat.ptr<double>(1)[2] += (height_rotate - height) / 2;

    cv::bitwise_not(srcImage, srcImage);
    cv::Mat img_rotate;

    //��ͼ��������任
    const double angle_less90 = std::fmod(degree, 90);
    const double angle_large = degree - angle_less90;
    if (std::abs(angle_large) - 0.1 > 0)
        warpAffine(srcImage, img_rotate, rotMat, cv::Size(width_rotate, height_rotate));
    else if (std::abs(angle_less90) - 0.1 > 0)
        warpAffine(srcImage, img_rotate, rotMat, cv::Size(width_rotate, height_rotate), INTER_CUBIC); // 
    else
    {
        cv::bitwise_not(srcImage, srcImage);
        return;
    }

    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img_rotate.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img_rotate.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
    cv::Rect rt = box.boundingRect();
    cv::getRectSubPix(img_rotate, rt.size(), box.center, srcImage);

    //srcImage = img_rotate;
    cv::bitwise_not(srcImage, srcImage);
}

void COcrByTesseract::improveImg(cv::Mat& srcImg) noexcept
{
    gammaCorrection(srcImg, srcImg);

    // 2> ��ֵ��
    //const int srcChannel = srcImg.channels();
    //int iTmp;
    //float thresh = 0;
    //unsigned int nCount = 0;
    //for (int row = 0; row < srcImg.rows; ++row)
    //{
    //    const uchar* srcPtr = srcImg.ptr<uchar>(row);
    //    for (int col = 0; col < srcImg.cols; ++col)
    //    {
    //        if (srcChannel == 3)
    //        {
    //            const int b = srcPtr[col * srcChannel];
    //            const int g = srcPtr[col * srcChannel + 1];
    //            const int r = srcPtr[col * srcChannel + 2];

    //            iTmp = (b + g + r) / 3.0;
    //        }
    //        else if (srcChannel == 1)
    //        {
    //            iTmp = srcPtr[col * srcChannel];
    //        }

    //        // ������ֵ
    //        if (iTmp > 0 && iTmp < 255)
    //        {
    //            // method 1:
    //            // thresh = 0.1 * thresh + 0.9 * iTmp; // ���Ȩ

    //            // method 2:
    //            thresh += iTmp;
    //            ++nCount;
    //        }
    //    }
    //}
    //thresh = thresh / nCount;
    //if (m_thresh != -1)
    //    thresh = m_thresh;
    //LOGI("��ֵ����ֵ: " << thresh);
    //threshold(srcImg, srcImg, thresh, 255, THRESH_BINARY);

    // method 2: 
    threshold(srcImg, srcImg, 0, 255, THRESH_OTSU | THRESH_BINARY);
#ifdef MY_DEBUG
    namedWindow("��ֵ��", WINDOW_AUTOSIZE);
    imshow("��ֵ��", srcImg);
#endif
}

void COcrByTesseract::gammaCorrection(const cv::Mat &img, cv::Mat& dst)
{
    //![changing-contrast-brightness-gamma-correction]
    dst = img.clone();
    LUT(img, m_lookUpTable, dst); // http://docs.opencv.org/master/d2/de8/group__core__array.html#gab55b8d062b7f5587720ede032d34156f
}

void COcrByTesseract::setThresh(float thresh)  noexcept
{
    m_thresh = thresh;
}

void CMyClock::clockBeg() noexcept
{
    m_tmpTime  = chrono::system_clock::now();
}

float CMyClock::clockEnd() noexcept
{
    const auto timePoint2 = chrono::system_clock::now();
    const auto tickets = timePoint2 - m_tmpTime;
    const auto timeLen = std::chrono::duration_cast<std::chrono::milliseconds>(tickets);

    return timeLen.count() / 1000.0;
}
