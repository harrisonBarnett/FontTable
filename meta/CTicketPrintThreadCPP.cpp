// TicketPrintThread.cpp : implementation file
//
#include "stdafx.h"
#include "TicketPrintThread.h"
#include <mfcext.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTicketPrintThread
IMPLEMENT_DYNCREATE(CTicketPrintThread, CWinThread)
CTicketPrintThread::CTicketPrintThread()
{
    m_pBuffer = NULL;
}
CTicketPrintThread::~CTicketPrintThread()
{
    delete m_pBuffer;
}
BOOL CTicketPrintThread::InitInstance()
{
    // TODO:  perform and per-thread initialization here
    return TRUE;
}
int CTicketPrintThread::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    return CWinThread::ExitInstance();
}
BEGIN_MESSAGE_MAP(CTicketPrintThread, CWinThread)
    //{{AFX_MSG_MAP(CTicketPrintThread)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CTicketPrintThread message handlers
int CTicketPrintThread::Run() 
{
//  WSAInit();
//  socketbridge.Run();
    SYSTEMTIME stRX;
    printf("int CTicketPrintThread::Run() \r\n");
    char OutBuffer[16384];
    CString URL = _T("http://andweb01.and.michelin.com/services/za/realtime_mcbu.php?k=46BAB17B-6342-11EC-B1EA-005056A412EE");
    CString Content;
    char Quality[20];
    CString QualityA;
    CString QualityB;
//  URL = _T("http://10.5.10.109/svc/macbu.txt");
    DownloadFile(URL,Content);
    char AddressA[] = "172.17.26.68";   //172.17.26.69
    char AddressB[] = "172.17.26.69";
    char Address[10];
    CString LineA = GetLine(Content,2);
    CString LineB = GetLine(Content,3);
    QualityA = GetRight(LineA,("\t"));
    QualityB = GetRight(LineB,("\t"));
    ::GetLocalTime(&stRX);
    CString FileName;
    WCHAR wTitle[50] = L"MACBU";
    WCHAR wName[50] = L"";
    //wTitle[0] = L'\0';
    wName[0] = L'\0';
    CString Time = GetCurrentTimeAsString(DATE_FORMAT_FILE);
    FileName.Format(_T("%s\\logs\\%s_ticket.txt"),GetAppPath(),Time);
    WriteBinaryFile(FileName, (BYTE*)(unsigned char*)m_pBuffer,m_dwBufferLen);
    FileName.Format(_T("%s\\logs\\%s_web_data.txt"),GetAppPath(),Time);
    WriteBinaryFile(FileName, (BYTE*)(unsigned char*)Content.GetBuffer(Content.GetLength()),Content.GetLength());
    swprintf(wTitle,L"%.04d%.02d%.02d%.02d%.02d%.02d",stRX.wYear,
        stRX.wMonth,
        stRX.wDay,
        stRX.wHour,
        stRX.wMinute,
        stRX.wSecond);
    CString OutData;
    CString TicketDataOut;
    Clean(QualityA);
    Clean(QualityB);
    int iPos = FindTokenInBuffer((unsigned char*)"\x02U14", 4,(unsigned char*)m_pBuffer,m_dwBufferLen);
    if(iPos != -1)
    {
        sprintf(Quality,"%.*s",8,m_pBuffer + iPos + 7);
    }
    printf("%s\tQual A %s\tQual B %s\n",Quality,QualityA,QualityB);
    if(QualityA ==Quality)
    {
        strcpy(Address,AddressA);
    }
    else if(QualityB == Quality)
    {
        strcpy(Address,AddressB);
    }
    else
    {
        strcpy(Address,AddressA);
    }
/*
    int iU9Pos = FindTokenInBuffer((unsigned char*)"\x02U9", 3,(unsigned char*)m_pBuffer,m_dwBufferLen);
    int iU10Pos = FindTokenInBuffer((unsigned char*)"\x02U10", 4,(unsigned char*)m_pBuffer,m_dwBufferLen);
    int iU14Pos = FindTokenInBuffer((unsigned char*)"\x02U14", 4,(unsigned char*)m_pBuffer,m_dwBufferLen);
    int iU15Pos = FindTokenInBuffer((unsigned char*)"\x02U15", 4,(unsigned char*)m_pBuffer,m_dwBufferLen);
//*/
    //printf("ParseMask\r\n");
    TicketDataOut = Quality;
    TicketDataOut += _T("\r\n");
    int iEndPos = 0;
    int iStartPos = 0;
    //int iTicketLenVal = 12;
    int iOutPos = 0;
//  char Marker[] = "\002E0001\r\n\002G\r";
    char MarkerStart[] = "\002U";
//  printf("\r\n%s\r\n",BufferToHexStringCpp((unsigned char*)Marker,iTicketLenVal));
    CString StringVal;
    int UIndex = 0;
    int iOutStringLength = 0;
    int uStartPos = 0;
    int uEndPos = 0;
    while((iEndPos = FindTokenInBuffer((unsigned char*)MarkerStart, 2,(unsigned char*)m_pBuffer,m_dwBufferLen,iStartPos)) > 0)
    {
        if(UIndex == 0)
        {
            uStartPos = iEndPos;
            memcpy( OutBuffer,  m_pBuffer,uStartPos);
            iOutPos = uStartPos;
        }
        iEndPos += 2;
        int iStart = iEndPos;
        int iEnd = iStart;
        while(iEnd < m_dwBufferLen)
        {
            if(m_pBuffer[iEnd] == '\r')
            {
                break;
            }
            iEnd++;
        }
    //  int2Str
    //  GetSafeInt2Str
    //  if(m_bVerbose)
        {
    //      printf("%d %d %d %d\t%d %d %d %.*s\r\n",iStartPos,iEndPos,iEndPos  - iStartPos,m_dwBufferLen,iStart,iEnd,iEnd-iStart,iEnd-iStart,m_pBuffer + iStart);
        }
        uEndPos = iEnd;
        StringVal.Format(_T("%.*s"),iEnd-iStart,m_pBuffer + iStart);
        if(StringVal.GetLength() >= 3)
        {
            if( StringVal.Left(2) == _T("09") || StringVal.Left(2) == _T("10") || StringVal.Left(2) == _T("14") || StringVal.Left(2) == _T("15"))
            {
                StringVal.Replace(_T(" "),_T("--"));
            }
            iOutStringLength = StringVal.GetLength();
            #if defined(UNICODE)
                sprintf(OutBuffer + iOutPos,"\002U%S\r",StringVal);
            #else
                sprintf(OutBuffer + iOutPos,"\002U%s\r",StringVal);
            #endif
            iOutPos += iOutStringLength + 3;
            TicketDataOut += StringVal + _T("\r\n");
        }
        printf("%s\r\n",StringVal);
        iStartPos = iEndPos;
        if(iEndPos >= m_dwBufferLen)
        {
            //printf("Past end of buffer");
            break;
        }
        UIndex++;
    }
        printf("%s\r\n",TicketDataOut);
    memcpy(OutBuffer + iOutPos,  m_pBuffer + uEndPos,m_dwBufferLen - uEndPos);
    iOutPos += m_dwBufferLen - uEndPos;
    int iWLen = TicketDataOut.GetLength() *2;
    WCHAR* Message = new WCHAR[iWLen];
    FileName.Format(_T("%s\\logs\\%s_ticket_data.txt"),GetAppPath(),Time);
    WriteBinaryFile(FileName, (BYTE*)(unsigned char*)TicketDataOut.GetBuffer(TicketDataOut.GetLength()),TicketDataOut.GetLength());
#if defined(_UNICODE)
    memcpy(TicketDataOut,Message,iWLen);
#else
    ToWCHARA(TicketDataOut,Message,iWLen);
#endif
    //MSMQSendMessageW(NULL,L"TicketDataOut",wTitle, (BYTE*)OutData.GetBuffer(TicketDataOut.GetLength()), TicketDataOut.GetLength());
    MSMQSendMessageW(NULL,L"TicketDataOut",wTitle,Message);
    delete Message;
    PrintTicket(Address,9100,OutBuffer,iOutPos);    
//  PrintTicket(Address,9100,m_pBuffer,m_dwBufferLen);
    delete m_pBuffer;
    m_pBuffer  = NULL;
    return CWinThread::Run();
}
void CTicketPrintThread::Terminate()
{
//  socketbridge.Cancel();
}
void CTicketPrintThread::CopyBuffer(BYTE *pBuffer, DWORD dwLength)
{
    m_pBuffer = new char[dwLength+1];
    m_dwBufferLen = dwLength;
    memcpy(m_pBuffer,pBuffer,dwLength);
}
void CTicketPrintThread::PrintTicket(char *Address, int iPort, char *data, int datalen)
{
     // Obtem o Endereco do wwp.icq.com
    printf("void CTicketPrintThread::PrintTicket(char *Address, int iPort, char *data, int datalen)\r\n");
    printf("%s %d %d\r\n",Address,iPort,datalen);
    printf("%s\r\n",BufferToHexString((unsigned char*)data,datalen));
    struct sockaddr_in PrinterAddress;
    struct hostent * host_inf;
//   WORD wVersionRequested;
   WSADATA wsaData;
   char  cEOL[] = { 13, 10, 0 };
   char cTemp[   5 ];
   char cData[ 256 ];
   char  cMsg[ 512 ];
   int socketPrinter;
   unsigned int nLoop;
    while( ( host_inf = gethostbyname( Address ) ) == NULL )
   {
   }
   // Make Socket
   PrinterAddress.sin_family      = AF_INET;
   PrinterAddress.sin_port        = htons( iPort );
   PrinterAddress.sin_addr.s_addr = *((unsigned long *)((host_inf->h_addr_list)[0] ));
   printf("socket\r\n");
   while( ( socketPrinter = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
   {
   }
   //printf("socket\r\n");
   while( connect( socketPrinter, (struct sockaddr *) &PrinterAddress, sizeof( PrinterAddress ) ) == SOCKET_ERROR )
   {
   }
   // Send Message
   //lOK = 
   printf("Send Data\n");
   ( send( socketPrinter, data, datalen, 0 ) >= 0 );
   // Close Socket
   closesocket( socketPrinter );
   // Close Winsock
}