#include "StdAfx.h"
#include "AudioRecord.h"


CAudioRecord::CAudioRecord()
	: m_pCallBack(NULL)
	, m_pContext(NULL)
	, m_lpDSC(NULL)
	, m_pDSNotify(NULL)
	, m_nextCaptureOffset(0)
	, m_bIsCreated(FALSE)
	, m_bIsRecording(FALSE)
{
	
}

CAudioRecord::~CAudioRecord()
{

}

LRESULT CAudioRecord::Init(LPGUID pGuid, int samplePerSec, int bitsPerSameple, int channels)
{
	HRESULT hr;

	ZeroMemory( &m_wformat, sizeof(m_wformat));
	m_wformat.wFormatTag = WAVE_FORMAT_PCM;
	m_wformat.nSamplesPerSec = samplePerSec;
	m_wformat.wBitsPerSample = bitsPerSameple; 
	m_wformat.nChannels = channels;
	m_wformat.nBlockAlign = m_wformat.nChannels * (m_wformat.wBitsPerSample / 8);
	m_wformat.nAvgBytesPerSec = m_wformat.nBlockAlign * m_wformat.nSamplesPerSec;
	m_wformat.cbSize = sizeof(WAVEFORMATEX);

	
	if( pGuid != NULL )
	{
		hr = DirectSoundCaptureCreate8(pGuid,&m_lpDSC,NULL);
	}
	else
	{
		hr= DirectSoundCaptureCreate8(&DSDEVID_DefaultCapture ,&m_lpDSC,NULL);
	}
	return hr;
}

void CAudioRecord::Release()
{
	if (m_lpDSC) m_lpDSC->Release();
	ZeroMemory( &m_wformat, sizeof(m_wformat));
}

BOOL CAudioRecord::Start(LPCTSTR lpFileName)
{
	CreateSoundFile(lpFileName);
	CreateSoundBuffer();

	if (m_lpDSC != NULL )
	{
		c_thread::Start();
		m_pDSB8->Start(DSCBSTART_LOOPING);
		m_bIsRecording = TRUE;
	}

	return TRUE;
}

void CAudioRecord::Stop()
{
	if (m_pDSB8) 
	{
		m_pDSB8->Stop();
		c_thread::Stop();
		Sleep(500);
	}

	if (m_hEvent)
	{
		CloseHandle(m_hEvent[0]);
		CloseHandle(m_hEvent[1]);
	}
	m_bIsRecording = FALSE;
	m_nextCaptureOffset = 0;

}

HRESULT CAudioRecord::CreateSoundBuffer()
{
	//only can create once
	HRESULT hr = 0;
	if (!m_bIsCreated )
	{
		
		DSCBUFFERDESC dscbd;
		LPDIRECTSOUNDCAPTUREBUFFER lpdscb;

		dscbd.dwSize = sizeof(DSCBUFFERDESC);
		dscbd.dwFlags = 0;
		dscbd.dwBufferBytes = 2*m_wformat.nAvgBytesPerSec; //2秒的buffer
		dscbd.dwReserved = 0;
		dscbd.lpwfxFormat = &m_wformat; 
		dscbd.dwFXCount = 0;
		dscbd.lpDSCFXDesc = NULL;

		if (SUCCEEDED(hr = m_lpDSC->CreateCaptureBuffer(&dscbd, &lpdscb, NULL)))
		{
			hr = lpdscb->QueryInterface(IID_IDirectSoundCaptureBuffer8, (LPVOID*)&m_pDSB8);
			lpdscb->Release(); 
			m_bIsCreated =TRUE;
		}

		if (SUCCEEDED(hr = lpdscb->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))) 
		{
			m_hEvent[0]= CreateEvent(NULL,FALSE,FALSE,NULL); 
			m_Notify[0].dwOffset = m_wformat.nAvgBytesPerSec - 1;  // 1秒通知
			m_Notify[0].hEventNotify = m_hEvent[0]; 
			m_hEvent[1]= CreateEvent(NULL,FALSE,FALSE,NULL); 
			m_Notify[1].dwOffset = 2 * m_wformat.nAvgBytesPerSec - 1;  // 2秒通知
			m_Notify[1].hEventNotify = m_hEvent[1]; 
			hr =m_pDSNotify->SetNotificationPositions( 2, m_Notify );
			m_pDSNotify->Release();
		}
	}
	return hr;
}

LRESULT CAudioRecord::CreateSoundFile(LPCTSTR lpFileName)
{
	HRESULT hr;
	hr = m_waveFile.Open(lpFileName,&m_wformat,WAVEFILE_WRITE);
	if (FAILED(hr)) 
	{
		
	}
	return hr;
};

void CAudioRecord::SetAudioRecordCallback(AudioRecordCallBack callback, void* pContext)
{
	m_pCallBack = callback;
	m_pContext	= pContext;
}

int	CAudioRecord::EnumSoundCardInfo(VECT_SOUNDCARD_INFO *soundInfo)
{
	if ( (DirectSoundCaptureEnumerate(DsEnumSDInfoProc, (LPVOID)&m_vSoundCardInfo)) == DS_OK )
	{
		VECT_SOUNDCARD_INFO::iterator it = m_vSoundCardInfo.begin();
		for (; it != m_vSoundCardInfo.end(); it++)
		{
			soundInfo->push_back(*it);
		}
		printf("Operation completed successfully.\n");
	}
	else
	{
		printf("we have a problem.\n");
	}
	return 0;
}

BOOL CAudioRecord::DsEnumSDInfoProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext )
{
	vector<SoundCardInfo>* vSDInfo = (vector<SoundCardInfo>*)lpContext;

	SoundCardInfo sdInfo;
	ZeroMemory(&sdInfo,sizeof(SoundCardInfo));

	// NULL is the primary sound driver. Make sure you don't memcpy from NULL. Visual Studio doesn't like it when you do that
	if ( lpGUID != NULL )
	{
		// copy the GUID over
		memcpy(&sdInfo.Guid, lpGUID, sizeof(GUID));
	}
	_tcscpy_s(sdInfo.description,lpszDesc);
	// and add the information to the list
	vSDInfo->push_back(sdInfo);
	return(TRUE);
}

int CAudioRecord::Execute()
{
	HRESULT hr;
	VOID *pbCaptureData = NULL;
	DWORD dwCaptureLength;
	VOID *pbCaptureData2 = NULL;
	DWORD dwCaptureLength2;
	UINT dwDataWrote = 0;
	DWORD dwReadPos = 0;
	DWORD dwCapturePos = 0;
	LONG lLockSize =  m_wformat.nAvgBytesPerSec;

	while (Get_Running())
	{
		hr = WaitForMultipleObjects(2, m_hEvent,FALSE,INFINITE );
		if (hr >= WAIT_OBJECT_0)
		{
			//锁住内存
			hr = m_pDSB8->Lock( m_nextCaptureOffset, lLockSize, &pbCaptureData, &dwCaptureLength, &pbCaptureData2, &dwCaptureLength2,0L );
			if (FAILED(hr)) return hr;

			// 将内存中的数据拷贝到wave文件中
			hr = m_waveFile.Write( dwCaptureLength, (BYTE*)pbCaptureData, &dwDataWrote );
			if (FAILED(hr)) return hr;

			// 移动偏移标志,循环移动
			m_nextCaptureOffset += dwCaptureLength; 
			m_nextCaptureOffset %= m_wformat.nAvgBytesPerSec*2; // Circular buffer

			if( pbCaptureData2 != NULL )
			{
				// 将内存中的数据拷贝到wave文件中
				hr = m_waveFile.Write( dwCaptureLength2, (BYTE*)pbCaptureData2, &dwDataWrote );
				if (FAILED(hr))	return hr;

				// 移动偏移标志
				m_nextCaptureOffset += dwCaptureLength2; 
				m_nextCaptureOffset %= m_wformat.nAvgBytesPerSec*2; // Circular buffer
			}

			//内存解锁
			hr = m_pDSB8->Unlock( pbCaptureData, dwCaptureLength, pbCaptureData2, dwCaptureLength2 );
			if (FAILED(hr))	return hr;
		}
	}
	m_waveFile.Close();

	return DS_OK;
}


