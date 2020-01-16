#ifndef _COMMONLIB_PORT_HEADER
#define _COMMONLIB_PORT_HEADER
// CommonLibPort.h: 
//
//////////////////////////////////////////////////////////////////////

#include "..\aCmmLib\Registry2.h"
#include "..\aCmmLib\CommonFun.h"
#include "..\aCmmLib\SecurityTool.h"
#include "..\aCmmLib\NtDriverController.h"
#include "..\aCmmLib\DiskSerial.h"
#include "..\aCmmLib\Base64.h"
#include "..\aCmmLib\XmlParse.h"
#include "..\aCmmLib\IniFile.h"
#include "..\aCmmLib\RegistryEx.h"
#include "..\aCmmLib\Allocator.h"
#include "..\aCmmLib\Filelog.h"
#include "../aCmmLib/ThreadMgr.h"	//add by zfq
#include "..\aCmmLib\MiniDump.h"
#include "..\aCmmLib\IPRange.h"
#include "..\aCmmLib\access_control.h"
#include "..\aCmmLib\XSharedMemoryQueue.h"
#include "..\aCmmLib\listctrl2.h"
#include "..\aCmmLib\Edit2.h"
#include "..\aCmmLib\TreeListWnd.h"
#include "..\aCmmLib\richeditctrl2.h"
#include "..\aCmmLib\XListCtrl.h"
#include "..\aCmmLib\PageQueryWnd.h"

#include "..\aCmmLib\ping1.h"
#include "..\aCmmLib\processexp.h"
#include "..\aCmmLib\SysTrayInfo.h"
#include "..\aCmmLib\WinUserMgr.h"
#include "..\aCmmLib\FileMap2.h"
#include "..\aCmmLib\TextProgressCtrl.h"
#include "..\aCmmLib\Service.h"
#include "..\aCmmLib\HdAcceleFun.h"

#include "..\aCmmLib\winfirewallop.h"

#include "..\aCmmLib\FileVersionInfo.h"
#include "..\aCmmLib\HtmlHelp2.h"
#include "..\aCmmLib\XMessageBox.h"
#include "..\Public\Include\CmmInterface.h"
#include "..\aCmmLib\PipeSvrThread.h"
#include "..\aCmmLib\iis.h"
#include "..\aCmmLib\iisweb.h"
#include "..\aCmmLib\iisftp.h"
#include "..\aCmmLib\BanCommand.h"

#include "../aCmmLib/ParseShd.h"
#include "../aCmmLib/ParseSpl.h"
#include "../aCmmLib/CHash.h"

#ifdef _DEBUG
	#pragma comment(lib,"..\\Debug\\aCmmLib.lib")
#else
	#pragma comment(lib,"..\\Release\\aCmmLib.lib")
#endif


#endif