#pragma once

#include "Ado.h"
#include <string>

/// ��ӡ����
enum OcrType
{
    Print = 1 ///< ��ӡ���ƴ�ӡ
  , Copy      ///< ��ӡ����
  , Scan      ///< ɨ��
  , Fax       ///< ����
};

/// ͼƬ��¼
struct ImgRecord 
{
    OcrType ocrType;
    std::string fileURI; // ͼƬ�ļ�ȫ·��
    std::string oriId;   // ���ݴ�ӡ����, ��Ӧ��ͬ����ֶ�
    std::string user;    // ��ӡͼƬ����
    std::string printAddr; // ��ӡ��ַ
    std::string submitAddr; // �ύ��ַ
    std::string onDataTime;
    std::string title;   // ͼƬ�ĵ��ı���
};

class COcrDBSource
{
public:
    COcrDBSource();
    ~COcrDBSource(void);

    bool checkAndConnect();
    void getImagesURI(std::queue<ImgRecord>& quImgRecord);
    void getKeyWords(std::vector<std::string>& keyWords);
    void insert2LogOcr(const ImgRecord& ird, const std::string& txtPath, bool bFind, int pageNo, const std::string& keyWord); ///< ������ʶ����־��(t_log_ocr) �� �б�׷�ݱ�(t_log_OcrChecked)
    std::vector<OcrType> getSrcTypes(); ///< ��ȡ�ѿ����Ĵ�ӡԴ����
public:
    boost::shared_ptr<CAdo> m_pAdo;
};
