#pragma once
class _AA_DLL_EXPORT_ RICHEDITFONT
{
public:
	RICHEDITFONT();
	static void LOGFONT2RICHEDITFONT(LOGFONT& LogFont, RICHEDITFONT& REFont, COLORREF color = RGB(0,0,0));
	static COLORREF RICHEDITFONT2LOGFONT(RICHEDITFONT& REFont, LOGFONT& LogFont);
public:
	LONG lfHeight; 
	COLORREF color;
	BYTE lfItalic; 
	BYTE lfUnderline; 
	BYTE lfStrikeOut; 
	BYTE lfBold;
	TCHAR lfFaceName[LF_FACESIZE]; 
};
