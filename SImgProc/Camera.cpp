#include "stdafx.h"

#include "camera.h"
#include "CommonFunction.h"

	HANDLE hPipe = INVALID_HANDLE_VALUE;

int SendRecive(CString sPipeName, CString sSend, CString* sReceive)
{
	
	BOOL bRet;

	CString sPipeFullName;
	sPipeFullName.Format(_T("\\\\.\\pipe\\%s"), sPipeName);
	

	if(hPipe==INVALID_HANDLE_VALUE)
	{
	hPipe = CreateFile(sPipeFullName,
		PIPE_ACCESS_DUPLEX,
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	}
	if (INVALID_HANDLE_VALUE == hPipe) 
	{
		CloseHandle(hPipe);
		return -1;
	}


	TCHAR buffer[256];
	ZeroMemory(buffer, sizeof(buffer));
	_stprintf(buffer, _T("%s"), sSend);
	DWORD writtenWordSize = 0;
	DWORD numberOfByteToWrite = _tcslen(buffer) * sizeof(TCHAR);
	BOOL writeRes = WriteFile(hPipe, buffer, numberOfByteToWrite, (LPDWORD)&writtenWordSize, NULL);

	ZeroMemory(buffer, 256);
	DWORD readDataSize = 0;
	BOOL readRes = ReadFile(hPipe, buffer, sizeof(buffer), (LPDWORD)&readDataSize, NULL);
	CString sReadBuffer;
	sReadBuffer.Format(_T("%s"), buffer);

	sReceive->Format(_T("read: %d\n%s"),readDataSize,buffer);

//	CloseHandle(hPipe);
	return 0;
}

int DLL_IE OpenCamera(CString sPipeName)
{
	CString sReceive;
	SendRecive(sPipeName, _T("OpenCamera"), &sReceive);
	return 0;

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	BOOL bRet;

	CString sPipeFullName;
	sPipeFullName.Format(_T("\\\\.\\pipe\\%s"), sPipeName);
	


	hPipe = CreateFile(sPipeFullName,
		PIPE_ACCESS_DUPLEX,
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (INVALID_HANDLE_VALUE == hPipe) 
	{
		AfxMessageBox(_T("Can not create file, as PIPE"));
		CloseHandle(hPipe);
		return -1;
	}

	AfxMessageBox(_T("Can create file, as PIPE"));

	TCHAR buffer[256];
	ZeroMemory(buffer, sizeof(buffer));
	_stprintf(buffer, _T("OpenCamera"));
	DWORD writtenWordSize = 0;
	DWORD numberOfByteToWrite = _tcslen(buffer) * sizeof(TCHAR);
	BOOL writeRes = WriteFile(hPipe, buffer, numberOfByteToWrite, (LPDWORD)&writtenWordSize, NULL);

	ZeroMemory(buffer, 256);
	DWORD readDataSize = 0;
	BOOL readRes = ReadFile(hPipe, buffer, sizeof(buffer), (LPDWORD)&readDataSize, NULL);
	CString sReadBuffer;
	sReadBuffer.Format(_T("%s"), buffer);
	CString sss;
	sss.Format(_T("read: %d\n%s"),readDataSize,buffer);
	AfxMessageBox(sss);



	CloseHandle(hPipe);
	return 0;
}

int DLL_IE GrabImage(CString sPipeName, ImgRGB* imgOut)
{
	imgOut->Set(640, 480, CHANNEL_3_8);

	  HANDLE hSharedMemory2 ;
	BYTE* pMemory2;

	CString sReceive;
	SendRecive(sPipeName, _T("GrabImage"), &sReceive);

//	if(sReceive.Compare(_T("Camera1")) != 0){return -1;}

	hSharedMemory2 = CreateFileMapping(NULL, NULL, PAGE_READWRITE, NULL, 640*480*3, _T("Camera1"));
  pMemory2 = (BYTE*)MapViewOfFile(hSharedMemory2, FILE_MAP_ALL_ACCESS, NULL, NULL, 640*480*3);

  for(int r=0; r<480; r++)
  {
	  for(int c=0; c<640; c++)
	  {
		  imgOut->byImgR[r*640+c]=pMemory2[r*640+c];
		  imgOut->byImgG[r*640+c]=pMemory2[r*640+c+640*480];
		  imgOut->byImgB[r*640+c]=pMemory2[r*640+c+640*480*2];
	  }
  }

	return 0;
}
int DLL_IE CloseCamera(CString sPipeName)
{
	
	CString sReceive;
	SendRecive(sPipeName, _T("CloseCamera"), &sReceive);
	return 0;

	/*
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	BOOL bRet;

	CString sPipeFullName;
	sPipeFullName.Format(_T("\\\\.\\pipe\\%s"), sPipeName);
	


	hPipe = CreateFile(sPipeFullName,
		PIPE_ACCESS_DUPLEX,
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (INVALID_HANDLE_VALUE == hPipe) 
	{
		AfxMessageBox(_T("Can not create file, as PIPE"));
		CloseHandle(hPipe);
		return -1;
	}

	AfxMessageBox(_T("Can create file, as PIPE"));

	char buffer[256];
	ZeroMemory(buffer, sizeof(buffer));
	sprintf(buffer, "CloseCamera");
	DWORD writtenWordSize = 0;
	DWORD numberOfByteToWrite = strlen(buffer) * sizeof(TCHAR);
	BOOL writeRes = WriteFile(hPipe, buffer, numberOfByteToWrite, (LPDWORD)&writtenWordSize, NULL);


//	AfxMessageBox(_T("Can create file, as PIPE"));
*/
	/*
	TCHAR buffer[256];
	ZeroMemory(buffer, sizeof(buffer));
	_stprintf_s(buffer, _T("OpenCamera"));
	DWORD writtenWordSize = 0;
	DWORD numberOfByteToWrite = _tcsclen(buffer) * sizeof(TCHAR);
	BOOL writeRes = WriteFile(hPipe, buffer, numberOfByteToWrite, (LPDWORD)&writtenWordSize, NULL);
	*/
	/*
	buffer[256];
	ZeroMemory(buffer, 256);
	DWORD readDataSize = 0;
	BOOL readRes = ReadFile(hPipe, buffer, sizeof(buffer), (LPDWORD)&readDataSize, NULL);
	CString sReadBuffer;
	sReadBuffer.Format(_T("%s"), buffer);
	CString sss;
	sss.Format(_T("read: %d\n%s"),readDataSize,buffer);
//	AfxMessageBox(sss);



	CloseHandle(hPipe);
	*/
	return 0;
}
