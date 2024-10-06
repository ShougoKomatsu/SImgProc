
// SImgProcTestDlg.cpp : �����t�@�C��
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

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CSImgProcTestDlg �_�C�A���O




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


// CSImgProcTestDlg ���b�Z�[�W �n���h���[

BOOL CSImgProcTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CSImgProcTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
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

	return TRUE;
}


void CSImgProcTestDlg::OnBnClickedButton1()
{
	BOOL bRet;
	bRet = Test1();
	if(bRet != TRUE){AfxMessageBox(_T("Test1 failed"));}
	bRet = Test2();
	if(bRet != TRUE){AfxMessageBox(_T("Test2 failed"));}
	bRet = Test3();
	if(bRet != TRUE){AfxMessageBox(_T("Test3 failed"));}
}
