// Video.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VideoCapture.h"
#include "VideoPlay.h"


void	VideoCaptureInit(HWND hWnd)
{
	CVideoCapture::GetInstance()->Init(hWnd);
}

void	VideoCaptureRelease()
{
	CVideoCapture::GetInstance()->Release();
}

void    VideoCaptureStart()
{
	CVideoCapture::GetInstance()->VideoCaptureStart();
}

void	VideoCaptureStop()
{
	CVideoCapture::GetInstance()->VideoCaptureStop();
}

void  VideoEnumCaptureDevice(VECT_CAPTURE_INFO *pVCaptureInfo)
{
	CVideoCapture::GetInstance()->VideoCaptureDeviceEnum(pVCaptureInfo);
}

void VideoCaptureSelectDevice(int nDeviceID)
{
	CVideoCapture::GetInstance()->SelectDevice(nDeviceID);
}


//play
void	VideoPlayInit(HWND hWnd)
{
	CVideoPlay::GetInstance()->Init(hWnd);
}

void	VideoPlayRelease()
{
	CVideoPlay::GetInstance()->Release();
}

void	VideoPlayStart(LPCTSTR lpFile)
{
	CVideoPlay::GetInstance()->VideoPlayStart(lpFile);
}

void	VideoPlayRun()
{
	CVideoPlay::GetInstance()->Run();
}

void	VideoPlayStop()
{
	CVideoPlay::GetInstance()->VideoPlayStop();
}

void	VideoPlayPause()
{
	CVideoPlay::GetInstance()->VideoPlayPause();
}

void  SnapshotBitmap(LPCTSTR lpFile)
{
	CVideoPlay::GetInstance()->SnapshotBitmap(lpFile);
}

void	VideoAddFilter(const GUID& clsid, LPCWCHAR wszName, IBaseFilter **ppF)
{
	CVideoPlay::GetInstance()->AddFilterByCLSID(clsid, wszName, ppF);
}

void	VideoConnectFilter(IBaseFilter *pSrc, IBaseFilter *pDest)
{
	CVideoPlay::GetInstance()->ConnectFilters(pSrc, pDest);
}