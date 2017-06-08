// Audio.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AUDIO_SDK.h"
#include "AudioPlay.h"
#include "AudioRecord.h"

CAudioPlay* CAudioPlay::s_Instance = NULL;

void AudioPlayInit(HWND hWnd, int cooperateLevel)
{
	CAudioPlay::GetInstance()->Init(hWnd,cooperateLevel);
}

void AudioPlayRelease()
{
	CAudioPlay::GetInstance()->Release();
}

void AudioPlayStart(LPCTSTR lpFileName)
{
	CAudioPlay::GetInstance()->Start(lpFileName);
}

void AudioPlayStop()
{
	CAudioPlay::GetInstance()->Stop();
}

void AudioPlayPause()
{

}

void AudioPlayContinue()
{

}


void AudioRecordInit(LPGUID guid, int samplePerSec, int bitsPerSameple, int channels)
{
	CAudioRecord::GetInstance()->Init(guid, samplePerSec, bitsPerSameple, channels);
}

void AudioRecordRelease()
{
	CAudioRecord::GetInstance()->Release();
}

void AudioRecordStart(LPCTSTR lpFileName)
{
	CAudioRecord::GetInstance()->Start(lpFileName);
}

void AudioRecordStop()
{
	CAudioRecord::GetInstance()->Stop();
}

void AduioRecordSetCallBack(AudioPlayCallBack pCallBack, void *context)
{
	CAudioRecord::GetInstance()->SetAudioRecordCallback(pCallBack,context);
}

void AudioREnumSoundCard(VECT_SOUNDCARD_INFO *vSoundCardInfo)
{
	CAudioRecord::GetInstance()->EnumSoundCardInfo(vSoundCardInfo);
}