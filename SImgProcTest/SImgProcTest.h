
// SImgProcTest.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSImgProcTestApp:
// このクラスの実装については、SImgProcTest.cpp を参照してください。
//

class CSImgProcTestApp : public CWinApp
{
public:
	CSImgProcTestApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSImgProcTestApp theApp;