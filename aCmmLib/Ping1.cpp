// Ping1.cpp: implementation of the CPing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ping1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPing::CPing()
{
  icmp_data = NULL;
  seq_no = 0;
  recvbuf = NULL;
  m_bRecordRout = FALSE;
  lpdest = NULL;
  datasize = DEF_PACKET_SIZE;

  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed: %d\n", GetLastError());
        return ;
    }
  m_hSocket = INVALID_SOCKET;

}

CPing::~CPing()
{

}




  

int CPing::Ping(char * dest,int timeout)
{   
	bool pingsuc=true;
	 m_hSocket = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,
                           WSA_FLAG_OVERLAPPED);

    if (m_hSocket == INVALID_SOCKET) 
    {
        printf("WSASocket() failed: %d\n", WSAGetLastError());
        return false;
    }

    if (m_bRecordRout)
    {
        // Setup the IP option header to go out on every ICMP packet
        //
        ZeroMemory(&m_ipopt, sizeof(m_ipopt));
        m_ipopt.code = IP_RECORD_ROUTE; // Record route option
        m_ipopt.ptr  = 4;               // Point to the first addr offset
        m_ipopt.len  = 39;              // Length of option header
  
        int ret = setsockopt(m_hSocket, IPPROTO_IP, IP_OPTIONS, 
            (char *)&m_ipopt, sizeof(m_ipopt));
        if (ret == SOCKET_ERROR)
        {
            printf("setsockopt(IP_OPTIONS) failed: %d\n", 
                WSAGetLastError());
        }
    }
    // Set the send/recv timeout values
    //
    int bread = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if(bread == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_RCVTIMEO) failed: %d\n", 
            WSAGetLastError());
        return false;
    }
    timeout = 1000;
    bread = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if (bread == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_SNDTIMEO) failed: %d\n", 
            WSAGetLastError());
        return false;
    }
    memset(&m_addrDest, 0, sizeof(m_addrDest));


	// BOOL benable=1;
 //	bread = setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, 
   //             (char*)&benable, sizeof(BOOL));

    //
    // Resolve the endpoint's name if necessary
    //
    m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(dest)) == INADDR_NONE)
    {   
		 struct hostent *hp = NULL;

        if ((hp = gethostbyname(dest)) != NULL)
        {
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
            m_addrDest.sin_family = hp->h_addrtype;
            printf("m_addrDest.sin_addr = %s\n", inet_ntoa(m_addrDest.sin_addr));
        }
        else
        {
            printf("gethostbyname() failed: %d\n", 
                WSAGetLastError());
            return false;
        }
    }        

    // 
    // Create the ICMP packet
    //       
    datasize += sizeof(IcmpHeader);  

    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    if (!icmp_data) 
    {
        printf("HeapAlloc() failed: %d\n", GetLastError());
        return false;
    }
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);
    //
    // Start sending/receiving ICMP packets
    //
    while(1) 
    {
        static int nCount = 0;
        int        bwrote;
                
        if (nCount++ == 2) 
		{
			nCount=0;
			return pingsuc;
            break;
		}
                
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = 
            checksum((USHORT*)icmp_data, datasize);

        bwrote = sendto(m_hSocket, icmp_data, datasize, 0, 
                     (struct sockaddr*)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				pingsuc=false;
                printf("timed out\n");
                continue;
            }
            TRACE("sendto() failed: %d  WSAEACCES=%d\n", WSAGetLastError(),WSAEACCES);
			nCount=0;
            return false;
        }
        if (bwrote < datasize) 
        {
            printf("Wrote %d bytes\n", bwrote);
        }

		int fromlen = sizeof(m_addrFrom);
        bread = recvfrom(m_hSocket, recvbuf, MAX_PACKET, 0, 
                    (struct sockaddr*)&m_addrFrom, &fromlen);
        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				pingsuc=false;
 
                continue;
            }
             nCount=0;
            return false;
        }
        pingsuc=DecodeICMPHeader(recvbuf, bread, &m_addrFrom);
        if(pingsuc)
		{
			nCount=0;
             return true;
		}
    }
	return pingsuc;
}

void CPing::Cleanup()
{
  if (m_hSocket != INVALID_SOCKET) 
        closesocket(m_hSocket);
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);

    WSACleanup();
    return ;
}

void CPing::FillICMPData(char *icmp_data, int datasize)
{
   IcmpHeader *icmp_hdr = NULL;
    char       *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        // Request an ICMP echo
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
}

void CPing::DecodeIPOptions(char *buf, int bytes)
{
     IpOptionHeader *ipopt = NULL;
    IN_ADDR         inaddr;
    int             i;
    HOSTENT        *host = NULL;

    ipopt = (IpOptionHeader *)(buf + 20);

    printf("RR:   ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
            printf("      ");
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,
                    sizeof(inaddr.S_un.S_addr), AF_INET);
        if (host)
            printf("(%-15s) %s\n", inet_ntoa(inaddr), host->h_name);
        else
            printf("(%-15s)\n", inet_ntoa(inaddr));
    }
    return;
}

USHORT CPing::checksum(USHORT *buffer, int size)
{
   unsigned long cksum=0;

    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}

 

bool CPing::DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
   IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    DWORD           tick;
    int    icmpcount = 0;

    iphdr = (IpHeader *)buf;
	// Number of 32-bit words * 4 = bytes
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();

    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
        DecodeIPOptions(buf, bytes);

    if (bytes  < iphdrlen + ICMP_MIN) 
    {
        printf("Too few bytes from %s\n", 
            inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
        printf("nonecho type %d recvd\n", icmphdr->i_type);
        return false ; 
    }
    // Make sure this is an ICMP reply to something we sent!
    //
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
        printf("someone else's packet!\n");
        return false;
    }
    

    icmpcount++;
    return true;
}

void CPing::usage(char *progname)
{
   printf("usage: ping -r <host> [data size]\n");
    printf("       -r           record route\n");
    printf("        host        remote machine to ping\n");
    printf("        datasize    can be up to 1KB\n");
    ExitProcess(-1);
}
