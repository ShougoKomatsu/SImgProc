#include "stdafx.h"
#include "math.h"
#include "CommonFunction.h"

inline void SWAP(double* a, double* b)
{
	double temp;
	temp=(*b);
	*b=(*a);
	*a=temp;
}
inline void SWAP(UINT* a, UINT* b)
{
	UINT temp;
	temp=(*b);
	*b=(*a);
	*a=temp;
}
inline void SWAP(int* a, int* b)
{
	int temp;
	temp=(*b);
	*b=(*a);
	*a=temp;
}

long SumVec(const double* dVec, int iLength, double* dOut)
{
	double* dVecAbs;
	double dSum;
	int* iIndex;

	dVecAbs=new double[iLength];
	for(int i=0; i<iLength; i++)
	{
		dVecAbs[i]=fabs(dVec[i]);
	}

	iIndex=new int[iLength];

	BOOL bRet;
	bRet = Index(dVecAbs, iLength, iIndex);
	if(bRet != TRUE){delete [] dVecAbs; delete [] iIndex; return -1;}
	delete [] dVecAbs;

	dSum=0;
	for(int i=0; i<iLength; i++)
	{
		dSum+=dVec[iIndex[i]];
	}
	delete [] iIndex;

	*dOut=dSum;
	return 0;
}

long MultVec(const double* d1In,const double* d2In, int iLength, double* dOuts)
{
	for(int i=0; i<iLength; i++)
	{
		dOuts[i]=d1In[i]*d2In[i];
	}

	return 0;
}
long InnerProduct(const double* d1In,const double* d2In, int iLength, double* dOut)
{
	double* dProductsAbs;
	dProductsAbs=new double[iLength];

	double* dProducts;
	dProducts=new double[iLength];
	for(int i=0; i<iLength; i++)
	{
		dProducts[i]=d1In[i]*d2In[i];
		dProductsAbs[i]=fabs(dProducts[i]);
	}

	double dSum;
	int* iIndex;
	iIndex=new int[iLength];

	BOOL bRet;
	bRet = Index(dProductsAbs, iLength, iIndex);
	if(bRet != TRUE){delete [] dProductsAbs; delete [] dProducts; delete [] iIndex; return -1;}
	delete [] dProductsAbs;
	dSum=0;
	for(int i=0; i<iLength; i++)
	{
		dSum+=dProducts[iIndex[i]];
	}
	delete [] dProducts;
	delete [] iIndex;

	*dOut=dSum;
	return 0;
}
long GetAdjParamSq1(const double* dRas, const double* dCas, const double* dRbs, const double* dCbs, int iLength, double* daPb)
{
	if(iLength < 1){return -1;}
	double dRa;
	double dCa;
	double dRb;
	double dCb;
	SumVec(dRas,iLength,&dRa);
	SumVec(dCas,iLength,&dCa);
	SumVec(dRbs,iLength,&dRb);
	SumVec(dCbs,iLength,&dCb);

	daPb[0]=1;
	daPb[1]=0;
	daPb[2]=(dRb-dRa)/(iLength*1.0);
	daPb[3]=0;
	daPb[4]=1;
	daPb[5]=(dCb-dCa)/(iLength*1.0);
	daPb[6]=0;
	daPb[7]=0;
	return 0;
}
/*
long GetAdjParamSq2(const double* dRas, const double* dCas, const double* dRbs, const double* dCbs, int iLength, double* daPb)
{
	long lRet;
	if(iLength<2{return -1;}
	double dRb2;
	double dCb2;
	InnerProduct(dRbs, dRbs, iLength, dRb2);
	InnerProduct(dCbs, dCbs, iLength, dCb2);
	double dRb;
	double dCb;
	SumVec(dRbs, iLength, dRb);
	SumVec(dCbs, iLength, dCb);
	double dRaRb;
	InnerProduct(dRas,dRbs,iLength, dRaRb);
	double dRaCb;
	InnerProduct(dRas,dCbs,iLength, dRaCb);
	
}
*/
BOOL Sort(const double* arr, int iLength, double* brr)
{
	double* dOut;
	dOut = new double [iLength];

	for(int ii=0; ii<iLength; ii++)
	{
		dOut[ii]=arr[ii];
	}

	int m=7;
	int nstack=64;
	int i, ir, j, k, jstack=-1, l=0, n=iLength;
	double a;
	int* istack;
	istack=new int[nstack];
	ir=n-1;
	for(;;)
	{
		if(ir-1<m)
		{
			for(j=l+1; j<=ir; j++)
			{
				a=dOut[j];
				for(i=j-1;i>=l;i--)
				{
					if(dOut[i]<=a){break;}
					dOut[i+1]=dOut[i];
				}
				dOut[i+1]=a;
			}
			if(jstack<0){break;}
			ir=istack[jstack--];
			l=istack[jstack--];
		}
		else
		{
			k=(l+ir)>>1;
			SWAP(&(dOut[l]),&(dOut[ir]));
		}
		if(dOut[l] > dOut[ir])
		{
			SWAP(&(dOut[l]), &(dOut[ir]));
		}
		if(dOut[l+1] > dOut[ir])
		{
			SWAP(&(dOut[l+1]), &(dOut[ir]));
		}
		if(dOut[l] > dOut[l+1])
		{
			SWAP(&(dOut[l]), &(dOut[l+1]));
		}
		i=l+1;
		j=ir;
		a=dOut[l+1];
		for(;;)
		{
			do
			{
				i++;
			}while(dOut[i]<a);

			do
			{
				j--;
			}while(dOut[j]>a);
			if(j<i){break;}

			SWAP(&(dOut[i]), &(dOut[j]));
		}
		dOut[l+1]=dOut[j];
		dOut[j]=a;
		jstack += 2;

		if(jstack>=nstack){return FALSE;}
		if(ir-i+1 >= j-1)
		{
			istack[jstack]=ir;
			istack[jstack-1]=i;
			ir=j-1;
		}
		else
		{
			istack[jstack]=j-1;
			istack[jstack-1]=l;
			l=i;
		}
	}

	for(int ii=0; ii<iLength; ii++)
	{
		brr[ii]=dOut[ii];
	}

	return TRUE;
}

BOOL Index(const double* dIn, int iLength, int* iIndexOut)
{
	int* iIndex;
	double* dALocal;
	dALocal = new double[iLength];
	iIndex = new int [iLength];
	

	for(int i=0; i<iLength; i++)
	{
		dALocal[i]=dIn[i];
		iIndex[i]=i;
	}

	int m=7;
	int nstack=64;
	int iIndexR, jstack=-1, indexL=0, n=iLength;
	int* istack;
	istack=new int[nstack];
	iIndexR=n-1;

	while(1)
	{
		if(iIndexR-1<m)
		{
			for(int j=indexL+1; j<=iIndexR; j++)
			{
				double a=dALocal[j];
				int b=iIndex[j];
				int i;
				for(i=j-1; i>=indexL; i--)
				{
					if(dALocal[i]<=a){break;}
					dALocal[i+1]=dALocal[i];
					iIndex[i+1]=iIndex[i];
				}
				dALocal[i+1]=a;
				iIndex[i+1]=b;
			}
			if(jstack<0){break;}
			iIndexR=istack[jstack];jstack--;
			indexL=istack[jstack];jstack--;
			continue;
		}

		int iIndexTemp=(indexL+iIndexR)>>1;
		SWAP(&(dALocal[iIndexTemp]),&(dALocal[indexL+1]));
		SWAP(&(iIndex[iIndexTemp]),&(iIndex[indexL+1]));

		if(dALocal[indexL] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL+1] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL+1]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL+1]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL] > dALocal[indexL+1])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[indexL+1]));
			SWAP(&(iIndex[indexL]), &(iIndex[indexL+1]));
		}


		int i=indexL+1;
		int j=iIndexR;
		double a=dALocal[indexL+1];
		int b=iIndex[indexL+1];
		while(1)
		{
			while(1){i++;if(dALocal[i]>=a){break;}}
			while(1){j--;if(dALocal[j]<=a){break;}}

			if(j<i){break;}

			SWAP(&(dALocal[i]), &(dALocal[j]));
			SWAP(&(iIndex[i]), &(iIndex[j]));
		}
		dALocal[indexL+1]=dALocal[j];
		dALocal[j]=a;
		iIndex[indexL+1]=iIndex[j];
		iIndex[j]=b;
		jstack += 2;

		if(jstack>=nstack){return FALSE;}
		if(iIndexR-i+1 >= j-1)
		{
			istack[jstack]=iIndexR;
			istack[jstack-1]=i;
			iIndexR=j-1;
		}
		else
		{
			istack[jstack]=j-1;
			istack[jstack-1]=indexL;
			indexL=i;
		}
	}

	for(int i=0; i<iLength; i++){iIndexOut[i]=iIndex[i];}

	delete [] iIndex;
	delete [] dALocal;
	return TRUE;
}
BOOL Index(const int* dIn, int iLength, int* iIndexOut)
{
	int* iIndex;
	int* dALocal;
	dALocal = new int[iLength];
	iIndex = new int [iLength];
	

	for(int i=0; i<iLength; i++)
	{
		dALocal[i]=dIn[i];
		iIndex[i]=i;
	}

	int m=7;
	int nstack=64;
	int iIndexR, jstack=-1, indexL=0, n=iLength;
	int* istack;
	istack=new int[nstack];
	iIndexR=n-1;

	while(1)
	{
		if(iIndexR-1<m)
		{
			for(int j=indexL+1; j<=iIndexR; j++)
			{
				int a=dALocal[j];
				int b=iIndex[j];
				int i;
				for(i=j-1; i>=indexL; i--)
				{
					if(dALocal[i]<=a){break;}
					dALocal[i+1]=dALocal[i];
					iIndex[i+1]=iIndex[i];
				}
				dALocal[i+1]=a;
				iIndex[i+1]=b;
			}
			if(jstack<0){break;}
			iIndexR=istack[jstack];jstack--;
			indexL=istack[jstack];jstack--;
			continue;
		}

		int iIndexTemp=(indexL+iIndexR)>>1;
		SWAP(&(dALocal[iIndexTemp]),&(dALocal[indexL+1]));
		SWAP(&(iIndex[iIndexTemp]),&(iIndex[indexL+1]));

		if(dALocal[indexL] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL+1] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL+1]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL+1]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL] > dALocal[indexL+1])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[indexL+1]));
			SWAP(&(iIndex[indexL]), &(iIndex[indexL+1]));
		}


		int i=indexL+1;
		int j=iIndexR;
		int a=dALocal[indexL+1];
		int b=iIndex[indexL+1];
		while(1)
		{
			while(1){i++;if(dALocal[i]>=a){break;}}
			while(1){j--;if(dALocal[j]<=a){break;}}

			if(j<i){break;}

			SWAP(&(dALocal[i]), &(dALocal[j]));
			SWAP(&(iIndex[i]), &(iIndex[j]));
		}
		dALocal[indexL+1]=dALocal[j];
		dALocal[j]=a;
		iIndex[indexL+1]=iIndex[j];
		iIndex[j]=b;
		jstack += 2;

		if(jstack>=nstack){return FALSE;}
		if(iIndexR-i+1 >= j-1)
		{
			istack[jstack]=iIndexR;
			istack[jstack-1]=i;
			iIndexR=j-1;
		}
		else
		{
			istack[jstack]=j-1;
			istack[jstack-1]=indexL;
			indexL=i;
		}
	}

	for(int i=0; i<iLength; i++){iIndexOut[i]=iIndex[i];}

	delete [] iIndex;
	delete [] dALocal;
	return TRUE;
}

BOOL Index(const UINT* dIn, int iLength, int* iIndexOut)
{
	int* iIndex;
	UINT* dALocal;
	dALocal = new UINT[iLength];
	iIndex = new int [iLength];
	

	for(int i=0; i<iLength; i++)
	{
		dALocal[i]=dIn[i];
		iIndex[i]=i;
	}

	int m=7;
	int nstack=64;
	int iIndexR, jstack=-1, indexL=0, n=iLength;
	int* istack;
	istack=new int[nstack];
	iIndexR=n-1;

	while(1)
	{
		if(iIndexR-1<m)
		{
			for(int j=indexL+1; j<=iIndexR; j++)
			{
				UINT a=dALocal[j];
				int b=iIndex[j];
				int i;
				for(i=j-1; i>=indexL; i--)
				{
					if(dALocal[i]<=a){break;}
					dALocal[i+1]=dALocal[i];
					iIndex[i+1]=iIndex[i];
				}
				dALocal[i+1]=a;
				iIndex[i+1]=b;
			}
			if(jstack<0){break;}
			iIndexR=istack[jstack];jstack--;
			indexL=istack[jstack];jstack--;
			continue;
		}

		int iIndexTemp=(indexL+iIndexR)>>1;
		SWAP(&(dALocal[iIndexTemp]),&(dALocal[indexL+1]));
		SWAP(&(iIndex[iIndexTemp]),&(iIndex[indexL+1]));

		if(dALocal[indexL] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL+1] > dALocal[iIndexR])
		{
			SWAP(&(dALocal[indexL+1]), &(dALocal[iIndexR]));
			SWAP(&(iIndex[indexL+1]), &(iIndex[iIndexR]));
		}
		if(dALocal[indexL] > dALocal[indexL+1])
		{
			SWAP(&(dALocal[indexL]), &(dALocal[indexL+1]));
			SWAP(&(iIndex[indexL]), &(iIndex[indexL+1]));
		}


		int i=indexL+1;
		int j=iIndexR;
		UINT a=dALocal[indexL+1];
		int b=iIndex[indexL+1];
		while(1)
		{
			while(1){i++;if(dALocal[i]>=a){break;}}
			while(1){j--;if(dALocal[j]<=a){break;}}

			if(j<i){break;}

			SWAP(&(dALocal[i]), &(dALocal[j]));
			SWAP(&(iIndex[i]), &(iIndex[j]));
		}
		dALocal[indexL+1]=dALocal[j];
		dALocal[j]=a;
		iIndex[indexL+1]=iIndex[j];
		iIndex[j]=b;
		jstack += 2;

		if(jstack>=nstack){return FALSE;}
		if(iIndexR-i+1 >= j-1)
		{
			istack[jstack]=iIndexR;
			istack[jstack-1]=i;
			iIndexR=j-1;
		}
		else
		{
			istack[jstack]=j-1;
			istack[jstack-1]=indexL;
			indexL=i;
		}
	}

	for(int i=0; i<iLength; i++){iIndexOut[i]=iIndex[i];}

	delete [] iIndex;
	delete [] dALocal;
	return TRUE;
}

BOOL LUDcmp(const double* dA, const int iRC, double* dLU, int* iIndex)
{

	for(int r=0; r<iRC; r++)
	{
		for(int c=0; c<iRC; c++)
		{
			dLU[r*iRC+c]=dA[r*iRC+c];
		}
	}

	const double TINY=1.0e-40;
	double* vv;
	vv=new double[iRC];
	for(int r=0; r<iRC; r++)
	{
		double big=0.0;
		for(int c=0; c<iRC; c++)
		{
			double temp=fabs(dLU[r*iRC+c]);
			if(temp>big){big=temp;}
		}
		if(big==0.0){return FALSE;}
		vv[r]=1.0/big;
	}
	int rMax=0;
	for(int k=0; k<iRC; k++)
	{
		double big=0.0;
		for(int r=k; r<iRC; r++)
		{
			double temp=vv[r]*fabs(dLU[r*iRC+k]);
			if(temp>big)
			{
				big=temp;
				rMax=r;
			}
		}
		if(k!=rMax)
		{
			for(int c=0; c<iRC; c++)
			{
				SWAP(&(dLU[rMax*iRC+c]),&(dLU[k*iRC+c]));
			}
			vv[rMax]=vv[k];
		}
		iIndex[k]=rMax;
		if(dLU[k*iRC+k]==0.0){dLU[k*iRC+k]=TINY;}

		for(int r=k+1; r<iRC; r++)
		{
			dLU[r*iRC+k]/=dLU[k*iRC+k];
			double temp=dLU[r*iRC+k];
			for(int c=k+1; c<iRC; c++)
			{
				dLU[r*iRC+c]-=temp*dLU[k*iRC+c];
			}
		}
	}

	delete [] vv;
	return TRUE;
}

void solve_v(const double* dB, const int iRC, const double* dLU, const int* iIndex, double* dAnsVec)
{
	double sum;
	for(int r=0; r<iRC; r++){dAnsVec[r]=dB[r];}
	int ii=0;
	for(int r=0; r<iRC; r++)
	{
		int rIndex=iIndex[r];
		sum=dAnsVec[rIndex];
		dAnsVec[rIndex]=dAnsVec[r];

		if(ii!=0)
		{
			for(int c=ii-1; c<r; c++){sum-=dLU[r*iRC+c]*dAnsVec[c];}
		}
		else if(sum != 0.0){ii=r+1;}

		dAnsVec[r]=sum;
	}
	for(int r=iRC-1; r>=0; r--)
	{
		sum=dAnsVec[r];
		for(int c=r+1; c<iRC; c++){sum-=dLU[r*iRC+c]*dAnsVec[c];}
		dAnsVec[r]=sum/dLU[r*iRC+r];
	}
}

void solve_m(const double* dB, const int iRC, const double* dLU, const int* iIndex, double* dAnsMat)
{
	double* xx;
	xx=new double[iRC];
	for(int c=0; c<iRC; c++)
	{
		for(int r=0; r<iRC; r++){xx[r]=dB[r*iRC+c];}
		solve_v(xx,iRC, dLU, iIndex, xx);
		for(int r=0; r<iRC; r++){dAnsMat[r*iRC+c]=xx[r];}
	}
	delete [] xx;
}

BOOL MatInverse(const double* dMat, const int iRC, double* dInvOut)
{
	int* iIndex;
	double* dLU;
	dLU=new double[iRC*iRC];
	iIndex=new int[iRC];

	LUDcmp(dMat,iRC, dLU, iIndex);

	double* dInv;
	dInv=new double[iRC*iRC];
	for(int r=0; r<iRC; r++)
	{
		for(int c=0; c<iRC; c++){dInv[r*iRC+c]=0;}
		dInv[r*iRC+r]=1;
	}
	solve_m(dInv, iRC, dLU, iIndex, dInv);
	delete [] dLU;
	delete [] iIndex;


	for(int r=0; r<iRC; r++)
	{
		for(int c=0; c<iRC; c++){dInvOut[r*iRC+c]=dInv[r*iRC+c];}
	}
	delete [] dInv;
	return TRUE;
}


