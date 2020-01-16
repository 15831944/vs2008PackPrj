#include "StdAfx.h"
#include "GhostScriptWrap.h"
#include "GhostScriptDef.h"

CGhostScriptWrap::CGhostScriptWrap(void)
{
}

CGhostScriptWrap::~CGhostScriptWrap(void)
{
}

BOOL CGhostScriptWrap::PdfToTiff(CString szPdfFilePath, CString szTiffFilePath, BOOL bColor /*= TRUE*/, int nDpi /*= Image_Deal_Max_Dpi*/, int nFirstPage /*= 0*/, int nLastPage /*= 0*/)
{
	CGhostScript gs;
	return gs.ExtractToTiff(szPdfFilePath.GetBuffer(), szTiffFilePath.GetBuffer(), bColor, nDpi, nFirstPage, nLastPage);
}

BOOL CGhostScriptWrap::PdfToTxt(CString szPdfFilePath, CString szTxtFilePath, int nFirstPage /*= 0*/, int nLastPage /*= 0*/)
{
	CGhostScript gs;
	return gs.ExtractToTxt(szPdfFilePath.GetBuffer(), szTxtFilePath.GetBuffer(), nFirstPage, nLastPage);
}

BOOL CGhostScriptWrap::PdfToPs(CString szPdfFilePath, CString szPsFilePath, int nFirstPage /*= 0*/, int nLastPage /*= 0*/)
{
	CGhostScript gs;
	return gs.ExtractToPs(szPdfFilePath.GetBuffer(), szPsFilePath.GetBuffer(), nFirstPage, nLastPage);
}

BOOL CGhostScriptWrap::PsToPdf(CString szPsFilePath, CString szPdfFilePath, int nFirstPage /*= 0*/, int nLastPage /*= 0*/)
{
	CGhostScript gs;
	return gs.ExtractToPdf(szPsFilePath.GetBuffer(), szPdfFilePath.GetBuffer(), nFirstPage, nLastPage);
}
