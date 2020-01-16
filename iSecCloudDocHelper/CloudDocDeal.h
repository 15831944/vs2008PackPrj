#pragma once

class CCloudDocDeal
{
public:
	CCloudDocDeal(void);
	CCloudDocDeal(PCloudJobInfo job);
	~CCloudDocDeal(void);

	void SetJobInfo(PCloudJobInfo job);
	PCloudJobInfo GetJobInfo();
	void Release();

	BOOL IsGzFile();
	BOOL IsImageFile();
	BOOL IsPdfFile();
	BOOL IsWordFile();
	BOOL IsPptFile();
	BOOL IsExcelFile();
	BOOL ConvertFile();

protected:
	BOOL InitFilePath();
	BOOL UnCompressGzFile(IN CString szFileGzPath, IN CString szFileOrgPath, BOOL bDelGzFile);
	BOOL CompressGzFile(IN CString szFileOrg, IN CString szFileGzPath, BOOL bDelOrgFile);
	CString GetFileNameByPathName(CString szPathName);
	CString GetDirByFilePath(CString szFilePath);

protected:
	PCloudJobInfo m_pJob;
	CString m_szOrgJobPath;	//云文档处理前的文档名，可能是gz压缩包，可能是未压缩的，如test.doc.gz或者test.doc
	CString m_szJobPath;	//云文档处理前的文档名，一定是未压缩的，如test.doc
	CString m_szDealSaveJobPath;	//云文档处理后的文档全路径,图片转换后一般是tif,office转换后pdf、tif
	CString m_szNewJobName;//云文档处理后的文档名，可能是gz压缩包，可能是未压缩的，如test.doc.tif.gz或者test.doc.tif，最终取文件是以这个文件为准
	CString m_szNewJobPath;
	BOOL m_bNeedDelOrgFile;
};
