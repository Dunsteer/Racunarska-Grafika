
// Bicikla.h : main header file for the Bicikla application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBiciklaApp:
// See Bicikla.cpp for the implementation of this class
//

class CBiciklaApp : public CWinApp
{
public:
	CBiciklaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBiciklaApp theApp;
