
// SImgProcTestDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SImgProcTest.h"
#include "SImgProcTestDlg.h"
#include "afxdialogex.h"

#include "..\\simgproc\\simgproc_ex.h"
//#pragma comment(lib, "..\\SImgProc.lib")

#include "..\\simgproc\\ImgProc.h"
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
	imgTest.byImg[7*10+1]=128;
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
void CSImgProcTestDlg::OnBnClickedButton1()
{
/*
	ImgRGB imgTest;
	imgTest.Assign(_T("d:\\20240718_131406.bmp"));
	Object obj;
	ImgRGB imgR, imgG, imgB;
	Decompose3(&imgTest,&imgR,&imgG,&imgB);
	Threshold(&imgR,240,255,&obj);
	obj.Connection(8);
	ImgRGB imgOut;
	PaintRegion(&imgTest, &obj, &imgOut);
	WriteImage(&imgOut,_T("d:\\test.bmp"));
	*/
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
