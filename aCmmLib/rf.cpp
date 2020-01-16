#include "stdafx.h"
#include "rf.h"

RICHEDITFONT::RICHEDITFONT()
{
	lfHeight = -12;
	color = RGB(0, 0, 0);
	lfItalic = 0;
	lfBold = 1;
	lfStrikeOut = 0;
	lfUnderline = 0;
	strcpy(lfFaceName, CCommonFun::GetFaceName());
}
void RICHEDITFONT::LOGFONT2RICHEDITFONT(LOGFONT& LogFont, RICHEDITFONT& REFont,  COLORREF color/* = RGB(0,0,0)*/)
{
	strcpy(REFont.lfFaceName, LogFont.lfFaceName);
	REFont.lfItalic = LogFont.lfItalic;
	REFont.lfHeight = LogFont.lfHeight;
	REFont.lfBold = (LogFont.lfWeight == FW_BOLD);
	REFont.lfUnderline = LogFont.lfUnderline;
	REFont.lfStrikeOut = LogFont.lfStrikeOut;
	REFont.color = color;
}
int   CALLBACK   EnumFontsProc(   
							   CONST   LOGFONT   *lplf,           //   logical-font   data   
							   CONST   TEXTMETRIC   *lptm,     //   physical-font   data   
							   DWORD   dwType,                         //   font   type   
							   LPARAM   lpData                         //   application-defined   data   
							   )   
{   
	*(DWORD*)lpData   =   lplf->lfCharSet;
	return 0;
}   
COLORREF RICHEDITFONT::RICHEDITFONT2LOGFONT(RICHEDITFONT& REFont, LOGFONT& LogFont)
{
	strcpy(LogFont.lfFaceName, REFont.lfFaceName);

	LogFont.lfHeight = REFont.lfHeight;
	LogFont.lfItalic = REFont.lfItalic;
	LogFont.lfStrikeOut = REFont.lfStrikeOut;
	LogFont.lfUnderline = REFont.lfUnderline;

	if (REFont.lfBold)
		LogFont.lfWeight = FW_BOLD;
	else
		LogFont.lfWeight = FW_NORMAL;

	DWORD   dwCharSet = GB2312_CHARSET;   
	EnumFonts(GetDC(NULL),  LogFont.lfFaceName,   EnumFontsProc,   (LPARAM)&dwCharSet);   
	LogFont.lfCharSet = dwCharSet;
	return REFont.color;
}
