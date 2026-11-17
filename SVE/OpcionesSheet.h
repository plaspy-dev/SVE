#pragma once

#include "ComportamientoPP.h"
#include "CamarasPP.h"
# include "Impresionpp.h"
# include "EscanerPP.h"
// COpcionesSheet

class COpcionesSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COpcionesSheet)

public:
	COpcionesSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COpcionesSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~COpcionesSheet();
	CComportamientoPP	m_pComportamiento;
	CCamarasPP			m_pCamaras;
	CImpresionPP		m_pImpresion;
	CEscanerPP			m_pEscaner;

protected:
	DECLARE_MESSAGE_MAP()
};


