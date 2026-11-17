#include "StdAfx.h"
#include "ImagenStream.h"
# include "DlgProgreso.h"

CImagenStream::CImagenStream(void)
{
	_refcount=0;
}

CImagenStream::~CImagenStream(void)
{
}

int CImagenStream::GetSize() const {

	return m_pArray.GetSize();
}

BYTE *CImagenStream::Copy() {

	BYTE *temp= new BYTE[m_pArray.GetSize()];
	memcpy(temp, m_pArray.GetData(), m_pArray.GetSize());
	return temp;
}

CByteArray *CImagenStream::GetArray() {

	return &m_pArray;
}

HRESULT STDMETHODCALLTYPE CImagenStream::Write(void const* pv, ULONG cb, ULONG* pcbWritten)
{
	CDlgProgreso *dlg= CDlgProgreso::GetDlgActual();
	BYTE *temp= (BYTE *)pv;
	for(ULONG i=0; i<cb; i++) 
		m_pArray.Add(temp[i]);
	*pcbWritten= cb;
	if(dlg)
		dlg->m_PCProgreso.StepIt();
	return S_OK;
}
