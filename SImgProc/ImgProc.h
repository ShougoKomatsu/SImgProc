
#pragma once
#include "SImgProc_ex.h"

#define CAP_BYTE(arg) (arg<0 ? 0 : (arg>255 ? 255 : BYTE(arg)))

BOOL Sort(double* arr, int iLength, double* brr);
BOOL Index(double* arr, int iLength, int* brr);
