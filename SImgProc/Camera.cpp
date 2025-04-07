#include "stdafx.h"

#include "camera.h"
#include "CommonFunction.h"


int CameraLocal::SendRecive(CString sPipeName, CString sSend, CString* sReceive)
{

	BOOL bRet;

	CString sPipeFullName;
	sPipeFullName.Format(_T("\\\\.\\pipe\\%s"), sPipeName);


	if(m_hPipe==INVALID_HANDLE_VALUE)
	{
		m_hPipe = CreateFile(sPipeFullName, PIPE_ACCESS_DUPLEX, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if (m_hPipe == INVALID_HANDLE_VALUE) {CloseHandle(m_hPipe); return -1;}


	TCHAR tchBuffer[256];
	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	_stprintf(tchBuffer, _T("%s"), sSend);
	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwNumberOfByteToWrite = (_tcslen(tchBuffer)+1) * sizeof(TCHAR);
	bRet = WriteFile(m_hPipe, tchBuffer, dwNumberOfByteToWrite, (LPDWORD)&dwNumberOfBytesWritten, NULL);

	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	DWORD dwNumberOfBytesRead = 0;
	bRet = ReadFile(m_hPipe, tchBuffer, sizeof(tchBuffer), (LPDWORD)&dwNumberOfBytesRead, NULL);

	sReceive->Format(_T("%s"), tchBuffer);
	FlushFileBuffers(m_hPipe);
//	AfxMessageBox(*sReceive);
	return 0;
}

int CameraLocal::OpenCamera(CString sPipeName)
{
	CString sReceive;
	int iRet = SendRecive(sPipeName, _T("OpenCamera,0"), &sReceive);
	if(iRet != 0){return -1;}
	BOOL bRet;
	CString sOut;
	CString sRemin;
	bRet = ExtractData(sReceive, _T(","), &sOut, &sRemin);

	if(sOut.Compare(_T("CHANNEL_1_24BGR"))==0)
	{
		m_iChannel=CHANNEL_1_24BGR;
	}
	if(sOut.Compare(_T("CHANNEL_3_8RGB"))==0)
	{
		m_iChannel=CHANNEL_3_8RGB;
	}
	if(sOut.Compare(_T("CHANNEL_1_8"))==0)
	{
		m_iChannel=CHANNEL_1_8;
	}
//	CString sss;
//	sss.Format(_T("%s, %d"), sReceive, m_iChannel);
//	AfxMessageBox(sss);

	bRet = ExtractData(sRemin, _T(","), &sOut, &sRemin);
	m_iWidht=_ttoi(sOut);
	m_iHeight=_ttoi(sRemin);
	
	int iColorsPerPixel;
	switch(m_iChannel)
	{
	case CHANNEL_1_8:{iColorsPerPixel=1; break;}
	case CHANNEL_3_8RGB:{iColorsPerPixel=3; break;}
	case CHANNEL_1_24BGR:{iColorsPerPixel=3; break;}
	}

//	sss.Format(_T("%d, %d, %d"), m_iChannel, m_iWidht, m_iHeight);
//	AfxMessageBox(sss);
	m_hSharedMemory = CreateFileMapping(NULL, NULL, PAGE_READWRITE, NULL, m_iWidht*m_iHeight*iColorsPerPixel, _T("Camera1"));
//	sss.Format(_T("m_hSharedMemory = %d"), m_hSharedMemory);
//	AfxMessageBox(sss);

	m_pbyMemory = (BYTE*)MapViewOfFile(m_hSharedMemory, FILE_MAP_ALL_ACCESS, NULL, NULL, m_iWidht*m_iHeight*iColorsPerPixel);
//	sss.Format(_T("m_pbyMemory = %d"), m_pbyMemory);
//	AfxMessageBox(sss);

	return 0;
}

int CameraLocal::GrabImage(ImgRGB* imgOut)
{
	CString sss;
//	sss.Format(_T("m_hSharedMemory = %d"), m_hSharedMemory);
//	AfxMessageBox(sss);

//	sss.Format(_T("m_pbyMemory = %d"), m_pbyMemory);
//	AfxMessageBox(sss);


	if(m_hSharedMemory == INVALID_HANDLE_VALUE){return -1;}
	if(m_pbyMemory == NULL){return -1;}


	imgOut->Set(m_iWidht, m_iHeight, m_iChannel);

	CString sReceive;
	int iRet = SendRecive(m_sPipeName, _T("GrabImage"), &sReceive);
	if(iRet != 0){return iRet;}
	//	if(sReceive.Compare(_T("Camera1")) != 0){return -1;}

	switch(m_iChannel)
	{
	case CHANNEL_1_8:
		{
			for(int r=0; r<m_iHeight; r++)
			{
				for(int c=0; c<m_iWidht; c++)
				{
					imgOut->byImg[r*m_iWidht+c]=m_pbyMemory[r*m_iWidht+c];
				}
			}
			break;
		}
	case CHANNEL_3_8RGB:
		{
			for(int r=0; r<m_iHeight; r++)
			{
				for(int c=0; c<m_iWidht; c++)
				{
					imgOut->byImgR[r*m_iWidht+c]=m_pbyMemory[m_iWidht*m_iHeight+0 + r*m_iWidht+c];
					imgOut->byImgG[r*m_iWidht+c]=m_pbyMemory[m_iWidht*m_iHeight+1 + r*m_iWidht+c];
					imgOut->byImgB[r*m_iWidht+c]=m_pbyMemory[m_iWidht*m_iHeight*2 + r*m_iWidht+c];
				}
			}
			break;
		}
	case CHANNEL_1_24BGR:
		{
			for(int r=0; r<m_iHeight; r++)
			{
				for(int c=0; c<m_iWidht; c++)
				{
					imgOut->byImg[(r*m_iWidht+c)*3+0]=m_pbyMemory[m_iWidht*m_iHeight+0 + r*m_iWidht+c];
					imgOut->byImg[(r*m_iWidht+c)*3+1]=m_pbyMemory[m_iWidht*m_iHeight+1 + r*m_iWidht+c];
					imgOut->byImg[(r*m_iWidht+c)*3+2]=m_pbyMemory[m_iWidht*m_iHeight+2 + r*m_iWidht+c];
				}
			}
			break;
		}
	}

	return 0;
}

int CameraLocal::CloseCamera()
{
	if(m_hPipe==INVALID_HANDLE_VALUE){return 0;}


	CString sReceive;
	int iRet = SendRecive(m_sPipeName, _T("CloseCamera"), &sReceive);
	if(iRet != 0){return iRet;}

	
	if(m_pbyMemory != NULL){UnmapViewOfFile(m_pbyMemory);}
	m_pbyMemory=NULL;
	if(m_hSharedMemory != INVALID_HANDLE_VALUE){CloseHandle(m_hSharedMemory);}
	m_hSharedMemory = INVALID_HANDLE_VALUE;
	CloseHandle(m_hPipe);
	m_hPipe=INVALID_HANDLE_VALUE;

	m_iChannel=CHANNEL_UNDEFINED;
	m_iWidht=0;
	m_iHeight=0;
	m_sPipeName.Format(_T(""));

	return 0;
}

static CameraLocal s_cameraLocal;
int DLL_IE Camera::OpenCamera(CString sPipeName){return s_cameraLocal.OpenCamera(sPipeName);}
int DLL_IE Camera::CloseCamera(){return s_cameraLocal.CloseCamera();}
int DLL_IE Camera::GrabImage(ImgRGB* imgOut){return s_cameraLocal.GrabImage(imgOut);}