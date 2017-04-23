// Audio.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AUDIO_SDK.h"
#include "AudioPlay.h"
#include "AudioRecord.h"

void AudioPlayInit(HWND hWnd, int cooperateLevel)
{
	CAudioPlay::get_instance()->Init(hWnd,cooperateLevel);
}

void AudioPlayRelease()
{
	CAudioPlay::get_instance()->Release();
}

void AudioPlayStart(LPCTSTR lpFileName)
{
	CAudioPlay::get_instance()->Start(lpFileName);
}

void AudioPlayStop()
{
	CAudioPlay::get_instance()->Stop();
}

void AudioPlayPause()
{

}

void AudioPlayContinue()
{

}


void AudioRecordInit(LPGUID guid, int samplePerSec, int bitsPerSameple, int channels)
{
	CAudioRecord::get_instance()->Init(guid, samplePerSec, bitsPerSameple, channels);
}

void AudioRecordRelease()
{
	CAudioRecord::get_instance()->Release();
}

void AudioRecordStart(LPCTSTR lpFileName)
{
	CAudioRecord::get_instance()->Start(lpFileName);
}

void AudioRecordStop()
{
	CAudioRecord::get_instance()->Stop();
}

void AduioRecordSetCallBack(AudioPlayCallBack pCallBack, void *context)
{
	CAudioRecord::get_instance()->SetAudioRecordCallback(pCallBack,context);
}

void AudioREnumSoundCard(VECT_SOUNDCARD_INFO *vSoundCardInfo)
{
	CAudioRecord::get_instance()->EnumSoundCardInfo(vSoundCardInfo);
}