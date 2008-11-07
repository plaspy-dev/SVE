// JFinger.h : main header file for the JFinger DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

# ifndef __JFINGER_EXPORT__
#define __JFINGER_EXPORT__ __declspec(dllimport)
# endif

# include <iofile/IOFile.h>

# define WM_FINGERPRINT	WM_USER+1054
# define WM_FINGERPUT	WM_USER+1055

# include "FingerBitmap.h"
# include "Showpic.h"
# include "fingerprint.h"
# include "FingerMessage.h"
# include "VMR_Capture.h"

class CJFingerApp : public CWinApp
{
public:
	CJFingerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

__JFINGER_EXPORT__ DWORD ConfirmarDlg(CFingerMessage *finger);

#ifndef _JFINGER_DLL_
	#ifdef _DEBUG
		# pragma comment(lib,"jfingerd")
	# else
		# pragma comment(lib,"jfinger")
	# endif
#endif

