#pragma once

/************************************************************************/
/* 
	CGhostScript类可能被多线程调用，所以编译GhostScript类库时，
	要求，启用宏：GS_THREADSAFE，添加GhostScript的多线程支持。
*/
/************************************************************************/

class CGhostScript
{
public:
	CGhostScript(void);
	~CGhostScript(void);

public:
	bool RunGsApiWithArgs(int argc, char **argv);

	//将PostScript文件转换成PDF文件。
	bool PsToPdf(char* pszPsFilePath, char* pszPdfFilePath);

	//将PDF文件转换成PostScript文件。
	bool PdfToPs(char* pszPdfFilePath, char* pszPsFilePath);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取PDF，支持PostScript和PDF输入。
	bool ExtractToPdf(char* pszFilePath, char* pszPdfFilePath, int nFirstPage = 0, int nLastPage = 0);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取PostScript，支持PostScript和PDF输入。
	bool ExtractToPs(char* pszFilePath, char* pszPsFilePath, int nFirstPage = 0, int nLastPage = 0);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取Tiff，支持PostScript和PDF输入。
	bool ExtractToTiff(char* pszFilePath, char* pszTiffFilePath, bool bColor = true, int nDpi = 300, int nFirstPage = 0, int nLastPage = 0);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取PNG，支持PostScript和PDF输入。
	//如果页码范围等于1，则输出文件名可以是固定的，如[output.png]。
	//如果页码范围大于1页，则输出文件名可以使用%d来输出多页，
	//	A:[output_%d.png],生成从output_1.png,output_2.png,output_3.png,...类似编号的文件。
	//	B:[output_%03d.png],生成从output_001.png,output_002.png,output_003.png,...类似编号的文件。
	bool ExtractToPng(char* pszFilePath, char* pszPngFilePath, bool bColor = true, int nDpi = 300, int nFirstPage = 0, int nLastPage = 0);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取JPEG，支持PostScript和PDF输入。
	//如果页码范围等于1，则输出文件名可以是固定的，如[output.jpg]。
	//如果页码范围大于1页，则输出文件名可以使用%d来输出多页，
	//	A:[output_%d.jpg],生成从output_1.jpg,output_2.jpg,output_3.jpg,...类似编号的文件。
	//	B:[output_%03d.jpg],生成从output_001.jpg,output_002.jpg,output_003.jpg,...类似编号的文件。
	bool ExtractToJpeg(char* pszFilePath, char* pszJpegFilePath, bool bColor = true, int nDpi = 300, int nFirstPage = 0, int nLastPage = 0);

	//指定起始和结束页码(从1开始,0表示所有页面)抽取Txt，支持PostScript和PDF输入，
	//但PostScript只支持单页，不支持范围。如：支持1-1，2-2，不支持0-0和1-2。
	bool ExtractToTxt(char* pszFilePath, char* pszTxtFilePath, int nFirstPage = 0, int nLastPage = 0);
};
