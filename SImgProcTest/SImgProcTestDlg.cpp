
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
#include "..\\simgproc\\Filter.cpp"
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


BYTE g_byData400[400]=
{
	215,154,110, 67, 62,116,203,165,126,  7,229, 13,241,202, 47,212,252, 62,170,113,
	224, 89,226, 76, 73,177,  7,176,199, 75, 43,217, 84, 84, 24,  4, 40, 41,174, 59,
	 41, 35,223,112, 92,240, 99,146, 70, 83,101, 93,125,109,233,204, 82, 85,240, 56,
	 40, 34, 43,206, 89, 51, 46,109, 84, 46, 67,207, 99,104, 35,191,180, 37, 21, 38,
	  0, 25, 66, 28,167,228,154,202, 43,104,193, 41, 19, 73,227,196,200,230,205, 71,
	179,216,205, 41, 52,153,165, 71,248,21,247,225,113,116,235,1,210,20,240,235,
	192, 67, 48, 98,125, 87,210, 51, 64,253,14,206,35,239,142,233,148,165,47,114,
	 56, 26, 10, 21,145,239,167,141,242,24,225,150,246,221,110,16,161,85,107,236,
	137, 37, 73,105,117,62,127,45,41,122,124,23,11,235,7,38,16,63,120,141,
	111,205, 94,103,196,49,210,125,17,212,228,237,160,230,150,10,237,230,164,227,
	 37,234, 78,232,221,230,13,129,111,15,61,34,50,253,247,26,88,209,188,12,
	22,  30,162,251,47,239,118,40,91,82,175,62,140,141,178,235,59,188,114,208,
	173,51,253,187,127,181,29,64,103,187,175,83,59,170,138,100,22,192,98,8,
	169,224,70,148,17,38,51,55,242,137,213,169,163,97,110,33,186,164,101,180,
	235,192,118,72,234,160,79,139,57,138,128,188,159,237,132,98,46,182,4,205,
	224,214,226,181,4,168,103,137,183,48,66,254,234,94,20,67,166,129,134,199,
	45,68,43,52,133,234,250,108,128,71,64,172,217,150,158,74,41,105,1,243,
	253,216,67,166,60,70,123,27,106,217,227,187,61,248,120,147,46,201,169,242,
	131,144,230,191,114,242,151,221,135,10,105,216,22,81,127,228,18,153,99,182,
	90,101,66,141,60,40,2,247,136,106,67,65,202,69,179,81,170,242,38,62
};

BYTE g_byMax7x7[400]=
{
	226,226,240,240,240,240,240,240,240,241,241,241,241,252,252,252,252,252,252,252,
226,226,240,240,240,240,240,240,240,241,241,241,241,252,252,252,252,252,252,252,
226,226,240,240,240,248,248,248,248,248,248,248,247,252,252,252,252,252,252,252,
226,226,240,240,240,248,253,253,253,253,253,253,253,252,252,252,252,252,252,252,
226,226,240,240,240,248,253,253,253,253,253,253,253,247,246,246,240,240,240,240,
223,223,240,240,240,248,253,253,253,253,253,253,253,247,246,246,240,240,240,240,
216,216,239,239,239,248,253,253,253,253,253,253,253,247,246,246,240,240,240,240,
234,234,239,239,239,248,253,253,253,253,253,253,253,253,253,253,253,247,240,240,
251,251,251,251,251,251,253,253,253,253,253,253,253,253,253,253,253,247,240,240,
253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,247,237,237,
253,253,253,253,253,253,251,242,242,246,253,253,253,253,253,253,253,247,237,237,
253,253,253,253,253,253,251,242,242,242,253,253,253,253,253,253,253,247,237,237,
253,253,253,253,253,253,251,242,254,254,254,254,254,254,254,253,253,247,237,237,
253,253,253,253,253,253,251,250,254,254,254,254,254,254,254,253,253,247,243,243,
253,253,253,253,253,253,251,250,254,254,254,254,254,254,254,248,248,243,243,243,
253,253,253,253,253,253,250,250,254,254,254,254,254,254,254,248,248,243,243,243,
253,253,253,253,250,250,250,250,254,254,254,254,254,254,254,248,248,243,243,243,
253,253,253,253,250,250,250,250,254,254,254,254,254,254,254,248,248,243,243,243,
253,253,253,253,250,250,250,250,254,254,254,254,254,254,254,248,248,243,243,243,
253,253,253,253,250,250,250,250,250,250,248,248,248,248,248,248,248,243,243,243
};

BYTE g_byMin7x7[400]=
{
	34,34,34,7,7,7,7,7,7,7,7,7,4,4,4,4,4,4,4,21,
0,0,0,0,7,7,7,7,7,7,7,7,4,4,4,4,4,4,4,21,
0,0,0,0,7,7,7,7,7,7,7,7,1,1,1,1,1,1,1,20,
0,0,0,0,7,7,7,7,7,7,7,7,1,1,1,1,1,1,1,20,
0,0,0,0,7,7,7,7,7,7,14,14,1,1,1,1,1,1,1,20,
0,0,0,0,10,10,21,14,14,11,11,7,1,1,1,1,1,1,1,16,
0,0,0,0,10,10,17,14,14,11,11,7,1,1,1,1,1,1,1,16,
0,0,0,0,10,10,13,13,13,11,11,7,1,1,1,1,1,1,1,12,
10,10,10,10,10,10,13,13,13,11,11,7,1,1,1,1,1,1,1,12,
10,10,10,10,10,10,13,13,13,11,11,7,7,7,7,7,7,7,8,8,
10,10,10,10,10,10,13,13,13,11,11,7,7,7,7,7,7,7,8,8,
22,17,17,13,13,13,13,13,13,11,11,7,7,7,7,4,4,4,4,4,
22,4,4,4,4,4,4,4,13,13,15,15,10,10,10,4,4,4,4,4,
22,4,4,4,4,4,4,4,13,13,15,15,15,20,20,1,1,1,1,1,
22,4,4,4,4,4,4,4,27,27,27,20,20,20,20,1,1,1,1,1,
43,4,4,4,4,4,4,4,10,10,10,10,10,18,18,1,1,1,1,1,
43,4,4,2,2,2,2,2,2,2,10,10,10,18,18,1,1,1,1,1,
43,4,4,2,2,2,2,2,2,2,10,10,10,18,18,1,1,1,1,1,
43,4,4,2,2,2,2,2,2,2,10,10,10,18,18,1,1,1,1,1,
43,43,40,2,2,2,2,2,2,2,10,10,10,18,18,1,1,1,1,1
};


BYTE g_byBmpData1_2[192]=
{
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
};

BYTE g_byBmpData1_16[192]=
{
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
};
};

BYTE g_byBmpData1_256[192]=
{
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40,
	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
};



BYTE g_byBmpData2_2[204]=
{
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
};

BYTE g_byBmpData2_16[204]=
{
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
};
};

BYTE g_byBmpData2_256[204]=
{
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0xFF, 0xC0, 0x80, 0x40, 0x00, 0x40, 0x80, 
	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 
};




BOOL MaxFilterTest()
{
	
	ImgRGB imgTest;
	ImgRGB imgResult;
	imgTest.Assign(g_byData400,20,20);

	MaxImage(&imgTest, &imgResult,0,0,19,19,7,7);
	CString sResult;
	for(int r=0; r<20; r++)
	{
		for(int c=0; c<20; c++)
		{
			CString sTemp;
			sTemp.Format(_T("%d,"),imgResult.byImg[r*20+c]);
			sResult+=sTemp;
		}
		sResult+="\n";
	}
	//	AfxMessageBox(sResult);
	for(int i=0; i< 400; i++)
	{
		if(imgResult.byImg[i]!=g_byMax7x7[i]){CString sss; sss.Format(_T("%d %d %d"), i, imgResult.byImg[i],g_byMax7x7[i]); AfxMessageBox(sss);return FALSE;}
	}

	int iSparse=2;
	for(int rStart=0; rStart<19; rStart+=iSparse)
	{
		for(int rEnd=rStart; rEnd<20; rEnd+=iSparse)
		{
			for(int cStart=0; cStart<19; cStart+=iSparse)
			{
				for(int cEnd=cStart; cEnd<20; cEnd+=iSparse)
				{

					for(int r=rStart; r<= rEnd; r++)
					{
						for(int c=cStart; c<= cEnd; c++)
						{
							for(int i=0; i<400; i++){imgResult.byImg[i]=255;}
							MaxImage(&imgTest, &imgResult,rStart,cStart,rEnd,cEnd,7,7);

							if(imgResult.byImg[r*20+c]!=g_byMax7x7[r*20+c]){CString sss; sss.Format(_T("(%d, %d) %d %d| rS = %d, re = %d, cS = %d, cE=%d"), r, c, imgResult.byImg[r*20+c],g_byMax7x7[r*20+c],rStart,rEnd,cStart,cEnd); AfxMessageBox(sss);return FALSE;}
						}
					}
				}
			}
		}
	}

	return TRUE;
}
void CSImgProcTestDlg::OnBnClickedButton1()
{
	if(MaxFilterTest()!=TRUE){AfxMessageBox(_T("MaxFilter Failed"));return;}
	AfxMessageBox(_T("ok"));
	return;
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
