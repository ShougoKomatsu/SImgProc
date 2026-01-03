#include "stdafx.h"
#include "SImgProc_ex.h"

void RunLength::Set(const int iRIn, const int iCStartIn, const int iCEndIn, const UINT uiLabelIn, const BOOL bValidIn)
{
	iR=iRIn;
	iCStart=iCStartIn;
	iCEnd=iCEndIn;
	uiLabel=uiLabelIn;
	bValid=bValidIn;
	bIsConnectionOperated=FALSE;
}

RunLength::RunLength(){iR=0; iCStart=0; iCEnd=0; uiLabel=0; bValid=FALSE;}


void RunLength::Copy(const RunLength* runLengthIn)
{
	if(runLengthIn==this){return;}

	this->Set(runLengthIn->iR,runLengthIn->iCStart, runLengthIn->iCEnd, runLengthIn->uiLabel, runLengthIn->bValid);
}

BOOL IsOverWrapped(const RunLength* runLength1, const RunLength* runLength2)
{

	if(runLength1->iR != runLength2->iR){return FALSE;}

	if(runLength1->bValid != TRUE){return FALSE;}
	if(runLength2->bValid != TRUE){return FALSE;}

	if(runLength1->uiLabel != runLength2->uiLabel){return FALSE;}

	if((runLength2->iCStart <= runLength1->iCStart) && (runLength1->iCStart <= runLength2->iCEnd))
	{
		return TRUE;
	}
	if((runLength2->iCStart  <= runLength1->iCEnd) && (runLength1->iCEnd <= runLength2->iCEnd))
	{
		return TRUE;
	}

	if((runLength1->iCStart < runLength2->iCStart) && (runLength2->iCEnd < runLength1->iCEnd))
	{
		return TRUE;
	}

	return FALSE;
}
