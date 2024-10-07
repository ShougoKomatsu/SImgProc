
#pragma once
#include "SImgProc_ex.h"

#define CAP_BYTE(arg) (arg<0 ? 0 : (arg>255 ? 255 : BYTE(arg)))

struct RunLength
{
	int iR;
	int iCStart;
	int iCEnd;
	UINT uiLabel;
	BOOL bValid;
	BOOL bIsConnectionOperated;
	void Set(int iRIn, int iCStartIn, int iCEndIn, UINT uiLabelIn, BOOL bValidIn)
	{
		iR=iRIn;
		iCStart=iCStartIn;
		iCEnd=iCEndIn;
		uiLabel=uiLabelIn;
		bValid=bValidIn;
		bIsConnectionOperated=FALSE;
	}
	RunLength(){iR=0; iCStart=0; iCEnd=0; uiLabel=0; bValid=FALSE;}
};

struct Object
{
	int m_iMaxID;
	int m_iBufNum;
	RunLength* runLength;

	Object()
	{
		runLength=NULL;
		Init();
	}
	BOOL Init()
	{
		if(runLength != NULL){delete [] runLength; runLength=NULL;}
		m_iMaxID=-1;
		m_iBufNum=0;
		return TRUE;
	}

	BOOL Alloc(int iBuf)
	{
		Init();
		runLength=new RunLength[iBuf];
		m_iBufNum=iBuf;
		return TRUE;
	}

	BOOL Expand(int iBuf)
	{
		if(iBuf==1)
		{
			
		this->Init();
		this->Alloc(iBuf);
		return TRUE;
		}
		Object objTemp;
		objTemp.Alloc(this->m_iMaxID+1);
		objTemp.Copy(this);

		this->Init();
		this->Alloc(iBuf);
		this->Copy(&objTemp);
		return TRUE;

	}

	BOOL Copy(Object* objSrc)
	{
		if(objSrc->m_iMaxID<0){return FALSE;}

		if(objSrc->m_iMaxID +1 >= this->m_iBufNum)
		{
			this->Alloc(objSrc->m_iMaxID +1);
		}
		this->m_iMaxID=-1;
		for(int iID=0; iID<=objSrc->m_iMaxID; iID++)
		{
			this->runLength[iID].Set(objSrc->runLength[iID].iR, objSrc->runLength[iID].iCStart, objSrc->runLength[iID].iCEnd, objSrc->runLength[iID].uiLabel, objSrc->runLength[iID].bValid);
			this->m_iMaxID++;
		}
		return TRUE;

	}

	BOOL Add(int iR, int iCStart, int iCEnd, UINT uiLabel)
	{
		if(m_iMaxID+1>=m_iBufNum)
		{
			if(m_iBufNum==0)
			{
				Expand(1);
			}
			else
			{
				Expand(m_iBufNum*2);
			}
		}
		m_iMaxID++;
		this->runLength[m_iMaxID].Set(iR, iCStart, iCEnd, uiLabel,TRUE);
		return TRUE;

	}


	BOOL IsNeighbor(RunLength* runLength1, RunLength* runLength2, int iNeighborPolicy)
	{
		if(runLength2->iR<runLength1->iR-1){return FALSE;}
		if(runLength2->iR>runLength1->iR+1){return FALSE;}

		int iExtention;
		if(iNeighborPolicy==4){iExtention=0;}
		else if(iNeighborPolicy==8){iExtention=1;}
		else{return FALSE;}

		BOOL bIsNeighbor=FALSE;
		if((runLength2->iCStart - iExtention <= runLength1->iCStart) && (runLength1->iCStart <= runLength2->iCEnd + iExtention))
		{
			return TRUE;
		}
		if((runLength2->iCStart - iExtention <= runLength1->iCEnd) && (runLength1->iCEnd <= runLength2->iCEnd+iExtention))
		{
			return TRUE;
		}

		if((runLength1->iCStart < runLength2->iCStart) && (runLength2->iCEnd < runLength1->iCEnd))
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL ConnectNeighbor(RunLength* runLength, int iID, int iNeighborPolicy)
	{
		if(runLength->bValid==FALSE){return FALSE;}
		int iSelfR=runLength->iR;
		
		for(int i=0; i<=this->m_iMaxID; i++)
		{
			if(this->runLength[i].bValid==FALSE){continue;}
			
			if(this->runLength[i].bIsConnectionOperated==TRUE){continue;}
			if(i==iID){continue;}

			if(IsNeighbor(runLength, &(this->runLength[i]), iNeighborPolicy)==TRUE)
			{

				this->runLength[i].bIsConnectionOperated=TRUE;
				this->runLength[i].uiLabel=runLength->uiLabel;
				ConnectNeighbor(&(this->runLength[i]), i, iNeighborPolicy);
				continue;
			}

			if(this->runLength[i].iR>iSelfR+1){break;}
		}
		return TRUE;
	}

	BOOL Connection(int iNeighborPolicy)
	{
		UINT uiMaxLabel=0;

		for(int iID=0; iID<=m_iMaxID; iID++)
		{
			if(uiMaxLabel<this->runLength[iID].uiLabel)
			{
				uiMaxLabel=this->runLength[iID].uiLabel;
				uiMaxLabel=this->runLength[iID].bIsConnectionOperated=FALSE;
			}
		}

		for(int iID=0; iID<=m_iMaxID; iID++)
		{
			if(this->runLength[iID].uiLabel == 0)
			{
				uiMaxLabel++;
				this->runLength[iID].uiLabel=uiMaxLabel;
				this->runLength[iID].bIsConnectionOperated=TRUE;
				ConnectNeighbor(&(this->runLength[iID]),iID,iNeighborPolicy);
			}
		}

		return TRUE;
	}

	BOOL Truncate()
	{
		UINT uiMaxLabel=0;

		for(int iID=0; iID<=m_iMaxID; iID++)
		{
			if(uiMaxLabel<this->runLength[iID].uiLabel)
			{
				uiMaxLabel=this->runLength[iID].uiLabel;
			}
		}
		UINT* uiNewLabelTable;
		uiNewLabelTable = new UINT[uiMaxLabel+1];

		memset(uiNewLabelTable, 0, sizeof(UINT)*(uiMaxLabel+1));

		UINT uiNewMaxLabel=0;
		for(int iID=0; iID<=m_iMaxID; iID++)
		{
			if(this->runLength[iID].uiLabel==0){continue;}
			if(uiNewLabelTable[this->runLength[iID].uiLabel]!=0){continue;}
			
			uiNewMaxLabel++;
			uiNewLabelTable[this->runLength[iID].uiLabel]=uiNewMaxLabel;
		}

		for(int iID=0; iID<=m_iMaxID; iID++)
		{
			this->runLength[iID].uiLabel=uiNewLabelTable[this->runLength[iID].uiLabel];
		}

		delete [] uiNewLabelTable;
		return TRUE;

	}
};

BOOL Threshold(ImgRGB* imgIn, BYTE byThreshMin, BYTE byThreshMax, Object* ObjOut);