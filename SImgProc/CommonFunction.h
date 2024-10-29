#include "stdafx.h"

#define Round(dX) ((dX>=INT_MAX-0.5)?(INT_MAX):((dX<=INT_MIN+0.5)?(INT_MIN):((dX>=0)?((int)(dX+0.5)):((int)(dX-0.5)))))
BOOL Sort(const double* arr, int iLength, double* brr);
BOOL Index(const double* dIn, int iLength, int* iIndexOut);
BOOL Index(const int* dIn, int iLength, int* iIndexOut);
BOOL Index(const UINT* dIn, int iLength, int* iIndexOut);



long GetAdjParamSq1(const double* dRas, const double* dCas, const double* dRbs, const double* dCbs, int iLength, double* daPb);

BOOL MatInverse(const double* dMat, const int iRC, double* dInvOut);