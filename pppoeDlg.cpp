// pppoeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pppoe.h"
#include "pppoeDlg.h"
#include "bohao.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPppoeDlg dialog

CPppoeDlg::CPppoeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPppoeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPppoeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPppoeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CPppoeDlg)
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPppoeDlg, CDialog)
	//{{AFX_MSG_MAP(CPppoeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPppoeDlg message handlers

BOOL CPppoeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPppoeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPppoeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPppoeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPppoeDlg::OnButton1() 
{  
	CRasEntry   m_RasEntry;   
        RASENTRY   RasEntry;   
        ::ZeroMemory(&RasEntry,sizeof(RASENTRY));   
       
		
		CString M,right;
		char buffer[MAX_PATH],inBufs[80]={""};
	
		GetCurrentDirectory(256,buffer); 
	
                 M=buffer;
		M=M+"\\bjgwbn.ini";
GetPrivateProfileString ("PPPOE","setup", NULL, inBufs, 80,M); 
right=inBufs;
		
		
if(right!="1") {
  WritePrivateProfileString("PPPOE","setup",  
       "1", M); 
      
	   //  Device   
      strcpy(RasEntry.szDeviceType,   "PPPoE");   
     strcpy(RasEntry.szDeviceName,   "PPPoE");   
      //  Options
	 RasEntry.dwfOptions   =RASEO_RemoteDefaultGateway | RASEO_ModemLights | RASEO_IpHeaderCompression 
						|RASEO_SwCompression;
   //  RasEntry.dwfOptions2=RASEO2_ReconnectIfDropped;  
	m_RasEntry.CreateRasEntry("长宽拨号",RasEntry);
			
		
}

	
      CString  strEntryName="长宽拨号";
	
	
	char szUserName[MAX_PATH], szPassword[MAX_PATH];
	GetDlgItemText(IDC_USERNAME,szUserName,MAX_PATH);
	GetDlgItemText(IDC_PASSWORD,szPassword,MAX_PATH);

	
	HRASCONN hRasConn = NULL;
 
	RASDIALPARAMS rdParams;							//拨号连接的信息
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, strEntryName);
	lstrcpy(rdParams.szUserName, szUserName);
	lstrcpy(rdParams.szPassword, szPassword);

	
	DWORD dwRet;
	dwRet = ::RasDial(NULL, NULL, &rdParams, 0L, NULL , NULL);

	
	if (dwRet)
	{
		char  szBuf[256] = "";CString error;
		::RasGetErrorString((UINT)dwRet, (LPSTR)szBuf, 256);
	//etDlgItemText(IDC_CONNECT_STATE,CString("连接错误! ") + CString(szBuf) );
	//fxBeginThread(sndError,this);
//		m_bConnecting=false;
   error=szBuf;
		//	WinExec("rasphone -a",SW_SHOW);
	MessageBox(error,error,MB_OK);	
		
	}    
 else
 {
	WinExec("pppoe.exe",NULL);    
	
   SendMessage(WM_CLOSE,0,0);
 }
}

void CPppoeDlg::OnButton2() 
{
	SendMessage(WM_CLOSE,0,0);	
}
