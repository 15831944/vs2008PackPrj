#ifndef OCRREGCO_H
#define OCRREGCO_H


///Ocr待识别的源类型
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

/// 识别结果
struct RegcoResult {
    bool isHit; ///< 是否中标
	char** pages;///< 所有页的的内容
	int pageCount;
    const unsigned char* pImgData; ///< 暂时表示第一次中标的图片(jpeg), 以后可能改成pdf 表示的多页图片
    int imgLen;
    EN_OcrSourceType imgType;
    int pageNo; ///< 中标页码, 起始需要为 1
    const char* keyword; ///< 第一次中标的关键词

    RegcoResult() 
        : isHit(false), pages(NULL), pageCount(0), pImgData(NULL), imgLen(0), pageNo(-1), keyword(""), imgType(en_OcrSourceJpg)
    {}
};

/// 每次识别完成的回调函数
///
/// @ocrResult 识别结果
/// @pUserData 用户数据
typedef void(*FNC_OnFinished)(const RegcoResult& ocrResult, void* pUserData);

/// Ocr识别基类
class COcrRegco
{
public:
    COcrRegco() : m_bInit(false) {};
    virtual ~COcrRegco() {};

    /// 是否开启日志
    virtual void EnableLog(bool val) = 0;
    /// 设置版本级别, 根据用户购买的版本, 划分服务等级
    ///
    /// @privilgeLevel 正常级别: 可对打印复印扫描传真内容中的文字进行关键字检索、追溯; 高级: 增加高亮显示；版面自动纠偏
    virtual void SetPrivilLevel(EN_OcrPrivilgeLevel privilgeLevel) = 0;
    /// 初始化函数，分配资源
    ///
    /// @dwTimeOutMS 超时毫秒数, 暂时没效果
    /// @return 返回是否初始化成功
    virtual bool Init(unsigned dwTimeOutMS) = 0;
	/// 设置检索关键词
	virtual void SetKeywords(char* keyWords[], int keyWordCount) = 0;
    /// 识别文件, 取得识别文本
	///
	/// @strSrcFilePath 待识别的源文件路径
	/// @callback 识别完成后的回调
    /// @bQuik 表示是否中标后立即退出, 如果为true, 则中标后立即执行 callback, 从而不继续后续页的识别
    /// @pUserData 用户数据
	/// @EN_OcrSourceType 源文件的类型
    /// @return 返回是否出错
    virtual bool RegcoFile(const char* strSrcFilePath, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) = 0;
    /// 识别存放在内存块中的文件
	///
	/// @pSourceFileMem 待识别的源文件内存指针
	/// @dwSourceFileSize 待识别的源文件大小
    /// @callback 识别完成后的回调
	/// @result 返回识别结果, 请调用Release 释放
    /// @bQuik 表示是否中标后立即退出, 如果为true, 则中标后立即执行 callback, 从而不继续后续页的识别
	/// @enOcrSourceType 源文件的类型
    /// @return 返回是否出错
    virtual bool RegcoMem(const unsigned char *pSourceFileMem, unsigned dwSourceFileSize, FNC_OnFinished callback, void* pUserData, bool bQuik
        , EN_OcrSourceType enOcrSourceType = en_OcrSourceUnknown) = 0;
	///释放资源, 在这里一般是释放 ocr 引擎对象
    virtual void Release() = 0;	
protected:
	int	m_bInit;	///< 是否已被初始化
};

#endif