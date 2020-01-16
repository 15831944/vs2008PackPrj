#pragma once
#include <Winspool.h>
class PrinterUtils
{
public:
    //Changed printer settings with devmode and returned changed DEVMODE struct
    static PDEVMODE ChangePrinterProperties( IN CString& szPrinterName, IN PDEVMODE pSrcDevMode, IN HANDLE hPrinter );

    //Changed printer settings with devmode for current OS version.
    static VOID ChangePrinterSettingsForCurrentOS( IN CString& szPrinterName, OUT PRINTER_INFO_2& pi2, IN DEVMODE& srcDevMode, IN HANDLE hPrinter );
    
    static VOID DevmodeCopy( IN const DEVMODE& fromDevMode, OUT DEVMODE& toDevMode );

    static BOOL SetPrinterSetting( IN CString& szPrinterName, IN DEVMODE& devMode );

	static BOOL GetPrinterDevMode(CString& pszPrinterName, DEVNAMES *&pDevNames, BYTE*& pDevMode);
};

