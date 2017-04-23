#ifndef	VIDEO_PLAY_H_
#define VIDEO_PLAY_H_


#include "VIDEO_SDK.H"


class CVideoPlay : public c_singleton<CVideoPlay>
{
private:
	IFilterGraph2	*m_pGraph;			//�˲������������
	IMediaControl	*m_pMediaControl;	//ý����ƽӿڣ���run��stop��pause
	IMediaEventEx	*m_pMediaEvent;		//ý���¼��ӿ�
	IBasicVideo		*m_pBasicVideo;		//��Ƶ�����ӿ�
	IBasicAudio		*m_pBasicAudio;		//��Ƶ�����ӿ�
	IVideoWindow	*m_pVideoWindow;	//��Ƶ���ڽӿ�
	IMediaSeeking	*m_pMediaSeeking;	//ý�嶨λ�ӿ�
	HWND			m_hParent;

	DWORD				mObjectTableEntry; 

public:
	CVideoPlay();
	virtual ~CVideoPlay();

public:
	virtual bool Init(HWND hWnd);		//�����˲������������
	virtual void Release(void);		//�ͷ����нӿ�

	IMediaEventEx * GetEventHandle(void);	//����IMediaEventExָ��

	//�������ŷ��������˲���
	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	//�Ͽ������˲���
	void DisconnectFilters(IPin * inOutputPin);
	//������ʾ����
	bool SetDisplayWindow(HWND inWindow);
	//���ô���֪ͨ��Ϣ
	bool SetNotifyWindow(HWND inWindow);
	//�����¼�
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	//ý������״̬
	bool VideoPlayStart(LPCTSTR lpFile);        // Control filter graph
	bool VideoPlayStop(void);
	bool VideoPlayPause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	//������ʾ����ȫ����ʾ
	bool SetFullScreen(BOOL inEnabled);
	bool GetFullScreen(void);

	// ý�嶨λ
	bool GetCurrentPosition(double * outPosition);
	bool GetStopPosition(double * outPosition);
	bool SetCurrentPosition(double inPosition);
	bool SetStartStopPosition(double inStart, double inStop);
	bool GetDuration(double * outDuration);
	bool SetPlaybackRate(double inRate);

	//����ý������: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);
	long GetAudioVolume(void);

	//������Ƶƽ��: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	//ץͼ
	bool SnapshotBitmap(const TCHAR *outFile);//const char * outFile);

	int m_nVolume;
	void ChangeAudioVolume(int nVolume);

	//��������
	void Mute();
	void UnMute();

private:
	//��ѯ�йؽӿ�
	bool QueryInterfaces(void);
};

#endif