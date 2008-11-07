// SITDoc.h: interfaz de la clase CAdminitradorDoc
//


#pragma once

class CAdminitradorDoc : public CDocument
{
protected: // Crear sólo a partir de serialización
	CAdminitradorDoc();
	DECLARE_DYNCREATE(CAdminitradorDoc)

// Atributos
public:

// Operaciones
public:

// Reemplazos
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementación
public:
	virtual ~CAdminitradorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


