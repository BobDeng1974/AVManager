#ifndef VIDEO_SDK_H_
#define VIDEO_SDK_H_

#define WM_GRAPHNOTIFY  (WM_USER + 0x0001)

#ifdef  VIDEO_EXPORTS
#define VIDEO_API    extern "C" __declspec(dllexport)  
#else
#define VIDEO_API    extern "C" /*__declspec(dllimport)*/
#endif


struct VideoCaptureInfo
{
	int	nDeviceID;
	IBaseFilter*	filter;
	TCHAR description[100];
};
typedef   std::vector<VideoCaptureInfo> VECT_CAPTURE_INFO;


//Play
VIDEO_API void	VideoPlayInit(HWND hWnd);

VIDEO_API void	VideoPlayRelease();

VIDEO_API void	VideoPlayStart(LPCTSTR lpFile);

VIDEO_API void	VideoPlayRun();

VIDEO_API void	VideoPlayStop();

VIDEO_API void  SnapshotBitmap(LPCTSTR lpFile);

VIDEO_API void	VideoAddFilter(const GUID& clsid, LPCWCHAR wszName, IBaseFilter **ppF);

VIDEO_API void VideoDelFilter(IBaseFilter *pF);

VIDEO_API void	VideoConnectFilter(IBaseFilter *pSrc, IBaseFilter *pDest);

VIDEO_API void	VideoDisConnectFilter(IBaseFilter *pSrc, IBaseFilter *pDest);

//Capture
VIDEO_API void	VideoCaptureInit(HWND hWnd);

VIDEO_API void	VideoCaptureRelease();

VIDEO_API void	VideoCaptureStart();

VIDEO_API void	VideoCaptureStop();

VIDEO_API void  VideoEnumCaptureDevice(VECT_CAPTURE_INFO *pVCaptureInfo);

VIDEO_API void VideoCaptureSelectDevice(int nDeviceID);
#endif