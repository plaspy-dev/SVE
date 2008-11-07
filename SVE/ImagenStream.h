#pragma once
#include "objidl.h"

class CImagenStream : public IStream
{
protected:
	CByteArray	m_pArray;
public:
	CImagenStream(void);
	~CImagenStream(void);
public:
	int GetSize() const;
	BYTE *Copy();
	CByteArray *GetArray();
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject)
	{ 
		if (iid == __uuidof(IUnknown)
			|| iid == __uuidof(IStream)
			|| iid == __uuidof(ISequentialStream))
		{
			*ppvObject = static_cast<IStream*>(this);
			AddRef();
			return S_OK;
		} else
			return E_NOINTERFACE; 
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void) 
	{ 
		return (ULONG)InterlockedIncrement(&_refcount); 
	}

	virtual ULONG STDMETHODCALLTYPE Release(void) 
	{
		ULONG res = (ULONG) InterlockedDecrement(&_refcount);
		/*if (res == 0) 
			delete this;*/
		return res;
	}

	// ISequentialStream Interface
public:
	virtual HRESULT STDMETHODCALLTYPE Read(void* pv, ULONG cb, ULONG* pcbRead)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE Write(void const* pv, ULONG cb, ULONG* pcbWritten);

	// IStream Interface
public:
	virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER)
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*,
		ULARGE_INTEGER*) 
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD)                                      
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE Revert(void)                                       
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)              
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)            
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE Clone(IStream **)                                  
	{ 
		return E_NOTIMPL;   
	}

	virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin,
		ULARGE_INTEGER* lpNewFilePointer)
	{ 
		ASSERT(false);
		/*DWORD dwMoveMethod;

		switch(dwOrigin)
		{
		case STREAM_SEEK_SET:
			dwMoveMethod = FILE_BEGIN;
			break;
		case STREAM_SEEK_CUR:
			dwMoveMethod = FILE_CURRENT;
			break;
		case STREAM_SEEK_END:
			dwMoveMethod = FILE_END;
			break;
		default:   
			return STG_E_INVALIDFUNCTION;
			break;
		}

		if (SetFilePointerEx(_hFile, liDistanceToMove, (PLARGE_INTEGER) lpNewFilePointer,
			dwMoveMethod) == 0)
			return HRESULT_FROM_WIN32(GetLastError());
			return S_OK;
			*/
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG* pStatstg, DWORD grfStatFlag) 
	{
		pStatstg->cbSize.LowPart=m_pArray.GetCount();
		return S_OK;
	}
private:
	LONG _refcount;
};
