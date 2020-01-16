#pragma once

class _AA_DLL_EXPORT_ CFileMap2
{
public:
	CFileMap2(void);
	~CFileMap2(void);
	enum SEEKPOS
	{
		SP_BEGIN = 0,
		SP_CUR,
		SP_END,
	};
public:
	BOOL CheckHandleValid();

	//以只读打开
	BOOL OpenReadOnly(CString strPath, DWORD dwShareMode = FILE_SHARE_READ);

	//打开文件
	BOOL Open(__in DWORD flProtect,
		/*
		PAGE_READONLY Gives read-only access to the committed region of pages. An attempt to write to or execute the committed region results in an access violation. The file specified by the hFile parameter must have been created with GENERIC_READ access. You cannot specify PAGE_READONLY without a file handle. 
		PAGE_READWRITE Gives read-write access to the committed region of pages. The file specified by hFile must have been created with GENERIC_READ and GENERIC_WRITE access. 
		*/
		__in	CString strPath, 
		__in     DWORD dwDesiredAccess,
		/*
		0 Specifies device query access to the object. An application can query device attributes without accessing the device. 
		GENERIC_READ Specifies read access to the object. Data can be read from the file and the file pointer can be moved. Combine with GENERIC_WRITE for read/write access. 
		GENERIC_WRITE Specifies write access to the object. Data can be written to the file and the file pointer can be moved. Combine with GENERIC_READ for read/write access. 
		*/
		__in     DWORD dwShareMode,
		/*
		If dwShareMode is zero, the object cannot be shared.
		FILE_SHARE_READ Indicates that subsequent open operations on the object will succeed only if read access is requested. 
		FILE_SHARE_WRITE Indicates that subsequent open operations on the object will succeed only if write access is requested. 
		*/

		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in     DWORD dwCreationDisposition,
		/*
		CREATE_NEW Creates a new file. The function fails if the specified file already exists. 
		CREATE_ALWAYS Creates a new file. If the file exists, the function overwrites the file and clears the existing attributes. 
		OPEN_EXISTING Opens the file. The function fails if the file does not exist. 
		For an explanation why you should use the OPEN_EXISTING flag if you are using the CreateFile function for devices, including the console, see Remarks.
		OPEN_ALWAYS Opens the file, if it exists. If the file does not exist, the function creates the file as if dwCreationDisposition were CREATE_NEW. 
		TRUNCATE_EXISTING Opens the file. Once opened, the file is truncated so that its size is zero bytes. The calling process must open the file with at least GENERIC_WRITE access. The function fails if the file does not exist. 
		*/
		__in     DWORD dwFlagsAndAttributes);

	//创建文件
	BOOL Create(CString strPath, UINT nLength, 
		__in     DWORD dwShareMode,
		/*
		If dwShareMode is zero, the object cannot be shared.
		FILE_SHARE_READ Indicates that subsequent open operations on the object will succeed only if read access is requested. 
		FILE_SHARE_WRITE Indicates that subsequent open operations on the object will succeed only if write access is requested. 
		*/

		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in     DWORD dwCreationDisposition,
		/*
		CREATE_NEW Creates a new file. The function fails if the specified file already exists. 
		CREATE_ALWAYS Creates a new file. If the file exists, the function overwrites the file and clears the existing attributes. 
		OPEN_EXISTING Opens the file. The function fails if the file does not exist. 
		For an explanation why you should use the OPEN_EXISTING flag if you are using the CreateFile function for devices, including the console, see Remarks.
		OPEN_ALWAYS Opens the file, if it exists. If the file does not exist, the function creates the file as if dwCreationDisposition were CREATE_NEW. 
		TRUNCATE_EXISTING Opens the file. Once opened, the file is truncated so that its size is zero bytes. The calling process must open the file with at least GENERIC_WRITE access. The function fails if the file does not exist. 
		*/
		__in     DWORD dwFlagsAndAttributes);
	void Close();
	void Flush();
	//INT64 Seek(INT64 nCurPos, INT64 nSeek);
	/* I/O */
	int Read(void* pBuf, UINT nCount);
	int Write(void* pBuf, UINT nCount);

	/* Position */
	bool Seek(INT64 lOffset, SEEKPOS eseekpos);
	UINT64 GetPosition();

	/* Length */
	UINT64 GetLength();
	bool SetLength(const INT64& nLength);
private:
	bool _seek(INT64 lOffset, SEEKPOS eseekpos);
	bool _checkFileExtended();
private:
	HANDLE m_hFileMapping;
	PBYTE m_pbFile;
	DWORD m_dwBytesInView;
	INT64 m_nFileSize;
	INT64 m_nViewBegin;//from begining of file
	INT64 m_nCurPos;//from begining of file
	HANDLE m_hFile;
	DWORD m_dwAllocGranularity;
	LONG m_lExtendOnWriteLength;
	DWORD m_dwflagsFileMapping;
	DWORD m_dwflagsView;
	bool m_bFileExtended;
};