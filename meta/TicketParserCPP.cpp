// CTicketTransformerRx.cpp: implementation of the CTicketTransformerRx class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TicketTransformerRx.h"
#include  <Core\Network\Socket\SocketInit.h>
#include "TicketPrintThread.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define SOCKET_RX_TIMEOUT 999
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define L1_PORT 10070
//#define IGATE_PORT 7878
CTicketTransformerRx::CTicketTransformerRx()
{
    /*m_SocketL1Listen
    m_SocketL1
    m_SocketIGateListen
    m_SocketIGate
    m_hEvtCancel
    m_hEvtReloadSetting
    m_BindAddressIGate
    m_BindAddressL1
    m_usPortIGate
    m_usPortL1
    m_BindIPAddress[16]
    m_Section
    SockAddrRemoteL1
    SockAddrRemoteIGate
  //*/
    m_L1Connected = false;
//DEL   m_IGateConnected = false;
    InitializeCriticalSection(&m_Section);
    strcpy(m_BindIPAddress,"0.0.0.0");
    m_bVerbose= true;
//DEL       m_usPortIGate = IGATE_PORT;
    m_usPortL1 = 9100;
    m_hSocketL1Listen = NULL;
    m_hSocketL1 = NULL;
//DEL       m_hSocketIGateListen = NULL;
//DEL   m_hSocketIGate = NULL;
    EnterCriticalSection (&m_Section);
    m_hEvtCancel =  WSACreateEvent ();
    m_hEvtReloadSetting =   WSACreateEvent ();
    LeaveCriticalSection (&m_Section);
}
CTicketTransformerRx::~CTicketTransformerRx()
{
    Cancel();
    DeleteCriticalSection(&m_Section);
}
void CTicketTransformerRx::LogError(CString In, int iValue)
{
    #if defined(__AFX__)
        AfxMessageBox(In);
    #else
        printf("%s\n",In);
    #endif
}
void CTicketTransformerRx::LogDebug(CString In, int iValue)
{
    #if defined(__AFX__)
        AfxMessageBox(In);
    #else
        printf("%s\n",In);
    #endif
}
void CTicketTransformerRx::Cancel()
{
    EnterCriticalSection (&m_Section);
    //::SetEvent(m_hEvtCancel);
    WSASetEvent(m_hEvtCancel);
    LeaveCriticalSection (&m_Section);
      WSACleanup();
}
void CTicketTransformerRx::Shutdown()
{
    if(m_hSocketL1Listen)
    {
        closesocket(m_hSocketL1Listen);
    }
//DEL       if(m_hSocketIGateListen)
//DEL       {
//DEL           closesocket(m_hSocketIGateListen);
//DEL       }
    if(m_hSocketL1)
    {
        closesocket(m_hSocketL1);
    }
    //DEL   if(m_hSocketIGate)
    //DEL   {
    //DEL   closesocket(m_hSocketIGate);
    //DEL   }
}
int CTicketTransformerRx::Init()
{
    int timeout;
    int err;
    SOCKADDR_IN saServerAddress;
    if(m_bVerbose)
    {
        printf("int CTicketTransformerRx::Init()\r\n");
    }
    if((m_hSocketL1Listen = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)//IPPROTO_UDP
    {
        if(m_bVerbose)
        {
            _ftprintf(stderr,_T("ERROR: Creating Socket L1Listen\n\t%s\n"),WSAGetLastErrorMessage());
        }
        return 0;
    }
//DEL       if((m_hSocketIGateListen = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)//IPPROTO_UDP
//DEL       {
//DEL           if(m_bVerbose)
//DEL           {
//DEL               _ftprintf(stderr,_T("ERROR: Creating Socket IGateListen\n\t%s\n"),WSAGetLastErrorMessage());
//DEL           }
//DEL           return 0;
//DEL       }
    saServerAddress.sin_family = AF_INET;
    saServerAddress.sin_addr.s_addr =inet_addr(m_BindIPAddress);  // Already in network byte order
    saServerAddress.sin_port = htons((unsigned short)m_usPortL1);
    const int iServerAddressSize = sizeof(saServerAddress);
    if (SOCKET_ERROR == bind(m_hSocketL1Listen, (SOCKADDR*)(&saServerAddress), iServerAddressSize))
    {
        if(m_bVerbose)
        {
            _ftprintf(stderr,_T("ERROR: Bind Socket Socket IGateListen\n\t%s\n"),WSAGetLastErrorMessage());
        }
        return 0;
    }
//DEL       saServerAddress.sin_family = AF_INET;
//DEL       saServerAddress.sin_addr.s_addr =inet_addr(m_BindIPAddress);  // Already in network byte order
//DEL       saServerAddress.sin_port = htons((unsigned short)m_usPortIGate);
//DEL       if (SOCKET_ERROR == bind(m_hSocketIGateListen, (SOCKADDR*)(&saServerAddress), iServerAddressSize))
//DEL       {
//DEL           if(m_bVerbose)
//DEL           {
//DEL               _ftprintf(stderr,_T("ERROR: Bind Socket Socket IGateListen\n\t%s\n"),WSAGetLastErrorMessage());
//DEL           }
//DEL           return 0;
//DEL       }
    timeout = SOCKET_RX_TIMEOUT;
    if(err = setsockopt(m_hSocketL1Listen,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) != NO_ERROR)
    {
        LogError(_T("TimeOut"));
        return 0;
    }
//DEL       if(err = setsockopt(m_hSocketIGateListen,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) != NO_ERROR)
//DEL       {
//DEL           LogError(_T("TimeOut"));
//DEL           return 0;
//DEL       }
    return 1;
}
int CTicketTransformerRx::Run()
{
//  int timeout = SOCKET_RX_TIMEOUT;
//  int err;
    int iReturnVal = -1;
    int iReceiveSize = 0;
    int timeout = SOCKET_RX_TIMEOUT;
    unsigned short sSequence = 0;
    iReceivePacketBufferL1Len = 0;
//DEL       char ReceivePacketBufferIGate[4096];  
//DEL       int iReceivePacketBufferIGateLen;
    DWORD StartTick = 0;
    DWORD EndTick = 0;
//DEL       WSAEVENT  hSocketEventIGateListen = WSACreateEvent();
    WSAEVENT  hSocketEventL1Listen = WSACreateEvent();
//DEL       WSAEVENT  hSocketEventIGate = NULL;
    WSAEVENT  hSocketEventL1 = NULL;
    WSAEVENT  hSocketEventL1Dummy = WSACreateEvent();
//DEL       WSAEVENT  hSocketEventIGateDummy = WSACreateEvent();
    WSANETWORKEVENTS Events;
    DWORD dwLastError = 0;;
    DWORD dwWait;
    HANDLE hEvent[6];
    DWORD dwEventCount = 6;
    bool bRunning = true;
    bool bStatus = false;
    int flags = 0;             
    int PacketData = 0;
    int iLen = 0;
    int iStatus = 0;
    char ReceiveBuffer[4096];  
    int RxSize = 4096;
    int iRxlen = 0;
    m_L1Connected = false;
//DEL   m_IGateConnected = false;
//  iReceiveSocketAddrSize = sizeof(sockaddr);
//  timeout = 1;
    if(m_bVerbose)
    {
        printf("int CTicketTransformerRx::Run()\r\n");
    }
    if(!Init())
    {
        printf("Init Fail:\n");
        return 0;
    }
    else
    {
        _tprintf(_T("int CTicketTransformerRx::Run() Init OK\n"));
    }
    _tprintf(_T("int CTicketTransformerRx::Run() Starting\n"));
    StartTick = GetTickCount();     // Get the Start time
//DEL   if((WSAEventSelect(m_hSocketIGateListen, hSocketEventIGateListen , FD_ACCEPT ) ==SOCKET_ERROR ))
//DEL   {
//DEL       printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL       return 0;
//DEL   }
    if((WSAEventSelect(m_hSocketL1Listen, hSocketEventL1Listen ,  FD_ACCEPT ) ==SOCKET_ERROR ))
    {
        printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
        return 0;
    }
    EnterCriticalSection (&m_Section);
/*
    hEvent[0] = NULL;   //ADDED
    hEvent[4] = NULL;   //ADDED
//DEL       hEvent[0] = NULL;hSocketEventIGateListen;
    hEvent[1] = hSocketEventL1Listen;
    hEvent[2] = m_hEvtCancel;
    hEvent[3] = m_hEvtReloadSetting;
//DEL       hEvent[4] = hSocketEventIGateDummy;
    hEvent[5] = hSocketEventL1Dummy;
//*/
    hEvent[0] = hSocketEventL1Listen;
    hEvent[1] = m_hEvtCancel;
    hEvent[2] = m_hEvtReloadSetting;
    hEvent[3] = hSocketEventL1Dummy;
    iReceivePacketBufferL1Len = 0;
      LeaveCriticalSection (&m_Section);
//DEL       if (SOCKET_ERROR == listen(m_hSocketIGateListen,0))
//DEL       {
//DEL           if(m_bVerbose)
//DEL           {
//DEL               _ftprintf(stderr,_T("ERROR:  Socket Listen IGateListen\n\t%s\n"),WSAGetLastErrorMessage());
//DEL           }
//DEL           return 0;
//DEL       }
//DEL       else
//DEL       {
//DEL           printf("Listen ok\n");
//DEL       }
    if (SOCKET_ERROR == listen(m_hSocketL1Listen,0))
    {
        if(m_bVerbose)
        {
            _ftprintf(stderr,_T("ERROR:  Socket Listen L1\n\t%s\n"),WSAGetLastErrorMessage());
        }
        return 0;
    }
    else
    {
        printf("Listen ok\n");
    }
    while(bRunning)//(( != SOCKET_ERROR)
    {
        if(m_bVerbose)
        {
            printf("int CTicketTransformerRx::Waiting()\r\n");
        }
        if((dwWait = WSAWaitForMultipleEvents(4,hEvent,FALSE,WSA_INFINITE,FALSE)) != WSA_WAIT_TIMEOUT )
        {
//DEL               printf("Event:\n");
//DEL               if(dwWait == WSA_WAIT_EVENT_0 ) //hSocketEventIGateListen
//DEL               {
//DEL                   printf("hSocketEventIGateListen:\n");
//DEL                   if((m_hSocketIGate = accept(m_hSocketIGateListen,NULL,NULL)) == INVALID_SOCKET)//&SockAddrRemoteIGate,&iSockAddrSizeRemoteIGate
//DEL                   {
//DEL                       printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                       return 0;
//DEL                   }
//DEL                   hSocketEventIGate = WSACreateEvent();
//DEL                   if((WSAEventSelect(m_hSocketIGate, hSocketEventIGate , FD_READ | FD_CLOSE ) ==SOCKET_ERROR ))
//DEL                   {
//DEL                       printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                       return 0;
//DEL                   }
//DEL                   else if(m_bVerbose)
//DEL                   {
//DEL                       printf("m_hSocketIGate Accepted\r\n");
//DEL                   }
//DEL                   hEvent[4] = hSocketEventIGate;
//DEL                   WSAResetEvent(hSocketEventIGateListen);
//DEL                   m_IGateConnected = true;
//DEL               }
//DEL           else 
            if(dwWait == WSA_WAIT_EVENT_0 ) //hSocketEventL1Listen
            {
                printf("hSocketEventL1Listen:\n");
                if((m_hSocketL1 = accept(m_hSocketL1Listen,NULL,NULL)) == INVALID_SOCKET)//&SockAddrRemoteL1,&iSockAddrSizeRemoteL1
                {
                    printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
                    return 0;
                }
                /*if(err = setsockopt(m_hSocketL1,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) != NO_ERROR)
                {
                    LogError(_T("TimeOut"));
                    printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
                    return 0;
                }
                //*/
                hSocketEventL1 = WSACreateEvent();
                if((WSAEventSelect(m_hSocketL1, hSocketEventL1 , FD_READ | FD_CLOSE ) ==SOCKET_ERROR ))
                {
                    printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
                    return 0;
                }
                else if(m_bVerbose)
                {
                    printf("m_hSocketL1Listen Accepted\r\n");
                }
                hEvent[3] = hSocketEventL1;
                WSAResetEvent(hSocketEventL1Listen);
                m_L1Connected = true;
            }
            else if(dwWait == WSA_WAIT_EVENT_0 + 1 )
            {
                    bRunning = false;
//DEL               if(hSocketEventIGate != NULL)
//DEL               {
//DEL                   WSACloseEvent(hSocketEventIGate);
//DEL                   hSocketEventIGate= NULL;
//DEL               }
                if(hSocketEventL1 != NULL)
                {
                    WSACloseEvent(hSocketEventL1);
                    hSocketEventL1= NULL;
                }
//DEL               WSACloseEvent(hSocketEventIGateListen);
                WSACloseEvent(hSocketEventL1Listen);
                WSACloseEvent(hSocketEventL1Dummy);
//DEL               WSACloseEvent(hSocketEventIGateDummy);
//DEL               hSocketEventIGateListen = NULL;
                    hSocketEventL1Listen = NULL;
                    hSocketEventL1Dummy = NULL;
//DEL               hSocketEventIGateDummy = NULL;
//DEL
            Shutdown();
        }
            else if(dwWait == WSA_WAIT_EVENT_0 + 2 )
            {
                WSAResetEvent(m_hEvtReloadSetting);
            //  OnReload();
            }
//DEL           else if(dwWait == WSA_WAIT_EVENT_0 + 4) //hSocketEventIGate
//DEL           {
//DEL               printf("hSocketIGate:\n");
//DEL               if((iStatus = WSAEnumNetworkEvents(m_hSocketIGate,  hSocketEventIGate,&Events)) == SOCKET_ERROR)
//DEL               {
//DEL                   printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                   return 0;
//DEL               }
//DEL               else
//DEL               {
//DEL                   if(Events.lNetworkEvents & FD_READ)
//DEL                   {
//DEL                       printf("FD_READ:\n");           
//DEL                       if((iReceivePacketBufferIGateLen =  recv(  m_hSocketIGate,        ReceivePacketBufferIGate,   4096,0)) == SOCKET_ERROR )
//DEL                       {
//DEL                           printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                           return 0;
//DEL                       }
//DEL                       else
//DEL                       {
//DEL
//DEL                           if(m_L1Connected) 
//DEL                           {
//DEL                               if(send( m_hSocketL1,ReceivePacketBufferIGate,iReceivePacketBufferIGateLen,0) == SOCKET_ERROR )
//DEL                               {
//DEL                                   printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                                   return 0;
//DEL                               }
//DEL                               else
//DEL                               {
//DEL                                   printf("Send OK:\n");   
//DEL                               }
//DEL                           }
//DEL                           else
//DEL                           {
//DEL                               printf("Not connected nothing to send to\n");
//DEL                           }
//DEL                       }
//DEL                       WSAResetEvent(hSocketEventIGate);
//DEL                   }
//DEL                   else if(Events.lNetworkEvents & FD_CLOSE)
//DEL                   {
//DEL                       printf("FD_CLOSE:\n");      
//DEL                       m_IGateConnected = false;
//DEL                       WSACloseEvent(hSocketEventIGate);
//DEL                       hSocketEventIGate = NULL;
//DEL                       hEvent[4] = hSocketEventIGateDummy;
//DEL                   }
//DEL               }           
//DEL               
//DEL           }
            else if(dwWait == WSA_WAIT_EVENT_0 + 3) //hSocketEventL1
            {
                printf("WSA_WAIT_EVENT_0 + 3 hSocketL1:\n");
                if(m_hSocketL1 == NULL)
                {
                }//
                else if((iStatus = WSAEnumNetworkEvents(m_hSocketL1,  hSocketEventL1,&Events)) == SOCKET_ERROR)
                {
                    printf("WSAEnumNetworkEvents %d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
                    return 0;
                }
                else
                {
                    int ls = 0;
                    if(Events.lNetworkEvents & FD_READ)
                    {
                        printf("FD_READ:\n");           
                    //  if((iReceivePacketBufferL1Len +=  recv(  m_hSocketL1,  ReceivePacketBufferL1 + iReceivePacketBufferL1Len ,   1048576 - iReceivePacketBufferL1Len ,0)) == SOCKET_ERROR )
                        if((iRxlen =  recv(  m_hSocketL1,  ReceiveBuffer,   RxSize ,0)) == SOCKET_ERROR )
                        {
                            printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
                            return 0;
                        }
                        else
                        {
    //char ReceiveBuffer[4096];  
    //int RxSize = 4096;
    //int iRxlen = 0'
                                ls = OnReceiveL1((unsigned char*)ReceiveBuffer,iRxlen);
//DEL                           if(m_IGateConnected) 
//DEL                           {
//DEL                               if(send( m_hSocketIGate,ReceivePacketBufferL1,iReceivePacketBufferL1Len,0) == SOCKET_ERROR )
//DEL                               {
//DEL                                   printf("%d ERROR %d\n%s\n",__LINE__,WSAGetLastError(),WSAGetLastErrorMessage());
//DEL                                   return 0;
//DEL                               }
//DEL                               else
//DEL                               {
//DEL                                   printf("Send OK:\n");   
//DEL                               }
//DEL                           }
//DEL                           else
//DEL                           {
//DEL                               printf("Not connected nothing to send to\n");
//DEL                           }
                        }
                        if(false)//ls == 1
                        {
                            closesocket(m_hSocketL1);
                            m_hSocketL1 = NULL;
                            WSACloseEvent(hSocketEventL1);
                            hSocketEventL1 = NULL;
                            hEvent[3] = hSocketEventL1Dummy;
                        }
                        else
                        {
                        //  WSAResetEvent(hSocketEventL1);
                        }
                    }
                    if(Events.lNetworkEvents & FD_CLOSE)
                    {
                        printf("FD_CLOSE:\n");      
                        m_L1Connected = false;
                        closesocket(m_hSocketL1);
                        WSACloseEvent(hSocketEventL1);
                        hSocketEventL1 = NULL;
                        hEvent[3] = hSocketEventL1Dummy;
                        printf("FD_CLOSE ED:\n");       
                        OnClose();
                        printf("On Close:\n");      
                    }
                    else
                    {
                //      WSAResetEvent(hSocketEventL1);
                    }
                    printf("End of Event\n");   
                }           
            }
        }
    }
    return 1;
}
//DEL   void CTicketTransformerRx::OnReceiveIGate(BYTE* pDATA,DWORD dwLen)
//DEL   {
//DEL   }
int CTicketTransformerRx::TestIt()
{
//  CBSMZGFMConnector   Connector;
//  Connector.DeclareProduction();  
//  Connector.NutRelease(_T("ML23502X022-26515STARC-1499001"));
//  Connector.DeclareProduction();  
//  Connector.NutRelease(_T("ML23502X022-26515STARC-1499001"));
    //  Connector.
    return -1;
}
/*
void CTicketTransformerRx::OnReceiveL1()
{
    printf("m_L1Socket RX\r\n");
    BYTE Buffer[4096];
    int iRcvd = m_L1Socket.Receive(&Buffer,4096);
    if (iRcvd !=  SOCKET_ERROR)
    {
        m_IGateSocket.Send(&Buffer,iRcvd);  
        return;
    }
    printf("m_L1Socket RX Error\r\n");
}
//*/
int CTicketTransformerRx::OnReceiveL1(BYTE* pDATA,DWORD dwLen)
{
    //ReceivePacketBufferL1 + iReceivePacketBufferL1Len ,   1048576 -iReceivePacketBufferL1Len
//  static int a = 0;
    int iRet = 0;
    //FIXME Only IF Source Is VMS;
    if((dwLen > 3) && (pDATA[dwLen -1] == 0xC)&&(pDATA[dwLen - 2] == 0xD)&&(pDATA[dwLen -3] == 0xD))
    {
        iRet = 1;
        dwLen -= 2;
    }
    memcpy(ReceivePacketBufferL1 + iReceivePacketBufferL1Len,pDATA,dwLen);
 //ReceivePacketBufferL1 + iReceivePacketBufferL1Len ,   1048576 - iReceivePacketBufferL1Len ,0)) == SOCKET_ERROR ) 
    if(m_bVerbose)
    {
        printf("void CTicketTransformerRx::OnReceiveL1(BYTE* pDATA,DWORD dwLen)\r\n");
        printf("%d bytes\r\n%s\r\n",dwLen,BufferToHexString((unsigned char*)ReceivePacketBufferL1 + iReceivePacketBufferL1Len,dwLen));
    //  int2Str();
    }
    iReceivePacketBufferL1Len += dwLen;
    int iEndPos = 0;
    int iStartPos = 0;
    //int iTicketLenVal = 12;
    int iTicketLenVal = 2;
//  char Marker[] = "\002E0001\r\n\002G\r";
    char Marker[] = "\002G";
    printf("\r\n%s\r\n",BufferToHexStringCpp((unsigned char*)Marker,iTicketLenVal));
    while((iEndPos = FindTokenInBuffer((unsigned char*)Marker, iTicketLenVal,(unsigned char*)&ReceivePacketBufferL1 + iStartPos,iReceivePacketBufferL1Len - iStartPos)) > 0)
    {
        iEndPos += iTicketLenVal;
        if(m_bVerbose)
        {
            printf("%d %d %d\r\n",iStartPos,iEndPos,iEndPos  - iStartPos);
            printf("Process ticket\r\n");
        }
        ProcessTicket((unsigned char*)ReceivePacketBufferL1 + iStartPos,iEndPos  - iStartPos );
        iStartPos = iEndPos;
    }
    iReceivePacketBufferL1Len -= iStartPos;
    memcpy(ReceivePacketBufferTemp,(unsigned char*)ReceivePacketBufferL1 + iStartPos,iReceivePacketBufferL1Len);
    memcpy(ReceivePacketBufferL1,ReceivePacketBufferTemp,iReceivePacketBufferL1Len);
    if(m_bVerbose)
    {
            printf("%d\r\n",iReceivePacketBufferL1Len);
    }
    if(iReceivePacketBufferL1Len < 4)
    {
        iReceivePacketBufferL1Len = 0;
    }
    return iRet;
}
void CTicketTransformerRx::ProcessTicket(BYTE *pData, DWORD dwLen)
{
    if(m_bVerbose)
    {
        printf("void CTicketTransformerRx::ProcessTicket(BYTE *pData, DWORD dwLen)\r\n");
    }
    CTicketPrintThread* pThread  = (CTicketPrintThread*)AfxBeginThread(RUNTIME_CLASS(CTicketPrintThread),THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
    pThread->m_bAutoDelete = TRUE;
    pThread->CopyBuffer(pData,dwLen);
    pThread->ResumeThread();
    return ;
}
void CTicketTransformerRx::OnClose()
{
    //ReceivePacketBufferTemp;
//  OnReceiveL1(NULL,NULL);
}