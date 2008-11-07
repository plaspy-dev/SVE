#pragma once
# include "FingerMessage.h"

class __JFINGER_EXPORT__ CFingerBitmap : public CBitmapPicture
{
	DECLARE_DYNCREATE(CFingerBitmap)
protected:
	bool		m_bDefault;	
	CString		m_szMensaje;

	COLORREF	m_crOver;
	COLORREF	m_crFocus;
	COLORREF	m_crDefault;
public:
	CFingerMessage	m_pMensaje;
	CFingerBitmap(void);
public:
	~CFingerBitmap(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	void SetDefault(bool bDefault=true);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetBorderColor(COLORREF color);
	void Clean();
	void SetMessage(CString mensaje);
	bool HasHuella();
	DWORD GetSize();
	CFingerMessage *GetHuella();
	void SetParametros(int alto, int ancho, int res);
	CFingerMessage *SetHuella(CFingerMessage *huella);
	CFingerMessage *SetHuella(CByteArray *bytes);
	inline	CFingerMessage *SetHuella(LPARAM lParam);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
