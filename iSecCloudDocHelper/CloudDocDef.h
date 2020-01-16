#ifndef _Cloud_Doc_Def_H_D82B4162_CC81_47e0_A799_EB3B16D3405C_
#define _Cloud_Doc_Def_H_D82B4162_CC81_47e0_A799_EB3B16D3405C_

typedef struct tag_CloudJobInfo
{
	int nJobId;
	int nJobState;
	int nFileState;
	int nFileDeleted;
	int nDocSize;				//云文档原始文档大小，如test.doc
	char cDocName[MAX_PATH];	//云文档原始文档名，如test.doc
	int nPaperSize;
	char cPageType[50];
	int nOrientation;
	int nCopies;
	int nColor;
	int nDoublePrint;
	int nPaperSource;
	int nTotalPage;
	int nOrgJobSize;			//云文档处理前的文档大小，可能是gz压缩包，可能是未压缩的，如test.doc.gz或者test.doc
	char cOrgJobName[MAX_PATH];	//云文档处理前的文档名，可能是gz压缩包，可能是未压缩的，如test.doc.gz或者test.doc
	int nOrgJobIsGz;			//云文档处理前的文档名，是否为gz压缩包
	int nNewJobSize;			//云文档处理后的文档大小，可能是gz压缩包，可能是未压缩的，如test.doc.tif.gz或者test.doc.tif
	char cNewJobName[MAX_PATH];	//云文档处理后的文档名，可能是gz压缩包，可能是未压缩的，如test.doc.tif.gz或者test.doc.tif，最终取文件是以这个文件为准
	int nNewJobIsGz;			//云文档处理后的文档名，是否是gz压缩包
	char cJobDir[MAX_PATH];		//云文档保存路径，不包含文件名
	int nUserId;
	double fSubmitTime;
	int nEntId;		//企业id
	int nEquipId;	//设备id
}CloudJobInfo, *PCloudJobInfo;

typedef CList<PCloudJobInfo, PCloudJobInfo> CListCloudJobInfo;

typedef struct tag_FtpInfo
{
	char szFtpIP[50];		//ftp服务器的IP
	int nFtpPort;			//ftp的端口号
	char szFtpUser[50];		//ftp的登录用户名
	char szFtpPwd[100];		//ftp的登录密码
	char szFtpDir[300];		//ftp默认目录
	char szFtpDiskDir[300];	//ftp默认目录指向的ftp服务器的硬盘目录，相对ftp服务器的外部程序来说没有意义。
}FtpInfo, *PFtpInfo;

#define Db_Name_Cloud TEXT("iSecCloud")		//真正云打印的云打印数据库名

#define Conn_DataBase_TimeOut_MS	(1000 * 30)		//连接数据库的时间间隔,毫秒
#define Check_Cloud_Doc_TimeOut_MS	(1000 * 5)		//检测所有需要处理云文档的时间间隔，毫秒
#define Check_Cloud_Doc_Deal_TimeOut_MS	(1000 * 1)	//检测下一个需要处理云文档的时间间隔，毫秒
#define Check_Cloud_Config_Change_TimeOut_MS	(1000 * 60 * 60 * 1)		//检测处理云文档策略更改的时间间隔，毫秒
#define Check_Cloud_Db_Conn_TimeOut_MS	(1000 * 60 * 8)		//检测云服务器数据库连接状态的时间间隔，毫秒

#define Max_Wait_Cloud_Doc_Deal_Count	(5000)		//等待处理的云打印文档队列的最大个数

#define Min_Cloud_Doc_Deal_Thread_Count	1	//云文档处理最小线程数

//常见可打印文档格式标识宏
#define Cloud_Doc_Format_UnKnow		0		//未知可打印文档
#define Cloud_Doc_Format_Word		1		//office word
#define Cloud_Doc_Format_PowerPoint	2		//office PowerPoint
#define Cloud_Doc_Format_Excel		3		//office Excel
#define Cloud_Doc_Format_PDF		4		//Adobe Portable Document Format (PDF)
#define Cloud_Doc_Format_XPS		5		//Microsoft XML Paper Specification (XPS)
#define Cloud_Doc_Format_WMF		6		//Windows Metafile Format (WMF)
#define Cloud_Doc_Format_EMF		7		//Windows Enhanced MetaFile format (EMF)
#define Cloud_Doc_Format_PS			8		//Adobe PostScript (PS)
#define Cloud_Doc_Format_PCL		9		//Printer Control Language (PCL)
#define Cloud_Doc_Format_SPL		10		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式)
#define Cloud_Doc_Format_JPG		11		//Joint Photographic Experts Group（联合图像专家小组/JPEG）
#define Cloud_Doc_Format_TIF		12		//Tagged Image File Format（标签图像文件格式/TIFF）
#define Cloud_Doc_Format_PNG		13		//Portable Network Graphic Format (可移植网络图形格式/PNG)
#define Cloud_Doc_Format_BMP		14		//Bitmap (位图)
#define Cloud_Doc_Format_GIF		15		//Graphics Interchange Format (图像互换格式/GIF)

//常见可打印文档格式
#define Cloud_Doc_Format_Suffix_Word				TEXT(".doc")		//office word 97-2003
#define Cloud_Doc_Format_Suffix_Word_GZ				TEXT(".doc.gz")		//office word 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_WordX				TEXT(".docx")		//office word 2007-later
#define Cloud_Doc_Format_Suffix_WordX_GZ			TEXT(".docx.gz")	//office word 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_PowerPoint			TEXT(".ppt")		//office PowerPoint 97-2003
#define Cloud_Doc_Format_Suffix_PowerPoint_GZ		TEXT(".ppt.gz")		//office PowerPoint 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_PowerPointX			TEXT(".pptx")		//office PowerPoint 2007-later
#define Cloud_Doc_Format_Suffix_PowerPointX_GZ		TEXT(".pptx.gz")	//office PowerPoint 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_Excel				TEXT(".xls")		//office Excel 97-2003
#define Cloud_Doc_Format_Suffix_Excel_GZ			TEXT(".xls.gz")		//office Excel 97-2003 压缩包
#define Cloud_Doc_Format_Suffix_ExcelX				TEXT(".xlsx")		//office Excel 2007-later
#define Cloud_Doc_Format_Suffix_ExcelX_GZ			TEXT(".xlsx.gz")	//office Excel 2007-later 压缩包
#define Cloud_Doc_Format_Suffix_PDF					TEXT(".pdf")		//Adobe Portable Document Format (PDF)
#define Cloud_Doc_Format_Suffix_PDF_GZ				TEXT(".pdf.gz")		//Adobe Portable Document Format (PDF) 压缩包
#define Cloud_Doc_Format_Suffix_XPS					TEXT(".xps")		//Microsoft XML Paper Specification (XPS)
#define Cloud_Doc_Format_Suffix_XPS_GZ				TEXT(".xps.gz")		//Microsoft XML Paper Specification (XPS) 压缩包
#define Cloud_Doc_Format_Suffix_WMF					TEXT(".wmf")		//Windows Metafile Format (WMF)
#define Cloud_Doc_Format_Suffix_WMF_GZ				TEXT(".wmf.gz")		//Windows Metafile Format (WMF) 压缩包
#define Cloud_Doc_Format_Suffix_EMF					TEXT(".emf")		//Windows Enhanced MetaFile format (EMF)
#define Cloud_Doc_Format_Suffix_EMF_GZ				TEXT(".emf.gz")		//Windows Enhanced MetaFile format (EMF) 压缩包
#define Cloud_Doc_Format_Suffix_PS					TEXT(".ps")			//Adobe PostScript (PS)
#define Cloud_Doc_Format_Suffix_PS_GZ				TEXT(".ps.gz")		//Adobe PostScript (PS) 压缩包
#define Cloud_Doc_Format_Suffix_PCL					TEXT(".pcl")		//Printer Control Language (PCL)
#define Cloud_Doc_Format_Suffix_PCL_GZ				TEXT(".pcl.gz")		//Printer Control Language (PCL) 压缩包
#define Cloud_Doc_Format_Suffix_SPL					TEXT(".spl")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式)
#define Cloud_Doc_Format_Suffix_SPL_GZ				TEXT(".spl.gz")		//Microsoft? Windows Spool File Format (SPL) (可能是pcl/ps/emf/xps等格式) 压缩包
#define Cloud_Doc_Format_Suffix_JPG					TEXT(".jpg")		//Joint Photographic Experts Group（联合图像专家小组/JPEG）
#define Cloud_Doc_Format_Suffix_JPG_GZ				TEXT(".jpg.gz")		//Joint Photographic Experts Group（联合图像专家小组/JPEG） 压缩包
#define Cloud_Doc_Format_Suffix_JPEG				TEXT(".jpeg")		//Joint Photographic Experts Group（联合图像专家小组/JPEG）
#define Cloud_Doc_Format_Suffix_JPEG_GZ				TEXT(".jpeg.gz")	//Joint Photographic Experts Group（联合图像专家小组/JPEG） 压缩包
#define Cloud_Doc_Format_Suffix_TIF					TEXT(".tif")		//Tagged Image File Format（标签图像文件格式/TIFF）
#define Cloud_Doc_Format_Suffix_TIF_GZ				TEXT(".tif.gz")		//Tagged Image File Format（标签图像文件格式/TIFF） 压缩包
#define Cloud_Doc_Format_Suffix_TIFF				TEXT(".tiff")		//Tagged Image File Format（标签图像文件格式/TIFF）
#define Cloud_Doc_Format_Suffix_TIFF_GZ				TEXT(".tiff.gz")	//Tagged Image File Format（标签图像文件格式/TIFF） 压缩包
#define Cloud_Doc_Format_Suffix_PNG					TEXT(".png")		//Portable Network Graphic Format (可移植网络图形格式/PNG)
#define Cloud_Doc_Format_Suffix_PNG_GZ				TEXT(".png.gz")		//Portable Network Graphic Format (可移植网络图形格式/PNG) 压缩包
#define Cloud_Doc_Format_Suffix_BMP					TEXT(".bmp")		//Bitmap (位图)
#define Cloud_Doc_Format_Suffix_BMP_GZ				TEXT(".bmp.gz")		//Bitmap (位图) 压缩包
#define Cloud_Doc_Format_Suffix_GIF					TEXT(".gif")		//Graphics Interchange Format (图像互换格式/GIF)
#define Cloud_Doc_Format_Suffix_GIF_GZ				TEXT(".gif.gz")		//Graphics Interchange Format (图像互换格式/GIF) 压缩包

#define Cloud_Doc_Format_Suffix_GZ					TEXT(".gz")			//云提交文档的压缩格式的后缀

#define Cloud_Doc_Path_Slash_Windows_Double			TEXT("\\\\")		//Windws文档路径的双反斜杠【\\】
#define Cloud_Doc_Path_Slash_Linux_Double			TEXT("//")			//Linux文档路径的双斜杠【//】
#define Cloud_Doc_Path_Slash_Windows				TEXT("\\")			//Windws文档路径的反斜杠【\】
#define Cloud_Doc_Path_Slash_Linux					TEXT("/")			//Linux文档路径的斜杠【/】
#ifdef WIN32
#define Cloud_Doc_Path_Slash_Current			Cloud_Doc_Path_Slash_Windows	//当前系统要求使用文档路径的斜杠		
#define Cloud_Doc_Path_Slash_Current_Reverse	Cloud_Doc_Path_Slash_Linux		//当前系统要求使用文档路径的反向斜杠		
#else
#define Cloud_Doc_Path_Slash_Current			Cloud_Doc_Path_Slash_Linux		//当前系统要求使用文档路径的斜杠		
#define Cloud_Doc_Path_Slash_Current_Reverse	Cloud_Doc_Path_Slash_Windows	//当前系统要求使用文档路径的反向斜杠		
#endif

#endif //_Cloud_Doc_Def_H_D82B4162_CC81_47e0_A799_EB3B16D3405C_
