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
	if (m_hPipe == INVALID_HANDLE_VALUE) {CloseHandle(m_hPipe); m_hPipe=INVALID_HANDLE_VALUE; return -1;}


	TCHAR tchBuffer[256];
	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	_stprintf_s(tchBuffer, _T("%s"), (LPCTSTR)sSend);
	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwNumberOfByteToWrite = (_tcslen(tchBuffer)+1) * sizeof(TCHAR);
	bRet = WriteFile(m_hPipe, tchBuffer, dwNumberOfByteToWrite, (LPDWORD)&dwNumberOfBytesWritten, NULL);
	if(bRet != TRUE){CloseHandle(m_hPipe); m_hPipe=INVALID_HANDLE_VALUE; return -1;}

	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	DWORD dwNumberOfBytesRead = 0;
	bRet = ReadFile(m_hPipe, tchBuffer, sizeof(tchBuffer), (LPDWORD)&dwNumberOfBytesRead, NULL);

	sReceive->Format(_T("%s"), tchBuffer);

	return 0;
}

int CameraLocal::OpenCamera(CString sPipeName)
{
	CString sReceive;
	int iRet = SendRecive(sPipeName, _T("OpenCamera"), &sReceive);
	if(iRet != 0){return -1;}

	CString sOut;
	CString sRemin;
	BOOL bRet;
	bRet = ExtractData(sReceive, _T(","), &sOut, &sRemin);

	if(sOut.Compare(_T("CHANNEL_1_24BGR"))==0){m_iChannel=CHANNEL_1_24BGR;}
	if(sOut.Compare(_T("CHANNEL_3_8RGB"))==0){m_iChannel=CHANNEL_3_8RGB;}
	if(sOut.Compare(_T("CHANNEL_1_8"))==0){m_iChannel=CHANNEL_1_8;}

	bRet = ExtractData(sRemin, _T(","), &sOut, &sRemin);
	m_iWidth=_ttoi(sOut);
	m_iHeight=_ttoi(sRemin);
	
	int iColorsPerPixel;
	switch(m_iChannel)
	{
	case CHANNEL_1_8:{iColorsPerPixel=1; break;}
	case CHANNEL_3_8RGB:{iColorsPerPixel=3; break;}
	case CHANNEL_1_24BGR:{iColorsPerPixel=3; break;}
	}

	if(m_hSharedMemory != INVALID_HANDLE_VALUE){CloseHandle(m_hSharedMemory); m_hSharedMemory = INVALID_HANDLE_VALUE;}
	m_hSharedMemory = CreateFileMapping(NULL, NULL, PAGE_READWRITE, NULL, m_iWidth*m_iHeight*iColorsPerPixel, _T("Camera1"));
	if(m_hSharedMemory == INVALID_HANDLE_VALUE){CloseCamera(); return -1;}

	if(m_pbyMemory != NULL){UnmapViewOfFile(m_pbyMemory); m_pbyMemory = NULL;}
	m_pbyMemory = (BYTE*)MapViewOfFile(m_hSharedMemory, FILE_MAP_ALL_ACCESS, NULL, NULL, m_iWidth*m_iHeight*iColorsPerPixel);
	if(m_pbyMemory == NULL){CloseCamera(); return -1;}

	return 0;
}

int CameraLocal::GrabImage(ImgRGB* imgOut)
{
	if(m_hSharedMemory == INVALID_HANDLE_VALUE){return -1;}
	if(m_pbyMemory == NULL){return -1;}


	imgOut->Set(m_iWidth, m_iHeight, m_iChannel);

	CString sReceive;
	int iRet = SendRecive(m_sPipeName, _T("GrabImage"), &sReceive);
	if(iRet != 0){return iRet;}
	//	if(sReceive.Compare(_T("Camera1")) != 0){return -1;}

	switch(m_iChannel)
	{
	case CHANNEL_1_8:{ break;}
	case CHANNEL_3_8RGB:
		{
			for(int r=0; r<m_iHeight; r++)
			{
				for(int c=0; c<m_iWidth; c++)
				{
					imgOut->byImgR[r*m_iWidth+c]=m_pbyMemory[m_iWidth*m_iHeight*0 + r*m_iWidth+c];
					imgOut->byImgG[r*m_iWidth+c]=m_pbyMemory[m_iWidth*m_iHeight*1 + r*m_iWidth+c];
					imgOut->byImgB[r*m_iWidth+c]=m_pbyMemory[m_iWidth*m_iHeight*2 + r*m_iWidth+c];
				}
			}
			break;
		}
	case CHANNEL_1_24BGR:{break;}
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
	if(m_hSharedMemory != INVALID_HANDLE_VALUE){CloseHandle(m_hSharedMemory);}
	CloseHandle(m_hPipe);
	m_hPipe=INVALID_HANDLE_VALUE;

	m_iChannel=CHANNEL_UNDEFINED;
	m_iWidth=0;
	m_iHeight=0;
	m_sPipeName.Format(_T(""));

	return 0;
}
int CameraLocal::SetParameter(CString sProp, CString sParam)
{
	if(m_hPipe==INVALID_HANDLE_VALUE){return 0;}


	CString sReceive;
	CString sSend; 
	sSend.Format(_T("SetParameter(%s,%s)"),sProp, sParam);
	AfxMessageBox(sSend);
	int iRet = SendRecive(m_sPipeName, sSend, &sReceive);
	if(iRet != 0){return iRet;}
	if(sReceive.CompareNoCase(_T("failed"))==0){return -1;}


	return 0;
}
static CameraLocal s_cameraLocal;
int DLL_IE Camera::OpenCamera(CString sPipeName){return s_cameraLocal.OpenCamera(sPipeName);}
int DLL_IE Camera::CloseCamera(){return s_cameraLocal.CloseCamera();}
int DLL_IE Camera::GrabImage(ImgRGB* imgOut){return s_cameraLocal.GrabImage(imgOut);}
int DLL_IE Camera::SetParameter(CString sProp, CString sParam){return s_cameraLocal.SetParameter(sProp, sParam);}