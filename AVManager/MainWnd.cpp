#include "stdafx.h"
#include "MainWnd.h"

#include <strmif.h>

DEFINE_GUID(CLSID_LavSplitter,
0x171252A0, 0x8820, 0x4AFE, 0x9D, 0xF8, 0x5C, 0x92, 0xB2, 0xD6, 0x6B, 0x04);

DEFINE_GUID(CLSID_LavSplitter_Source,
0xB98D13E7, 0x55DB, 0x4385, 0xA3, 0x3D, 0x09, 0xFD, 0x1B, 0xA2, 0x63, 0x38);


DEFINE_GUID(CLSID_LavVideoDecoder,
	0xEE30215D, 0x164F, 0x4A92, 0xA4, 0xEB, 0x9D, 0x4C, 0x13, 0x39, 0x0F, 0x9F);

DEFINE_GUID(CLSID_LavAudioDecoder,
	0xE8E73B6B, 0x4CB3, 0x44A4, 0xBE, 0x99, 0x4F, 0x7B, 0xCB, 0x96, 0xE4, 0x91);

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("skin\\");
}


CDuiString CMainWnd::GetSkinFile()
{
	return GetSkinFolder() + _T("MainWnd.xml");
}


LPCTSTR CMainWnd::GetWindowClassName(void) const
{
	return _T("MainWnd");
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("")) == 0)
	{
		//return new CListItemUI(m_PaintManager);
	}
	return NULL;
}


void CMainWnd::InitWindow()
{
	m_VideoWnd.CreateDuiWindow(m_hWnd, _T("视频"), WS_CHILD);
	m_VideoWnd.ShowWindow();
	VideoPlayInit(m_VideoWnd.GetHWND());

	
}


void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("btn_close")) {
			VideoPlayStop();
			VideoPlayRelease();			
			m_VideoWnd.Close();
			Close();
		}
		else if (msg.pSender->GetName() == _T("btn_play_video"))
		{
			OnBtnPlayVideo();
		}
		else if (msg.pSender->GetName() == _T("btn_layered_window"))
		{
			OnBtnLayeredWindow();
		}
	}
}


LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	return 0;
}


void CMainWnd::OnBtnPlayVideo()
{
	HRESULT hr = S_OK;
	IBaseFilter *pLavSplitterSource;	
	IFileSourceFilter *pFileSourceFilter;
	IBaseFilter *pLavVideoDecoder;
	IBaseFilter *pLavAudioDecoder;
	IBaseFilter *pVideoRenderer;
	IBaseFilter *pAudioRender;

	VideoAddFilter(CLSID_LavSplitter_Source, L"Lav Splitter Source", &pLavSplitterSource);
	hr = pLavSplitterSource->QueryInterface(IID_IFileSourceFilter, (void **)&pFileSourceFilter);
	hr = pFileSourceFilter->Load(_T("G:\\电影\\JPJJ第一部.mp4"), NULL);

	VideoAddFilter(CLSID_LavVideoDecoder, L"Lav Video Decoder", &pLavVideoDecoder);
	VideoConnectFilter(pLavSplitterSource, pLavVideoDecoder);
	VideoAddFilter(CLSID_LavAudioDecoder, L"Lav Audio Decoder", &pLavAudioDecoder);
	VideoConnectFilter(pLavSplitterSource, pLavAudioDecoder);

	VideoAddFilter(CLSID_VideoMixingRenderer9, L"Video Mixing Renderer-9", &pVideoRenderer);
	VideoConnectFilter(pLavVideoDecoder, pVideoRenderer);

	VideoAddFilter(CLSID_AudioRender, L"Audio render", &pAudioRender);
	VideoConnectFilter(pLavAudioDecoder, pAudioRender);

	
	VideoPlayRun();

	

	//VideoPlayStart(_T("G:\\电影\\JPJJ第一部.mp4"));
}


void CMainWnd::OnBtnLayeredWindow()
{
	m_LovingWnd.CreateDuiWindow(m_hWnd, _T("loving"), WS_POPUP);
	m_LovingWnd.ShowWindow();
}