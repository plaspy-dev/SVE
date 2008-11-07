//////////////////////////////////////////////////////////////////////
//
//  This class is designed to provide simple interface for 
//  simultaneous Video Capture & Preview using DirectShow
//
//////////////////////////////////////////////////////////////////////
//
//	References: MS DirectShow Samples
//
//		
//////////////////////////////////////////////////////////////////////
//
//	This class was written by Sagar K.R . 
//  Use of this class is not restricted in any
//	way whatsoever.Please report the bugs to krssagar@firsteccom.co.kr
//
//	Special thanks to all the members at The Code Project! 
//	(www.codeproject.com)
//
//////////////////////////////////////////////////////////////////////

// VMR_Capture.h: interface for the CVMR_Capture class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <atlbase.h>
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>

//#define WM_GRAPHNOTIFY  WM_USER+13
enum PLAYSTATE {Stopped, Paused, Running, Init};


class __JFINGER_EXPORT__ CVMR_Capture  
{
public:

	CVMR_Capture();
	static int EnumDevices(HWND hList);
	HRESULT Init(int iDeviceID,HWND hWnd,int iWidth,int iHeight);
	DWORD GetFrame(BYTE ** pFrame);
	BOOL Pause();
	DWORD ImageCapture(LPCTSTR szFile);
	DWORD GrabFrame();
	bool  IsPaused();
	virtual ~CVMR_Capture();	
	void SetPause(bool pause=true);

protected:
	
	bool			m_bPause;
	IGraphBuilder *m_pGB ;
	IMediaControl *m_pMC;
	IMediaEventEx *m_pME ;
	//IMediaEvent *pME ;
	
	
	IVMRWindowlessControl9 *m_pWC;
	IPin * m_pCamOutPin;
	IBaseFilter *m_pDF;

	PLAYSTATE m_psCurrent;

	int m_nWidth;
	int m_nHeight;

	BYTE *m_pFrame;
	long m_nFramelen;

	
	bool BindFilter(int deviceId, IBaseFilter **pFilter);
	HRESULT InitializeWindowlessVMR(HWND hWnd);
	HRESULT InitVideoWindow(HWND hWnd,int width, int height);
	void StopCapture();
	void CloseInterfaces(void);
	
	void DeleteMediaType(AM_MEDIA_TYPE *pmt);
	bool Convert24Image(BYTE *p32Img,BYTE *p24Img,DWORD dwSize32);



private:
	
};

