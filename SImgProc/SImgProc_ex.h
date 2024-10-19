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
	BOOL Set(int iWidthIn, int iHeightIn, int iChannelIn);
	BOOL Init();
	ImgRGB();
	~ImgRGB();
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
	BOOL SortR();
	BOOL Truncate();
	BOOL ReCheckID();
	BOOL IsInRegion(double dR, double dC);
	BOOL UnionOverwrappedRunlength();
};

BOOL DLL_IE WriteImage(const ImgRGB* imgRGB, CString sFilePath);
BOOL DLL_IE ReadImage(CString sFilePath, ImgRGB* imgRGB);
BOOL DLL_IE ConvertImage(ImgRGB* imgIn, ImgRGB* imgOut,CString sDstColor);
BOOL DLL_IE Decompose3(ImgRGB* imgIn, ImgRGB* imgR, ImgRGB* imgG, ImgRGB* imgB);
BOOL DLL_IE Compose3(ImgRGB* imgR, ImgRGB* imgG, ImgRGB* imgB, ImgRGB* imgOut);

BOOL DLL_IE Screenshot(ImgRGB* imgRGB);
BOOL DLL_IE CropImage(ImgRGB* imgRGBin, ImgRGB* imgRGBout, int iR0, int iC0, int iR1, int iC1);

BOOL DLL_IE IsInRegion(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC);
BOOL DLL_IE IsInRegionMask(ImgRGB* imgTarget, ImgRGB* imgModel, ImgRGB* imgMask, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC);
BOOL DLL_IE FindModel(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC, double dThreshPercent=0.0);
BOOL DLL_IE FindModelPyramid(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, double dThreshPercent, int* iFoundR, int* iFoundC);

BOOL DLL_IE GetValue(ImgRGB* imgRGBin, int iR, int iC, int* iValueR, int* iValueG, int* iValueB);
BOOL DLL_IE GetValueInRegion(ImgRGB* imgRGBin, int iR0, int iC0, int iR1, int iC1, double* dValueR, double* dValueG, double* dValueB);


BOOL DLL_IE SortRegion(Object* objIn, CString sMode, CString sAscDsc, Object* objOut);
BOOL DLL_IE Threshold(ImgRGB* imgIn, BYTE byThreshMin, BYTE byThreshMax, Object* ObjOut);
BOOL DLL_IE AreaCenter(Object* obj, double* dArea, double* dR, double* dC);
BOOL DLL_IE SelectObj(Object* objIn, int iID, Object* objOut);
BOOL DLL_IE PaintRegion(ImgRGB* imgIn, Object* objIn, ImgRGB* imgOut, BYTE byR, BYTE byG, BYTE byB);
BOOL DLL_IE PaintRegion(ImgRGB* imgIn, Object* objIn, ImgRGB* imgOut);
BOOL DLL_IE Decompose3(ImgRGB* imgIn, ImgRGB* imgR, ImgRGB* imgG, ImgRGB* imgB);
BOOL DLL_IE SelectShape(Object* objIn, Object* objOut, CString sFeature, double dMin, double dMax);
BOOL DLL_IE ConcatObj(Object* objIn1, Object* objIn2, Object* objOut);

BOOL DLL_IE Connection(Object* objIn, Object* objOut, int iNeighborPolicy);