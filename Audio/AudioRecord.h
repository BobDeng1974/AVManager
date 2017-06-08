#ifndef AUDIO_RECORDER_H_
#define AUDIO_RECORDER_H_

#include "AUDIO_SDK.h"
#include "WaveFile.h"

class CAudioRecord 
{
private:
	CAudioRecord();
	~CAudioRecord();
public:
	static CAudioRecord* GetInstance();
	static CAudioRecord* s_Instance;

	LRESULT	Init(LPGUID pGuid,int samplePerSec, int bitsPerSameple, int channels);
	void	Release();
	void	SetAudioRecordCallback(AudioRecordCallBack callback, void* Context);
	int		EnumSoundCardInfo(VECT_SOUNDCARD_INFO *soundInfo);
	static  BOOL CALLBACK DsEnumSDInfoProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext );


	BOOL	Start(LPCTSTR lpFileName);
	void	Stop();
	static  LRESULT RecordThread(LPVOID lpParam);
	HRESULT CreateSoundBuffer();

	LRESULT	CreateSoundFile(LPCTSTR lpFileName);

	static unsigned int __stdcall StartAddr(void* param);
	int	Execute();
private:
	AudioRecordCallBack m_pCallBack;
	void*				m_pContext;
	vector<SoundCardInfo>    m_vSoundCardInfo;

	LPDIRECTSOUNDCAPTURE8 m_lpDSC;               //¶ÔÏó
	LPDIRECTSOUNDCAPTUREBUFFER8 m_pDSB8;  //»º³åÇø
	LPDIRECTSOUNDNOTIFY8 m_pDSNotify;
	DSBPOSITIONNOTIFY    m_Notify[2];
	HANDLE				 m_hEvent[2];
	DWORD				 m_nextCaptureOffset;
	WAVEFORMATEX		 m_wformat;
	BOOL				 m_bIsRecording;
	BOOL				 m_bIsCreated;
	CWaveFile			 m_waveFile;
	BOOL				m_bRunning;
};


#endif