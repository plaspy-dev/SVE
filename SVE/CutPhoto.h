#pragma once
#include <Componentes/Componentes.h>

class CCutPhoto : public C32BitImageProcessor
{
public:
	CCutPhoto(void);
public:
	~CCutPhoto(void);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
};


class CCutPhotoV : public C32BitImageProcessor
{
public:
	CCutPhotoV(void);
public:
	~CCutPhotoV(void);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
};
