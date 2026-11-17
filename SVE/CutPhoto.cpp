#include "StdAfx.h"
#include "CutPhoto.h"

CCutPhoto::CCutPhoto(void)
{
}

CCutPhoto::~CCutPhoto(void)
{
}

BOOL CCutPhoto::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest) {

	int realy=0;
	for (int nY = 0; nY < sizeSrc.cy; nY++)
	{
		bool copiar=false;
		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			if(pRGBSrc->btBlue>100 || pRGBSrc->btGreen>100 || pRGBSrc->btRed>100) {
				copiar=true;
				break;
			}
		}
		if(copiar) {
			for (int nX = 0; nX < sizeSrc.cx; nX++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[realy * sizeSrc.cx + nX];
				*pRGBDest = *pRGBSrc;
			}
			realy++;
		}
	}
	sizeDest.cy= realy;
	
	realy=0;
	for (int nY = 0; nY < sizeSrc.cy; nY++)
	{
		bool copiar=false;
		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			if(pRGBSrc->btBlue<100 || pRGBSrc->btGreen<100 || pRGBSrc->btRed<100) {
				copiar=true;
				break;
			}
		}
		if(copiar) 
			realy++;
	}
	if(realy<sizeDest.cy) {

		realy=0;
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			bool copiar=false;
			for (int nX = 0; nX < sizeSrc.cx; nX++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				if(pRGBSrc->btBlue<100 || pRGBSrc->btGreen<100 || pRGBSrc->btRed<100) {
					copiar=true;
					break;
				}
			}
			if(copiar) {
				for (int nX = 0; nX < sizeSrc.cx; nX++)
				{
					RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
					RGBX* pRGBDest = &pDestPixels[realy * sizeSrc.cx + nX];
					*pRGBDest = *pRGBSrc;
				}
				realy++;
			}
		}
		sizeDest.cy= realy;
	}
	return TRUE;
}

CCutPhotoV::CCutPhotoV(void)
{
}

CCutPhotoV::~CCutPhotoV(void)
{
}

BOOL CCutPhotoV::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest) {

	int realx=0;

	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		bool copiar=false;
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			if(pRGBSrc->btBlue>100 || pRGBSrc->btGreen>100 || pRGBSrc->btRed>100) {
				copiar=true;
				break;
			}
		}
		if(copiar) 
			realx++;
	}

	sizeDest.cx= realx;
	realx=0;

	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		bool copiar=false;
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			if(pRGBSrc->btBlue>100 || pRGBSrc->btGreen>100 || pRGBSrc->btRed>100) {
				copiar=true;
				break;
			}
		}
		if(copiar) {
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + realx];
				*pRGBDest = *pRGBSrc;
			}
			realx++;
		}
	}
	return TRUE;
}
