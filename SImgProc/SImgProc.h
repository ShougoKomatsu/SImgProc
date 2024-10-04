// SImgProc.h : SImgProc.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSImgProcApp
// このクラスの実装に関しては SImgProc.cpp を参照してください。
//

class CSImgProcApp : public CWinApp
{
public:
	CSImgProcApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
