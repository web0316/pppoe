// pppoeDlg.h : header file
//

#if !defined(AFX_PPPOEDLG_H__82480ADC_4D03_4408_8B61_FB00982DF4AF__INCLUDED_)
#define AFX_PPPOEDLG_H__82480ADC_4D03_4408_8B61_FB00982DF4AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPppoeDlg dialog

class CPppoeDlg : public CDialog
{
// Construction
public:
	CPppoeDlg(CWnd* pParent = NULL);	// standard constructor
CString m_strUserName,m_strPassword;
// Dialog Data
	//{{AFX_DATA(CPppoeDlg)
	enum { IDD = IDD_PPPOE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPppoeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPppoeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPPOEDLG_H__82480ADC_4D03_4408_8B61_FB00982DF4AF__INCLUDED_)
