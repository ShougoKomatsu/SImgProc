#include "stdafx.h"


#pragma once
#ifdef __IMGPROC__
#undef DLL_IE
#define DLL_IE __declspec(dllexport)
#else
#define DLL_IE __declspec(dllimport)
#endif

#define CHANNEL_UNDEFINED (0)
#define CHANNEL_1_8 (1)
#define CHANNEL_3_8 (3)
#define CHANNEL_1_24BGR (24)
#define CHANNEL_3_FLOAT (103)


enum RECTANGLE_TYPE
{
	TYPE_UNDEFINED = 0,
	TYPE_RECTANGLE1 = 1
};

struct Rectangle
{
	int iR0;
	int iR1;
	int iC0;
	int iC1;
	int iType;
};




struct ImgRegion
{
	UINT* uiImg;
	int iWidth;
	int iHeight;
	int iChannel;//è´óàégÇ§Ç©Ç‡ÇµÇÍÇ»Ç¢
	BOOL Set(int iWidthIn, int iHeightIn)
	{
		Init();
		if(iWidthIn<=0){return FALSE;}
		if(iHeightIn<=0){return FALSE;}

		iWidth = iWidthIn;
		iHeight = iHeightIn;
		iChannel = 0;
		uiImg= new UINT[iWidth*iHeight*3];
		return TRUE;
	}
	BOOL Init()
	{
		if(uiImg!=NULL){delete [] uiImg; uiImg=NULL;}
		iWidth=0;
		iHeight=0;
		iChannel=CHANNEL_UNDEFINED;
		return TRUE;
	}
	ImgRegion(){uiImg = NULL;Init();}
	~ImgRegion(){Init();}
};


struct DLL_IE ImgRGB
{
	int iWidth;
	int iHeight;
	int iChannel;
	BYTE* byImgR;
	BYTE* byImgG;
	BYTE* byImgB;
	double* dImg1;
	double* dImg2;
	double* dImg3;
	BYTE* byImg;
	BOOL Set(int iWidthIn, int iHeightIn, int iChannelIn)
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
		return TRUE;
	}
	BOOL Init()
	{
		if(byImg!=NULL){delete [] byImg; byImg=NULL;}
		if(byImgR!=NULL){delete [] byImgR; byImgR=NULL;}
		if(byImgG!=NULL){delete [] byImgG; byImgG=NULL;}
		if(byImgB!=NULL){delete [] byImgB; byImgB=NULL;}
		if(dImg1!=NULL){delete [] dImg1; dImg1=NULL;}
		if(dImg2!=NULL){delete [] dImg2; dImg2=NULL;}
		if(dImg3!=NULL){delete [] dImg3; dImg3=NULL;}
		iWidth=0;
		iHeight=0;
		iChannel=CHANNEL_UNDEFINED;
		return TRUE;
	}
	ImgRGB(){byImg = NULL; byImgR=NULL; byImgG=NULL; byImgB=NULL; dImg1=NULL; dImg2=NULL; dImg3=NULL;Init();}
	~ImgRGB(){Init();}
	BOOL Assign(CString sFilePath);
	BOOL Assign(const ImgRGB* imgRGBIn);
};


struct ImgRGBPyramid
{
	ImgRGB imgRGB;
	int iWidthOriginal;
	int iHeightOriginal;
	BOOL DLL_IE SetPyramid(ImgRGB* imgRGBIn);
};

struct ImgMap
{

	int iWidth;
	int iHeight;
	UINT* uiMap;
	BOOL Set(int iWidthIn, int iHeightIn)
	{
		Init();
		if(iWidthIn<=0){return FALSE;}
		if(iHeightIn<=0){return FALSE;}

		if(iWidthIn*iHeightIn>(UINT_MAX>>8)){return FALSE;}

		iWidth = iWidthIn;
		iHeight = iHeightIn;
		uiMap = new UINT[iWidth*iHeight];
		memset(uiMap,0,iWidth*iHeight);
		return TRUE;
	}
	BOOL Init()
	{
		if(uiMap!=NULL){delete [] uiMap; uiMap=NULL;}
		iWidth=0;
		iHeight=0;
		return TRUE;
	}
	ImgMap(){uiMap=NULL;Init();}
	~ImgMap(){Init();}
};


struct RunLength
{
	int iR;
	int iCStart;
	int iCEnd;
	UINT uiLabel;
	BOOL bValid;
	BOOL bIsConnectionOperated;
	void Set(int iRIn, int iCStartIn, int iCEndIn, UINT uiLabelIn, BOOL bValidIn);
	void Copy(RunLength* runLengthIn);
	RunLength();
};

struct Object
{
	UINT m_uiMaxLabel;
	int m_iMaxID;
	int m_iBufNum;
	RunLength* runLength;

	Object();
	BOOL Init();
	BOOL Alloc(int iBuf);
	BOOL Expand(int iBuf);
	BOOL Copy(Object* objSrc);
	BOOL Add(int iR, int iCStart, int iCEnd, UINT uiLabel);
	BOOL IsNeighbor(RunLength* runLength1, RunLength* runLength2, int iNeighborPolicy);
	BOOL ConnectNeighbor(RunLength* runLength, int iID, int iNeighborPolicy);
	BOOL Connection(int iNeighborPolicy);

	BOOL Truncate();

	BOOL IsInRegion(double dR, double dC);
	BOOL UnionOverwrappedRunlength();
};


BOOL DLL_IE Screenshot(ImgRGB* imgRGB);
BOOL DLL_IE CropImage(ImgRGB* imgRGBin, ImgRGB* imgRGBout, int iR0, int iC0, int iR1, int iC1);

BOOL DLL_IE IsInRegion(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC);
BOOL DLL_IE IsInRegionMask(ImgRGB* imgTarget, ImgRGB* imgModel, ImgRGB* imgMask, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC);
BOOL DLL_IE FindModel(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC, double dThreshPercent=0.0);
BOOL DLL_IE FindModelPyramid(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, double dThreshPercent, int* iFoundR, int* iFoundC);

BOOL DLL_IE GetValue(ImgRGB* imgRGBin, int iR, int iC, int* iValueR, int* iValueG, int* iValueB);
BOOL DLL_IE GetValueInRegion(ImgRGB* imgRGBin, int iR0, int iC0, int iR1, int iC1, double* dValueR, double* dValueG, double* dValueB);

BOOL DLL_IE WriteImage(const ImgRGB* imgRGB, CString sFilePath);

BOOL DLL_IE SortRegion(Object* objIn, CString sMode, CString sAscDsc, Object* objOut);
BOOL DLL_IE Threshold(ImgRGB* imgIn, BYTE byThreshMin, BYTE byThreshMax, Object* ObjOut);
BOOL DLL_IE AreaCenter(Object* obj, double* dArea, double* dR, double* dC);
BOOL DLL_IE SelectObj(Object* objIn, int iID, Object* objOut);
