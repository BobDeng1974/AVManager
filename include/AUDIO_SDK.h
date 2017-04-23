#ifndef AUDIO_SDK_H_
#define AUDIO_SDK_H_


#ifdef	AUDIO_EXPORTS
#define AUDIO_API    extern "C" __declspec(dllexport)  
#else
#define AUDIO_API    extern "C" /*__declspec(dllimport)*/
#endif


	
#define DS_NORMAL  1  
#define	DS_PRIORITY 2
#define	DS_EXCLUSIVE 3
#define	DS_WRITEPRIMARY 4

//采样参数
#define SAMPLE_PER_SEC   44100
#define BITS_PER_SAMPLE  16
#define CHANNELS		 2


struct SoundCardInfo
{
	GUID Guid;
	TCHAR description[100];
};
typedef   std::vector<SoundCardInfo> VECT_SOUNDCARD_INFO;



typedef void (* AudioPlayCallBack)(uint32 type, WPARAM wParam, LPARAM lParam, void* pContent);
typedef void ( * AudioRecordCallBack )(uint32 type, WPARAM wParam, LPARAM lParam, void* pContext);

//Play
AUDIO_API void AudioPlayInit(HWND hWnd, int cooperateLevel = DS_PRIORITY);

AUDIO_API void AudioPlayRelease();

AUDIO_API void AudioPlayStart(LPCTSTR lpFileName);

AUDIO_API void AudioPlayStop();

AUDIO_API void AudioPlayPause();

AUDIO_API void AudioPlayContinue();

AUDIO_API void AudioPEnumSoundCard(VECT_SOUNDCARD_INFO *vSoundCardInfo);


//Recorder
AUDIO_API void AudioRecordInit(LPGUID guid = NULL, int samplePerSec = SAMPLE_PER_SEC, int bitsPerSameple = BITS_PER_SAMPLE, int channels = CHANNELS);

AUDIO_API void AudioRecordRelease();

AUDIO_API void AudioRecordStart(LPCTSTR lpFileName);

AUDIO_API void AudioRecordStop();

AUDIO_API void AduioRecordSetCallBack(AudioPlayCallBack pCallBack, void *context);

AUDIO_API void AudioREnumSoundCard(VECT_SOUNDCARD_INFO *vSoundCardInfo);
#endif