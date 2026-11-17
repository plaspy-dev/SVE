// SVE.h: archivo de encabezado principal para la aplicación SVE
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

# include <jfinger/JFinger.h>
# include <conector/Conector.h>
# include <IOFile/IOFile.h>
# include <hhelp/Hhelp.h>
# include <JPrint/JPrint.h>

#include "resource.h"       // Símbolos principales

# define	GRUPO_ADMINISTRADOR		1


# define	WM_INSERTODS	WM_USER+2000
// CSVEApp:
// Consulte la sección SVE.cpp para obtener información sobre la implementación de esta clase
//

class CSVEApp : public CWinAppEx
{
public:
	CSVEApp();
// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CSVEApp theApp;
