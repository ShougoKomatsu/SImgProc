#include "StdAfx.h"
#include "ImgProc.h"
#include "CommonFunction.h"

#include "runlength.h"
#include "Camera.h"

BOOL ConvertImage(ImgRGB* imgIn, ImgRGB* imgOut,CString sDstColor)
{
	if(imgIn==NULL){return FALSE;}
	if(imgOut==NULL){return FALSE;}
	if(imgIn==imgOut){return FALSE;}//‚¢‚¸‚ê‘Î‰ž‚·‚é

	if(sDstColor.CompareNoCase(_T("hsv"))==0)
	{
		imgOut->Set(imgIn->iWidth, imgIn->iWidth, CHANNEL_3_FLOAT);

		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				BYTE byR=imgIn->byImgR[r*imgIn->iWidth+c];
				BYTE byG=imgIn->byImgG[r*imgIn->iWidth+c];
				BYTE byB=imgIn->byImgB[r*imgIn->iWidth+c];

				BYTE byMax=(byR>=byG ? (byR >= byB ? byR : (byG>=byB ? byG : byB)) : (byG>=byB ? byG : byB));
				BYTE byMin=(byR<=byG ? (byR <= byB ? byR : (byB<=byG ? byB : byG)) : (byB<=byG ? byB : byG));
				if(byMax==0)
				{
					imgOut->dImg1[r*imgIn->iWidth+c] =0;
					imgOut->dImg2[r*imgIn->iWidth+c] =0;
					imgOut->dImg3[r*imgIn->iWidth+c] =0;
					continue;
				}

				if(byMax==byR)
				{
					if(byMin==byG)
					{
						//byMin==byG==byB‚Ì‚Æ‚«AbyMin=byB‚É‚È‚é‚Ì‚Å‚±‚±‚É‚Í—ˆ‚È‚¢
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(5+(byMax-byB)/(byMax*1.0));
					}
					else
					{
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(0+(byMax-byG)/(byMax*1.0));
					}
				}
				if(byMax==byG)
				{
					if(byMin==byB)
					{
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(1+(byMax-byR)/(byMax*1.0));
					}
					else
					{
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(2+(byMax-byB)/(byMax*1.0));
					}
				}
				if(byMax==byB)
				{
					if(byMin==byR)
					{
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(3+(byMax-byG)/(byMax*1.0));
					}
					else
					{
						imgOut->dImg1[r*imgIn->iWidth+c] = 60.0*(4+(byMax-byR)/(byMax*1.0));
					}
				}
				imgOut->dImg2[r*imgIn->iWidth+c] = (byMax-byMin)/(byMax*1.0);
				imgOut->dImg3[r*imgIn->iWidth+c] = byMax;

			}
		}
		return TRUE;
	}

	if(sDstColor.CompareNoCase(_T("rgb"))==0)
	{
		if(imgIn->iChannel!=CHANNEL_3_FLOAT){return FALSE;}

		imgOut->Set(imgIn->iWidth, imgIn->iWidth, CHANNEL_3_8RGB);

		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				double dH=imgIn->dImg1[r*imgIn->iWidth+c];
				double dS=imgIn->dImg2[r*imgIn->iWidth+c];
				double dV=imgIn->dImg3[r*imgIn->iWidth+c];

				if(dH<=60.0)
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV;
					imgOut->byImgG[r*imgIn->iWidth+c] = dV*(dS*((dH-0)/60.0));
					imgOut->byImgB[r*imgIn->iWidth+c] = dV*(1 - dS);
				}
				else if(dH<=120.0)
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV*(dS*(dH-60.0)/60.0);
					imgOut->byImgG[r*imgIn->iWidth+c] = dV;
					imgOut->byImgB[r*imgIn->iWidth+c] = dV*(1 - dS);
				}
				else if(dH<=180.0)
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV*(1 - dS);
					imgOut->byImgG[r*imgIn->iWidth+c] = dV;
					imgOut->byImgB[r*imgIn->iWidth+c] = dV*(dS*(dH-120.0)/60.0);
				}
				else if(dH<=240.0)
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV*(1 - dS);
					imgOut->byImgG[r*imgIn->iWidth+c] = dV*(dS*(dH-180.0)/60.0);
					imgOut->byImgB[r*imgIn->iWidth+c] = dV;
				}
				else if(dH<=300.0)
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV*(dS*(dH-240.0)/60.0);
					imgOut->byImgG[r*imgIn->iWidth+c] = dV*(1 - dS);
					imgOut->byImgB[r*imgIn->iWidth+c] = dV;
				}
				else
				{
					imgOut->byImgR[r*imgIn->iWidth+c] = dV;
					imgOut->byImgG[r*imgIn->iWidth+c] = dV*(1 - dS);
					imgOut->byImgB[r*imgIn->iWidth+c] = dV*(dS*(dH-300.0)/60.0);
				}
			}
		}
		return TRUE;
	}

	return TRUE;
}

BOOL Decompose3(ImgRGB* imgIn, ImgRGB* imgR, ImgRGB* imgG, ImgRGB* imgB)
{
	if((imgIn->iChannel != CHANNEL_3_8RGB) && (imgIn->iChannel != CHANNEL_1_24BGR)){return FALSE;}
	imgR->Set(imgIn->iWidth, imgIn->iHeight, CHANNEL_1_8);
	imgG->Set(imgIn->iWidth, imgIn->iHeight, CHANNEL_1_8);
	imgB->Set(imgIn->iWidth, imgIn->iHeight, CHANNEL_1_8);

	imgR->objDomain.Copy(&(imgIn->objDomain));
	imgG->objDomain.Copy(&(imgIn->objDomain));
	imgB->objDomain.Copy(&(imgIn->objDomain));
	if(imgIn->iChannel==CHANNEL_3_8RGB)
	{
		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				imgR->byImg[r*imgR->iWidth+c]=imgIn->byImgR[r*imgIn->iWidth+c];
				imgG->byImg[r*imgG->iWidth+c]=imgIn->byImgG[r*imgIn->iWidth+c];
				imgB->byImg[r*imgB->iWidth+c]=imgIn->byImgB[r*imgIn->iWidth+c];
			}
		}
		return TRUE;
	}

	if(imgIn->iChannel==CHANNEL_1_24BGR)
	{
		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				imgB->byImg[r*imgB->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+0];
				imgG->byImg[r*imgG->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+1];
				imgR->byImg[r*imgR->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+2];
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Compose3(ImgRGB* imgR, ImgRGB* imgG, ImgRGB* imgB, ImgRGB* imgOut)
{
	if(imgR->iChannel != CHANNEL_1_8){return FALSE;}
	if(imgG->iChannel != CHANNEL_1_8){return FALSE;}
	if(imgB->iChannel != CHANNEL_1_8){return FALSE;}
	if(imgR->iHeight != imgG->iHeight){return FALSE;}
	if(imgR->iHeight != imgB->iHeight){return FALSE;}
	if(imgR->iWidth != imgG->iWidth){return FALSE;}
	if(imgR->iWidth != imgB->iWidth){return FALSE;}

	imgOut->Set(imgR->iWidth, imgR->iHeight, CHANNEL_3_8RGB);

	for(int r=0; r<imgR->iHeight; r++)
	{
		for(int c=0; c<imgR->iWidth; c++)
		{
			imgOut->byImgR[r*imgOut->iWidth+c]=imgR->byImg[r*imgR->iWidth+c];
			imgOut->byImgG[r*imgOut->iWidth+c]=imgG->byImg[r*imgG->iWidth+c];
			imgOut->byImgB[r*imgOut->iWidth+c]=imgB->byImg[r*imgB->iWidth+c];
		}
	}
	return TRUE;
}

BOOL Threshold(ImgRGB* imgIn, BYTE byThreshMin, BYTE byThreshMax, Object* ObjOut)
{
	if(imgIn->iChannel != CHANNEL_1_8){return FALSE;}

	ObjOut->Init();

	for(int r=0; r<imgIn->iHeight; r++)
	{
		if(imgIn->objDomain.IsRInRegion(r)==FALSE){continue;}

		int iIDStart;
		int iIDEnd;
		imgIn->objDomain.GetRunlengthIDsInR(r,&iIDStart, &iIDEnd);

		for(int iID=iIDStart; iID<=iIDEnd; iID++)
		{
			int iStartC=-1;
			int iCStart=max(imgIn->objDomain.runLength[iID].iCStart,0);
			int iCEnd=min(imgIn->objDomain.runLength[iID].iCEnd,imgIn->iWidth-1);
			for(int c=iCStart; c<=iCEnd; c++)
			{

				if(iStartC>=0)
				{
					if((imgIn->byImg[r*imgIn->iWidth+c] < byThreshMin)
						|| (imgIn->byImg[r*imgIn->iWidth+c] > byThreshMax))
					{
						ObjOut->Add(r,iStartC,c-1,0);
						iStartC=-1;
					}
					continue;
				}

				if((imgIn->byImg[r*imgIn->iWidth+c] >= byThreshMin)
					&& (imgIn->byImg[r*imgIn->iWidth+c] <= byThreshMax))
				{
					iStartC=c;
				}

			}

			if(iStartC>=0)
			{
				ObjOut->Add(r,iStartC,iCEnd,0);
			}
		}

	}

	return TRUE;
}
BOOL ReduceDomain(ImgRGB* imgRGBIn, Object* objIn, ImgRGB* imgRGBOut)
{
	imgRGBOut->Assign(imgRGBIn);
	imgRGBOut->objDomain.Copy(objIn);
	return TRUE;
}
BOOL SelectObj(Object* objIn, int iLabel, Object* objOut)
{
	objOut->Init();
	for(int iID=0; iID<=objIn->m_iMaxID; iID++)
	{
		if(objIn->runLength[iID].uiLabel==iLabel+1)
		{
			objOut->Add(objIn->runLength[iID].iR,objIn->runLength[iID].iCStart,objIn->runLength[iID].iCEnd,0);
		}
	}
	return TRUE;
}BOOL GetValue(ImgRGB* imgRGBin, int iR, int iC, int* iValueR, int* iValueG, int* iValueB)
{
	if(imgRGBin==NULL){return FALSE;}
	if(iR<0){return FALSE;}
	if(iC<0){return FALSE;}
	if(iR>=imgRGBin->iHeight){return FALSE;}
	if(iC>=imgRGBin->iWidth){return FALSE;}

	if(imgRGBin->iChannel==CHANNEL_1_24BGR)
	{
		*iValueB=imgRGBin->byImg[3*(iR*imgRGBin->iWidth+iC)+0];
		*iValueG=imgRGBin->byImg[3*(iR*imgRGBin->iWidth+iC)+1];
		*iValueR=imgRGBin->byImg[3*(iR*imgRGBin->iWidth+iC)+2];
		return TRUE;
	}
	if(imgRGBin->iChannel==CHANNEL_3_8RGB)
	{
		*iValueB=imgRGBin->byImgB[iR*imgRGBin->iWidth+iC];
		*iValueG=imgRGBin->byImgG[iR*imgRGBin->iWidth+iC];
		*iValueR=imgRGBin->byImgR[iR*imgRGBin->iWidth+iC];
		return TRUE;
	}
	if(imgRGBin->iChannel==CHANNEL_1_8)
	{
		*iValueB=imgRGBin->byImg[iR*imgRGBin->iWidth+iC];
		*iValueG=imgRGBin->byImg[iR*imgRGBin->iWidth+iC];
		*iValueR=imgRGBin->byImg[iR*imgRGBin->iWidth+iC];
		return TRUE;
	}
	return FALSE;
}

BOOL GetValueInRegion(ImgRGB* imgRGBin, int iR0, int iC0, int iR1, int iC1, double* dValueR, double* dValueG, double* dValueB)
{
	if(imgRGBin==NULL){return FALSE;}
	if(iR0>iR1){return FALSE;}
	if(iC0>iC1){return FALSE;}
	if(iR0<0){return FALSE;}
	if(iC0<0){return FALSE;}
	if(iR1>=imgRGBin->iHeight-1){return FALSE;}
	if(iC1>=imgRGBin->iWidth-1){return FALSE;}

	ULONGLONG ullSumR=0;
	ULONGLONG ullSumG=0;
	ULONGLONG ullSumB=0;
	if(imgRGBin->iChannel==CHANNEL_1_24BGR)
	{
		for(int r=iR0; r<=iR1; r++)
		{
			for(int c=iC0; c<=iC1; c++)
			{
				ullSumB+=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
				ullSumG+=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
				ullSumR+=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
			}
		}
		*dValueB=ullSumB/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		*dValueG=ullSumG/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		*dValueR=ullSumR/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		return TRUE;
	}

	if(imgRGBin->iChannel==CHANNEL_3_8RGB)
	{
		for(int r=iR0; r<=iR1; r++)
		{
			for(int c=iC0; c<=iC1; c++)
			{
				ullSumB+=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
				ullSumG+=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
				ullSumR+=imgRGBin->byImgR[r*imgRGBin->iWidth+c];
			}
		}
		*dValueB=ullSumB/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		*dValueG=ullSumG/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		*dValueR=ullSumR/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		return TRUE;
	}

	if(imgRGBin->iChannel==CHANNEL_1_8)
	{
		for(int r=iR0; r<=iR1; r++)
		{
			for(int c=iC0; c<=iC1; c++)
			{
				ullSumB+=imgRGBin->byImg[r*imgRGBin->iWidth+c];
			}
		}
		*dValueB=ullSumB/((iR1-iR0+1)*(iC1-iC0+1)*1.0);
		*dValueG=(*dValueB);
		*dValueR=(*dValueB);
		return TRUE;
	}
	return FALSE;
}

BOOL CropImage(ImgRGB* imgRGBin, ImgRGB* imgRGBout, int iR0, int iC0, int iR1, int iC1)
{
	if(imgRGBin==NULL){return FALSE;}
	if(imgRGBout==NULL){return FALSE;}
	if(iR0>iR1){return FALSE;}
	if(iC0>iC1){return FALSE;}
	if(iR0<0){return FALSE;}
	if(iC0<0){return FALSE;}
	if(iR1>=imgRGBin->iHeight){return FALSE;}
	if(iC1>=imgRGBin->iWidth){return FALSE;}

	if(imgRGBin==imgRGBout)
	{
		//
	}
	else
	{
		imgRGBout->Init();
		imgRGBout->Set(iC1-iC0+1, iR1-iR0+1, imgRGBin->iChannel);
		if(imgRGBin->iChannel==CHANNEL_1_24BGR)
		{

			for(int r=iR0; r<=iR1; r++)
			{for(int c=iC0; c<=iC1; c++)
			{
				imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+0]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
				imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+1]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
				imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+2]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
			}}
		}
		if(imgRGBin->iChannel==CHANNEL_3_8RGB)
		{
			for(int r=iR0; r<=iR1; r++)
			{for(int c=iC0; c<=iC1; c++)
			{
				imgRGBout->byImgB[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
				imgRGBout->byImgG[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
				imgRGBout->byImgR[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgR[r*imgRGBin->iWidth+c];
			}}
		}
		if(imgRGBin->iChannel==CHANNEL_1_8)
		{
			for(int r=iR0; r<=iR1; r++)
			{for(int c=iC0; c<=iC1; c++)
			{
				imgRGBout->byImg[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImg[r*imgRGBin->iWidth+c];
			}}
		}


	}
	return TRUE;
}

BOOL Screenshot(ImgRGB* imgRGB)
{
	HWND hDesktop = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hDesktop, &rect);


	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = rect.right;
	bmpInfo.bmiHeader.biHeight = -rect.bottom;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;


	LPDWORD lpPixel;
	HDC hDC = GetDC(hDesktop);
	HBITMAP hBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
	HDC hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);


	BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);

	int iWidth;
	int iHeight;
	iWidth = (rect.right-rect.left);
	iHeight = (rect.bottom-rect.top);

	imgRGB->Set(iWidth, iHeight, CHANNEL_1_24BGR);
	GenRectangle1(&(imgRGB->objDomain),0, 0, iHeight-1, iWidth-1);
	int iFillerSize;

	if((iWidth%4)==0)
	{
		iFillerSize = 0;
	}
	else
	{
		iFillerSize = 4-(iWidth*3)%4;
	}

	int iBitSize;
	iBitSize = ((iWidth*3)+iFillerSize)*iHeight;

	BYTE* byDataTempBGR;
	byDataTempBGR = new BYTE[iBitSize];

	GetDIBits(hMemDC, hBitmap, 0, rect.bottom, byDataTempBGR, (BITMAPINFO *)&bmpInfo, DIB_RGB_COLORS);
	for(int r=0; r<iHeight; r++)
	{
		for(int c=0; c<iWidth; c++)
		{
			imgRGB->byImg[3*(r*iWidth+c)+0] = byDataTempBGR[3*(r*iWidth+c) + r*iFillerSize +0];
			imgRGB->byImg[3*(r*iWidth+c)+1] = byDataTempBGR[3*(r*iWidth+c) + r*iFillerSize +1];
			imgRGB->byImg[3*(r*iWidth+c)+2] = byDataTempBGR[3*(r*iWidth+c) + r*iFillerSize +2];
		}
	}
	SAFE_DELETE(byDataTempBGR);


	ReleaseDC(hDesktop, hDC);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);

	return TRUE;
}

BOOL IsInRegion(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC)
{
	int iModelHeight;
	int iModelWidth;

	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}


	iModelHeight = imgModel->iHeight;
	iModelWidth = imgModel->iWidth;

	
	int iR0Local;
	int iC0Local;
	int iR1Local;
	int iC1Local;

	int iInclR;
	int iInclC;

	if(iR0<iR1)
	{
		iInclR=+1;
		iR0Local = max(iR0, 0);
		iR1Local = min(iR1, imgTarget->iHeight-1);
	}
	else
	{
		iInclR=-1;
		iR0Local = min(iR0, imgTarget->iHeight-1);
		iR1Local = max(iR1, 0);
	}

	if(iC0<iC1)
	{
		iInclC=+1;
		iC0Local = max(iC0, 0);
		iC1Local = min(iC1, imgTarget->iWidth-1);
	}
	else
	{
		iInclC=-1;
		iC0Local = min(iC0, imgTarget->iWidth-1);
		iC1Local = max(iC1, 0);
	}
	
	int iScanHeight;
	int iScanWidth;
	iScanHeight = abs(iR1Local-iR0Local)-iModelHeight + 2;
	iScanWidth = abs(iC1Local-iC0Local)-iModelWidth + 2;

	if(iScanHeight<=0){return FALSE;}
	if(iScanWidth<=0){return FALSE;}

	BOOL bFound;

	int iRStart, iCStart;

	if(iR0<iR1)
	{
		iRStart=iR0Local;
	}
	else
	{
		iRStart=iR0Local-iModelHeight+1;
	}
	if(iC0<iC1)
	{
		iCStart = iC0Local;
	}
	else
	{
		iCStart = iC0Local-iModelWidth+1;
	}

	int iPtrTarget;
	int iPtrModel;

	if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_3_8RGB))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;
				bFound = TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
						iPtrModel = (r)*imgModel->iWidth+(c);
						if(imgTarget->byImg[iPtrTarget + 0] != (imgModel->byImgB[iPtrModel])){bFound = FALSE; break;}
						if(imgTarget->byImg[iPtrTarget + 1] != (imgModel->byImgG[iPtrModel])){bFound = FALSE; break;}
						if(imgTarget->byImg[iPtrTarget + 2] != (imgModel->byImgR[iPtrModel])){bFound = FALSE; break;}
					}
					if(bFound == FALSE){break;}
				}
				if(bFound == TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2; return TRUE;}
			}
		}
	}

	if((imgTarget->iChannel==CHANNEL_3_8RGB) && (imgModel->iChannel == CHANNEL_3_8RGB))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;
				bFound = TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = (iTargetR + r)*imgTarget->iWidth+(iTargetC+c);
						iPtrModel = (r)*imgModel->iWidth+(c);
						if(imgTarget->byImgR[iPtrTarget] != (imgModel->byImgR[iPtrModel])){bFound = FALSE; break;}
						if(imgTarget->byImgG[iPtrTarget] != (imgModel->byImgG[iPtrModel])){bFound = FALSE; break;}
						if(imgTarget->byImgB[iPtrTarget] != (imgModel->byImgB[iPtrModel])){bFound = FALSE; break;}
					}
					if(bFound == FALSE){break;}
				}
				if(bFound == TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2; return TRUE;}
				break;
			}
		}
	}

	if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_1_24BGR))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;
				bFound = TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
						iPtrModel = 3*((r)*imgModel->iWidth+(c));
						if(imgTarget->byImg[iPtrTarget+0] != (imgModel->byImg[iPtrModel + 0])){bFound = FALSE; break;}
						if(imgTarget->byImg[iPtrTarget+1] != (imgModel->byImg[iPtrModel + 1])){bFound = FALSE; break;}
						if(imgTarget->byImg[iPtrTarget+2] != (imgModel->byImg[iPtrModel + 2])){bFound = FALSE; break;}
					}
					if(bFound == FALSE){break;}
				}
				if(bFound == TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2; return TRUE;}
				break;
			}
		}
	}


	return FALSE;
}


inline BYTE bySubAbs(BYTE by1, BYTE by2)
{
	if(by1>=by2){return by1-by2;}
	return by2-by1;
}

BOOL CropImage2(ImgRGB* imgRGBin, ImgRGB* imgRGBout, int iR0, int iC0, int iR1, int iC1)
{
	if(imgRGBin==NULL){return FALSE;}
	if(imgRGBout==NULL){return FALSE;}
	if(iR0>iR1){return FALSE;}
	if(iC0>iC1){return FALSE;}
	if(iR0<0){return FALSE;}
	if(iC0<0){return FALSE;}
	if(iR1>=imgRGBin->iHeight){return FALSE;}
	if(iC1>=imgRGBin->iWidth){return FALSE;}

	if(imgRGBin==imgRGBout)
	{
		//
	}
	else
	{
		imgRGBout->Init();
		int iWidthP1;
		int iHeightP1;
		if(((iC1-iC0+1)%2)==0){iWidthP1=0;}else{iWidthP1=1;}
		if(((iR1-iR0+1)%2)==0){iHeightP1=0;}else{iHeightP1=1;}

		imgRGBout->Set(iC1-iC0+1+iWidthP1, iR1-iR0+1+iHeightP1, imgRGBin->iChannel);
		if(imgRGBin->iChannel==CHANNEL_1_24BGR)
		{

			for(int r=iR0; r<=iR1; r++)
			{
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+0]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+1]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0))+2]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
				}
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0+1))+0]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0+1))+1]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
					imgRGBout->byImg[3*((r-iR0)*imgRGBout->iWidth+(c-iC0+1))+2]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
				}
			}
			if(iHeightP1==1)
			{
				int r=iR1;
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0))+0]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0))+1]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0))+2]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
				}
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0+1))+0]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+0];
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0+1))+1]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+1];
					imgRGBout->byImg[3*((r+1-iR0)*imgRGBout->iWidth+(c-iC0+1))+2]=imgRGBin->byImg[3*(r*imgRGBin->iWidth+c)+2];
				}
			}
		}
		if(imgRGBin->iChannel==CHANNEL_3_8RGB)
		{
			for(int r=iR0; r<=iR1; r++)
			{
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImgB[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
					imgRGBout->byImgG[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
					imgRGBout->byImgR[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgR[r*imgRGBin->iWidth+c];

				}
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImgB[(r-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
					imgRGBout->byImgG[(r-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
					imgRGBout->byImgR[(r-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgR[r*imgRGBin->iWidth+c];
				}
			}
			if(iHeightP1==1)
			{
				int r=iR1;
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImgB[(r+1-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
					imgRGBout->byImgG[(r+1-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
					imgRGBout->byImgR[(r+1-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImgR[r*imgRGBin->iWidth+c];

				}
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImgB[(r+1-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgB[r*imgRGBin->iWidth+c];
					imgRGBout->byImgG[(r+1-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgG[r*imgRGBin->iWidth+c];
					imgRGBout->byImgR[(r+1-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImgR[r*imgRGBin->iWidth+c];
				}
			}
		}
		if(imgRGBin->iChannel==CHANNEL_1_8)
		{
			for(int r=iR0; r<=iR1; r++)
			{
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImg[(r-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImg[r*imgRGBin->iWidth+c];

				}
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImg[(r-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImg[r*imgRGBin->iWidth+c];
				}
			}
			if(iHeightP1==1)
			{
				int r=iR1;
				for(int c=iC0; c<=iC1; c++)
				{
					imgRGBout->byImg[(r+1-iR0)*imgRGBout->iWidth+(c-iC0)]=imgRGBin->byImg[r*imgRGBin->iWidth+c];
				}	
				if(iWidthP1==1)
				{
					int c=iC1;
					imgRGBout->byImg[(r+1-iR0)*imgRGBout->iWidth+(c-iC0+1)]=imgRGBin->byImg[r*imgRGBin->iWidth+c];
				}
			}
		}


	}
	return TRUE;
}

int GetPointerOffset(int iW, int iH, int iLevel)
{
	int iOffset=0;
	int iParentLevelH=iH;
	int iParentLevelW=iW;
	for(int i=0; i<iLevel; i++)
	{
		iOffset+=iParentLevelH*iParentLevelW;
		iParentLevelH = (((iParentLevelH%1)==1) ? (iParentLevelH+1)/2 : (iParentLevelH)/2);
		iParentLevelW = (((iParentLevelW%1)==1) ? (iParentLevelW+1)/2 : (iParentLevelW)/2);
	}
	return iOffset;
}

int GetLevelSize(int iHW, int iLevel)
{
	int iParentLevelHW=iHW;
	for(int i=1; i<iLevel; i++)
	{
		iParentLevelHW = (((iParentLevelHW%1)==1) ? (iParentLevelHW+1)/2 : (iParentLevelHW)/2);
	}
	return iParentLevelHW;
}
BOOL ImgRGBPyramid::SetPyramid(ImgRGB* imgRGBIn)
{
	this->imgRGB.Set(imgRGBIn->iWidth, imgRGBIn->iHeight*2,CHANNEL_3_8RGB);

	BYTE* byR;
	BYTE* byG;
	BYTE* byB;

	byR = this->imgRGB.byImgR;
	byG = this->imgRGB.byImgG;
	byB = this->imgRGB.byImgB;

	int iOffset = 0;
	int iParentLevelH=imgRGBIn->iHeight;
	int iParentLevelW=imgRGBIn->iWidth;

	if(imgRGBIn->iChannel==CHANNEL_3_8RGB)
	{
		for(int r=0; r<iParentLevelH; r++)
		{for(int c=0; c<iParentLevelW; c++)
		{
			byB[r*imgRGBIn->iWidth+c]=imgRGBIn->byImgB[r*imgRGBIn->iWidth+c];
			byG[r*imgRGBIn->iWidth+c]=imgRGBIn->byImgG[r*imgRGBIn->iWidth+c];
			byR[r*imgRGBIn->iWidth+c]=imgRGBIn->byImgR[r*imgRGBIn->iWidth+c];

		}
		}}
	else if(imgRGBIn->iChannel==CHANNEL_1_24BGR)
	{
		for(int r=0; r<iParentLevelH; r++)
		{for(int c=0; c<iParentLevelW; c++)
		{
			byB[r*imgRGBIn->iWidth+c]=imgRGBIn->byImg[3*(r*imgRGBIn->iWidth+c)+0];
			byG[r*imgRGBIn->iWidth+c]=imgRGBIn->byImg[3*(r*imgRGBIn->iWidth+c)+1];
			byR[r*imgRGBIn->iWidth+c]=imgRGBIn->byImg[3*(r*imgRGBIn->iWidth+c)+2];

		}
		}
	}
	int iLevel=0;
	int iLevelW;
	int iLevelH;
	while(1)
	{
		iLevel++;
		if(iParentLevelH<=1){break;}
		if(iParentLevelW<=1){break;}
		iLevelH = (((iParentLevelH%1)==1) ? (iParentLevelH+1)/2 : (iParentLevelH)/2);
		iLevelW = (((iParentLevelW%1)==1) ? (iParentLevelW+1)/2 : (iParentLevelW)/2);
		int iParentOffset=iOffset;
		iOffset=GetPointerOffset(imgRGBIn->iWidth, imgRGBIn->iHeight, iLevel);
		for(int r=0; r<iLevelH; r++)
		{
			if(((iParentLevelH%1)==1) && (r==iLevelH-1))
			{
				for(int c=0; c<iLevelW; c++)
				{
					if(((iParentLevelW%1)==1) && (c==iLevelW-1))
					{
						byB[r*iLevelW+c+iOffset]=((int)byB[2*r*iParentLevelW+2*c+iParentOffset]+(int)byB[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
						byB[r*iLevelW+c+iOffset]=((int)byG[2*r*iParentLevelW+2*c+iParentOffset]+(int)byG[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
						byB[r*iLevelW+c+iOffset]=((int)byR[2*r*iParentLevelW+2*c+iParentOffset]+(int)byR[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
					}
					else
					{
						byB[r*iLevelW+c+iOffset]=byB[2*r*iParentLevelW+2*c+iParentOffset];
						byG[r*iLevelW+c+iOffset]=byB[2*r*iParentLevelW+2*c+iParentOffset];
						byR[r*iLevelW+c+iOffset]=byB[2*r*iParentLevelW+2*c+iParentOffset];
					}
				}
			}
			else
			{
				for(int c=0; c<iLevelW; c++)
				{
					if(((iParentLevelW%1)==1) && (c==iLevelW-1))
					{
						byB[r*iLevelW+c+iOffset]=((int)byB[2*r*iParentLevelW+2*c+iParentOffset]+(int)byB[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
						byB[r*iLevelW+c+iOffset]=((int)byG[2*r*iParentLevelW+2*c+iParentOffset]+(int)byG[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
						byB[r*iLevelW+c+iOffset]=((int)byR[2*r*iParentLevelW+2*c+iParentOffset]+(int)byR[(2*r+1)*iParentLevelW+2*c+iParentOffset])/2;
					}
					else
					{
						byB[r*iLevelW+c+iOffset]=((int)byB[2*r*iParentLevelW+2*c+iParentOffset]+(int)byB[2*r*iParentLevelW+(2*c+1)+iParentOffset]+(int)byB[(2*r+1)*iParentLevelW+2*c+iParentOffset]+(int)byB[2*r*iParentLevelW+(2*c+1)+iParentOffset])/4;
						byG[r*iLevelW+c+iOffset]=((int)byG[2*r*iParentLevelW+2*c+iParentOffset]+(int)byG[2*r*iParentLevelW+(2*c+1)+iParentOffset]+(int)byG[(2*r+1)*iParentLevelW+2*c+iParentOffset]+(int)byG[2*r*iParentLevelW+(2*c+1)+iParentOffset])/4;
						byR[r*iLevelW+c+iOffset]=((int)byR[2*r*iParentLevelW+2*c+iParentOffset]+(int)byR[2*r*iParentLevelW+(2*c+1)+iParentOffset]+(int)byR[(2*r+1)*iParentLevelW+2*c+iParentOffset]+(int)byR[2*r*iParentLevelW+(2*c+1)+iParentOffset])/4;
					}
				}
			}
		}
		iParentLevelH=iLevelH;
		iParentLevelW=iLevelW;
	}
	return TRUE;
}


inline BOOL UpdateSumRDirection
	(
	BYTE* byImage, int iImgWidth, int iImgHeight,
	int r, 
	int iHalfHeight,
	UINT* uiSumOfEachC, int* iTotalHeight)
{
	if(byImage == NULL){return FALSE;}
	if(uiSumOfEachC == NULL){return FALSE;}
	if(iTotalHeight == NULL){return FALSE;}

	if(r==0)
	{

		for(int c=0; c<iImgWidth; c++)
		{
			uiSumOfEachC[c]=0;
			for(int rLocal=0; rLocal<=iHalfHeight; rLocal++)
			{
				uiSumOfEachC[c]+=byImage[(r+rLocal)*iImgWidth+c];
			}
		}

		*iTotalHeight=iHalfHeight+1;
		return TRUE;
	}

	if(r<iHalfHeight+1)
	{
		for(int c=0; c<iImgWidth; c++)
		{
			uiSumOfEachC[c]+=byImage[(r+iHalfHeight)*iImgWidth+c];
		}
		*iTotalHeight=iHalfHeight+iHalfHeight+1;

		return TRUE;
	}
	if(r<iImgHeight-iHalfHeight)
	{
		for(int c=0; c<iImgWidth; c++)
		{
			uiSumOfEachC[c]-=byImage[(r-iHalfHeight-1)*iImgWidth+c];
			uiSumOfEachC[c]+=byImage[(r+iHalfHeight)*iImgWidth+c];
		}
		*iTotalHeight=iHalfHeight+iHalfHeight+1;

		return TRUE;
	}

	for(int c=0; c<iImgWidth; c++)
	{
		uiSumOfEachC[c] -= byImage[(r-iHalfHeight-1)*iImgWidth+c];
	}
	*iTotalHeight=iHalfHeight+iHalfHeight+1;
	return TRUE;
}

#define MAX(a,b) ((a>=b)?(a):(b))
#define MIN(a,b) ((a<=b)?(a):(b))
inline BOOL UpdateMaxRDirection
	(
	BYTE* byImage, int iImgWidth, int iImgHeight,
	int r, 
	int iHalfHeight,
	BYTE* byMaxOfEachC)
{
	if(byImage == NULL){return FALSE;}
	if(byMaxOfEachC == NULL){return FALSE;}

	if(r==0)
	{
		for(int c=0; c<iImgWidth; c++)
		{
			byMaxOfEachC[c]=byImage[c];
			for(int rLocal=1; rLocal<=iHalfHeight; rLocal++)
			{
				byMaxOfEachC[c]=MAX(byMaxOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	if(r<iHalfHeight+1)//r = 1, 2, 3
	{
		for(int c=0; c<iImgWidth; c++)
		{
			byMaxOfEachC[c]=MAX(byMaxOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
		}
		return TRUE;
	}

	if(r<iImgHeight-iHalfHeight)//r = 4, 5, 6, ..., 15, 16
	{
		for(int c=0; c<iImgWidth; c++)
		{
			if(byMaxOfEachC[c] > byImage[(r-iHalfHeight-1)*iImgWidth+c])
			{
				byMaxOfEachC[c]=MAX(byMaxOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
				continue;
			}

			if(byImage[(r-iHalfHeight-1)*iImgWidth+c] <= byImage[(r+iHalfHeight)*iImgWidth+c])
			{
				byMaxOfEachC[c]=byImage[(r+iHalfHeight)*iImgWidth+c];
				continue;
			}

			byMaxOfEachC[c]=byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rLocal=-iHalfHeight+1; rLocal<=iHalfHeight; rLocal++)
			{
				byMaxOfEachC[c]=MAX(byMaxOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	for(int c=0; c<iImgWidth; c++)//r = 17, 18, 19
	{
		if(byMaxOfEachC[c] <= byImage[(r-iHalfHeight-1)*iImgWidth+c])//r-iHalfHeight-1 = 13, 14, 15
		{
			byMaxOfEachC[c]=byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rLocal=-iHalfHeight+1; rLocal< iImgHeight-r; rLocal++)
			{
				byMaxOfEachC[c]=MAX(byMaxOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
	}
	return TRUE;
}
inline BOOL UpdateMinRDirection
	(
	BYTE* byImage, int iImgWidth, int iImgHeight,
	int r, 
	int iHalfHeight,
	BYTE* byMinOfEachC)
{
	if(byImage == NULL){return FALSE;}
	if(byMinOfEachC == NULL){return FALSE;}

	if(r==0)
	{

		for(int c=0; c<iImgWidth; c++)
		{
			byMinOfEachC[c]=255;
			for(int rLocal=0; rLocal<=iHalfHeight; rLocal++)
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	if(r<iHalfHeight+1)
	{
		for(int c=0; c<iImgWidth; c++)
		{
			byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
		}
		return TRUE;
	}

	if(r<iImgHeight-iHalfHeight)
	{
		for(int c=0; c<iImgWidth; c++)
		{
			
			if(byMinOfEachC[c] < byImage[(r-iHalfHeight-1)*iImgWidth+c])
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
			}
			else
			{
				if(byImage[(r-iHalfHeight-1)*iImgWidth+c] >= byImage[(r+iHalfHeight)*iImgWidth+c])
				{
					byMinOfEachC[c]=byImage[(r+iHalfHeight)*iImgWidth+c];
				}
				else
				{
				
					byMinOfEachC[c]=255;
					for(int rLocal=-iHalfHeight; rLocal<=iHalfHeight; rLocal++)
					{
						byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
					}
				}
			}
		}
		return TRUE;
	}

	for(int c=0; c<iImgWidth; c++)
	{
		if(byMinOfEachC[c] > byImage[(r-iHalfHeight-1)*iImgWidth+c])
		{
			byMinOfEachC[c]=255;
			for(int rLocal=-iHalfHeight; rLocal< iImgHeight-r; rLocal++)
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
	}
	return TRUE;
}

BOOL SumCDirection
	(
	UINT* uiSumOfEachC,
	int iImgWidth,
	int iHalfWidth,
	UINT* uiSumOfRC, int* iTotalCs)
{
	uiSumOfRC[0]=0;
	for(int c=0; c<iHalfWidth; c++)
	{
		uiSumOfRC[0]+=uiSumOfEachC[c];
	}
	iTotalCs[0]=iHalfWidth+1;

	for(int c=1; c<=iHalfWidth; c++)
	{
		uiSumOfRC[c]=uiSumOfRC[c-1]+uiSumOfEachC[c+iHalfWidth];
		iTotalCs[c]=iTotalCs[c-1]+1;
	}

	for(int c=iHalfWidth+1; c<iImgWidth-iHalfWidth; c++)
	{
		uiSumOfRC[c]=uiSumOfRC[c-1]-uiSumOfEachC[c-iHalfWidth];
		uiSumOfRC[c]+=uiSumOfEachC[c+iHalfWidth];
		iTotalCs[c]=2*iHalfWidth+1;
	}

	for(int c=iImgWidth-iHalfWidth+1; c+iHalfWidth<iImgWidth; c++)
	{
		uiSumOfRC[c]=uiSumOfRC[c]-uiSumOfEachC[c+iHalfWidth];
		iTotalCs[c]=iTotalCs[c-1]-1;
	}
	return TRUE;
}

BOOL MaxCDirection
	(
	BYTE* byMaxOfEachC,
	int iImgWidth,
	int iHalfWidth,
	BYTE* byMaxOfRC)
{
	for(int i=0; i<iImgWidth; i++){byMaxOfRC[i]=0;}
	byMaxOfRC[0]=byMaxOfEachC[0];
	for(int c=1; c<=iHalfWidth; c++)
	{
		byMaxOfRC[0]=MAX(byMaxOfRC[0], byMaxOfEachC[c]);
	}

	for(int c=1; c<=iHalfWidth; c++)
	{
		byMaxOfRC[c]=MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
	}

	for(int c=iHalfWidth+1; c<iImgWidth-iHalfWidth; c++)
	{

		if(byMaxOfRC[c-1]>byMaxOfEachC[c-iHalfWidth-1])
		{
			byMaxOfRC[c]= MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
			continue;
		}
		if(byMaxOfEachC[c-iHalfWidth-1] < byMaxOfEachC[c+iHalfWidth])
		{
			byMaxOfRC[c]=MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
			continue;
		}

		byMaxOfRC[c]=byMaxOfEachC[c-iHalfWidth];
		for(int cLocal=-iHalfWidth+1; cLocal<= iHalfWidth; cLocal++)
		{
			byMaxOfRC[c]=MAX(byMaxOfRC[c], byMaxOfEachC[c+cLocal]);
		}
	}

	for(int c=iImgWidth-iHalfWidth; c<iImgWidth; c++)
	{
		if(byMaxOfRC[c-1]>byMaxOfEachC[c-iHalfWidth-1])
		{
			byMaxOfRC[c]=byMaxOfRC[c-1];
			continue;
		}

		byMaxOfRC[c]=byMaxOfEachC[c-iHalfWidth];
		for(int cLocal=-iHalfWidth+1; cLocal< iImgWidth-c; cLocal++)
		{
			byMaxOfRC[c]=MAX(byMaxOfRC[c], byMaxOfEachC[c+cLocal]);
		}
	}
	return TRUE;
}

BOOL MinCDirection
	(
	BYTE* byMinOfEachC,
	int iImgWidth,
	int iHalfWidth,
	BYTE* byMinOfRC)
{
	byMinOfRC[0]=0;
	for(int c=0; c<iHalfWidth; c++)
	{
		byMinOfRC[0]=MIN(byMinOfRC[0], byMinOfEachC[c]);
	}

	for(int c=1; c<=iHalfWidth; c++)
	{
		byMinOfRC[c]=MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);
	}

	for(int c=iHalfWidth+1; c<iImgWidth-iHalfWidth; c++)
	{

		if(byMinOfRC[c-1]<byMinOfEachC[c-iHalfWidth-1])
		{
			byMinOfRC[c]= MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);
		}
		else
		{
			if(byMinOfEachC[c-iHalfWidth-1] > byMinOfEachC[c+iHalfWidth])
			{
				byMinOfRC[c]=MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);

			}
			else
			{

				byMinOfRC[c]=255;
				for(int cLocal=-iHalfWidth; cLocal<= iHalfWidth; cLocal++)
				{
					byMinOfRC[c]=MIN(byMinOfRC[c], byMinOfEachC[c+cLocal]);
				}
			}
		}
	}

	for(int c=iImgWidth-iHalfWidth+1; c+iHalfWidth<iImgWidth; c++)
	{
		if(byMinOfRC[c-1]>byMinOfEachC[c-iHalfWidth-1])
		{
			byMinOfRC[c]=255;
			for(int cLocal=-iHalfWidth; cLocal< iHalfWidth-c; cLocal++)
			{
				byMinOfRC[c]=MIN(byMinOfRC[c], byMinOfEachC[c+cLocal]);
			}
		}

	}
	return TRUE;
}

BOOL DLL_IE MeanImage(ImgRGB* imgIn, ImgRGB* imgResult, int iFilterWidth, int iFilterHeight)
{
	int iImgWidth=imgIn->iWidth;
	int iImgHeight=imgIn->iHeight;
	if((imgIn->iChannel == CHANNEL_1_24BGR) || (imgIn->iChannel == CHANNEL_3_8RGB))
	{
		UINT* uiFilteredOfEachC_R;
		UINT* uiFilteredOfEachC_G;
		UINT* uiFilteredOfEachC_B;

		uiFilteredOfEachC_R=new UINT[iImgWidth];
		uiFilteredOfEachC_G=new UINT[iImgWidth];
		uiFilteredOfEachC_B=new UINT[iImgWidth];

		UINT* uiFilteredOfRC_R;
		UINT* uiFilteredOfRC_G;
		UINT* uiFilteredOfRC_B;

		uiFilteredOfRC_R=new UINT[iImgWidth];
		uiFilteredOfRC_G=new UINT[iImgWidth];
		uiFilteredOfRC_B=new UINT[iImgWidth];

		int iTotalHeight_R;
		int iTotalHeight_G;
		int iTotalHeight_B;
		int* iTotalCs_R;
		int* iTotalCs_G;
		int* iTotalCs_B;
		iTotalCs_R=new int [iImgWidth];
		iTotalCs_G=new int [iImgWidth];
		iTotalCs_B=new int [iImgWidth];
		ImgRGB imgR1, imgG1, imgB1;
		ImgRGB imgR2, imgG2, imgB2;


		Decompose3(imgIn,&imgR1,&imgG1,&imgB1);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgG2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			UpdateSumRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_R, &iTotalHeight_R);
			UpdateSumRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_G, &iTotalHeight_G);
			UpdateSumRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_B, &iTotalHeight_B);

			SumCDirection(uiFilteredOfEachC_R, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_R, iTotalCs_R);
			SumCDirection(uiFilteredOfEachC_G, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_G, iTotalCs_G);
			SumCDirection(uiFilteredOfEachC_B, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_B, iTotalCs_B);
			for(int c=0; c<iImgWidth; c++)
			{
				imgR2.byImg[r*iImgWidth+c]=BYTE(uiFilteredOfRC_R[c]/(iTotalCs_R[c]*iTotalHeight_R*1.0));
				imgG2.byImg[r*iImgWidth+c]=BYTE(uiFilteredOfRC_G[c]/(iTotalCs_G[c]*iTotalHeight_G*1.0));
				imgB2.byImg[r*iImgWidth+c]=BYTE(uiFilteredOfRC_B[c]/(iTotalCs_B[c]*iTotalHeight_B*1.0));
			}
		}
		Compose3(imgResult,&imgR2,&imgG2,&imgB2);
		SAFE_DELETE( uiFilteredOfRC_R);
		SAFE_DELETE( uiFilteredOfRC_G);
		SAFE_DELETE( uiFilteredOfRC_B);

		SAFE_DELETE( iTotalCs_R);
		SAFE_DELETE( iTotalCs_G);
		SAFE_DELETE( iTotalCs_B);

		SAFE_DELETE( uiFilteredOfEachC_R);
		SAFE_DELETE( uiFilteredOfEachC_G);
		SAFE_DELETE( uiFilteredOfEachC_B);
		return TRUE;
	}

	if(imgIn->iChannel == CHANNEL_1_8)
	{
		UINT* uiFilteredOfEachC;

		uiFilteredOfEachC=new UINT[iImgWidth];;

		UINT* uiFilteredOfRC;

		uiFilteredOfRC=new UINT[iImgWidth];

		int iTotalHeight;
		int* iTotalCs;
		iTotalCs=new int [iImgWidth];
		imgResult->Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			UpdateSumRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC, &iTotalHeight);

			SumCDirection(uiFilteredOfEachC, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC, iTotalCs);
			for(int c=0; c<iImgWidth; c++)
			{
				imgResult->byImg[r*iImgWidth+c]=BYTE(uiFilteredOfRC[c]/(iTotalCs[c]*iTotalHeight*1.0));
			}
		}
		SAFE_DELETE( uiFilteredOfRC);
		SAFE_DELETE( iTotalCs);
		SAFE_DELETE( uiFilteredOfEachC);
		return TRUE;
	}

	return FALSE;
}

BOOL DLL_IE MaxImage(ImgRGB* imgIn, ImgRGB* imgResult, int iFilterWidth, int iFilterHeight)
{
	int iImgWidth=imgIn->iWidth;
	int iImgHeight=imgIn->iHeight;
	if((imgIn->iChannel == CHANNEL_1_24BGR) || (imgIn->iChannel == CHANNEL_3_8RGB))
	{
		BYTE* uiFilteredOfEachC_R;
		BYTE* uiFilteredOfEachC_G;
		BYTE* uiFilteredOfEachC_B;

		uiFilteredOfEachC_R=new BYTE[iImgWidth];
		uiFilteredOfEachC_G=new BYTE[iImgWidth];
		uiFilteredOfEachC_B=new BYTE[iImgWidth];

		BYTE* uiFilteredOfRC_R;
		BYTE* uiFilteredOfRC_G;
		BYTE* uiFilteredOfRC_B;

		uiFilteredOfRC_R=new BYTE[iImgWidth];
		uiFilteredOfRC_G=new BYTE[iImgWidth];
		uiFilteredOfRC_B=new BYTE[iImgWidth];

		ImgRGB imgR1, imgG1, imgB1;
		ImgRGB imgR2, imgG2, imgB2;


		Decompose3(imgIn,&imgR1,&imgG1,&imgB1);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgG2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			UpdateMaxRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_R);
			UpdateMaxRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_G);
			UpdateMaxRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_B);

			MaxCDirection(uiFilteredOfEachC_R, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_R);
			MaxCDirection(uiFilteredOfEachC_G, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_G);
			MaxCDirection(uiFilteredOfEachC_B, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_B);
			for(int c=0; c<iImgWidth; c++)
			{
				imgR2.byImg[r*iImgWidth+c]=uiFilteredOfRC_R[c];
				imgG2.byImg[r*iImgWidth+c]=uiFilteredOfRC_G[c];
				imgB2.byImg[r*iImgWidth+c]=uiFilteredOfRC_B[c];
			}
		}
		Compose3(imgResult,&imgR2,&imgG2,&imgB2);
		SAFE_DELETE( uiFilteredOfEachC_R);
		SAFE_DELETE( uiFilteredOfEachC_G);
		SAFE_DELETE( uiFilteredOfEachC_B);

		SAFE_DELETE( uiFilteredOfRC_R);
		SAFE_DELETE( uiFilteredOfRC_G);
		SAFE_DELETE( uiFilteredOfRC_B);

		return TRUE;
	}

	if(imgIn->iChannel == CHANNEL_1_8)
	{
		BYTE* uiFilteredOfEachC;

		uiFilteredOfEachC=new BYTE[iImgWidth];;

		BYTE* uiFilteredOfRC;

		uiFilteredOfRC=new BYTE[iImgWidth];

		int iTotalHeight;
		int* iTotalCs;
		iTotalCs=new int [iImgWidth];
		imgResult->Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			if(r==8)
			{
				r=r;
			}
			UpdateMaxRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC);

			MaxCDirection(uiFilteredOfEachC, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC);
			for(int c=0; c<iImgWidth; c++)
			{
				imgResult->byImg[r*iImgWidth+c]=uiFilteredOfRC[c];
			}
		}
		SAFE_DELETE( uiFilteredOfRC);
		SAFE_DELETE( iTotalCs);
		SAFE_DELETE( uiFilteredOfEachC);
		return TRUE;
	}

	return FALSE;
}	

BOOL DLL_IE MinImage(ImgRGB* imgIn, ImgRGB* imgResult, int iFilterWidth, int iFilterHeight)
{
	int iImgWidth=imgIn->iWidth;
	int iImgHeight=imgIn->iHeight;
	if((imgIn->iChannel == CHANNEL_1_24BGR) || (imgIn->iChannel == CHANNEL_3_8RGB))
	{
		BYTE* uiFilteredOfEachC_R;
		BYTE* uiFilteredOfEachC_G;
		BYTE* uiFilteredOfEachC_B;

		uiFilteredOfEachC_R=new BYTE[iImgWidth];
		uiFilteredOfEachC_G=new BYTE[iImgWidth];
		uiFilteredOfEachC_B=new BYTE[iImgWidth];

		BYTE* uiFilteredOfRC_R;
		BYTE* uiFilteredOfRC_G;
		BYTE* uiFilteredOfRC_B;

		uiFilteredOfRC_R=new BYTE[iImgWidth];
		uiFilteredOfRC_G=new BYTE[iImgWidth];
		uiFilteredOfRC_B=new BYTE[iImgWidth];

		ImgRGB imgR1, imgG1, imgB1;
		ImgRGB imgR2, imgG2, imgB2;


		Decompose3(imgIn,&imgR1,&imgG1,&imgB1);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgG2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		imgR2.Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			UpdateMinRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_R);
			UpdateMinRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_G);
			UpdateMinRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC_B);

			MinCDirection(uiFilteredOfEachC_R, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_R);
			MinCDirection(uiFilteredOfEachC_G, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_G);
			MinCDirection(uiFilteredOfEachC_B, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC_B);
			for(int c=0; c<iImgWidth; c++)
			{
				imgR2.byImg[r*iImgWidth+c]=uiFilteredOfRC_R[c];
				imgG2.byImg[r*iImgWidth+c]=uiFilteredOfRC_G[c];
				imgB2.byImg[r*iImgWidth+c]=uiFilteredOfRC_B[c];
			}
		}
		Compose3(imgResult,&imgR2,&imgG2,&imgB2);
		SAFE_DELETE( uiFilteredOfEachC_R);
		SAFE_DELETE( uiFilteredOfEachC_G);
		SAFE_DELETE( uiFilteredOfEachC_B);

		SAFE_DELETE( uiFilteredOfRC_R);
		SAFE_DELETE( uiFilteredOfRC_G);
		SAFE_DELETE( uiFilteredOfRC_B);

		return TRUE;
	}

	if(imgIn->iChannel == CHANNEL_1_8)
	{
		BYTE* uiFilteredOfEachC;

		uiFilteredOfEachC=new BYTE[iImgWidth];;

		BYTE* uiFilteredOfRC;

		uiFilteredOfRC=new BYTE[iImgWidth];

		int iTotalHeight;
		int* iTotalCs;
		iTotalCs=new int [iImgWidth];
		imgResult->Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		for(int r=0; r<iImgHeight; r++)
		{
			UpdateMinRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, (iFilterHeight-1)/2, uiFilteredOfEachC);

			MinCDirection(uiFilteredOfEachC, iImgWidth, (iFilterWidth-1)/2, uiFilteredOfRC);
			for(int c=0; c<iImgWidth; c++)
			{
				imgResult->byImg[r*iImgWidth+c]=uiFilteredOfRC[c];
			}
		}
		SAFE_DELETE( uiFilteredOfRC);
		SAFE_DELETE( iTotalCs);
		SAFE_DELETE( uiFilteredOfEachC);
		return TRUE;
	}

	return FALSE;
}
BOOL DLL_IE SubImage(ImgRGB* Image1, ImgRGB* Image2, ImgRGB* ImageResult, double dMult, double dAdd)
{
	if(Image1->iWidth != Image2->iWidth){return FALSE;}
	if(Image1->iHeight != Image2->iHeight){return FALSE;}

	int iImgHeight = Image1->iHeight;
	int iImgWidth = Image1->iWidth;
	if(Image1->iChannel == CHANNEL_1_8)
	{
		if(Image1->iChannel == CHANNEL_1_8)
		{
			ImageResult->Set(Image1->iWidth, Image1->iHeight, CHANNEL_1_8);

			for(int r=0; r<iImgHeight; r++)
			{
				for(int c=0; c<iImgWidth; c++)
				{
					int iSub=int(Image1->byImg[r*iImgWidth+c])-int(Image1->byImg[r*iImgWidth+c]);
					double dResult=iSub*dMult+dAdd;
					ImageResult->byImg[r*iImgWidth+c]=BYTE(MAX(MIN(dResult,255),0));
				}
			}
		return TRUE;
		}
		return FALSE;
	}

	if(Image1->iChannel == CHANNEL_3_8RGB)
	{
		if(Image1->iChannel == CHANNEL_3_8RGB)
		{
			ImageResult->Set(Image1->iWidth, Image1->iHeight, CHANNEL_3_8RGB);

			for(int r=0; r<iImgHeight; r++)
			{
				for(int c=0; c<iImgWidth; c++)
				{
					int iSub;
					double dResult;
					iSub=int(Image1->byImgR[r*iImgWidth+c])-int(Image1->byImgR[r*iImgWidth+c]);
					dResult=iSub*dMult+dAdd;
					ImageResult->byImgR[r*iImgWidth+c]=BYTE(MAX(MIN(dResult,255),0));
					
					iSub=int(Image1->byImgG[r*iImgWidth+c])-int(Image1->byImgG[r*iImgWidth+c]);
					dResult=iSub*dMult+dAdd;
					ImageResult->byImgG[r*iImgWidth+c]=BYTE(MAX(MIN(dResult,255),0));
					
					iSub=int(Image1->byImgB[r*iImgWidth+c])-int(Image1->byImgB[r*iImgWidth+c]);
					dResult=iSub*dMult+dAdd;
					ImageResult->byImgB[r*iImgWidth+c]=BYTE(MAX(MIN(dResult,255),0));
				}
			}
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}



/*
inline BOOL SumRDirection
	(
	BYTE* byTargetCropped, int iImgTargetCroppedW,
	BYTE* byModel, int iModelW, int iModelH, 
	UINT* uiMap, int iMapW, int iMapR, int iMapC, 
	int c, 
	BYTE* byMinOfEachCTarget, 
	BYTE* byMaxOfEachCTarget, 
	UINT* uiSumOfEachCTarget,
	BYTE* byMinOfEachCModel, 
	BYTE* byMaxOfEachCModel,
	UINT* uiSumOfEachCModel)
{
	if(byMaxOfEachCTarget[c] < byMinOfEachCModel[c])
	{
		uiMap[iMapR*iMapW+iMapC]+=uiSumOfEachCModel[c] - uiSumOfEachCTarget[c];
		return TRUE;
	}
	if(byMinOfEachCTarget[c] > byMaxOfEachCModel[c])
	{
		uiMap[iMapR*iMapW+iMapC]+=uiSumOfEachCTarget[c] - uiSumOfEachCModel[c];
		return TRUE;
	}

	for(int r=0; r<iModelH; r++)
	{
		uiMap[iMapR*iMapW+iMapC]+=bySubAbs(byTargetCropped[r*iImgTargetCroppedW+c+iMapC] , (byModel[r*iModelW+c]));
	}

	return TRUE;
}

BOOL UpdateVerticalInformation(BYTE* byDataR, BYTE* byDataG, BYTE* byDataB, int r, BYTE* byMinOfEachCTargetR, BYTE* byMaxOfEachCTargetR, UINT* uiSumOfEachCTargetR, BYTE* byMinOfEachCTargetG, BYTE* byMaxOfEachCTargetG, UINT* uiSumOfEachCTargetG,
	BYTE* byMinOfEachCTargetB, BYTE* byMaxOfEachCTargetB, UINT* uiSumOfEachCTargetB, int iCropW, int iModelHeight)
{
	for(int c=0; c<iCropW; c++)
	{
		uiSumOfEachCTargetR[c]+=byDataR[iModelHeight*iCropW+c];
		uiSumOfEachCTargetG[c]+=byDataG[r*iCropW+c];
		uiSumOfEachCTargetB[c]+=byDataB[r*iCropW+c];
	}
	for(int c=0; c<iCropW; c++)
	{
	for(int r=0; r<iModelHeight; r++)
	{
		if(byMinOfEachCTargetR[c]<byDataR[r*iCropW+c]){byMinOfEachCTargetR[c]=byDataR[r*iCropW+c];}
		if(byMinOfEachCTargetG[c]<byDataG[r*iCropW+c]){byMinOfEachCTargetG[c]=byDataG[r*iCropW+c];}
		if(byMinOfEachCTargetB[c]<byDataB[r*iCropW+c]){byMinOfEachCTargetB[c]=byDataB[r*iCropW+c];}

		if(byMaxOfEachCTargetR[c]>byDataR[r*iCropW+c]){byMaxOfEachCTargetR[c]=byDataR[r*iCropW+c];}
		if(byMaxOfEachCTargetG[c]>byDataG[r*iCropW+c]){byMaxOfEachCTargetG[c]=byDataG[r*iCropW+c];}
		if(byMaxOfEachCTargetB[c]>byDataB[r*iCropW+c]){byMaxOfEachCTargetB[c]=byDataB[r*iCropW+c];}

	}

	}
}
return TRUE;
}

BOOL FindModelFast(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC)
{
int iModelHeight;
int iModelWidth;

if(imgTarget == NULL){return FALSE;}
if(imgModel == NULL){return FALSE;}


iModelHeight = imgModel->iHeight;
iModelWidth = imgModel->iWidth;

UINT* uiMap;
int iMapW;
int iMapH;
iMapW=(iC1-iC0+1)-imgModel->iWidth+1;
iMapH=(iR1-iR0+1)-imgModel->iHeight+1;
if(iMapW<=0){return FALSE;}
if(iMapH<=0){return FALSE;}
uiMap = new UINT[iMapW*iMapH];
int iR1Local;
int iC1Local;

iR1Local = iR1;
iC1Local = iC1;

if(iR1Local>=imgTarget->iHeight){iR1Local = imgTarget->iHeight-1;}
if(iC1Local>=imgTarget->iWidth){iC1Local = imgTarget->iWidth-1;}
int iCropH;
int iCropW;
iCropH=iR1Local-iR0+1;
iCropW=iC1Local-iC0+1;
int iScanHeight;
int iScanWidth;
iScanHeight = iR1Local-iR0-iModelHeight + 2;
iScanWidth = iC1Local-iC0-iModelWidth + 2;

if(iScanHeight<=0){return FALSE;}
if(iScanWidth<=0){return FALSE;}

BOOL bFound;
int iREnd, iCEnd;

iREnd = iR0+iScanHeight;
iCEnd = iC0+iScanWidth;

int iPtrTarget;
int iPtrModel;

memset(uiMap,0,iMapW*iMapH);
BYTE* byDataR;
BYTE* byDataG;
BYTE* byDataB;

BYTE* byMaxOfEachCModelR;
BYTE* byMinOfEachCModelR;
UINT* uiSumOfEachCModelR;
BYTE* byMaxOfEachCModelG;
BYTE* byMinOfEachCModelG;
UINT* uiSumOfEachCModelG;
BYTE* byMaxOfEachCModelB;
BYTE* byMinOfEachCModelB;
UINT* uiSumOfEachCModelB;

BYTE* byMaxOfEachCTargetR;
BYTE* byMinOfEachCTargetR;
UINT* uiSumOfEachCTargetR;
BYTE* byMaxOfEachCTargetG;
BYTE* byMinOfEachCTargetG;
UINT* uiSumOfEachCTargetG;
BYTE* byMaxOfEachCTargetB;
BYTE* byMinOfEachCTargetB;
UINT* uiSumOfEachCTargetB;

byDataR = new BYTE[iCropH*iCropW];
byDataG = new BYTE[iCropH*iCropW];
byDataB = new BYTE[iCropH*iCropW];

byMaxOfEachCModelR = new BYTE[iModelWidth];
byMinOfEachCModelR = new BYTE[iModelWidth];
uiSumOfEachCModelR = new UINT[iModelWidth];
byMaxOfEachCModelG = new BYTE[iModelWidth];
byMinOfEachCModelG = new BYTE[iModelWidth];
uiSumOfEachCModelG = new UINT[iModelWidth];
byMaxOfEachCModelB = new BYTE[iModelWidth];
byMinOfEachCModelB = new BYTE[iModelWidth];
uiSumOfEachCModelB = new UINT[iModelWidth];

byMaxOfEachCTargetR = new BYTE[iCropW];
byMinOfEachCTargetR = new BYTE[iCropW];
uiSumOfEachCTargetR = new UINT[iCropW];
byMaxOfEachCTargetG = new BYTE[iCropW];
byMinOfEachCTargetG = new BYTE[iCropW];
uiSumOfEachCTargetG = new UINT[iCropW];
byMaxOfEachCTargetB = new BYTE[iCropW];
byMinOfEachCTargetB = new BYTE[iCropW];
uiSumOfEachCTargetB = new UINT[iCropW];

for(int r=0;r<iCropH; r++)
{
int iTargetR=iR0+r;
for(int c=0; c<iCropW; c++)
{
int iTargetC=iC0+c;
byDataR[r*iCropW+c] = imgTarget->byImg[3*(iTargetR*imgTarget->iWidth+(iTargetC))+2];
byDataG[r*iCropW+c] = imgTarget->byImg[3*(iTargetR*imgTarget->iWidth+(iTargetC))+1];
byDataB[r*iCropW+c] = imgTarget->byImg[3*(iTargetR*imgTarget->iWidth+(iTargetC))+0];

}
}

if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_3_8RGB))
{
for(int c=0; c<iModelWidth; c++)
{
byMinOfEachCModelR[c]=255;
byMaxOfEachCModelR[c]=0;
uiSumOfEachCModelR[c]=0;
byMinOfEachCModelG[c]=255;
byMaxOfEachCModelG[c]=0;
uiSumOfEachCModelG[c]=0;
byMinOfEachCModelB[c]=255;
byMaxOfEachCModelB[c]=0;
uiSumOfEachCModelB[c]=0;
for(int r=0; r<iModelHeight; r++)
{
if(byMinOfEachCModelR[c]<imgModel->byImgR[r*iModelWidth+c]){byMinOfEachCModelR[c]=imgModel->byImgR[r*iModelWidth+c];}
if(byMinOfEachCModelG[c]<imgModel->byImgG[r*iModelWidth+c]){byMinOfEachCModelG[c]=imgModel->byImgG[r*iModelWidth+c];}
if(byMinOfEachCModelB[c]<imgModel->byImgB[r*iModelWidth+c]){byMinOfEachCModelB[c]=imgModel->byImgB[r*iModelWidth+c];}

if(byMaxOfEachCModelR[c]>imgModel->byImgR[r*iModelWidth+c]){byMaxOfEachCModelR[c]=imgModel->byImgR[r*iModelWidth+c];}
if(byMaxOfEachCModelG[c]>imgModel->byImgG[r*iModelWidth+c]){byMaxOfEachCModelG[c]=imgModel->byImgG[r*iModelWidth+c];}
if(byMaxOfEachCModelB[c]>imgModel->byImgB[r*iModelWidth+c]){byMaxOfEachCModelB[c]=imgModel->byImgB[r*iModelWidth+c];}

uiSumOfEachCModelR[c]+=imgModel->byImgR[r*iModelWidth+c];
uiSumOfEachCModelG[c]+=imgModel->byImgG[r*iModelWidth+c];
uiSumOfEachCModelB[c]+=imgModel->byImgB[r*iModelWidth+c];
}
}

for(int c=0; c<iCropW; c++)
{
byMinOfEachCTargetR[c]=255;
byMaxOfEachCTargetR[c]=0;
uiSumOfEachCTargetR[c]=0;
byMinOfEachCTargetG[c]=255;
byMaxOfEachCTargetG[c]=0;
uiSumOfEachCTargetG[c]=0;
byMinOfEachCTargetB[c]=255;
byMaxOfEachCTargetB[c]=0;
uiSumOfEachCTargetB[c]=0;
for(int r=0; r<iModelHeight; r++)
{
if(byMinOfEachCTargetR[c]<byDataR[r*iCropW+c]){byMinOfEachCTargetR[c]=byDataR[r*iCropW+c];}
if(byMinOfEachCTargetG[c]<byDataG[r*iCropW+c]){byMinOfEachCTargetG[c]=byDataG[r*iCropW+c];}
if(byMinOfEachCTargetB[c]<byDataB[r*iCropW+c]){byMinOfEachCTargetB[c]=byDataB[r*iCropW+c];}

if(byMaxOfEachCTargetR[c]>byDataR[r*iCropW+c]){byMaxOfEachCTargetR[c]=byDataR[r*iCropW+c];}
if(byMaxOfEachCTargetG[c]>byDataG[r*iCropW+c]){byMaxOfEachCTargetG[c]=byDataG[r*iCropW+c];}
if(byMaxOfEachCTargetB[c]>byDataB[r*iCropW+c]){byMaxOfEachCTargetB[c]=byDataB[r*iCropW+c];}

uiSumOfEachCTargetR[c]+=byDataR[r*iCropW+c];
uiSumOfEachCTargetG[c]+=byDataG[r*iCropW+c];
uiSumOfEachCTargetB[c]+=byDataB[r*iCropW+c];
}
}

int iMapR=0;
for(int iMapC=0; iMapC<iMapW; iMapC++)
{
for(int c=0; c<iModelWidth; c++)
{
SumRDirection(
byDataR, iCropW,  
imgModel->byImgR, imgModel->iWidth, imgModel->iHeight,
uiMap, iMapW, iMapR, iMapC,
c,
byMinOfEachCTargetR, 
byMaxOfEachCTargetR, 
uiSumOfEachCTargetR,
byMinOfEachCModelR, 
byMaxOfEachCModelR,
uiSumOfEachCModelR);

SumRDirection(
byDataG, iCropW,  
imgModel->byImgG, imgModel->iWidth, imgModel->iHeight,
uiMap, iMapW, iMapR, iMapC,
c,
byMinOfEachCTargetG, 
byMaxOfEachCTargetG, 
uiSumOfEachCTargetG,
byMinOfEachCModelG, 
byMaxOfEachCModelG,
uiSumOfEachCModelG);

SumRDirection(
byDataB, iCropW,  
imgModel->byImgB, imgModel->iWidth, imgModel->iHeight,
uiMap, iMapW, iMapR, iMapC,
c,
byMinOfEachCTargetB, 
byMaxOfEachCTargetB, 
uiSumOfEachCTargetB,
byMinOfEachCModelB, 
byMaxOfEachCModelB,
uiSumOfEachCModelB);
}
}


for(int iMapR=0; iMapR<iMapH; iMapR++)
{
for(int iMapC=0; iMapC<iMapW; iMapC++)
{

int iTargetR=iR0+iMapR;
int iTargetC=iC0+iMapC;
for(int r=0; r<iModelHeight; r++)
{
for(int c=0; c<iModelWidth; c++)
{
if(iTargetR + r>=imgTarget->iHeight){uiMap[iMapR*iMapW+iMapC]+=255*3;break;}
if(iTargetC + c>=imgTarget->iWidth){uiMap[iMapR*iMapW+iMapC]+=255*3;break;}
iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
iPtrModel = (r)*imgModel->iWidth+(c);

uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTarget->byImgR[iPtrTarget + 0] , (imgModel->byImgB[iPtrModel]));
uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTarget->byImgR[iPtrTarget + 1] , (imgModel->byImgG[iPtrModel]));
uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTarget->byImgR[iPtrTarget + 2] , (imgModel->byImgR[iPtrModel]));

}
}
}
}
UINT iScoreMin;
iScoreMin=UINT_MAX;
int iMaxR;
int iMaxC;


iMaxR=-1;
iMaxC=-1;
for(int iMapR=0; iMapR<iMapH; iMapR++)
{
for(int iMapC=0; iMapC<iMapW; iMapC++)
{
if(uiMap[iMapR*iMapW+iMapC]<iScoreMin){iScoreMin=uiMap[iMapR*iMapW+iMapC]; iMaxR=iMapR; iMaxC=iMapC;}

}
}
(*iFoundR)=iMaxR+iR0;
(*iFoundC)=iMaxC+iC0;
return TRUE;
}
return FALSE;
}
BOOL FindModel2(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC)
{
int iModelHeight;
int iModelWidth;

if(imgTarget == NULL){return FALSE;}
if(imgModel == NULL){return FALSE;}


iModelHeight = imgModel->iHeight;
iModelWidth = imgModel->iWidth;



if(
((imgTarget->iHeight>128) || (imgTarget->iWidth>128))
&&(imgTarget->iHeight>64)
&&(imgTarget->iWidth>64)
&&(imgModel->iHeight>16)
&&(imgModel->iWidth>16)
)

{
ImgRGB imgTargetCropped;
ImgRGB imgTargetPylam;
ImgRGB imgModelPylam;
CropImage(imgTarget, &imgTargetCropped,iR0, iC0, iR1, iC1);
CreatePyramid(&imgTargetCropped, &imgTargetPylam);
CreatePyramid(imgModel, &imgModelPylam);
BOOL bRet;
int iFoundR0, iFoundC0, iFoundR1, iFoundC1;
//		bRet = FindModel2(&imgTargetPylam, &imgModelPylam, 0, 0, imgTargetPylam.iHeight-1, imgTargetPylam.iWidth-1, &iFoundR0, &iFoundC0, &iFoundR1, &iFoundC1);
if(bRet != TRUE){return FALSE;}
}
else
{
int iFoundR, iFoundC;
BOOL bRet;
//		bRet = FindModel(&imgTarget, imgModel, iR0, iC0, iR1, iC1,&iFoundR, &iFoundC);
if(bRet != TRUE){return FALSE;}
}
return TRUE;
}

BOOL FindModelPyramid(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, double dThreshPercent, int* iFoundR0, int* iFoundC0, int* iFoundR1, int* iFoundC1)
{

if(imgTarget == NULL){return FALSE;}
if(imgModel == NULL){return FALSE;}
if(dThreshPercent<0){return FALSE;}
if(dThreshPercent>1){dThreshPercent=1;}
ImgRGB imgTargetTemp;
ImgRGB imgModelTemp;

CropImage2(imgTarget, &imgTargetTemp, iR0, iC0, iR1, iC1);
CropImage2(imgModel, &imgModelTemp, 0, 0, imgModel->iHeight-1, imgModel->iWidth-1);


int iModelHeight;
int iModelWidth;
iModelHeight = imgModelTemp.iHeight;
iModelWidth = imgModelTemp.iWidth;

UINT uiThresh;

uiThresh=UINT(dThreshPercent*255*iModelHeight*iModelWidth);
UINT* uiMap;
int iMapW;
int iMapH;
iMapW=imgTargetTemp.iWidth-imgModelTemp.iWidth+1;
iMapH=imgTargetTemp.iHeight-imgModelTemp.iHeight+1;
if(iMapW<=0){return FALSE;}
if(iMapH<=0){return FALSE;}
uiMap = new UINT[iMapW*iMapH];
int iR1Local;
int iC1Local;

iR1Local = imgTargetTemp.iHeight-1;
iC1Local = imgTargetTemp.iWidth-1;


int iScanHeight;
int iScanWidth;
iScanHeight = iR1Local-iModelHeight + 2;
iScanWidth = iC1Local-iModelWidth + 2;

if(iScanHeight<=0){return FALSE;}
if(iScanWidth<=0){return FALSE;}

BOOL bFound;


int iPtrTarget;
int iPtrModel;

memset(uiMap,0,iMapW*iMapH);
if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_3_8RGB))
{
for(int iMapR=0; iMapR<iMapH; iMapR++)
{
for(int iMapC=0; iMapC<iMapW; iMapC++)
{

int iTargetR=iMapR;
int iTargetC=iMapC;
for(int r=0; r<iModelHeight; r++)
{
for(int c=0; c<iModelWidth; c++)
{
if(iTargetR + r>=imgTargetTemp.iHeight){uiMap[iMapR*iMapW+iMapC]+=255*3;break;}
if(iTargetC + c>=imgTargetTemp.iWidth){uiMap[iMapR*iMapW+iMapC]+=255*3;break;}
iPtrTarget = 3*((iTargetR + r)*imgTargetTemp.iWidth+(iTargetC+c));
iPtrModel = (r)*imgModelTemp.iWidth+(c);

uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTargetTemp.byImgR[iPtrTarget + 0] , (imgModelTemp.byImgB[iPtrModel]));
uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTargetTemp.byImgR[iPtrTarget + 2] , (imgModelTemp.byImgG[iPtrModel]));
uiMap[iMapR*iMapW+iMapC]+=bySubAbs(imgTargetTemp.byImgR[iPtrTarget + 3] , (imgModelTemp.byImgR[iPtrModel]));

}
}
}
}
int iScoreMax;
iScoreMax=0;
int iMaxR;
int iMaxC;


iMaxR=-1;
iMaxC=-1;
int iFoundR0Temp;
int iFoundR1Temp;
int iFoundC0Temp;
int iFoundC1Temp;
iFoundR0Temp=0;
for(int iMapR=0; iMapR<iMapH; iMapR++)
{
for(int iMapC=0; iMapC<iMapW; iMapC++)
{
if(uiMap[iMapR*iMapW+iMapC]>uiThresh){iFoundR0Temp=iMapR; break;}
}
if(iFoundR0Temp>0){break;}
}
iFoundR1Temp=iMapH-1;
for(int iMapR=iMapH-1; iMapR>=0; iMapR--)
{
for(int iMapC=0; iMapC<iMapW; iMapC++)
{
if(uiMap[iMapR*iMapW+iMapC]>uiThresh){iFoundR1Temp=iMapR; break;}
}
if(iFoundR1Temp<iMapH-1){break;}
}
iFoundC0Temp=0;
for(int iMapC=0; iMapC<iMapW; iMapC++)
{
for(int iMapR=0; iMapR<iMapH; iMapR++)
{
if(uiMap[iMapR*iMapW+iMapC]>uiThresh){iFoundC0Temp=iMapC; break;}
}
if(iFoundC0Temp>0){break;}
}

iFoundC1Temp=iMapW-1;
for(int iMapC=iMapW-1; iMapC>=0; iMapC--)
{			for(int iMapR=0; iMapR<iMapH; iMapR++)
{
if(uiMap[iMapR*iMapW+iMapC]>uiThresh){iFoundC1Temp=iMapC; break;}
}
if(iFoundC1Temp<iMapW-1){break;}
}

*iFoundR0=iFoundR0Temp*2;
*iFoundR1=iFoundR1Temp*2+imgModel->iHeight;
*iFoundC0=iFoundC0Temp*2;
*iFoundC1=iFoundC1Temp*2+imgModel->iWidth;
return TRUE;
}
return FALSE;
}

BOOL FindModelPyramidRecursion(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, double dThreshPercent, int* iFoundR0, int* iFoundC0, int* iFoundR1, int* iFoundC1)
{

if(imgTarget == NULL){return FALSE;}
if(imgModel == NULL){return FALSE;}
if(dThreshPercent<0){return FALSE;}
if(dThreshPercent>1){dThreshPercent=1;}
ImgRGB imgTargetCropped;
ImgRGB imgModelCropped;
ImgRGB imgTargetPylam;
ImgRGB imgModelPylam;

if(
(imgTarget->iHeight<=64)
||(imgTarget->iWidth<=64)
||(imgModel->iHeight<=16)
||(imgModel->iWidth<=16)
)

{
return FindModelPyramid(imgTarget, imgModel, iR0, iC0, iR1, iC1, dThreshPercent, iFoundR0, iFoundC0, iFoundR1, iFoundC1);
}

CropImage2(imgTarget, &imgTargetCropped, iR0, iC0, iR1, iC1);
CropImage2(imgModel, &imgModelCropped, 0, 0, imgModel->iHeight-1, imgModel->iWidth-1);

CreatePyramid(&imgTargetCropped, &imgTargetPylam);
CreatePyramid(&imgModelCropped, &imgModelPylam);


return FindModelPyramidRecursion(&imgTargetPylam, &imgModelPylam, 0, 0, imgTargetPylam.iHeight-1, imgTargetPylam.iWidth-1, dThreshPercent, iFoundR0, iFoundC0, iFoundR1, iFoundC1);
}

*/


BOOL CorrelMap(ImgRGB* imgTarget, ImgRGB* imgModel, ImgMap* imgMap, int iR0, int iC0, int iR1, int iC1)
{
	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}

	int iModelHeight = imgModel->iHeight;
	int iModelWidth = imgModel->iWidth;


	int iR0Local;
	int iC0Local;

	int iR1Local;
	int iC1Local;

	iR0Local = iR0;
	iC0Local = iC0;

	iR1Local = iR1;
	iC1Local = iC1;

	if(iR0Local<0){iR0Local=0;}
	if(iC0Local<0){iC0Local=0;}
	if(iR1Local>=imgTarget->iHeight){iR1Local = imgTarget->iHeight-1;}
	if(iC1Local>=imgTarget->iWidth){iC1Local = imgTarget->iWidth-1;}

	if(((iC1Local-iC0Local+1)-imgModel->iWidth+1)<=0){return FALSE;}
	if(((iR1Local-iR0Local+1)-imgModel->iHeight+1)<=0){return FALSE;}

	imgMap->Set((iC1Local-iC0Local+1)-imgModel->iWidth+1,(iR1Local-iR0Local+1)-imgModel->iHeight+1);


	if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_3_8RGB))
	{
		for(int iMapR=0; iMapR<imgMap->iHeight; iMapR++)
		{
			for(int iMapC=0; iMapC<imgMap->iWidth; iMapC++)
			{

				int iTargetR=iR0Local+iMapR;
				int iTargetC=iC0Local+iMapC;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						if(iTargetR + r>=imgTarget->iHeight){imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=255*3;break;}
						if(iTargetC + c>=imgTarget->iWidth){imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=255*3;break;}
						int iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
						int iPtrModel = (r)*imgModel->iWidth+(c);

						imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImg[iPtrTarget + 0] , (imgModel->byImgB[iPtrModel]));
						imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImg[iPtrTarget + 1] , (imgModel->byImgG[iPtrModel]));
						imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImg[iPtrTarget + 2] , (imgModel->byImgR[iPtrModel]));

					}
				}
			}
		}
	}
	return TRUE;
}

inline void SumHorizontalLine(UINT* uiRSum, UINT* uiSumC, UINT uiImgW, UINT uiFilterHalfW)
{
	uiSumC[0]=uiRSum[0];
	for(UINT cl=1; cl<=uiFilterHalfW; cl++)
	{
		uiSumC[0]+=uiRSum[cl];
	}

	for(UINT c=1; c<=uiFilterHalfW; c++)
	{
		uiSumC[c]=uiSumC[c-1]+uiRSum[c+uiFilterHalfW];
	}
	for(UINT c=uiFilterHalfW+1; c<uiImgW-uiFilterHalfW; c++)
	{
		uiSumC[c]=uiSumC[c-1]+uiRSum[c+uiFilterHalfW]-uiRSum[c-uiFilterHalfW-1];
	}
	for(INT c=uiImgW-uiFilterHalfW; c<uiImgW; c++)
	{
		uiSumC[c]=uiSumC[c-1]-uiRSum[c-uiFilterHalfW-1];
	}
	return;
}

void VerticalSumR0(BYTE* byImg, UINT* uiRSumData, UINT uiImgW, UINT uiFilterHalfH)
{
	for(UINT c=0; c<uiImgW; c++)
	{
		uiRSumData[c]=byImg[c];
		for(UINT rl=1; rl<uiFilterHalfH; rl++)
		{
			uiRSumData[c]+=byImg[rl*uiImgW+c];
		}
	}
	return;
}


inline void UpdateVerticalSum(BYTE* byImg, UINT* uiRSumData, UINT r_global, UINT uiImgW, UINT uiImgH, UINT uiFilterHalfH)
{
	if(r_global>uiFilterHalfH)
	{
		UINT uiROff = (r_global-uiFilterHalfH-1)*uiImgW;
		for(UINT c=0; c<uiImgW; c++)
		{
			uiRSumData[c]-=byImg[uiROff+c];
		}
	}


	if(r_global+uiFilterHalfH>=uiImgH){return;}

	UINT uiROff=(r_global+uiFilterHalfH)*uiImgW;
	for(UINT c=0; c<uiImgW; c++)
	{
		uiRSumData[c]+=byImg[uiROff+c];
	}
	return;
}

inline void DivideHorizontalLine(UINT* uiSum, BYTE* byMean, UINT uiImgW, UINT uiSumH, UINT uiFilterHalfW)
{
	UINT uiSumW=uiFilterHalfW+1;
	for(UINT c=0; c<uiFilterHalfW; c++)
	{
		byMean[c]=uiSum[c]/(uiSumH*uiSumW);
		uiSumW++;
	}
	UINT uiSumHW=uiSumH*uiSumW;
	for(UINT c=uiImgW-uiFilterHalfW; c<uiImgW; c++)
	{
		uiSumW--;
		byMean[c]=uiSum[c]/(uiSumH*uiSumW);
	}
}

inline void MeanDivide(UINT* uiSum, BYTE* byMean, UINT uiImgW, UINT uiImgH, UINT uiFilterHalfW, UINT uiFilterHalfH)
{
	UINT uiSumH=uiFilterHalfH+1;
	for(UINT r=0; r<uiFilterHalfH; r++)
	{
		DivideHorizontalLine(&(uiSum[r*uiImgW]), &(byMean[r*uiImgW]), uiImgW, uiSumH, uiFilterHalfW);
		uiSumH++;
	}

	for(UINT r=uiFilterHalfH; r<uiImgH-uiFilterHalfH; r++)
	{
		DivideHorizontalLine(&(uiSum[r*uiImgW]), &(byMean[r*uiImgW]), uiImgW, uiSumH, uiFilterHalfW);
	}

	for(UINT r=uiImgH-uiFilterHalfH; r<uiImgH; r++)
	{
		uiSumH--;
		DivideHorizontalLine(&(uiSum[r*uiImgW]), &(byMean[r*uiImgW]), uiImgW, uiSumH, uiFilterHalfW);
	}
}

BOOL MeanFilter(BYTE* byImg, BYTE* byResult, UINT uiImgW, UINT uiImgH, UINT uiFilterW, UINT uiFilterH)
{
	if(byImg==NULL){return FALSE;}
	if(byResult == NULL){return FALSE;}
	if(uiFilterW < 3){return FALSE;}
	if(uiFilterH<3){return FALSE;}
	if((uiFilterW%2)!=1){return FALSE;}
	if((uiFilterH%2)!=1){return FALSE;}
	if(uiImgW<uiFilterW){return FALSE;}
	if(uiImgH<uiFilterH){return FALSE;}

	if((uiFilterW*uiFilterH)>= (UINT_MAX >> 8)){return FALSE;}

	UINT uiFilterHalfW = (uiFilterW-1)/2;
	UINT uiFilterHalfH = (uiFilterH-1)/2;

	UINT* uiImgSum=NULL;
	uiImgSum=new UINT[uiImgW*uiImgH];
	if(uiImgSum==NULL){return FALSE;}


	UINT* uiRSumData=NULL;
	uiRSumData=new UINT[uiImgW*uiImgH];
	if(uiRSumData==NULL){SAFE_DELETE(uiImgSum); return FALSE;}
	VerticalSumR0(byImg, uiRSumData, uiImgW, uiFilterHalfH);
	SumHorizontalLine(uiRSumData, &(uiImgSum[0*uiImgW]), uiImgW, uiFilterHalfW);
	for(UINT r=1; r<uiImgH; r++)
	{
		UpdateVerticalSum(byImg, uiRSumData, r, uiImgW, uiImgH, uiFilterHalfH);
		SumHorizontalLine(uiRSumData, &(uiImgSum[r*uiImgW]), uiImgW, uiFilterHalfW);
	}
	MeanDivide(uiImgSum, byResult, uiImgW, uiImgH, uiFilterHalfW, uiFilterHalfH);
	SAFE_DELETE(uiImgSum);
	SAFE_DELETE(uiRSumData);
	return TRUE;
}


inline BYTE byMaxC(BYTE* byImg, UINT uiImgW, UINT r0, UINT c0, UINT uiFilterW, UINT* uiMaxCL)
{
	BYTE byMax=0;
	*uiMaxCL=0;
	for(UINT cl=0; cl<uiFilterW; cl++)
	{
		if(byMax<= byImg[r0 * uiImgW+c0+cl])
		{
			byMax=byImg[r0 * uiImgW+c0+cl];
			*uiMaxCL=cl;
		}
	}
	return byMax;
}





inline BYTE byMaxR(BYTE* byImg, UINT uiImgW, UINT r0, UINT c0, UINT uiFilterH, UINT* uiMaxRL)
{
	BYTE byMax=0;
	for(int rl=0; rl<uiFilterH; rl++)
	{
		if(byMax<=byImg[(r0+rl)*uiImgW+c0])
		{
			byMax=byImg[(r0+rl)*uiImgW+c0];
			*uiMaxRL=rl;
		}
	}
	return byMax;
}

inline BYTE byMaxRC(BYTE* byImg, int iImgW, int r0, int c0, int iFilterW, int iFilterH)
{
	BYTE byMax=0;
	for(int rl=0; rl<iFilterH; rl++)
	{
		for(int cl=0; cl<iFilterW; cl++)
		{
			if(byMax<=byImg[(r0+rl)*iImgW+cl+c0])
			{
				byMax=byImg[(r0+rl)*iImgW+cl+c0];
			}
		}
	}
	return byMax;
}






inline void UpdateRDirectionMax(BYTE* byImgExp, BYTE* byRMaxData, int r_origin, int iImgW, int iFilterHalfW, int iFilterHalfH, UINT* uiMaxPositions)
{
	if(r_origin==0)
	{
		for(int c=0; c<iImgW; c++)
		{
			byRMaxData[c+iFilterHalfW]=byMaxR(byImgExp, iImgW, r_origin+iFilterHalfH, c, iFilterHalfH+1, &(uiMaxPositions[c]));
		}
		return;
	}

	for(int c=0; c<iImgW; c++)
	{
		if(byRMaxData[c+iFilterHalfW] < byImgExp[(r_origin+2*iFilterHalfH)*iImgW+c])
		{
			byRMaxData[c+iFilterHalfW]=byImgExp[(r_origin+2*iFilterHalfH)*iImgW+c];
			uiMaxPositions[c]=2*iFilterHalfH;
			continue;
		}

		if(uiMaxPositions[c]==0)
		{
			byRMaxData[c+iFilterHalfW]=byMaxR(byImgExp, iImgW, r_origin, c, 2*iFilterHalfH+1, &(uiMaxPositions[c]));
			continue;
		}
		(uiMaxPositions[c])--;
	}
}


BOOL MaxFilter(BYTE* byImg, BYTE* byResult, UINT uiImgW, UINT uiImgH, UINT uiFilterW, UINT uiFilterH)
{
	if(byImg!=NULL){return FALSE;}
	if(byResult!=NULL){return FALSE;}

	BYTE* byImgExpanded;
	BYTE* byRMaxData;
	UINT* uiMaxPositions;
	UINT uiMaxPositionCL;

	if((uiFilterW % 2)!=1){return FALSE;}
	if((uiFilterH % 2)!=1){return FALSE;}

	UINT uiFilterHalfW = (uiFilterW-1)/2;
	UINT uiFilterHalfH = (uiFilterH-1)/2;

	BOOL bSameMemory=FALSE;
	BYTE* byResultLocal;
	BYTE* pbyResult;
	if(byImg==byResult)
	{
		bSameMemory=TRUE;
		byResultLocal=new BYTE[uiImgW*uiImgH];
		pbyResult=byResultLocal;
	}
	else
	{
		pbyResult=byResult;
	}

	byImgExpanded = new BYTE[uiImgW*(uiImgH + 2 * uiFilterHalfH)];
	byRMaxData = new BYTE[uiImgW + 2*uiFilterHalfW];
	uiMaxPositions = new UINT[uiImgW];

	memset(byImgExpanded, 0, uiImgW*(uiImgH + 2 * uiFilterHalfH));
	for(UINT r=0; r<uiImgH; r++)
	{
		for(UINT c=0; c<uiImgW; c++)
		{
			byImgExpanded[(r + uiFilterHalfH)*uiImgW+c] = byImg[r*uiImgW+c];
		}
	}
	memset(byRMaxData, 0, uiImgW + 2*uiFilterHalfW);
	for(UINT r=0; r<uiImgH; r++)
	{
		UpdateRDirectionMax(byImgExpanded, byRMaxData, r, uiImgW, uiFilterHalfW, uiFilterHalfH, uiMaxPositions);

		pbyResult[r*uiImgW+0]=byMaxC(byRMaxData, uiImgW + uiFilterHalfW, 0, 0, uiFilterW, &(uiMaxPositionCL));
		for(UINT c=1; c<uiImgW; c++)
		{
			if(byRMaxData[c+2*uiFilterHalfW]>pbyResult[r*uiImgW+c-1])
			{
				pbyResult[r*uiImgW+c] = byRMaxData[c+2*uiFilterHalfW];
				uiMaxPositionCL = 2*uiFilterHalfW;
				continue;
			}
			if(uiMaxPositionCL==0)
			{
				pbyResult[r*uiImgW+c] = byMaxC(byRMaxData, uiImgW+2*uiFilterHalfW, 0, c, uiFilterW,&uiMaxPositionCL);
				continue;
			}
			uiMaxPositionCL--;
			pbyResult[r*uiImgW+c] = pbyResult[r*uiImgW+c-1];
		}
	}

	if(bSameMemory==TRUE)
	{
		for(UINT r=0; r<uiImgH; r++)
		{
			for(UINT c=0; c<uiImgW; c++)
			{
				byResult[r*uiImgW+c]=byResultLocal[r*uiImgW+c];
			}
		}
		SAFE_DELETE(byResultLocal);
	}

	SAFE_DELETE(uiMaxPositions);
	SAFE_DELETE(byImgExpanded);
	SAFE_DELETE(byRMaxData);
	return TRUE;
}
BOOL InvertImage(BYTE* byImg, BYTE* byResult, UINT uiImgW, UINT uiImgH)
{
	if(byImg!=NULL){return FALSE;}
	if(byResult!=NULL){return FALSE;}

	for(UINT r=1; r<uiImgH-1; r++)
	{
		for(UINT c=1; c<uiImgW-1; c++)
		{
			byResult[r*uiImgW+c]=255-byResult[r*uiImgW+c];
		}
	}
	return TRUE;
}
BOOL LaplacianFilter(BYTE* byImg, BYTE* byResult, UINT uiImgW, UINT uiImgH, UINT uiFilterW, UINT uiFilterH)
{
	if(byImg!=NULL){return FALSE;}
	if(byResult!=NULL){return FALSE;}

	BOOL bSameMemory=FALSE;
	BYTE* byResultLocal;
	BYTE* pbyResult;
	if(byImg==byResult)
	{
		bSameMemory=TRUE;
		byResultLocal=new BYTE[uiImgW*uiImgH];
		pbyResult=byResultLocal;
	}
	else
	{
		pbyResult=byResult;
	}


	memset(pbyResult, 0, uiImgW*uiImgH);

	for(UINT r=1; r<uiImgH-1; r++)
	{
		for(UINT c=1; c<uiImgW-1; c++)
		{
			int iSum=0;
			iSum+=byImg[(r-1)*uiImgW+(c+0)]*(-1);
			iSum+=byImg[(r+0)*uiImgW+(c-1)]*(-1);
			iSum+=byImg[(r+0)*uiImgW+(c+1)]*(-1);
			iSum+=byImg[(r+1)*uiImgW+(c+0)]*(-1);
			iSum+=byImg[(r+0)*uiImgW+(c+0)]*(4);
			if(iSum<0){iSum=0;}
			if(iSum>=255){iSum=255;}

			pbyResult[r*uiImgW+c]=(BYTE)(iSum);
		}
	}

	if(bSameMemory==TRUE)
	{
		for(UINT r=0; r<uiImgH; r++)
		{
			for(UINT c=0; c<uiImgW; c++)
			{
				byResult[r*uiImgW+c]=byResultLocal[r*uiImgW+c];
			}
		}
		SAFE_DELETE(byResultLocal);
	}

	return TRUE;
}

BOOL CreateMinMaxSumImg(ImgRGB* img,int iR0, int iC0, int iR1, int iC1,int iKernelWidth, int iKernelHeight, ImgRGB* imgMax, ImgRGB* imgMin, ImgMap* imgSum)
{
	if(iR0<0){return FALSE;}
	if(iC0<0){return FALSE;}
	if(iR1>img->iHeight-1){return FALSE;}
	if(iC1>img->iWidth-1){return FALSE;}
	int iROIW=(iC1-iC0+1);
	int iROIH=(iR1-iR0+1);

	if(iROIW<iKernelWidth){return FALSE;}
	if(iROIH<iKernelHeight){return FALSE;}

	int iResultWidth = (iROIW/iKernelWidth) + (iROIW%iKernelWidth);
	int iResultHeight= (iROIH/iKernelHeight) + (iROIH%iKernelHeight);

	imgMax->Set(iResultWidth, iResultHeight,img->iChannel);
	imgMin->Set(iResultWidth, iResultHeight,img->iChannel);
	imgSum->Set(iResultWidth, iResultHeight);


	return TRUE;
}



BOOL CorrelMapPyramid(ImgRGB* imgTarget, int iPointerTargetOffset, int iTargetWidth, int iTargetHeight, ImgRGB* imgModel, int iPointerModelOffset, int iModelWidth, int iModelHeight, ImgMap* imgMap, int iR0, int iC0, int iR1, int iC1)
{
	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}



	int iR0Local;
	int iC0Local;

	int iR1Local;
	int iC1Local;

	iR0Local = iR0;
	iC0Local = iC0;

	iR1Local = iR1;
	iC1Local = iC1;

	if(iR0Local<0){iR0Local=0;}
	if(iC0Local<0){iC0Local=0;}
	if(iR1Local>=iTargetHeight){iR1Local = iTargetHeight-1;}
	if(iC1Local>=iTargetWidth){iC1Local = iTargetWidth-1;}

	if(((iC1Local-iC0Local+1)-imgModel->iWidth+1)<=0){return FALSE;}
	if(((iR1Local-iR0Local+1)-imgModel->iHeight+1)<=0){return FALSE;}

	imgMap->Set((iC1Local-iC0Local+1)-imgModel->iWidth+1,(iR1Local-iR0Local+1)-imgModel->iHeight+1);


	for(int iMapR=0; iMapR<imgMap->iHeight; iMapR++)
	{
		for(int iMapC=0; iMapC<imgMap->iWidth; iMapC++)
		{

			int iTargetR=iR0Local+iMapR;
			int iTargetC=iC0Local+iMapC;
			for(int r=0; r<iModelHeight; r++)
			{
				for(int c=0; c<iModelWidth; c++)
				{
					if(iTargetR + r>=imgTarget->iHeight){imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=255*3;break;}
					if(iTargetC + c>=imgTarget->iWidth){imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=255*3;break;}
					int iPtrTarget = (iTargetR + r)*imgTarget->iWidth+(iTargetC+c)+iPointerTargetOffset;
					int iPtrModel = (r)*imgModel->iWidth+(c)+iPointerModelOffset;

					imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImgR[iPtrTarget] , (imgModel->byImgB[iPtrModel]));
					imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImgG[iPtrTarget] , (imgModel->byImgG[iPtrModel]));
					imgMap->uiMap[iMapR*imgMap->iWidth+iMapC]+=bySubAbs(imgTarget->byImgB[iPtrTarget] , (imgModel->byImgR[iPtrModel]));

				}
			}
		}
	}
	return TRUE;
}

BOOL FindModel(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC, double dThreshPercent)
{
	int iModelHeight;
	int iModelWidth;

	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}



	iModelHeight = imgModel->iHeight;
	iModelWidth = imgModel->iWidth;
	ImgMap imgMap;
	int iR0Local=iR0;
	int iC0Local=iC0;
	int iR1Local=iR1;
	int iC1Local=iC1;
	if(iR0Local<0){iR0Local=0;}
	if(iC0Local<0){iC0Local=0;}
	if(iR1Local>=imgTarget->iHeight){iR1Local = imgTarget->iHeight-1;}
	if(iC1Local>=imgTarget->iWidth){iC1Local = imgTarget->iWidth-1;}



	UINT iScoreMin;
	iScoreMin=UINT_MAX;
	int iMaxR;
	int iMaxC;

	BOOL bRet;
	bRet = CorrelMap(imgTarget, imgModel, &imgMap, iR0, iC0, iR1, iC1);
	if(bRet != TRUE){return FALSE;}

	iMaxR=-1;
	iMaxC=-1;
	for(int iMapR=0; iMapR<imgMap.iHeight; iMapR++)
	{
		for(int iMapC=0; iMapC<imgMap.iWidth; iMapC++)
		{
			if(imgMap.uiMap[iMapR*imgMap.iWidth+iMapC]<iScoreMin){iScoreMin=imgMap.uiMap[iMapR*imgMap.iWidth+iMapC]; iMaxR=iMapR; iMaxC=iMapC;}
		}
	}
	if((1-(imgMap.uiMap[iMaxR*imgMap.iWidth+iMaxC]/((double)imgModel->iWidth*imgModel->iHeight*255)))*100<dThreshPercent){return FALSE;}
	(*iFoundR)=iMaxR+iR0Local;
	(*iFoundC)=iMaxC+iC0Local;
	return TRUE;
}

BOOL DetectArea(ImgMap* imgMap, UINT uiThreshScore, int* iR0, int* iC0, int* iR1, int* iC1)
{
	BOOL bFoundR0=FALSE;
	BOOL bFoundC0=FALSE;
	BOOL bFoundR1=FALSE;
	BOOL bFoundC1=FALSE;

	for(int r= 0; r<imgMap->iHeight; r++)
	{
		for(int c=0; c<imgMap->iWidth; c++)
		{
			if(imgMap->uiMap[r*imgMap->iWidth+c]>uiThreshScore){bFoundR0=TRUE;*iR0=r; break;}
		}
		if(bFoundR0==TRUE){ break;}
	}

	for(int c=0; c<imgMap->iWidth; c++)
	{
		for(int r= 0; r<imgMap->iHeight; r++)
		{
			if(imgMap->uiMap[r*imgMap->iWidth+c]>uiThreshScore){bFoundC0=TRUE; *iC0=c;break;}
		}
		if(bFoundC0==TRUE){ break;}
	}

	for(int r= imgMap->iHeight-1; r>=0; r--)
	{
		for(int c=0; c<imgMap->iWidth; c++)
		{
			if(imgMap->uiMap[r*imgMap->iWidth+c]>uiThreshScore){bFoundR1=TRUE;*iR1=r; break;}
		}
		if(bFoundR1==TRUE){ break;}
	}

	for(int c=imgMap->iWidth-1; c>=0; c--)
	{
		for(int r= 0; r<imgMap->iHeight; r++)
		{
			if(imgMap->uiMap[r*imgMap->iWidth+c]>uiThreshScore){bFoundC1=TRUE; *iC1=c;break;}
		}
		if(bFoundC1==TRUE){ break;}
	}
	if(bFoundR0 != TRUE){*iR0=0; *iC0=0; *iR1=0; *iC1=0; return FALSE;}
	if(bFoundC0 != TRUE){*iR0=0; *iC0=0; *iR1=0; *iC1=0; return FALSE;}
	if(bFoundR1 != TRUE){*iR0=0; *iC0=0; *iR1=0; *iC1=0; return FALSE;}
	if(bFoundC1 != TRUE){*iR0=0; *iC0=0; *iR1=0; *iC1=0; return FALSE;}

	return TRUE;
}
BOOL FindModelPyramid(ImgRGB* imgTarget, ImgRGB* imgModel, int iR0, int iC0, int iR1, int iC1, double dThreshPercent, int* iFoundR, int* iFoundC)
{

	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}
	if(dThreshPercent<0){return FALSE;}
	if(dThreshPercent>1){dThreshPercent=1;}
	ImgRGBPyramid imgTargetPyram;
	ImgRGBPyramid imgModelPyram;

	ImgMap imgMap;

	imgTargetPyram.SetPyramid(imgTarget);
	imgModelPyram.SetPyramid(imgModel);

	int iLevelStart=3;
	int iR0Level = GetLevelSize(iR0, iLevelStart);
	int iC0Level = GetLevelSize(iC0, iLevelStart);
	int iR1Level = GetLevelSize(iR1, iLevelStart);
	int iC1Level = GetLevelSize(iC1, iLevelStart);

	for(int iLevel=iLevelStart; iLevel>=0; iLevel--)
	{
		int iTargetOffset = GetPointerOffset(imgTarget->iWidth, imgTarget->iHeight, iLevel);
		int iModelOffset = GetPointerOffset(imgTarget->iWidth, imgTarget->iHeight, iLevel);
		int iTargetWidthLevel=GetLevelSize(imgTarget->iWidth, iLevel);
		int iTargetHeightLevel=GetLevelSize(imgTarget->iHeight, iLevel);
		int iModelWidthLevel=GetLevelSize(imgModel->iWidth, iLevel);
		int iModelHeightLevel=GetLevelSize(imgModel->iHeight, iLevel);

		if(iR0Level<0){iR0Level=0;}
		if(iC0Level<0){iC0Level=0;}
		if(iR1Level>=iTargetHeightLevel){iR1Level = iTargetWidthLevel-1;}
		if(iC1Level>=iTargetWidthLevel){iC1Level = iTargetHeightLevel-1;}

		BOOL bRet;
		bRet = CorrelMapPyramid(&(imgTargetPyram.imgRGB), iTargetOffset, iTargetWidthLevel, iTargetHeightLevel, &(imgModelPyram.imgRGB), iModelOffset, iModelWidthLevel, iModelHeightLevel, &imgMap, iR0Level, iC0Level, iR1Level, iC1Level);
		if(bRet != TRUE){return FALSE;}

		if(iLevel==0)
		{
			UINT iScoreMin;
			iScoreMin=UINT_MAX;
			int iMaxR=-1;
			int iMaxC=-1;
			for(int iMapR=0; iMapR<imgMap.iHeight; iMapR++)
			{
				for(int iMapC=0; iMapC<imgMap.iWidth; iMapC++)
				{
					if(imgMap.uiMap[iMapR*imgMap.iWidth+iMapC]<iScoreMin){iScoreMin=imgMap.uiMap[iMapR*imgMap.iWidth+iMapC]; iMaxR=iMapR; iMaxC=iMapC;}
				}
			}
			if((1-(imgMap.uiMap[iMaxR*imgMap.iWidth+iMaxC]/((double)imgModel->iWidth*imgModel->iHeight*255)))*100<dThreshPercent){return FALSE;}
			(*iFoundR)=iMaxR+iR0Level;
			(*iFoundC)=iMaxC+iC0Level;
			return TRUE;
		}

		int iDetectR0;
		int iDetectC0;
		int iDetectR1;
		int iDetectC1;
		bRet = DetectArea(&imgMap, (UINT)(iModelWidthLevel*iModelHeightLevel*dThreshPercent*255), &iDetectR0, &iDetectC0, &iDetectR1, &iDetectC1);

		iR0Level = iDetectR0*2-1;
		iC0Level = iDetectC0*2-1;
		iR1Level = (iDetectR1+iModelHeightLevel)*2+1;
		iC1Level = (iDetectC1+iModelWidthLevel)*2+1;

	}


	return FALSE;
}


BOOL IsInRegionMask(ImgRGB* imgTarget, ImgRGB* imgModel, ImgRGB* imgMask, int iR0, int iC0, int iR1, int iC1, int* iFoundR, int* iFoundC)
{
	int iModelHeight;
	int iModelWidth;

	if(imgTarget == NULL){return FALSE;}
	if(imgModel == NULL){return FALSE;}
	if(imgMask == NULL){return FALSE;}

	if(imgModel->iHeight != imgMask->iHeight){return FALSE;}
	if(imgModel->iWidth != imgMask->iWidth){return FALSE;}


	iModelHeight = imgModel->iHeight;
	iModelWidth = imgModel->iWidth;


	int iR0Local;
	int iC0Local;
	int iR1Local;
	int iC1Local;


	int iInclR;
	int iInclC;

	if(iR0<iR1)
	{
		iInclR=+1;
		iR0Local = max(iR0, 0);
		iR1Local = min(iR1, imgTarget->iHeight-1);
	}
	else
	{
		iInclR=-1;
		iR0Local = min(iR0, imgTarget->iHeight-1);
		iR1Local = max(iR1, 0);
	}

	if(iC0<iC1)
	{
		iInclC=+1;
		iC0Local = max(iC0, 0);
		iC1Local = min(iC1, imgTarget->iWidth-1);
	}
	else
	{
		iInclC=-1;
		iC0Local = min(iC0, imgTarget->iWidth-1);
		iC1Local = max(iC1, 0);
	}




	int iScanHeight;
	int iScanWidth;
	iScanHeight = abs(iR1Local-iR0Local)-iModelHeight + 2;
	iScanWidth = abs(iC1Local-iC0Local)-iModelWidth + 2;
	
	if(iScanHeight<=0){return FALSE;}
	if(iScanWidth<=0){return FALSE;}

	int iRStart, iCStart;

	if(iR0<iR1)
	{
		iRStart=iR0Local;
	}
	else
	{
		iRStart=iR0Local-iModelHeight+1;
	}
	if(iC0<iC1)
	{
		iCStart = iC0Local;
	}
	else
	{
		iCStart = iC0Local-iModelWidth+1;
	}

	int iPtrTarget;
	int iPtrModel;

	if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_3_8RGB))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;
				BOOL bOK_R=TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
						iPtrModel = ((r)*imgModel->iWidth)+(c);
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 0] , (imgModel->byImgB[iPtrModel])) > imgMask->byImgB[iPtrModel]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 1] , (imgModel->byImgG[iPtrModel])) > imgMask->byImgG[iPtrModel]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 2] , (imgModel->byImgR[iPtrModel])) > imgMask->byImgR[iPtrModel]){bOK_R=FALSE;break;}
					}
					if(bOK_R==FALSE){break;}
				}
				if(bOK_R==TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2;return TRUE;}
			}
		}
	}

	if((imgTarget->iChannel==CHANNEL_3_8RGB) && (imgModel->iChannel == CHANNEL_3_8RGB))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;

				BOOL bOK_R=TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = (iTargetR + r)*imgTarget->iWidth+(iTargetC+c);
						iPtrModel = (r)*imgModel->iWidth+(c);
						if(bySubAbs(imgTarget->byImgR[iPtrTarget] , (imgModel->byImgR[iPtrModel])) > imgMask->byImgR[iPtrModel]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImgG[iPtrTarget] , (imgModel->byImgG[iPtrModel])) > imgMask->byImgG[iPtrModel]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImgB[iPtrTarget] , (imgModel->byImgB[iPtrModel])) > imgMask->byImgB[iPtrModel]){bOK_R=FALSE;break;}
					}
					if(bOK_R==FALSE){break;}
				}
				if(bOK_R==TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2; return TRUE;}
			}
		}
	}

	if((imgTarget->iChannel==CHANNEL_1_24BGR) && (imgModel->iChannel == CHANNEL_1_24BGR))
	{
		for(int iIterR = 0; iIterR<iScanHeight; iIterR++)
		{
			int iTargetR=iRStart+iIterR*iInclR;
			for(int iIterC = 0; iIterC<iScanWidth; iIterC++)
			{
				int iTargetC=iCStart+iIterC*iInclC;
				BOOL bOK_R=TRUE;
				for(int r=0; r<iModelHeight; r++)
				{
					for(int c=0; c<iModelWidth; c++)
					{
						iPtrTarget = 3*((iTargetR + r)*imgTarget->iWidth+(iTargetC+c));
						iPtrModel = 3*((r)*imgModel->iWidth+(c));
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 0] , (imgModel->byImg[iPtrModel + 0])) > imgMask->byImg[iPtrModel + 0]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 1] , (imgModel->byImg[iPtrModel + 1])) > imgMask->byImg[iPtrModel + 1]){bOK_R=FALSE;break;}
						if(bySubAbs(imgTarget->byImg[iPtrTarget + 2] , (imgModel->byImg[iPtrModel + 2])) > imgMask->byImg[iPtrModel + 2]){bOK_R=FALSE;break;}
					}
					if(bOK_R==FALSE){break;}
				}
				if(bOK_R==TRUE){*iFoundR = iTargetR+iModelHeight/2; *iFoundC = iTargetC+iModelWidth/2; return TRUE;}
			}
		}
	}


	return FALSE;
}


BOOL ConcatObj(Object* objIn1, Object* objIn2, Object* objOut)
{
	if(objIn1->m_iMaxID<0){objOut->Copy(objIn2); return TRUE;}
	if(objIn2->m_iMaxID<0){objOut->Copy(objIn1); return TRUE;}
	Object objIn1Local;
	Object objIn2Local;

	objIn1Local.Copy(objIn1);
	objIn2Local.Copy(objIn2);
	UINT uiMaxLabel1=objIn1Local.m_uiMaxLabel;
	for(int iID=0; iID<=objIn2Local.m_iMaxID; iID++)
	{
		objIn2Local.runLength[iID].uiLabel+=uiMaxLabel1+1;
	}


	objOut->Alloc(objIn1Local.m_iMaxID+1 + objIn2Local.m_iMaxID+1);

	int iNewID=0;
	for(int iID=0; iID<=objIn1Local.m_iMaxID; iID++)
	{
		objOut->runLength[iNewID].Copy(&(objIn1Local.runLength[iID]));
		iNewID++;
	}

	for(int iID=0; iID<=objIn2Local.m_iMaxID; iID++)
	{
		objOut->runLength[iNewID].Copy(&(objIn2Local.runLength[iID]));
		iNewID++;
	}

	objOut->ReCheckID();
	return TRUE;
}

BOOL ConvertImageCHANNEL_3_8RGB(ImgRGB* imgIn, ImgRGB* imgOut)
{
	imgOut->Set(imgIn->iWidth, imgIn->iHeight, CHANNEL_3_8RGB);

	if(imgIn->iChannel==CHANNEL_3_8RGB){imgOut->Assign(imgIn);return TRUE;}

	if(imgIn->iChannel==CHANNEL_1_24BGR)
	{
		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				imgOut->byImgB[r*imgIn->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+0];
				imgOut->byImgG[r*imgIn->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+1];
				imgOut->byImgR[r*imgIn->iWidth+c]=imgIn->byImg[3*(r*imgIn->iWidth+c)+2];
			}
		}
		return TRUE;
	}


	if(imgIn->iChannel==CHANNEL_1_8)
	{
		for(int r=0; r<imgIn->iHeight; r++)
		{
			for(int c=0; c<imgIn->iWidth; c++)
			{
				imgOut->byImgB[r*imgIn->iWidth+c]=imgIn->byImg[r*imgIn->iWidth+c];
				imgOut->byImgG[r*imgIn->iWidth+c]=imgIn->byImg[r*imgIn->iWidth+c];
				imgOut->byImgR[r*imgIn->iWidth+c]=imgIn->byImg[r*imgIn->iWidth+c];
			}
		}
		return TRUE;
	}

	return FALSE;
}

BOOL PaintRegion(ImgRGB* imgIn, Object* objIn, ImgRGB* imgOut, BYTE byR, BYTE byG, BYTE byB)
{
	BOOL bRet;
	bRet = ConvertImageCHANNEL_3_8RGB(imgIn, imgOut);
	if(bRet != TRUE){return FALSE;}

	for(int iID=0; iID<= objIn->m_iMaxID; iID++)
	{
		int r=objIn->runLength[iID].iR;
		for(int c= objIn->runLength[iID].iCStart; c<= objIn->runLength[iID].iCEnd; c++)
		{
			imgOut->byImgR[r*imgOut->iWidth+c]=byR;
			imgOut->byImgG[r*imgOut->iWidth+c]=byG;
			imgOut->byImgB[r*imgOut->iWidth+c]=byB;
		}
	}

	return TRUE;
}

BYTE g_byR[3]={255, 0, 0};
BYTE g_byG[3]={0, 255, 0};
BYTE g_byB[3]={0, 0, 255};
UINT g_uiColored=3;

BOOL PaintRegion(ImgRGB* imgIn, Object* objIn, ImgRGB* imgOut)
{
	BOOL bRet;
	bRet = ConvertImageCHANNEL_3_8RGB(imgIn, imgOut);
	if(bRet != TRUE){return FALSE;}

	for(int iID=0; iID<= objIn->m_iMaxID; iID++)
	{
		int r=objIn->runLength[iID].iR;
		for(int c= objIn->runLength[iID].iCStart; c<= objIn->runLength[iID].iCEnd; c++)
		{
			if(objIn->runLength[iID].uiLabel==0)
			{
				imgOut->byImgR[r*imgOut->iWidth+c]=g_byR[0];
				imgOut->byImgG[r*imgOut->iWidth+c]=g_byG[0];
				imgOut->byImgB[r*imgOut->iWidth+c]=g_byB[0];
				continue;
			}
			imgOut->byImgR[r*imgOut->iWidth+c]=g_byR[(objIn->runLength[iID].uiLabel-1)%g_uiColored];
			imgOut->byImgG[r*imgOut->iWidth+c]=g_byG[(objIn->runLength[iID].uiLabel-1)%g_uiColored];
			imgOut->byImgB[r*imgOut->iWidth+c]=g_byB[(objIn->runLength[iID].uiLabel-1)%g_uiColored];
		}
	}

	return TRUE;
}
BOOL SelectShape(Object* objIn, Object* objOut, CString sFeature, double dMin, double dMax)
{

	Object objTemp;
	double dA, dR, dC;

	objOut->Init();
	if(sFeature.CompareNoCase(_T("area"))==0)
	{
		for(int i=0; i<=objIn->m_uiMaxLabel; i++)
		{
			SelectObj(objIn, i, &objTemp);
			AreaCenter(&objTemp, &dA, &dR, &dC);
			if((dA>=dMin)&&(dA<=dMax))
			{
				ConcatObj(objOut,&objTemp,objOut);
			}
		}
	}
	objOut->ReCheckID();
	return TRUE;
}

BOOL Connection(Object* objIn, Object* objOut, int iNeighborPolicy)
{
	Object objTemp;
	BOOL bRet;

	bRet = objTemp.Copy(objIn);
	if(bRet != TRUE){return FALSE;}

	bRet = objTemp.Connection(iNeighborPolicy);
	if(bRet != TRUE){return FALSE;}

	bRet = objOut->Copy(&objTemp);
	if(bRet != TRUE){return FALSE;}

	return TRUE;
}

BOOL SortRegion(Object* objIn, CString sMode, CString sAscDsc, Object* objOut)
{
	if((sAscDsc.CompareNoCase(_T("Asc"))!=0)&&(sAscDsc.CompareNoCase(_T("Dsc"))!=0)){return FALSE;}
	Object objLocal;
	objLocal.Copy(objIn);
	objLocal.Truncate();

	if(sMode.CompareNoCase(_T("area"))==0)
	{
		double* dFeatures;
		dFeatures = new double[objLocal.m_uiMaxLabel];
		UINT* uiFeatures;
		uiFeatures = new UINT[objLocal.m_uiMaxLabel];
		memset(uiFeatures,0,sizeof(UINT)*objLocal.m_uiMaxLabel);
		int* iIndex;
		iIndex = new int[objLocal.m_uiMaxLabel];

		for(int iID=0; iID<=objLocal.m_iMaxID; iID++)
		{
			if(objLocal.runLength[iID].uiLabel==0){continue;}
			uiFeatures[objLocal.runLength[iID].uiLabel-1] += objLocal.runLength[iID].iCEnd-objLocal.runLength[iID].iCStart+1;
		}
		Index(uiFeatures,objLocal.m_uiMaxLabel,iIndex);

		if(sAscDsc.CompareNoCase(_T("Asc"))==0)
		{
			for(int iID=0; iID<=objLocal.m_iMaxID; iID++)
			{
				if(objLocal.runLength[iID].uiLabel==0){continue;}
				objLocal.runLength[iID].uiLabel = iIndex[objLocal.runLength[iID].uiLabel]+1;
			}
		}
		if(sAscDsc.CompareNoCase(_T("Dsc"))==0)
		{
			for(int iID=0; iID<=objLocal.m_iMaxID; iID++)
			{
				if(objLocal.runLength[iID].uiLabel==0){continue;}
				objLocal.runLength[iID].uiLabel = objLocal.m_iMaxID - iIndex[objLocal.runLength[iID].uiLabel];
			}
		}

		objLocal.ReCheckID();
		objOut->Copy(&objLocal);
		SAFE_DELETE(iIndex);
		SAFE_DELETE(uiFeatures);
		SAFE_DELETE(dFeatures);
		return TRUE;
	}

	if((sMode.CompareNoCase(_T("row"))==0) || (sMode.CompareNoCase(_T("column"))==0))
	{
		int* iIndex;
		iIndex = new int[objLocal.m_uiMaxLabel];

		double* dAs;
		double* dRs;
		double* dCs;
		dAs=new double[objLocal.m_uiMaxLabel+1];
		dRs=new double[objLocal.m_uiMaxLabel+1];
		dCs=new double[objLocal.m_uiMaxLabel+1];
		AreaCenter(&objLocal, dAs,dRs,dCs,objLocal.m_uiMaxLabel+1);
		if(sMode.CompareNoCase(_T("row"))==0)
		{
			Index(&(dRs[1]),objLocal.m_uiMaxLabel,iIndex);
		}
		if(sMode.CompareNoCase(_T("column"))==0)
		{
			Index(&(dCs[1]),objLocal.m_uiMaxLabel,iIndex);
		}
		SAFE_DELETE(dAs);
		SAFE_DELETE(dRs);
		SAFE_DELETE(dCs);
		if(sAscDsc.CompareNoCase(_T("Asc"))==0)
		{	
			for(int iID=0; iID<=objLocal.m_iMaxID; iID++)
			{
				if(objLocal.runLength[iID].uiLabel==0){continue;}
				objLocal.runLength[iID].uiLabel = iIndex[objLocal.runLength[iID].uiLabel-1]+1;
			}
		}
		if(sAscDsc.CompareNoCase(_T("Dsc"))==0)
		{	
			for(int iID=0; iID<=objLocal.m_iMaxID; iID++)
			{
				if(objLocal.runLength[iID].uiLabel==0){continue;}
				objLocal.runLength[iID].uiLabel = objLocal.m_uiMaxLabel - iIndex[objLocal.runLength[iID].uiLabel-1];
			}
		}
		SAFE_DELETE(iIndex);
		objLocal.ReCheckID();
		objOut->Copy(&objLocal);
	}
	return TRUE;
}

BOOL AreaCenter(Object* obj, double* dArea, double* dR, double* dC)
{
	int iArea=0;
	UINT uiRSum=0;
	UINT uiCSum=0;
	for(int iID=0; iID<=obj->m_iMaxID; iID++)
	{
		int iAreaTemp;
		iAreaTemp = obj->runLength[iID].iCEnd - obj->runLength[iID].iCStart+1;
		iArea+=iAreaTemp;
		uiCSum+=(obj->runLength[iID].iCEnd + obj->runLength[iID].iCStart)/(2.0) * iAreaTemp;
		uiRSum+=(obj->runLength[iID].iR) * iAreaTemp;
	}
	*dArea=double(iArea);
	*dR=uiRSum/(*dArea);
	*dC=uiCSum/(*dArea);
	return TRUE;
}

BOOL AreaCenter(Object* obj, double* dAreas, double* dRs, double* dCs, int iLength)
{
	if(iLength<obj->m_uiMaxLabel+1){return FALSE;}

	int* iAreas;
	UINT* uiRSums;
	UINT* uiCSums;

	iAreas = new int[obj->m_uiMaxLabel+1];
	uiRSums = new UINT[obj->m_uiMaxLabel+1];
	uiCSums = new UINT[obj->m_uiMaxLabel+1];

	for(int i=0; i<=obj->m_uiMaxLabel; i++)
	{
		iAreas[i]=0;
		uiRSums[i]=0;
		uiCSums[i]=0;
	}
	for(int iID=0; iID<=obj->m_iMaxID; iID++)
	{
		int iAreaTemp;
		iAreaTemp = obj->runLength[iID].iCEnd - obj->runLength[iID].iCStart+1;
		iAreas[obj->runLength[iID].uiLabel]+=iAreaTemp;
		uiCSums[obj->runLength[iID].uiLabel]+=(obj->runLength[iID].iCEnd + obj->runLength[iID].iCStart)/(2.0) * iAreaTemp;
		uiRSums[obj->runLength[iID].uiLabel]+=(obj->runLength[iID].iR) * iAreaTemp;
	}
	for(int i=0; i<=obj->m_uiMaxLabel; i++)
	{
		if(iAreas[i]==0){dAreas[i]=0; dRs[i]=0; dCs[i]=0; continue;}
		dAreas[i]=double(iAreas[i]);
		dRs[i]=uiRSums[i]/(dAreas[i]);
		dCs[i]=uiCSums[i]/(dAreas[i]);
	}
	SAFE_DELETE(uiRSums);
	SAFE_DELETE(iAreas);
	SAFE_DELETE(uiCSums);
	return TRUE;
}
BOOL ReadImage(CString sFilePath, ImgRGB* imgRGB)
{
	return imgRGB->Assign(sFilePath);
}

BOOL WriteImage(const ImgRGB* imgRGB, CString sFilePath)
{
	BITMAPINFOHEADER bmih;
	BOOL bRet;

	CFile f;
	BITMAPFILEHEADER bmfh;



	int iFillerSize;

	if(((imgRGB->iWidth*3)%4)==0)
	{
		iFillerSize = 0;
	}
	else
	{
		iFillerSize = (4-((imgRGB->iWidth*3)%4));
	}


	int iBitSize;

	iBitSize = ((imgRGB->iWidth*3)+iFillerSize)*(imgRGB->iHeight);

	bmfh.bfType=0x4d42;
	bmfh.bfSize =  0x36+iBitSize;
	bmfh.bfOffBits = 0x36;
	bmfh.bfReserved1=0;
	bmfh.bfReserved2 = 0;

	bmih.biSize=0x00000028;
	bmih.biWidth=imgRGB->iWidth;
	bmih.biHeight=imgRGB->iHeight;
	bmih.biPlanes=1;
	bmih.biBitCount=24;
	bmih.biCompression=0;
	bmih.biSizeImage=iBitSize;
	bmih.biXPelsPerMeter=0;
	bmih.biYPelsPerMeter=0;
	bmih.biClrUsed=0;
	bmih.biClrImportant	=0;


	bRet = f.Open(sFilePath, CFile::modeCreate|CFile::modeWrite);
	if(bRet != TRUE){return FALSE;}


	f.Write((BYTE*)&bmfh,sizeof(bmfh));
	f.Write((BYTE*)&bmih,sizeof(bmih));


	BYTE* byOutBuf;

	byOutBuf = new BYTE[iBitSize];

	if(imgRGB->iChannel==CHANNEL_3_FLOAT)
	{
		for(int r=0; r<imgRGB->iHeight; r++)
		{
			for(int c=0; c<imgRGB->iWidth; c++)
			{
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+0] = CAP_BYTE(imgRGB->dImg1[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c]*(256/360.0));
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+1] = CAP_BYTE(imgRGB->dImg2[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c]*255);
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+2] = CAP_BYTE(imgRGB->dImg3[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c]);
			}
		}
	}
	if(imgRGB->iChannel==CHANNEL_3_8RGB)
	{
		for(int r=0; r<imgRGB->iHeight; r++)
		{
			for(int c=0; c<imgRGB->iWidth; c++)
			{
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+0] = imgRGB->byImgB[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c];
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+1] = imgRGB->byImgG[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c];
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+2] = imgRGB->byImgR[(imgRGB->iHeight-r-1)*imgRGB->iWidth+c];
			}
		}
	}

	if(imgRGB->iChannel==CHANNEL_1_24BGR)
	{
		for(int r=0; r<imgRGB->iHeight; r++)
		{
			for(int c=0; c<imgRGB->iWidth; c++)
			{
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+0] = imgRGB->byImg[3*((imgRGB->iHeight-r-1)*imgRGB->iWidth+c)+0];
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+1] = imgRGB->byImg[3*((imgRGB->iHeight-r-1)*imgRGB->iWidth+c)+1];
				byOutBuf[3*(r*imgRGB->iWidth+c)+r*iFillerSize+2] = imgRGB->byImg[3*((imgRGB->iHeight-r-1)*imgRGB->iWidth+c)+2];
			}
		}
	}


	f.Write((void*)(byOutBuf),iBitSize);
	f.Close();
	SAFE_DELETE(byOutBuf);

	return TRUE;
}


/*
BOOL Search()
{
int* iMap;
iSearchWidth = imgTarget->iWidth - imgModel->iWidth +1;
iSearchHeight = imgTarget->iHeight - imgModel->iHeight+1;
if(iSearchWidth<=0){return FALSE;}
if(iSearchHeight<=0){return FALSE;}
iMap = new int[iSearchWidth * iSearchHeight];
for(int r = 0; r<iSearchHeight; r++)
{
for(int c=0;c<iSearchWidth;c++){}
}
}
*/

