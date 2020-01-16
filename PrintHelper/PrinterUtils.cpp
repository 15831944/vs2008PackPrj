#include "stdafx.h"
#include "PrinterUtils.h"
#include "WinVerUtils.h"
#define PRINT(message) theLog.Write(message)

#define PRINT_ERROR(error) 
	

//using namespace PrinterUtils;

PDEVMODE PrinterUtils::ChangePrinterProperties(CString& szPrinterName, PDEVMODE pSrcDevMode, HANDLE hPrinter)
{
    DWORD dwNeeded = 0;
    PDEVMODE pDevMode = NULL;

    if( !hPrinter )
    {
        if ( !::OpenPrinter(szPrinterName.GetBuffer(), &hPrinter, NULL) )
        {
            PRINT("Error: OpenPrinter failed");
            PRINT_ERROR(::GetLastError());
            return NULL;
        }
    }

    dwNeeded = ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), NULL, NULL, 0);

    pDevMode = (PDEVMODE)::malloc(dwNeeded);
    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, NULL, DM_OUT_BUFFER) )
    {
        ::free(pDevMode);
        ::ClosePrinter(hPrinter);
        PRINT(_T("Error: DocumentProperties failed"));

        return NULL;
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_ORIENTATION;
    pDevMode->dmOrientation = pSrcDevMode->dmOrientation;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_ORIENTATION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERSIZE;
    pDevMode->dmPaperSize = pSrcDevMode->dmPaperSize;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_PAPERSIZE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERWIDTH;
    pDevMode->dmPaperWidth = pSrcDevMode->dmPaperWidth;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_PAPERWIDTH"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERLENGTH;
    pDevMode->dmPaperLength = pSrcDevMode->dmPaperLength;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_PAPERLENGTH"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_SCALE;
    pDevMode->dmScale = pSrcDevMode->dmScale;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_SCALE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COPIES;
    pDevMode->dmCopies = pSrcDevMode->dmCopies;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_COPIES"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PRINTQUALITY;
    pDevMode->dmPrintQuality = pSrcDevMode->dmPrintQuality;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_PRINTQUALITY"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COLOR;
    pDevMode->dmColor = pSrcDevMode->dmColor;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_COLOR"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_DUPLEX;
    pDevMode->dmDuplex = pSrcDevMode->dmDuplex;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_DUPLEX"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_YRESOLUTION;
    pDevMode->dmYResolution = pSrcDevMode->dmYResolution;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_YRESOLUTION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_TTOPTION;
    pDevMode->dmTTOption = pSrcDevMode->dmTTOption;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_TTOPTION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COLLATE;
    pDevMode->dmCollate = pSrcDevMode->dmCollate;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_COLLATE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_FORMNAME;
    ::memcpy(pDevMode->dmFormName, pSrcDevMode->dmFormName, sizeof(pSrcDevMode->dmFormName));

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_FORMNAME"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_MEDIATYPE;
    pDevMode->dmMediaType = pSrcDevMode->dmMediaType;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_MEDIATYPE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_DITHERTYPE;
    pDevMode->dmDitherType = pSrcDevMode->dmDitherType;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        PRINT(_T("Fails setting DM_DITHERTYPE"));
    }

	pDevMode->dmFields |= pSrcDevMode->dmFields & DM_ICMINTENT;
	pDevMode->dmICMIntent = pSrcDevMode->dmICMIntent;

	if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
	{
		PRINT(_T("Fails setting DM_DITHERTYPE"));
	}

	pDevMode->dmFields |= pSrcDevMode->dmFields & DM_NUP;
	pDevMode->dmNup = pSrcDevMode->dmNup;

	if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
	{
		PRINT(_T("Fails setting DM_DITHERTYPE"));
	}

	pDevMode->dmFields |= pSrcDevMode->dmFields & DM_ICMMETHOD;
	pDevMode->dmICMMethod = pSrcDevMode->dmICMMethod ;

	if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
	{
		PRINT(_T("Fails setting DM_DITHERTYPE"));
	}

    ::ClosePrinter(hPrinter);

    return pDevMode;

    

}

VOID PrinterUtils::ChangePrinterSettingsForCurrentOS( IN CString& szPrinterName, OUT PRINTER_INFO_2& pi2, IN DEVMODE& srcDevmode, IN HANDLE hPrinter )
{
    PDEVMODE pDevmodeSupport = NULL;

    if( WinVerUtils::IsWin2k() )
    {
        PRINT(_T("Set settings for Win2k"));

        //::memcpy(pi2.pDevMode, &srcDevmode, sizeof(srcDevmode));
        DevmodeCopy(srcDevmode, *pi2.pDevMode);
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, hPrinter);
    }
    else if( WinVerUtils::IsWin2k3OrLower() )
    {
        PRINT(_T("Set settings for Win2k3 or lower"));

        pi2.pDevMode = &srcDevmode;
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, NULL);

        if ( pDevmodeSupport )
        {
            pi2.pSecurityDescriptor = NULL; // Do not attempt to set security descriptor...
            pi2.pDevMode->dmFields = pDevmodeSupport->dmFields;
            pi2.pDevMode->dmDriverExtra = 0;
        }
    }
    else if( WinVerUtils::IsWin2008R2() )
    {
        PRINT(_T("Set settings for Win2008 R2"));
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, NULL);
        if ( pDevmodeSupport )
        {
            pi2.pDevMode = &srcDevmode;
            pi2.pDevMode->dmFields = pDevmodeSupport->dmFields;
        }
    }
    else
    {
        DevmodeCopy(srcDevmode, *pi2.pDevMode);
    }

    if ( pDevmodeSupport )
    {
        ::free(pDevmodeSupport);
    }
}

VOID PrinterUtils::DevmodeCopy( IN const DEVMODE& fromDevMode, OUT DEVMODE& toDevMode )
{
    ::ZeroMemory(&toDevMode, sizeof(toDevMode));

    ::memcpy(toDevMode.dmDeviceName, fromDevMode.dmDeviceName, sizeof(fromDevMode.dmDeviceName));
    ::memcpy(toDevMode.dmFormName,   fromDevMode.dmFormName,   sizeof(fromDevMode.dmFormName));

    toDevMode.dmBitsPerPel         = fromDevMode.dmBitsPerPel;
    toDevMode.dmCollate            = fromDevMode.dmCollate;
    toDevMode.dmColor              = fromDevMode.dmColor;
    toDevMode.dmCopies             = fromDevMode.dmCopies;
    toDevMode.dmDefaultSource      = fromDevMode.dmDefaultSource;
    toDevMode.dmDisplayFixedOutput = fromDevMode.dmDisplayFixedOutput;
    toDevMode.dmDisplayFlags       = fromDevMode.dmDisplayFlags;
    toDevMode.dmDisplayFrequency   = fromDevMode.dmDisplayFrequency;
    toDevMode.dmDisplayOrientation = fromDevMode.dmDisplayOrientation;
    toDevMode.dmDriverExtra        = fromDevMode.dmDriverExtra;
    toDevMode.dmDriverVersion      = fromDevMode.dmDriverVersion;
    toDevMode.dmDuplex             = fromDevMode.dmDuplex;
    toDevMode.dmFields             = fromDevMode.dmFields;
    toDevMode.dmLogPixels          = fromDevMode.dmLogPixels;
    toDevMode.dmNup                = fromDevMode.dmNup;
    toDevMode.dmOrientation        = fromDevMode.dmOrientation;
    toDevMode.dmPaperLength        = fromDevMode.dmPaperLength;
    toDevMode.dmPaperSize          = fromDevMode.dmPaperSize;
    toDevMode.dmPaperWidth         = fromDevMode.dmPaperWidth;
    toDevMode.dmPrintQuality       = fromDevMode.dmPrintQuality;
    toDevMode.dmSpecVersion        = fromDevMode.dmSpecVersion;
    toDevMode.dmTTOption           = fromDevMode.dmTTOption;
    toDevMode.dmPelsHeight         = fromDevMode.dmPelsHeight;
    toDevMode.dmPelsWidth          = fromDevMode.dmPelsWidth;
    toDevMode.dmScale              = fromDevMode.dmScale;
    toDevMode.dmSize               = fromDevMode.dmSize;
    toDevMode.dmYResolution        = fromDevMode.dmYResolution;
    toDevMode.dmPosition.x         = fromDevMode.dmPosition.x;
    toDevMode.dmPosition.y         = fromDevMode.dmPosition.y;
    toDevMode.dmDitherType         = fromDevMode.dmDitherType;
    toDevMode.dmReserved1          = fromDevMode.dmReserved1;
    toDevMode.dmReserved2          = fromDevMode.dmReserved2;
    toDevMode.dmPanningHeight      = fromDevMode.dmPanningHeight;
    toDevMode.dmPanningWidth       = fromDevMode.dmPanningWidth;
    toDevMode.dmICMIntent          = fromDevMode.dmICMIntent;
    toDevMode.dmICMMethod          = fromDevMode.dmICMMethod;
    toDevMode.dmMediaType          = fromDevMode.dmMediaType;
}

BOOL PrinterUtils::SetPrinterSetting( IN CString& szPrinterName, IN DEVMODE& devMode)
{
    HANDLE hPrinter = NULL;
    PRINTER_DEFAULTS pd;
    ::ZeroMemory(&pd, sizeof(pd));
    pd.DesiredAccess = PRINTER_ALL_ACCESS;

    // Open a handle to the printer
    BOOL bFlag = ::OpenPrinter( szPrinterName.GetBuffer(), &hPrinter, &pd );
    if ( (FALSE == bFlag) || (NULL == hPrinter) )
    {
        PRINT(_T("ERROR: Can't open printer"));
        PRINT_ERROR(::GetLastError());

        return FALSE;
    }

    DWORD dwNeeded = 0;
    // Getting buffer size
    bFlag = ::GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
    if ( (FALSE == bFlag) && (ERROR_INSUFFICIENT_BUFFER != ::GetLastError()) || (0 == dwNeeded) )
    {
        PRINT(_T("ERROR: Can't get pi2 struct size"));
        PRINT_ERROR(::GetLastError());

        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Allocate a buffer of the correct size.
    PRINTER_INFO_2 *pi2 = (PRINTER_INFO_2 *)::GlobalAlloc(GPTR,dwNeeded);
    if ( NULL == pi2 )
    {
        PRINT(_T("ERROR: Can't get malloc the buffer"));
        PRINT_ERROR(::GetLastError());

        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Getting PRINTER_INFO_2
    bFlag = ::GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
    if ( FALSE == bFlag )
    {
        PRINT(_T("ERROR: Can't call GetPrinter get PRINTER_INFO_2"));
        PRINT_ERROR(::GetLastError());

        ::GlobalFree(pi2);
        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Modify PRINTER_INFO_2 for current OS
    ChangePrinterSettingsForCurrentOS(szPrinterName, *pi2, devMode, hPrinter);

    // Update printer information
    bFlag = ::SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
    if ( FALSE == bFlag )
    {
        PRINT(_T("Can't call SetPrinter"));
        PRINT_ERROR(::GetLastError());
    }

    // Tell other apps that there was a change
    SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L, (LPARAM)szPrinterName.GetBuffer(), SMTO_NORMAL, 1000, NULL);

    // Clean up
    ::GlobalFree(pi2);
    ::ClosePrinter(hPrinter);

    return bFlag;
}

BOOL PrinterUtils::GetPrinterDevMode(CString& pszPrinterName, DEVNAMES *&pDevNames, BYTE*& pDevMode)
{
	BOOL bRet = FALSE;
	// Open printer
	HANDLE hPrinter;
	if (OpenPrinter(pszPrinterName.GetBuffer(), &hPrinter, NULL) == FALSE)
	{
		theLog.Write("OpenPrinter %s err ", pszPrinterName);
		return FALSE;
	}
	// obtain PRINTER_INFO_2 structure and close printer
	DWORD dwBytesReturned, dwBytesNeeded;
	GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
	PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)new BYTE[dwBytesNeeded];
	
	if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded,
		&dwBytesReturned) == 0) 
	{
		delete p2;
		p2 = 0;
		theLog.Write("GetPrinter err ");
		ClosePrinter(hPrinter);
		return FALSE;
	}
	
	DEVMODE* pDev = 0;
	do 
	{
		//远程打印机没有权限打开
		if(p2->pDevMode)
		{
			// Allocate a global handle for DEVMODE
			pDevMode = new BYTE[sizeof(*p2->pDevMode) +
				p2->pDevMode->dmDriverExtra];

			// copy DEVMODE data from PRINTER_INFO_2::pDevMode
			memcpy(pDevMode, p2->pDevMode, sizeof(*p2->pDevMode) +
				p2->pDevMode->dmDriverExtra);


			// Compute size of DEVNAMES structure from PRINTER_INFO_2's data
			DWORD drvNameLen = lstrlen(p2->pDriverName)+1;  // driver name
			DWORD ptrNameLen = lstrlen(p2->pPrinterName)+1; // printer name
			DWORD porNameLen = lstrlen(p2->pPortName)+1;    // port name

			// Allocate a global handle big enough to hold DEVNAMES.
			pDevNames = (DEVNAMES*)new BYTE[ sizeof(DEVNAMES) +
				(drvNameLen + ptrNameLen + porNameLen)*sizeof(TCHAR)];


			// Copy the DEVNAMES information from PRINTER_INFO_2
			// tcOffset = TCHAR Offset into structure
			int tcOffset = sizeof(DEVNAMES)/sizeof(TCHAR);
			ASSERT(sizeof(DEVNAMES) == tcOffset*sizeof(TCHAR));

			pDevNames->wDriverOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName,
				drvNameLen*sizeof(TCHAR));
			tcOffset += drvNameLen;

			pDevNames->wDeviceOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName,
				ptrNameLen*sizeof(TCHAR));
			tcOffset += ptrNameLen;

			pDevNames->wOutputOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName,
				porNameLen*sizeof(TCHAR));
			pDevNames->wDefault = 0;

			bRet = TRUE;
			break;
		}
		
		theLog.Write("remote printer,no right,so check by doc");
		//-----------------------------
		//
		
		DWORD dwNeeded = ::DocumentProperties(NULL, hPrinter, pszPrinterName.GetBuffer(), NULL, NULL, 0);
		pDev = (DEVMODE*)new BYTE[dwNeeded];
		if ( IDOK != ::DocumentProperties(NULL, hPrinter, pszPrinterName.GetBuffer(), (DEVMODE*)pDev, NULL, DM_OUT_BUFFER) )
		{
			theLog.Write(_T("Error: DocumentProperties failed"));
			break;
		}
		else
		{
			// Allocate a global handle for DEVMODE
			pDevMode = new BYTE[sizeof(*pDev) +
				pDev->dmDriverExtra];

			// copy DEVMODE data from PRINTER_INFO_2::pDevMode
			memcpy(pDevMode, pDev, sizeof(*pDev) +
				pDev->dmDriverExtra);


			// Compute size of DEVNAMES structure from PRINTER_INFO_2's data
			DWORD drvNameLen = lstrlen(p2->pDriverName)+1;  // driver name
			DWORD ptrNameLen = lstrlen(p2->pPrinterName)+1; // printer name
			DWORD porNameLen = lstrlen(p2->pPortName)+1;    // port name

			// Allocate a global handle big enough to hold DEVNAMES.
			pDevNames = (DEVNAMES*)new BYTE[ sizeof(DEVNAMES) +
				(drvNameLen + ptrNameLen + porNameLen)*sizeof(TCHAR)];


			// Copy the DEVNAMES information from PRINTER_INFO_2
			// tcOffset = TCHAR Offset into structure
			int tcOffset = sizeof(DEVNAMES)/sizeof(TCHAR);
			ASSERT(sizeof(DEVNAMES) == tcOffset*sizeof(TCHAR));

			pDevNames->wDriverOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName,
				drvNameLen*sizeof(TCHAR));
			tcOffset += drvNameLen;

			pDevNames->wDeviceOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName,
				ptrNameLen*sizeof(TCHAR));
			tcOffset += ptrNameLen;

			pDevNames->wOutputOffset = tcOffset;
			memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName,
				porNameLen*sizeof(TCHAR));
			pDevNames->wDefault = 0;

			bRet = TRUE;
		}
		
	} while (0);
	
	if(pDev)
	{
		delete pDev;
	}
	
	if(p2)
	{
		delete p2;
	}
	ClosePrinter(hPrinter);
	return bRet;
}