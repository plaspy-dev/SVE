// OpcionesSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "OpcionesSheet.h"


// COpcionesSheet

IMPLEMENT_DYNAMIC(COpcionesSheet, CPropertySheet)

COpcionesSheet::COpcionesSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

COpcionesSheet::COpcionesSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_pComportamiento);
	AddPage(&m_pImpresion);
	AddPage(&m_pEscaner);
	AddPage(&m_pCamaras);
}

COpcionesSheet::~COpcionesSheet()
{
}


BEGIN_MESSAGE_MAP(COpcionesSheet, CPropertySheet)
END_MESSAGE_MAP()


// COpcionesSheet message handlers
