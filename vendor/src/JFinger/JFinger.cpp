// JFinger.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "JFinger.h"
# include "DlgConfirmar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJFingerApp

BEGIN_MESSAGE_MAP(CJFingerApp, CWinApp)
END_MESSAGE_MAP()


// CJFingerApp construction

CJFingerApp::CJFingerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CJFingerApp object

CJFingerApp theApp;


// CJFingerApp initialization

BOOL CJFingerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

DWORD ConfirmarDlg(CFingerMessage *finger) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgConfirmar dlg;
	dlg.m_pFinger= finger;
	return dlg.DoModal();
}

