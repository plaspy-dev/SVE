// SVE.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "SVE.h"
#include "MainFrm.h"

#include "SVEView.h"
# include "atlbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSVEApp

BEGIN_MESSAGE_MAP(CSVEApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// Construcción de CSVEApp

CSVEApp::CSVEApp()
{
}

CSVEApp theApp;

BOOL CSVEApp::InitInstance()
{

#ifndef _DEBUG
	if(!RegisterFile(CApplicacion::GetAppPath("sve.exe"))) {
		AfxMessageBox("SVE esta corrupto.Por favor reinstale la aplicación.");
		return false;
	}
#endif

	CString myid;
	myid.Format("{6A9FA33B-7458-41%d-B96A-B9D28D4B4DF9}",CApplicacion::GetID());
	if(!CWinAppEx::InitInstance(myid))
		return false;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	if ( !AfxInitRichEdit2() )
	{
		AfxMessageBox("Error iniciando control RichEdit", MB_ICONEXCLAMATION );
		return FALSE;
	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("SDG"));

#if defined(ENABLE_LICENSE_CHECK) && !defined(_DEBUG)
	CTrialInfo trial;
	CString temp= m_lpCmdLine;
	if( temp.Find("-Register")!=-1) 
		return false;

	CMyRegKey key;
	key.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\control");
	CDefender info(key.GetString("licencia","", ""));
	if( info.IsValidSerial()!=&info) {
		if(GetLicenceDlg()!=IDOK)
			return false;
		CDefender info1(key.GetString("licencia","", ""));
		if( info1.IsValidSerial()!=&info1) {
			CTrialInfo trial1;
			if( trial1.GetLeftDays()<0)
				return false;
		}
	}
#endif

	HWND handle= FindWindow(NULL, "SDGFinger - Cliente");
	if(handle && ::IsWindow(handle)) {
		switch(AfxMessageBox("SDGFinger - Cliente.\r¿Se esta ejecutando desea cerrarlo?.", MB_ICONQUESTION | MB_YESNOCANCEL)) {
			case IDYES:
				SendMessage(handle, WM_COMMAND, 2003, 0);
				break;
			case IDCANCEL:
				return false;
				break;
		}
	}

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// quitar de aqui
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

void CSVEApp::OnFileNew() {

	if(((CMainFrame* )m_pMainWnd)->m_pUsuario) {
		((CMainFrame* )m_pMainWnd)->m_bInactivo= true;
	} else if( ((CMainFrame* )m_pMainWnd)->Conectar()) { // load papel defaults
		PRINTDLG pd;
		pd.lStructSize=(DWORD)sizeof(PRINTDLG);
		BOOL bRet=GetPrinterDeviceDefaults(&pd);
		if(bRet)
		{
			// protect memory handle with ::GlobalLock and ::GlobalUnlock
			DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(m_hDevMode);
			// set orientation to landscape
			pDevMode->dmOrientation= GetProfileInt("printer", "orientacion", DMORIENT_PORTRAIT);
			pDevMode->dmPaperSize= GetProfileInt("printer", "papel", DMPAPER_LETTER);
			::GlobalUnlock(m_hDevMode);
		}
	}
}

void CSVEApp::OnFileOpen() {

}

CDocument* CSVEApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return NULL;
}

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CSVEApp::OnAppAbout()
{
	AboutBox(SVE_VERSION);
}


// Controladores de mensaje de CSVEApp

