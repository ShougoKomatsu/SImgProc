// SImgProc.h : SImgProc.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CSImgProcApp
// ���̃N���X�̎����Ɋւ��Ă� SImgProc.cpp ���Q�Ƃ��Ă��������B
//

class CSImgProcApp : public CWinApp
{
public:
	CSImgProcApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
