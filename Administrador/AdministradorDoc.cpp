// SITDoc.cpp: implementación de la clase CAdminitradorDoc
//

#include "stdafx.h"
#include "Administrador.h"

#include "AdministradorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAdminitradorDoc

IMPLEMENT_DYNCREATE(CAdminitradorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAdminitradorDoc, CDocument)
//	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// Construcción o destrucción de CAdminitradorDoc

CAdminitradorDoc::CAdminitradorDoc()
{
	// TODO: agregar aquí el código de construcción único

}

CAdminitradorDoc::~CAdminitradorDoc()
{
}

BOOL CAdminitradorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// Serialización de CAdminitradorDoc

void CAdminitradorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: agregar aquí el código de almacenamiento
	}
	else
	{
		// TODO: agregar aquí el código de carga
	}
}


// Diagnósticos de CAdminitradorDoc

#ifdef _DEBUG
void CAdminitradorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdminitradorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CAdminitradorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if( !lpszPathName) {
		
		OnNewDocument();
		return true;
	}
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	return TRUE;
}
