#ifndef	VIDEO_PLAY_H_
#define VIDEO_PLAY_H_


#include "VIDEO_SDK.H"


class CVideoPlay : public c_singleton<CVideoPlay>
{
private:
	IFilterGraph2	*m_pGraph;			//滤波器链表管理器
	IMediaControl	*m_pMediaControl;	//媒体控制接口，如run、stop、pause
	IMediaEventEx	*m_pMediaEvent;		//媒体事件接口
	IBasicVideo		*m_pBasicVideo;		//视频基本接口
	IBasicAudio		*m_pBasicAudio;		//音频基本接口
	IVideoWindow	*m_pVideoWindow;	//视频窗口接口
	IMediaSeeking	*m_pMediaSeeking;	//媒体定位接口
	HWND			m_hParent;

	DWORD				mObjectTableEntry; 

public:
	CVideoPlay();
	virtual ~CVideoPlay();

public:
	virtual bool Init(HWND hWnd);		//生成滤波器链表管理器
	virtual void Release(void);		//释放所有接口

	IMediaEventEx * GetEventHandle(void);	//返回IMediaEventEx指针

	//根据引脚方向连接滤波器
	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	//断开连接滤波器
	void DisconnectFilters(IPin * inOutputPin);
	//设置显示窗口
	bool SetDisplayWindow(HWND inWindow);
	//设置窗口通知消息
	bool SetNotifyWindow(HWND inWindow);
	//处理事件
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	//媒体运行状态
	bool VideoPlayStart(LPCTSTR lpFile);        // Control filter graph
	bool VideoPlayStop(void);
	bool VideoPlayPause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	//设置显示窗口全屏显示
	bool SetFullScreen(BOOL inEnabled);
	bool GetFullScreen(void);

	// 媒体定位
	bool GetCurrentPosition(double * outPosition);
	bool GetStopPosition(double * outPosition);
	bool SetCurrentPosition(double inPosition);
	bool SetStartStopPosition(double inStart, double inStop);
	bool GetDuration(double * outDuration);
	bool SetPlaybackRate(double inRate);

	//设置媒体音量: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);
	long GetAudioVolume(void);

	//设置音频平衡: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	//抓图
	bool SnapshotBitmap(const TCHAR *outFile);//const char * outFile);

	int m_nVolume;
	void ChangeAudioVolume(int nVolume);

	//静音开关
	void Mute();
	void UnMute();

private:
	//查询有关接口
	bool QueryInterfaces(void);
};

#endif