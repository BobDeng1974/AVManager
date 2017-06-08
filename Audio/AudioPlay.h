#ifndef DIRECT_SOUND_H_
#define DIRECT_SOUND_H_

#include "AUDIO_SDK.h"


class CAudioPlay
{
private:
	CAudioPlay();
	~CAudioPlay();

public:
	static CAudioPlay* GetInstance();
	static CAudioPlay* s_Instance;

	void Init(HWND hWnd, int cooperativeLevel = 2);
	void Release();

	
	BOOL Start(LPCTSTR lpFile);
	BOOL Stop();
	int  Execute();

	BOOL OpenFile(LPCTSTR lpFile);
	BOOL CreateSoundBuffer();
	LRESULT CopyDataToSoundBuffer();

	static unsigned int __stdcall StartAddr(void* param);
private:
	AudioPlayCallBack	m_pPlayCallback;
	void* m_pPlayContext;
	LPDIRECTSOUND8 m_lpDS;
	LPDIRECTSOUNDBUFFER8 m_pDSB8;
	HANDLE m_hEvent[2];

	LPDIRECTSOUNDNOTIFY8 m_pDSNotify;; 
	DSBPOSITIONNOTIFY	m_DSNotify[2];//����֪ͨ��־������
	DWORD  m_dwNextWriteOffset;

	WAVEFORMATEX m_waveFormat;
	HANDLE		 m_hFile;
	BOOL		 m_bIsPlaying;
	BOOL		m_bRunning;
};



#endif