#ifndef OCRREGCO_H
#define OCRREGCO_H


///Ocr��ʶ���Դ����
typedef enum tag_EN_OcrSourceType
{
	en_OcrSourceUnknown	= -1	
  , en_OcrSourcePdf	= 0
  , en_OcrSourceTif	= 1
  , en_OcrSourceJpg	= 2
  , en_OcrSourcePng	= 3
} EN_OcrSourceType;

typedef enum {
    en_OcrPrivilgeNormal = 0
  , en_OcrPrivilgeAdvanced = 1
} EN_OcrPrivilgeLevel;

/// ʶ����
struct RegcoResult {
    bool isHit; ///< �Ƿ��б�
	char** pages;///< ����ҳ�ĵ�����
	int pageCount;
    const unsigned char* pImgData; ///< ��ʱ��ʾ��һ���б��ͼƬ(jpeg), �Ժ���ܸĳ�pdf ��ʾ�Ķ�ҳͼƬ
    int imgLen;
    EN_OcrSourceType imgType;
    int pageNo; ///< �б�ҳ��, ��ʼ��ҪΪ 1
    const char* keyword; ///< ��һ���б�Ĺؼ���

    RegcoResult() 
        : isHit(false), pages(NULL), pageCount(0), pImgData(NULL), imgLen(0), pageNo(-1), keyword(""), imgType(en_OcrSourceJpg)
    {}
};

/// ÿ��ʶ����ɵĻص�����
///
/// @ocrResult ʶ����
/// @pUserData �û�����
typedef void(*FNC_OnFinished)(const RegcoResult& ocrResult, void* pUserData);

/// Ocrʶ�����
class COcrRegco
{
public:
    COcrRegco() : m_bInit(false) {};
    virtual ~COcrRegco() {};

    /// �Ƿ�����־
    virtual void EnableLog(bool val) = 0;
    /// ���ð汾����, �����û�����İ汾, ���ַ���ȼ�
    ///
    /// @privilgeLevel ��������: �ɶԴ�ӡ��ӡɨ�贫�������е����ֽ��йؼ��ּ�����׷��; �߼�: ���Ӹ�����ʾ�������Զ���ƫ
    virtual void SetPrivilLevel(EN_OcrPrivilgeLevel privilgeLevel) = 0;
    /// ��ʼ��������������Դ
    ///
    /// @dwTimeOutMS ��ʱ������, ��ʱûЧ��
    /// @return �����Ƿ��ʼ���ɹ�
    virtual bool Init(unsigned dwTimeOutMS) = 0;
	/// ���ü����ؼ���
	virtual void SetKeywords(char* keyWords[], int keyWordCount) = 0;
    /// ʶ���ļ�, ȡ��ʶ���ı�
	///
	/// @strSrcFilePath ��ʶ���Դ�ļ�·��
	/// @callback ʶ����ɺ�Ļص�
    /// @bQuik ��ʾ�Ƿ��б�������˳�, ���Ϊtrue, ���б������ִ�� callback, �Ӷ�����������ҳ��ʶ��
    /// @pUserData �û�����
	/// @EN_OcrSourceType Դ�ļ�������
    /// @return �����Ƿ����
    virtual bool RegcoFile(const char* strSrcFilePath, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) = 0;
    /// ʶ�������ڴ���е��ļ�
	///
	/// @pSourceFileMem ��ʶ���Դ�ļ��ڴ�ָ��
	/// @dwSourceFileSize ��ʶ���Դ�ļ���С
    /// @callback ʶ����ɺ�Ļص�
	/// @result ����ʶ����, �����Release �ͷ�
    /// @bQuik ��ʾ�Ƿ��б�������˳�, ���Ϊtrue, ���б������ִ�� callback, �Ӷ�����������ҳ��ʶ��
	/// @enOcrSourceType Դ�ļ�������
    /// @return �����Ƿ����
    virtual bool RegcoMem(const unsigned char *pSourceFileMem, unsigned dwSourceFileSize, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) = 0;
	///�ͷ���Դ, ������һ�����ͷ� ocr �������
    virtual void Release() = 0;	
protected:
	int	m_bInit;	///< �Ƿ��ѱ���ʼ��
};

#endif