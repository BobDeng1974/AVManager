#include "StdAfx.h"
#include "VideoCapture.h"


CVideoCapture* CVideoCapture::s_Instance = NULL;

CVideoCapture::CVideoCapture()
	: m_pBuilder(NULL)
	, m_pGraph(NULL)
	, m_bPlaying(FALSE)
{
	m_captureInfo = NULL;
}

CVideoCapture::~CVideoCapture()
{

}

CVideoCapture* CVideoCapture::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new CVideoCapture();
	}
	return s_Instance;
}

HRESULT CVideoCapture::Init(HWND hWnd)
{
	HRESULT hr = NOERROR;

	m_hParent = hWnd;
	hr |= CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IFilterGraph2, (void **)&m_pGraph);
	hr |= CoCreateInstance(CLSID_CaptureGraphBuilder2,0,CLSCTX_INPROC_SERVER,IID_ICaptureGraphBuilder2,(void**)&m_pBuilder);
	m_pBuilder->SetFiltergraph(m_pGraph);
	if (SUCCEEDED(hr))
	{
		return QueryInterfaces();
	}
	
	return hr;
}

BOOL CVideoCapture::QueryInterfaces()
{
	if (m_pGraph)
	{
		HRESULT hr;
		hr |= m_pGraph->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
		hr |= m_pGraph->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow);
		
		return SUCCEEDED(hr);
	}
	
	return FALSE;
}

void CVideoCapture::Release()
{
	if (m_pMediaControl != NULL)
	{
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if (m_pGraph != NULL)
	{
		m_pGraph->Release();
		m_pGraph = NULL;
	}
	if (m_pBuilder != NULL)
	{
		m_pBuilder->Release();
		m_pBuilder = NULL;
	}
}

bool CVideoCapture::VideoCaptureDeviceEnum(VECT_CAPTURE_INFO *pVCaptureInfo)
{
	HRESULT hr= S_OK;

	ICreateDevEnum *pSystemDeviceEnumerator= NULL;
	hr = CoCreateInstance(  CLSID_SystemDeviceEnum,	NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pSystemDeviceEnumerator);
	if( !hr )
	{
		return false;
	}

	IEnumMoniker *pVideoCaptureDeviceEnumerator= NULL;
	hr= pSystemDeviceEnumerator->CreateClassEnumerator( CLSID_VideoInputDeviceCategory,	&pVideoCaptureDeviceEnumerator,	0);
	if( !hr )
	{
		return false;
	}

	int i = 0;
	VideoCaptureInfo vCaptureInfo;
	IMoniker *pMoniker= NULL;
	while( pVideoCaptureDeviceEnumerator->Next( 1, &pMoniker, NULL) == S_OK )
	{
		VARIANT var;
		VariantInit(&var);

		// make filters properties accessible
		IPropertyBag *pPropBag= NULL;
		hr = pMoniker->BindToStorage( 0, 0, IID_IPropertyBag, (void**)&pPropBag);
		if( FAILED(hr))
		{
			pPropBag= NULL;
		}
		else
		{
			hr = pPropBag->Read( L"FriendlyName", &var, 0);
			if( FAILED(hr))
			{
			}
			else
			{
				vCaptureInfo.nDeviceID = i;
				_tcscpy_s(vCaptureInfo.description,var.bstrVal);
				hr = m_pGraph->AddSourceFilterForMoniker(pMoniker, 0, vCaptureInfo.description, &vCaptureInfo.filter);
				pVCaptureInfo->push_back(vCaptureInfo);
				m_vCaptureInfo.push_back(vCaptureInfo);
				pMoniker->Release();
				pMoniker= NULL;
			}

			pPropBag->Release();
			pPropBag= NULL;
		}

		VariantClear(&var);
		i++;
	}

	//释放
	pSystemDeviceEnumerator->Release();
	pSystemDeviceEnumerator= NULL;
	pVideoCaptureDeviceEnumerator->Release();
	pVideoCaptureDeviceEnumerator= NULL;

	return TRUE;
}

void CVideoCapture::SelectDevice(int nDeviceID)
{
	HRESULT hr;
	LONGLONG start=MAXLONGLONG, stop=MAXLONGLONG;
	bool was_playing = m_bPlaying;

	if (was_playing) VideoCaptureStop();

	if (m_captureInfo)
	{
		m_pGraph->RemoveFilter(m_captureInfo->filter);
		m_pGraph->AddFilter(m_captureInfo->filter, m_captureInfo->description);
	}

	start = 0;
	for (unsigned int i = 0; i < m_vCaptureInfo.size(); i++)
	{
		if (m_vCaptureInfo[i].nDeviceID == nDeviceID)
		{
			m_captureInfo = &m_vCaptureInfo[i];
			break;
		}
	}


	//connect graph with current source filter
	hr = m_pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_captureInfo->filter, NULL,NULL/*samplegrabberfilter, nullrenderer*/);
	if (hr != S_OK) throw hr;

	//start streaming
	hr = m_pBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_captureInfo->filter, &start, &stop, 1,2);
	if (hr < 0) throw hr;

	//restart
	if (was_playing) VideoCaptureStart();
}

BOOL CVideoCapture::VideoCaptureStart()
{
	HRESULT hr= S_OK;
	SetDisPlayWindow(m_hParent);
	hr = m_pMediaControl->Run();
	if( FAILED(hr)) return FALSE;
	m_bPlaying = TRUE;

	return true;
}

BOOL CVideoCapture::VideoCaptureStop()
{
	m_pMediaControl->Stop();
	m_pVideoWindow->put_Visible(OAFALSE);
	m_bPlaying = FALSE;

	return TRUE;
}

BOOL CVideoCapture::SetDisPlayWindow(HWND hWnd)
{
	if (m_pVideoWindow)
	{
		// 首先隐藏视频窗口
		m_pVideoWindow->put_Visible(OAFALSE);
		m_pVideoWindow->put_Owner((OAHWND)hWnd);

		//获取输入窗口的显示区域
		RECT windowRect;
		::GetClientRect(hWnd, &windowRect);
		m_pVideoWindow->put_Left(windowRect.left);
		m_pVideoWindow->put_Top(windowRect.top);
		m_pVideoWindow->put_Width(windowRect.right - windowRect.left);
		m_pVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		m_pVideoWindow->put_MessageDrain((OAHWND) hWnd);
		// 恢复视频窗口
		if (hWnd != NULL)
		{
			m_pVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			m_pVideoWindow->put_Visible(OAFALSE);
		}
		return true;
	}
	return false;
}
// int	 CVideoCapture::Execute()
// {
// 	return 0;
// 
// 	//分辨率
// // 	hr= pBuilder2->FindInterface(&PIN_CATEGORY_CAPTURE, 
// // 		&MEDIATYPE_Video,pSrc, IID_IAMStreamConfig,(void **)&pSC); 
// // 	if (FAILED(hr))
// // 	{
// // 		AfxMessageBox("fail");
// // 	}
// // 	AM_MEDIA_TYPE * mmt=NULL;
// // 	pSC->GetFormat(&mmt);    //取得默认参数
// // 	VIDEOINFOHEADER * pvih = (VIDEOINFOHEADER*)mmt->pbFormat;
// // 	pvih->bmiHeader.biHeight=240;  //修改采集视频的高为240
// // 
// // 	pvih->bmiHeader.biWidth=320;   //修改采集视频的宽为320
// // 
// // 	mmt->pbFormat = (unsigned char *) pvih;
// // 	pSC->SetFormat(mmt);   //重新设置参数
// }
