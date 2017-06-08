#ifndef VIDEO_CAPTURE_H_
#define VIDEO_CAPTURE_H_

#include "VIDEO_SDK.H"

class CVideoCapture
{
private:
	CVideoCapture();
	~CVideoCapture();
public:
	static CVideoCapture* GetInstance();
	static CVideoCapture* s_Instance;

	HRESULT	Init(HWND hWnd);
	void	Release();

	bool	VideoCaptureDeviceEnum(VECT_CAPTURE_INFO *pVCaptureInfo);
	void	SelectDevice(int nDeviceID);
	BOOL	VideoCaptureStart();
	BOOL	VideoCaptureStop();
	BOOL	SetDisPlayWindow(HWND hWnd);
private:
	BOOL	QueryInterfaces();
private:
	ICaptureGraphBuilder2* m_pBuilder;
	IFilterGraph2     *m_pGraph; 
	IMediaControl     *m_pMediaControl ; 
	IVideoWindow	  *m_pVideoWindow;
	VECT_CAPTURE_INFO m_vCaptureInfo;
	VideoCaptureInfo  *m_captureInfo;
	HWND			  m_hParent;
	bool	m_bPlaying;
};

#endif