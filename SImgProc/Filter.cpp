#include "stdafx.h"
#include "Filter.h"




inline BOOL UpdateSumRDirection
	(
	const BYTE* byImage, const int iImgWidth, const int iImgHeight,
	const int r, 
	const int iC0, const int iC1,
	const int iHalfHeight,
	UINT* uiSumOfEachC, int* iTotalHeight, const BOOL bFirstRow=FALSE)
{
	if(byImage == NULL){return FALSE;}
	if(uiSumOfEachC == NULL){return FALSE;}
	if(iTotalHeight == NULL){return FALSE;}

	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	if((r==0) || (bFirstRow==TRUE))
	{
		int iStartROffset = -1*min(r, iHalfHeight);
		for(int c=iC0Local; c<=iC1Local; c++)
		{
			uiSumOfEachC[c] = byImage[(r+iStartROffset)*iImgWidth+c];
			for(int rOffset = iStartROffset+1; rOffset<=iHalfHeight; rOffset++)
			{
				uiSumOfEachC[c] += byImage[(r+rOffset)*iImgWidth+c];
			}
		}

		*iTotalHeight = iHalfHeight-iStartROffset+1;
		return TRUE;
	}

	if(r<iHalfHeight+1)
	{
		for(int c=iC0Local; c <= iC1Local; c++)
		{
			uiSumOfEachC[c]+=byImage[(r+iHalfHeight)*iImgWidth+c];
		}
		*iTotalHeight = r+iHalfHeight+1;

		return TRUE;
	}

	if(r < iImgHeight-iHalfHeight)
	{
		for(int c=iC0Local; c<=iC1Local; c++)
		{
			uiSumOfEachC[c] -= byImage[(r-iHalfHeight-1)*iImgWidth+c];
			uiSumOfEachC[c] += byImage[(r+iHalfHeight)*iImgWidth+c];
		}
		*iTotalHeight = 2*iHalfHeight+1;

		return TRUE;
	}

	for(int c = iC0Local; c <= iC1Local; c++)
	{
		uiSumOfEachC[c] -= byImage[(r-iHalfHeight-1)*iImgWidth+c];
	}

	*iTotalHeight = iImgHeight-r +iHalfHeight;
	return TRUE;
}

inline BOOL UpdateMaxRDirection
	(
	const BYTE* byImage, const int iImgWidth, const int iImgHeight,
	const int r, 
	const int iC0, const int iC1,
	const int iHalfHeight,
	BYTE* byMaxOfEachC, const BOOL bFirstRow=FALSE)
{
	if(byImage == NULL){return FALSE;}
	if(byMaxOfEachC == NULL){return FALSE;}

	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	if((r==0) || (bFirstRow==TRUE))
	{
		int iStartROffset = -1*min(r, iHalfHeight);
		for(int c = iC0Local; c <= iC1Local; c++)
		{
			byMaxOfEachC[c] = byImage[(r+iStartROffset)*iImgWidth+c];
			for(int rOffset = iStartROffset+1; rOffset<=iHalfHeight; rOffset++)
			{
				byMaxOfEachC[c] = MAX(byMaxOfEachC[c], byImage[(r+rOffset)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	if(r < iHalfHeight+1)//r = 1, 2, 3
	{
		for(int c = iC0Local; c <= iC1Local; c++)
		{
			byMaxOfEachC[c] = MAX(byMaxOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
		}
		return TRUE;
	}

	if(r < iImgHeight-iHalfHeight)//r = 4, 5, 6, ..., 15, 16
	{
		for(int c = iC0Local; c <= iC1Local; c++)
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

			byMaxOfEachC[c] = byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rOffset = -iHalfHeight+1; rOffset <= iHalfHeight; rOffset++)
			{
				byMaxOfEachC[c] = MAX(byMaxOfEachC[c], byImage[(r+rOffset)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	for(int c = iC0Local; c <= iC1Local; c++)//r = 17, 18, 19
	{
		if(byMaxOfEachC[c] <= byImage[(r-iHalfHeight-1)*iImgWidth+c])//r-iHalfHeight-1 = 13, 14, 15
		{
			byMaxOfEachC[c] = byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rOffset = -iHalfHeight+1; rOffset < iImgHeight - r; rOffset++)
			{
				byMaxOfEachC[c] = MAX(byMaxOfEachC[c], byImage[(r+rOffset)*iImgWidth+c]);
			}
		}
	}
	return TRUE;
}
inline BOOL UpdateMinRDirection
	(
	const BYTE* byImage, const int iImgWidth, const int iImgHeight,
	const int r, 
	const int iC0, const int iC1,
	const int iHalfHeight,
	BYTE* byMinOfEachC, const BOOL bFirstRow=FALSE)
{
	if(byImage == NULL){return FALSE;}
	if(byMinOfEachC == NULL){return FALSE;}
	
	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	if((r==0) || (bFirstRow==TRUE))
	{
		int iStartROffset = -1*min(r, iHalfHeight);
		for(int c = iC0Local; c <= iC1Local; c++)
		{
			byMinOfEachC[c]=byImage[(r+iStartROffset)*iImgWidth+c];
			for(int rOffset = iStartROffset+1; rOffset<=iHalfHeight; rOffset++)
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+rOffset)*iImgWidth+c]);
			}
		}
		return TRUE;
	}

	if(r < iHalfHeight+1)
	{
		for(int c = iC0Local; c <= iC1Local; c++)
		{
			byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
		}
		return TRUE;
	}

	if(r<iImgHeight-iHalfHeight)
	{
		for(int c = iC0Local; c <= iC1Local; c++)
		{
			if(byMinOfEachC[c] < byImage[(r-iHalfHeight-1)*iImgWidth+c])
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+iHalfHeight)*iImgWidth+c]);
				continue;
			}
			if(byImage[(r-iHalfHeight-1)*iImgWidth+c] >= byImage[(r+iHalfHeight)*iImgWidth+c])
			{
				byMinOfEachC[c]=byImage[(r+iHalfHeight)*iImgWidth+c];
				continue;
			}

			byMinOfEachC[c] = byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rLocal = -iHalfHeight+1; rLocal <= iHalfHeight; rLocal++)
			{
				byMinOfEachC[c] = MIN(byMinOfEachC[c], byImage[(r+rLocal)*iImgWidth+c]);
			}
		}
		return TRUE;
	}
	
	for(int c = iC0Local; c <= iC1Local; c++)
	{
		if(byMinOfEachC[c] >= byImage[(r-iHalfHeight-1)*iImgWidth+c])
		{
			byMinOfEachC[c] = byImage[(r-iHalfHeight)*iImgWidth+c];
			for(int rOffset = -iHalfHeight+1; rOffset< iImgHeight-r; rOffset++)
			{
				byMinOfEachC[c]=MIN(byMinOfEachC[c], byImage[(r+rOffset)*iImgWidth+c]);
			}
		}
	}
	return TRUE;
}

BOOL SumCDirection
	(
	UINT* uiSumOfEachC,
	int iImgWidth,
	const int iC0, const int iC1,
	int iHalfWidth,
	UINT* uiSumOfRC, int* iTotalCs)
{
	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	int iStartCOffset = -1*min(iC0Local, iHalfWidth);
	int iEndCOffset = min(iImgWidth-1-iC0Local, iHalfWidth);

	uiSumOfRC[iC0Local] = uiSumOfEachC[iC0Local];
	for(int cOffset = iStartCOffset+1; cOffset <= iEndCOffset; cOffset++)
	{
		uiSumOfRC[iC0Local] += uiSumOfEachC[iC0Local+cOffset];
	}
	iTotalCs[iC0Local]=iEndCOffset - iStartCOffset + 1;

	for(int c = iC0Local+1; c <= iC1Local; c++)
	{
		if(c-iHalfWidth < 0)
		{
			uiSumOfRC[c]=uiSumOfRC[c-1]+uiSumOfEachC[c+iHalfWidth];
			iTotalCs[c]=iTotalCs[c-1]+1;
			continue;
		}

		if(c+iHalfWidth <= iImgWidth-1)
		{
			uiSumOfRC[c]=uiSumOfRC[c-1]-uiSumOfEachC[c-iHalfWidth];
			uiSumOfRC[c]+=uiSumOfEachC[c+iHalfWidth];
			iTotalCs[c]=2*iHalfWidth+1;
			continue;
		}

		uiSumOfRC[c]=uiSumOfRC[c]-uiSumOfEachC[c+iHalfWidth];
		iTotalCs[c]=iTotalCs[c-1]-1;
	}

	return TRUE;
}

inline BOOL MaxCDirection
	(
	const BYTE* byMaxOfEachC,
	const int iImgWidth,
	const int iC0, const int iC1,
	const int iHalfWidth,
	BYTE* byMaxOfRC)
{
	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	int iStartCOffset = -1*min(iC0Local, iHalfWidth);
	int iEndCOffset = min(iImgWidth-1-iC0Local, iHalfWidth);

	for(int i = 0; i<iImgWidth; i++){byMaxOfRC[i] = 0;}

	byMaxOfRC[iC0Local] = byMaxOfEachC[iC0Local];
	for(int cOffset = iStartCOffset+1; cOffset <= iEndCOffset; cOffset++)
	{
		byMaxOfRC[iC0Local] = MAX(byMaxOfRC[iC0Local], byMaxOfEachC[iC0Local+cOffset]);
	}


	for(int c = iC0Local+1; c <= iC1Local; c++)
	{
		if(c-iHalfWidth < 0)
		{
			byMaxOfRC[c] = MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
			continue;
		}

		if(c+iHalfWidth <= iImgWidth-1)
		{
			if(byMaxOfRC[c-1] > byMaxOfEachC[c-iHalfWidth-1])
			{
				byMaxOfRC[c] = MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
				continue;
			}
			if(byMaxOfEachC[c-iHalfWidth-1] < byMaxOfEachC[c+iHalfWidth])
			{
				byMaxOfRC[c] = MAX(byMaxOfRC[c-1], byMaxOfEachC[c+iHalfWidth]);
				continue;
			}

			byMaxOfRC[c] = byMaxOfEachC[c-iHalfWidth];
			for(int cOffset = -iHalfWidth+1; cOffset<= iHalfWidth; cOffset++)
			{
				byMaxOfRC[c] = MAX(byMaxOfRC[c], byMaxOfEachC[c + cOffset]);
			}
			continue;
		}

		if(byMaxOfRC[c-1] > byMaxOfEachC[c-iHalfWidth-1])
		{
			byMaxOfRC[c] = byMaxOfRC[c-1];
			continue;
		}

		byMaxOfRC[c] = byMaxOfEachC[c-iHalfWidth];
		for(int cOffset = -iHalfWidth+1; cOffset < iImgWidth-c; cOffset++)
		{
			byMaxOfRC[c] = MAX(byMaxOfRC[c], byMaxOfEachC[c + cOffset]);
		}
	}

	return TRUE;
}


BOOL MinCDirection
	(
	const BYTE* byMinOfEachC,
	const int iImgWidth,
	const int iC0, const int iC1,
	const int iHalfWidth,
	BYTE* byMinOfRC)
{
	int iC0Local = max(0, iC0);
	int iC1Local = min(iImgWidth-1, iC1);

	int iStartCOffset = -1*min(iC0Local, iHalfWidth);
	int iEndCOffset = min(iImgWidth-1-iC0Local, iHalfWidth);

	for(int i = 0; i<iImgWidth; i++){byMinOfRC[i] = 0;}

	byMinOfRC[iC0Local] = byMinOfEachC[iC0Local];
	for(int cOffset=iStartCOffset+1; cOffset <= iEndCOffset; cOffset++)
	{
		byMinOfRC[iC0Local]=MIN(byMinOfRC[iC0Local], byMinOfEachC[iC0Local+cOffset]);
	}
	
	for(int c = iC0Local+1; c <= iC1Local; c++)
	{
		if(c-iHalfWidth < 0)
		{
			byMinOfRC[c] = MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);
			continue;
		}

		if(c+iHalfWidth <= iImgWidth-1)
		{
			if(byMinOfRC[c-1] < byMinOfEachC[c-iHalfWidth-1])
			{
				byMinOfRC[c]= MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);
				continue;
			}
			if(byMinOfEachC[c-iHalfWidth-1] > byMinOfEachC[c+iHalfWidth])
			{
				byMinOfRC[c]=MIN(byMinOfRC[c-1], byMinOfEachC[c+iHalfWidth]);
				continue;
			}

			byMinOfRC[c]=byMinOfEachC[c-iHalfWidth];
			for(int cOffset = -iHalfWidth; cOffset <= iHalfWidth; cOffset++)
			{
				byMinOfRC[c] = MIN(byMinOfRC[c], byMinOfEachC[c+cOffset]);
			}
			continue;
		}

		if(byMinOfRC[c-1] < byMinOfEachC[c-iHalfWidth-1])
		{
			byMinOfRC[c]=byMinOfRC[c-1];
			continue;
		}

		byMinOfRC[c] = byMinOfEachC[c-iHalfWidth];
		for(int cOffset = -iHalfWidth; cOffset < iImgWidth-c; cOffset++)
		{
			byMinOfRC[c] = MIN(byMinOfRC[c], byMinOfEachC[c+cOffset]);
		}
	}

	return TRUE;
}

BOOL DLL_IE MeanImage(const ImgRGB* imgIn, ImgRGB* imgResult, const int iR0, const int iC0, const int iR1, const int iC1, const int iFilterWidth, const int iFilterHeight)
{
	int iImgWidth=imgIn->iWidth;
	int iImgHeight=imgIn->iHeight;
	int iHalfHeight=(iFilterHeight-1)/2;
	int iHalfWidth=(iFilterWidth-1)/2;

	int iStartR=max(0, iR0-iHalfHeight);
	int iStartC=max(0, iC0-iHalfWidth);
	int iEndR=min(iImgHeight-1, iR1+iHalfHeight);
	int iEndC=min(iImgWidth-1, iC1+iHalfWidth);

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
		
		if(iStartR != 0)
		{
			UpdateSumRDirection(imgR1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_R, &iTotalHeight_R);
			UpdateSumRDirection(imgG1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_G, &iTotalHeight_G);
			UpdateSumRDirection(imgB1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_B, &iTotalHeight_B);
		}
		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateSumRDirection(imgR1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_R, &iTotalHeight_R);
			UpdateSumRDirection(imgG1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_G, &iTotalHeight_G);
			UpdateSumRDirection(imgB1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_B, &iTotalHeight_B);

			SumCDirection(uiFilteredOfEachC_R, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_R, iTotalCs_R);
			SumCDirection(uiFilteredOfEachC_G, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_G, iTotalCs_G);
			SumCDirection(uiFilteredOfEachC_B, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_B, iTotalCs_B);
			for(int c=iStartC; c<=iEndC; c++)
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
		
		if(iStartR != 0)
		{
			UpdateSumRDirection(imgIn->byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC, &iTotalHeight, TRUE);
		}

		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateSumRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC, &iTotalHeight);

			SumCDirection(uiFilteredOfEachC, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC, iTotalCs);
			for(int c=iStartC; c<=iEndC; c++)
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

BOOL DLL_IE MaxImage(const ImgRGB* imgIn, ImgRGB* imgResult, const int iR0, const int iC0, const int iR1, const int iC1, const int iFilterWidth, const int iFilterHeight)
{
	int iImgWidth = imgIn->iWidth;
	int iImgHeight = imgIn->iHeight;
	int iHalfHeight = (iFilterHeight-1)/2;
	int iHalfWidth = (iFilterWidth-1)/2;

	int iStartR = max(0, iR0-iHalfHeight);
	int iStartC = max(0, iC0-iHalfWidth);
	int iEndR = min(iImgHeight-1, iR1+iHalfHeight);
	int iEndC = min(iImgWidth-1, iC1+iHalfWidth);

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

		if(iStartR != 0)
		{
			UpdateMaxRDirection(imgR1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_R, TRUE);
			UpdateMaxRDirection(imgG1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_G, TRUE);
			UpdateMaxRDirection(imgB1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_B, TRUE);
		}
		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateMaxRDirection(imgR1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_R);
			UpdateMaxRDirection(imgG1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_G);
			UpdateMaxRDirection(imgB1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC_B);

			MaxCDirection(uiFilteredOfEachC_R, iImgWidth, iStartC, iEndC, iHalfWidth, uiFilteredOfRC_R);
			MaxCDirection(uiFilteredOfEachC_G, iImgWidth, iStartC, iEndC, iHalfWidth, uiFilteredOfRC_G);
			MaxCDirection(uiFilteredOfEachC_B, iImgWidth, iStartC, iEndC, iHalfWidth, uiFilteredOfRC_B);
			for(int c=iStartC; c<=iEndC; c++)
			{
				imgR2.byImg[r*iImgWidth+c]=uiFilteredOfRC_R[c];
				imgG2.byImg[r*iImgWidth+c]=uiFilteredOfRC_G[c];
				imgB2.byImg[r*iImgWidth+c]=uiFilteredOfRC_B[c];
			}
		}
		Compose3(imgResult, &imgR2, &imgG2, &imgB2);
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

		imgResult->Set(iImgWidth, iImgHeight, CHANNEL_1_8);

		if(iStartR != 0)
		{
			UpdateMaxRDirection(imgIn->byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC, TRUE);
		}

		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateMaxRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC);

			MaxCDirection(uiFilteredOfEachC, iImgWidth, iStartC, iEndC, iHalfWidth, uiFilteredOfRC);
			for(int c=iStartC; c<=iEndC; c++)
			{
				imgResult->byImg[r*iImgWidth+c]=uiFilteredOfRC[c];
			}
		}
		SAFE_DELETE( uiFilteredOfRC);
		SAFE_DELETE( uiFilteredOfEachC);
		return TRUE;
	}

	return FALSE;
}	

BOOL DLL_IE MinImage(const ImgRGB* imgIn, ImgRGB* imgResult, const int iR0, const int iC0, const int iR1, const int iC1, const int iFilterWidth, const int iFilterHeight)
{
	int iImgWidth=imgIn->iWidth;
	int iImgHeight=imgIn->iHeight;
	int iHalfHeight=(iFilterHeight-1)/2;
	int iHalfWidth=(iFilterWidth-1)/2;

	int iStartR=max(0, iR0-iHalfHeight);
	int iStartC=max(0, iC0-iHalfWidth);
	int iEndR=min(iImgHeight-1, iR1+iHalfHeight);
	int iEndC=min(iImgWidth-1, iC1+iHalfWidth);

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
		
		if(iStartR != 0)
		{
			UpdateMinRDirection(imgR1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_R);
			UpdateMinRDirection(imgG1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_G);
			UpdateMinRDirection(imgB1.byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_B);
		}
		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateMinRDirection(imgR1.byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_R);
			UpdateMinRDirection(imgG1.byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_G);
			UpdateMinRDirection(imgB1.byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC_B);

			MinCDirection(uiFilteredOfEachC_R, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_R);
			MinCDirection(uiFilteredOfEachC_G, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_G);
			MinCDirection(uiFilteredOfEachC_B, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC_B);
			for(int c=iStartC; c<=iEndC; c++)
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

		imgResult->Set(iImgWidth, iImgHeight, CHANNEL_1_8);
		
		if(iStartR != 0)
		{
			UpdateMinRDirection(imgIn->byImg, iImgWidth, iImgHeight, iStartR-1, iStartC, iEndC, iHalfHeight, uiFilteredOfEachC, TRUE);
		}

		for(int r=iStartR; r<=iEndR; r++)
		{
			UpdateMinRDirection(imgIn->byImg, iImgWidth, iImgHeight, r, iStartC, iEndC, (iFilterHeight-1)/2, uiFilteredOfEachC);

			MinCDirection(uiFilteredOfEachC, iImgWidth, iStartC, iEndC, (iFilterWidth-1)/2, uiFilteredOfRC);
			for(int c=iStartC; c<=iEndC; c++)
			{
				imgResult->byImg[r*iImgWidth+c]=uiFilteredOfRC[c];
			}
		}
		SAFE_DELETE( uiFilteredOfRC);
		SAFE_DELETE( uiFilteredOfEachC);
		return TRUE;
	}

	return FALSE;
}