
#pragma once
#include <Winspool.h>

//��װ�����ӡ��
//
class CPrinterInstallHelper
{
public:
	CPrinterInstallHelper(void);
	~CPrinterInstallHelper(void);
public:
	BOOL DoSharePrinterNT(LPTSTR szPrinterName, LPTSTR szShareName, BOOL bShare);
	
	void WriteExePath(CString szFilePath);
	void SetDefaultPrinter();
	void CopyDriverFile();
	void DelDriverFile();
	BOOL Uninstall();
	BOOL DeleteOrRenameFile(CString szFilePath);
	
	BOOL DeletePrinterX();
	BOOL DeletePrinterDriverX();
	BOOL DeletePortX();

	BOOL Install(bool bOnlyInstallDrv=false);
	void AddPrinterProcessX();

	BOOL AddPrinterX();
	BOOL AddPrinterDriverX();
	BOOL AddPortX();

	//---------------
	//��װ�������
	void CopyDriverFileY(CString sCustom);
	BOOL AddPrinterY();
	BOOL AddPrinterDriverY(CString sCustom);
	BOOL AddPortY();

	BOOL UninstallPortMon(const MONITOR_INFO_2 &mi2);
	int InstallPrinterMonitor(LPCTSTR portMonName, LPCTSTR portMonFileName);

	void SetDefaultPath(char* pPath);

	void DeleteAllJob();

	void SetWorkDir();

	int SetDefaultParam();

	//add by zxl, 20150821,ɾ��WindowsͼƬ��ӡ��������Ϣ
	BOOL DeletePhotoPrintWizardPram();
	//add by zxl,200170620,���ô�ӡ����Ĭ��ɫ��ģʽ
	void SetPrinterDefaultColorMode(WORD wColor);

private:
	char m_szPrinterName[100];
	char m_szDriverName[100];
	char m_szPortName[100];
	char m_szProcName[100];
	char m_szProcDll[MAX_PATH];
	char m_szPrinterKeyPath[MAX_PATH];
	CString m_sDefaultPath;
};
