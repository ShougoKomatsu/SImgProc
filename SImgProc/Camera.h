#include "stdafx.h"
#pragma once
#include "SImgProc_ex.h"

struct CameraLocal
{
	HANDLE m_hPipe;
	HANDLE m_hSharedMemory;
	BYTE* m_pbyMemory;

	int m_iChannel;
	int m_iWidth;
	int m_iHeight;
	CString m_sPipeName;

	CameraLocal()
	{
		m_hPipe=INVALID_HANDLE_VALUE;
		m_hSharedMemory=INVALID_HANDLE_VALUE;
		m_pbyMemory=NULL;

		m_iChannel=CHANNEL_UNDEFINED;
		m_iWidth=0;
		m_iHeight=0;
		m_sPipeName.Format(_T(""));
	}
	
	~CameraLocal(){CloseCamera();}
	int SendRecive(CString sPipeName, CString sSend, CString* sReceive);
	int OpenCamera(CString sPipeName);
	int CloseCamera();
	int GrabImage(ImgRGB* imgOut);
};