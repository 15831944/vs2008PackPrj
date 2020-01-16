#ifndef _VIRTUALDRIVERDEF_3C168011_98BB_4EE1_AB4C_38DE384721CD_H_ 
#define _VIRTUALDRIVERDEF_3C168011_98BB_4EE1_AB4C_38DE384721CD_H_

//其中%s是最终要替换的打印机驱动名[IST_PRINTER_NAME]

#define XAB_PRINTER_KEY_PATH	_T("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\%s")
#define HIDE_QRCODE				1

#define XAB_JOB_DIR				_T("JobDir")
#define XAB_JOB_EXE				_T("JobExe")

#define XAB_PORTMON_NAME		_T("Xab Port Monitor")
#define XAB_PORTMON_DLL			_T("XabPortMon.dll")

#define IST_PRINTER_NAME		_T("Pantum Cloud Print")	//默认的打印机驱动名
#define IST_PRINTER_DRIVER		_T("%sDriver")
#define IST_PRINTER_PORT		_T("%sPort")
#define IST_PRINTER_DATATYPE	_T("RAW")
#define IST_PRINTER_PROC		_T("%sProc")
#define IST_PRINTER_PROC_DLL	_T("%sProc.dll")
#define _PrintEnvironment		NULL

#ifdef _AMD64_
#define PTINTER_BIN_DIR _T("x64\\printer")
#else
#define PTINTER_BIN_DIR _T("x86\\printer")
#endif

#ifdef _AMD64_
#define PRODUCT_CUSTOM_REGISTY_KEY	_T("SOFTWARE\\Wow6432Node\\iSecStar") 
#else
#define PRODUCT_CUSTOM_REGISTY_KEY	_T("SOFTWARE\\iSecStar") 
#endif

#ifdef _AMD64_
#define XAB_MONITORENVNT		_T("Windows x64")
#else
#define XAB_MONITORENVNT		_T("Windows NT x86")
#endif

#define WINDOWS_SPOOL_SVR_NAME	_T("Spooler")


#endif	//_VIRTUALDRIVERDEF_3C168011_98BB_4EE1_AB4C_38DE384721CD_H_

