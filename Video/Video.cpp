// Video.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VideoCapture.h"
#include "VideoPlay.h"


void	VideoCaptureInit(HWND hWnd)
{
	CVideoCapture::get_instance()->Init(hWnd);
}

void	VideoCaptureRelease()
{
	CVideoCapture::get_instance()->Release();
}

void    VideoCaptureStart()
{
	CVideoCapture::get_instance()->VideoCaptureStart();
}

void	VideoCaptureStop()
{
	CVideoCapture::get_instance()->VideoCaptureStop();
}

void  VideoEnumCaptureDevice(VECT_CAPTURE_INFO *pVCaptureInfo)
{
	CVideoCapture::get_instance()->VideoCaptureDeviceEnum(pVCaptureInfo);
}

void VideoCaptureSelectDevice(int nDeviceID)
{
	CVideoCapture::get_instance()->SelectDevice(nDeviceID);
}


//play
void	VideoPlayInit(HWND hWnd)
{
	CVideoPlay::get_instance()->Init(hWnd);
}

void	VideoPalyRelease()
{
	CVideoPlay::get_instance()->Release();
}

void	VideoPlayStart(LPCTSTR lpFile)
{
	CVideoPlay::get_instance()->VideoPlayStart(lpFile);
}

void	VideoPlayStop()
{
	CVideoPlay::get_instance()->VideoPlayStop();
}

void	VideoPlayPause()
{
	CVideoPlay::get_instance()->VideoPlayPause();
}

void  SnapshotBitmap(LPCTSTR lpFile)
{
	CVideoPlay::get_instance()->SnapshotBitmap(lpFile);
}