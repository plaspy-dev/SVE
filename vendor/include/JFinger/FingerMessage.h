#pragma once

class __JFINGER_EXPORT__ CFingerMessage
{
protected:
	CString		m_szID;
	int			m_nTemplate;
	unsigned char *m_cTemplate;
	int			m_nAncho;
	int			m_nAlto;
	int			m_nResolucion;
	unsigned char *m_cRaw;
	HBITMAP		m_hBitmap;
	bool		m_bValida;
public:
	CFingerMessage(CString szID="");
	~CFingerMessage(void);

public:
	CFingerMessage &operator = (CFingerMessage &temp);
	CString GetID();
	bool		SetHuella(unsigned int width, unsigned int height, unsigned char* rawImage, int res);
	bool		SetHuella(CByteArray *bytes);
	bool		IsValid();
	bool		Prepare();
	bool		Compare(const char *dedo, int *score=NULL);
	void		Clean();
	void		SetParametros(int alto, int ancho, int res);
	DWORD		GetSize();
	BYTE		*GetRaw();
	BYTE		*GetTemplate();
	int			GetTamanoTemplate();
	int			GetAlto();
	int			GetAncho();
	int			GetResolucion();
	HBITMAP		GetBitmap();
	bool		SaveToFile(CString archivo);
};
