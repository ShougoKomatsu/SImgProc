#include "stdafx.h"
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

BOOL Sort(double* arr, int iLength, double* brr)
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

BOOL Index(double* dIn, int iLength, int* iIndexOut)
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
BOOL Index(int* dIn, int iLength, int* iIndexOut)
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

BOOL Index(UINT* dIn, int iLength, int* iIndexOut)
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