//******************************************
//纯C++的Socket访问 Http封装类，Neeao修改
//http://neeao.com
//2009-08-25
//******************************************

#include "stdafx.h"
#include "Request.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Request::Request()
{

}

Request::~Request()
{

}

//*******************************************************************************************************
//MemBufferCreate:
// Passed a MemBuffer structure, will allocate a memory buffer
// of MEM_BUFFER_SIZE. This buffer can then grow as needed.
//*******************************************************************************************************
void Request::MemBufferCreate(MemBuffer *b)
{
	b->size = MEM_BUFFER_SIZE;
	b->buffer =(unsigned char *) malloc( b->size );
	b->position = b->buffer;
}

//*******************************************************************************************************
// MemBufferGrow:
// Double the size of the buffer that was passed to this function.
//*******************************************************************************************************
void Request::MemBufferGrow(MemBuffer *b)
{
	size_t sz;
	sz = b->position - b->buffer;
	b->size = b->size *2;
	b->buffer =(unsigned char *) realloc(b->buffer,b->size);
	b->position = b->buffer + sz; // readjust current position
}

//*******************************************************************************************************
// MemBufferAddByte:
// Add a single byte to the memory buffer, grow if needed.
//*******************************************************************************************************
void Request::MemBufferAddByte(MemBuffer *b,unsigned char byt)
{
	if( (size_t)(b->position-b->buffer) >= b->size )
		MemBufferGrow(b);

	*(b->position++) = byt;
}

//*******************************************************************************************************
// MemBufferAddBuffer:
// Add a range of bytes to the memory buffer, grow if needed.
//*******************************************************************************************************
void Request::MemBufferAddBuffer(MemBuffer *b,
								 unsigned char *buffer, size_t size)
{
	while( ((size_t)(b->position-b->buffer)+size) >= b->size )
		MemBufferGrow(b);
	memcpy(b->position,buffer,size);
	b->position+=size;
}

//*******************************************************************************************************
// GetHostAddress:
// Resolve using DNS or similar(WINS,etc) the IP
// address for a domain name such as www.wdj.com.
//*******************************************************************************************************
DWORD Request::GetHostAddress(LPCSTR host)
{
	struct hostent *phe;
	char *p;
	phe = gethostbyname( host );

	if(phe==NULL)
		return 0;

	p = *phe->h_addr_list;
	return *((DWORD*)p);
}

//*******************************************************************************************************
// SendString:
// Send a string(null terminated) over the specified socket.
//*******************************************************************************************************
void Request::SendString(SOCKET sock,LPCSTR str)
{
	send(sock,str,strlen(str),0);
}

//*******************************************************************************************************
// ValidHostChar:
// Return TRUE if the specified character is valid
// for a host name, i.e. A-Z or 0-9 or -.:
//*******************************************************************************************************
BOOL Request::ValidHostChar(char ch)
{
	return( isalpha(ch) || isdigit(ch)
		|| ch=='-' || ch=='.' || ch==':' );
}

//*******************************************************************************************************
// ParseURL:
// Used to break apart a URL such as
// http://www.localhost.com:80/TestPost.htm into protocol, port, host and request.
//*******************************************************************************************************
void Request::ParseURL(string url,LPSTR protocol,int lprotocol,LPSTR host,int lhost,LPSTR request,int lrequest,int *port)
{
	char *work,*ptr,*ptr2;

	*protocol = *host = *request = 0;
	*port=80;

	work = strdup(url.c_str());
	strupr(work);

	ptr = strchr(work,':'); // find protocol if any
	if(ptr!=NULL)
	{
		*(ptr++) = 0;
		lstrcpyn(protocol,work,lprotocol);
	}
	else
	{
		lstrcpyn(protocol,"HTTP",lprotocol);
		ptr = work;
	}
	if( (*ptr=='/') && (*(ptr+1)=='/') ) // skip past opening /'s
		ptr+=2;
	ptr2 = ptr; // find host
	while( ValidHostChar(*ptr2) && *ptr2 )
		ptr2++;
	*ptr2=0;
	lstrcpyn(host,ptr,lhost);
	lstrcpyn(request,url.c_str() + (ptr2-work),lrequest); // find the request
	ptr = strchr(host,':'); // find the port number, if any
	if(ptr!=NULL)
	{
		*ptr=0;
		*port = atoi(ptr+1);
	}
	free(work);
}
string UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

//CP_ACP=ANSI,CP_UTF8=utf-8
CString UTF8Convert(CString &str,int sourceCodepage,int targetCodepage)
{
	int len=str.GetLength(); 
	int unicodeLen=MultiByteToWideChar(sourceCodepage,0,(LPCSTR)str.GetBuffer(),-1,NULL,0); 
	str.ReleaseBuffer();
	wchar_t * pUnicode; 
	pUnicode=new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	MultiByteToWideChar(sourceCodepage,0,(LPCSTR)str.GetBuffer(),-1,(LPWSTR)pUnicode,unicodeLen); 
	str.ReleaseBuffer();
	BYTE * pTargetData; 
	int targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(LPSTR)NULL,0,NULL,NULL); 
	pTargetData=new BYTE[targetLen+1]; 
	memset(pTargetData,0,targetLen+1); 
	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(LPSTR)pTargetData,targetLen,NULL,NULL); 
	CString rt; 
	rt.Format(_T("%s"),pTargetData); 
	delete pUnicode; 
	delete pTargetData; 
	return rt; 
} 

//*******************************************************************************************************
// SendHTTP:
// Main entry point for this code.
// url - The URL to GET/POST to/from.
// headerSend - Headers to be sent to the server.
// post - Data to be posted to the server, NULL if GET.
// postLength - Length of data to post.
// req - Contains the message and headerSend sent by the server.
//
// returns 1 on failure, 0 on success.
//*******************************************************************************************************
int Request::SendHTTP(string url,LPCSTR headerReceive,BYTE *post,
					  DWORD postLength,HTTPRequest *req)
{
	WSADATA WsaData;
	SOCKADDR_IN sin;
	SOCKET sock;
	char buffer[512];
	char protocol[20],host[256],request[1024];
	int l,port,chars,err;
	MemBuffer headersBuffer,messageBuffer;
	char headerSend[1024];
	BOOL done;




	ParseURL(url,protocol,sizeof(protocol),host,sizeof(host), // Parse the URL
		request,sizeof(request),&port);
	if(strcmp(protocol,"HTTP"))
	{
		//WriteLogEx("!!Request::SendHTTP,1,protocol=%s", protocol);
		return 1;
	}

	err = WSAStartup (0x0101, &WsaData); // Init Winsock
	if(err!=0)
	{
		//WriteLogEx("!!Request::SendHTTP,2,WSAStartup fail,err=%d", GetLastError());
		return 1;
	}

	sock = socket (AF_INET, SOCK_STREAM, 0);
	//if (socket == INVALID_SOCKET)
	if (sock == INVALID_SOCKET)
	{
		//WriteLogEx("!!Request::SendHTTP,3,socket fail,err=%d", GetLastError());
		return 1;
	}

	sin.sin_family = AF_INET; //Connect to web sever
	sin.sin_port = htons( (unsigned short)port );
	sin.sin_addr.s_addr = GetHostAddress(host);

	if( connect (sock,(LPSOCKADDR)&sin, sizeof(SOCKADDR_IN) ) )
	{
		//WriteLogEx("!!Request::SendHTTP,4,connect fail,err=%d,host=%s,port=%d"
//			, GetLastError(), host, port);
		return 1;
	}

	//WriteLogEx("Request::SendHTTP,5");


	if( !*request )
		lstrcpyn(request,"/",sizeof(request));

	if( post == NULL )
	{
		SendString(sock,"GET ");
		strcpy(headerSend, "GET ");
	}
	else
	{
		SendString(sock,"POST ");
		strcpy(headerSend, "POST ");
	}

	//WriteLogEx("Request::SendHTTP,6");

	SendString(sock,request);
	strcat(headerSend, request);

	SendString(sock," HTTP/1.0\r\n");
	strcat(headerSend, " HTTP/1.0\r\n");

	SendString(sock,"Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	strcat(headerSend, "Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	SendString(sock,"Accept-Language: en-us\r\n");
	strcat(headerSend, "Accept-Language: en-us\r\n");

	SendString(sock,"Accept-Encoding: gzip, default\r\n");
	strcat(headerSend, "Accept-Encoding: gzip, default\r\n");
	SendString(sock,"User-Agent: Neeao/4.0\r\n");
	strcat(headerSend, "User-Agent: Neeao/4.0\r\n");

	//WriteLogEx("Request::SendHTTP,8");

	if(postLength)
	{
		sprintf(buffer,"Content-Length: %ld\r\n",postLength);
		SendString(sock,buffer);
		strcat(headerSend, buffer);
	}
	//SendString(sock,"Cookie: mycookie=blablabla\r\n");
	// printf("Cookie: mycookie=blablabla\r\n");
	SendString(sock,"Host: ");
	strcat(headerSend, "Host: ");

	SendString(sock,host);
	strcat(headerSend, host);

	SendString(sock,"\r\n");
	strcat(headerSend, "\r\n");

	if( (headerReceive!=NULL) && *headerReceive )
	{
		SendString(sock,headerReceive);
		strcat(headerSend, headerReceive);
	}

	//WriteLogEx("Request::SendHTTP,10");

	SendString(sock,"\r\n"); // Send a blank line to signal end of HTTP headerReceive
	strcat(headerSend, "\r\n");

	if( (post!=NULL) && postLength )
	{
		send(sock,(const char*)post,postLength,0);
		post[postLength] = '\0';

		strcat(headerSend, (const char*)post);
	}

	//strcpy(req->headerSend, headerSend);
	req->headerSend = (char*) malloc( sizeof(char*) * strlen(headerSend));
	strcpy(req->headerSend, (char*) headerSend );

	//WriteLogEx("Request::SendHTTP,12");


	MemBufferCreate(&headersBuffer );
	chars = 0;
	done = FALSE;

	while(!done)
	{
		l = recv(sock,buffer,1,0);
		if(l<0)
			done=TRUE;

		switch(*buffer)
		{
		case '\r':
			break;
		case '\n':
			if(chars==0)
				done = TRUE;
			chars=0;
			break;
		default:
			chars++;
			break;
		}

		MemBufferAddByte(&headersBuffer,*buffer);
	}

	req->headerReceive = (char*) headersBuffer.buffer;
	*(headersBuffer.position) = 0;

	//WriteLogEx("Request::SendHTTP,14");

	MemBufferCreate(&messageBuffer); // Now read the HTTP body

	do
	{
		l = recv(sock,buffer,sizeof(buffer)-1,0);
		if(l<0)
			break;
		*(buffer+l)=0;
		MemBufferAddBuffer(&messageBuffer, (unsigned char*)&buffer, l);
	} while(l>0);
	*messageBuffer.position = 0;
	req->message = (char*) messageBuffer.buffer;
	req->messageLength = (messageBuffer.position - messageBuffer.buffer);

	//WriteLogEx("Request::SendHTTP,16");

	closesocket(sock); // Cleanup

	return 0;
}


//*******************************************************************************************************
// SendRequest
//
//*******************************************************************************************************
int Request::SendRequest(bool IsPost, string url, string& psHeaderSend, string& psHeaderReceive, string& psMessage)
{
	HTTPRequest req;
	int i,rtn;
	LPSTR buffer;

	req.headerSend = NULL;
	req.headerReceive = NULL;
	req.message = NULL;

	//Read in arguments


	if(IsPost)
	{ /* POST */
		i = psHeaderSend.length();
		buffer = (char*) malloc(i+1);
		strcpy(buffer, psHeaderSend.c_str());

		rtn = SendHTTP( url,
			"Content-Type: multipart/form-data;boundary=ZnGpDtePMx0KrHh_G0X99Yef9r8JZsRJSXC",
			(unsigned char*)buffer,
			i,
			&req);

		free(buffer);
	}
	else/* GET */
	{
		rtn = SendHTTP(url,NULL,NULL,0,&req);
	}



	if(!rtn) //Output message and/or headerSend
	{
		psHeaderSend = req.headerSend;
		psHeaderReceive = req.headerReceive;
		//网页的文本是UTF8格式,要转换成GBK
		string sTmp = req.message;
		psMessage = UTF8ToGBK(sTmp); 
		free(req.headerSend);
		free(req.headerReceive);
		free(req.message);
		return 1;
	}
	else
	{
		//WriteLogEx("!!Request::SendRequest,11,IsPost=%d,rtn=%d", IsPost, rtn);
		return 0;
	}
}
