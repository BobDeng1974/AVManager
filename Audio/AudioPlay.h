#ifndef DIRECT_SOUND_H_
#define DIRECT_SOUND_H_

#include "AUDIO_SDK.h"


class CAudioPlay : public c_singleton<CAudioPlay>
	, public c_thread
{
public:
	CAudioPlay();
	~CAudioPlay();

	void Init(HWND hWnd, int cooperativeLevel = 2);
	void Release();

	
	BOOL Start(LPCTSTR lpFile);
	BOOL Stop();
	int  Execute();

	BOOL OpenFile(LPCTSTR lpFile);
	BOOL CreateSoundBuffer();
	LRESULT CopyDataToSoundBuffer();
private:
	AudioPlayCallBack	m_pPlayCallback;
	void* m_pPlayContext;
	LPDIRECTSOUND8 m_lpDS;
	LPDIRECTSOUNDBUFFER8 m_pDSB8;
	HANDLE m_hEvent[2];

	LPDIRECTSOUNDNOTIFY8 m_pDSNotify;; 
	DSBPOSITIONNOTIFY	m_DSNotify[2];//设置通知标志的数组
	DWORD  m_dwNextWriteOffset;

	WAVEFORMATEX m_waveFormat;
	HANDLE		 m_hFile;
	BOOL		 m_bIsPlaying;
};



#endif