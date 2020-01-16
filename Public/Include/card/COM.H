#ifndef  __COMMTREAD_H
#define  __COMMTREAD_H

//#include "StdAfx.h" 

#define GWL_PGPSINFO 0 
#define GPSEXTRABYTES sizeof( LONG ) 

#define MAXPORTS 4 

#define CN_SEND WM_USER+100 

#define RXQUEUE 4096 
#define TXQUEUE 4096 

// cursor states 

#define CS_HIDE 0x00 
#define CS_SHOW 0x01 

// Flow control flags 

#define FC_DTRDSR 0x01 
#define FC_RTSCTS 0x02 
#define FC_XONXOFF 0x04 

// ascii definitions 

#define ASCII_BEL 0x07 
#define ASCII_BS 0x08 
#define ASCII_LF 0x0A 
#define ASCII_CR 0x0D 
#define ASCII_XON 0x11 
#define ASCII_XOFF 0x13 

//function registed by user
typedef void (* SERIAL_FUNC)(long nLength,unsigned char *abIn) ;

// data structures 


typedef struct tagGPSINFO 
{ 
	HANDLE idComDev; 
	BYTE bPort; 
	BOOL fConnected; 
	BYTE bByteSize,bParity,bStopBits; 
	DWORD dwBaudRate; 

	HANDLE hPostEvent,hWatchThread,hWatchEvent; 
	HWND hTermWnd; 
	DWORD dwThreadID; 
	OVERLAPPED osWrite,osRead; 
} GPSINFO, *PGPSINFO ; 
#define COMDEV( x ) (x -> idComDev) 
#define PORT( x ) (x -> bPort) 
#define CONNECTED( x ) (x -> fConnected) 
#define BYTESIZE( x ) (x -> bByteSize) 
#define PARITY( x ) (x -> bParity) 
#define STOPBITS( x ) (x -> bStopBits) 
#define BAUDRATE( x ) (x -> dwBaudRate) 

#define POSTEVENT( x ) (x -> hPostEvent) 
#define HTHREAD( x ) (x -> hWatchThread) 
#define THREADID( x ) (x -> dwThreadID) 
#define WRITE_OS( x ) (x -> osWrite) 
#define READ_OS( x ) (x -> osRead) 

// function prototypes (private) 
void def_func(long nLength,unsigned char *abIn);

//LRESULT NEAR CreateGPSInfo(HWND,BYTE nPort=1); 

LRESULT NEAR CreateGPSInfo(SERIAL_FUNC func = def_func,BYTE nPort=1,long nBaud = 9600); 

BOOL NEAR DestroyGPSInfo(); 

int NEAR ReadCommBlock(LPSTR,int); 
BOOL NEAR WriteCommBlock(LPSTR,DWORD); 
BOOL NEAR OpenConnection(); 
BOOL NEAR SetupConnection(); 
BOOL NEAR CloseConnection(); 

// function prototypes (public) 

DWORD FAR PASCAL CommWatchProc(LPSTR); 

int CommOpen(SERIAL_FUNC func,BYTE nPort,long nBaud);

int CommClose();

int ReadComm(LPSTR lpByte,DWORD dwBytesToRead);

int WriteComm(LPSTR lpByte,DWORD dwBytesToWrite);




#endif


