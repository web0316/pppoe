
#include <windows.h>
#include <tchar.h>
#include "string.h"
#include "memory.h"
#include "commctrl.h"
#include "raserror.h"
#include "notify.h"
#include "ras.h"
#include "tapihelp.h"
#include "resource.h"
#include "windev.h"
#include "autoras.h"
#ifdef USE_SIP
# include <sipapi.h>
typedef BOOL (WINAPI* LPFNSIP)(SIPINFO*);
typedef DWORD (WINAPI* LPFNSIPSTATUS)();
#endif
#include "msgqueue.h"
#define RNAAPP_MAGIC_NUM 0x006A6D6D
#define Shell_NotifyIcon PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 6, (DWORD,PNOTIFYICONDATA))

#define SIP_UP      0
#define SIP_DOWN    1
#define RNA_GETINFO 3

const TCHAR     szAppName[] = TEXT("rnaapp");
TCHAR        EntryName[RAS_MaxEntryName + 1];
RASENTRY     v_RasEntry;   
HWND         v_hMainWnd;
HWND         v_hStatusWnd;
HWND         v_hCancelBtn;
HWND         v_hDismissBtn;
HINSTANCE    v_hInstance;
HRASCONN     v_hRasConn;
RASDIALPARAMS   v_RasDialParams;
BOOL         v_fMinOnConnect;
BOOL v_fNoPassword;
BOOL v_fNoMsgBox;
BOOL v_fInErrorBox;
BOOL v_fReturnFalse;
DWORD v_RefCount;
DWORD v_RetryCnt;
BOOL            v_fPassword;
HICON           v_hDirectIcon;
HICON           v_hDialupIcon;
HICON v_hVPNIcon;
HICON v_hNotifyIcon;
DWORD v_dwDeviceID;
RNAAPP_INFO v_RNAAppInfo;
#ifdef USE_SIP
LPFNSIP         v_lpfnSipGetInfo;
LPFNSIP         v_lpfnSipSetInfo;
LPFNSIPSTATUS v_lpfnSipStatus;
HINSTANCE  v_hSipLib;
#endif
HANDLE              g_hMsgQueue;
DIALER_NOTIFICATION g_DialerNotification;
BOOL                g_bAttemptConnectSuccessful = FALSE;    
                                                            
                                                            
                                                            

BOOL v_RASOE_PreviewUserPwRestore;


static BOOL NetDisconnect;
static BOOL CredentialPrompt;
static  HWND    hCredentials;

#define IDM_TASKBAR_NOTIFY  (WM_USER + 200)
#define IDM_START_RASDIAL   (WM_USER + 201)


#ifdef DEBUG
DBGPARAM dpCurSettings =
{
    TEXT("rnaapp"),
{
TEXT("Unused"),TEXT("Unused"),TEXT("Unused"),TEXT("Unused"),
TEXT("Unused"),TEXT("Unused"),TEXT("Unused"),TEXT("Unused"),
TEXT("Unused"),TEXT("Ras"),TEXT("Tapi"),TEXT("Misc"),
TEXT("Alloc"),TEXT("Function"),TEXT("Warning"),TEXT("Error")
},
    0x00000000
};

#define ZONE_RAS        DEBUGZONE(9)
#define ZONE_TAPI       DEBUGZONE(10) 
#define ZONE_MISC       DEBUGZONE(11) 
#define ZONE_ALLOC      DEBUGZONE(12) 
#define ZONE_FUNCTION   DEBUGZONE(13) 
#define ZONE_WARN       DEBUGZONE(14) 
#define ZONE_ERROR      DEBUGZONE(15) 

#endif  
#define SCREEN_WIDTH    240
#define SCREEN_HEIGTH   80


#define RNAAPP_IPC_WNDCLS _T("RNAAPP__IPC__WNDCLS")


typedef struct _tagTHREADINFO
{
HWND hWnd;
HANDLE hEvent;
} THREADINFO, *PTHREADINFO;

DWORD g_dwThreadId;

void CALLBACK
lineCallbackFunc(DWORD dwDevice, DWORD dwMsg, DWORD dwCallbackInstance,
 DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
   return;
}
void
ShowError (DWORD Message, DWORD Title)
{
    TCHAR           szTemp[218];
    TCHAR           szTemp2[128];

if( v_fNoMsgBox ) {
return;
}
LoadString (v_hInstance, Message, szTemp, sizeof(szTemp)/sizeof(TCHAR));
LoadString (v_hInstance, Title, szTemp2, sizeof(szTemp2)/sizeof(TCHAR));
MessageBox( v_hMainWnd, szTemp, szTemp2, MB_SETFOREGROUND | MB_APPLMODAL | MB_OK |
MB_ICONEXCLAMATION);
} 
void
SetTapiDisplayStuff(HWND hDlg)
{
    TCHAR szPhone[256];
    LPTSTR  pLocName;

szPhone[0] = TEXT(' 
BOOL
RasDisconnect(
DWORD ErrorCode)

{
DWORD StringID;

if( CredentialPrompt )
{

NetDisconnect = TRUE;
SendMessage( hCredentials, WM_COMMAND, IDCANCEL, 0 );
NetDisconnect = FALSE;
}


SetStatusWnd (IDS_DISCONNECTED);

sndPlaySound(TEXT("RNIntr"), SND_ALIAS|SND_ASYNC|SND_NODEFAULT);

if (!v_RNAAppInfo.ErrorCode)
{
v_RNAAppInfo.ErrorCode = ErrorCode;
}


    if( v_hRasConn )
{
        RasHangUp( v_hRasConn );
        v_hRasConn = 0;
}

if( !v_fNoMsgBox && !v_fInErrorBox && ErrorCode != NO_ERROR)
{
StringID = RasErrorToStringID (ErrorCode, IDS_DISCONNECTED, FALSE);
ShowError (StringID, IDS_REM_NET);
}

if (v_fInErrorBox)
{
v_fReturnFalse = TRUE;
return TRUE;
}
else
{
return FALSE;
}
}

BOOL
RasDialEvent( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    RASCONNSTATE    RasState = (RASCONNSTATE)wParam;
TCHAR szTemp[128];
TCHAR szFmtStr[128];
LPTSTR pPhoneStr;
DWORD StringID;
    DWORD           Flag;

DEBUGMSG (ZONE_RAS, (TEXT("RasDialEvent: wParam: 0x%Xrn"), wParam));

    switch(RasState) {
    case RASCS_OpenPort:
SetStatusWnd (IDS_OPENING_PORT);

                if(v_RASOE_PreviewUserPwRestore)
                {
                   v_RasEntry.dwfOptions |= RASEO_PreviewUserPw;
                   v_RASOE_PreviewUserPwRestore = FALSE;
                   RasSetEntryProperties(NULL, EntryName, &v_RasEntry, sizeof(v_RasEntry), NULL,0);
                }

        break;

    case RASCS_PortOpened:
if (!_tcscmp (v_RasEntry.szDeviceType, RASDT_Modem)) {
pPhoneStr = NULL;

        Flag = 0;
        if (v_RasEntry.dwfOptions & RASEO_UseCountryAndAreaCodes){
        Flag |= LINETRANSLATEOPTION_FORCELD;
        }

        if (v_RasEntry.dwfOptions & RASEO_DialAsLocalCall) {
        Flag |= LINETRANSLATEOPTION_FORCELOCAL;
        }


pPhoneStr = TapiLineTranslateAddress(v_dwDeviceID,
v_RasEntry.dwCountryCode,
v_RasEntry.szAreaCode, v_RasEntry.szLocalPhoneNumber, FALSE, Flag);

if (pPhoneStr) {
LoadString (v_hInstance, IDS_DIAL_WITH_NUMBER, szFmtStr,
sizeof(szFmtStr)/sizeof(TCHAR));
wsprintf (szTemp, szFmtStr, pPhoneStr);
LocalFree (pPhoneStr);
SetWindowText (GetDlgItem(v_hMainWnd, IDC_STATUSMSG), szTemp);
} else {
SetStatusWnd (IDS_DIAL_NO_NUMBER);
}
} else {
if (v_RetryCnt++) {
LoadString (v_hInstance, IDS_CONNECTING_TO_HOST_RETRY,
szFmtStr, sizeof(szFmtStr)/sizeof(TCHAR));
wsprintf (szTemp, szFmtStr, v_RetryCnt);
SetWindowText(GetDlgItem(v_hMainWnd, IDC_STATUSMSG), szTemp);
} else {
SetStatusWnd (IDS_CONNECTING_TO_HOST);
}
}
        break;

    case RASCS_ConnectDevice:
SetStatusWnd (IDS_CONNECTING_DEVICE);
        break;

    case RASCS_DeviceConnected:
SetStatusWnd (IDS_DEVICE_CONNECTED);
        break;

    case RASCS_AllDevicesConnected:
SetStatusWnd (IDS_PHY_EST);
        break;

    case RASCS_Authenticate:
SetStatusWnd (IDS_AUTH_USER);
        break;

    case RASCS_AuthNotify:
SetStatusWnd (IDS_AUTH_EVENT);

        switch( lParam ) {
        case ERROR_RESTRICTED_LOGON_HOURS:
StringID = IDS_AUTH_ERR_REST_HOUR;
break;
case ERROR_ACCT_DISABLED:
StringID = IDS_AUTH_ERR_ACCT_DISABLED;
break;
        case ERROR_PASSWD_EXPIRED:
StringID = IDS_AUTH_ERR_PWD_EXP;
break;
case ERROR_NO_DIALIN_PERMISSION:
StringID = IDS_AUTH_ERR_NO_DIALIN;
break;
        case ERROR_CHANGING_PASSWORD:
StringID = IDS_AUTH_ERR_CHG_PWD;
break;
case ERROR_USER_DISCONNECTION:

return RasDisconnect(NO_ERROR);

default :
StringID = IDS_AUTH_ERR_UNKNOWN;
break;

}

v_RasDialParams.szUserName[0] = TEXT(' 
void PositionSIP(int nSipState)
{
#ifdef USE_SIP

    if (v_lpfnSipGetInfo && v_lpfnSipSetInfo)
    {
    SIPINFO si;


        memset(&si, 0, sizeof(SIPINFO));
        si.cbSize = sizeof(SIPINFO);
        if ((*v_lpfnSipGetInfo)(&si))
        {

            if ((!(si.fdwFlags & SIPF_ON) && SIP_UP == nSipState) ||
                (si.fdwFlags & SIPF_ON && !(SIP_UP == nSipState)))
            {
                si.fdwFlags ^= SIPF_ON;
        (*v_lpfnSipSetInfo)(&si);
            }
        }
}
#endif
}

void LoadSIP(void)
{
#ifdef USE_SIP
    if( (v_hSipLib = LoadLibrary( TEXT("coredll.dll") )) &&
        (v_lpfnSipStatus = (LPFNSIPSTATUS)GetProcAddress( v_hSipLib, TEXT("SipStatus"))) &&
    (v_lpfnSipGetInfo = (LPFNSIP)GetProcAddress( v_hSipLib, TEXT("SipGetInfo"))) &&
    (v_lpfnSipSetInfo = (LPFNSIP)GetProcAddress( v_hSipLib, TEXT("SipSetInfo"))) &&
    (SIP_STATUS_AVAILABLE == v_lpfnSipStatus()) )
{
DEBUGMSG(1, (L"RNAAPP: Sip Loaded & readyrn"));
    }
    else
    {
FreeLibrary(v_hSipLib);
v_hSipLib = NULL;
        v_lpfnSipStatus = NULL;
    v_lpfnSipGetInfo = v_lpfnSipSetInfo = NULL;
}
#endif
}


void FindOwnedWindow(HWND hWnd)
{
    DWORD dwThreadId;
    DWORD dwProcessId;
    HWND hWndChildFirst = GetWindow(hWnd, GW_HWNDFIRST);
    HWND hWndChildNext = hWndChildFirst;
    while (hWndChildNext)
    {
        if (GetWindow(hWndChildNext, GW_OWNER) == hWnd)
        {
            dwThreadId = GetWindowThreadProcessId( hWndChildNext, &dwProcessId);
            if (dwProcessId != GetCurrentProcessId())
            {
                g_dwThreadId = dwThreadId;
                break;   
            }
        }
        hWndChildNext = GetWindow(hWndChildNext, GW_HWNDNEXT);
    }
}

LRESULT CALLBACK
WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    TCHAR           szTemp[128];
TCHAR szTemp2[128];
DWORD RetVal;
    NOTIFYICONDATA  tnid;
DWORD StringID;
    int cxScreen;

    switch( message ) {
    case WM_INITDIALOG:

if (!_tcscmp (v_RasEntry.szDeviceType, RASDT_Modem)) {
            SendMessage(GetDlgItem(hWnd, IDC_STATUSICON), STM_SETIMAGE,
                        IMAGE_ICON, (LPARAM) v_hDialupIcon);
} else if (!_tcscmp (v_RasEntry.szDeviceType, RASDT_Direct)) {
            SendMessage(GetDlgItem(hWnd, IDC_STATUSICON), STM_SETIMAGE,
                        IMAGE_ICON, (LPARAM) v_hDirectIcon);
} else if ((!_tcscmp (v_RasEntry.szDeviceType, RASDT_Vpn)) ||
                   (!_tcscmp (v_RasEntry.szDeviceType, RASDT_PPPoE))) {
            SendMessage(GetDlgItem(hWnd, IDC_STATUSICON), STM_SETIMAGE,
                        IMAGE_ICON, (LPARAM) v_hVPNIcon);
} else {
RETAILMSG (1, (TEXT("Error, unhandled Device Typern")));
ASSERT (0);
}


LoadString (v_hInstance, IDS_CONNECTING_TO, szTemp,
sizeof(szTemp)/sizeof(TCHAR));
        wsprintf( szTemp2, szTemp, EntryName );
        SetWindowText( hWnd, szTemp2 );


        tnid.cbSize = sizeof(NOTIFYICONDATA);
        tnid.hWnd = hWnd;
        tnid.uID = 13;
        tnid.uFlags = NIF_MESSAGE | NIF_ICON;
        tnid.uCallbackMessage = IDM_TASKBAR_NOTIFY;

if (!_tcscmp (v_RasEntry.szDeviceType, RASDT_Modem)) {
v_hNotifyIcon = (HICON )LoadImage( v_hInstance,
   MAKEINTRESOURCE(IDI_DIALUP),
   IMAGE_ICON, 16, 16,
   LR_DEFAULTCOLOR);
} else if (!_tcscmp (v_RasEntry.szDeviceType, RASDT_Direct)) {
v_hNotifyIcon = (HICON )LoadImage( v_hInstance,
   MAKEINTRESOURCE(IDI_DIRECTCC),
   IMAGE_ICON, 16, 16,
   LR_DEFAULTCOLOR);
} else if ((!_tcscmp (v_RasEntry.szDeviceType, RASDT_Vpn)) ||
                   (!_tcscmp (v_RasEntry.szDeviceType, RASDT_PPPoE))) {
v_hNotifyIcon = (HICON )LoadImage( v_hInstance,
   MAKEINTRESOURCE(IDI_VPN),
   IMAGE_ICON, 16, 16,
   LR_DEFAULTCOLOR);
} else {
RETAILMSG (1, (TEXT("Error, unhandled Device Typern")));
ASSERT (0);
}
tnid.hIcon = v_hNotifyIcon;


        return TRUE;

    case WM_CREATE:
        break;

    case IDM_START_RASDIAL:
cxScreen = GetSystemMetrics(SM_CXSCREEN);



        memset( (char *)&v_RasDialParams, 0, sizeof( RASDIALPARAMS ) );

        v_RasDialParams.dwSize = sizeof(RASDIALPARAMS);
        _tcscpy( v_RasDialParams.szEntryName, EntryName );
        RasGetEntryDialParams( NULL, &v_RasDialParams, &v_fPassword );


        ShowWindow( hWnd, SW_SHOW );
if(!v_fNoPassword && !_tcscmp (v_RasEntry.szDeviceType, RASDT_Modem))
{


               if(v_RasEntry.dwfOptions & RASEO_PreviewUserPw)
               {
                   v_RasEntry.dwfOptions &= ~RASEO_PreviewUserPw;
                   v_RASOE_PreviewUserPwRestore = TRUE;
                   RasSetEntryProperties(NULL, EntryName, &v_RasEntry, sizeof(v_RasEntry), NULL,0);
               }
               else
                   v_RASOE_PreviewUserPwRestore = FALSE;


PositionSIP(SIP_UP);


if (!DialogBox(v_hInstance, MAKEINTRESOURCE((cxScreen < 480) ? IDD_USERPWD2 :
       IDD_USERPWD), hWnd, (DLGPROC)DlgProcCredentials))
{
     // Exit.
    v_RNAAppInfo.ErrorCode = ERROR_USER_DISCONNECTION;
    DestroyWindow (hWnd);
                    PositionSIP(SIP_DOWN);
            break;
}

                PositionSIP(SIP_DOWN);
SendMessage( hWnd, WM_PAINT, 0, 0 );
}

SetForegroundWindow ( hWnd );

if( RetVal = RasDial( NULL, NULL,
 &v_RasDialParams,
 0xffffffff,
 hWnd,
 &v_hRasConn ) )
{
RETAILMSG (1, (TEXT("Error from RasDial %d %drn"),
                                                   RetVal, GetLastError()));

                        if(v_RASOE_PreviewUserPwRestore)
                        {
                            v_RasEntry.dwfOptions |= RASEO_PreviewUserPw;
                            v_RASOE_PreviewUserPwRestore = FALSE;
                            RasSetEntryProperties(NULL, EntryName, &v_RasEntry, sizeof(v_RasEntry), NULL,0);
                        }

if (!v_fNoMsgBox) {
StringID = RasErrorToStringID (RetVal, IDS_RASDIAL_ERR, TRUE);
ShowError (StringID, IDS_REM_NET);
}

if( v_hRasConn ) {
RasHangUp( v_hRasConn );
v_hRasConn = 0;
}

v_RNAAppInfo.ErrorCode = RetVal;


DestroyWindow (hWnd);
}
break;
case RNA_RASCMD :
switch (wParam) {
case RNA_ADDREF :
v_RefCount++;
DEBUGMSG (ZONE_WARN, (TEXT("AddRef %drn"), v_RefCount));
break;
case RNA_DELREF :
if (v_RefCount) {
v_RefCount--;
}
DEBUGMSG (ZONE_WARN, (TEXT("DelRef %drn"), v_RefCount));
if (!v_RefCount) {
SendMessage (hWnd, WM_COMMAND, IDCANCEL, 0);
DEBUGMSG (ZONE_WARN, (TEXT("Sending myself a cancelrn")));
}
break;
case RNA_GETINFO :
if (IsWindow((HWND)lParam)) {
DEBUGMSG (ZONE_WARN,
  (TEXT("GetInfo: Sending WM_NETCONNECT to 0x%Xrn"),
   lParam));
if (IsWindow((HWND)v_RNAAppInfo.hWndRNAApp))
{
SendNotifyMessage ((HWND)lParam, WM_NETCONNECT, (WPARAM)TRUE,
 (LPARAM)&v_RNAAppInfo);
}
else
{


RNAAPP_INFO rnai;

memset(&rnai, 0, sizeof(RNAAPP_INFO));
rnai.dwSize = sizeof(RNAAPP_INFO);
rnai.hWndRNAApp = (DWORD)hWnd;
_tcscpy(rnai.RasEntryName, EntryName);

SendNotifyMessage ((HWND)lParam, WM_NETCONNECT, (WPARAM)FALSE, (LPARAM)&rnai);
}
} else {
DEBUGMSG (ZONE_ERROR,
  (TEXT("GetInfo: lParam 0x%X is not a windowrn"),
   lParam));
}
break;
default :
DEBUGMSG (ZONE_WARN, (TEXT("Unknown RNA_RASCMD %drn"), wParam));
break;
}
break;

case WM_TIMER :
break;

    case WM_COMMAND:

        switch( LOWORD( wParam ) ) {
        case IDDISMISS:
{
        if(IsAPIReady(SH_SHELL))
         ShowWindow(hWnd, SW_HIDE);
    else
SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}
            break;

        case IDCANCEL:
         DEBUGMSG( ZONE_MISC, (TEXT("IDCANCEL:v_hRasConn: 0x%8xrn"),
  v_hRasConn ));

            if( v_hRasConn ) {
         DEBUGMSG( ZONE_MISC, ( TEXT( "-> RasHangUprn" )));
                RasHangUp( v_hRasConn );
         DEBUGMSG( ZONE_MISC, ( TEXT( "<- RasHangUprn" )));
                v_hRasConn = 0;
            }

            if (g_bAttemptConnectSuccessful != TRUE)
            {

                if (g_hMsgQueue)
                {
                    g_DialerNotification.dwNotificationId = DIALER_CANCELLED;

                    WriteMsgQueue(
             g_hMsgQueue,
             &g_DialerNotification,
             sizeof(DIALER_NOTIFICATION),
             0x00,
             0x00);
                }
            }

v_RNAAppInfo.ErrorCode = ERROR_USER_DISCONNECTION;
sndPlaySound(TEXT("RNEnd"), SND_ALIAS|SND_ASYNC|SND_NODEFAULT);
            DestroyWindow( hWnd );
            break;
        }
        break;

    case WM_RASDIALEVENT:

        if( FALSE == RasDialEvent( hWnd, message, wParam, lParam ) ) {


if (!v_RNAAppInfo.ErrorCode) {
v_RNAAppInfo.ErrorCode = ERROR_DISCONNECTION;
}

            FindOwnedWindow(hWnd);
        
            DestroyWindow( hWnd );
        }
        break;

    case IDM_TASKBAR_NOTIFY:

        if( lParam == WM_LBUTTONDBLCLK) {
            DEBUGMSG( ZONE_MISC,
  (TEXT("Got IDM_TASKBAR_NOTIFY: LBUTTONDOWNrn")));
SetForegroundWindow(hWnd);
            ShowWindow( hWnd, SW_SHOWNORMAL );
        }
        break;

    case WM_DESTROY:
        DEBUGMSG(1, (TEXT("DESTROYn")));

        tnid.cbSize = sizeof(NOTIFYICONDATA);
        tnid.hWnd = hWnd;
        tnid.uID = 13;

        if(IsAPIReady(SH_SHELL))
         Shell_NotifyIcon(NIM_DELETE, &tnid);
        PostQuitMessage(0);
        break;

    default:
break;
    }
    return (0);
} 
BOOL
InitApplication(HINSTANCE hInstance)
{
    return TRUE;
}

BOOL
InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    DWORD   ScreenX = GetSystemMetrics(SM_CXSCREEN);
    DWORD   ScreenY = GetSystemMetrics(SM_CYSCREEN);

    v_hDirectIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_DIRECTCC));
    v_hDialupIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_DIALUP));
    v_hVPNIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_VPN));

    v_hMainWnd = CreateDialog(hInstance,
                              MAKEINTRESOURCE( GetSystemMetrics(SM_CXSCREEN) <
   480 ? IDD_DCCSTATUS2 :
   IDD_DCCSTATUS),
  NULL, WndProc);

    if (v_hMainWnd) {
SetWindowLong (v_hMainWnd, DWL_USER, RNAAPP_MAGIC_NUM);
if(IsAPIReady(SH_SHELL))
ShowWindow(v_hMainWnd, SW_HIDE);
else
SetWindowPos(v_hMainWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        UpdateWindow(v_hMainWnd);
    }

    return (NULL != v_hMainWnd);
}
BOOL FindRNAAppWindow(HWND hWnd, LPARAM lParam)
{
    TCHAR   szClassName[32];

GetClassName (hWnd, szClassName, sizeof(szClassName)/sizeof(TCHAR));

if (!_tcscmp (szClassName, TEXT("Dialog")) &&
(RNAAPP_MAGIC_NUM == GetWindowLong (hWnd, DWL_USER))) {
*((HWND *)lParam) = hWnd;
return FALSE;
}
return TRUE;
}





static HRASCONN GetRasConnHandle(IN LPCTSTR lpszEntryName)
{
HRASCONN hRasConn = NULL;
RASCONN rasconn = {0};
LPRASCONN lpRasConn = &rasconn;
DWORD cb = sizeof(RASCONN);
DWORD dwConnections = 1;
DWORD dwRetVal = 0;
HLOCAL hMem = NULL;
DWORD i = 0;

if (!lpszEntryName || !*lpszEntryName)
{
hRasConn = NULL;
goto cleanup;
}

lpRasConn->dwSize = sizeof(RASCONN);
dwRetVal = RasEnumConnections(lpRasConn, &cb, &dwConnections);

if (0 != dwRetVal)
{
if (ERROR_BUFFER_TOO_SMALL == dwRetVal)
{


hMem = LocalAlloc(LPTR, cb);

if (!hMem)
{
hRasConn = NULL;
goto cleanup;
}

lpRasConn = LocalLock(hMem);

lpRasConn->dwSize = sizeof(RASCONN);

if (0 != RasEnumConnections(lpRasConn, &cb, &dwConnections))
{
hRasConn = NULL;
goto cleanup;
}
}
else
{
hRasConn = NULL;
goto cleanup;
}
}

for (i = 0; i < dwConnections; i++)
{
if (!_tcscmp(lpszEntryName, lpRasConn[i].szEntryName))
{
hRasConn = lpRasConn[i].hrasconn;
break;
}
}

cleanup:
if (hMem)
{
LocalUnlock(hMem);
LocalFree(hMem);
}

return hRasConn;
} 
static LRESULT CALLBACK ReceiverWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
RNAAPP_INFO rnai;

if (WM_NETCONNECT == uMsg)
{
memset(&rnai, 0, sizeof rnai);
memcpy(&rnai, (PRNAAPP_INFO)lParam, sizeof rnai);



if (0 == _tcsicmp(EntryName, rnai.RasEntryName))
{


if(IsAPIReady(SH_SHELL))
{
ShowWindow((HWND)rnai.hWndRNAApp, SW_SHOW);
}
else
{
SetWindowPos((HWND)rnai.hWndRNAApp, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

UpdateWindow((HWND)rnai.hWndRNAApp);
}

return 0;
}

return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



static DWORD ActivateStatusThreadProc(LPVOID lpParameter)
{
PTHREADINFO pti = (THREADINFO *)lpParameter;
HWND hWnd = NULL;
WNDCLASS wc;
MSG msg;

memset(&wc, 0, sizeof wc);
wc.lpfnWndProc = ReceiverWndProc;
wc.hInstance = v_hInstance;
wc.lpszClassName = RNAAPP_IPC_WNDCLS;

RegisterClass(&wc);

hWnd = CreateWindowEx(0, RNAAPP_IPC_WNDCLS, NULL, 0, CW_USEDEFAULT,
CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, v_hInstance, 0);

if (!IsWindow(hWnd))
{
pti->hWnd = NULL;
return FALSE;
}

pti->hWnd = hWnd;
SetEvent(pti->hEvent);

while (GetMessage(&msg, NULL, 0, 0))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}

DestroyWindow(hWnd);
UnregisterClass(RNAAPP_IPC_WNDCLS, v_hInstance);

return TRUE;
}



static BOOL CALLBACK EnumStatusWindowsProc(IN HWND hWnd, IN LPARAM lParam)
{
TCHAR szClassName[32];

GetClassName (hWnd, szClassName, sizeof(szClassName)/sizeof(TCHAR));

if (!_tcscmp (szClassName, TEXT("Dialog")) && (RNAAPP_MAGIC_NUM == GetWindowLong (hWnd, DWL_USER)))
{
if (IsWindow(hWnd))
{
SendMessage (hWnd, RNA_RASCMD, RNA_GETINFO, lParam);
}
}

return TRUE;
}



static BOOL ActivateStatusDlg()
{
BOOL fOk = TRUE;
DWORD dwThreadID = 0;
HTHREAD hThread = NULL;
HLOCAL hMem = NULL;
PTHREADINFO pti = NULL;

hMem = LocalAlloc(LPTR, sizeof(THREADINFO));

if (!hMem)
{
fOk = FALSE;
goto cleanup;
}

pti = LocalLock(hMem);

memset(pti, 0, sizeof(THREADINFO));



pti->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

if (!pti->hEvent)
{
fOk = FALSE;
goto cleanup;
}

hThread = CreateThread(NULL, 0, ActivateStatusThreadProc, (LPVOID)pti, 0, &dwThreadID);

if (!hThread)
{
fOk = FALSE;
goto cleanup;
}

WaitForSingleObject(pti->hEvent, INFINITE);

EnumWindows(EnumStatusWindowsProc, (LPARAM)pti->hWnd);

cleanup:
if (hThread)
{
PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
WaitForSingleObject(hThread, INFINITE);
CloseHandle(hThread);
}

if (pti && pti->hEvent)
{
CloseHandle(pti->hEvent);
}

pti = NULL;

if (hMem)
{
LocalUnlock(hMem);
LocalFree(hMem);
}

return fOk;
} 
int WINAPI
#ifdef UNDER_CE
WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPWSTR pCmdLine,
     int nCmdShow)
#else
WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPSTR pCmdLine,
     int nCmdShow)
#endif
{
    MSG             msg;
    BOOL            fParmError = FALSE;
    BOOL            fHaveQuote;
    DWORD           cb;
    DWORD           Index;
    long            lReturn;
DWORD     dwDebug;
BOOL     TapiInitialF = FALSE;
    MSGQUEUEOPTIONS sOptions;

DEBUGREGISTER(0);

    v_hInstance = hInstance;
v_hMainWnd = NULL;
memset(&v_RNAAppInfo, 0, sizeof(v_RNAAppInfo));

#ifdef USE_TTCONTROLS
if (!InitCommCtrlTable()) {
DEBUGMSG (ZONE_ERROR, (TEXT("RNAAPP: No commctrl.dll ")));
}
else {
INITCOMMONCONTROLSEX icce;
icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
icce.dwICC  = ICC_TOOLTIP_CLASSES|ICC_CAPEDIT_CLASS;
CallCommCtrlFunc(InitCommonControlsEx)(&icce);
}
#endif



    sOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
sOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
sOptions.dwMaxMessages = 8;
sOptions.cbMaxMessage = sizeof(DIALER_NOTIFICATION);
sOptions.bReadAccess = FALSE;

g_hMsgQueue = CreateMsgQueue(AUTORAS_MSGQUEUE_NAME, &sOptions);

    if (g_hMsgQueue == NULL)
    {
        DEBUGMSG(ZONE_ERROR,
            (TEXT("RNAAPP:: Failed CreateMsgQueue.. ")));

    }
    else
    {

        g_DialerNotification.dwNotificationId = DIALER_START;

        WriteMsgQueue(
g_hMsgQueue,
&g_DialerNotification,
sizeof(DIALER_NOTIFICATION),
0x00,
0x00);
    }


LoadSIP();