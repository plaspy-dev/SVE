#pragma once

#include "fingerbitmap.h"
# include "GrFinger.h"
# include "FingerMessage.h"


// wParam del mensaje, lParam
// GR_FINGER_UP, fingermessage
// GR_PLUG, this
// GR_UNPLUG, this 

class __JFINGER_EXPORT__ CFingerPrint
{

protected:
	static CWnd		*m_pParent;
	CPtrArray		m_pLectores;

	CFingerMessage *GetFinger(CString idsensor);

public:
	void ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res);
	void FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event);
	void StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event);
public:
	CFingerPrint(CWnd *parent);
	~CFingerPrint(void);
	static CWnd *SetParent(CWnd *parent);
	void Init();
	int	 GetLectores(CStringArray *lectores=NULL);
	CFingerMessage *GetFingerPrint(int index);
};

// globales
void CALLBACK ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res);
void CALLBACK FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event);
void CALLBACK StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event);
