#include "StdAfx.h"
#include "FingerMessage.h"
# include <GrFinger.h>

CFingerMessage::CFingerMessage(CString szID)
{
	m_szID= szID;
	m_nTemplate=0;
	m_hBitmap= NULL;
	m_cTemplate=new unsigned char[GR_MAX_SIZE_TEMPLATE];
	m_nAncho=0;
	m_nAlto=0;
	m_nResolucion=0;
	m_cRaw=new unsigned char[GR_MAX_IMAGE_HEIGHT * GR_MAX_IMAGE_WIDTH];
	m_bValida=false;
}

CFingerMessage::~CFingerMessage(void)
{
	if( m_cTemplate)
		delete m_cTemplate;
	if( m_cRaw)
		delete m_cRaw;
}

CString CFingerMessage::GetID() {
	return m_szID;
}

void CFingerMessage::Clean() {

	m_szID.Empty();
	m_nTemplate= 0;
	m_bValida=false;
}

CFingerMessage &CFingerMessage::operator = (CFingerMessage &temp) {

	if( &temp!=this) {

		m_szID= temp.m_szID;
		m_nTemplate= temp.m_nTemplate;
		memcpy(m_cTemplate, temp.m_cTemplate, GR_MAX_SIZE_TEMPLATE);
		m_nAncho= temp.m_nAncho;
		m_nAlto= temp.m_nAlto;
		m_nResolucion= temp.m_nResolucion;
		memcpy(m_cRaw, temp.m_cRaw, GR_MAX_IMAGE_HEIGHT * GR_MAX_IMAGE_WIDTH);
		m_hBitmap= temp.m_hBitmap;
		m_bValida= temp.m_bValida;
	}
	return *this;
}

bool CFingerMessage::SetHuella(unsigned int width, unsigned int height, unsigned char* rawImage, int res) {

	memcpy(m_cRaw, rawImage, width*height);
	m_nAlto= height;
	m_nAncho = width;
	m_nResolucion = res;

	GrCapRawImageToHandle(m_cRaw, m_nAncho, m_nAlto, NULL, m_hBitmap);

	// extraer template

	m_nTemplate=GR_MAX_SIZE_TEMPLATE;

	int result=GrExtract(m_cRaw, m_nAncho, m_nAlto, m_nResolucion, (char*)m_cTemplate,
		&m_nTemplate, GR_DEFAULT_CONTEXT);
	// if error, set template size to 0
	m_bValida= result>0;
	return m_bValida;
}

bool CFingerMessage::SetHuella(CByteArray *bytes) {

	if( bytes==NULL)
		return false;
	memcpy(m_cRaw, bytes->GetData(), bytes->GetCount());
	// aqui hago algo
	GrCapRawImageToHandle(m_cRaw, m_nAncho, m_nAlto, NULL, m_hBitmap);
	m_nTemplate=GR_MAX_SIZE_TEMPLATE;
	int result=GrExtract(m_cRaw, m_nAncho, m_nAlto, m_nResolucion, (char*)m_cTemplate,&m_nTemplate, GR_DEFAULT_CONTEXT);
	// if error, set template size to 0
	if (result < 0){
		m_bValida=false;
	} else if( result==GR_BAD_QUALITY) {
		m_bValida= false;
		// m_pBitmap->Clean();
	} else {
		m_bValida= true;
	}
	return m_bValida;
}


bool CFingerMessage::IsValid() {

#ifdef _DEBUG
	if(m_bValida) 
		ASSERT(m_nTemplate!=0);
#endif
	return m_bValida;
}

bool CFingerMessage::Prepare() {

	if( !IsValid())
		return false;
	return GrIdentifyPrepare((char*)m_cTemplate, GR_DEFAULT_CONTEXT)>=0;
}

bool CFingerMessage::Compare(const char *dedo, int *score) {

	ASSERT(dedo);
	if( !(dedo && IsValid()))
		return false;
	int realscore=0;
	bool igual= GrIdentify((char*)dedo, &realscore, GR_DEFAULT_CONTEXT)==GR_MATCH;
	// bool igual= GrVerify((char *)m_pBitmap->m_cTemplate, (char *)dedo->GetDedo(0), &realscore, GR_DEFAULT_CONTEXT)==GR_MATCH;
	if(score)
		*score= realscore;
	return igual;
}

void CFingerMessage::SetParametros(int alto, int ancho, int res) {

	m_nAlto= alto;
	m_nAncho= ancho;
	m_nResolucion= res;
}

DWORD CFingerMessage::GetSize() {

	return m_nAlto*m_nAncho;
}

BYTE *CFingerMessage::GetRaw() {

	return m_cRaw;
}
BYTE *CFingerMessage::GetTemplate() {

	return m_cTemplate;
}
int	CFingerMessage::GetTamanoTemplate() {

	return m_nTemplate;
}
int	CFingerMessage::GetAlto() {

	return m_nAlto;
}
int	CFingerMessage::GetAncho() {

	return m_nAncho;
}
int	CFingerMessage::GetResolucion() {

	return m_nResolucion;
}

HBITMAP	CFingerMessage::GetBitmap() {

	return m_hBitmap;
}

bool CFingerMessage::SaveToFile(CString archivo) {

	char file[MAX_PATH];
	strcpy(file, archivo);
	return GrCapSaveRawImageToFile(m_cRaw, m_nAncho, m_nAlto, file, GRCAP_IMAGE_FORMAT_BMP) == GR_OK;
}
