#include "stdafx.h"
#include "SImgProc_ex.h"
#include "RunLength.h"
#include "CommonFunction.h"

Object::Object()
{
	runLength=NULL;
	Init();
}
BOOL Object::Init()
{
	if(runLength != NULL){delete [] runLength; runLength=NULL;}
	m_iMaxID=-1;
	m_iBufNum=0;
	m_uiMaxLabel=0;
	return TRUE;
}

BOOL Object::Alloc(int iBuf)
{
	Init();
	runLength=new RunLength[iBuf];
	m_iBufNum=iBuf;
	return TRUE;
}

BOOL Object::Expand(int iBuf)
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

BOOL Object::Copy(Object* objSrc)
{
	if(objSrc==this){return TRUE;}

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
		this->m_uiMaxLabel=objSrc->m_uiMaxLabel;
	}
	return TRUE;

}

BOOL Object::Add(int iR, int iCStart, int iCEnd, UINT uiLabel)
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


BOOL Object::IsNeighbor(RunLength* runLength1, RunLength* runLength2, int iNeighborPolicy)
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

BOOL Object::ConnectNeighbor(RunLength* runLength, int iID, int iNeighborPolicy)
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

BOOL Object::Connection(int iNeighborPolicy)
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
	this->m_uiMaxLabel=uiMaxLabel;
	return TRUE;
}


BOOL Object::Truncate()
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

	m_uiMaxLabel=uiNewMaxLabel;
	for(int iID=0; iID<=m_iMaxID; iID++)
	{
		this->runLength[iID].uiLabel=uiNewLabelTable[this->runLength[iID].uiLabel];
	}
	delete [] uiNewLabelTable;
	return TRUE;

}
BOOL Object::IsInRegion(int dR, int dC)
{
	for(int iID=0; iID<=this->m_iMaxID; iID++)
	{
		if(this->runLength[iID].iR<dR){continue;}
		if(this->runLength[iID].iR>dR){return FALSE;}

		if(this->runLength[iID].iCStart<=dC)
		{
			if(dC<=this->runLength[iID].iCEnd)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL Object::GetRunlengthIDsInR(int iRIn, int* iIDStart, int* iIDEnd)
{
	BOOL bStart;
	bStart=FALSE;
	for(int i=0; i<=this->m_iMaxID; i++)
	{
		if(this->runLength[i].iR<iRIn){continue;}
		if(this->runLength[i].iR==iRIn)
		{
			if(bStart==FALSE){bStart=TRUE; *iIDStart=i;}
		}
		if(this->runLength[i].iR>iRIn)
		{
			if(bStart==TRUE){*iIDEnd=i-1; return TRUE;}
			return FALSE;
		}
	}
	return FALSE;
}

BOOL Object::UnionOverwrappedRunlength()
{
	if(this->m_iMaxID<0){return FALSE;}
	this->SortR();
	while(1)
	{
		Object objTemp;
		objTemp.Alloc(this->m_iMaxID+1);

		objTemp.runLength[0].Copy(&(this->runLength[0]));
		int iNewID=0;
			
		BOOL bTreatedAny=FALSE;
		for(int iID=1; iID<=this->m_iMaxID; iID++)
		{
			BOOL bTreated=FALSE;
			for(int iID2=iNewID; iID2>=iNewID; iID2--)
			{
				if(IsOverWrapped(&(this->runLength[iID]),  &(objTemp.runLength[iID2])) == TRUE)
				{

					objTemp.runLength[iID2].iCStart=min(this->runLength[iID].iCStart,objTemp.runLength[iID2].iCStart);
					objTemp.runLength[iID2].iCEnd=max(this->runLength[iID].iCEnd,objTemp.runLength[iID2].iCEnd);
					bTreated=TRUE;
					bTreatedAny=TRUE;
					break;
				}
			}
			if(bTreated==TRUE){continue;}

			iNewID++;
			objTemp.runLength[iNewID].Copy(&(this->runLength[iID]));
		}
		objTemp.m_iMaxID=iNewID;
		this->Copy(&objTemp);

		if(bTreatedAny==FALSE){return TRUE;}
		bTreatedAny=FALSE;
	}
	return FALSE;
}
BOOL Object::SortR()
{
	int* iRs;
	int* iIndex;
	int iLength=this->m_iMaxID+1;
	iRs=new int[iLength];
	iIndex=new int[iLength];
	for(int i=0; i<iLength; i++)
	{
		iRs[i]=this->runLength[i].iR;
	}

	Index(iRs, iLength, iIndex);
	delete [] iRs;

	Object objTemp;
	for(int i=0; i<iLength; i++)
	{
		RunLength* runLengthTemp=&(this->runLength[iIndex[i]]);
		objTemp.Add(runLengthTemp->iR, runLengthTemp->iCStart, runLengthTemp->iCEnd, runLengthTemp->uiLabel);
	}
	delete [] iIndex;

	this->Copy(&objTemp);

	return TRUE;
}

BOOL Object::ReCheckID()
{
	this->Truncate();
	this->SortR();

	for(int i=0; i<this->m_iBufNum; i++)
	{
		if(this->runLength[i].bValid==FALSE){continue;}
		if(this->m_uiMaxLabel<this->runLength[i].uiLabel){this->m_uiMaxLabel=this->runLength[i].uiLabel;}
		this->m_iMaxID=i;
	}

	return TRUE;
}
BOOL Object::IsRInRegion(int iRIn)
{
	for(int i=0; i<=this->m_iMaxID; i++)
	{
		if(this->runLength[i].iR<iRIn){continue;}
		if(this->runLength[i].iR==iRIn){return TRUE;}
		if(this->runLength[i].iR>iRIn){return FALSE;}
	}
	return FALSE;
}
BOOL GenRectangle1(Object* objOut, int iR0, int iC0, int iR1, int iC1)
{
	int iR0Local=min(iR0, iR1);
	int iR1Local=max(iR0, iR1);
	int iC0Local=min(iC0, iC1);
	int iC1Local=max(iC0, iC1);

	objOut->Alloc(iR1Local-iR0Local+1);

	for(int i=0; i<iR1Local-iR0Local+1; i++)
	{
		int r=i+iR0Local;
		objOut->runLength[i].Set(r,iC0, iC1,0,TRUE);
	}
	objOut->ReCheckID();
	return TRUE;
}

BOOL Object::Union1()
{
	for(int i=0; i<= this->m_iMaxID; i++)
	{
		this->runLength[i].uiLabel=0;
	}
	UnionOverwrappedRunlength();
	this->ReCheckID();
	return TRUE;
}
BOOL Union1(Object* objIn, Object* objOut)
{
	objOut->Copy(objIn);
	objOut->Union1();
	return TRUE;
}