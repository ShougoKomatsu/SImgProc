#include "stdafx.h"
#include "SImgProc_ex.h"
#include "CommonFunction.h"
ImgRGB::ImgRGB(){byImg = NULL; byImgR=NULL; byImgG=NULL; byImgB=NULL; dImg1=NULL; dImg2=NULL; dImg3=NULL;Init();}

ImgRGB::~ImgRGB(){Init();}
BOOL ImgRGB::Init()
{
	if(byImg!=NULL){SAFE_DELETE(byImg); byImg=NULL;}
	if(byImgR!=NULL){SAFE_DELETE(byImgR); byImgR=NULL;}
	if(byImgG!=NULL){SAFE_DELETE(byImgG); byImgG=NULL;}
	if(byImgB!=NULL){SAFE_DELETE(byImgB); byImgB=NULL;}
	if(dImg1!=NULL){SAFE_DELETE(dImg1); dImg1=NULL;}
	if(dImg2!=NULL){SAFE_DELETE(dImg2); dImg2=NULL;}
	if(dImg3!=NULL){SAFE_DELETE(dImg3); dImg3=NULL;}
	iWidth=0;
	iHeight=0;
	iChannel=CHANNEL_UNDEFINED;
	this->objDomain.Init();
	return TRUE;
}

BOOL ImgRGB::Set(int iWidthIn, int iHeightIn, int iChannelIn)
{
	Init();
	if(iWidthIn<=0){return FALSE;}
	if(iHeightIn<=0){return FALSE;}

	iWidth = iWidthIn;
	iHeight = iHeightIn;
	iChannel = iChannelIn;
	if(iChannel == CHANNEL_3_FLOAT)
	{
		dImg1 = new double[iWidth*iHeight];
		dImg2 = new double[iWidth*iHeight];
		dImg3 = new double[iWidth*iHeight];
	}
	if(iChannel == CHANNEL_1_8)
	{
		byImg = new BYTE[iWidth*iHeight];
		memset(byImg,0,iWidth*iHeight);
	}
	if(iChannel == CHANNEL_3_8)
	{
		byImgR = new BYTE[iWidth*iHeight];
		byImgG = new BYTE[iWidth*iHeight];
		byImgB = new BYTE[iWidth*iHeight];
		byImg=NULL;
	}
	if(iChannel == CHANNEL_1_24BGR)
	{
		byImg = new BYTE[iWidth*iHeight*3];
	}
	GenRectangle1(&(this->objDomain), 0, 0, iHeight-1, iWidth-1);
	return TRUE;
}

BOOL ImgRGB::Assign(CString sFilePath)
{

	this->Init();

	BITMAPINFOHEADER bmih;
	CFileFind cf;
	BOOL bRet;

	CFile f;
	BITMAPFILEHEADER bmfh;
	ULONGLONG ullSize;
	ULONG ulSize;
	BYTE* byData;

	bRet = cf.FindFile(sFilePath);
	if(bRet != TRUE){cf.Close(); return FALSE;}
	cf.Close();

	bRet = f.Open(sFilePath, CFile::modeRead);
	if(bRet != TRUE){return FALSE;}

	ullSize = f.SeekToEnd();
	if(ullSize>=ULONG_MAX){f.Close(); return FALSE;}
	ulSize = (ULONG)ullSize;
	f.SeekToBegin();
	byData = new BYTE[ulSize];
	f.Read(byData, ulSize);
	f.Close();

	for(int i=0; i<sizeof(bmfh); i++)
	{
		((BYTE*)&bmfh)[i]=byData[i];
	}
	if(bmfh.bfType != 0x4d42){SAFE_DELETE(byData); return FALSE;}

	for(int i=0; i<sizeof(bmih); i++)
	{
		((BYTE*)&bmih)[i]=byData[sizeof(bmfh)+i];
	}


	int iWidthLocal;
	int iHeightLocal;

	iWidthLocal = bmih.biWidth;
	if(bmih.biHeight<0){iHeightLocal=-1*(bmih.biHeight);}
	else{iHeightLocal=(bmih.biHeight);}

	this->Set(iWidthLocal, iHeightLocal, CHANNEL_3_8);

	ULONG ulOffset;
	ulOffset=bmfh.bfOffBits;
	int iFiller;

	iFiller = iWidth%4;

	for(int r=0; r<iHeightLocal; r++)
	{
		for(int c=0; c< iWidthLocal; c++)
		{
			(this->byImgB)[(this->iHeight - r -1) *this->iWidth+c]=byData[ulOffset +3*( r*iWidthLocal + c)+r*iFiller+0];
			(this->byImgG)[(this->iHeight - r -1) *this->iWidth+c]=byData[ulOffset +3*( r*iWidthLocal + c)+r*iFiller+1];
			(this->byImgR)[(this->iHeight - r -1) *this->iWidth+c]=byData[ulOffset +3*( r*iWidthLocal + c)+r*iFiller+2];
		}
	}

	SAFE_DELETE(byData);
	GenRectangle1(&(this->objDomain), 0, 0, iHeightLocal-1, iWidthLocal-1);
	return TRUE;
}


BOOL ImgRGB::Assign(const ImgRGB* imgRGBIn)
{
	if(imgRGBIn==this){return TRUE;}

	GenRectangle1(&(this->objDomain), 0, 0, imgRGBIn->iHeight-1, imgRGBIn->iWidth-1);
	this->Set(imgRGBIn->iWidth,imgRGBIn->iHeight,imgRGBIn->iChannel);

	if(iChannel==CHANNEL_1_24BGR)
	{
		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c<iWidth; c++)
			{
				this->byImg[3*(r*iWidth+c)+0]=imgRGBIn->byImg[3*(r*iWidth+c)+0];
				this->byImg[3*(r*iWidth+c)+1]=imgRGBIn->byImg[3*(r*iWidth+c)+1];
				this->byImg[3*(r*iWidth+c)+2]=imgRGBIn->byImg[3*(r*iWidth+c)+2];
			}
		}
		return TRUE;
	}
	if(iChannel==CHANNEL_1_8)
	{
		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c<iWidth; c++)
			{
				this->byImg[r*iWidth+c]=imgRGBIn->byImg[r*iWidth+c];
			}
		}
		return TRUE;
	}
	if(iChannel==CHANNEL_3_8)
	{
		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c<iWidth; c++)
			{
				this->byImgB[r*iWidth+c]=imgRGBIn->byImgB[r*iWidth+c];
				this->byImgG[r*iWidth+c]=imgRGBIn->byImgG[r*iWidth+c];
				this->byImgR[r*iWidth+c]=imgRGBIn->byImgR[r*iWidth+c];
			}
		}
		return TRUE;
	}
	return FALSE;
}

