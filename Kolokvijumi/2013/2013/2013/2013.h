
// 2013.h : main header file for the 2013 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy2013App:
// See 2013.cpp for the implementation of this class
//

class CMy2013App : public CWinApp
{
public:
	CMy2013App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy2013App theApp;
