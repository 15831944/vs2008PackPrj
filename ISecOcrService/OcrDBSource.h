#pragma once

#include "Ado.h"
#include <string>

/// 打印类型
enum OcrType
{
    Print = 1 ///< 打印及云打印
  , Copy      ///< 复印留底
  , Scan      ///< 扫描
  , Fax       ///< 传真
};

/// 图片记录
struct ImgRecord 
{
    OcrType ocrType;
    std::string fileURI; // 图片文件全路径
    std::string oriId;   // 根据打印类型, 对应不同表的字段
    std::string user;    // 打印图片的人
    std::string printAddr; // 打印地址
    std::string submitAddr; // 提交地址
    std::string onDataTime;
    std::string title;   // 图片文档的标题
};

class COcrDBSource
{
public:
    COcrDBSource();
    ~COcrDBSource(void);

    bool checkAndConnect();
    void getImagesURI(std::queue<ImgRecord>& quImgRecord);
    void getKeyWords(std::vector<std::string>& keyWords);
    void insert2LogOcr(const ImgRecord& ird, const std::string& txtPath, bool bFind, int pageNo, const std::string& keyWord); ///< 插入已识别日志表(t_log_ocr) 和 中标追溯表(t_log_OcrChecked)
    std::vector<OcrType> getSrcTypes(); ///< 获取已开启的打印源类型
public:
    boost::shared_ptr<CAdo> m_pAdo;
};
