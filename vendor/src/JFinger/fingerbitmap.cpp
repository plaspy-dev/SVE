#include "StdAfx.h"
# include "jfinger.h"
#include "FingerBitmap.h"
# include "GrFinger.h"

IMPLEMENT_DYNAMIC(CFingerBitmap, CBitmapPicture)

CFingerBitmap::CFingerBitmap(void)
{

	m_crOver= ::GetSysColor(COLOR_INACTIVECAPTION);
	m_crFocus=  ::GetSysColor( COLOR_HIGHLIGHT);
	m_crDefault=  ::GetSysColor(COLOR_3DSHADOW);

	m_bDefault= false;
	m_bHideInfo=true;
}

CFingerBitmap::~CFingerBitmap(void)
{
}

BEGIN_MESSAGE_MAP(CFingerBitmap, CBitmapPicture)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CFingerBitmap::Clean() {

	m_pMensaje.Clean();
	SetBitmap((HBITMAP) NULL);
	SetWindowText(m_szMensaje);
	Invalidate();
}
void CFingerBitmap::SetMessage(CString mensaje) {

	m_szMensaje= mensaje;
}

void CFingerBitmap::SetDefault(bool bDefault/*=true*/) {

	m_bDefault= bDefault;
}

void CFingerBitmap::SetBorderColor(COLORREF color) {

	m_crDefault=color;
	Invalidate();
}

BOOL CFingerBitmap::OnEraseBkgnd(CDC* pDC)
{
	CBitmapPicture::OnEraseBkgnd(pDC);
	CRect rect;
	GetClientRect(rect);

	DWORD color= m_bDefault ? m_crOver : m_crDefault;
	if(GetFocus()==this) 
		color= m_crFocus;
	
	CPen pen(PS_SOLID,2,color);
	CPen penl(PS_SOLID, 1, color);

	CBrush *brush=(CBrush *) pDC->SelectStockObject(HasHuella() ? NULL_BRUSH : WHITE_BRUSH);
	CPen *oldpen= pDC->SelectObject(&penl);

	pDC->Rectangle(rect);

	pDC->SelectObject(&pen);

	pDC->MoveTo(rect.right, rect.top+1);
	pDC->LineTo(rect.BottomRight());
	pDC->LineTo(rect.left+1, rect.bottom);
	
	/*pDC->LineTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);*/

	pDC->SelectObject(oldpen);
	pDC->SelectObject(brush);
	return TRUE;
}

bool CFingerBitmap::HasHuella() {

	return m_pMensaje.IsValid();
}

DWORD CFingerBitmap::GetSize() {

	return m_pMensaje.GetSize();
}

void CFingerBitmap::SetParametros(int alto, int ancho, int res) {
	
	m_pMensaje.SetParametros(alto, ancho, res);
}

CFingerMessage *CFingerBitmap::GetHuella() {

	return &m_pMensaje;
}

CFingerMessage *CFingerBitmap::SetHuella(CFingerMessage *huella) {

	ASSERT(huella);
	m_pMensaje= *huella;
	if(m_pMensaje.IsValid()) {
		SetWindowText("");
		SetBitmap(m_pMensaje.GetBitmap());
		Invalidate();
		return &m_pMensaje;
	} else {
		Clean();
	}
	return NULL;
}

CFingerMessage *CFingerBitmap::SetHuella(LPARAM lParam) {

	return SetHuella((CFingerMessage *)lParam);
}

CFingerMessage *CFingerBitmap::SetHuella(CByteArray *bytes) {

	m_pMensaje.SetHuella(bytes);
	return SetHuella(&m_pMensaje);
}

void CFingerBitmap::OnSetFocus(CWnd* pOldWnd)
{
	CBitmapPicture::OnSetFocus(pOldWnd);
	ValidateScroll(this);
	Invalidate();
}

void CFingerBitmap::OnKillFocus(CWnd* pNewWnd) {

	CBitmapPicture::OnKillFocus(pNewWnd);
	Invalidate();
}

void CFingerBitmap::OnSysColorChange() 
{
	CBitmapPicture::OnSysColorChange();

	m_crOver= ::GetSysColor(COLOR_INACTIVECAPTION);
	m_crFocus=  ::GetSysColor( COLOR_HIGHLIGHT);
	m_crDefault=  ::GetSysColor(COLOR_3DSHADOW);
}

void CFingerBitmap::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetFocus();
	CBitmapPicture::OnLButtonUp(nFlags, point);
}
