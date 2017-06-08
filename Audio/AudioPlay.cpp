#include "StdAfx.h"
#include "AudioPlay.h"
#include <process.h>


CAudioPlay::CAudioPlay()
{
	m_dwNextWriteOffset = 0;
	m_hFile = NULL;
	m_bIsPlaying = FALSE;
	m_bRunning = FALSE;
}

CAudioPlay::~CAudioPlay()
{

}

CAudioPlay* CAudioPlay::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new CAudioPlay();
	}
	return s_Instance;
}

void CAudioPlay::Init(HWND hWnd, int cooperativeLevel /* = 2 */)
{
	// Create DirectSound.
	if ( DS_OK != DirectSoundCreate8(NULL, &m_lpDS, NULL))
	{
		return ;
	}

	// Set cooperative level.
	if (DS_OK != m_lpDS->SetCooperativeLevel(hWnd, cooperativeLevel))
	{
		return ;
	}
}

void CAudioPlay::Release()
{
	if (m_lpDS)  m_lpDS->Release();
}

BOOL CAudioPlay::Start(LPCTSTR lpFile)
{
	if (m_bIsPlaying != FALSE)	Stop();
	
	if (!OpenFile(lpFile)) return FALSE;
	CreateSoundBuffer();
	CopyDataToSoundBuffer();//先拷贝2秒的数据准备播放
	m_pDSB8->SetCurrentPosition(0);
	m_pDSB8->Play(0,0,DSBPLAY_LOOPING);
	unsigned threadId = 0;
	HANDLE h = (HANDLE)_beginthreadex(NULL, NULL, StartAddr, this, 0, &threadId);
	if (h != INVALID_HANDLE_VALUE)
	{
		m_bRunning = TRUE;
		CloseHandle(h);
	}
	return TRUE;
}

BOOL CAudioPlay::Stop()
{
	if (m_pDSB8 != NULL)
	{
		m_pDSB8->Stop();
		m_pDSB8->Release();
		m_pDSB8 = NULL;
	}

	if (m_hFile != NULL)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (m_hEvent != NULL)
	{
		CloseHandle(m_hEvent[0]);
		CloseHandle(m_hEvent[1]);
		
	}
	m_dwNextWriteOffset = 0;
	m_bIsPlaying = 0;
	m_bRunning = FALSE;
	return TRUE;
}

unsigned int CAudioPlay::StartAddr(void* param)
{
	CAudioPlay* pThis = static_cast<CAudioPlay*>(param);
	pThis->Execute();
	return 0;
}

int CAudioPlay::Execute()
{
	m_bIsPlaying = TRUE;
	while (m_bRunning)
	{		
		DWORD res = WaitForMultipleObjects (2, m_hEvent, FALSE, INFINITE);
		if(res >= WAIT_OBJECT_0)
		{
			CopyDataToSoundBuffer();
		}
	}
	return 0;
}

BOOL CAudioPlay::OpenFile(LPCTSTR lpFile)
{
	DWORD nLen=0;
	m_hFile = (HANDLE)CreateFile(lpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}
	SetFilePointer(m_hFile,20,NULL,FILE_BEGIN);
	ReadFile(m_hFile,&m_waveFormat,1*sizeof(WAVEFORMATEX),&nLen,NULL);
	SetFilePointer(m_hFile,20 + sizeof(WAVEFORMATEX),NULL,FILE_BEGIN);
	return TRUE;
}

BOOL CAudioPlay::CreateSoundBuffer()
{
	DSBUFFERDESC dsbDesc;                //description
	LPDIRECTSOUNDBUFFER pDsb = NULL;	 //buffer


	memset(&dsbDesc, 0, sizeof(DSBUFFERDESC)); 
	dsbDesc.dwSize = sizeof(DSBUFFERDESC); 
	dsbDesc.dwFlags =  DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFX| DSBCAPS_CTRLVOLUME \
		| DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbDesc.dwBufferBytes = 4 * m_waveFormat.nAvgBytesPerSec;  //4second
	dsbDesc.lpwfxFormat = &m_waveFormat; 

	if (DS_OK == m_lpDS->CreateSoundBuffer(&dsbDesc, &pDsb, NULL))
	{ 
		pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pDSB8);
		pDsb->Release();
	}
	//设置DirectSound通知 机制
	m_hEvent[0] = CreateEvent(NULL,false,false,NULL);
	m_hEvent[1] = CreateEvent(NULL,false,false,NULL);
	m_DSNotify[0].dwOffset = m_waveFormat.nAvgBytesPerSec - 1;   //1秒通知
	m_DSNotify[1].dwOffset = 3*m_waveFormat.nAvgBytesPerSec - 1; //3秒通知
	m_DSNotify[0].hEventNotify = m_hEvent[0];
	m_DSNotify[1].hEventNotify = m_hEvent[1];

	if(FAILED(pDsb->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify )))	return FALSE;
	m_pDSNotify->SetNotificationPositions(2,m_DSNotify);
	m_pDSNotify->Release();

	return TRUE;
}

LRESULT CAudioPlay::CopyDataToSoundBuffer()
{
	HRESULT hr;
	DWORD dwBytesWrittenToBuffer = 0;
	VOID* pDSLockedBuffer = NULL;
	VOID* pDSLockedBuffer2 = NULL;
	DWORD dwDSLockedBufferSize;
	DWORD dwDSLockedBufferSize2;


	hr = m_pDSB8->Lock(m_dwNextWriteOffset,m_waveFormat.nAvgBytesPerSec *2 ,&pDSLockedBuffer,&dwDSLockedBufferSize, &pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
	if(hr == DSERR_BUFFERLOST)
	{
		m_pDSB8->Restore();
		m_pDSB8->Lock(m_dwNextWriteOffset,m_waveFormat.nAvgBytesPerSec *2 ,&pDSLockedBuffer,&dwDSLockedBufferSize,
			&pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
	}
	if(SUCCEEDED(hr))
	{
		//write
		ReadFile(m_hFile,(BYTE*)pDSLockedBuffer,dwDSLockedBufferSize,&dwBytesWrittenToBuffer,0);
		m_dwNextWriteOffset += dwBytesWrittenToBuffer;

		if (NULL != pDSLockedBuffer2) 
		{ 
			ReadFile(m_hFile,(BYTE*)pDSLockedBuffer2,dwDSLockedBufferSize2,&dwBytesWrittenToBuffer,0);
			m_dwNextWriteOffset += dwBytesWrittenToBuffer;
		} 
		m_dwNextWriteOffset %= (m_waveFormat.nAvgBytesPerSec * 4);
		hr = m_pDSB8->Unlock(pDSLockedBuffer,dwDSLockedBufferSize,pDSLockedBuffer2,dwDSLockedBufferSize2);

		if( dwBytesWrittenToBuffer < m_waveFormat.nAvgBytesPerSec * 2 )
		{
			Stop();
		}
		
	}
	return hr;
}