
#pragma once
#include "SImgProc_ex.h"

#define CAP_BYTE(arg) (arg<0 ? 0 : (arg>255 ? 255 : BYTE(arg)))
BOOL GetHistgram(const ImgRGB* imgRGB, int r0, int c0, int r1, int c1,  int* iHistR, int* iHistG, int* iHistB);
BOOL GetHistgram(const ImgRGB* imgRGB, int r0, int c0, int r1, int c1,  int* iHist);