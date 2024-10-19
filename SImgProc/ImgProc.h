
#pragma once
#include "SImgProc_ex.h"

#define CAP_BYTE(arg) (arg<0 ? 0 : (arg>255 ? 255 : BYTE(arg)))
