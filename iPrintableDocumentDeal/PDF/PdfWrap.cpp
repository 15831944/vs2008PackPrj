#include "StdAfx.h"
#include "PdfWrap.h"
#include "../Image/BitmapPalette.h"
#include "../../Public/PdfHelperDef.h"

CPdfWrap::CPdfWrap(void)
{
}

CPdfWrap::~CPdfWrap(void)
{
}

int CPdfWrap::GetPdfPageCount(CString csPdfFilePath)
{
	int nPageCount = 0;
	CPdfHelper pdf;
	if (!pdf.OpenFile(csPdfFilePath.GetString()))
	{
		theLog.Write("!!CPdfWrap::GetPdfPageCount,pdf.OpenFile,fail");
	}
	else
	{
		nPageCount = pdf.GetPageCount();
	}
	return nPageCount;
}

BOOL CPdfWrap::PDFToTiff(CString csPdfFilePath, CString csTiffFilePath, BOOL bColor, int nDpi /*= Image_Deal_Max_Dpi*/)
{
	theLog.Write("CPdfWrap::PDFToTiff,1,csPdfFilePath=%s, csTiffFilePath=%s, bColor=%d, nDpi=%d", csPdfFilePath, csTiffFilePath, bColor, nDpi);

	CPdfHelper pdf;
	if (!pdf.OpenFile(csPdfFilePath.GetString()))
	{
		theLog.Write("!!CPdfWrap::PDFToTiff,2,pdf.OpenFile fail");
		return FALSE;
	}

	int nPageCount = pdf.GetPageCount();
	if (nPageCount <= 0)
	{
		theLog.Write("!!CPdfWrap::PDFToTiff,3,nPageCount=%d", nPageCount);
		return FALSE;
	}
	
	theLog.Write("CPdfWrap::PDFToTiff,4,nPageCount=%d", nPageCount);

	BOOL bFirstSucc = FALSE;
	BOOL bAllSucc = FALSE;
	Status stat = Ok;
	CLSID imageClsid;
	GetEncoderClsidEx(csTiffFilePath, &imageClsid);
	CStringW wcsTiffFilePath(csTiffFilePath);
	Bitmap* pImageTiff = NULL;
	BITMAPINFO *pBi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	EncoderParameters encoderParameters;
	ULONG parameterValue;

	// An EncoderParameters object has an array of
	// EncoderParameter objects. In this case, there is only
	// one EncoderParameter object in the array.
	encoderParameters.Count = 1;

	// Initialize the one EncoderParameter object.
	encoderParameters.Parameter[0].Guid = EncoderSaveFlag;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &parameterValue;

	double hDPI = nDpi;
	double vDPI = nDpi;
	ImageColorMode colorMode = bColor ? imageModeBGR8 : imageModeMono8;
	int nWidth = 0;
	int nHeight = 0;
	int nStride = 0;
	unsigned char *scan0 = NULL;
	for (int nPageIndex=0; nPageIndex<nPageCount; nPageIndex++)
	{
		if (pdf.GetBitmap(nPageIndex, hDPI, vDPI, colorMode, nWidth, nHeight, nStride, &scan0))
		{
			int nStep = 1;
			WORD biBitCount = 32;
			switch (colorMode)
			{
			case imageModeMono1:
				biBitCount = 1;
				nStep = 255;
				break;
			case imageModeMono8:
				biBitCount = 8;
				nStep = 1;
				break;
			case imageModeRGB8:
			case imageModeBGR8:
				biBitCount = 24;
				break;
			case imageModeXBGR8:
				biBitCount = 32;
				break;
			}

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
#if 1
			switch (biBitCount)
			{
			case 8:
				memcpy_s((unsigned char*)pBi+sizeof(BITMAPINFOHEADER), 256*sizeof(RGBQUAD), gBitmapPalette256, sizeof(gBitmapPalette256));
				break;
			case 4:
				memcpy_s((unsigned char*)pBi+sizeof(BITMAPINFOHEADER), 256*sizeof(RGBQUAD), gBitmapPalette16, sizeof(gBitmapPalette16));
				break;
			case 2:
				memcpy_s((unsigned char*)pBi+sizeof(BITMAPINFOHEADER), 256*sizeof(RGBQUAD), gBitmapPalette4, sizeof(gBitmapPalette4));
				break;
			case 1:
				memcpy_s((unsigned char*)pBi+sizeof(BITMAPINFOHEADER), 256*sizeof(RGBQUAD), gBitmapPalette2, sizeof(gBitmapPalette2));
				break;
			}
#else
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
#endif
			if (nPageIndex == 0)
			{
				pImageTiff = new Bitmap(pBi, scan0);
				pImageTiff->SetResolution(hDPI, vDPI);
				parameterValue = EncoderValueMultiFrame;
				stat = pImageTiff->Save(wcsTiffFilePath, &imageClsid, &encoderParameters);
				if(stat != Ok)
				{
					theLog.Write("!!CPdfWrap::PDFToTiff,7,fail,nPageIndex=%d,stat=%d", nPageIndex, stat);
				}
				else
				{
					bFirstSucc = TRUE;	//第一页处理成功
					if (nPageCount == 1)	//文档只有1页
					{
						bAllSucc = TRUE;	//最后一页处理成功
					}
				}
			}
			else
			{
				Bitmap bitmap(pBi, scan0);
				parameterValue = EncoderValueFrameDimensionPage;
				stat = pImageTiff->SaveAdd(&bitmap, &encoderParameters);
				if(stat != Ok)
				{
					theLog.Write("!!CPdfWrap::PDFToTiff,8,fail,nPageIndex=%d,stat=%d", nPageIndex, stat);
					break;
				}
				else if ((nPageIndex+1) == nPageCount)
				{
					bAllSucc = TRUE;	//最后一页处理成功
				}
			}
			if (!bFirstSucc)
			{
				break;
			}
		}
		else
		{
			theLog.Write("!!CPdfWrap::PDFToTiff,9,GetBitmap,fail,nPageIndex=%d", nPageIndex);
			break;
		}
		if (scan0)
		{
			free(scan0);
			scan0 = NULL;
		}
	}

	if (pImageTiff)
	{
		delete pImageTiff;
	}
	if (pBi)
	{
		free(pBi);
	}
	if (scan0)
	{
		free(scan0);
	}

	if (bFirstSucc && bAllSucc)
	{
		return TRUE;
	}
	else
	{
		//转换失败，删除可能残留的tiff文件
		DeleteFile(csTiffFilePath);
		return FALSE;
	}
}
