// DiskSerial.cpp: implementation of the CDiskSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DiskSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiskSerial::CDiskSerial()
{

}

CDiskSerial::~CDiskSerial()
{

}
BOOL CDiskSerial::DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned)
{
      // Set up data structures for IDENTIFY command.
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
   pSCIP -> irDriveRegs.bFeaturesReg = 0;
   pSCIP -> irDriveRegs.bSectorCountReg = 1;
   pSCIP -> irDriveRegs.bSectorNumberReg = 1;
   pSCIP -> irDriveRegs.bCylLowReg = 0;
   pSCIP -> irDriveRegs.bCylHighReg = 0;

      // Compute the drive number.
   pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

      // The command can either be IDE identify or ATAPI identify.
   pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
   pSCIP -> bDriveNumber = bDriveNum;
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

   return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
               (LPVOID) pSCIP,
               sizeof(SENDCMDINPARAMS) - 1,
               (LPVOID) pSCOP,
               sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
               lpcbBytesReturned, NULL) );
}

int  CDiskSerial::ReadPhysicalDriveInNT(const int idrive,PDRIVE_INFO_OK buf,int buflen)
{
	int done = 0;
	int drive = 0;
	
	drive = idrive;
	HANDLE hPhysicalDriveIOCTL = 0;

    //  Try to get a handle to PhysicalDrive IOCTL, report failure
    //  and exit if can't.
    char driveName [256];

	if(idrive>=0)
	{
		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);
		
		//  Windows NT, Windows 2000, must have admin rights
		
		hPhysicalDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			sprintf (driveName, "\\\\.\\PhysicalDrive%d", i);
			
			//  Windows NT, Windows 2000, must have admin rights
			
			hPhysicalDriveIOCTL = CreateFileA (driveName,
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, 0, NULL);
			if ((hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)&&
				(hPhysicalDriveIOCTL != NULL))
			{
				break;				
			}
		}

	}

    if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
    {
		//MessageBox(0,"OK","OK",MB_YESNOCANCEL);
		GETVERSIONOUTPARAMS VersionParams;
        DWORD               cbBytesReturned = 0;

        // Get the version, etc of PhysicalDrive IOCTL
        memset ((void*) &VersionParams, 0, sizeof(VersionParams));
		
		if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
                   NULL, 
                   0,
                   &VersionParams,
                   sizeof(VersionParams),
                   &cbBytesReturned, NULL) )
        {         
			//done = 0;		//设备打开错误			
        }

        // If there is a IDE device at number "i" issue commands
        // to the device
		
        if (VersionParams.bIDEDeviceMap > 0)
        {
			BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
            SENDCMDINPARAMS  scip;
            //SENDCMDOUTPARAMS OutCmd;
			
			
			// Now, get the ID sector for all IDE devices in the system.
            // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
            // otherwise use the IDE_ATA_IDENTIFY command
            bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
                      IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

            memset (&scip, 0, sizeof(scip));
            memset (IdOutCmd, 0, sizeof(IdOutCmd));
			

            if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
                       &scip, 
                       (PSENDCMDOUTPARAMS)&IdOutCmd, 
                       (BYTE) bIDCmd,
                       (BYTE) drive,
                       &cbBytesReturned))
            {
				
				DWORD diskdata [256];
				int ijk = 0;
				USHORT *pIdSector = (USHORT *)
                             ((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;
				
				for (ijk = 0; ijk < 256; ijk++)
					diskdata [ijk] = pIdSector [ijk];

			   	//将diskdata中的硬盘信息保存到buf中
				buf->Heads = diskdata[3];
				buf->Cylinders = diskdata[1];
				buf->Sectors = diskdata[6];
				char szTmp[1024] = {0};
				ConvertToString(diskdata,23,26,szTmp);
				strcpy(buf->ControlNum , szTmp);
				memset(szTmp,0,1024);
				ConvertToString(diskdata,10,19,szTmp);
				strcpy(buf->SerialNumber , szTmp);
				memset(szTmp,0,1024);
				ConvertToString(diskdata,27,46,szTmp);
				strcpy(buf->ModalNumber , szTmp);
				
				if (diskdata [0] & 0x0080)
					buf->DriveType = 0;		//可移动驱动器
				else if (diskdata [0] & 0x0040)
					buf->DriveType = 0;		//固定驱动器

               done = -1;
            }
	    }

        CloseHandle (hPhysicalDriveIOCTL);
	}
	return done;
}

VOID ChangeByteOrder(PCHAR szString, USHORT uscStrSize)
{
    
    USHORT i;
    CHAR temp;
    
    for (i = 0; i < uscStrSize; i+=2)
    {
        temp = szString[i];
        szString[i] = szString[i+1];
        szString[i+1] = temp;
    }
}
void DetectIDE(BYTE bIDEDeviceMap)
{
 /*   if (bIDEDeviceMap&1){
        if (bIDEDeviceMap&16){
            cout<<"ATAPI device is attached to primary controller, drive 0."<<endl;
        }else{
            cout<<"IDE device is attached to primary controller, drive 0."<<endl;
        }
    }
    if (bIDEDeviceMap&2){
        if (bIDEDeviceMap&32){
            cout<<"ATAPI device is attached to primary controller, drive 1."<<endl;
        }else{
            cout<<"IDE device is attached to primary controller, drive 1."<<endl;
        }
    }
    if (bIDEDeviceMap&4){
        if (bIDEDeviceMap&64){
            cout<<"ATAPI device is attached to secondary controller, drive 0."<<endl;
        }else{
            cout<<"IDE device is attached to secondary controller, drive 0."<<endl;
        }
    }
    if (bIDEDeviceMap&8){
        if (bIDEDeviceMap&128){
            cout<<"ATAPI device is attached to secondary controller, drive 1."<<endl;
        }else{
            cout<<"IDE device is attached to secondary controller, drive 1."<<endl;
        }
    }*/
}
int  CDiskSerial::ReadDrivePortsInWin9X (int idrive, PDRIVE_INFO_OK buf,int buflen)
{
 
	ZeroMemory(&vers,sizeof(vers));
    //We start in 95/98/Me
    h=CreateFileA("\\\\.\\Smartvsd",0,0,0,CREATE_NEW,0,0);
    if (!h){
       // cout<<"open smartvsd.vxd failed"<<endl;
        exit(0);
    }
    
    if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){
       // cout<<"DeviceIoControl failed:DFP_GET_VERSION"<<endl;
        CloseHandle(h);
        return -1;
    }
    //If IDE identify command not supported, fails
    if (!(vers.fCapabilities&1)){
       // cout<<"Error: IDE identify command not supported.";
        CloseHandle(h);
        return -1;
    }
    //Display IDE drive number detected
    DetectIDE(vers.bIDEDeviceMap);
    //Identify the IDE drives
    for (j=0;j<4;j++){
        PIDSECTOR phdinfo;
        char s[41];
        
        ZeroMemory(&in,sizeof(in));
        ZeroMemory(&out,sizeof(out));
        if (j&1){
            in.irDriveRegs.bDriveHeadReg=0xb0;
        }else{
            in.irDriveRegs.bDriveHeadReg=0xa0;
        }
        if (vers.fCapabilities&(16>>j)){
            
            //We don't detect a ATAPI device.
//            cout<<"Drive "<<(int)(j+1)<<" is a ATAPI device, we don't detect it"<<endl;
            continue;
        }else{
            in.irDriveRegs.bCommandReg=0xec;
        }
        in.bDriveNumber=j;
        in.irDriveRegs.bSectorCountReg=1;
        in.irDriveRegs.bSectorNumberReg=1;
        in.cBufferSize=512;
        if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){
//            cout<<"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl;
            CloseHandle(h);
            return -1;
        }
        phdinfo=(PIDSECTOR)out.bBuffer;
        memcpy(s,phdinfo->sModelNumber,40);
        s[40]=0;
        ChangeByteOrder(s,40);
//        cout<<endl<<"Module Number:"<<s<<endl;
        memcpy(s,phdinfo->sFirmwareRev,8);
        s[8]=0;
        ChangeByteOrder(s,8);
//        cout<<"\tFirmware rev:"<<s<<endl;
        memcpy(s,phdinfo->sSerialNumber,20);
        s[20]=0;
        ChangeByteOrder(s,20);
	    strcpy(buf->SerialNumber , s);
        //cout<<"\tSerial Number:"<<s<<endl;
        //cout<<"\tCapacity:"<<phdinfo->ulTotalAddressableSectors/2/1024<<"M"<<endl<<endl;
    }
    
    //Close handle before quit
    CloseHandle(h);
	return 1;

}





int  CDiskSerial::ReadIdeDriveAsScsiDriveInNT (int controller,PDRIVE_INFO_OK buf,int buflen)
{
   int done = FALSE;
//   int controller = 0;

//   for (controller = 0; controller < 2; controller++)
//   {
      HANDLE hScsiDriveIOCTL = 0;
      char   driveName [256];

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      //sprintf (driveName, "\\\\.\\Scsi%d:", controller);

         //  Windows NT, Windows 2000, any rights should do
      hScsiDriveIOCTL = CreateFileA (driveName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
      //            controller, GetLastError ());

      if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
      {
         int drive = 0;

         for (drive = 0; drive < 2; drive++)
         {
            char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
            SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
            SENDCMDINPARAMS *pin =
                   (SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
            DWORD dummy;
   
            memset (buffer, 0, sizeof (buffer));
            p -> HeaderLength = sizeof (SRB_IO_CONTROL);
            p -> Timeout = 10000;
            p -> Length = SENDIDLENGTH;
            p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
            strncpy ((char *) p -> Signature, "SCSIDISK", 8);
  
            pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
            pin -> bDriveNumber = drive;

            if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) +
                                         sizeof (SENDCMDINPARAMS) - 1,
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
                                 &dummy, NULL))
            {
               SENDCMDOUTPARAMS *pOut =
                    (SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
               IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
               if (pId -> sModelNumber [0])
               {
                  DWORD diskdata [256];
                  int ijk = 0;
                  USHORT *pIdSector = (USHORT *) pId;
          
                  for (ijk = 0; ijk < 256; ijk++)
                     diskdata [ijk] = pIdSector [ijk];
					
				buf->Heads = diskdata[3];
				buf->Cylinders = diskdata[1];
				buf->Sectors = diskdata[6];
				char szTmp[1024] = {0};
				ConvertToString(diskdata,23,26,szTmp);
				strcpy(buf->ControlNum , szTmp);
				memset(szTmp,0,1024);
				ConvertToString(diskdata,10,19,szTmp);
				strcpy(buf->SerialNumber , szTmp);
				memset(szTmp,0,1024);
				ConvertToString(diskdata,27,46,szTmp);
				strcpy(buf->ModalNumber , szTmp);
				
				if (diskdata [0] & 0x0080)
					buf->DriveType = 0;		//可移动驱动器
				else if (diskdata [0] & 0x0040)
					buf->DriveType = 0;		//固定驱动器


                  done = TRUE;
               }
            }
         }
         CloseHandle (hScsiDriveIOCTL);
      }
 //  }

   return done;
}

char * CDiskSerial::ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex,char* string)
{
  // char string [1024];
   int index = 0;
   int position = 0;

      //  each integer has two characters stored in it backwards
   for (index = firstIndex; index <= lastIndex; index++)
   {
         //  get high byte for 1st character
      string [position] = (char) (diskdata [index] / 256);
      position++;

         //  get low byte for 2nd character
      string [position] = (char) (diskdata [index] % 256);
      position++;
   }

      //  end the string 
   string [position] = '\0';

      //  cut off the trailing blanks
   for (index = position - 1; index > 0 && ' ' == string [index]; index--)
      string [index] = '\0';

   return string;
}


#include "wbemcli.h" 
#pragma comment(lib,"wbemuuid.lib") 


CString CDiskSerial::GetDiskSerialID()
{
#if 0 //del by zfq,2014-10-23
	_DRIVER_INFO_OK buf = {0};
	CString strSerial;
	OSVERSIONINFO ov;  
	ov.dwOSVersionInfoSize = sizeof(ov); 
	GetVersionEx(&ov); 
	if (VER_PLATFORM_WIN32_NT == ov.dwPlatformId  &&  ov.dwMajorVersion >=4)
	{
		
		ReadPhysicalDriveInNT(-1,&buf,sizeof(_DRIVER_INFO_OK));
		strSerial.Format(_T("%s"),buf.SerialNumber);
	}
	else
	{
		ReadDrivePortsInWin9X(-1,&buf,sizeof(_DRIVER_INFO_OK));
		strSerial.Format(_T("%s"),buf.SerialNumber);
   	    
	}
	return strSerial;
#else
	CString strSerial = _T(""); 

	////////////////////////////////////////////////////////////////////////// 
	// 通过WMI获取硬盘序列号 
	// added by jarry 2014-06-20 
	{
		IWbemServices* pWbemServices = NULL; 
		IWbemLocator* pWbemLocator = NULL; 
		IEnumWbemClassObject *pEnumWbemObj = NULL; 
		IWbemClassObject *pWbemObj = NULL; 
		IClientSecurity* security = NULL; 
		do 
		{ 
			CoInitializeEx(NULL,0); 
			CoInitializeSecurity(NULL,-1,NULL,NULL,RPC_C_AUTHN_LEVEL_NONE,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,NULL,NULL); 

			// 创建wmi命名空间 
			if (FAILED(CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator,(LPVOID *)&pWbemLocator))) 
				break; 

			// 存储空间清空 
			WCHAR sNetWorkResName[20]; 
			memset(sNetWorkResName,0,sizeof(WCHAR) * 20); 
			CString sResName; 
			sResName.Format(_T("ROOT\\CIMV2")); 

			// 如果网络资源路径为空,则创建WMI服务 
#ifdef UNICODE 
			swprintf(sNetWorkResName, L"%s",sResName); 
#else 
			MultiByteToWideChar(CP_GB2312,0,sResName.GetBuffer(),-1,sNetWorkResName,sResName.GetLength() + 1); 
			sResName.ReleaseBuffer(-1); 
#endif 
			if (FAILED(pWbemLocator->ConnectServer(sNetWorkResName,NULL,NULL,0,NULL,0,0,&pWbemServices))) 
				break; 

			//设置WMI服务的安全级别 
			if (FAILED(pWbemServices->QueryInterface(IID_IClientSecurity, (PVOID*) &security))) 
				break; 

			if (FAILED(security->SetBlanket(pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,NULL, RPC_C_AUTHN_LEVEL_CONNECT, RPC_C_IMP_LEVEL_IMPERSONATE,NULL, EOAC_NONE))) 
				break; 

			WCHAR pSQLName[100]; 
			WCHAR pSQLString[250]; 
			sResName.Format(_T("WQL")); 
#ifdef UNICODE 
			swprintf(pSQLName, L"%s",sResName); 
#else 
			MultiByteToWideChar(CP_GB2312,0,sResName.GetBuffer(),-1,pSQLName,sResName.GetLength() + 1); 
			sResName.ReleaseBuffer(-1); 
#endif 

			// 得到查询语句 
			sResName.Format(_T("SELECT * FROM Win32_DiskDrive")); 
#ifdef UNICODE 
			swprintf(pSQLString, L"%s",sResName); 
#else 
			MultiByteToWideChar(CP_GB2312,0,sResName.GetBuffer(),-1,pSQLString,sResName.GetLength() + 1); 
			sResName.ReleaseBuffer(-1); 
#endif 

			// 执行得到查询结果 
			if (FAILED(pWbemServices->ExecQuery(pSQLName,pSQLString,WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumWbemObj))) 
				break; 

			// 得到值 
			ULONG uRet = 0; 
			while(pEnumWbemObj) 
			{ 
				if (FAILED(pEnumWbemObj->Next(WBEM_INFINITE,1,&pWbemObj,&uRet)) || !uRet) 
					break; 

				VARIANT variant; 
				VariantInit(&variant); 
				sResName.Format(_T("PNPDeviceID")); 
#ifdef UNICODE 
				swprintf(pSQLName, L"%s",sResName); 
#else 
				MultiByteToWideChar(CP_GB2312,0,sResName.GetBuffer(),-1,pSQLName,sResName.GetLength() + 1); 
				sResName.ReleaseBuffer(-1); 
#endif 

				// 返回正确值 
				if (SUCCEEDED(pWbemObj->Get(pSQLName,0,&variant,0,0)) && V_VT(&variant)!=VT_NULL) 
				{ 
					char pMsg[MAX_PATH]; 
					memset(pMsg,0,sizeof(char) * MAX_PATH); 
					WideCharToMultiByte(CP_GB2312,0,variant.bstrVal,wcslen(variant.bstrVal) + 1,pMsg, 2* (wcslen(variant.bstrVal) + 1),NULL,NULL); 
					std::string stdCode(pMsg); 
//					strSerial = CCommonFun::TranStdStringToCString(stdCode); 
					strSerial = stdCode.c_str(); 
				} 
				break; 
			} 
		} while (0); 

		strSerial.Replace('\\','/'); 
		if(strSerial.GetLength() > 50) 
		{ 
			CString sTemp = strSerial; 
			strSerial.Format(_T("%s%s"),sTemp.Left(30),sTemp.Right(19)); 
		} 
		if (security) 
			security->Release(); 
		if (pEnumWbemObj) 
			pEnumWbemObj->Release(); 
		if (pWbemLocator) 
			pWbemLocator->Release(); 
		if (pWbemServices) 
			pWbemServices->Release(); 
		CoUninitialize(); 
	} 

	////////////////////////////////////////////////////////////////////////// 
	// 如果上述方案获取失败 
	if (strSerial.IsEmpty()) 
	{ 
		_DRIVER_INFO_OK buf = {0}; 
		OSVERSIONINFO ov; 
		ov.dwOSVersionInfoSize = sizeof(ov); 
		GetVersionEx(&ov); 
		if (VER_PLATFORM_WIN32_NT == ov.dwPlatformId && ov.dwMajorVersion >=4) 
		{ 

			ReadPhysicalDriveInNT(-1,&buf,sizeof(_DRIVER_INFO_OK)); 
			strSerial.Format(_T("%s"),buf.SerialNumber); 
			strSerial.TrimLeft(); 
			strSerial.TrimRight(); 
			if (strSerial.IsEmpty()) 
			{ 
				memset(&buf,0,sizeof(buf)); 
				ReadIdeDriveAsScsiDriveInNT(0,&buf,sizeof(buf)); 
				strSerial.Format(_T("%s"),buf.SerialNumber); 

			} 
		} 
		else 
		{ 
			ReadDrivePortsInWin9X(-1,&buf,sizeof(_DRIVER_INFO_OK)); 
			strSerial.Format(_T("%s"),buf.SerialNumber); 

		} 
	} 
	return strSerial; 

#endif
}
