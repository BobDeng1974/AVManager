#ifndef AUDIO_RECORDER_H_
#define AUDIO_RECORDER_H_

#include "AUDIO_SDK.h"
#include "WaveFile.h"

class CAudioRecord : public c_thread 
	,public c_singleton<CAudioRecord>
{
public:
	CAudioRecord();
	~CAudioRecord();

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

	virtual int Execute();
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
};


#endif