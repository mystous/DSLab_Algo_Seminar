
// TowerOfHanoi.h : main header file for the TowerOfHanoi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTowerOfHanoiApp:
// See TowerOfHanoi.cpp for the implementation of this class
//

class CTowerOfHanoiApp : public CWinApp
{
public:
	CTowerOfHanoiApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTowerOfHanoiApp theApp;
