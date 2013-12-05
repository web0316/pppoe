// pppoe.h : main header file for the PPPOE application
//

#if !defined(AFX_PPPOE_H__8B6816A8_9E4F_44D8_A5D7_BD6A43F5A9FF__INCLUDED_)
#define AFX_PPPOE_H__8B6816A8_9E4F_44D8_A5D7_BD6A43F5A9FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPppoeApp:
// See pppoe.cpp for the implementation of this class
//

class CPppoeApp : public CWinApp
{
public:
	CPppoeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPppoeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPppoeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPPOE_H__8B6816A8_9E4F_44D8_A5D7_BD6A43F5A9FF__INCLUDED_)
