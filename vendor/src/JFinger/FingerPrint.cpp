#include "StdAfx.h"
# include "jfinger.h"
#include "fingerprint.h"

CFingerPrint *g_FingerPrint=NULL;
CWnd *CFingerPrint::m_pParent=NULL;

void CALLBACK ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res) {
	
	if(g_FingerPrint)
		g_FingerPrint->ImageEventHandler(idSensor, width, height, rawImage, res);
}

void CALLBACK FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event) {
	if(g_FingerPrint)
		g_FingerPrint->FingerEventHandler(idSensor, event);
}

void CALLBACK StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event) {

	if(g_FingerPrint)
		g_FingerPrint->StatusEventHandler(idSensor, event);
}

CFingerPrint::CFingerPrint(CWnd *parent)
{
	m_pParent= parent;
    // template
	g_FingerPrint= this;
	Init();
	if(AfxGetApp()->GetProfileInt("fingerprint", "ithreshold", 0))
		GrSetIdentifyParameters(AfxGetApp()->GetProfileInt("fingerprint", "ithreshold", 45)
		,AfxGetApp()->GetProfileInt("fingerprint", "irotation", 180)
		, GR_DEFAULT_CONTEXT);
	if(AfxGetApp()->GetProfileInt("fingerprint", "vthreshold", 0))
		GrSetVerifyParameters(AfxGetApp()->GetProfileInt("fingerprint", "vthreshold", 25)
		, AfxGetApp()->GetProfileInt("fingerprint", "vrotation", 180)
		, GR_DEFAULT_CONTEXT);
}

CWnd *CFingerPrint::SetParent(CWnd *parent) {

	CWnd *old= m_pParent;
	m_pParent= parent;
	return old;
}

CFingerPrint::~CFingerPrint(void)
{
	for( int i=0; i<m_pLectores.GetCount(); i++) {
		CFingerMessage *mess= (CFingerMessage *)m_pLectores[i];
		delete mess;
	}
	m_pParent=NULL;
	g_FingerPrint=NULL;
	GrFinalize();
	GrCapFinalize();
}

void CFingerPrint::Init() {

	GrInitialize();
	GrCapInitialize(::StatusEventHandler);
}

CFingerMessage *CFingerPrint::GetFinger(CString idsensor) {

	CFingerMessage *mess=NULL;
	for( int i=0; i<m_pLectores.GetCount(); i++) {

		mess= (CFingerMessage *)m_pLectores[i];
		if(mess->GetID()==idsensor) 
			break;
		mess=NULL;
	}
	return mess;
}

void CFingerPrint::ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res) {
	
	// Copying aquired image
	CFingerMessage *mess= GetFinger(idSensor);
	if(!mess)
		return;
	mess->SetHuella(width, height, rawImage, res);
	if(m_pParent && ::IsWindow(*m_pParent)) 
		m_pParent->SendMessage(WM_FINGERPRINT, GR_FINGER_UP, (LPARAM)mess);
}

void CFingerPrint::FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event) {
	// Just signals that a finger event ocurred.
	// WriteEvent(idSensor, event);
}

void CFingerPrint::StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event) {

	if (event == GR_PLUG) {
		if(!GetFinger(idSensor)) {
			m_pLectores.Add(new CFingerMessage(idSensor));
		}
		GrCapStartCapture(idSensor, ::FingerEventHandler, ::ImageEventHandler);
		if(m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_FINGERPRINT, event, (LPARAM)this);
	} else if (event == GR_UNPLUG) {
		// Stop capturing from unplugged sensor
		CFingerMessage *mess;
		for( int i=0; i<m_pLectores.GetCount(); i++) {
			mess= (CFingerMessage *) m_pLectores[i];
			if(mess->GetID()==idSensor) {
				delete mess;
				m_pLectores.RemoveAt(i);
				break;
			}
		}
		GrCapStopCapture(idSensor);
		if(m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_FINGERPRINT, event, (LPARAM)this);
	}
}

int	 CFingerPrint::GetLectores(CStringArray *lectores/*=NULL*/) {

	if( lectores) {
		lectores->RemoveAll();
		for( int i=0; i<m_pLectores.GetCount(); i++) {
			CFingerMessage *mess=(CFingerMessage *)m_pLectores[i];
			lectores->Add(mess->GetID());
		}
	}
	return m_pLectores.GetCount();
}

CFingerMessage *CFingerPrint::GetFingerPrint(int index) {

	return (CFingerMessage *)m_pLectores[index];
}

