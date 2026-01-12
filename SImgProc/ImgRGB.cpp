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

BOOL ImgRGB::Set(const int iWidthIn, const int iHeightIn, const int iChannelIn)
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
	if(iChannel == CHANNEL_3_8RGB)
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

BOOL DLL_IE ReadBmpFromData(BOOL bHeader, BYTE* byData, ImgRGB* imgRGB)
{
	ULONG ulInfoOffset;
	BITMAPINFOHEADER bmih;

	if(bHeader==FALSE)
	{
		ulInfoOffset=0;
	}
	else
	{
		ulInfoOffset=sizeof(BITMAPFILEHEADER) ;

	}

	for(int i=0; i<sizeof(bmih); i++)
	{
		((BYTE*)&bmih)[i] = byData[ulInfoOffset+i];
	}
	ULONG ulDataOffset;
	
	if(bHeader==FALSE)
	{
		ulDataOffset = bmih.biSize+((BITMAPINFOHEADER*)byData)->biClrUsed*4;
	}
	else
	{
		ulDataOffset = ((BITMAPFILEHEADER*)byData)->bfOffBits;
	}


	int iWidth;
	int iHeight;

	iWidth = bmih.biWidth;
	if(bmih.biHeight<0){iHeight = -1*(bmih.biHeight);}
	else{iHeight = (bmih.biHeight);}

	imgRGB->Set(iWidth, iHeight, CHANNEL_3_8RGB);

	int iBitCount = bmih.biBitCount;

	if (iBitCount == 24) 
	{
		int iRowSize = ((bmih.biBitCount * iWidth + 31) / 32)*4;
		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c< iWidth; c++)
			{
				(imgRGB->byImgB)[(iHeight - r -1) *iWidth+c] = byData[ulDataOffset +r*iRowSize +3*c + 0];
				(imgRGB->byImgG)[(iHeight - r -1) *iWidth+c] = byData[ulDataOffset +r*iRowSize +3*c + 1];
				(imgRGB->byImgR)[(iHeight - r -1) *iWidth+c] = byData[ulDataOffset +r*iRowSize +3*c + 2];
			}
		}
		GenRectangle1(&(imgRGB->objDomain), 0, 0, iHeight-1, iWidth-1);
		return TRUE;
	}

	if(iBitCount==1)
	{
		int iRowSize = ((bmih.biBitCount * iWidth + 31) / 32)*4;
		int iPaletteSize = ((bmih.biClrUsed == 0) ? 2 : bmih.biClrUsed);
		BYTE* byPalette=NULL;
		byPalette = new BYTE[iPaletteSize *4];
		for(int i=0; i<iPaletteSize*4; i++)
		{
			byPalette[i]=byData[ulInfoOffset+ sizeof(bmih)+i];
		}

		for(int r=0; r<iHeight; r++)
		{
			for (int c8 = 0; c8 < (iWidth+7)/8; c8++)
			{
				for(int iDataPos = 0; iDataPos<8; iDataPos++)
				{
					if(c8*8 + iDataPos >= iWidth){break;}
					BYTE iIndex = ((byData[ulDataOffset + r*iRowSize + c8]) >> ((7 - iDataPos)*0x01)) & 0x01;

					(imgRGB->byImgB)[(iHeight - r -1) *iWidth+(c8*8+iDataPos)] = byPalette[4*iIndex + 0];
					(imgRGB->byImgG)[(iHeight - r -1) *iWidth+(c8*8+iDataPos)] = byPalette[4*iIndex + 1];
					(imgRGB->byImgR)[(iHeight - r -1) *iWidth+(c8*8+iDataPos)] = byPalette[4*iIndex + 2];
				}
			}
		}
		SAFE_DELETE(byPalette);
		GenRectangle1(&(imgRGB->objDomain), 0, 0, iHeight-1, iWidth-1);
		return TRUE;
	}

	if(iBitCount==4)
	{
		int iRowSize = (( (iWidth+1) / 2 + 3) / 4) * 4;
		int iPaletteSize = ((bmih.biClrUsed == 0) ? 16 : bmih.biClrUsed);
		BYTE* byPalette=NULL;
		byPalette = new BYTE[iPaletteSize *4];
		for(int i=0; i<iPaletteSize*4; i++)
		{
			byPalette[i]=byData[ulInfoOffset+sizeof(bmih)+i];
		}

		for(int r=0; r<iHeight; r++)
		{
			for(int c2=0; c2< (iWidth+1)/2; c2++)
			{
				for(int iDataPos = 0; iDataPos<2; iDataPos++)
				{
					if(c2*2 + iDataPos >= iWidth){break;}
					BYTE iIndex = ((byData[ulDataOffset + r*iRowSize + c2]) >> ((1-iDataPos)*0x04)) & 0x0F;

					(imgRGB->byImgB)[(iHeight - r -1) *iWidth+2*c2+iDataPos] = byPalette[4*iIndex + 0];
					(imgRGB->byImgG)[(iHeight - r -1) *iWidth+2*c2+iDataPos] = byPalette[4*iIndex + 1];
					(imgRGB->byImgR)[(iHeight - r -1) *iWidth+2*c2+iDataPos] = byPalette[4*iIndex + 2];
				}
			}
		}
		SAFE_DELETE(byPalette);
		GenRectangle1(&(imgRGB->objDomain), 0, 0, iHeight-1, iWidth-1);
		return TRUE;
	}

	if(iBitCount==8)
	{
		int iRowSize = ((bmih.biBitCount * iWidth + 31) / 32)*4;
		int iPaletteSize = ((bmih.biClrUsed == 0) ? 256 : bmih.biClrUsed);
		BYTE* byPalette=NULL;
		byPalette = new BYTE[iPaletteSize *4];
		for(int i=0; i<iPaletteSize*4; i++)
		{
			byPalette[i]=byData[ulInfoOffset+sizeof(bmih)+i];
		}

		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c< iWidth; c++)
			{
				int iIndex = byData[ulDataOffset + r*iRowSize + c];
				(imgRGB->byImgB)[(iHeight - r -1) *iWidth+c] = byPalette[4*iIndex + 0];
				(imgRGB->byImgG)[(iHeight - r -1) *iWidth+c] = byPalette[4*iIndex + 1];
				(imgRGB->byImgR)[(iHeight - r -1) *iWidth+c] = byPalette[4*iIndex + 2];
			}
		}
		SAFE_DELETE(byPalette);
		GenRectangle1(&(imgRGB->objDomain), 0, 0, iHeight-1, iWidth-1);
		return TRUE;
	}
	return FALSE;
}


BOOL ImgRGB::Assign(const CString sFilePath)
{

	this->Init();

	CFileFind cf;
	BOOL bRet;

	CFile f;
	ULONGLONG ullSize;
	ULONG ulSize;

	bRet = cf.FindFile(sFilePath);
	if(bRet != TRUE){cf.Close(); return FALSE;}
	cf.Close();

	bRet = f.Open(sFilePath, CFile::modeRead);
	if(bRet != TRUE){return FALSE;}

	ullSize = f.SeekToEnd();
	if(ullSize>=ULONG_MAX){f.Close(); return FALSE;}
	ulSize = (ULONG)ullSize;
	f.SeekToBegin();

	BYTE* byData;
	byData = new BYTE[ulSize];
	f.Read(byData, ulSize);
	f.Close();

	if((byData[0] == 0x42) && (byData[1] == 0x4d))
	{
		bRet = ReadBmpFromData(TRUE, byData,  this);
		SAFE_DELETE(byData);
		return bRet;
	}

	SAFE_DELETE(byData);
	return FALSE;
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
	if(iChannel==CHANNEL_3_8RGB)
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

BOOL ImgRGB::Assign(const BYTE* byData, const int iWidth, const int iHeight)
{

	GenRectangle1(&(this->objDomain), 0, 0, iHeight-1, iWidth-1);
	this->Set(iWidth,iHeight,CHANNEL_1_8);

		for(int r=0; r<iHeight; r++)
		{
			for(int c=0; c<iWidth; c++)
			{
				this->byImg[r*iWidth+c]=byData[r*iWidth+c];
			}
		}
		return TRUE;
}

