#pragma once
#define ISEC_HOOK_PRINT_MAP_NAME				_T("Global\\{B2E829D8-0F91-4347-BD7C-10DB2563E5D4}")
#define ISEC_HOOK_PRINT_MUTEX_NAME				_T("Global\\{BBE243E3-2451-4165-9CC4-CE53D0309A86}")
#define ISEC_HOOK_PRINT_PIPE					_T("\\\\.\\pipe\\{4E20768B-22E0-4553-80C7-FE2EE7FB2A02}")
#define WM_PRINT_DOC_EVENT	WM_USER + 10000

#pragma pack(push, 1)
#define DOC_LOG_SIZE 1024
class CMapData
{
public:
	int m_nSize;
	int m_nUsed;
	BYTE m_bIndexAry[DOC_LOG_SIZE];
	char m_szDocType[1024];
};

class CDocumentLogNode
{
public:
	char m_szDocName[MAX_PATH + 1];
	char m_szProcess[MAX_PATH + 1];
	char m_szPath[MAX_PATH + 1];
	HANDLE m_hFile;
	DWORD m_dwPId;
};

#pragma pack(pop)