#include "StdAfx.h"
#include "IOPrinter.h"
#include "PrinterUtils.h"
#include "..\aCmmLib\IniFile.h"
#include <math.h>
#include <map>

#define File_Read_Length_Unit	(1024*1024)		//�ļ���ȡ���ȵ�Ԫ��1M��С

ColorMatrix colorMatrix = {
	0.299f, 0.299f, 0.299f, 0, 0,
	0.587f, 0.587f, 0.587f, 0, 0,
	0.114f, 0.114f, 0.114f, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1
};

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

#include <AtlBase.h>

class CPrintWithWaterMarked
{
public:
	CPrintWithWaterMarked();
	~CPrintWithWaterMarked();

	Gdiplus::Bitmap* GetMarkedImg();
	BOOL Init(Image* img,JobSecNode* pSec);
	void DrawImg(Image* img);
private:
	Gdiplus::Font* InitFont(WCHAR* pVal,int nFontSize,SizeF& oSize);
	void OnDrawHeader(Graphics* grp);
	void OnDrawFooter(Graphics* grp);
	void OnDrawWatermark(Graphics* grp);
private:
	Bitmap* m_pWaterBmp;
	Graphics* m_pGrp;
	//WCHAR* Copyright;
	Gdiplus::Font* m_pFontHeader;
	Gdiplus::Font* m_pFontFooter;
	Gdiplus::Font* m_pFontWater;
	SizeF m_oHeaderSize;
	SizeF m_oFooterSize;
	SizeF m_oWaterSize;

	int m_nWidth;
	int m_nHeight;
	JobSecNode* m_pSecNode;
	BOOL m_bHasRotate;
};

CPrintWithWaterMarked::CPrintWithWaterMarked()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_pWaterBmp = 0;
	m_pGrp = 0;
	m_pFontWater = 0;
	m_pFontHeader = 0;
	m_pFontFooter = 0;
	m_pSecNode = 0;
	//Copyright = L"Copyright - 2011 �Ű���";
	m_bHasRotate = FALSE;
}
CPrintWithWaterMarked::~CPrintWithWaterMarked()
{
	if (m_pWaterBmp)
	{
		delete m_pWaterBmp;
		m_pWaterBmp = 0;
	}
	if (m_pGrp)
	{
		delete m_pGrp;
		m_pGrp = 0;
	}
	
	if (m_pFontWater)
	{
		delete m_pFontWater;
		m_pFontWater = 0;
	}
	if (m_pFontHeader)
	{
		delete m_pFontHeader;
		m_pFontHeader = 0;
	}
	if (m_pFontFooter)
	{
		delete m_pFontFooter;
		m_pFontFooter = 0;
	}
}
Bitmap* CPrintWithWaterMarked::GetMarkedImg()
{
	return m_pWaterBmp;
}
BOOL CPrintWithWaterMarked::Init(Image* img,JobSecNode* pSec)
{
	ASSERT(img);
	ASSERT(pSec);
	m_pSecNode = pSec;

	m_nWidth = img->GetWidth();
	m_nHeight = img->GetHeight();

	//create a Bitmap the Size of the original photograph
	m_pWaterBmp = new Bitmap(m_nWidth, m_nHeight, PixelFormat32bppARGB);	//PixelFormat24bppRGB��emf�ļ�������£�����ȫ����ɺ�ɫ�ˡ�
	ASSERT(m_pWaterBmp);
	if (!m_pWaterBmp)
	{
		return FALSE;
	}
	m_pWaterBmp->SetResolution(img->GetHorizontalResolution(), img->GetVerticalResolution());
	//load the Bitmap into a Graphics object 
	m_pGrp = Graphics::FromImage(m_pWaterBmp);
	m_pGrp->SetSmoothingMode(SmoothingModeAntiAlias);
	ASSERT(m_pGrp);
	if (!m_pGrp)
	{
		return FALSE;
	}
	return TRUE;
}

void CPrintWithWaterMarked::DrawImg(Image* img)
{
	ASSERT(img);
	Graphics* grp = m_pGrp;
	RectF rf(0, 0, m_nWidth, m_nHeight);

	//Draws the photo Image object at original size to the graphics object.
	grp->DrawImage(
		img,                               // Photo Image object
		rf, // Rectangle structure
		0,                                      // x-coordinate of the portion of the source image to draw. 
		0,                                      // y-coordinate of the portion of the source image to draw. 
		m_nWidth,                                // Width of the portion of the source image to draw. 
		m_nHeight,                               // Height of the portion of the source image to draw. 
		UnitPixel );							// Units of measure 

	//ҳü
	OnDrawHeader(grp);
	//ҳ��
	OnDrawFooter(grp);
	//ˮӡ
	OnDrawWatermark(grp);
}


Gdiplus::Font* CPrintWithWaterMarked::InitFont(WCHAR* pVal,int nFontSize,SizeF& oSize)
{
	Gdiplus::Font* crFont = new Gdiplus::Font(L"arial", nFontSize, FontStyleBold);
	if (crFont)
	{
		PointF origin(0.0f, 0.0f);
		RectF boundRect;
		m_pGrp->MeasureString(pVal,-1, crFont,origin,&boundRect);
		oSize.Height = boundRect.Height;
		oSize.Width = boundRect.Width;
		return crFont;
	}
	return NULL;
}
void CPrintWithWaterMarked::OnDrawHeader(Graphics* grp)
{
	WCHAR* pVal = NULL;
	if (m_pSecNode && m_pSecNode->header.GetLength() > 0)
	{
		USES_CONVERSION;
		pVal = A2W(m_pSecNode->header);
	}
	if (!pVal)
	{
		return;
	}
	if (!m_pFontHeader)
	{
		m_pFontHeader = InitFont(pVal,12,m_oHeaderSize);
	}
	if (!m_pFontHeader)
	{
		return ;
	}
	theLog.Write("printer the header,");
	//Since all photographs will have varying heights, determine a 
	//position 50% from the bottom of the image
	int yPixlesFromBottom = (int)(m_nHeight*.95);

	//Now that we have a point size use the Copyrights string height 
	//to determine a y-coordinate to draw the string of the photograph
	float yPosFromBottom = ((m_nHeight - yPixlesFromBottom)-(m_oHeaderSize.Height/2));

	//Determine its x-coordinate by calculating the center of the width of the image
	float xCenterOfImg = (m_nWidth/2);

	//Define the text layout by setting the text alignment to centered
	StringFormat StrFormat;
	StrFormat.SetAlignment(StringAlignmentCenter);

	//define a Brush which is semi trasparent black (Alpha set to 153)
	Gdiplus::SolidBrush semiTransBrush2(Gdiplus::Color(153, 0, 0, 0));

	PointF pf(xCenterOfImg+1,yPosFromBottom+1);
	//Draw the Copyright string
	grp->DrawString(pVal, -1,                //string of text
		m_pFontHeader,                                   //font
		pf,  //Position
		&StrFormat,
		&semiTransBrush2);

	//define a Brush which is semi trasparent white (Alpha set to 153)
	SolidBrush semiTransBrush(Color(153, 255, 255, 255));

	PointF pf2(xCenterOfImg,yPosFromBottom);
	//Draw the Copyright string a second time to create a shadow effect
	//Make sure to move this text 1 pixel to the right and down 1 pixel
	grp->DrawString(pVal,-1,                 //string of text
		m_pFontHeader,                                   //font
		pf2,										//Position
		&StrFormat,
		&semiTransBrush);					//Brush
}

void CPrintWithWaterMarked::OnDrawFooter(Graphics* grp)
{
	WCHAR* pVal = NULL;
	if (m_pSecNode && m_pSecNode->footer.GetLength() > 0)
	{
		USES_CONVERSION;
		pVal = A2W(m_pSecNode->footer);
	}
	if (!pVal)
	{
		return;
	}
	if (!m_pFontFooter)
	{
		m_pFontFooter = InitFont(pVal,12,m_oFooterSize);
	}
	if (!m_pFontFooter)
	{
		return ;
	}
	theLog.Write("printer the footer,");
	//Since all photographs will have varying heights, determine a 
	//position 50% from the bottom of the image
	int yPixlesFromBottom = (int)(m_nHeight*.05);

	//Now that we have a point size use the Copyrights string height 
	//to determine a y-coordinate to draw the string of the photograph
	float yPosFromBottom = ((m_nHeight - yPixlesFromBottom)-(m_oFooterSize.Height/2));

	//Determine its x-coordinate by calculating the center of the width of the image
	float xCenterOfImg = (m_nWidth/2);

	//Define the text layout by setting the text alignment to centered
	StringFormat StrFormat;
	StrFormat.SetAlignment(StringAlignmentCenter);

	//define a Brush which is semi trasparent black (Alpha set to 153)
	Gdiplus::SolidBrush semiTransBrush2(Gdiplus::Color(153, 0, 0, 0));

	PointF pf(xCenterOfImg+1,yPosFromBottom+1);
	//Draw the Copyright string
	grp->DrawString(pVal, -1,                //string of text
		m_pFontFooter,                                   //font
		pf,  //Position
		&StrFormat,
		&semiTransBrush2);

	//define a Brush which is semi trasparent white (Alpha set to 153)
	SolidBrush semiTransBrush(Color(153, 255, 255, 255));

	PointF pf2(xCenterOfImg,yPosFromBottom);
	//Draw the Copyright string a second time to create a shadow effect
	//Make sure to move this text 1 pixel to the right and down 1 pixel
	grp->DrawString(pVal,-1,                 //string of text
		m_pFontFooter,                                   //font
		pf2,										//Position
		&StrFormat,
		&semiTransBrush);					//Brush
}
void CPrintWithWaterMarked::OnDrawWatermark(Graphics* grp)
{
#if 0
	WCHAR* pVal = NULL;
	if (m_pSecNode && m_pSecNode->water.GetLength() > 0)
	{
		USES_CONVERSION;
		pVal = A2W(m_pSecNode->water);
	}
	if (!pVal)
	{
		return;
	}

	if (!m_pFontWater)
	{
		m_pFontWater = InitFont(pVal,20,m_oWaterSize);
	}
	//Gdiplus::Font* font = InitFont(pVal,16,oSize);
	if (!m_pFontWater)
	{
		return ;
	}
	theLog.Write("printer the water,");
	//Since all photographs will have varying heights, determine a 
	//position 50% from the bottom of the image
	int yPixlesFromBottom = (int)(m_nHeight*.5);

	//Now that we have a point size use the Copyrights string height 
	//to determine a y-coordinate to draw the string of the photograph
	float yPosFromBottom = ((m_nHeight - yPixlesFromBottom)-(m_oWaterSize.Height/2));

	//Determine its x-coordinate by calculating the center of the width of the image
	float xCenterOfImg = (m_nWidth/2);

	//Define the text layout by setting the text alignment to centered
	StringFormat StrFormat;
	StrFormat.SetAlignment(StringAlignmentCenter);

	//define a Brush which is semi trasparent black (Alpha set to 153)
	Gdiplus::SolidBrush semiTransBrush2(Gdiplus::Color(153, 0, 0, 0));

	PointF pf(xCenterOfImg+1,yPosFromBottom+1);
	//Draw the Copyright string
	grp->DrawString(pVal, -1,                //string of text
		m_pFontWater,                                   //font
		pf,  //Position
		&StrFormat,
		&semiTransBrush2);

	//define a Brush which is semi trasparent white (Alpha set to 153)
	SolidBrush semiTransBrush(Color(153, 255, 255, 255));

	PointF pf2(xCenterOfImg,yPosFromBottom);
	//Draw the Copyright string a second time to create a shadow effect
	//Make sure to move this text 1 pixel to the right and down 1 pixel
	grp->DrawString(pVal,-1,                 //string of text
		m_pFontWater,                                   //font
		pf2,										//Position
		&StrFormat,
		&semiTransBrush);					//Brush

#else
	int m_nFontSize = 80;
	int m_nAlpha	= 30;

	WCHAR* pVal = NULL;
	if (m_pSecNode && m_pSecNode->water.GetLength() > 0)
	{
		USES_CONVERSION;
		pVal = A2W(m_pSecNode->water);
	}
	if (!pVal)
	{
		return;
	}

	if (!m_pFontWater)
	{
		m_pFontWater = InitFont(pVal,m_nFontSize,m_oWaterSize);
	}
	//Gdiplus::Font* font = InitFont(pVal,16,oSize);
	if (!m_pFontWater)
	{
		return ;
	}

	//��б45�Ƚ�����
	if (!m_bHasRotate)
	{
		double pi = 3.1415926;
		grp->RotateTransform(-atan(m_nHeight/m_nWidth*1.0)/pi*180);
		grp->TranslateTransform(0, 0);
		m_bHasRotate = TRUE;
	}

	int nPageSize = sqrt((m_nHeight * m_nHeight) + (m_nWidth * m_nWidth)*1.0);
	int nCountHeight = nPageSize / (m_oWaterSize.Height);
	int nCountWeight = nPageSize / (m_oWaterSize.Width);
	if (nCountWeight%2 != 0)
	{
		nCountWeight = nCountWeight + 1;
	}

	for (int height=0; height<=nCountHeight; height++)
	{
		for (int weight=0; weight<=nCountWeight; weight++)
		{
			//Now that we have a point size use the Copyrights string height 
			//to determine a y-coordinate to draw the string of the photograph
			float yPosFromBottom = m_oWaterSize.Height * height;

			//Determine its x-coordinate by calculating the center of the width of the image
			float xCenterOfImg = (weight - nCountWeight/2) * m_oWaterSize.Width;

			//Define the text layout by setting the text alignment to centered
			StringFormat StrFormat;
			StrFormat.SetAlignment(StringAlignmentCenter);

			//define a Brush which is semi trasparent black (Alpha set to 153)
			Gdiplus::SolidBrush semiTransBrush2(Gdiplus::Color(m_nAlpha, 0, 0, 0));

			PointF pf(xCenterOfImg+1,yPosFromBottom+1);
			//Draw the Copyright string
			grp->DrawString(pVal, -1,                //string of text
				m_pFontWater,                                   //font
				pf,  //Position
				&StrFormat,
				&semiTransBrush2);
			//define a Brush which is semi trasparent white (Alpha set to 153)
			SolidBrush semiTransBrush(Color(m_nAlpha, 255, 255, 255));

			PointF pf2(xCenterOfImg,yPosFromBottom);
			//Draw the Copyright string a second time to create a shadow effect
			//Make sure to move this text 1 pixel to the right and down 1 pixel
			grp->DrawString(pVal,-1,                 //string of text
				m_pFontWater,                                   //font
				pf2,										//Position
				&StrFormat,
				&semiTransBrush);					//Brush
		}
	}
#endif
}


//-------------------------------------------
CString GetDefaultPath()
{
	TCHAR path[MAX_PATH];
	CString sDefaultPath;
	if(GetModuleFileName(0,path,MAX_PATH)==0) 
		return _T("");
	TCHAR *pdest;
	pdest=_tcsrchr(path,_T('\\'));
	if(pdest) 
		*pdest=0;
	sDefaultPath=path;
	sDefaultPath+=_T("\\");
	return sDefaultPath;
}

//��ӡ��ֽ����Ϣ
struct CPrinterPaperInfo
{
	CPrinterPaperInfo()
	{
		m_szPrinterName.clear();
		m_pwdPapers = NULL;
		m_pptPaperSize = NULL;
		m_pcPaperName = NULL;
		m_nPaperCount = 0;
		m_wdDefaultPaperSize = DMPAPER_A4;
	}
	~CPrinterPaperInfo()
	{
		m_szPrinterName.clear();
		if (m_pwdPapers)
		{
			delete[] m_pwdPapers;
			m_pwdPapers = NULL;
		}

		if (m_pptPaperSize)
		{
			delete[] m_pptPaperSize;
			m_pptPaperSize = NULL;
		}

		if (m_pcPaperName)
		{
			delete[] m_pcPaperName;
			m_pcPaperName = NULL;
		}

		m_nPaperCount = 0;
		m_wdDefaultPaperSize = DMPAPER_A4;
	}

	string m_szPrinterName;
	WORD* m_pwdPapers;
	POINT* m_pptPaperSize;
	char* m_pcPaperName;
	int m_nPaperCount;
	WORD m_wdDefaultPaperSize;
};

typedef map<string, CPrinterPaperInfo*> CPrinterPaperInfoMap;

//��ȡֽ����Ϣ������
#define CPrinterPaperSize_Support_PaperName	
class CPrinterPaperSize
{
protected:
	CPrinterPaperSize()
	{
		Release();
	}

public:
	static CPrinterPaperSize& GetInstance()
	{	
		static CPrinterPaperSize one;	
		return one;	
	}

	~CPrinterPaperSize()
	{
		Release();
	}

	void Release()
	{
		CCriticalSection2::Owner lock(m_cs4PrinterPaperInfoMap);

		CPrinterPaperInfoMap::iterator it;
		for (it=m_oPrinterPaperInfoMap.begin(); it!=m_oPrinterPaperInfoMap.end(); it++)
		{
			CPrinterPaperInfo* pPrinterPaperInfo = it->second;
			if (pPrinterPaperInfo)
			{
				delete pPrinterPaperInfo;
			}
		}
		m_oPrinterPaperInfoMap.clear();
	}

	void SetPrinterName(string szPrinterName)
	{
		CCriticalSection2::Owner lock(m_cs4PrinterPaperInfoMap);

		CPrinterPaperInfo* pPrinterPaperInfo = GetPrinterPaperInfo(szPrinterName);
		if (!pPrinterPaperInfo)
		{
			pPrinterPaperInfo = new CPrinterPaperInfo;
			pPrinterPaperInfo->m_szPrinterName = szPrinterName;
			pPrinterPaperInfo->m_wdDefaultPaperSize = GetPrinterDefaultPaperSize(szPrinterName);
			GetPrinterPaper(szPrinterName, pPrinterPaperInfo);
			m_oPrinterPaperInfoMap.insert(pair<string,CPrinterPaperInfo*>(szPrinterName, pPrinterPaperInfo));
		}
	}

	BOOL GetPaperInfoByPageSize(string szPrinterName, WORD wWidthMMT, WORD wHeightMMT, WORD& wdPaperSize, WORD& wdOrientation)
	{
		CPrinterPaperInfo* pPrinterPaperInfo = GetPrinterPaperInfo(szPrinterName);
		if (!pPrinterPaperInfo)
		{
			theLog.Write("!!GetPaperInfoByPageSize,1,fail,szPrinterName=%s,pPrinterPaperInfo=%p"
				, szPrinterName.c_str(), pPrinterPaperInfo);
			return FALSE;
		}

		WORD* pwdPapers = pPrinterPaperInfo->m_pwdPapers;
		POINT* pptPaperSize = pPrinterPaperInfo->m_pptPaperSize;
		char* pcPaperName = pPrinterPaperInfo->m_pcPaperName;
		int nPaperCount = pPrinterPaperInfo->m_nPaperCount;
		WORD wdDefaultPaperSize = pPrinterPaperInfo->m_wdDefaultPaperSize;

		if (nPaperCount<=0 || !pptPaperSize)
		{
			theLog.Write("!!GetPaperInfoByPageSize,2,fail,szPrinterName=%s,nPaperCount=%d,pptPaperSize=%p"
				, szPrinterName.c_str(), nPaperCount, pptPaperSize);
			return FALSE;
		}

		wdOrientation = (wWidthMMT > wHeightMMT) + 1;	//��ӡ����

		int nWidthMMT = wWidthMMT/* + 100*/;
		int nHeightMMT = wHeightMMT/* + 100*/;
		if (nWidthMMT > nHeightMMT)
		{
			int nTemp = nWidthMMT;
			nWidthMMT = nHeightMMT;
			nHeightMMT = nTemp;
		}

		int nDefaultPaperSizeIndex = -1;	//Ĭ��ֽ�͵���������
		int nPaperSizeIndex = -1;			//ƥ��ֽ�͵���������
		int nPaperDiffFactor = 0x7FFFFFFF;	//ֽ���������

		for (int index=0; index<nPaperCount; index++)
		{
			if (pwdPapers[index] == wdDefaultPaperSize)
			{
				nDefaultPaperSizeIndex = index;
			}
			int nWidthDif = pptPaperSize[index].x-nWidthMMT;
			int nHeigthDif = pptPaperSize[index].y-nHeightMMT;
			int nTemp = (nWidthDif * nWidthDif) + (nHeigthDif * nHeigthDif);	
			if (nTemp < nPaperDiffFactor)//ȡ��ƽ����С��ƥ��ֽ��
			{
				nPaperDiffFactor = nTemp;
				nPaperSizeIndex = index;
			}
		}

		if (nPaperSizeIndex >= 0)
		{
			wdPaperSize = pwdPapers[nPaperSizeIndex];	//��ӡֽ��
		}

		if (nPaperDiffFactor > 10000)
		{
			theLog.Write("########CPrinterPaperSize, nPaperDiffFactor(%d), Use DefaultPaperSize(%d)", nPaperDiffFactor, wdDefaultPaperSize);
			wdPaperSize = wdDefaultPaperSize;	//ƥ�����̫��,ʹ��Ĭ�ϴ�ӡֽ��
			nPaperSizeIndex = nDefaultPaperSizeIndex;
		}

#ifdef CPrinterPaperSize_Support_PaperName
		char *pPaperName = pcPaperName + (nPaperSizeIndex*64);
		theLog.Write("CPrinterPaperSize,wdPaperSize=%d,wdOrientation=%d,PaperName=[%s]", wdPaperSize, wdOrientation, pPaperName);
#endif
		return TRUE;
	}

	BOOL GetPaperInfoByEmfHeader(string szPrinterName, PENHMETAHEADER pEmfHeader, WORD& wdPaperSize, WORD& wdOrientation)
	{
		if (!pEmfHeader)
		{
			theLog.Write("!!GetPaperSizeByEmfHeader,1,fail,pEmfHeader=%p", pEmfHeader);
			return FALSE;
		}

		int nEmfWidthMM = pEmfHeader->szlMillimeters.cx;
		int nEmfHeightMM = pEmfHeader->szlMillimeters.cy;
// 		int nEmfWidthMMT = 2 * (5 * nEmfWidthMM + 50);	//ԭʼ�㷨
// 		int nEmfHeightMMT = 2 * (5 * nEmfHeightMM + 50);
		int nEmfWidthMMT = nEmfWidthMM * 10 + 100;
		int nEmfHeightMMT = nEmfHeightMM * 10 + 100;
		return GetPaperInfoByPageSize(szPrinterName, nEmfHeightMMT, nEmfHeightMMT, wdPaperSize, wdOrientation);
	}

	BOOL GetPaperInfoByImage(string szPrinterName, Image& image, WORD& wdPaperSize, WORD& wdOrientation)
	{
		int nImageWidthMM = 0;
		int nImageHeightMM = 0;
		GetImageDeviceSize(image, nImageWidthMM, nImageHeightMM);
// 		int nImageWidthMMT = 2 * (5 * nImageWidthMM + 50);	//ԭʼ�㷨
// 		int nImageHeightMMT = 2 * (5 * nImageHeightMM + 50);
		int nImageWidthMMT = nImageWidthMM * 10 + 100;
		int nImageHeightMMT = nImageHeightMM * 10 + 100;
		return GetPaperInfoByPageSize(szPrinterName, nImageHeightMMT, nImageHeightMMT, wdPaperSize, wdOrientation);
	}

protected:
	int GetPrinterPaper(string szPrinterName, CPrinterPaperInfo* pPrinterPaperInfo)
	{
		if (!pPrinterPaperInfo)
		{
			return 0;
		}

		pPrinterPaperInfo->m_nPaperCount = 0;
		do 
		{
			if (szPrinterName.empty())
			{
				theLog.Write("!!GetPrinterPaper,1,GetPrinterPaperSize fail,szPrinterName.IsEmpty()");
				break;
			}

			string szPort = GetPrinterPort(szPrinterName);
			if (szPort.empty())
			{
				theLog.Write("!!GetPrinterPaper,2,GetPrinterPaperSize fail,szPort.IsEmpty()");
				break;
			}

			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERS, NULL, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,3,GetPrinterPaperSize DeviceCapabilities DC_PAPERS fail,m_nPaperCount=%d,err=%u", pPrinterPaperInfo->m_nPaperCount, GetLastError());
				break;
			}
			theLog.Write("GetPrinterPaper,3.2,GetPrinterPaperSize DeviceCapabilities DC_PAPERS succ,m_nPaperCount=%d", pPrinterPaperInfo->m_nPaperCount);

			pPrinterPaperInfo->m_pwdPapers = new WORD[pPrinterPaperInfo->m_nPaperCount];	//array of WORD
			memset(pPrinterPaperInfo->m_pwdPapers, 0x0, sizeof(WORD)*pPrinterPaperInfo->m_nPaperCount);
			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERS, (LPSTR)pPrinterPaperInfo->m_pwdPapers, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,4,GetPrinterPaperSize DeviceCapabilities DC_PAPERS fail,m_nPaperCount=%d,err=%u",pPrinterPaperInfo-> m_nPaperCount, GetLastError());
				break;
			}

			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERSIZE, NULL, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,5,GetPrinterPaperSize DeviceCapabilities DC_PAPERSIZE fail,m_nPaperCount=%d,err=%u", pPrinterPaperInfo->m_nPaperCount, GetLastError());
				break;
			}
			theLog.Write("GetPrinterPaper,5.2,GetPrinterPaperSize DeviceCapabilities DC_PAPERSIZE succ,m_nPaperCount=%d", pPrinterPaperInfo->m_nPaperCount);

			pPrinterPaperInfo->m_pptPaperSize = new POINT[pPrinterPaperInfo->m_nPaperCount];	//array of POINT structures
			memset(pPrinterPaperInfo->m_pptPaperSize, 0x0, sizeof(POINT)*pPrinterPaperInfo->m_nPaperCount);
			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERSIZE, (LPSTR)pPrinterPaperInfo->m_pptPaperSize, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,6,GetPrinterPaperSize DeviceCapabilities DC_PAPERSIZE fail,m_nPaperCount=%d,err=%u", pPrinterPaperInfo->m_nPaperCount, GetLastError());
				break;
			}

#ifdef CPrinterPaperSize_Support_PaperName
			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERNAMES, NULL, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,7,GetPrinterPaperSize DeviceCapabilities DC_PAPERNAMES fail,m_nPaperCount=%d,err=%u", pPrinterPaperInfo->m_nPaperCount, GetLastError());
				break;
			}
			theLog.Write("GetPrinterPaper,7.2,GetPrinterPaperSize DeviceCapabilities DC_PAPERNAMES succ,m_nPaperCount=%d", pPrinterPaperInfo->m_nPaperCount);

			pPrinterPaperInfo->m_pcPaperName = new char[pPrinterPaperInfo->m_nPaperCount*64];	//Each string buffer is 64 characters long and contains the name of a paper form.
			memset(pPrinterPaperInfo->m_pcPaperName, 0x0, pPrinterPaperInfo->m_nPaperCount*64);
			pPrinterPaperInfo->m_nPaperCount = DeviceCapabilities(szPrinterName.c_str(),szPort.c_str(), DC_PAPERNAMES, (LPSTR)pPrinterPaperInfo->m_pcPaperName, NULL);
			if (pPrinterPaperInfo->m_nPaperCount<=0)
			{
				theLog.Write("!!GetPrinterPaper,8,GetPrinterPaperSize DeviceCapabilities DC_PAPERNAMES fail,m_nPaperCount=%d,err=%u", pPrinterPaperInfo->m_nPaperCount, GetLastError());
				break;
			}

			for (int i=0; i<pPrinterPaperInfo->m_nPaperCount; i++)
			{
				theLog.Write("PaperSize,i=%d\t%d\t%s\t(%d, %d)", i+1, pPrinterPaperInfo->m_pwdPapers[i], pPrinterPaperInfo->m_pcPaperName+(i*64), pPrinterPaperInfo->m_pptPaperSize[i].x, pPrinterPaperInfo->m_pptPaperSize[i].y);
			}

#endif
		} while (FALSE);

		theLog.Write("GetPrinterPaper,9,m_nPaperCount=%d", pPrinterPaperInfo->m_nPaperCount);
		return pPrinterPaperInfo->m_nPaperCount;
	}

	string GetPrinterPort(string szPrnterName)
	{
		string szPort;
		HANDLE h = 0;
		PRINTER_DEFAULTS pd;
		pd.pDatatype     = NULL;
		pd.pDevMode      = NULL;
		pd.DesiredAccess = PRINTER_ALL_ACCESS;
		char cPrinter[MAX_PATH] = {0};
		strcpy(cPrinter, szPrnterName.c_str());
		if(OpenPrinter(cPrinter,&h,&pd) && h)
		{
			DWORD dwSize = 0;
			GetPrinter(h,2,0,0,&dwSize);
			//theLog.Write("GetPrinterStatus %d",dwSize);
			if (dwSize > 0)
			{
				BYTE* pData = new BYTE[dwSize];
				if(GetPrinter(h,2,pData,dwSize,&dwSize))
				{
					PRINTER_STATUS_BUSY;
					PRINTER_INFO_2* pInfo = (PRINTER_INFO_2*)pData;
					szPort = pInfo->pPortName;
				}
				else
				{
					theLog.Write("!!GetPrinterPort,GetPrinter,3, err %d",::GetLastError());				
				}
				delete[] pData;
			}
			else
			{
				theLog.Write("!!GetPrinterPort,GetPrinter,2, err %d",::GetLastError());				
			}
			ClosePrinter(h);
		}
		else
		{
			theLog.Write("!!GetPrinterPort,OpenPrinter,1, err %d",::GetLastError());				
		}

		return szPort;
	}

	WORD GetPrinterDefaultPaperSize(string szPrnterName)
	{
		WORD wdPaperSize = DMPAPER_A4;
		if (!szPrnterName.empty())
		{
			TCHAR tcPrinterName[128] = {0};
			strcpy_s(tcPrinterName, 128, szPrnterName.c_str());
			HANDLE hPrinter = NULL;
			if (OpenPrinter(tcPrinterName, &hPrinter, NULL))
			{
				LONG lRet = DocumentProperties(NULL, hPrinter, tcPrinterName, NULL, NULL, 0);
				if (lRet > 0)
				{
					BYTE* pData = new BYTE[lRet];
					memset(pData, 0x0, lRet);
					lRet = DocumentProperties(NULL, hPrinter, tcPrinterName, (PDEVMODE)pData, NULL, DM_OUT_BUFFER);
					if (lRet == IDOK)
					{
						DEVMODE* pDev = (DEVMODE*)pData;
						if (pDev->dmFields & DM_PAPERSIZE)
						{
							wdPaperSize = pDev->dmPaperSize;
						}
						else
						{
							theLog.Write("!!GetPrinterDefaultPaperSize,Not Support DM_PAPERSIZE,dmFields=%d", pDev->dmFields);
						}
					}
					else
					{
						theLog.Write("!!GetPrinterDefaultPaperSize,DocumentProperties,BB fail,err=%u,lRet=%d", GetLastError(), lRet);
					}
					delete[] pData;
				}
				else
				{
					theLog.Write("!!GetPrinterDefaultPaperSize,DocumentProperties,AA fail,err=%u,lRet=%d", GetLastError(), lRet);
				}
				ClosePrinter(hPrinter);
			}
			else
			{
				theLog.Write("!!GetPrinterDefaultPaperSize,OpenPrinter fail,err=%u", GetLastError());
			}
		}
		else
		{
			theLog.Write("!!GetPrinterDefaultPaperSize,szPrnterName.IsEmpty()");
		}
		theLog.Write("GetPrinterDefaultPaperSize,wdPaperSize=%d", wdPaperSize);
		return wdPaperSize;
	}

	//ͨ��ͼƬ��dpi(600/300)�ʹ�С(pixel)����ȡ��ӡͼƬ���豸��С(mm)
	void GetImageDeviceSize(Image& image, int& nDeviceWidthMM, int& nDeviceHeightMM)
	{
		int nImageWidth = image.GetWidth();
		int nImageHeight = image.GetHeight();
		float fImageWidthDpi = image.GetHorizontalResolution();
		float fImageHeightDpi = image.GetVerticalResolution();
		int nDeviceWidthMMT = nImageWidth / fImageWidthDpi * 25.4 * 10;	//1 in = 25.4 mm,�����Ǻ��׵�10��
		int nDeviceHeightMMT = nImageHeight / fImageHeightDpi * 25.4 * 10;	
		nDeviceWidthMM = MulDiv(nDeviceWidthMMT, 1, 10);	//�������룬��MulDiv(2564, 1, 10) == 256
		nDeviceHeightMM = MulDiv(nDeviceHeightMMT, 1, 10);	//�������룬��MulDiv(2566, 1, 10) == 257
		theLog.Write("GetImageDeviceSize,nDeviceWidthMM=%d,nDeviceHeightMM=%d", nDeviceWidthMM, nDeviceHeightMM);
	}

	CPrinterPaperInfo* GetPrinterPaperInfo(string szPrnterName)
	{
		CCriticalSection2::Owner lock(m_cs4PrinterPaperInfoMap);

		CPrinterPaperInfo* pPrinterPaperInfo = NULL;
		CPrinterPaperInfoMap::iterator it = m_oPrinterPaperInfoMap.find(szPrnterName);
		if (it != m_oPrinterPaperInfoMap.end())
		{
			pPrinterPaperInfo = it->second;
		}
		return pPrinterPaperInfo;
	}

private:
	CPrinterPaperInfoMap m_oPrinterPaperInfoMap;
	CCriticalSection2 m_cs4PrinterPaperInfoMap;
};

CIOPrinter::CIOPrinter(void)
{
	m_dwColor = DMCOLOR_MONOCHROME;
	m_dwDuplex = DMDUP_SIMPLEX;
	m_dwOrientation = DMORIENT_PORTRAIT;
	m_dwPaperSize = DMPAPER_A4;
	m_dwPaperLength = 0;
	m_dwPaperWidth = 0;
	m_dwScale = 100;
	m_dwCopies = 1;
	m_dwCollate = DMCOLLATE_TRUE;
	m_dwDefaultSource = DMBIN_AUTO;
	m_dwPrintQuality = DMRES_HIGH;
	m_dwPagesPerPaper = 1;
	m_bAutoPaperSize = FALSE;
	m_nPrinted = 0;
	m_eType = BT_THIN_CLT;
	m_nCurrentPage = 0;
	EnableDefaultSource(FALSE);

	m_bUseA4Print = g_oIniFile.GetVal("PrtCtrlConfig", "UseA4Print", FALSE);//add by zxl,20150113,��A3/A4�Ĵ�ӡ����ת����A4ֽ�ʹ�ӡ
	m_bForceMonochromePrint = g_oIniFile.GetVal("PrtCtrlConfig", "ForceHbPrint", TRUE);	//add by zxl,20150728,��һЩ��ӡ���������޷��ڴ�ӡʱ����ɫ��ģʽ������ǿ��ʹ�úڰ�ת��
	m_bForceXpsDriverPrint = g_oIniFile.GetVal("PrtCtrlConfig", "ForceXpsDriverPrint", FALSE);	//add by zxl, 20151014,��һЩ��ӡ���������޷��ڴ�ӡXPS�ļ�������ǿ��ʹ���µĴ�ӡ������(XPS����)
	theLog.Write("CIOPrinter::CIOPrinter, m_bUseA4Print=%d, m_bForceMonochromePrint=%d, m_bForceXpsDriverPrint=%d"
		, m_bUseA4Print, m_bForceMonochromePrint, m_bForceXpsDriverPrint);
}

CIOPrinter::~CIOPrinter(void)
{
}

static CString GetTempName(CString strPath,CString sName)
{
	const int nMin = 0;
	const int nMax = 999;
	//const CString strBase(_T("TestPrint"));
	static int nLastKnown = nMin;
	strPath.TrimRight("\\");

	sName.Replace('*',' ');
	sName.Replace('%',' ');
	bool bFound = false;
	CString strPathName;
	while (!bFound) {
		if (nLastKnown > nMax)
			break;
		strPathName = strPath;
		strPathName.AppendFormat(_T("\\%s_%03.3d.xps"), sName,nLastKnown++);
		HANDLE hFile = ::CreateFile(strPathName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			::CloseHandle(hFile);
			bFound = TRUE;
		}
	}
	if (!bFound)
		strPathName.Empty();
	return(strPathName);
}
//��ȡĬ�ϴ�ӡ��
CString CIOPrinter::GetDefault()
{
	CString sDefault;
	char szBuf[1024] = {0};
	DWORD dwSize = 1024;
	GetDefaultPrinter(szBuf,&dwSize);
	sDefault = szBuf;
	return sDefault;
}

void CIOPrinter::EnableColor(BOOL b)
{
	if (b)
	{
		m_dwColor = DMCOLOR_COLOR;
	}
	else
	{
		m_dwColor = DMCOLOR_MONOCHROME;
	}
	theLog.Write("CIOPrinter::EnableColor,m_dwColor=%d", m_dwColor);
}

void CIOPrinter::SetDuplex(WORD dw)
{
	DMDUP_SIMPLEX;
	DMDUP_VERTICAL;
	DMDUP_HORIZONTAL;
	m_dwDuplex = dw;
	theLog.Write("CIOPrinter::SetDuplex,m_dwDuplex=%d", m_dwDuplex);
}

void CIOPrinter::SetOrientation(WORD dw)
{
	DMORIENT_PORTRAIT;
	m_dwOrientation = dw;
	theLog.Write("CIOPrinter::SetOrientation,m_dwOrientation=%d", m_dwOrientation);
}

void CIOPrinter::SetPaperLength(WORD dw)
{
	if (dw>=10)	//����̫С�������ӡ��ʧ��
	{
		m_dwPaperLength = dw;
	}
	theLog.Write("CIOPrinter::SetPaperLength,m_dwPaperLength=%d,dw=%d", m_dwPaperLength, dw);
}

void CIOPrinter::SetPaperWidth(WORD dw)
{
	if (dw>=10)	//����̫С�������ӡ��ʧ��
	{
		m_dwPaperWidth = dw;
	}
	theLog.Write("CIOPrinter::SetPaperWidth,m_dwPaperWidth=%d,dw=%d", m_dwPaperWidth, dw);
}

void CIOPrinter::SetScale(WORD dw)
{
	m_dwScale = dw;
	theLog.Write("CIOPrinter::SetScale,m_dwScale=%d", m_dwScale);
}

void CIOPrinter::SetCopies(WORD dw)
{
	if (dw<1 || dw>9999)
	{
		m_dwCopies = 1;
	}
	else
	{
		m_dwCopies = dw;
	}
	theLog.Write("CIOPrinter::SetCopies,m_dwCopies=%d", m_dwCopies);
}

void CIOPrinter::EnableCollate(BOOL b)
{
	if (b)
	{
		m_dwCollate = DMCOLLATE_TRUE;
	}
	else
	{
		m_dwCollate = DMCOLLATE_FALSE;
	}
	theLog.Write("CIOPrinter::EnableCollate,m_dwCollate=%d", m_dwCollate);
}

void CIOPrinter::EnableDefaultSource(BOOL bEnable)
{
	m_bEnableDefaultSource = bEnable;
	theLog.Write("CIOPrinter::EnableDefaultSource,m_bEnableDefaultSource=%d", m_bEnableDefaultSource);
}

void CIOPrinter::EnableAutoPaperSize(BOOL bEnable)
{
	m_bAutoPaperSize = bEnable;
	theLog.Write("CIOPrinter::EnableAutoPaperSize,m_bAutoPaperSize=%d", m_bAutoPaperSize);
}

BOOL CIOPrinter::IsEnableDefaultSource()
{
	return m_bEnableDefaultSource;
}


//���ô�ӡ��ֽ����Դ
/* bin selections */
// #define DMBIN_FIRST         DMBIN_UPPER		//ֽ��һ
// #define DMBIN_UPPER         1				//ֽ��һ
// #define DMBIN_ONLYONE       1				//ֽ��һ
// #define DMBIN_LOWER         2				//ֽ�ж�
// #define DMBIN_MIDDLE        3				//ֽ����
// #define DMBIN_MANUAL        4				//�ֶ���ֽ��
// #define DMBIN_ENVELOPE      5				//�ŷ��ֽ��
// #define DMBIN_ENVMANUAL     6				//�ֶ��ŷ�
// #define DMBIN_AUTO          7				//��ӡ������/�Զ�
// #define DMBIN_TRACTOR       8
// #define DMBIN_SMALLFMT      9
// #define DMBIN_LARGEFMT      10
// #define DMBIN_LARGECAPACITY 11				//������ֽ��
// #define DMBIN_CASSETTE      14				//�๦�ܽ�ֽ��
// #define DMBIN_FORMSOURCE    15				//�Զ�
// #define DMBIN_LAST          DMBIN_FORMSOURCE
// #define DMBIN_USER          256     /* device specific bins start here */


/*ʩ��ֽѡ��в���
15	�Զ�
7	�Զ�ѡ��
1	ֽ��1
3	ֽ��2
2	ֽ��3
257	ֽ��4
261	**ֽ��3[������] 
262	**ֽ��4[������] 
4	ֽ��5(����ֽ��)
*/
void CIOPrinter::SetSLPrinterSource(Paper_Source PaperSource)	//ʩ��
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(1);
		break;
	case Paper_Source_2:
		SetDefaultSource(3);
		break;
	case Paper_Source_3:
		SetDefaultSource(2);
		break;
	case Paper_Source_4:
		SetDefaultSource(257);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(261);
		break;
	case Paper_Source_LARGEFMT:
		SetDefaultSource(262);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetSLPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*��ֽ֥ѡ��в���
7	�Զ�
1	ֽ�� 1
3	ֽ�� 2
2	ֽ�� 3
257	ֽ�� 4
258	��������ֽ��
11	��չ��������ֽ��
4	��·����
259	��ҳ����Ԫ
*/
void CIOPrinter::SetDZPrinterSource(Paper_Source PaperSource)	//��֥
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(1);
		break;
	case Paper_Source_2:
		SetDefaultSource(3);
		break;
	case Paper_Source_3:
		SetDefaultSource(2);
		break;
	case Paper_Source_4:
		SetDefaultSource(257);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(11);
		break;
	case Paper_Source_LARGEFMT:
		SetDefaultSource(258);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetDZPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
7	�Զ�
257	ֽ��1
258	ֽ��2
259	ֽ��3
260	ֽ��4
1000	LCT[������ֽ��]
4	����ֽ��
*/
void CIOPrinter::SetKMPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(257);
		break;
	case Paper_Source_2:
		SetDefaultSource(258);
		break;
	case Paper_Source_3:
		SetDefaultSource(259);
		break;
	case Paper_Source_4:
		SetDefaultSource(260);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(1000);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetKMPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
15	�Զ�ѡ��
7	ʹ�ô�ӡ������
1	��ֽϻ1
2	��ֽϻ2
3	��ֽϻ3
257	��ֽϻ4
258	��ֽϻ5
14	�๦�ܽ�ֽ��
4	�ֶ�ֽ��
6	�ֶ��ŷ�
5	�ŷ��ֽ��
*/
void CIOPrinter::SetLMPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(15);
		break;
	case Paper_Source_1:
		SetDefaultSource(1);
		break;
	case Paper_Source_2:
		SetDefaultSource(2);
		break;
	case Paper_Source_3:
		SetDefaultSource(3);
		break;
	case Paper_Source_4:
		SetDefaultSource(257);
		break;
	case Paper_Source_5:
		SetDefaultSource(258);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_ENVMANUAL:
		SetDefaultSource(6);
		break;
	case Paper_Source_ENVELOPE:
		SetDefaultSource(5);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetLMPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*���ֽѡ��в���
7	�Զ�ѡ��ֽ��
1	ֽ��1
3	ֽ��2
2	ֽ��3
4	����̨
11	������ֽ��
*/
void CIOPrinter::SetLGPrinterSource(Paper_Source PaperSource)	//���
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(1);
		break;
	case Paper_Source_2:
		SetDefaultSource(3);
		break;
	case Paper_Source_3:
		SetDefaultSource(2);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(11);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetLGPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
7	�Զ���Դѡ��
14	ֽ�� 1
4	����;����
*/
void CIOPrinter::SetJCPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(14);
		break;
//��Ϣ��ȫ,��ʱֻ֪����һ��ֽ��
// 	case Paper_Source_2:
// 		SetDefaultSource(3);
// 		break;
// 	case Paper_Source_3:
// 		SetDefaultSource(2);
// 		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
// 	case Paper_Source_LARGECAPACITY:
// 		SetDefaultSource(11);
// 		break;
	default:
		theLog.Write("!!CIOPrinter::SetJCPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*��ֽѡ��в���
7	�Զ�
257	ֽ��1
258	ֽ��2
259	ֽ��3
260	ֽ��4
1000	LCT[������ֽ��]
4	����ֽ��
*/
void CIOPrinter::SetZDPrinterSource(Paper_Source PaperSource)	//��	- �������һ����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(257);
		break;
	case Paper_Source_2:
		SetDefaultSource(258);
		break;
	case Paper_Source_3:
		SetDefaultSource(259);
		break;
	case Paper_Source_4:
		SetDefaultSource(260);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(4);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(1000);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetZDPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
7	�Զ�ѡ��
261	����ֽ��
257	ֽ��1
258	ֽ��2
259	ֽ��3
260	ֽ��4
263	ֽ��5[������ֽ��]
*/
void CIOPrinter::SetXPPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(257);
		break;
	case Paper_Source_2:
		SetDefaultSource(258);
		break;
	case Paper_Source_3:
		SetDefaultSource(259);
		break;
	case Paper_Source_4:
		SetDefaultSource(260);
		break;
// 	case Paper_Source_5:
// 		SetDefaultSource(263);
// 		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(261);
		break;
	case Paper_Source_LARGECAPACITY:
		SetDefaultSource(263);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetXPPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
7	�Զ�ѡ��
6	����;ֽ��
1	ֽ�� 1
2	ֽ�� 2
257	ֽ�� 3
258	ֽ�� 4
*/
void CIOPrinter::SetSXPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(7);
		break;
	case Paper_Source_1:
		SetDefaultSource(1);
		break;
	case Paper_Source_2:
		SetDefaultSource(2);
		break;
	case Paper_Source_3:
		SetDefaultSource(257);
		break;
	case Paper_Source_4:
		SetDefaultSource(258);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(6);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetSXPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

/*����ֽѡ��в���
15	 �Զ�ѡ��
257	 ��ӡ���Զ�ѡ��
258	 �ֶ���ֽ
259	 ֽ�� 1
260	 ֽ�� 2
261	 ֽ�� 3
262	 ֽ�� 4
512	 �ŷ�������
*/
void CIOPrinter::SetHPPrinterSource(Paper_Source PaperSource)	//����
{
	switch (PaperSource)
	{
	case Paper_Source_AUTO:
		SetDefaultSource(15);
		break;
	case Paper_Source_1:
		SetDefaultSource(259);
		break;
	case Paper_Source_2:
		SetDefaultSource(260);
		break;
	case Paper_Source_3:
		SetDefaultSource(261);
		break;
	case Paper_Source_4:
		SetDefaultSource(262);
		break;
	case Paper_Source_MANUAL:
		SetDefaultSource(258);
		break;
	case Paper_Source_ENVELOPE:
		SetDefaultSource(512);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetHPPrinterSource, UnKnow Soruce=%d", PaperSource);
		break;
	}
}

void CIOPrinter::SetDefaultSourceByBrand(BRAND_TYPE eType, Paper_Source PaperSource)
{
	m_eType = eType;
	switch (m_eType)
	{
	case BT_SL:
	case BT_SL2:
		SetSLPrinterSource(PaperSource);
		break;
	case BT_DZ:
		SetDZPrinterSource(PaperSource);
		break;
	case BT_KM:
		SetKMPrinterSource(PaperSource);
		break;
	case BT_LM:
		SetLMPrinterSource(PaperSource);
		break;
	case BT_LG:
		SetLGPrinterSource(PaperSource);
		break;
	case BT_JC:
		SetJCPrinterSource(PaperSource);
		break;
	case BT_ZD:
		SetZDPrinterSource(PaperSource);
		break;
	case BT_XP:
		SetXPPrinterSource(PaperSource);
		break;
	case BT_SX:
		SetSXPrinterSource(PaperSource);
		break;
	case BT_HP:
		SetHPPrinterSource(PaperSource);
		break;
	default:
		theLog.Write("!!CIOPrinter::SetDefaultSourceByBrand, UnKnow BRAND_TYPE=%d, PaperSource=%d", eType, PaperSource);
		break;
	}
}

void CIOPrinter::SetDefaultSource(WORD dw)
{
	EnableDefaultSource(TRUE);
	DMBIN_AUTO;
	m_dwDefaultSource = dw;
	theLog.Write("CIOPrinter::SetDefaultSource,m_dwDefaultSource=%d", m_dwDefaultSource);
}

void CIOPrinter::SetPrintQuality(WORD dw)
{
	m_dwPrintQuality = dw;
	theLog.Write("CIOPrinter::SetPrintQuality,m_dwPrintQuality=%d", m_dwPrintQuality);
}

void CIOPrinter::SetPagesPerPaper(WORD dw)
{
	if (dw == 1 || dw == 2 || dw == 4 || dw == 6 || dw == 8	|| dw == 9 || dw == 16)
	{
		m_dwPagesPerPaper = dw;
	}
	else
	{
		m_dwPagesPerPaper = 1;
		theLog.Write("##CIOPrinter::SetPagesPerPaper,not support dw=%d,change to 1", dw);
	}
	theLog.Write("CIOPrinter::SetPagesPerPaper,m_dwPagesPerPaper=%d", m_dwPagesPerPaper);
}

void CIOPrinter::GetAllPageInOneParam(Rect& printRect, Image& image, AllPageInOneParam& param)
{	
	int nPagesPerPaper = m_dwPagesPerPaper;
	int nCurrentPage = m_nCurrentPage;
	if (nPagesPerPaper == 1)
	{
		//�����κ�����
		param.x_offset = 0.0;
		param.y_offset = 0.0;
		param.rotate_radians = 0.0;
	}
	else if (nCurrentPage >= 0)
	{
		WORD paper_orientation = (printRect.Width < printRect.Height) ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE;	//��ӡֽ����
		WORD page_orientation = (image.GetWidth() < image.GetHeight()) ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE;		//��ӡ���ݷ���
		int nPageNum = nCurrentPage % nPagesPerPaper;	//nPageNum��nCurrentPage��0Ϊ��1ҳ
		switch (nPagesPerPaper)
		{
		case 2:	//����һ
			{
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 2;
					printRect.Height = temp;
					param.x_offset = 0.0;
					param.y_offset = (2 - nPageNum) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					//printRect.Width = printRect.Width;
					printRect.Height = printRect.Height / 2;
					param.x_offset = printRect.Width;
					param.y_offset = (2 - nPageNum) * (printRect.Height);
					param.rotate_radians = 180.05;	//��ת180��,Ŀǰ�������ȡ180.0ʱ��emf��ӡ�ڻ�ͼʱ��
													//����������½��ǳ����ص������ԭ��δ֪��
													//��ʱȡһ����ӽ��Ĳ��Ҳ����½���ͼ������ֵ180.05��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					printRect.Width = printRect.Width / 2;
					//printRect.Height = printRect.Height;
					param.x_offset = nPageNum * (printRect.Width);
					param.y_offset = 0.0;
					param.rotate_radians = 0.0;	//��ת0��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					int temp = printRect.Height;
					printRect.Height = printRect.Width / 2;
					printRect.Width = temp;
					param.x_offset = nPageNum * (printRect.Height);
					param.y_offset = printRect.Width;
					param.rotate_radians = 270.0;	//��ת270��
				}
			}
			break;
		case 4:	//�ĺ�һ
			{
				int nRow = (nPageNum < 2) ? 0 : 1;
				int nCol = nPageNum - (nRow * 2);
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					printRect.Width = printRect.Width / 2;
					printRect.Height = printRect.Height / 2;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 2;
					printRect.Height = temp / 2;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 2;
					printRect.Height = temp / 2;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					printRect.Width = printRect.Width / 2;
					printRect.Height = printRect.Height / 2;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
			}
			break;
		case 6:	//����һ
			{
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					int nRow = 3 - (nPageNum % 3) - 1;
					int nCol = (nPageNum < 3) ? 0 : 1;
					int temp = printRect.Height;
					printRect.Height = printRect.Width / 2;
					printRect.Width = temp / 3;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					int nRow = 3 - (nPageNum % 3) - 1;
					int nCol = (nPageNum < 3) ? 0 : 1;
					printRect.Width = printRect.Width / 2;
					printRect.Height = printRect.Height / 3;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					int nRow = (nPageNum < 3) ? 0 : 1;
					int nCol = nPageNum % 3;
					printRect.Width = printRect.Width / 3;
					printRect.Height = printRect.Height / 2;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					int nRow = (nPageNum < 3) ? 0 : 1;
					int nCol = nPageNum % 3;
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 2;
					printRect.Height = temp / 3;
					param.x_offset = (nCol + 1) * (printRect.Height);
					param.y_offset = nRow * (printRect.Width);
					param.rotate_radians = 90.0;	//��ת90��
				}
			}
			break;
		case 8:	//�˺�һ
			{
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					int nRow = 4 - (nPageNum % 4) - 1;
					int nCol = (nPageNum < 4) ? 0 : 1;
					int temp = printRect.Height;
					printRect.Height = printRect.Width / 2;
					printRect.Width = temp / 4;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					int nRow = 4 - (nPageNum % 4) - 1;
					int nCol = (nPageNum < 4) ? 0 : 1;
					printRect.Width = printRect.Width / 2;
					printRect.Height = printRect.Height / 4;
					param.x_offset = (nCol + 1) * (printRect.Width);
					param.y_offset = (nRow + 1) * (printRect.Height);
					param.rotate_radians = 180.05;	//��ת180��,Ŀǰ�������ȡ180.0ʱ��emf��ӡ�ڻ�ͼʱ��
													//����������½��ǳ����ص������ԭ��δ֪��
													//��ʱȡһ����ӽ��Ĳ��Ҳ����½���ͼ������ֵ180.05��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					int nRow = (nPageNum < 4) ? 0 : 1;
					int nCol = nPageNum % 4;
					printRect.Width = printRect.Width / 4;
					printRect.Height = printRect.Height / 2;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					int nRow = (nPageNum < 4) ? 0 : 1;
					int nCol = nPageNum % 4;
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 2;
					printRect.Height = temp / 4;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
			}
			break;
		case 9:	//�ź�һ
			{
				int nRow = (nPageNum < 3) ? 0 : ((nPageNum < 6) ? 1 : 2);
				int nCol = nPageNum - (nRow * 3);
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					printRect.Width = printRect.Width / 3;
					printRect.Height = printRect.Height / 3;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 3;
					printRect.Height = temp / 3;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 3;
					printRect.Height = temp / 3;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					printRect.Width = printRect.Width / 3;
					printRect.Height = printRect.Height / 3;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
			}
			break;
		case 16:	//ʮ����һ
			{
				int nRow = (nPageNum < 4) ? 0 : ((nPageNum < 8) ? 1 : ((nPageNum < 12) ? 2 : 3));
				int nCol = nPageNum - (nRow * 4);
				if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_PORTRAIT)
				{
					printRect.Width = printRect.Width / 4;
					printRect.Height = printRect.Height / 4;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
				else if (paper_orientation==DMORIENT_PORTRAIT && page_orientation==DMORIENT_LANDSCAPE)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 4;
					printRect.Height = temp / 4;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_PORTRAIT)
				{
					int temp = printRect.Width;
					printRect.Width = printRect.Height / 4;
					printRect.Height = temp / 4;
					param.x_offset = nCol * (printRect.Height);
					param.y_offset = (nRow + 1) * (printRect.Width);
					param.rotate_radians = 270.0;	//��ת270��
				}
				else /*if (paper_orientation==DMORIENT_LANDSCAPE && page_orientation==DMORIENT_LANDSCAPE)*/
				{
					printRect.Width = printRect.Width / 4;
					printRect.Height = printRect.Height / 4;
					param.x_offset = nCol * (printRect.Width);
					param.y_offset = nRow * (printRect.Height);
					param.rotate_radians = 0.0;	//��ת0��
				}
			}
			break;
		default:
			{
				theLog.Write("!!CIOPrinter::GetAllPageInOneParam,not support AllPageInOneParam,nPagesPerPaper=%d", nPagesPerPaper);
			}
			break;
		}	
	}
}

void CIOPrinter::SetPrinted(int nPrinted)
{
	m_nPrinted = nPrinted;
	theLog.Write("CIOPrinter::SetPrinted,m_nPrinted=%d", m_nPrinted);
}

void CIOPrinter::SetPaperSize(WORD dw)
{
	DMPAPER_A4;
	if (DMPAPER_A4 == dw)
	{
		m_dwPaperSize = DMPAPER_A4;
	}
	else if (DMPAPER_A3 == dw)
	{
		m_dwPaperSize = DMPAPER_A3;
	} 
	else
	{
		if (m_bUseA4Print)
		{
			theLog.Write("!!CIOPrinter::SetPaperSize,PapeSize [%d] is not support,changeto DMPAPER_A4.", dw);
			m_dwPaperSize = DMPAPER_A4;
		}
		else
		{
			m_dwPaperSize = dw;
		}
	}
	theLog.Write("CIOPrinter::SetPaperSize,m_dwPaperSize=%d", m_dwPaperSize);
}

//GetPrinterStatus��֧��Windows 95/98/Me
DWORD CIOPrinter::GetPrinterStatus(CString sName)
{
	DWORD dwStatus = 0;
	HANDLE h = 0;
	PRINTER_DEFAULTS pd;
	pd.pDatatype     = NULL;
	pd.pDevMode      = NULL;
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	if(OpenPrinter(sName.GetBuffer(),&h,&pd) && h)
	{
		DWORD dwSize = 0;
		GetPrinter(h,6,0,0,&dwSize);
		//theLog.Write("GetPrinterStatus %d",dwSize);
		if (dwSize > 0)
		{
			BYTE* pData = new BYTE[dwSize];
			if(GetPrinter(h,6,pData,dwSize,&dwSize))
			{
				PRINTER_STATUS_BUSY;
				PRINTER_INFO_6* pInfo = (PRINTER_INFO_6*)pData;
				dwStatus = pInfo->dwStatus;
			}
			else
			{
				theLog.Write("getprinter err %d",::GetLastError());				
			}
			delete[] pData;
		}
		ClosePrinter(h);
	}
	else
	{
		theLog.Write("!!CIOPrinter::GetPrinterStatus,OpenPrinter fail,err=%u,sName=%s", GetLastError(), sName);
	}
	
	sName.ReleaseBuffer();
	return dwStatus;
}
void CIOPrinter::SetSelPrinter(CString sName)
{
	theLog.Write("sel printer name %s",sName);
	m_sPrinter = sName;
	CPrinterPaperSize::GetInstance().SetPrinterName(m_sPrinter.GetString());	//add by zxl,20150706,��ȡ��ӡ��֧�ֵ�ֽ��
	HANDLE h = 0;
	PRINTER_DEFAULTS pd;
	pd.pDatatype     = NULL;
	pd.pDevMode      = NULL;
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	if(OpenPrinter(sName.GetBuffer(),&h,0) && h)
	{
		/*
		DWORD dwSize = 0;
		GetPrinter(h,2,0,0,&dwSize);
		theLog.Write("SetSelPrinter %d",dwSize);
		if (dwSize > 0)
		{
			BYTE* pData = new BYTE[dwSize];
			if(GetPrinter(h,2,pData,dwSize,&dwSize))
			{
				PRINTER_STATUS_BUSY;
				DWORD dwNew = PRINTER_STATUS_IO_ACTIVE;
				PRINTER_INFO_2* pInfo = (PRINTER_INFO_2*)pData;
				dwNew = 0;
				BOOL b = 0;//SetPrinter(h,0,(BYTE*)&dwNew,PRINTER_CONTROL_SET_STATUS);
				theLog.Write("print(%s) (%d)(%d),%d",sName,pInfo->Status,b,::GetLastError());
			}
			else
			{
				theLog.Write("SetSelPrinter err %d",::GetLastError());
			}
			delete[] pData;
		}*/
		ClosePrinter(h);
	}
	else
	{
		theLog.Write("open the printer name %s,faile ,%d",sName,::GetLastError());
		m_sPrinter.Empty();
	}
	sName.ReleaseBuffer();
}

BOOL CIOPrinter::PrintTiffFile(CString sPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom,int nTo, BOOL bUsePhysicalArea /*= FALSE*/)
{
	if (m_nPrinted > 0)
	{
		return PrintTiffFileBreakPoint(sPath, sDocName, pChargeCall, nFrom, nTo, bUsePhysicalArea);
	}
	theLog.Write("CIOPrinter::PrintTiffFile,sPath=%s,sDocName=%s,nFrom=%d,nTo=%d,bUsePhysicalArea=%d",sPath,sDocName,nFrom,nTo,bUsePhysicalArea);
	int nErrRet = SP_NOTREPORTED;
	int nPrinted = 0;
	HDC hdcPrinter = 0;
	float dpiRatio = 1.0;
	ImageAttributes *pImgAtt = NULL;
	Graphics* pGraphics = NULL;

	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintTiffFile %s",m_sPrinter);

	DEVNAMES* pDevName = 0;
	BYTE* pDev = 0;
	if(!PrinterUtils::GetPrinterDevMode(m_sPrinter,pDevName,pDev))
	{
		theLog.Write("GetPrinterDevMode err");
		goto ERR_PRINT;
	}
	if (!pDev)
	{
		theLog.Write("GetPrinterDevMode pDev = null err");
		goto ERR_PRINT;
	}
	PDEVMODE pDevmodeWork = (PDEVMODE)pDev;
		
	//�ı䷽��
	if(pDevmodeWork->dmFields & DM_ORIENTATION)  
	{
		if (pDevmodeWork->dmOrientation == m_dwOrientation)
		{
			theLog.Write("the printer no change orientation %d",m_dwOrientation);
		}
		else
		{
			theLog.Write("change the orientation %d",m_dwOrientation);
			pDevmodeWork->dmOrientation = m_dwOrientation;
		}
	}
	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		if (pDevmodeWork->dmColor == m_dwColor)
		{
			theLog.Write("the printer no change color %d",m_dwColor);
		}
		else
		{
			theLog.Write("change the color %d",m_dwColor);
			pDevmodeWork->dmColor = m_dwColor;
		}
	}
	else
	{
		theLog.Write("no support color Field, ForceMonochromePrint");
		m_bForceMonochromePrint = TRUE;
	}

	if (m_bForceMonochromePrint)
	{
		//һ������²���Ҫ����ɫ�ļ�ת���ɻҶ�ͼƬ,
		//����,�����ӡ�豸��֧�ֲ�ɫ���Ƶ�ʱ��,����ʹ��,
		if (m_dwColor == DMCOLOR_MONOCHROME)
		{
			theLog.Write("change the ImageAttributes to DMCOLOR_MONOCHROME");
			pImgAtt = new ImageAttributes();
			ASSERT(pImgAtt);
			pImgAtt->SetColorMatrix(&colorMatrix, ColorMatrixFlagsSkipGrays, ColorAdjustTypeDefault);
		}
	}

	//�ı�ֽ�Ŵ�С
	DM_PAPERSIZE;
	if (pDevmodeWork->dmFields & DM_PAPERSIZE)
	{
		if (pDevmodeWork->dmPaperSize == m_dwPaperSize)
		{
			theLog.Write("the printer no change papersize %d",m_dwPaperSize);
		}
		else
		{
			theLog.Write("change the papersize %d",m_dwPaperSize);
			pDevmodeWork->dmPaperSize = m_dwPaperSize;
		}
	}

	//ָ���Զ���ֽ�Ŵ�С��ӡ�ĵ�
	if ((m_dwPaperSize>=DMPAPER_USER) && (m_dwPaperWidth>=10) && (m_dwPaperLength>=10))
	{
		theLog.Write("�Զ���ֽ�Ŵ�ӡ m_dwPaperSize=%d,m_dwPaperWidth=%d,m_dwPaperLength=%d",m_dwPaperSize,m_dwPaperWidth, m_dwPaperLength);
		if (m_dwPaperWidth > m_dwPaperLength)
		{
			pDevmodeWork->dmOrientation = DMORIENT_LANDSCAPE;
			pDevmodeWork->dmPaperWidth = m_dwPaperLength;
			pDevmodeWork->dmPaperLength = m_dwPaperWidth;
		}
		else
		{
			pDevmodeWork->dmOrientation = DMORIENT_PORTRAIT;
			pDevmodeWork->dmPaperWidth = m_dwPaperWidth;
			pDevmodeWork->dmPaperLength = m_dwPaperLength;
		}
		//dmPaperSize 
		//For printer devices only, selects the size of the paper to print on. 
		//This member can be set to zero if the length and width of the paper are both set by the dmPaperLength and dmPaperWidth members. 
		//Otherwise, the dmPaperSize member can be set to a device specific value greater than or equal to DMPAPER_USER or to one of the following predefined values. 
		pDevmodeWork->dmPaperSize = DMPAPER_USER;
		pDevmodeWork->dmFields |= DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH;
	}

	//˫���ӡ
	theLog.Write("dup = %d",pDevmodeWork->dmDuplex);
	if (DM_DUPLEX & pDevmodeWork->dmFields)
	{
		DMDUP_SIMPLEX;
		DMDUP_VERTICAL;
		DMDUP_HORIZONTAL;
		if (pDevmodeWork->dmDuplex == m_dwDuplex)
		{
			theLog.Write("the printer no change duplex %d",m_dwDuplex);
		}
		else
		{
			theLog.Write("change the duplex %d",m_dwDuplex);
			pDevmodeWork->dmDuplex = m_dwDuplex;
		}
	}

	//�ı��ӡ����
	DM_COPIES;
	if (pDevmodeWork->dmFields & DM_COPIES)
	{
		if (pDevmodeWork->dmCopies == m_dwCopies)
		{
			theLog.Write("the printer no change copies %d",m_dwCopies);
		}
		else
		{
			theLog.Write("change the copies %d",m_dwCopies);
			pDevmodeWork->dmCopies = m_dwCopies;
		}
	}

	//��ݴ�ӡ
	DM_COLLATE;
	if (pDevmodeWork->dmFields & DM_COLLATE)
	{
		if (pDevmodeWork->dmCollate == m_dwCollate)
		{
			theLog.Write("the printer no change collate %d",m_dwCollate);
		}
		else
		{
			theLog.Write("change the collate %d",m_dwCollate);
			pDevmodeWork->dmCollate = m_dwCollate;
		}
	}

	//ֽ����Դ
	if (IsEnableDefaultSource() && (DM_DEFAULTSOURCE & pDevmodeWork->dmFields))
	{
		if (pDevmodeWork->dmDefaultSource == m_dwDefaultSource)
		{
			theLog.Write("the printer no change DefaultSource %d",m_dwDefaultSource);
		}
		else
		{
			theLog.Write("change the DefaultSource %d",m_dwDefaultSource);
			pDevmodeWork->dmDefaultSource = m_dwDefaultSource;
		}
	}

	hdcPrinter = ::CreateDC((LPCTSTR)pDevName + pDevName->wDriverOffset,
		(LPCTSTR)pDevName + pDevName->wDeviceOffset,
		(LPCTSTR)pDevName + pDevName->wOutputOffset,
		(DEVMODE*)pDev);
	if (hdcPrinter == NULL)
	{
		theLog.Write("!!CIOPrinter::PrintTiffFile,11,hdcPrinter = null");
		goto ERR_PRINT;
	}
	else
	{		
		// call StartDoc() to begin printing
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = "RAW";
		docinfo.lpszOutput = NULL;
		VerifyDocName(sDocName);
		if (sDocName.IsEmpty())
		{
			docinfo.lpszDocName = _T("Fragment");
		}
		else
		{
			docinfo.lpszDocName = sDocName;
		}

		// if it fails, complain and exit gracefully
		nErrRet = StartDoc(hdcPrinter, &docinfo);
		if (nErrRet <= 0)
		{
			SP_ERROR;	
			theLog.Write("!CIOPrinter::PrintTiffFile,12,StartDoc failed, nErrRet=%d, err=%d, hdcPrinter=%x, lpszDocName=%s", nErrRet, GetLastError(), hdcPrinter, docinfo.lpszDocName);
			goto ERR_PRINT;
		}
		
		CStringW wsPath(sPath);
		Image image(wsPath);
		UINT count = 0;
		count = image.GetFrameDimensionsCount();
		theLog.Write("The number of dimensions is %d.\n", count);
		GUID* pDimensionIDs = (GUID*)malloc(sizeof(GUID)*count);

		// Get the list of frame dimensions from the Image object.
		image.GetFrameDimensionsList(pDimensionIDs, count);
		for (int n = 0 ; n < count; n ++)
		{
			// Display the GUID of the first (and only) frame dimension.
			WCHAR strGuid[39];
			StringFromGUID2(pDimensionIDs[n], strGuid, 39);
			theLog.Write("The first (and only) dimension ID is %S.\n", strGuid);

			// Get the number of frames in the first dimension.
			UINT frameCount = image.GetFrameCount(&pDimensionIDs[n]);
			theLog.Write("The number of frames in that dimension is %d.\n", frameCount);

			if(nFrom >= 0 && nTo >= nFrom && nTo < frameCount)
			{
				theLog.Write(_T("print tiff from = %d to = %d ,copy = %d"),nFrom,nTo,m_dwCopies);
				m_nCurrentPage = 0;
				BOOL bAllInOne = (m_dwPagesPerPaper == 1) ? FALSE : TRUE;	//�Ƿ���һ��ӡ
				for (int m = nFrom; m <= nTo; m ++)
				{
					BOOL bBeginPage = (!bAllInOne || ((m_nCurrentPage % m_dwPagesPerPaper) == 0)) ? TRUE : FALSE;	//�Ƿ�һ��ֽ��ʼ����ҳ
					BOOL bEndPage = (!bAllInOne || (m == nTo) || (((m_nCurrentPage+1) % m_dwPagesPerPaper) == 0)) ? TRUE : FALSE;	//�Ƿ�һ��ֽ��������ҳ
					theLog.Write(_T("print tiff nFrom = %d, m = %d, bBeginPage=%d, bEndPage=%d"),nFrom,m,bBeginPage,bEndPage);

					image.SelectActiveFrame(&FrameDimensionPage,m);

					if (!bAllInOne)
					{
						ResetPrinterHDC(hdcPrinter, pDevmodeWork, image); //����HDC
					}

					if (bBeginPage)
					{
						nErrRet = StartPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							theLog.Write("StartPage failed");
							goto ERR_PRINT;
						}

						pGraphics = Graphics::FromHDC(hdcPrinter);
						pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
						pGraphics->SetInterpolationMode(InterpolationModeHighQuality); 
						pGraphics->SetPageUnit(UnitPixel);
						pGraphics->SetPageScale(1.0);
					}

					GraphicsState state = pGraphics->Save();

					Rect rc;
					GetPrintDeviceArea(hdcPrinter, image, rc, *pGraphics, bUsePhysicalArea);	//��ȡ��ӡ����
					int nPictureWidth = image.GetWidth();
					int nPictureHeight = image.GetHeight();
					theLog.Write("print tiff rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
						,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

					if (IsWater())
					{
						CPrintWithWaterMarked water;
						water.Init(&image,&m_oSec);
						water.DrawImg(&image);
						pGraphics->DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}
					else
					{
						pGraphics->DrawImage(&image, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}
					
					pGraphics->Restore(state);

					if (bEndPage)
					{
						pGraphics->ReleaseHDC(hdcPrinter);
						delete pGraphics;
						pGraphics = NULL;

						nErrRet = EndPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							theLog.Write("EndPage failed");
							goto ERR_PRINT;
						}
						nPrinted++;
						//�ص�֪ͨ���½��棬֪ͨ�������۷�
						if(pChargeCall)
						{
							pChargeCall->OnNewPagePrinted(m);//PrintTiffFile
						}
					}
					m_nCurrentPage++;
				}
			}
		}

		if(pDimensionIDs)
		{
			free(pDimensionIDs);
			pDimensionIDs = NULL;
		}

		nErrRet = EndDoc(hdcPrinter);
		if (nErrRet <= 0)
		{
		    theLog.Write("EndDoc failed");
			goto ERR_PRINT;
		}

		if(pChargeCall)
		{
			pChargeCall->OnPrintedCmplt(nPrinted*m_dwCopies);//PrintTiffFile
		}
	}
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}
	if (pGraphics)
	{
		delete pGraphics;
	}

	return TRUE;

ERR_PRINT:
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}
	if (pGraphics)
	{
		delete pGraphics;
	}

	theLog.Write("print tiff err:%d,prined:%d",nErrRet,nPrinted);
	if (pChargeCall)
	{
		pChargeCall->OnPrintedErr(nPrinted*m_dwCopies,nErrRet);
	}
	return FALSE;
}

BOOL CIOPrinter::PrintTiffFileBreakPoint(CString sPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom,int nTo, BOOL bUsePhysicalArea /*= FALSE*/)
{
	theLog.Write("CIOPrinter::PrintTiffFileBreakPoint,sPath=%s,sDocName=%s,nFrom=%d,nTo=%d,nType=%d,bUsePhysicalArea=%d",sPath,sDocName,nFrom,nTo,bUsePhysicalArea);
	int nErrRet = SP_NOTREPORTED;
	int nPrinted = 0;
	HDC hdcPrinter = 0;
	float dpiRatio = 1.0;
	ImageAttributes *pImgAtt = NULL;

	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintTiffFileBreakPoint %s",m_sPrinter);

	DEVNAMES* pDevName = 0;
	BYTE* pDev = 0;
	if(!PrinterUtils::GetPrinterDevMode(m_sPrinter,pDevName,pDev))
	{
		theLog.Write("GetPrinterDevMode err");
		goto ERR_PRINT;
	}
	if (!pDev)
	{
		theLog.Write("GetPrinterDevMode pDev = null err");
		goto ERR_PRINT;
	}
	PDEVMODE pDevmodeWork = (PDEVMODE)pDev;

	dpiRatio=300.0f/pDevmodeWork->dmPrintQuality;
	theLog.Write(" dpiRatio = %0.2f",dpiRatio);
	//�ı䷽��
	if(pDevmodeWork->dmFields & DM_ORIENTATION)  
	{
		if (pDevmodeWork->dmOrientation == m_dwOrientation)
		{
			theLog.Write("the printer no change orientation %d",m_dwOrientation);
		}
		else
		{
			theLog.Write("change the orientation %d",m_dwOrientation);
			pDevmodeWork->dmOrientation = m_dwOrientation;
		}
	}
	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		if (pDevmodeWork->dmColor == m_dwColor)
		{
			theLog.Write("the printer no change color %d",m_dwColor);
		}
		else
		{
			theLog.Write("change the color %d",m_dwColor);
			pDevmodeWork->dmColor = m_dwColor;
		}
	}
	else
	{
		theLog.Write("no support color Field, ForceMonochromePrint");
		m_bForceMonochromePrint = TRUE;
	}

	if (m_bForceMonochromePrint)
	{
		//һ������²���Ҫ����ɫ�ļ�ת���ɻҶ�ͼƬ,
		//����,�����ӡ�豸��֧�ֲ�ɫ���Ƶ�ʱ��,����ʹ��,
		if (m_dwColor == DMCOLOR_MONOCHROME)
		{
			theLog.Write("change the ImageAttributes to DMCOLOR_MONOCHROME");
			pImgAtt = new ImageAttributes();
			ASSERT(pImgAtt);
			pImgAtt->SetColorMatrix(&colorMatrix, ColorMatrixFlagsSkipGrays, ColorAdjustTypeDefault);
		}
	}

	//�ı�ֽ�Ŵ�С
	DM_PAPERSIZE;
	if (pDevmodeWork->dmFields & DM_PAPERSIZE)
	{
		if (pDevmodeWork->dmPaperSize  == m_dwPaperSize)
		{
			theLog.Write("the printer no change papersize %d",m_dwPaperSize);
		}
		else
		{
			theLog.Write("change the papersize %d",m_dwPaperSize);
			pDevmodeWork->dmPaperSize  = m_dwPaperSize;
		}
	}

	//ָ���Զ���ֽ�Ŵ�С��ӡ�ĵ�
	if ((m_dwPaperSize>=DMPAPER_USER) && (m_dwPaperWidth>=10) && (m_dwPaperLength>=10))
	{
		theLog.Write("�Զ���ֽ�Ŵ�ӡ m_dwPaperSize=%d,m_dwPaperWidth=%d,m_dwPaperLength=%d",m_dwPaperSize,m_dwPaperWidth, m_dwPaperLength);
		if (m_dwPaperWidth > m_dwPaperLength)
		{
			pDevmodeWork->dmOrientation = DMORIENT_LANDSCAPE;
			pDevmodeWork->dmPaperWidth = m_dwPaperLength;
			pDevmodeWork->dmPaperLength = m_dwPaperWidth;
		}
		else
		{
			pDevmodeWork->dmOrientation = DMORIENT_PORTRAIT;
			pDevmodeWork->dmPaperWidth = m_dwPaperWidth;
			pDevmodeWork->dmPaperLength = m_dwPaperLength;
		}
		pDevmodeWork->dmPaperSize = DMPAPER_USER;
		pDevmodeWork->dmFields |= DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH;
	}

	//˫���ӡ
	theLog.Write("dup = %d",pDevmodeWork->dmDuplex);
	if (DM_DUPLEX & pDevmodeWork->dmFields)
	{
		DMDUP_SIMPLEX;
		DMDUP_VERTICAL;
		DMDUP_HORIZONTAL;
		if (pDevmodeWork->dmDuplex == m_dwDuplex)
		{
			theLog.Write("the printer no change duplex %d",m_dwDuplex);
		}
		else
		{
			theLog.Write("change the duplex %d",m_dwDuplex);
			pDevmodeWork->dmDuplex = m_dwDuplex;
		}
	}

#if 0
	//�ı��ӡ����
	DM_COPIES;
	if (pDevmodeWork->dmFields & DM_COPIES)
	{
		if (pDevmodeWork->dmCopies == m_dwCopies)
		{
			theLog.Write("the printer no change copies %d",m_dwCopies);
		}
		else
		{
			theLog.Write("change the copies %d",m_dwCopies);
			pDevmodeWork->dmCopies = m_dwCopies;
		}
	}
#endif

	//��ݴ�ӡ
	DM_COLLATE;
	if (pDevmodeWork->dmFields & DM_COLLATE)
	{
		if (pDevmodeWork->dmCollate == m_dwCollate)
		{
			theLog.Write("the printer no change collate %d",m_dwCollate);
		}
		else
		{
			theLog.Write("change the collate %d",m_dwCollate);
			pDevmodeWork->dmCollate = m_dwCollate;
		}
	}

	//ֽ����Դ
	if (IsEnableDefaultSource() && (DM_DEFAULTSOURCE & pDevmodeWork->dmFields))
	{
		if (pDevmodeWork->dmDefaultSource == m_dwDefaultSource)
		{
			theLog.Write("the printer no change DefaultSource %d",m_dwDefaultSource);
		}
		else
		{
			theLog.Write("change the DefaultSource %d",m_dwDefaultSource);
			pDevmodeWork->dmDefaultSource = m_dwDefaultSource;
		}
	}

	hdcPrinter = ::CreateDC((LPCTSTR)pDevName + pDevName->wDriverOffset,
		(LPCTSTR)pDevName + pDevName->wDeviceOffset,
		(LPCTSTR)pDevName + pDevName->wOutputOffset,
		(DEVMODE*)pDev);
	if (hdcPrinter == NULL)
	{
		theLog.Write("hdcPrinter = null,err=%u",GetLastError());
		goto ERR_PRINT;
	}
	else
	{		
		// call StartDoc() to begin printing
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = "RAW";
		docinfo.lpszOutput = NULL;
		VerifyDocName(sDocName);
		if (sDocName.IsEmpty())
		{
			docinfo.lpszDocName = _T("Fragment");
		}
		else
		{
			docinfo.lpszDocName = sDocName;
		}

		// if it fails, complain and exit gracefully
		nErrRet = StartDoc(hdcPrinter, &docinfo);
		if (nErrRet <= 0)
		{
			SP_ERROR;			
			goto ERR_PRINT;
		}

		CStringW wsPath(sPath);
		Image image(wsPath);
		UINT count = 0;
		count = image.GetFrameDimensionsCount();
		theLog.Write("The number of dimensions is %d.\n", count);
		GUID* pDimensionIDs = (GUID*)malloc(sizeof(GUID)*count);

		// Get the list of frame dimensions from the Image object.
		image.GetFrameDimensionsList(pDimensionIDs, count);
		for (int n = 0 ; n < count; n ++)
		{
			// Display the GUID of the first (and only) frame dimension.
			WCHAR strGuid[39];
			StringFromGUID2(pDimensionIDs[n], strGuid, 39);
			theLog.Write("The first (and only) dimension ID is %S.\n", strGuid);

			// Get the number of frames in the first dimension.
			UINT frameCount = image.GetFrameCount(&pDimensionIDs[n]);
			theLog.Write("The number of frames in that dimension is %d.\n", frameCount);

			if((nFrom == -1 && nTo == -1))
			{
				nFrom = 0;
				nTo = frameCount - 1;
			}

			int nFrom2 = 0;
			int nCopy = m_dwCopies;
			if (nCopy == 1)
			{
				nCopy = (m_nPrinted > 0) ? 0 : 1;
				nFrom2 = m_nPrinted;
			}
			else
			{
				int nPagePerCopy = nTo - nFrom + 1;
				int nTotalPage = nCopy * nPagePerCopy;
				int nLeftPage = nTotalPage - m_nPrinted;
				nCopy = nLeftPage / nPagePerCopy;
				int nLeftPage2 = nLeftPage - (nCopy * nPagePerCopy);
				nFrom2 = nPagePerCopy - nLeftPage2;
			}

			if(nFrom >= 0 && nTo >= nFrom && nTo < frameCount)
			{
				theLog.Write(_T("print tiff from = %d to = %d ,copy = %d, nFrom2=%d"),nFrom,nTo,nCopy, nFrom2);
				for (int i=nFrom2; i <= nTo; i++)
				{
					theLog.Write(_T("print tiff nFrom2 = %d i = %d"),nFrom2,i);

					image.SelectActiveFrame(&FrameDimensionPage,i);

					ResetPrinterHDC(hdcPrinter, pDevmodeWork, image); //����HDC

					nErrRet = StartPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}

					Graphics graphics(hdcPrinter);
					graphics.SetSmoothingMode(SmoothingModeHighQuality);
					graphics.SetInterpolationMode(InterpolationModeHighQuality); 
					graphics.SetPageUnit(UnitPixel);
					graphics.SetPageScale(1.0);

					Rect rc;
					GetPrintDeviceArea(hdcPrinter, image, rc, graphics, bUsePhysicalArea);	//��ȡ��ӡ����

					int nPictureWidth = image.GetWidth();
					int nPictureHeight = image.GetHeight();
					theLog.Write("print tiff AA rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
						,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

					//���ˮӡ
					if (IsWater())
					{
						CPrintWithWaterMarked water;
						water.Init(&image,&m_oSec);
						water.DrawImg(&image);
						graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}
					else
					{
						graphics.DrawImage(&image, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}

					graphics.ReleaseHDC(hdcPrinter);

					nErrRet = EndPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}
					nPrinted++;
					//�ص�֪ͨ���½��棬֪ͨ�������۷�
					if(pChargeCall)
					{
						pChargeCall->OnNewPagePrinted(nFrom2);//PrintTiffFileBreakPoint
					}
				}		

				for (int x = 0; x < nCopy; x++ )
				{
					theLog.Write("print tiff TotalCopy=%d, CurrentCopy=%d",nCopy, x);
					for (int m = nFrom; m <= nTo; m ++)
					{
						theLog.Write(_T("print tiff nFrom = %d, m = %d"),nFrom,m);

						image.SelectActiveFrame(&FrameDimensionPage,m);

						ResetPrinterHDC(hdcPrinter, pDevmodeWork, image); //����HDC

						nErrRet = StartPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}

						Graphics graphics(hdcPrinter);
						graphics.SetSmoothingMode(SmoothingModeHighQuality);
						graphics.SetInterpolationMode(InterpolationModeHighQuality); 
						graphics.SetPageUnit(UnitPixel);
						graphics.SetPageScale(1.0);

						Rect rc;
						GetPrintDeviceArea(hdcPrinter, image, rc, graphics, bUsePhysicalArea);	//��ȡ��ӡ����

						int nPictureWidth = image.GetWidth();
						int nPictureHeight = image.GetHeight();
						theLog.Write("print tiff BB rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
							,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

						//���ˮӡ
						if (IsWater())
						{
							CPrintWithWaterMarked water;
							water.Init(&image,&m_oSec);
							water.DrawImg(&image);
							graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
						}
						else
						{
							graphics.DrawImage(&image, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
						}
						
						graphics.ReleaseHDC(hdcPrinter);

						nErrRet = EndPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}
						nPrinted++;
						//�ص�֪ͨ���½��棬֪ͨ�������۷�
						if(pChargeCall)
						{
							pChargeCall->OnNewPagePrinted(m);//PrintTiffFileBreakPoint
						}
					}
				}
			
			}
			else
			{
				theLog.Write("!!print tiff,nFrom=%d, nTo=%d,frameCount=%d", nFrom, nTo, frameCount);
				ASSERT(0);
			}
		}

		if(pDimensionIDs)
		{
			free(pDimensionIDs);
			pDimensionIDs = NULL;
		}

		if(pChargeCall)
		{
			pChargeCall->OnPrintedCmplt(nPrinted);//PrintTiffFileBreakPoint 
		}
		nErrRet = EndDoc(hdcPrinter);
		if (nErrRet <= 0)
		{
			goto ERR_PRINT;
		}

	}
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}

	return TRUE;

ERR_PRINT:
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}

	theLog.Write("print tiff err:%d,prined:%d",nErrRet,nPrinted);
	if (pChargeCall)
	{
		pChargeCall->OnPrintedErr(nPrinted,nErrRet);
	}
	return FALSE;
}

BOOL CIOPrinter::PrintEmfFile(CString sPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom/* = -1*/,int nTo/* = -1*/)
{
	if (m_nPrinted > 0)
	{
		return PrintEmfFileBreakPoint(sPath, sDocName, pChargeCall, nFrom, nTo);
	}
	theLog.Write("CIOPrinter::PrintEmfFile,1,sPath=%s,sDocName=%s,nFrom=%d,nTo=%d",sPath,sDocName,nFrom,nTo);
	int nErrRet = SP_NOTREPORTED;
	int nPrinted = 0;
	HDC hdcPrinter = 0;
	ImageAttributes *pImgAtt = NULL;
	Graphics* pGraphics = NULL;

	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintEmfFile,2,Printer=[%s]",m_sPrinter);

	DEVNAMES* pDevName = 0;
	BYTE* pDev = 0;
	if(!PrinterUtils::GetPrinterDevMode(m_sPrinter,pDevName,pDev))
	{
		theLog.Write("GetPrinterDevMode err");
		goto ERR_PRINT;
	}
	if (!pDev)
	{
		theLog.Write("GetPrinterDevMode pDev = null err");
		goto ERR_PRINT;
	}
	PDEVMODE pDevmodeWork = (PDEVMODE)pDev;

	//�ı䷽��
	if(pDevmodeWork->dmFields & DM_ORIENTATION)  
	{
		if (pDevmodeWork->dmOrientation == m_dwOrientation)
		{
			theLog.Write("the printer no change orientation %d",m_dwOrientation);
		}
		else
		{
			theLog.Write("change the orientation");
			pDevmodeWork->dmOrientation = m_dwOrientation;
		}
	}

	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		if (pDevmodeWork->dmColor == m_dwColor)
		{
			theLog.Write("the printer no change color %d",m_dwColor);
		}
		else
		{
			theLog.Write("change the color %d",m_dwColor);
			pDevmodeWork->dmColor = m_dwColor;
		}
	}
	else
	{
		theLog.Write("no support color Field, ForceMonochromePrint");
		m_bForceMonochromePrint = TRUE;
	}

	if (m_bForceMonochromePrint)
	{
		//һ������²���Ҫ����ɫ�ļ�ת���ɻҶ�ͼƬ,
		//����,�����ӡ�豸��֧�ֲ�ɫ���Ƶ�ʱ��,����ʹ��,
		if (m_dwColor == DMCOLOR_MONOCHROME)
		{
			theLog.Write("change the ImageAttributes to DMCOLOR_MONOCHROME");
			pImgAtt = new ImageAttributes();
			ASSERT(pImgAtt);
			pImgAtt->SetColorMatrix(&colorMatrix, ColorMatrixFlagsSkipGrays, ColorAdjustTypeDefault);
		}
	}

	//�ı�ֽ�Ŵ�С
	DM_PAPERSIZE;
	if (pDevmodeWork->dmFields & DM_PAPERSIZE)
	{
		if (pDevmodeWork->dmPaperSize  == m_dwPaperSize)
		{
			theLog.Write("the printer no change papersize %d",m_dwPaperSize);
		}
		else
		{
			theLog.Write("change the papersize %d",m_dwPaperSize);
			pDevmodeWork->dmPaperSize  = m_dwPaperSize;
		}
	}

	//ָ���Զ���ֽ�Ŵ�С��ӡ�ĵ�
	if ((m_dwPaperSize>=DMPAPER_USER) && (m_dwPaperWidth>=10) && (m_dwPaperLength>=10))
	{
		theLog.Write("�Զ���ֽ�Ŵ�ӡ m_dwPaperSize=%d,m_dwPaperWidth=%d,m_dwPaperLength=%d",m_dwPaperSize,m_dwPaperWidth, m_dwPaperLength);
		if (m_dwPaperWidth > m_dwPaperLength)
		{
			pDevmodeWork->dmOrientation = DMORIENT_LANDSCAPE;
			pDevmodeWork->dmPaperWidth = m_dwPaperLength;
			pDevmodeWork->dmPaperLength = m_dwPaperWidth;
		}
		else
		{
			pDevmodeWork->dmOrientation = DMORIENT_PORTRAIT;
			pDevmodeWork->dmPaperWidth = m_dwPaperWidth;
			pDevmodeWork->dmPaperLength = m_dwPaperLength;
		}
		pDevmodeWork->dmPaperSize = DMPAPER_USER;
		pDevmodeWork->dmFields |= DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH;
	}

	//˫���ӡ
	theLog.Write("dup = %d",pDevmodeWork->dmDuplex);
	if (DM_DUPLEX & pDevmodeWork->dmFields)
	{
		DMDUP_SIMPLEX;
		if (pDevmodeWork->dmDuplex == m_dwDuplex)
		{
			theLog.Write("the printer no change duplex %d",m_dwDuplex);
		}
		else
		{
			theLog.Write("change the duplex %d",m_dwDuplex);
			pDevmodeWork->dmDuplex = m_dwDuplex;
		}
	}

	//�ı��ӡ����
	DM_COPIES;
	if (pDevmodeWork->dmFields & DM_COPIES)
	{
		if (pDevmodeWork->dmCopies == m_dwCopies)
		{
			theLog.Write("the printer no change copies %d",m_dwCopies);
		}
		else
		{
			theLog.Write("change the copies %d",m_dwCopies);
			pDevmodeWork->dmCopies = m_dwCopies;
		}
	}

	//��ݴ�ӡ
	DM_COLLATE;
	if (pDevmodeWork->dmFields & DM_COLLATE)
	{
		if (pDevmodeWork->dmCollate == m_dwCollate)
		{
			theLog.Write("the printer no change collate %d",m_dwCollate);
		}
		else
		{
			theLog.Write("change the collate %d",m_dwCollate);
			pDevmodeWork->dmCollate = m_dwCollate;
		}
	}

	//ѡ���ӡֽ����Դ
	DM_DEFAULTSOURCE;
	if (IsEnableDefaultSource() && (pDevmodeWork->dmFields & DM_DEFAULTSOURCE))
	{
		if (pDevmodeWork->dmDefaultSource == m_dwDefaultSource)
		{
			theLog.Write("the printer no change paper source %d",m_dwDefaultSource);
		}
		else
		{
			theLog.Write("change the paper source %d",m_dwDefaultSource);
			pDevmodeWork->dmDefaultSource = m_dwDefaultSource;
		}
	}

	hdcPrinter = ::CreateDC((LPCTSTR)pDevName + pDevName->wDriverOffset,
		(LPCTSTR)pDevName + pDevName->wDeviceOffset,
		(LPCTSTR)pDevName + pDevName->wOutputOffset,
		(DEVMODE*)pDev);
	if (hdcPrinter == NULL)
	{
		theLog.Write("hdcPrinter = null");
		goto ERR_PRINT;
	}
	else
	{		
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = "RAW";
		docinfo.lpszOutput = NULL;
		VerifyDocName(sDocName);
		if (sDocName.IsEmpty())
		{
			docinfo.lpszDocName = _T("Fragment");
		}
		else
		{
			docinfo.lpszDocName = sDocName;
		}

		// call StartDoc() to begin printing
		nErrRet = StartDoc(hdcPrinter, &docinfo);
		if (nErrRet <= 0)
		{
			SP_ERROR;			
			goto ERR_PRINT;
		}

		CParseSpl spl;
		if (spl.InitParse(sPath, TRUE))
		{
			DWORD dwPagePerCopy = spl.GetPageCountPerCopy();
			if(nFrom >= 0 && nTo >= nFrom && nTo < dwPagePerCopy)
			{
				theLog.Write(_T("print emf from = %d to = %d ,copy = %d"),nFrom,nTo,m_dwCopies);
				m_nCurrentPage = 0;
				BOOL bAllInOne = (m_dwPagesPerPaper == 1) ? FALSE : TRUE;	//�Ƿ���һ��ӡ
				for (int m = nFrom; m <= nTo; m ++)
				{
					BOOL bBeginPage = (!bAllInOne || ((m_nCurrentPage % m_dwPagesPerPaper) == 0)) ? TRUE : FALSE;	//�Ƿ�һ��ֽ��ʼ����ҳ
					BOOL bEndPage = (!bAllInOne || (m == nTo) || (((m_nCurrentPage+1) % m_dwPagesPerPaper) == 0)) ? TRUE : FALSE;	//�Ƿ�һ��ֽ��������ҳ
					theLog.Write("print emf nFrom = %d, m = %d, bBeginPage=%d, bEndPage=%d",nFrom,m,bBeginPage,bEndPage);
					HENHMETAFILE hEmf = GetEmf(spl, m);
					if (hEmf == NULL)
					{
						theLog.Write("!!hEmf == NULL, pageindex=%d", m);
						goto ERR_PRINT;
					}
					ENHMETAHEADER emfheader;
					GetEnhMetaFileHeader(hEmf, sizeof(ENHMETAHEADER), &emfheader);
					Metafile mf(hEmf, TRUE);
					if (!bAllInOne)
					{
						ResetPrinterHDC(hdcPrinter, pDevmodeWork, &emfheader); //����HDC
					}

					if (bBeginPage)
					{
						nErrRet = StartPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}

						pGraphics = Graphics::FromHDC(hdcPrinter);
						pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
						pGraphics->SetInterpolationMode(InterpolationModeHighQuality); 
						pGraphics->SetPageUnit(UnitPixel);
						pGraphics->SetPageScale(1.0);
					}

					GraphicsState state = pGraphics->Save();

					Rect rc;
					GetPrintDeviceArea(hdcPrinter, mf, rc, *pGraphics);	//��ȡ��ӡ����
					int nPictureWidth = mf.GetWidth();
					int nPictureHeight = mf.GetHeight();
					theLog.Write("print emf rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
						,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

					if (IsWater())
					{
						CPrintWithWaterMarked water;
						water.Init(&mf,&m_oSec);
						water.DrawImg(&mf);
						pGraphics->DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}
					else
					{
						pGraphics->DrawImage(&mf, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}

					pGraphics->Restore(state);

					if (bEndPage)
					{
						pGraphics->ReleaseHDC(hdcPrinter);
						delete pGraphics;
						pGraphics = NULL;

						nErrRet = EndPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							theLog.Write("EndPage failed");
							goto ERR_PRINT;
						}

						nPrinted++;
						//�ص�֪ͨ���½��棬֪ͨ�������۷�
						if(pChargeCall)
						{
							pChargeCall->OnNewPagePrinted(m);//PrintEmfFile
						}
					}
					m_nCurrentPage++;
				}
			}
			else
			{
				theLog.Write("!!nFrom=%d,nTo=%d,dwPagePerCopy=%d", nFrom, nTo, dwPagePerCopy);
				ASSERT(0);
			}
		}
		else
		{
			theLog.Write("!!spl InitParse fail.sPath=[%s]", sPath);	
		}

		nErrRet = EndDoc(hdcPrinter);
		if (nErrRet <= 0)
		{
			goto ERR_PRINT;
		}

		if(pChargeCall)
		{
			pChargeCall->OnPrintedCmplt(nPrinted * m_dwCopies);//PrintEmfFile
		}
	}
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}
	if (pGraphics)
	{
		delete pGraphics;
	}

	return TRUE;

ERR_PRINT:
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}
	if (pGraphics)
	{
		delete pGraphics;
	}

	theLog.Write("print emf err:%d,prined:%d",nErrRet,nPrinted);
	if (pChargeCall)
	{
		pChargeCall->OnPrintedErr(nPrinted * m_dwCopies,nErrRet);
	}
	return FALSE;
}

BOOL CIOPrinter::PrintEmfFileBreakPoint(CString sPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom/* = -1*/,int nTo/* = -1*/)
{
	theLog.Write("CIOPrinter::PrintEmfFileBreakPoint,1,sPath=%s,sDocName=%s,nFrom=%d,nTo=%d",sPath,sDocName,nFrom,nTo);
	int nErrRet = SP_NOTREPORTED;
	int nPrinted = 0;
	HDC hdcPrinter = 0;
	ImageAttributes *pImgAtt = NULL;

	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintEmfFileBreakPoint,2,Printer=[%s]",m_sPrinter);

	DEVNAMES* pDevName = 0;
	BYTE* pDev = 0;
	if(!PrinterUtils::GetPrinterDevMode(m_sPrinter,pDevName,pDev))
	{
		theLog.Write("GetPrinterDevMode err");
		goto ERR_PRINT;
	}
	if (!pDev)
	{
		theLog.Write("GetPrinterDevMode pDev = null err");
		goto ERR_PRINT;
	}
	PDEVMODE pDevmodeWork = (PDEVMODE)pDev;

	//�ı䷽��
	if(pDevmodeWork->dmFields & DM_ORIENTATION)  
	{
		if (pDevmodeWork->dmOrientation == m_dwOrientation)
		{
			theLog.Write("the printer no change orientation %d",m_dwOrientation);
		}
		else
		{
			theLog.Write("change the orientation");
			pDevmodeWork->dmOrientation = m_dwOrientation;
		}
	}

	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		if (pDevmodeWork->dmColor == m_dwColor)
		{
			theLog.Write("the printer no change color %d",m_dwColor);
		}
		else
		{
			theLog.Write("change the color %d",m_dwColor);
			pDevmodeWork->dmColor = m_dwColor;
		}
	}
	else
	{
		theLog.Write("no support color Field, ForceMonochromePrint");
		m_bForceMonochromePrint = TRUE;
	}

	if (m_bForceMonochromePrint)
	{
		//һ������²���Ҫ����ɫ�ļ�ת���ɻҶ�ͼƬ,
		//����,�����ӡ�豸��֧�ֲ�ɫ���Ƶ�ʱ��,����ʹ��,
		if (m_dwColor == DMCOLOR_MONOCHROME)
		{
			theLog.Write("change the ImageAttributes to DMCOLOR_MONOCHROME");
			pImgAtt = new ImageAttributes();
			ASSERT(pImgAtt);
			pImgAtt->SetColorMatrix(&colorMatrix, ColorMatrixFlagsSkipGrays, ColorAdjustTypeDefault);
		}
	}

	//�ı�ֽ�Ŵ�С
	DM_PAPERSIZE;
	if (pDevmodeWork->dmFields & DM_PAPERSIZE)
	{
		if (pDevmodeWork->dmPaperSize  == m_dwPaperSize)
		{
			theLog.Write("the printer no change papersize %d",m_dwPaperSize);
		}
		else
		{
			theLog.Write("change the papersize %d",m_dwPaperSize);
			pDevmodeWork->dmPaperSize  = m_dwPaperSize;
		}
	}

	//ָ���Զ���ֽ�Ŵ�С��ӡ�ĵ�
	if ((m_dwPaperSize>=DMPAPER_USER) && (m_dwPaperWidth>=10) && (m_dwPaperLength>=10))
	{
		theLog.Write("�Զ���ֽ�Ŵ�ӡ m_dwPaperSize=%d,m_dwPaperWidth=%d,m_dwPaperLength=%d",m_dwPaperSize,m_dwPaperWidth, m_dwPaperLength);
		if (m_dwPaperWidth > m_dwPaperLength)
		{
			pDevmodeWork->dmOrientation = DMORIENT_LANDSCAPE;
			pDevmodeWork->dmPaperWidth = m_dwPaperLength;
			pDevmodeWork->dmPaperLength = m_dwPaperWidth;
		}
		else
		{
			pDevmodeWork->dmOrientation = DMORIENT_PORTRAIT;
			pDevmodeWork->dmPaperWidth = m_dwPaperWidth;
			pDevmodeWork->dmPaperLength = m_dwPaperLength;
		}
		pDevmodeWork->dmPaperSize = DMPAPER_USER;
		pDevmodeWork->dmFields |= DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH;
	}

	//˫���ӡ
	theLog.Write("dup = %d",pDevmodeWork->dmDuplex);
	if (DM_DUPLEX & pDevmodeWork->dmFields)
	{
		DMDUP_SIMPLEX;
		if (pDevmodeWork->dmDuplex == m_dwDuplex)
		{
			theLog.Write("the printer no change duplex %d",m_dwDuplex);
		}
		else
		{
			theLog.Write("change the duplex %d",m_dwDuplex);
			pDevmodeWork->dmDuplex = m_dwDuplex;
		}
	}

#if 0
	//�ı��ӡ����
	DM_COPIES;
	if (pDevmodeWork->dmFields & DM_COPIES)
	{
		if (pDevmodeWork->dmCopies == m_dwCopies)
		{
			theLog.Write("the printer no change copies %d",m_dwCopies);
		}
		else
		{
			theLog.Write("change the copies %d",m_dwCopies);
			pDevmodeWork->dmCopies = m_dwCopies;
		}
	}
#endif

	//��ݴ�ӡ
	DM_COLLATE;
	if (pDevmodeWork->dmFields & DM_COLLATE)
	{
		if (pDevmodeWork->dmCollate == m_dwCollate)
		{
			theLog.Write("the printer no change collate %d",m_dwCollate);
		}
		else
		{
			theLog.Write("change the collate %d",m_dwCollate);
			pDevmodeWork->dmCollate = m_dwCollate;
		}
	}

	//ѡ���ӡֽ����Դ
	DM_DEFAULTSOURCE;
	if (IsEnableDefaultSource() && (pDevmodeWork->dmFields & DM_DEFAULTSOURCE))
	{
		if (pDevmodeWork->dmDefaultSource == m_dwDefaultSource)
		{
			theLog.Write("the printer no change paper source %d",m_dwDefaultSource);
		}
		else
		{
			theLog.Write("change the paper source %d",m_dwDefaultSource);
			pDevmodeWork->dmDefaultSource = m_dwDefaultSource;
		}
	}

	hdcPrinter = ::CreateDC((LPCTSTR)pDevName + pDevName->wDriverOffset,
		(LPCTSTR)pDevName + pDevName->wDeviceOffset,
		(LPCTSTR)pDevName + pDevName->wOutputOffset,
		(DEVMODE*)pDev);
	if (hdcPrinter == NULL)
	{
		theLog.Write("hdcPrinter = null");
		goto ERR_PRINT;
	}
	else
	{		
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = "RAW";
		docinfo.lpszOutput = NULL;
		VerifyDocName(sDocName);
		if (sDocName.IsEmpty())
		{
			docinfo.lpszDocName = _T("Fragment");
		}
		else
		{
			docinfo.lpszDocName = sDocName;
		}

		// call StartDoc() to begin printing
		nErrRet = StartDoc(hdcPrinter, &docinfo);
		if (nErrRet <= 0)
		{
			SP_ERROR;			
			goto ERR_PRINT;
		}

		CParseSpl spl;
		if (spl.InitParse(sPath, TRUE))
		{
			int nCopy = m_dwCopies;
			int nFrom2 = 0;
			if (nCopy == 1)
			{
				nCopy = (m_nPrinted > 0) ? 0 : 1;
				nFrom2 = m_nPrinted;
			}
			else
			{
				int nPagePerCopy = nTo - nFrom + 1;
				int nTotalPage = nCopy * nPagePerCopy;
				int nLeftPage = nTotalPage - m_nPrinted;
				nCopy = nLeftPage / nPagePerCopy;
				int nLeftPage2 = nLeftPage - (nCopy * nPagePerCopy);
				nFrom2 = nPagePerCopy - nLeftPage2;
			}

			DWORD dwPagePerCopy = spl.GetPageCountPerCopy();
			if(nFrom >= 0 && nTo >= nFrom && nTo < dwPagePerCopy)
			{
				theLog.Write(_T("print emf from = %d to = %d ,copy = %d,nFrom2=%d"),nFrom,nTo,nCopy, nFrom2);
				for (int i=nFrom2; i <= nTo; i++)
				{
					theLog.Write(_T("print emf nFrom2 = %d, i = %d"), nFrom2, i);
					HENHMETAFILE hEmf = GetEmf(spl, i);
					if (hEmf == NULL)
					{
						theLog.Write("!!hEmf == NULL, pageindex=%d", i);
						goto ERR_PRINT;
					}
					ENHMETAHEADER emfheader;
					GetEnhMetaFileHeader(hEmf, sizeof(ENHMETAHEADER), &emfheader);
					Metafile mf(hEmf, TRUE);
					ResetPrinterHDC(hdcPrinter, pDevmodeWork, &emfheader); //����HDC

					nErrRet = StartPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}

					Graphics graphics(hdcPrinter);
					graphics.SetSmoothingMode(SmoothingModeHighQuality);
					graphics.SetInterpolationMode(InterpolationModeHighQuality); 
					graphics.SetPageUnit(UnitPixel);
					graphics.SetPageScale(1.0);

					Rect rc;
					GetPrintDeviceArea(hdcPrinter, mf, rc, graphics);	//��ȡ��ӡ����
					int nPictureWidth = mf.GetWidth();
					int nPictureHeight = mf.GetHeight();
					theLog.Write("print emf AA rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
						,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

					if (IsWater())
					{
						CPrintWithWaterMarked water;
						water.Init(&mf,&m_oSec);
						water.DrawImg(&mf);
						graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}
					else
					{
						graphics.DrawImage(&mf, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
					}

					graphics.ReleaseHDC(hdcPrinter);

					nErrRet = EndPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}
					nPrinted++;
					//�ص�֪ͨ���½��棬֪ͨ�������۷�
					if(pChargeCall)
					{
						pChargeCall->OnNewPagePrinted(nFrom2);//PrintEmfFileBreakPoint
					}
				}			
				for (int x = 0; x < nCopy; x++ )
				{
					theLog.Write(_T("print emf from = %d to = %d ,x = %d"),nFrom,nTo,x);
					for (int m = nFrom; m <= nTo; m ++)
					{
						theLog.Write(_T("print emf nFrom = %d, m = %d"), nFrom, m);
						HENHMETAFILE hEmf = GetEmf(spl, m);
						if (hEmf == NULL)
						{
							theLog.Write("!!hEmf == NULL, pageindex=%d", m);
							goto ERR_PRINT;
						}
						ENHMETAHEADER emfheader;
						GetEnhMetaFileHeader(hEmf, sizeof(ENHMETAHEADER), &emfheader);
						Metafile mf(hEmf, TRUE);
						ResetPrinterHDC(hdcPrinter, pDevmodeWork, &emfheader); //����HDC

						nErrRet = StartPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}

						Graphics graphics(hdcPrinter);
						graphics.SetSmoothingMode(SmoothingModeHighQuality);
						graphics.SetInterpolationMode(InterpolationModeHighQuality); 
						graphics.SetPageUnit(UnitPixel);
						graphics.SetPageScale(1.0);

						Rect rc;
						GetPrintDeviceArea(hdcPrinter, mf, rc, graphics);	//��ȡ��ӡ����
						int nPictureWidth = mf.GetWidth();
						int nPictureHeight = mf.GetHeight();
						theLog.Write("print emf BB rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
							,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

						if (IsWater())
						{
							CPrintWithWaterMarked water;
							water.Init(&mf,&m_oSec);
							water.DrawImg(&mf);
							graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
						}
						else
						{
							graphics.DrawImage(&mf, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel, pImgAtt);
						}

						graphics.ReleaseHDC(hdcPrinter);

						nErrRet = EndPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}
						nPrinted++;
						//�ص�֪ͨ���½��棬֪ͨ�������۷�
						if(pChargeCall)
						{
							pChargeCall->OnNewPagePrinted(m);//PrintEmfFileBreakPoint
						}
					}
				}
			}
			else
			{
				theLog.Write("!!nFrom=%d,nTo=%d,dwPagePerCopy=%d", nFrom, nTo, dwPagePerCopy);
				ASSERT(0);
			}				
		}
		else
		{
			theLog.Write("!!spl InitParse fail.sPath=[%s]", sPath);	
		}

		nErrRet = EndDoc(hdcPrinter);
		if (nErrRet <= 0)
		{
			goto ERR_PRINT;
		}

		if(pChargeCall)
		{
			pChargeCall->OnPrintedCmplt(nPrinted);//PrintEmfFileBreakPoint
		}
	}
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}
	return TRUE;

ERR_PRINT:
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImgAtt)
	{
		delete pImgAtt;
	}

	theLog.Write("print emf err:%d,prined:%d",nErrRet,nPrinted);
	if (pChargeCall)
	{
		pChargeCall->OnPrintedErr(nPrinted,nErrRet);
	}
	return FALSE;
}


HENHMETAFILE CIOPrinter::GetEmf(CParseSpl& spl, DWORD dwPageIndex)
{
#ifdef ENABLE_MEMORY_MAP_PARSE_SPL
	HENHMETAFILE hEMF = NULL;
	PEMF_DATA emf = spl.GetEmfDataByPageIndex(dwPageIndex);
	if (emf && emf->pData)
	{
		hEMF = SetEnhMetaFileBits(emf->dwSize, emf->pData);
	}
	else
	{
		theLog.Write("!!CIOPrinter::GetEmf,1,fail,emf=%p, pData=%p", emf, (emf!=NULL)?emf->pData:NULL);
	}
	return hEMF;
#else
	return spl.GetEmfDataByPageIndex(dwPageIndex);
#endif
}

//��ȡָ��������PDFͼƬ����
Image* CIOPrinter::GetPdf(CPdfHelper& pdf, DWORD dwPageIndex, BOOL bColor)
{
	Image* pImage = NULL;
	IStream* pIStream = NULL;
	HRESULT hr = S_OK;
	Status stat = Ok;
	double hDPI = 300.0;
	double vDPI = 300.0;
	ImageColorMode colorMode = bColor ? imageModeBGR8 : imageModeMono8;
	int nWidth = 0;
	int nHeight = 0;
	int nStride = 0;
	unsigned char *scan0 = NULL;
	if (pdf.GetBitmap(dwPageIndex, hDPI, vDPI, colorMode, nWidth, nHeight, nStride, &scan0))
	{
		int nStep = 1;
		WORD biBitCount = 32;
		if (colorMode == imageModeMono1)
		{
			biBitCount = 1;
			nStep = 255;
		}
		else if (colorMode == imageModeMono8)
		{
			biBitCount = 8;
			nStep = 1;
		}
		else if ((colorMode == imageModeRGB8) || (colorMode == imageModeBGR8))
		{
			biBitCount = 24;
		}
		else if (colorMode == imageModeXBGR8)
		{
			biBitCount = 32;
		}

		BITMAPINFO *pBi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		memset(pBi, 0x0, sizeof(BITMAPINFO));
		pBi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBi->bmiHeader.biWidth = nWidth;
		pBi->bmiHeader.biHeight = (nStride>0) ? -nHeight : nHeight;	// If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. 
		//If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner. 
		pBi->bmiHeader.biSizeImage = 0;
		pBi->bmiHeader.biXPelsPerMeter = hDPI / 0.0254;
		pBi->bmiHeader.biYPelsPerMeter = vDPI / 0.0254;
		pBi->bmiHeader.biPlanes = 1;
		pBi->bmiHeader.biBitCount = biBitCount;
		pBi->bmiHeader.biCompression = BI_RGB;
		pBi->bmiHeader.biClrUsed = 0;
		pBi->bmiHeader.biClrImportant = 0;
		if (biBitCount <= 8)
		{
			int nColorCount = 1 << biBitCount;
			unsigned char* pPalette = (unsigned char*)pBi + sizeof(BITMAPINFOHEADER);
			for (int i=0; i<nColorCount; i++)
			{
				int nIndex = i * sizeof(RGBQUAD);
				int nColor = i * nStep;
				//nColor = (nColor>255) ? 255 : nColor;
				pPalette[nIndex] = nColor;
				pPalette[nIndex+1] = nColor;
				pPalette[nIndex+2] = nColor;
				pPalette[nIndex+3] = 0;
			}
		}
		Bitmap bitmap(pBi, scan0);
		free(pBi);
		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);
		hr = CreateStreamOnHGlobal(NULL, TRUE, &pIStream);
		if(FAILED(hr))
		{
			theLog.Write("!!CIOPrinter::GetPdf,1,fail,err=%u", GetLastError());
			goto Exit;
		}
		stat = bitmap.Save(pIStream, &bmpClsid);
		if (stat != Ok)
		{
			theLog.Write("!!CIOPrinter::GetPdf,2,fail,stat=%d", stat);
			goto Exit;
		}
		pImage = new Image(pIStream);
	}
	else
	{
		theLog.Write("!!CIOPrinter::GetPdf,3,fail");
		goto Exit;
	}

Exit:
	if (scan0)
		pdf.FreeBitmapData(scan0);
	if(pIStream)
		pIStream->Release();

	return pImage;
}

void CIOPrinter::GetPrintDeviceArea(HDC& hdcPrinter, Image& image, Rect& printRect, Graphics& graphics, BOOL bUsePhysicalArea /*= FALSE*/)
{
	int nImageWidth = image.GetWidth();
	int nImageHeight = image.GetHeight();
	int nDeviceHORZRES = GetDeviceCaps(hdcPrinter, HORZRES);			//�ɴ�ӡҳ����(��λ:pixels)
	int nDeviceVERTRES = GetDeviceCaps(hdcPrinter, VERTRES);			//�ɴ�ӡҳ��߶�(��λ:pixels)
	int nDeviceWidth = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);		//������(�豸��λ)
	int nDeviceHeight = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT);		//����߶�(�豸��λ)
	int nDeviceOffSetX = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETX);	//���Ͻ�xƫ��(�豸��λ)
	int nDeviceOffSetY = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETY);	//���Ͻ�yƫ��(�豸��λ)
	int nScalingFactorX = GetDeviceCaps(hdcPrinter, SCALINGFACTORX);	//Scaling factor x
	int nScalingFactorY = GetDeviceCaps(hdcPrinter, SCALINGFACTORY);	//Scaling factor y
	theLog.Write("CIOPrinter::GetPrintDeviceArea,nDeviceHORZRES=%d,nDeviceVERTRES=%d,nDeviceWidth=%d,nDeviceHeight=%d,"
					"nDeviceOffSetX=%d,nDeviceOffSetY=%d,nScalingFactorX=%d, nScalingFactorY=%d, nImageWidth=%d, nImageHeight=%d"
					, nDeviceHORZRES, nDeviceVERTRES, nDeviceWidth, nDeviceHeight
					, nDeviceOffSetX, nDeviceOffSetY, nScalingFactorX, nScalingFactorY
					, nImageWidth, nImageHeight);

	if (bUsePhysicalArea)
	{
		printRect.X = -nDeviceOffSetX;
		printRect.Y = -nDeviceOffSetY;
		printRect.Width = nDeviceWidth;
		printRect.Height = nDeviceHeight;
	}
	else
	{
		printRect.X = 0;
		printRect.Y = 0;
		printRect.Width = nDeviceHORZRES;
		printRect.Height = nDeviceVERTRES;
	}

	Matrix matrix;
	matrix.Translate(printRect.X, printRect.Y);

	//��ȡ���һ��ӡ����
	AllPageInOneParam param;
	memset(&param, 0x0, sizeof(AllPageInOneParam));
	GetAllPageInOneParam(printRect, image, param);

	matrix.Translate(param.x_offset, param.y_offset);
	matrix.Rotate(param.rotate_radians);

	double fScaleWidth = 1.0 * printRect.Width / nImageWidth;
	double fScaleHeight = 1.0 * printRect.Height / nImageHeight;
	double fScale = (fScaleWidth < fScaleHeight) ? fScaleWidth : fScaleHeight;

	matrix.Translate((printRect.Width - nImageWidth*fScale) / 2, (printRect.Height - nImageHeight*fScale) / 2);
	matrix.Scale(fScale, fScale);
	graphics.SetTransform(&matrix);

	printRect.X = 0;	//����ͼ��ԭ�㶨Ϊ��0,0��������������ԭ�㡣
	printRect.Y = 0;
	printRect.Width = nImageWidth;	//��ӡ��ΧΪͼƬ�Ŀ��
	printRect.Height = nImageHeight;

	theLog.Write("CIOPrinter::GetPrintDeviceArea,2,OffsetX=%0.2f, OffsetY=%0.2f,"
		"param.x_offset=%0.2f, param.y_offset=%0.2f, param.rotate_radians=%0.2f"
		, matrix.OffsetX(), matrix.OffsetY()
		, param.x_offset, param.y_offset, param.rotate_radians);
}

//����HDC
void CIOPrinter::ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, PENHMETAHEADER pEmfHeader)
{
	if (!pDevMode && !pEmfHeader)
	{
		theLog.Write("!!CIOPrinter::ResetPrinterHDC,1,pDevMode=%p,pEmfHeader=%p", pDevMode, pEmfHeader);
		return;
	}

	WORD wdPaperSize = DMPAPER_A4;
	WORD wdOrientation = DMORIENT_PORTRAIT;
	if (m_bAutoPaperSize)
	{
		CPrinterPaperSize::GetInstance().GetPaperInfoByEmfHeader(m_sPrinter.GetString(), pEmfHeader, wdPaperSize, wdOrientation);
		if ((DMPAPER_A4 != wdPaperSize) && (DMPAPER_A3 != wdPaperSize) && m_bUseA4Print)
		{
			theLog.Write("##CIOPrinter::ResetPrinterHDC,wdPaperSize [%d] is not support,changeto DMPAPER_A4.", wdPaperSize);
			wdPaperSize = DMPAPER_A4;
		}
	}
	else
	{
		wdPaperSize = pDevMode->dmPaperSize;	//�����Ĵ�ӡֽ��
		wdOrientation = (pEmfHeader->szlMillimeters.cx > pEmfHeader->szlMillimeters.cy) ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;	//��ӡ����
	}

	if (pDevMode->dmOrientation != wdOrientation || pDevMode->dmPaperSize != wdPaperSize)
	{
		pDevMode->dmOrientation = wdOrientation;
		if (pDevMode->dmPaperSize == 0 || pDevMode->dmPaperSize >= DMPAPER_USER)	
		{
			//dmPaperSizeΪ0���ߴ��ڵ���DMPAPER_USER(256)����ʾ���Զ���ֽ�ʹ�С���˴���Ҫ����dmPaperSize,����ԭʼֵ
		}
		else
		{
			pDevMode->dmPaperSize = wdPaperSize;
		}
		hdcPrinter = ResetDC(hdcPrinter, pDevMode);
	}
}

//����HDC
void CIOPrinter::ResetPrinterHDC(HDC& hdcPrinter, PDEVMODE pDevMode, Image& image)
{
	if (!pDevMode)
	{
		theLog.Write("!!CIOPrinter::ResetPrinterHDC,1,pDevMode=%p", pDevMode);
		return;
	}

	WORD wdPaperSize = DMPAPER_A4;
	WORD wdOrientation = DMORIENT_PORTRAIT;
	if (m_bAutoPaperSize)
	{
		CPrinterPaperSize::GetInstance().GetPaperInfoByImage(m_sPrinter.GetString(), image, wdPaperSize, wdOrientation);
		if ((DMPAPER_A4 != wdPaperSize) && (DMPAPER_A3 != wdPaperSize) && m_bUseA4Print)
		{
			theLog.Write("##CIOPrinter::ResetPrinterHDC,wdPaperSize [%d] is not support,changeto DMPAPER_A4.", wdPaperSize);
			wdPaperSize = DMPAPER_A4;
		}
	}
	else
	{
		wdPaperSize = pDevMode->dmPaperSize;	//�����Ĵ�ӡֽ��
		wdOrientation = (image.GetWidth() > image.GetHeight()) ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;	//��ӡ����
	}

	if (pDevMode->dmOrientation != wdOrientation || pDevMode->dmPaperSize != wdPaperSize)
	{
		//pDevMode->dmOrientation = wdOrientation;
		if (pDevMode->dmPaperSize == 0 || pDevMode->dmPaperSize >= DMPAPER_USER)	
		{
			//dmPaperSizeΪ0���ߴ��ڵ���DMPAPER_USER(256)����ʾ���Զ���ֽ�ʹ�С���˴���Ҫ����dmPaperSize,����ԭʼֵ
		}
		else
		{
			pDevMode->dmPaperSize = wdPaperSize;
		}
		hdcPrinter = ResetDC(hdcPrinter, pDevMode);
	}
}

BOOL CIOPrinter::IsXpsFileFormat(CString szFilePath)
{
	BOOL bRet = FALSE;
	if (!PathFileExists(szFilePath))
	{
		printf("!!CIOPrinter::IsXpsFileFormat,1,PathFileExists fail,szFilePath=[%s]", szFilePath);
		return bRet;
	}

	try
	{
		CFile file;
		if(!file.Open(szFilePath,CFile::modeRead))
		{
			printf("!!CIOPrinter::IsXpsFileFormat,2,openfile fail,szFilePath=[%s],err=%u", szFilePath, GetLastError());
			return bRet;
		}
		else
		{
			DWORD dwFileSign = 0;
			DWORD dwRead = file.Read(&dwFileSign, sizeof(DWORD));
			if (dwFileSign == 0x04034B50)	//xps�ļ����ļ���ͷ�ĸ��ֽ���504B0304
			{
				bRet = TRUE;
			}
			file.Close();
		}
	}
	catch (CException* e)
	{
	}

	return bRet;
}

void CIOPrinter::VerifyDocName(CString& szDocName)		//��֤�ĵ����Ϸ���
{
	szDocName.Trim();
	int nDocNameLen = szDocName.GetLength();
	if (nDocNameLen >= MAX_PATH)
	{
		int nLen = (MAX_PATH / 2) - 1;
		szDocName.Format("%s~%s", szDocName.Left(nLen), szDocName.Right(nLen));
	}
}

void WINAPI CIOPrinter::PrintProcessCallBack(PrintProcessCallBackData* pCallBackData)
{
	if (pCallBackData)
	{
		int nPrintedPage = pCallBackData->nPrintedPage;	//�˴�����1Ϊ��ʼ����
		BOOL bPrintComplete = pCallBackData->bPrintComplete;
		BOOL bError = pCallBackData->bError;
		IPrtStationPrintCallback* pCall = (IPrtStationPrintCallback*)pCallBackData->pData;
		if (pCall)
		{
			if (bError && bPrintComplete)
			{
				pCall->OnPrintedErr(nPrintedPage,0);
			}
			else if (!bError && bPrintComplete)
			{
				pCall->OnPrintedCmplt(nPrintedPage);//PrintProcessCallBack
			}
			else
			{
				pCall->OnNewPagePrinted(nPrintedPage-1);//�˴�����0Ϊ��ʼ����PrintProcessCallBack
			}
		}
	}
	else
	{
		theLog.Write("##PrintProcessCallBack,pCallBackData=%p", pCallBackData);
	}
}

//add in 2016.04.18
BOOL CIOPrinter::PrintPdfFile(CString szPdfPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom /*= -1*/,int nTo /*= -1*/)
{
	if (m_nPrinted > 0)
	{
		return PrintPdfFileBreakPoint(szPdfPath, sDocName, pChargeCall, nFrom, nTo);
	}
	theLog.Write("CIOPrinter::PrintPdfFile,1,szPdfPath=%s,sDocName=%s,nFrom=%d,nTo=%d",szPdfPath,sDocName,nFrom,nTo);
	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintPdfFile,2,Printer=[%s]",m_sPrinter);

	BOOL bForceGray = FALSE;
#if 0	//
	if ((m_eType == BT_SL) || (m_eType == BT_SL2))
	{
		if (m_dwColor == DMCOLOR_MONOCHROME)
		{
			bForceGray = TRUE;	//Ŀǰ��ֻ��ʩ�ִ�ӡ���޷����ƺڰ׺Ͳ�ɫ���˴�ǿ��ʹ�úڰ״�ӡ
		}
	}
#else
	if (m_dwColor == DMCOLOR_MONOCHROME)
	{
		bForceGray = TRUE;
	}
#endif

	BOOL bRet = FALSE;
	CPdfHelper helper;
	if (helper.OpenFile(szPdfPath.GetString()))
	{
		PrintProcessCallBackData* pCallBackData = new PrintProcessCallBackData;
		memset(pCallBackData, 0x0, sizeof(PrintProcessCallBackData));
		pCallBackData->pData = pChargeCall;
		VerifyDocName(sDocName);
		BOOL bUseCropBox = FALSE;
		BOOL bUseFullPage = TRUE;
		BOOL bUsePDFPageSize = m_bAutoPaperSize;
		WaterInfo* pWater = NULL;
		if (IsWater())
		{
			pWater = new WaterInfo;
			strcpy_s(pWater->Header, m_oSec.header.GetString());
			strcpy_s(pWater->Footer, m_oSec.footer.GetString());
			strcpy_s(pWater->Water, m_oSec.water.GetString());
		}

		bRet = helper.PrintToPrinter(m_sPrinter.GetString(), sDocName.GetString(), nFrom, nTo
			, m_dwPagesPerPaper, m_dwColor, m_dwPaperSize, m_dwOrientation, m_dwCopies, m_dwCollate, m_dwDuplex
			, m_dwDefaultSource, m_dwPaperWidth, m_dwPaperLength, bUseCropBox, bUseFullPage
			, bUsePDFPageSize, bForceGray, pWater, PrintProcessCallBack, pCallBackData);
		delete pCallBackData;
		if (pWater)
		{
			delete pWater;
		}
		if (!bRet)
		{
			theLog.Write("!!CIOPrinter::PrintPdfFile,3,PrintToPrinter fail");
		}
	}
	else
	{
		theLog.Write("!!CIOPrinter::PrintPdfFile,4,OpenFile fail");
	}
	helper.CloseFile();

	theLog.Write("CIOPrinter::PrintPdfFile,5");

	return bRet;
}

BOOL CIOPrinter::PrintPdfFileBreakPoint(CString szPdfPath,CString sDocName,IPrtStationPrintCallback* pChargeCall,int nFrom /*= -1*/,int nTo /*= -1*/)
{
	theLog.Write("CIOPrinter::PrintPdfFileBreakPoint,1,szPdfPath=%s,sDocName=%s,nFrom=%d,nTo=%d",szPdfPath,sDocName,nFrom,nTo);
	int nErrRet = SP_NOTREPORTED;
	int nPrinted = 0;
	HDC hdcPrinter = 0;

	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}
	theLog.Write("CIOPrinter::PrintPdfFileBreakPoint,2,Printer=[%s]",m_sPrinter);

	Image* pImage = NULL;
	DEVNAMES* pDevName = 0;
	BYTE* pDev = 0;
	if(!PrinterUtils::GetPrinterDevMode(m_sPrinter,pDevName,pDev))
	{
		theLog.Write("CIOPrinter::PrintPdfFileBreakPoint,GetPrinterDevMode err");
		goto ERR_PRINT;
	}
	if (!pDev)
	{
		theLog.Write("CIOPrinter::PrintPdfFileBreakPoint,GetPrinterDevMode pDev = null err");
		goto ERR_PRINT;
	}
	PDEVMODE pDevmodeWork = (PDEVMODE)pDev;

	//�ı䷽��
	if(pDevmodeWork->dmFields & DM_ORIENTATION)  
	{
		if (pDevmodeWork->dmOrientation == m_dwOrientation)
		{
			theLog.Write("the printer no change orientation %d",m_dwOrientation);
		}
		else
		{
			theLog.Write("change the orientation");
			pDevmodeWork->dmOrientation = m_dwOrientation;
		}
	}

	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pDevmodeWork->dmFields & DM_COLOR)
	{
		if (pDevmodeWork->dmColor == m_dwColor)
		{
			theLog.Write("the printer no change color %d",m_dwColor);
		}
		else
		{
			theLog.Write("change the color %d",m_dwColor);
			pDevmodeWork->dmColor = m_dwColor;
		}
	}
	else
	{
		theLog.Write("no support color Field");
	}

	//�ı�ֽ�Ŵ�С
	DM_PAPERSIZE;
	if (pDevmodeWork->dmFields & DM_PAPERSIZE)
	{
		if (pDevmodeWork->dmPaperSize  == m_dwPaperSize)
		{
			theLog.Write("the printer no change papersize %d",m_dwPaperSize);
		}
		else
		{
			theLog.Write("change the papersize %d",m_dwPaperSize);
			pDevmodeWork->dmPaperSize  = m_dwPaperSize;
		}
	}

	//ָ���Զ���ֽ�Ŵ�С��ӡ�ĵ�
	if ((m_dwPaperSize>=DMPAPER_USER) && (m_dwPaperWidth>=10) && (m_dwPaperLength>=10))
	{
		theLog.Write("�Զ���ֽ�Ŵ�ӡ m_dwPaperSize=%d,m_dwPaperWidth=%d,m_dwPaperLength=%d",m_dwPaperSize,m_dwPaperWidth, m_dwPaperLength);
		if (m_dwPaperWidth > m_dwPaperLength)
		{
			pDevmodeWork->dmOrientation = DMORIENT_LANDSCAPE;
			pDevmodeWork->dmPaperWidth = m_dwPaperLength;
			pDevmodeWork->dmPaperLength = m_dwPaperWidth;
		}
		else
		{
			pDevmodeWork->dmOrientation = DMORIENT_PORTRAIT;
			pDevmodeWork->dmPaperWidth = m_dwPaperWidth;
			pDevmodeWork->dmPaperLength = m_dwPaperLength;
		}
		pDevmodeWork->dmPaperSize = DMPAPER_USER;
		pDevmodeWork->dmFields |= DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH;
	}

	//˫���ӡ
	theLog.Write("dup = %d",pDevmodeWork->dmDuplex);
	if (DM_DUPLEX & pDevmodeWork->dmFields)
	{
		DMDUP_SIMPLEX;
		if (pDevmodeWork->dmDuplex == m_dwDuplex)
		{
			theLog.Write("the printer no change duplex %d",m_dwDuplex);
		}
		else
		{
			theLog.Write("change the duplex %d",m_dwDuplex);
			pDevmodeWork->dmDuplex = m_dwDuplex;
		}
	}

#if 0
	//�ı��ӡ����
	DM_COPIES;
	if (pDevmodeWork->dmFields & DM_COPIES)
	{
		if (pDevmodeWork->dmCopies == m_dwCopies)
		{
			theLog.Write("the printer no change copies %d",m_dwCopies);
		}
		else
		{
			theLog.Write("change the copies %d",m_dwCopies);
			pDevmodeWork->dmCopies = m_dwCopies;
		}
	}
#endif

	//��ݴ�ӡ
	DM_COLLATE;
	if (pDevmodeWork->dmFields & DM_COLLATE)
	{
		if (pDevmodeWork->dmCollate == m_dwCollate)
		{
			theLog.Write("the printer no change collate %d",m_dwCollate);
		}
		else
		{
			theLog.Write("change the collate %d",m_dwCollate);
			pDevmodeWork->dmCollate = m_dwCollate;
		}
	}

	//ѡ���ӡֽ����Դ
	DM_DEFAULTSOURCE;
	if (IsEnableDefaultSource() && (pDevmodeWork->dmFields & DM_DEFAULTSOURCE))
	{
		if (pDevmodeWork->dmDefaultSource == m_dwDefaultSource)
		{
			theLog.Write("the printer no change paper source %d",m_dwDefaultSource);
		}
		else
		{
			theLog.Write("change the paper source %d",m_dwDefaultSource);
			pDevmodeWork->dmDefaultSource = m_dwDefaultSource;
		}
	}

	hdcPrinter = ::CreateDC((LPCTSTR)pDevName + pDevName->wDriverOffset,
		(LPCTSTR)pDevName + pDevName->wDeviceOffset,
		(LPCTSTR)pDevName + pDevName->wOutputOffset,
		(DEVMODE*)pDev);
	if (hdcPrinter == NULL)
	{
		theLog.Write("hdcPrinter = null");
		goto ERR_PRINT;
	}
	else
	{		
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszOutput = NULL;
		VerifyDocName(sDocName);
		if (sDocName.IsEmpty())
		{
			docinfo.lpszDocName = _T("Fragment");
		}
		else
		{
			docinfo.lpszDocName = sDocName;
		}

		// call StartDoc() to begin printing
		nErrRet = StartDoc(hdcPrinter, &docinfo);
		if (nErrRet <= 0)
		{
			SP_ERROR;			
			goto ERR_PRINT;
		}

		CPdfHelper pdf;
		if (pdf.OpenFile(szPdfPath.GetString()))
		{
			int nCopy = m_dwCopies;
			int nFrom2 = 0;
			if (nCopy == 1)
			{
				nCopy = (m_nPrinted > 0) ? 0 : 1;
				nFrom2 = m_nPrinted;
			}
			else
			{
				int nPagePerCopy = nTo - nFrom + 1;
				int nTotalPage = nCopy * nPagePerCopy;
				int nLeftPage = nTotalPage - m_nPrinted;
				nCopy = nLeftPage / nPagePerCopy;
				int nLeftPage2 = nLeftPage - (nCopy * nPagePerCopy);
				nFrom2 = nPagePerCopy - nLeftPage2;
			}

			BOOL bColor = (m_dwColor == DMCOLOR_COLOR) ? TRUE : FALSE;
			DWORD dwPagePerCopy = pdf.GetPageCount();
			if(nFrom >= 0 && nTo >= nFrom && nTo < dwPagePerCopy)
			{
				theLog.Write(_T("print pdf from = %d to = %d ,copy = %d,nFrom2=%d"),nFrom,nTo,nCopy, nFrom2);
				for (int i=nFrom2; i <= nTo; i++)
				{
					theLog.Write(_T("print pdf nFrom2 = %d, i = %d"), nFrom2, i);
					pImage = GetPdf(pdf, i, bColor);
					if (pImage == NULL)
					{
						theLog.Write("!!pImage == NULL, pageindex=%d", i);
						goto ERR_PRINT;
					}
					ResetPrinterHDC(hdcPrinter, pDevmodeWork, *pImage); //����HDC

					nErrRet = StartPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}

					Graphics graphics(hdcPrinter);
					graphics.SetSmoothingMode(SmoothingModeHighQuality);
					graphics.SetInterpolationMode(InterpolationModeHighQuality); 
					graphics.SetPageUnit(UnitPixel);
					graphics.SetPageScale(1.0);

					Rect rc;
					GetPrintDeviceArea(hdcPrinter, *pImage, rc, graphics, TRUE);	//��ȡ��ӡ����,PDF��������������
					int nPictureWidth = pImage->GetWidth();
					int nPictureHeight = pImage->GetHeight();
					theLog.Write("print pdf AA rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
						,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

					if (IsWater())
					{
						CPrintWithWaterMarked water;
						water.Init(pImage,&m_oSec);
						water.DrawImg(pImage);
						graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel);
					}
					else
					{
						graphics.DrawImage(pImage, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel);
					}

					delete pImage;
					pImage = NULL;

					graphics.ReleaseHDC(hdcPrinter);

					nErrRet = EndPage(hdcPrinter);
					if (nErrRet <= 0)
					{
						goto ERR_PRINT;
					}
					nPrinted++;
					//�ص�֪ͨ���½��棬֪ͨ�������۷�
					if(pChargeCall)
					{
						pChargeCall->OnNewPagePrinted(nFrom2);//PrintPdfFileBreakPoint
					}
				}			
				for (int x = 0; x < nCopy; x++ )
				{
					theLog.Write(_T("print pdf from = %d to = %d ,x = %d"),nFrom,nTo,x);
					for (int m = nFrom; m <= nTo; m ++)
					{
						theLog.Write(_T("print pdf nFrom = %d, m = %d"), nFrom, m);
						pImage = GetPdf(pdf, m, bColor);
						if (pImage == NULL)
						{
							theLog.Write("!!pImage == NULL, pageindex=%d", m);
							goto ERR_PRINT;
						}
						
						ResetPrinterHDC(hdcPrinter, pDevmodeWork, *pImage); //����HDC

						nErrRet = StartPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}

						Graphics graphics(hdcPrinter);
						graphics.SetSmoothingMode(SmoothingModeHighQuality);
						graphics.SetInterpolationMode(InterpolationModeHighQuality); 
						graphics.SetPageUnit(UnitPixel);
						graphics.SetPageScale(1.0);

						Rect rc;
						GetPrintDeviceArea(hdcPrinter, *pImage, rc, graphics, TRUE);	//��ȡ��ӡ����,PDF��������������
						int nPictureWidth = pImage->GetWidth();
						int nPictureHeight = pImage->GetHeight();
						theLog.Write("print pdf BB rc.X=%d, rc.Y=%d, rc.Width=%d, rc.Height=%d, nPictureWidth=%d, nPictureHeight=%d"
							,rc.X, rc.Y, rc.Width, rc.Height, nPictureWidth, nPictureHeight);

						if (IsWater())
						{
							CPrintWithWaterMarked water;
							water.Init(pImage,&m_oSec);
							water.DrawImg(pImage);
							graphics.DrawImage(water.GetMarkedImg(), rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel);
						}
						else
						{
							graphics.DrawImage(pImage, rc, 0, 0, nPictureWidth, nPictureHeight, UnitPixel);
						}

						delete pImage;
						pImage = NULL;

						graphics.ReleaseHDC(hdcPrinter);

						nErrRet = EndPage(hdcPrinter);
						if (nErrRet <= 0)
						{
							goto ERR_PRINT;
						}
						nPrinted++;
						//�ص�֪ͨ���½��棬֪ͨ�������۷�
						if(pChargeCall)
						{
							pChargeCall->OnNewPagePrinted(m);//PrintPdfFileBreakPoint 
						}
					}
				}
			}
			else
			{
				theLog.Write("!!nFrom=%d,nTo=%d,dwPagePerCopy=%d", nFrom, nTo, dwPagePerCopy);
				ASSERT(0);
			}				
		}
		else
		{
			theLog.Write("!!pdf OpenFile fail.szPdfPath=[%s]", szPdfPath);	
		}

		nErrRet = EndDoc(hdcPrinter);
		if (nErrRet <= 0)
		{
			goto ERR_PRINT;
		}

		if(pChargeCall)
		{
			pChargeCall->OnPrintedCmplt(nPrinted);//PrintPdfFileBreakPoint
		}
	}
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImage)
	{
		delete pImage;
	}
	
	return TRUE;

ERR_PRINT:
	if(hdcPrinter)
	{
		::DeleteDC(hdcPrinter);
		hdcPrinter = 0;
	}
	if (pDev)
	{
		delete pDev;
	}
	if (pDevName)
	{
		delete pDevName;
	}
	if (pImage)
	{
		delete pImage;
	}

	theLog.Write("print pdf err:%d,prined:%d",nErrRet,nPrinted);
	if (pChargeCall)
	{
		pChargeCall->OnPrintedErr(nPrinted,nErrRet);
	}
	return FALSE;
}

//add in 2013.06.13
BOOL CIOPrinter::PrintSplFile(CString szSplPath,CString sDocName, IPrtStationPrintCallback* pChargeCall, int nTotalPage)
{
	if (m_sPrinter.IsEmpty())
	{
		m_sPrinter = GetDefault();
	}

	int nLen = szSplPath.GetLength();
	if(4 >= nLen || !pChargeCall || m_sPrinter.IsEmpty())
	{
		theLog.Write("!!CIOPrinter::PrintSplFile,1,pChargeCall=0x%x,sDocName=[%s],m_sPrinter=[%s],szSplPath=[%s],m_sPrinter=[%s]"
			, pChargeCall, sDocName, m_sPrinter, szSplPath, m_sPrinter);
		return FALSE;
	}

	CString szExtName = szSplPath.Right(4);
	if(0 == szExtName.CompareNoCase(".spl"))
	{//��ӡspl
		theLog.Write("==CIOPrinter::PrintSplFile,2,PrintOther,m_sPrinter=[%s]",m_sPrinter);

		if (m_bForceXpsDriverPrint && IsXpsFileFormat(szSplPath))
		{
			CString szPrinterNameXps;
			szPrinterNameXps.Format("%s_XPS", m_sPrinter);
			m_sPrinter = szPrinterNameXps;
			theLog.Write("##CIOPrinter::PrintSplFile,BB,IsXpsFileFormat,new m_sPrinter=[%s]", m_sPrinter);
		}

		if(PrintSpl(m_sPrinter, szSplPath))
		{//��ӡ�ɹ�
			theLog.Write("==CIOPrinter::PrintSplFile,22222222,PrintOther,m_sPrinter=[%s]",m_sPrinter);

			if(pChargeCall)
			{
				pChargeCall->OnPrintedCmplt(nTotalPage);//PrintSplFile
			}
			return TRUE;
		}
		else
		{//��ӡʧ��
			int nPrinted = 0;
			if(pChargeCall)
			{
				pChargeCall->OnPrintedErr(nPrinted, -1);
			}
			return FALSE;
		}
	}//if(0 == szExtName.CompareNoCase(".spl"))
	
	theLog.Write("!!CIOPrinter::PrintSplFile,12,m_sPrinter=[%s],szSplPath=[%s]", m_sPrinter, szSplPath);
	return FALSE;
}

BOOL CopySplFile(CString szSource, CString szDest)
{
	if(szSource.IsEmpty() || szDest.IsEmpty())
	{
		theLog.Write("!!CIOPrinter::CopySplFile,1,szSource=%s,szDest=%s", szSource, szDest);
		return FALSE;
	}
	if(!::PathFileExists(szSource))
	{
		theLog.Write("!!CIOPrinter::CopySplFile,2,NOT EXISTS,szSource=%s,", szSource);
		return FALSE;
	}

	return CopyFile(szSource, szDest, FALSE);	//add by zxl,20131029

	CFile oFileSource;
	CFile oFileDest;
	DWORD dwFileLen = 0;
	DWORD dwBufLen = 40960;
	DWORD dwCount = 0;
	DWORD i = 0;
	BYTE *pBuf = NULL;
	DWORD dwHasReadLen = 0;
	DWORD dwNeedReadLen = 0;
	DWORD dwRet = 0;
	BOOL BFail = FALSE;
	if(!oFileSource.Open(szSource, CFile::modeRead))
	{
		theLog.Write("!!CIOPrinter::CopySplFile,3,Open fail,err=%d,szSource=%s,szDest=%s", GetLastError(), szSource, szDest);
		goto ERR1;
	}
	dwFileLen = oFileSource.GetLength();

	if(!oFileDest.Open(szDest, CFile::modeCreate|CFile::modeWrite))
	{
		theLog.Write("!!CIOPrinter::CopySplFile,4,Open fail,err=%d,szSource=%s,szDest=%s", GetLastError(), szSource, szDest);
		goto ERR2;
	}

	pBuf = new BYTE[dwBufLen];
	if(!pBuf)
	{
		theLog.Write("!!CIOPrinter::CopySplFile,5,new fail,dwBufLen=%d,err=%d,szSource=%s,szDest=%s"
			, dwBufLen, GetLastError(), szSource, szDest);
		goto ERR3;
	}

	if(dwFileLen <= dwBufLen)
	{
		try
		{
			while(dwHasReadLen < dwFileLen)
			{
				dwNeedReadLen = dwFileLen - dwHasReadLen;
				dwRet = oFileSource.Read(pBuf + dwHasReadLen, dwNeedReadLen);
				if(0 >= dwRet)
				{
					theLog.Write("!!CIOPrinter::CopySplFile,6,read fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s]", GetLastError(), dwHasReadLen, dwFileLen, szSource);
					BFail = TRUE;
					break;
				}

				dwHasReadLen += dwRet;
			}
		}
		catch(...)
		{
			theLog.Write("!!CIOPrinter::CopySplFile,7,read catch sth,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s]", GetLastError(), dwHasReadLen, dwFileLen, szSource);
			goto ERR4;
		}
		if(BFail)
		{
			theLog.Write("!!CIOPrinter::CopySplFile,8,read fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szSource);
			goto ERR4;
		}

		try
		{
			oFileDest.Write(pBuf, dwHasReadLen);
		}
		catch(...)
		{
			theLog.Write("!!CIOPrinter::CopySplFile,9,write fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s],szDest=%s", GetLastError(), dwHasReadLen, dwFileLen, szSource, szDest);
			goto ERR4;
		}
	}//if(dwFileLen <= dwBufLen)
	else
	{
		dwCount = dwFileLen/dwBufLen;
		if(0 != (dwFileLen % dwBufLen))
		{
			dwCount++;
		}
		DWORD dwThisLoopNeedRdLen = dwBufLen;
		for(i = 0; i < dwCount; i++)
		{
			if(dwCount - 1 == i)
			{
				dwThisLoopNeedRdLen = dwFileLen - (dwBufLen * i);
			}
			try
			{
				dwHasReadLen = 0;
				while(dwHasReadLen < dwThisLoopNeedRdLen)
				{
					dwNeedReadLen = dwThisLoopNeedRdLen - dwHasReadLen;
					dwRet = oFileSource.Read(pBuf + dwHasReadLen, dwNeedReadLen);
					if(0 >= dwRet)
					{
						theLog.Write("!!CIOPrinter::CopySplFile,16,read fail,i=%d,err=%d,dwHasReadLen=%d,dwThisLoopNeedRdLen=%d,szSource=[%s]"
							, i, GetLastError(), dwHasReadLen, dwThisLoopNeedRdLen, szSource);
						BFail = TRUE;
						break;
					}

					dwHasReadLen += dwRet;
				}
			}
			catch(...)
			{
				theLog.Write("!!CIOPrinter::CopySplFile,17,read catch sth,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s]", GetLastError(), dwHasReadLen, dwFileLen, szSource);
				break;			
			}
			if(BFail)
			{
				theLog.Write("!!CIOPrinter::CopySplFile,18,read fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szSource);
				goto ERR4;
			}

			try
			{
				oFileDest.Write(pBuf, dwThisLoopNeedRdLen);
			}
			catch(...)
			{
				theLog.Write("!!CIOPrinter::CopySplFile,19,write fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szSource=[%s],szDest=%s", GetLastError(), dwHasReadLen, dwFileLen, szSource, szDest);
				goto ERR4;
			}
		}//for(i = 0; i < dwCount; i++)
	}//else
	

	delete []pBuf;
	oFileDest.Flush();
	oFileDest.Close();
	oFileSource.Close();	

	return TRUE;
ERR4:
	delete []pBuf;
ERR3:
	oFileDest.Close();
ERR2:
	oFileSource.Close();
ERR1:
	return FALSE;
}

BOOL ChangeJobColorIfNeededEx3(CString szSplPath, CString &szNewSplPath, DWORD dwColor, DWORD dwDuplex)
{
	if(szSplPath.IsEmpty() || !::PathFileExists(szSplPath))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,1,szSplPath=%s", szSplPath);
		return FALSE;
	}

	if (DMCOLOR_COLOR == dwColor)
	{//�û��в�ɫȨ��
		theLog.Write("==ChangeJobColorIfNeededEx3,2,�û��в�ɫȨ��,��Ҫ�޸�,ԭ����ӡ");
		return TRUE;
	}
	else
	{//�û�û�в�ɫȨ��
		theLog.Write("==ChangeJobColorIfNeededEx3,3,�û�û�в�ɫȨ��,����Ҫ�޸���ɫ����");
	}

	DWORD dwOriPathLen = szSplPath.GetLength();
	int nDotPos = szSplPath.ReverseFind('.');
	szNewSplPath.Format("%s_2.spl", szSplPath.Left(nDotPos));

	theLog.Write("==CIOPrinter::ChangeJobColorIfNeededEx3,4,szNewSplPath=[%s]", szNewSplPath);

	CFile oFileSource;
	CFile oFileDest;
	DWORD dwRet = 0;
	BOOL BFirst = TRUE;
	DWORD dwBufLen = 1024 * 2048 + 1;//2M   // 40961;	//40K + 1
	BYTE *pBuf = new BYTE[dwBufLen];
	if(!pBuf)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,5,new BYTE[%d] fail,err=%d,szSplPath=%s"
			, dwBufLen, GetLastError(), szSplPath);
		goto ERR1;
	}

	if(!oFileSource.Open(szSplPath, CFile::modeRead))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,6,Open fail,err=%d,szSplPath=%s,szNewSplPath=%s", GetLastError(), szSplPath, szNewSplPath);
		goto ERR2;
	}	

	if(!oFileDest.Open(szNewSplPath, CFile::modeCreate|CFile::modeWrite))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,7,Open fail,err=%d,szSplPath=%s,szNewSplPath=%s", GetLastError(), szSplPath, szNewSplPath);
		goto ERR3;
	}

	do 
	{
		dwRet = oFileSource.Read(pBuf, dwBufLen - 1);
		if(0 < dwRet)
		{
			if(BFirst)
			{//�����ַ���
				BFirst = FALSE;

				int nRet = 0;
				BYTE *pPos = NULL;
				CString szTmp ;
				pBuf[dwBufLen - 1] = 0x0;	//���һ��,��֤�����γ��ַ���
				szTmp.Format("%s", pBuf);
				DWORD dwOriLen = szTmp.GetLength();
				pPos = pBuf + dwOriLen;	//�ַ����������Ч����
				DWORD dwLeftLen = dwRet - dwOriLen;	//�ַ����������Ч���泤��
				CString szEnableDuplex = "@PJL SET DUPLEX=ON";
				CString szDisableDuplex = "@PJL SET DUPLEX=OFF";

#if 0	//˫���ӡ��ʱû�п���
				if(0 == dwDuplex)	//��ʱ���ɵ���,����Ҫ������
				{//��Ҫ����					
					nRet = szTmp.Replace(szEnableDuplex, szDisableDuplex);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,20,�ҵ�˫�棬�滻�ɵ���");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,20,û���ҵ�˫��");
					}
				}
				else
				{//��Ҫ˫��
					szTmp.Replace(szDisableDuplex, szEnableDuplex);
				}
#endif

#if 0
				//����pcl5�Ĵ�ӡ�ļ�
				CString szEnableColor1 = "@PJL SET DATAMODE=COLOR";	//���ò�ɫ
				CString szEnableColor2 = "@PJL SET RENDERMODE=COLOR";	//���ò�ɫ
				CString szDisableColor1 = "@PJL SET DATAMODE=GRAYSCALE";	//���úڰ�
				CString szDisableColor2 = "@PJL SET RENDERMODE=GRAYSCALE";	//���úڰ�
				if(DMCOLOR_COLOR == dwColor)
				{//���ò�ɫ
					nRet = szTmp.Replace(szDisableColor1, szEnableColor1);
					nRet = szTmp.Replace(szDisableColor2, szEnableColor2);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,30,�ҵ��ڰף��滻�ɲ�ɫ");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,31,û���ҵ��ڰ�");
					}
				}
				else
				{//���úڰ�
					nRet = szTmp.Replace(szEnableColor1, szDisableColor1);
					nRet = szTmp.Replace(szEnableColor2, szDisableColor2);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,40,�ҵ���ɫ���滻�ɺڰ�");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,41,û���ҵ���ɫ");
					}
				}
#endif

#if 0
				//�������pcl6�Ĵ�ӡ�ļ�
				CString szEnableColor1 = "@PJL SET PLANESINUSE = 3";				//���ò�ɫ
				CString szEnableColor2 = "@PJL SET DRIVERCOLORSELECT = COLOR";		//���ò�ɫ
				CString szDisableColor1 = "@PJL SET PLANESINUSE = 1";				//���úڰ�
				CString szDisableColor2 = "@PJL SET DRIVERCOLORSELECT = GRAYSCALE";	//���úڰ�
				/************************************************************************/
				/*            ������ɫ���������ʱ�Ȳ���������������£�              */
				/*      û�еĲ�������@PJL SET PLANESINUSE��							*/
				/*		������������²�����											*/
				/*      @PJL SET MLT2COLOR = ON											*/
				/*      @PJL SET DRIVERCOLORSELECT = COLOR								*/
				/*      @PJL SET MLT2COLORCOLOR = 2										*/
				/*      ������MLT2COLORCOLOR����2=��+�죬3=��+����4=��+��				*/
				/*      ������MLT2COLORCOLOR����6=��+�ƣ�7=��+�࣬8=��+���				*/
				/************************************************************************/

#elif 0
				//����ʩ��pcl6�Ĵ�ӡ�ļ�
				CString szEnableColor1 = "@PJL COMMENT FXJOBINFO COLORMODE=COLOR";		//���ò�ɫ
				CString szEnableColor2 = "@PJL SET RENDERMODE=COLOR";					//���ò�ɫ
				CString szDisableColor1 = "@PJL COMMENT FXJOBINFO COLORMODE=GRAYSCALE";	//���úڰ�
				CString szDisableColor2 = "@PJL SET RENDERMODE=GRAYSCALE";				//���úڰ�
#elif 0
				//�������pcl6�Ĵ�ӡ�ļ�
				CString szEnableColor1 = "@PJL SET DATAMODE = COLOR";	//���ò�ɫ
				CString szEnableColor2 = "@PJL SET RENDERMODE = COLOR";	//���ò�ɫ
				CString szDisableColor1 = "@PJL SET DATAMODE = GRAYSCALE";	//���úڰ�
				CString szDisableColor2 = "@PJL SET RENDERMODE = GRAYSCALE";	//���úڰ�
#elif 1
				//�������PostScript�Ĵ�ӡ�ļ�
				CString szEnableColor1 = "%%BeginFeature: colorbw color";			//���ò�ɫ
				CString szEnableColor2 = "(cmyk) RCsetdevicecolor";					//���ò�ɫ
				CString szDisableColor1 = "%%BeginFeature: colorbw blackandwhite";	//���úڰ�
				CString szDisableColor2 = "(gray) RCsetdevicecolor";				//���úڰ�
#endif

				if(DMCOLOR_COLOR != dwColor)
				{//ֻ��ʹ�úڰ״�ӡ
					nRet = szTmp.Replace(szEnableColor1, szDisableColor1);
					nRet = szTmp.Replace(szEnableColor2, szDisableColor2);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::ChangeJobColorIfNeededEx3,40,�ҵ���ɫ���滻�ɺڰ�");
					}
					else
					{
						theLog.Write("==CIOPrinter::ChangeJobColorIfNeededEx3,41,ֻ�кڰ״�ӡȨ��,����û���ҵ���ɫ,��Ҫ�޸��ļ�.");
						goto SUCC1;
					}
				}

				DWORD dwNewTmpLen = szTmp.GetLength();

				oFileDest.Write(szTmp.GetBuffer(), dwNewTmpLen);
				szTmp.ReleaseBuffer();
				if(0 < dwLeftLen)
				{
					oFileDest.Write(pPos, dwLeftLen);
				}
			}//if(BFirst)
			else
			{
				oFileDest.Write(pBuf, dwRet);
			}
		}//if(0 < dwRet)
		else
		{
			break;
		}
	} while (1);

	oFileDest.Close();
	oFileSource.Close();

	if(!::DeleteFile(szSplPath))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,50,DeleteFile fail,err=%d,szSplPath=[%s]", GetLastError(), szSplPath);
	}
	if(!::MoveFile(szNewSplPath, szSplPath))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx3,51,MoveFile fail,err=%d,szSplPath=[%s]", GetLastError(), szSplPath);
	}

#pragma message("================CIOPrinter::ChangeJobColorIfNeededEx3,100,���ԣ���ʱû��ɾ���������spl��ʱ�ļ�")

	delete []pBuf;
	return TRUE;
ERR4:
	oFileDest.Close();
ERR3:
	oFileSource.Close();
ERR2:
	delete []pBuf;
ERR1:
	return FALSE;
SUCC1:
	oFileDest.Close();
	oFileSource.Close();
	::DeleteFile(szNewSplPath);
	delete []pBuf;
	return TRUE;
}

BOOL ChangeJobColorIfNeededEx2(CString szSplPath, CString &szNewSplPath, DWORD dwColor, DWORD dwDuplex)
{
	if(szSplPath.IsEmpty() || !::PathFileExists(szSplPath))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx2,1,szSplPath=%s", szSplPath);
		return FALSE;
	}

	DWORD dwOriPathLen = szSplPath.GetLength();
	int nDotPos = szSplPath.ReverseFind('.');
	szNewSplPath.Format("%s_2.spl", szSplPath.Left(nDotPos));

	theLog.Write("==CIOPrinter::ChangeJobColorIfNeededEx2,1.2,szNewSplPath=[%s]", szNewSplPath);

	CFile oFileSource;
	CFile oFileDest;
	DWORD dwRet = 0;
	BOOL BFirst = TRUE;
	DWORD dwBufLen = 40961;	//40K + 1
	BYTE *pBuf = new BYTE[dwBufLen];
	if(!pBuf)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx2,2,new BYTE[%d] fail,err=%d,szSplPath=%s"
			, dwBufLen, GetLastError(), szSplPath);
		goto ERR1;
	}

	if(!oFileSource.Open(szSplPath, CFile::modeRead))
	{
		theLog.Write("!!CIOPrinter::CopySplFile,3,Open fail,err=%d,szSplPath=%s,szNewSplPath=%s", GetLastError(), szSplPath, szNewSplPath);
		goto ERR2;
	}	

	if(!oFileDest.Open(szNewSplPath, CFile::modeCreate|CFile::modeWrite))
	{
		theLog.Write("!!CIOPrinter::CopySplFile,4,Open fail,err=%d,szSplPath=%s,szNewSplPath=%s", GetLastError(), szSplPath, szNewSplPath);
		goto ERR3;
	}

	do 
	{
		dwRet = oFileSource.Read(pBuf, dwBufLen - 1);
		if(0 < dwRet)
		{
			if(BFirst)
			{//�����ַ���
				BFirst = FALSE;

				int nRet = 0;
				BYTE *pPos = NULL;
				CString szTmp ;
				pBuf[dwBufLen - 1] = 0x0;	//���һ��,��֤�����γ��ַ���
				szTmp.Format("%s", pBuf);
				DWORD dwOriLen = szTmp.GetLength();
				pPos = pBuf + dwOriLen;	//�ַ����������Ч����
				DWORD dwLeftLen = dwRet - dwOriLen;	//�ַ����������Ч���泤��
				CString szEnableDuplex = "@PJL SET DUPLEX = ON";
				CString szDisableDuplex = "@PJL SET DUPLEX = OFF";

#if 0
				if(0 == dwDuplex)	//��ʱ���ɵ���,����Ҫ������
				{//��Ҫ����					
					nRet = szTmp.Replace(szEnableDuplex, szDisableDuplex);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,20,�ҵ�˫�棬�滻�ɵ���");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,20,û���ҵ�˫��");
					}
				}
				else
				{//��Ҫ˫��
					szTmp.Replace(szDisableDuplex, szEnableDuplex);
				}
#endif

				CString szEnableColor = "@PJL SET RENDERMODE = COLOR";	//���ò�ɫ
				CString szDisableColor = "@PJL SET RENDERMODE = GRAYSCALE";	//���úڰ�
				if(DMCOLOR_COLOR == dwColor)
				{//���ò�ɫ
					nRet = szTmp.Replace(szDisableColor, szEnableColor);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,30,�ҵ��ڰף��滻�ɲ�ɫ");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,30,û���ҵ��ڰ�");
					}
				}
				else
				{//���úڰ�
					nRet = szTmp.Replace(szEnableColor, szDisableColor);
					if(0 != nRet)
					{
						theLog.Write("==CIOPrinter::CopySplFile,40,�ҵ���ɫ���滻�ɺڰ�");
					}
					else
					{
						theLog.Write("==CIOPrinter::CopySplFile,41,û���ҵ���ɫ");
					}
				}

				DWORD dwNewTmpLen = szTmp.GetLength();

				oFileDest.Write(szTmp.GetBuffer(), dwNewTmpLen);
				szTmp.ReleaseBuffer();
				if(0 < dwLeftLen)
				{
					oFileDest.Write(pPos, dwLeftLen);
				}
			}//if(BFirst)
			else
			{
				oFileDest.Write(pBuf, dwRet);
			}
		}//if(0 < dwRet)
		else
		{
			break;
		}
	} while (1);

	oFileDest.Close();
	oFileSource.Close();

	if(!::DeleteFile(szSplPath))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx2,50,DeleteFile fail,err=%d,szSplPath=[%s]", GetLastError(), szSplPath);
	}
	if(!::CopyFile(szNewSplPath, szSplPath, FALSE))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx2,51,CopyFile fail,err=%d,szSplPath=[%s]", GetLastError(), szSplPath);
	}

#pragma message("================CIOPrinter::ChangeJobColorIfNeededEx2,100,���ԣ���ʱû��ɾ���������spl��ʱ�ļ�")
	::DeleteFile(szNewSplPath);

	delete []pBuf;
	return TRUE;
ERR4:
	oFileDest.Close();
ERR3:
	oFileSource.Close();
ERR2:
	delete []pBuf;
ERR1:
	return FALSE;
}

BOOL ChangeJobColorIfNeededEx(CString szPrintName, DWORD dwColor, DWORD dwPaperSize)
{
	HANDLE hPrinter = NULL;
	if ( !::OpenPrinter(szPrintName.GetBuffer(), &hPrinter, NULL) )
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,1,OpenPrinter fail,err=%d,szPrintName=%s", GetLastError(), szPrintName);
		return FALSE;
	}

	BOOL bNeedSet = FALSE;
	PDEVMODE DevMode = NULL;
	DWORD dwBytes       = DocumentProperties(NULL, hPrinter, szPrintName.GetBuffer(), NULL, NULL, 0);
	szPrintName.ReleaseBuffer();
	HGLOBAL lhDevMode   = GlobalAlloc(GHND, dwBytes);
	if(!lhDevMode)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,2,GlobalAlloc fail,err=%d,dwBytes=%d"
			, GetLastError(), dwBytes);
		goto ERR1;
	}
	DevMode = (PDEVMODE)GlobalLock(lhDevMode);                  //��ȡPDevMode�ṹ
	if(!DevMode)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,3,GlobalLock fail,err=%d,dwBytes=%d,lhDevMode=0x%x"
			, GetLastError(), dwBytes, lhDevMode);
		goto ERR2;
	}
	DocumentProperties(NULL, hPrinter, szPrintName.GetBuffer(), DevMode, NULL, DM_OUT_BUFFER);
	if (!DevMode)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,4,DocumentProperties fail,err=%d,dwBytes=%d,lhDevMode=0x%x"
			, GetLastError(), dwBytes, lhDevMode);
		goto ERR2;
	}
#if 0
	DevMode->dmFields = DevMode->dmFields|DM_PAPERSIZE;   
	DevMode->dmFields = DevMode->dmFields|DM_PAPERLENGTH;
	DevMode->dmFields = DevMode->dmFields|DM_PAPERWIDTH;
	DevMode->dmPaperSize    = DMPAPER_USER;//��Ϊ�Զ���ֽ��
	//            DevMode->dmPaperLength  = m_paperHeight;
	//            DevMode->dmPaperWidth   = m_paperWidth;
#endif

	theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,6,dmColor=%d,dmPaperSize=%d,dmDuplex=0x%x,dmFields=0x%x"
		, DevMode->dmColor, DevMode->dmPaperSize, DevMode->dmDuplex, DevMode->dmFields);

	//�ı���ɫ
	if (DevMode->dmFields & DM_COLOR)
	{
		if (DevMode->dmColor == dwColor)
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,11,the printer no change color %d",dwColor);
		}
		else
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,12,change the color %d",dwColor);
			DevMode->dmColor = dwColor;
			bNeedSet = TRUE;

			DevMode->dmFields |= DevMode->dmFields & DM_COLOR;
			if(IDOK != DocumentProperties(NULL, hPrinter, szPrintName.GetBuffer(), DevMode, DevMode, DM_IN_BUFFER|DM_OUT_BUFFER))
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,12.2,change the color %d,fail,err=%d",dwColor, GetLastError());
			}
			szPrintName.ReleaseBuffer();
		}

		
	}
	else
	{
		theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13,no color");
	}

	//���Ե�˫��
	{
#if 0
		//˫���ӡ
		if (DM_DUPLEX & DevMode->dmFields)
		{
			DMDUP_SIMPLEX;
			DMDUP_VERTICAL;
			DMDUP_HORIZONTAL;
//			if (DevMode->dmDuplex == m_dwDuplex)	//��ʱ����
			if (DevMode->dmDuplex == DMDUP_HORIZONTAL)	//��ʱ����
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.2,the printer no change duplex %d", DMDUP_HORIZONTAL);
			}
			else
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.3,change the duplex %d", DMDUP_HORIZONTAL);
				DevMode->dmDuplex = DMDUP_SIMPLEX;
			}
		}
#else
			DevMode->dmFields |= DevMode->dmFields & DM_DUPLEX;
			DevMode->dmDuplex = DMDUP_SIMPLEX;

			if(IDOK != DocumentProperties(NULL, hPrinter, szPrintName.GetBuffer(), DevMode, DevMode, DM_IN_BUFFER|DM_OUT_BUFFER))
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,13.211,change the duplex %d,fail,err=%d", DevMode->dmPaperSize, GetLastError());
			}
			else
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.211,change the duplex OK!");
			}
			szPrintName.ReleaseBuffer();
#endif
	}

	//����a4תa3
#if 0
	{
		//�ı�ֽ�Ŵ�С
		DM_PAPERSIZE;
		if (DevMode->dmFields & DM_PAPERSIZE)
		{
			if (DevMode->dmPaperSize  == dwPaperSize)
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.2,the printer no change papersize %d",dwPaperSize);
			}
			else
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.4,change the papersize %d", dwPaperSize);
				DevMode->dmPaperSize  = dwPaperSize;
				bNeedSet = TRUE;

				DevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERSIZE;
				if(IDOK != DocumentProperties(NULL, hPrinter, NULL, DevMode, DevMode, DM_IN_BUFFER|DM_OUT_BUFFER))
				{
					theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,12.2,change the color %d,fail,err=%d",dwColor, GetLastError());
				}

			}
		}
		else
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,13.5,No PaperSize");
		}
	}
#else
	DevMode->dmPaperSize = DMPAPER_A3;
	bNeedSet = TRUE;
	DevMode->dmFields |= DevMode->dmFields & DM_PAPERSIZE;
	if(IDOK != DocumentProperties(NULL, hPrinter, szPrintName.GetBuffer(), DevMode, DevMode, DM_IN_BUFFER|DM_OUT_BUFFER))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,14.2,change the size %d,fail,err=%d", DevMode->dmPaperSize, GetLastError());
	}
	szPrintName.ReleaseBuffer();

	if(!PrinterUtils::SetPrinterSetting( szPrintName, *DevMode))
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeededEx,15,SetPrinterSetting fail,err=%d", GetLastError());
	}

#endif

	theLog.Write("CIOPrinter::ChangeJobColorIfNeededEx,17,dmColor=%d,dmPaperSize=%d"
		, DevMode->dmColor, DevMode->dmPaperSize);

#if 0
	if(bNeedSet)
	{
		dwBytes = DocumentProperties(NULL, hPrinter, NULL, DevMode, DevMode, DM_IN_BUFFER|DM_OUT_BUFFER);
	}
#endif


	GlobalUnlock(lhDevMode);
	GlobalFree(lhDevMode);
	::ClosePrinter(hPrinter);
	return TRUE;
ERR3:
	GlobalUnlock(lhDevMode);
ERR2:
	GlobalFree(lhDevMode);
ERR1:
	::ClosePrinter(hPrinter);
	return FALSE;
}

BOOL ChangeJobColorIfNeeded(HANDLE hPrinter, int nJobID, DWORD dwColor, DWORD dwPaperSize)
{
	if(!hPrinter || INVALID_HANDLE_VALUE == hPrinter)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,1,hPrinter=0x%x", hPrinter);
		return FALSE;
	}

	DWORD cbNeed = 0;
	BYTE *pJobData = NULL;
	JOB_INFO_2* pJobInfo = NULL;
	BOOL bRet = FALSE;
	BOOL bNeedSet = FALSE;

	if(!GetJob(hPrinter,nJobID,2,NULL,0,&cbNeed))
	{
		if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,2,GetJob1 error err��%d",GetLastError());
			goto ERR1;
		}
	}	
	pJobData = new BYTE[cbNeed];
	if(!pJobData)
	{
		theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,3,new BYTE[%d] error err��%d", cbNeed, GetLastError());
		goto ERR1;
	}
	memset(pJobData,0,cbNeed);
	DWORD dwState = 0;

	if(!GetJob(hPrinter,nJobID,2,pJobData,cbNeed,&cbNeed))//JOB_STATUS_SPOOLING
	{
		theLog.Write("!!CPrinterJobHelper::ChangeJobColorIfNeeded,4,GetJob2 error: %d",GetLastError());
		goto ERR2;
	}	
	pJobInfo = (JOB_INFO_2 *)pJobData;//DMDUP_SIMPLEX

	theLog.Write("##CIOPrinter::ChangeJobColorIfNeeded,10,pPrinterName:%s,pMachineName:%s pUserName:%s pDocument:%s  pDatatype:%s  pPrintProcessor:%s  "
		" pDriverName:%s  pStatus:%s  Status:%d   Position:%d  TotalPages:%d  PagesPrinted:%d  Copys:%d "
		" dmColor:[%d],dmDuplex:%d,orig = %d,dmFields=[0x%x],dmPaperSize=[0x%x]",
		pJobInfo->pPrinterName,pJobInfo->pMachineName,pJobInfo->pUserName,pJobInfo->pDocument,pJobInfo->pDatatype,
		pJobInfo->pPrintProcessor,pJobInfo->pDriverName,pJobInfo->pStatus,pJobInfo->Status,pJobInfo->Position,
		pJobInfo->TotalPages,pJobInfo->PagesPrinted,pJobInfo->pDevMode->dmCopies,
		pJobInfo->pDevMode->dmColor,
		pJobInfo->pDevMode->dmDuplex,
		pJobInfo->pDevMode->dmOrientation
		, pJobInfo->pDevMode->dmFields
		, pJobInfo->pDevMode->dmPaperSize);


	//�ı���ɫ
	DMCOLOR_COLOR;
	if (pJobInfo->pDevMode->dmFields & DM_COLOR)
	{
		if (pJobInfo->pDevMode->dmColor == dwColor)
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,11,the printer no change color %d",dwColor);
		}
		else
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,12,change the color %d",dwColor);
			pJobInfo->pDevMode->dmColor = dwColor;
			bNeedSet = TRUE;
		}
	}
	else
	{
		theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,13,no color");
	}

#pragma message("=============================CIOPrinter::ChangeJobColorIfNeeded,���ԣ���ǿ��������ҵ����")

	//����a4תa3
#if 0
	{
		//�ı�ֽ�Ŵ�С
		DM_PAPERSIZE;
		if (pJobInfo->pDevMode->dmFields & DM_PAPERSIZE)
		{
			if (pJobInfo->pDevMode->dmPaperSize  == dwPaperSize)
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,13.2,the printer no change papersize %d",dwPaperSize);
			}
			else
			{
				theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,13.4,change the papersize %d", dwPaperSize);
				pJobInfo->pDevMode->dmPaperSize  = dwPaperSize;
				bNeedSet = TRUE;
			}
		}
		else
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,13.5,No PaperSize");
		}
	}
#else
	pJobInfo->pDevMode->dmPaperSize = DMPAPER_A3;
	bNeedSet = TRUE;
#endif

	theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,14,bNeedSet=%d", bNeedSet);

#if 0
	//����
	if(bNeedSet)
	{
		if(!SetJob(hPrinter, nJobID, 2, pJobData, 0))
		{
			theLog.Write("CIOPrinter::ChangeJobColorIfNeeded,15,SetJob fail,nJobID=%d,dwColor=0x%x"
				, nJobID, dwColor);
			goto ERR2;
		}
#if 0
		do
		{
			PRINTER_INFO_9 *pInfo9 = NULL;
			DWORD dwTmpBufLen = 0;
			BYTE *pTmpBuf = NULL;
			LPDEVMODE pDevMd = NULL;
			if(!GetPrinter(hPrinter, 9, NULL, 0, &dwTmpBufLen))
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,15.2,GetPrinter fail,nJobID=%d,err=%d"
					, nJobID, GetLastError());
				break;
			}

			pTmpBuf = new BYTE[dwTmpBufLen];
			if(!pTmpBuf)
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,15.3,new BYTE[%d] fail,nJobID=%d,err=%d"
					, dwTmpBufLen, nJobID, GetLastError());
				break;
			}

			if(!GetPrinter(hPrinter, 9, pTmpBuf, dwTmpBufLen, &dwTmpBufLen))
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,15.4,GetPrinter fail,nJobID=%d,err=%d"
					, nJobID, GetLastError());
				delete []pTmpBuf;
				break;
			}

			pInfo9 = (PRINTER_INFO_9 *)pTmpBuf;
			pDevMd = pInfo9->pDevMode;
			if(!pDevMd)
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,15.5,pDevMode=0x%x,nJobID=%d,err=%d"
					, pDevMd, nJobID, GetLastError());
				delete []pTmpBuf;
				break;
			}
			theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,16,dmColor=[0x%x],dmPaperSize=[0x%x]"
				, pDevMd->dmColor, pDevMd->dmPaperSize);


			delete []pTmpBuf;
		}
		while(0);
#endif
	}
#endif

	delete []pJobData;

#if 0
	{//��ʱ����
		if(!GetJob(hPrinter,nJobID,2,NULL,0,&cbNeed))
		{
			if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,22,GetJob1 error err��%d",GetLastError());
				goto ERR1;
			}
		}	
		pJobData = new BYTE[cbNeed];
		if(!pJobData)
		{
			theLog.Write("!!CIOPrinter::ChangeJobColorIfNeeded,23,new BYTE[%d] error err��%d", cbNeed, GetLastError());
			goto ERR1;
		}
		memset(pJobData,0,cbNeed);
		DWORD dwState = 0;

		if(!GetJob(hPrinter,nJobID,2,pJobData,cbNeed,&cbNeed))//JOB_STATUS_SPOOLING
		{
			theLog.Write("!!CPrinterJobHelper::ChangeJobColorIfNeeded,24,GetJob2 error: %d",GetLastError());
			goto ERR2;
		}	
		pJobInfo = (JOB_INFO_2 *)pJobData;//DMDUP_SIMPLEX

		theLog.Write("##CIOPrinter::ChangeJobColorIfNeeded,30,pPrinterName:%s,pMachineName:%s pUserName:%s pDocument:%s  pDatatype:%s  pPrintProcessor:%s  "
			" pDriverName:%s  pStatus:%s  Status:%d   Position:%d  TotalPages:%d  PagesPrinted:%d  Copys:%d "
			" dmColor:[%d],dmDuplex:%d,orig = %d,dmFields=[0x%x],dmPaperSize=[0x%x]",
			pJobInfo->pPrinterName,pJobInfo->pMachineName,pJobInfo->pUserName,pJobInfo->pDocument,pJobInfo->pDatatype,
			pJobInfo->pPrintProcessor,pJobInfo->pDriverName,pJobInfo->pStatus,pJobInfo->Status,pJobInfo->Position,
			pJobInfo->TotalPages,pJobInfo->PagesPrinted,pJobInfo->pDevMode->dmCopies,
			pJobInfo->pDevMode->dmColor,
			pJobInfo->pDevMode->dmDuplex,
			pJobInfo->pDevMode->dmOrientation
			, pJobInfo->pDevMode->dmFields
			, pJobInfo->pDevMode->dmPaperSize);

		delete []pJobData;
	}
#endif
	return TRUE;
ERR2:
	delete []pJobData;
ERR1:
	return FALSE;
}

//add in 2016.04.18
BOOL CIOPrinter::PrintPdf(CString szPrinter, CString szDocName, CString szPdfPath)
{
	theLog.Write("CIOPrinter::PrintPdf,1,szPrinter=%s, szDocName=%s, szPdfPath=%s", szPrinter, szDocName, szPdfPath);

	BOOL bOperateSucc = FALSE;
	BOOL bWriteSucc = TRUE;
	BYTE* pData = NULL;
	HANDLE hPrinter = NULL;
	CFile file;

	do 
	{
		char* pJobType = "RAW";
		PRINTER_DEFAULTS pd;
		ZeroMemory(&pd, sizeof(PRINTER_DEFAULTS));
		pd.DesiredAccess = PRINTER_ALL_ACCESS;
		pd.pDatatype = pJobType;

		char cPrinterName[MAX_PATH] = {0};
		strcpy(cPrinterName, szPrinter);
		if(!OpenPrinter(cPrinterName,&hPrinter,&pd))
		{
			theLog.Write("!!CIOPrinter::PrintPdf,2,OpenPrinter fail, err=%u", GetLastError());
			break;
		}

		if(!file.Open(szPdfPath,CFile::modeRead))
		{
			theLog.Write("!!CIOPrinter::PrintPdf,3,Open file fail, err=%u", GetLastError());
			break;
		}

		theLog.Write("CIOPrinter::PrintPdf,4,Open file succ, len=%u", file.GetLength());

		pData = new BYTE[File_Read_Length_Unit];
		if(!pData)
		{
			theLog.Write("!!CIOPrinter::PrintPdf,4,pData=%p, err=%u", pData, GetLastError());
			break;
		}

		DOC_INFO_1 docinfo;
		VerifyDocName(szDocName);
		docinfo.pDocName = szDocName.GetBuffer();
		docinfo.pOutputFile = NULL;
		docinfo.pDatatype = pJobType;
		if (!StartDocPrinter(hPrinter,1,(LPBYTE)&docinfo))
		{
			theLog.Write("!!CIOPrinter::PrintPdf,5,StartDocPrinter fail,err=%u", GetLastError());
			break;
		}

		if (!StartPagePrinter(hPrinter))
		{
			theLog.Write("!!CIOPrinter::PrintPdf,6,StartPagePrinter fail,err=%u", GetLastError());
			break;
		}

		while (TRUE)
		{
			memset(pData, 0x0, File_Read_Length_Unit);
			DWORD dwRead = file.Read(pData, File_Read_Length_Unit);
			if (dwRead > 0)
			{
				DWORD dwWrite = 0;
				if (!WritePrinter(hPrinter,pData,dwRead,&dwWrite))
				{
					bWriteSucc = FALSE;
					theLog.Write("!!CIOPrinter::PrintPdf,7,WritePrinter fail,dwRead=%d, dwWrite=%d,err=%u"
						, dwRead, dwWrite, GetLastError());
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (!EndDocPrinter(hPrinter))
		{
			theLog.Write("!!CIOPrinter::PrintPdf,8,EndDocPrinter fail,err=%u", GetLastError());
		}

		bOperateSucc = TRUE;

	} while (FALSE);
	
	file.Close();

	if (hPrinter)
	{
		ClosePrinter(hPrinter);
	}
	if(pData)
	{
		delete[] pData;	
	}
	theLog.Write("CIOPrinter::PrintPdf,9,bOperateSucc=%d,bWriteSucc=%d", bOperateSucc, bWriteSucc);
	return (bOperateSucc && bWriteSucc);
}

//add in 2013.06.13,for ����ֱ����ļ�8900x6800,600DPI
BOOL CIOPrinter::PrintSpl(CString szPrinter, CString szSplPath)
{
	theLog.Write("==CIOPrinter::PrintSpl,1,szSplPath=%s", szSplPath);
	char* pJobType = "RAW";
	HANDLE            hPrinter;
	BOOL BRet = FALSE;
	PRINTER_DEFAULTS   pd;
	ZeroMemory( &pd, sizeof(PRINTER_DEFAULTS) );
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	pd.pDatatype = pJobType;

#if 0
	char cPrinterName[100];
	strcpy(cPrinterName, szPrinter);
#endif

	BYTE* pTemp = NULL;

#if 0
	//��ϵͳĿ¼��ɾ�����ǵ�ǰ��ӡ���ĵ�
	CString sPath;
	//ȡwindowsĿ¼��system32Ŀ¼
	char bysSysPath[255]; 
	memset(bysSysPath, 0x0, sizeof(bysSysPath) / sizeof(bysSysPath[0]));
	GetSystemDirectory(bysSysPath, MAX_PATH); 
	sPath.Format(_T("%s\\spool\\PRINTERS\\"), bysSysPath);
	CCommonFun::DeleteDir(sPath, TRUE);
#endif


	do 
	{
		//999999999999999
#if 0
		if(!ChangeJobColorIfNeededEx(szPrinter, m_dwColor, m_dwPaperSize))
		{
			theLog.Write("!!CIOPrinter::PrintSpl,1.15,ChangeJobColorIfNeededEx fail,err=%d"
				, GetLastError());
		}
#endif
		CString szNewSplPath = "";
#if 0	//ChangeJobColorIfNeededEx2�����Richo PCL6��ͨ ��ӡ������
		if(ChangeJobColorIfNeededEx2(szSplPath, szNewSplPath, m_dwColor, m_dwDuplex))
#else	//ChangeJobColorIfNeededEx3�����Richo PCL5��Richo PCL6ͨ�� ��ӡ������
		if(ChangeJobColorIfNeededEx3(szSplPath, szNewSplPath, m_dwColor, m_dwDuplex))
#endif
		{//�˺����޸���ҵ����
			theLog.Write("==CIOPrinter::PrintSpl,1.10,ChangeJobColorIfNeededEx3 OK");
		}
		else
		{
			theLog.Write("!!CIOPrinter::PrintSpl,1.11,ChangeJobColorIfNeededEx3 fail,szNewSplPath=%s", szNewSplPath);
		}

//		if(!OpenPrinter(cPrinterName,&hPrinter,&pd ) )
		if(!OpenPrinter(szPrinter.GetBuffer(),&hPrinter,&pd ) )
		{
			szPrinter.ReleaseBuffer();
			theLog.Write("!!CIOPrinter::PrintSpl,2,OpenPrinter fail,err=%d,szPrinter=%s", GetLastError(), szPrinter);
			break;
		}

		szPrinter.ReleaseBuffer();

		BYTE pData[1024] = {0};
		DWORD dwNeed = 0;
		BOOL bRet = AddJob(hPrinter,1,pData,1024,&dwNeed);
		if(!bRet)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,2.2,OpenPrinter fail,err=%d,szPrinter=%s", GetLastError(), szPrinter);
			break;
		}
		ADDJOB_INFO_1* pInfo = (ADDJOB_INFO_1*)pData;
		CString szFile = pInfo->Path;
		int nJobID = pInfo->JobId;

		//----------------------------------------------
#if 1	//===========================================

// 		theLog.Write("=====CopySplFile(szSplPath, szFile) before");
// 		if(!CopySplFile(szSplPath, szFile))
// 		{
// 			theLog.Write("!!CIOPrinter::PrintSpl,2.3,CopySplFile fail,err=%d,szPrinter=[%s],szSplPath=[%s],szNewFile=[%s]"
// 				, GetLastError(), szPrinter, szSplPath, szFile);
// 			break;
// 		}
// 		theLog.Write("=====CopySplFile(szSplPath, szFile) after");
// 		theLog.Write("=====MoveFile(szSplPath, szFile) before");
		if(!MoveFile(szSplPath, szFile))
		{
			theLog.Write("!!CIOPrinter::PrintSpl,2.3,MoveFile fail,err=%d,szPrinter=[%s],szSplPath=[%s],szFile=[%s]"
				, GetLastError(), szPrinter, szSplPath, szFile);
			break;
		}
// 		theLog.Write("=====MoveFile(szSplPath, szFile) after");
#else
		CFile file;
		if(file.Open(szSplPath, CFile::modeRead) == 0)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,3,Open fail,err=%d,szPrinter=[%s],szSplPath=[%s]", GetLastError(), szPrinter, szSplPath);
			break;
		}
		DWORD dwFileLen = file.GetLength();
		pTemp = new BYTE[dwFileLen];
		if(!pTemp)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,4,new fail,err=%d,nFileLen=%d,szPrinter=[%s],szSplPath=[%s]", GetLastError(), dwFileLen,  szPrinter, szSplPath);
			break;
		}

#if 1
		DWORD dwHasReadLen = 0;
		DWORD dwNeedReadLen = 0;
		DWORD dwRet = 0;
		BOOL BFail = FALSE;
		try
		{
			while(dwHasReadLen < dwFileLen)
			{
				dwNeedReadLen = dwFileLen - dwHasReadLen;
				dwRet = file.Read(pTemp + dwHasReadLen, dwNeedReadLen);
				if(0 >= dwRet)
				{
					theLog.Write("!!CIOPrinter::PrintSpl,5,read fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szPrinter=[%s],szSplPath=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szPrinter, szSplPath);
					BFail = TRUE;
					break;
				}

				dwHasReadLen += dwRet;
			}
		}
		catch(...)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,5.2,read catch sth,err=%d,dwHasReadLen=%d,nFileLen=%d,szPrinter=[%s],szSplPath=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szPrinter, szSplPath);
			break;			
		}
		if(BFail)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,6,read fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szPrinter=[%s],szSplPath=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szPrinter, szSplPath);
			break;
		}
#else
		nRet = file.Read(pTemp,nFileLen);
#endif
		file.Close();

		if(file.Open(szFile,CFile::modeCreate|CFile::modeWrite) == 0)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,7,Open fail,err=%d,dwHasReadLen=%d,nFileLen=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szFile, szSplPath);
			break ;
		}
		try
		{
			file.Write((BYTE*)pTemp, dwFileLen);
		}
		catch(...)
		{
			theLog.Write("!!CIOPrinter::PrintSpl,8,write catch sth,err=%d,dwHasReadLen=%d,nFileLen=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), dwHasReadLen, dwFileLen,  szFile, szSplPath);
			break ;
		}
		file.Close();
#endif

#if 0
		//�˺���ֻ�ǰ���ҵ���Դ���־�У��������޸�
		if(!ChangeJobColorIfNeeded(hPrinter, nJobID, m_dwColor, m_dwPaperSize))
		{
			theLog.Write("!!CIOPrinter::PrintSpl,8.5,ChangeJobColorIfNeeded fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]"
				, GetLastError(), nJobID,  szFile, szSplPath);
		}
#endif


		bRet = ScheduleJob(hPrinter,nJobID);
		if(bRet)
		{
			int nError = GetLastError();   
			theLog.Write("==CIOPrinter::PrintSpl,9,ScheduleJob OK,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
		}
		else
		{
			theLog.Write("!!CIOPrinter::PrintSpl,10,ScheduleJob fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
			break;
		}

#if 0


		//add by zfq,2013.06.27,start
		if(bBigFile)
		{
			if(!SetJob(hPrinter, nJobID, 0, NULL, JOB_CONTROL_RESUME))
			{
				theLog.Write("!!CIOPrinter::PrintSpl,10.5,SetJob fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
			}
			else
			{
				theLog.Write("==CIOPrinter::PrintSpl,10.6,SetJob ok,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
			}
			if(!SetJob(hPrinter, nJobID, 0, NULL, JOB_CONTROL_RESTART))
			{
				theLog.Write("!!CIOPrinter::PrintSpl,10.7,SetJob fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
			}
			else
			{
				theLog.Write("==CIOPrinter::PrintSpl,10.8,SetJob ok,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
			}
		}
		DWORD cbNeed = 0;
		if(!GetJob(hPrinter, nJobID, 2, NULL, 0, &cbNeed))
		{
			int nError = GetLastError();   
			theLog.Write("!!CIOPrinter::PrintSpl,11,GetJob fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", GetLastError(), nJobID,  szFile, szSplPath);
		}
		else
		{
			theLog.Write("==CIOPrinter::PrintSpl,11.2,GetJob OK,cbNeed=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]", cbNeed, nJobID,  szFile, szSplPath);
		}
		//add by zfq,2013.06.27,end
#endif

#if 0
		//100000
		if(!ChangeJobColorIfNeeded(hPrinter, nJobID, m_dwColor, m_dwPaperSize))
		{
			theLog.Write("!!CIOPrinter::PrintSpl,11.5,ChangeJobColorIfNeeded fail,err=%d,nJobID=%d,szFile=[%s],szSplPath=[%s]"
				, GetLastError(), nJobID,  szFile, szSplPath);
		}
#endif

		BRet = TRUE;
	} while (0);

	if(hPrinter)
	{
		ClosePrinter(hPrinter);
	}


	if(pTemp)
		delete[] pTemp;

	return BRet;
}

BOOL CIOPrinter::IsWater()
{
	if (m_oSec.header.GetLength() > 0 ||
		m_oSec.footer.GetLength() > 0 ||
		m_oSec.water.GetLength() > 0)
	{
		return TRUE;
	}
	return FALSE;
}
void CIOPrinter::SetFooter(CString sVal)
{
	theLog.Write("footer = %s",sVal);
	m_oSec.footer = sVal;
}
void CIOPrinter::SetHeader(CString sVal)
{
	theLog.Write("header = %s",sVal);
	m_oSec.header = sVal;
}
void CIOPrinter::SetWatermark(CString sVal)
{
	theLog.Write("watermark = %s",sVal);
	m_oSec.water = sVal;
}

void CIOPrinter::KeepJob(HANDLE hPrint,BOOL bEnable ,BOOL& bKeep)
{
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	DWORD  cByteNeeded, nReturned=0, cByteUsed;

	if (!GetPrinter(hPrint, 2, NULL, 0, &cByteNeeded))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return ;
	}
	pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
	if (!(pPrinterInfo))
		return ;

	if (!GetPrinter(hPrint, 2, (LPBYTE)pPrinterInfo, cByteNeeded, &cByteUsed))
	{
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return ;
	} 
	if (pPrinterInfo->Attributes & PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS)
	{
		bKeep = TRUE;
	}
	else
	{
		bKeep = FALSE;
	}
	if(bEnable)
		pPrinterInfo->Attributes |= PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS;
	else
		pPrinterInfo->Attributes |= ~PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS;

	SetPrinter(hPrint,2,(BYTE*)pPrinterInfo,0);

	if (pPrinterInfo)
	{
		free(pPrinterInfo);
		pPrinterInfo = NULL;
	}
}