
// SImgProcTestDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SImgProcTest.h"
#include "SImgProcTestDlg.h"
#include "afxdialogex.h"

#include "..\\simgproc\\simgproc_ex.h"
//#pragma comment(lib, "..\\SImgProc.lib")

#include "..\\simgproc\\ImgProc.h"
#include "..\\simgproc\\commonfunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSImgProcTestDlg ダイアログ




CSImgProcTestDlg::CSImgProcTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSImgProcTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSImgProcTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSImgProcTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSImgProcTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSImgProcTestDlg メッセージ ハンドラー

BOOL CSImgProcTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSImgProcTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSImgProcTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSImgProcTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL Test1()
{
	Object obj;
	obj.Add(0,0,0,0);
	obj.Add(1,2,2,0);
	obj.Connection(4);

	if(obj.runLength[0].uiLabel != 1){return FALSE;}
	if(obj.runLength[1].uiLabel != 2){return FALSE;}

	return TRUE;
}

BOOL Test2()
{
	Object obj;
	obj.Add(0,0,0,0);
	obj.Add(0,2,2,0);
	obj.Add(1,0,2,0);
	obj.Connection(4);

	if(obj.runLength[0].uiLabel != 1){return FALSE;}
	if(obj.runLength[1].uiLabel != 1){return FALSE;}
	if(obj.runLength[2].uiLabel != 1){return FALSE;}

	return TRUE;
}

BOOL Test3()
{
	Object obj;
	obj.Add(0,0,0,0);
	obj.Add(0,2,2,1);
	obj.Add(1,0,2,3);
	obj.Truncate();

	if(obj.runLength[0].uiLabel != 0){return FALSE;}
	if(obj.runLength[1].uiLabel != 1){return FALSE;}
	if(obj.runLength[2].uiLabel != 2){return FALSE;}
	double dA, dR, dC;
	AreaCenter(&obj, &dA, &dR, &dC);
	if(dA != 5){return FALSE;}
	if(dR != 3/5.0){return FALSE;}
	if(dC != 1){return FALSE;}

	return TRUE;
}

BOOL Test4()
{
	ImgRGB imgTest;
	imgTest.Set(10,10,CHANNEL_1_8);
	imgTest.byImg[3*10+1]=128;
	Object obj;
	Threshold(&imgTest,100,200,&obj);

	if(obj.m_iMaxID != 0){return FALSE;}
	if(obj.runLength[0].iR != 3){return FALSE;}
	if(obj.runLength[0].iCStart != 1){return FALSE;}
	if(obj.runLength[0].iCEnd != 1){return FALSE;}
	return TRUE;
}

BOOL Test5()
{
	Object obj;
	obj.Add(0,0,0,0);
	obj.Add(0,2,2,0);
	obj.Add(2,0,2,0);
	obj.Add(3,0,1,0);
	obj.Connection(4);


	Object obj2;
	SelectObj(&obj,2,&obj2);

	if(obj2.m_iMaxID != 1){return FALSE;}
	if(obj2.runLength[0].iR != 2){return FALSE;}
	if(obj2.runLength[0].iCStart != 0){return FALSE;}
	if(obj2.runLength[0].iCEnd != 2){return FALSE;}
	if(obj2.runLength[1].iR != 3){return FALSE;}
	if(obj2.runLength[1].iCStart != 0){return FALSE;}
	if(obj2.runLength[1].iCEnd != 1){return FALSE;}
	return TRUE;
}

BOOL TestSort()
{
	double a[5]={1,3,2,4,5};
	int b[5];

	Index(a,5,b);
	if(b[0] != 0){return FALSE;}
	if(b[1] != 2){return FALSE;}
	if(b[2] != 1){return FALSE;}
	if(b[3] != 3){return FALSE;}
	if(b[4] != 4){return FALSE;}

	return TRUE;
}

BOOL TestSortRegion()
{
	
	ImgRGB imgTest;
	imgTest.Set(10,10,CHANNEL_1_8);
	imgTest.byImg[3*10+1]=128;
	imgTest.byImg[5*10+1]=128;
	imgTest.byImg[5*10+2]=128;
	imgTest.byImg[7*10+3]=128;
	Object obj;
	Threshold(&imgTest,100,200,&obj);

	if(obj.m_iMaxID != 2){return FALSE;}
	if(obj.runLength[1].iR != 5){return FALSE;}
	if(obj.runLength[1].iCStart != 1){return FALSE;}
	if(obj.runLength[1].iCEnd != 2){return FALSE;}
	obj.Connection(4);
	if(obj.runLength[1].uiLabel != 2){return FALSE;}
	Object objOut;
	SortRegion(&obj,_T("area"), _T("dsc"),&objOut);
	if(objOut.runLength[1].uiLabel != 1){return FALSE;}
	



	SortRegion(&obj,_T("row"), _T("asc"),&objOut);
	if(objOut.runLength[0].uiLabel != 1){return FALSE;}
	if(objOut.runLength[1].uiLabel != 2){return FALSE;}
	if(objOut.runLength[2].uiLabel != 3){return FALSE;}
	
	SortRegion(&obj,_T("row"), _T("dsc"),&objOut);
	if(objOut.runLength[0].uiLabel != 3){return FALSE;}
	if(objOut.runLength[1].uiLabel != 2){return FALSE;}
	if(objOut.runLength[2].uiLabel != 1){return FALSE;}

	
	SortRegion(&obj,_T("column"), _T("asc"),&objOut);
	if(objOut.runLength[0].uiLabel != 1){return FALSE;}
	if(objOut.runLength[1].uiLabel != 2){return FALSE;}
	if(objOut.runLength[2].uiLabel != 3){return FALSE;}
	
	SortRegion(&obj,_T("column"), _T("dsc"),&objOut);
	if(objOut.runLength[0].uiLabel != 3){return FALSE;}
	if(objOut.runLength[1].uiLabel != 2){return FALSE;}
	if(objOut.runLength[2].uiLabel != 1){return FALSE;}
	return TRUE;
}
BOOL TestUnionOverwrappedRunlength()
{
	Object obj;
	obj.Alloc(4);
	obj.runLength[0].Set(1,2,5,1,TRUE);
	obj.runLength[1].Set(2,2,5,2,TRUE);
	obj.runLength[2].Set(1,4,7,1,TRUE);
	obj.runLength[3].Set(2,4,7,3,TRUE);
	obj.m_iMaxID=3;
	obj.UnionOverwrappedRunlength();

	if(obj.runLength[0].iCEnd != 7){return FALSE;}
	if(obj.runLength[1].iCEnd != 5){return FALSE;}
	if(obj.runLength[2].iCStart != 4){return FALSE;}
	return TRUE;
}
BOOL TestUnionOverwrappedRunlength2()
{
	Object obj;
	obj.Alloc(4);
	obj.runLength[0].Set(1,10,15,1,TRUE);
	obj.runLength[1].Set(2,2,5,2,TRUE);
	obj.runLength[2].Set(1,2,5,1,TRUE);
	obj.runLength[3].Set(1,4,12,1,TRUE);
	obj.m_iMaxID=3;
	obj.UnionOverwrappedRunlength();

	if(obj.m_iMaxID!=1){return FALSE;}
	if(obj.runLength[0].iCEnd != 15){return FALSE;}
	if(obj.runLength[0].iCStart != 2){return FALSE;}
	if(obj.runLength[1].iCStart != 2){return FALSE;}
	return TRUE;
}
BOOL TestPaintRegion()
{
	ImgRGB imgTest;
	imgTest.Set(10,10,CHANNEL_1_8);
	imgTest.byImg[3*10+1]=128;
	imgTest.byImg[5*10+1]=128;
	imgTest.byImg[5*10+2]=128;
	imgTest.byImg[7*10+1]=128;
	Object obj;
	Threshold(&imgTest,100,200,&obj);

	ImgRGB imgOut;
	PaintRegion(&imgTest, &obj, &imgOut, 255,100,200);
	if(imgOut.byImgR[3*10+1] != 255){return FALSE;}
	if(imgOut.byImgG[3*10+1] != 100){return FALSE;}
	if(imgOut.byImgB[3*10+1] != 200){return FALSE;}
	if(imgOut.byImgR[5*10+2] != 255){return FALSE;}
	if(imgOut.byImgG[5*10+2] != 100){return FALSE;}
	if(imgOut.byImgB[5*10+2] != 200){return FALSE;}
	if(imgOut.byImgR[5*10+3] != 0){return FALSE;}
	if(imgOut.byImgG[5*10+3] != 0){return FALSE;}
	if(imgOut.byImgB[5*10+3] != 0){return FALSE;}
	return TRUE;
}
BOOL TestPaintRegion2()
{
	ImgRGB imgTest;
	imgTest.Set(10,10,CHANNEL_1_8);
	imgTest.byImg[3*10+1]=128;
	imgTest.byImg[5*10+1]=128;
	imgTest.byImg[5*10+2]=128;
	imgTest.byImg[7*10+1]=128;
	Object obj;
	Threshold(&imgTest,100,200,&obj);
	obj.Connection(4);

	ImgRGB imgOut;
	PaintRegion(&imgTest, &obj, &imgOut);
	if(imgOut.byImgR[3*10+1] != 255){return FALSE;}
	if(imgOut.byImgG[3*10+1] != 0){return FALSE;}
	if(imgOut.byImgB[3*10+1] != 0){return FALSE;}
	if(imgOut.byImgR[5*10+1] != 0){return FALSE;}
	if(imgOut.byImgG[5*10+1] != 255){return FALSE;}
	if(imgOut.byImgB[5*10+1] != 0){return FALSE;}
	if(imgOut.byImgR[5*10+2] != 0){return FALSE;}
	if(imgOut.byImgG[5*10+2] != 255){return FALSE;}
	if(imgOut.byImgB[5*10+2] != 0){return FALSE;}
	if(imgOut.byImgR[7*10+1] != 0){return FALSE;}
	if(imgOut.byImgG[7*10+1] != 0){return FALSE;}
	if(imgOut.byImgB[7*10+1] != 255){return FALSE;}
	return TRUE;
}
BOOL TestSortR()
{
	Object obj;
	obj.Alloc(4);
	obj.runLength[0].Set(1,10,15,1,TRUE);
	obj.runLength[1].Set(2,2,5,2,TRUE);
	obj.runLength[2].Set(1,2,5,1,TRUE);
	obj.runLength[3].Set(1,4,12,1,TRUE);
	obj.m_iMaxID=3;

	obj.SortR();
	if(obj.runLength[0].iR!=1){return FALSE;}
	if(obj.runLength[1].iR!=1){return FALSE;}
	if(obj.runLength[2].iR!=1){return FALSE;}
	if(obj.runLength[3].iR!=2){return FALSE;}

	if(obj.runLength[0].iCStart!=10){return FALSE;}
	if(obj.runLength[1].iCStart!=2){return FALSE;}
	if(obj.runLength[2].iCStart!=4){return FALSE;}
	if(obj.runLength[3].iCStart!=2){return FALSE;}

	if(obj.runLength[0].iCEnd!=15){return FALSE;}
	if(obj.runLength[1].iCEnd!=5){return FALSE;}
	if(obj.runLength[2].iCEnd!=12){return FALSE;}
	if(obj.runLength[3].iCEnd!=5){return FALSE;}
	return TRUE;
}
BOOL TestSelectShape()
{
	Object obj;
	obj.Alloc(4);
	obj.runLength[0].Set(1,10,15,1,TRUE);
	obj.runLength[1].Set(2,2,5,2,TRUE);
	obj.runLength[2].Set(1,2,5,1,TRUE);
	obj.runLength[3].Set(1,4,12,1,TRUE);
	obj.m_iMaxID=3;
	obj.m_uiMaxLabel=2;
	Object objOut;

	SelectShape(&obj,&objOut,_T("area"), 3,5);
	if(objOut.m_iMaxID!=0){return FALSE;}
	if(objOut.runLength[0].iR!=2){return FALSE;}
	if(objOut.runLength[0].iCStart!=2){return FALSE;}
	if(objOut.runLength[0].iCEnd!=5){return FALSE;}
	return TRUE;
}
BOOL TestConcatObj()
{
	Object obj1;
	obj1.Alloc(4);
	obj1.runLength[0].Set(1,10,15,1,TRUE);
	obj1.runLength[1].Set(2,2,5,2,TRUE);
	obj1.runLength[2].Set(1,2,5,1,TRUE);
	obj1.m_iMaxID=2;
	obj1.m_uiMaxLabel=2;

	Object obj2;
	obj2.Alloc(4);
	obj2.runLength[0].Set(1,10,15,1,TRUE);
	obj2.runLength[1].Set(2,2,5,2,TRUE);
	obj2.runLength[2].Set(1,2,5,1,TRUE);
	obj2.m_iMaxID=2;
	obj2.m_uiMaxLabel=2;

	Object objOut;
	ConcatObj(&obj1, &obj2, &objOut);
	if(objOut.m_iMaxID != 5){return FALSE;}
	if(objOut.m_uiMaxLabel != 5){return FALSE;}
	return TRUE;
}
BOOL TestAreaCenter()
{
	Object obj;
	obj.Alloc(4);
	obj.runLength[0].Set(1,10,15,1,TRUE);
	obj.runLength[1].Set(2,2,5,2,TRUE);
	obj.runLength[2].Set(2,2,5,1,TRUE);
	obj.runLength[3].Set(3,4,12,1,TRUE);
	obj.m_iMaxID=3;

	obj.ReCheckID();

	double dA[4];
	double dR[4];
	double dC[4];
	AreaCenter(&obj,dA, dR, dC, 4);
	if(dA[1] != 19){return FALSE;}
	if(dA[2] != 4){return FALSE;}

	if(dR[1] < 2.15){return FALSE;}
	if(dR[1] > 2.16){return FALSE;}
	if(dR[2] != 2){return FALSE;}
	
	if(dC[1] < 8.47){return FALSE;}
	if(dC[1] > 8.48){return FALSE;}
	if(dC[2] != 3.5){return FALSE;}
	

	return TRUE;
}

BOOL TestGenRectangle1()
{
	Object obj;
	GenRectangle1(&obj,2,4,3,6);

	if(obj.m_iMaxID!=1){return FALSE;}
	if(obj.m_uiMaxLabel!=0){return FALSE;}

	if(obj.runLength[0].iR!=2){return FALSE;}
	if(obj.runLength[0].iCStart!=4){return FALSE;}
	if(obj.runLength[0].iCEnd!=6){return FALSE;}

	if(obj.runLength[1].iR!=3){return FALSE;}
	if(obj.runLength[1].iCStart!=4){return FALSE;}
	if(obj.runLength[1].iCEnd!=6){return FALSE;}
	return TRUE;
}
BOOL TestGetRunlengthIDsInR()
{
		Object obj;
	obj.Alloc(4);
	obj.runLength[1].Set(2,2,5,1,TRUE);
	obj.runLength[2].Set(2,10,15,1,TRUE);
	obj.runLength[3].Set(3,4,12,1,TRUE);
	obj.runLength[0].Set(1,10,15,1,TRUE);

	obj.ReCheckID();

	int iIDStart;
	int iIDEnd;
	obj.GetRunlengthIDsInR(2,&iIDStart, &iIDEnd);
	if(iIDStart != 1){return FALSE;}
	if(iIDEnd != 2){return FALSE;}
	return TRUE;
}

BOOL TestReduceDomain()
{
	ImgRGB imgTest;
	imgTest.Set(10,10,CHANNEL_1_8);
	imgTest.byImg[3*10+1]=128;
	imgTest.byImg[5*10+3]=128;
	imgTest.byImg[5*10+4]=128;
	imgTest.byImg[5*10+5]=128;
	imgTest.byImg[5*10+6]=128;
	imgTest.byImg[5*10+7]=128;
	imgTest.byImg[5*10+8]=128;
	imgTest.byImg[5*10+9]=128;
	imgTest.byImg[6*10+6]=128;
	Object obj;
	GenRectangle1(&obj, 5,5,7,7);
	ImgRGB imgTest2;
	ReduceDomain(&imgTest,&obj,&imgTest2);
	Threshold(&imgTest2,100,200,&obj);
	if(obj.m_iMaxID!=1){return FALSE;}
	if(obj.runLength[0].iR != 5){return FALSE;}
	if(obj.runLength[0].iCStart != 5){return FALSE;}
	if(obj.runLength[0].iCEnd != 7){return FALSE;}
	if(obj.runLength[1].iR != 6){return FALSE;}
	if(obj.runLength[1].iCStart != 6){return FALSE;}
	if(obj.runLength[1].iCEnd != 6){return FALSE;}
	return TRUE;
}
BOOL TestArithmetic()
{
	if(Round(1)!=1){return FALSE;}
	if(Round(1.5)!=2){return FALSE;}
	if(Round(-1.5)!=-2){return FALSE;}
	if(Round(INT_MAX-0.5)!=INT_MAX){return FALSE;}
	if(Round(INT_MIN+0.5)!=INT_MIN){return FALSE;}
	if(Round(LLONG_MAX-1)!=INT_MAX){return FALSE;}
	if(Round(LLONG_MIN+1)!=INT_MIN){return FALSE;}
	return TRUE;
}

BOOL TestAdjust()
{
	double dRas[6]={1,2,3,4,5,6};
	double dCas[6]={21,22,23,24,25,26};
	double dRbs[6]={11,12,13,14,15,16};
	double dCbs[6]={41,42,43,44,45,46};
	double daPb[8];
	GetAdjParamSq1(dRas, dCas, dRbs, dCbs, 6, daPb);

	if(daPb[0]!=1){return FALSE;}
	if(daPb[1]!=0){return FALSE;}
	if(daPb[2]!=10){return FALSE;}
	if(daPb[3]!=0){return FALSE;}
	if(daPb[4]!=1){return FALSE;}
	if(daPb[5]!=20){return FALSE;}
	if(daPb[6]!=0){return FALSE;}
	if(daPb[7]!=0){return FALSE;}
	return TRUE;
}
BOOL TestInvert()
{
	double dMat[9]={1,2,4,4,8,8,7,9,9};
	double dInv[9];
//	LUDcmp a(dMat,3);
//	a.inverse(dInv);
 MatInverse(dMat, 3, dInv);
	if(dInv[0] != 0){return FALSE;}
	if(dInv[1] <= -0.451){return FALSE;}
	if(dInv[1] >= -0.449){return FALSE;}
	if(dInv[2] <= 0.399){return FALSE;}
	if(dInv[2] >= 0.401){return FALSE;}

	if(dInv[3] != -0.5){return FALSE;}
	if(dInv[4] <= 0.474){return FALSE;}
	if(dInv[4] >= 0.476){return FALSE;}
	if(dInv[5] <= -0.201){return FALSE;}
	if(dInv[5] >= -0.199){return FALSE;}

	if(dInv[6] != 0.5){return FALSE;}
	if(dInv[7] != -0.125){return FALSE;}
	if(dInv[8] != 0){return FALSE;}
	return TRUE;
}

void CSImgProcTestDlg::OnBnClickedButton1()
{
	Camera camera;
	camera.OpenCamera(_T("Camera0327"));
	Sleep(1000);
	camera.SetParameter(_T("EXPOSURE"),_T("10"));
	ImgRGB imgTest;
	camera.GrabImage(&imgTest);
	WriteImage(&imgTest,_T("d:\\cameratest.bmp"));

	camera.CloseCamera();
	return;
	/*
	ImgRGB imgTest;
	ImgRGB imgResult;
	ImgRGB imgResult2;
	Screenshot(&imgTest);

	MinImage(&imgTest, &imgResult,9,9);
	MaxImage(&imgResult, &imgResult2,9,9);

	SubImage(&imgTest, &imgResult2,&imgResult,255,127);
	WriteImage(&imgResult,_T("d:\\testa.bmp"));
	WriteImage(&imgTest,_T("d:\\testb.bmp"));


	ImgRGB img1, img2, imgR, imgd;
	Object obj1, obj2, obj3;
Screenshot(&img1);	
Decompose3(&img1,&imgR, &imgd, &imgd);
Threshold(&imgR, 0,128, &obj1);
Connection(&obj1,&obj2,4);
SelectShape(&obj2,&obj3,_T("area"), 10, 10000000);
SelectObj(&obj3,0,&obj1);
	ImgRGB imgTest;
	imgTest.Assign(_T("d:\\20240718_131406.bmp"));
	Object obj;
	GenRectangle1(&obj,500,500, 1000, 1000);

	ImgRGB imgR, imgG, imgB,imgR2;
	Decompose3(&imgTest,&imgR,&imgG,&imgB);
	ReduceDomain(&imgR,&obj,&imgR2);
	Threshold(&imgR2,240,255,&obj);
	obj.Connection(8);
	ImgRGB imgOut;
	PaintRegion(&imgTest, &obj, &imgOut);
	WriteImage(&imgOut,_T("d:\\test.bmp"));
	*/
	
	
	if(TestInvert()!=TRUE){AfxMessageBox(_T("TestInvert failed"));}
	if(TestAdjust()!=TRUE){AfxMessageBox(_T("TestAdjust failed"));}
	if(TestArithmetic()!=TRUE){AfxMessageBox(_T("TestArithmetic failed"));}

	if(TestReduceDomain() != TRUE){AfxMessageBox(_T("TestReduceDomain failed"));}
	if(TestGetRunlengthIDsInR() != TRUE){AfxMessageBox(_T("TestGetRunlengthIDsInR failed"));}
	if(TestGenRectangle1() != TRUE){AfxMessageBox(_T("TestGenRectangle1 failed"));}
	if(TestAreaCenter() != TRUE){AfxMessageBox(_T("TestAreaCenter failed"));}
	if(TestConcatObj() != TRUE){AfxMessageBox(_T("TestConcatObj failed"));}
	if(TestSelectShape() != TRUE){AfxMessageBox(_T("TestSelectShape failed"));}
	if(TestSortR() != TRUE){AfxMessageBox(_T("TestSortR failed"));}
	if(TestPaintRegion2() != TRUE){AfxMessageBox(_T("TestPaintRegion2 failed"));}
	if(TestPaintRegion() != TRUE){AfxMessageBox(_T("TestPaintRegion failed"));}
	if(TestUnionOverwrappedRunlength2() != TRUE){AfxMessageBox(_T("TestUnionOverwrappedRunlength2 failed"));}
	if(TestUnionOverwrappedRunlength() != TRUE){AfxMessageBox(_T("TestUnionOverwrappedRunlength failed"));}
	if(TestSortRegion() != TRUE){AfxMessageBox(_T("TestSortRegion failed"));}
	if(TestSort() != TRUE){AfxMessageBox(_T("TestSort failed"));}


	BOOL bRet;
	bRet = Test1();
	if(bRet != TRUE){AfxMessageBox(_T("Test1 failed"));}
	bRet = Test2();
	if(bRet != TRUE){AfxMessageBox(_T("Test2 failed"));}
	bRet = Test3();
	if(bRet != TRUE){AfxMessageBox(_T("Test3 failed"));}
	bRet = Test4();
	if(bRet != TRUE){AfxMessageBox(_T("Test4 failed"));}
	bRet = Test5();
	if(bRet != TRUE){AfxMessageBox(_T("Test5 failed"));}
}
