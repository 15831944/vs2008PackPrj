#pragma once

class CWinFireWallOp
{
public:
	CWinFireWallOp(void);
	~CWinFireWallOp(void);

	BOOL IsWinFireWallEnabled();
	BOOL IsExceptModeEnabled();
	void CloseFireWall();
	void EnableExceptMode();
	void ExceptProgram(CString sPath);
	void ExceptUDPPort(UINT nPort);
	void ExceptTCPPort(UINT nPort);


	void Install(CString sPath,CString sParam,DWORD dwWait = TRUE,DWORD dwShowCmd = SW_HIDE);
	
};
