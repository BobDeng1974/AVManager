#include "stdafx.h"
#include "MainWnd.h"


#include <strmif.h>
#include <commdlg.h>

DEFINE_GUID(CLSID_LavSplitter_Source,
0xB98D13E7, 0x55DB, 0x4385, 0xA3, 0x3D, 0x09, 0xFD, 0x1B, 0xA2, 0x63, 0x38);


DEFINE_GUID(CLSID_LavVideoDecoder,
	0xEE30215D, 0x164F, 0x4A92, 0xA4, 0xEB, 0x9D, 0x4C, 0x13, 0x39, 0x0F, 0x9F);

DEFINE_GUID(CLSID_LavAudioDecoder,
	0xE8E73B6B, 0x4CB3, 0x44A4, 0xBE, 0x99, 0x4F, 0x7B, 0xCB, 0x96, 0xE4, 0x91);

CMainWnd::CMainWnd()
{
	m_pLavSplitterSource = NULL;
	m_pFileSourceFilter = NULL;
	m_pLavVideoDecoder = NULL;
	m_pLavAudioDecoder = NULL;
	m_pVideoRenderer = NULL;
	m_pAudioRender = NULL;
}

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
	if (_tcsicmp(pstrClass, _T("FileListUI")) == 0)
	{
		return new CFileListUI(m_PaintManager);
	}
	return NULL;
}


void CMainWnd::InitWindow()
{
	m_VideoWnd.CreateDuiWindow(m_hWnd, _T("视频"), WS_CHILD);
	m_VideoWnd.ShowWindow(SW_HIDE);
	VideoPlayInit(m_VideoWnd.GetHWND());
	//SetWindowPos(m_VideoWnd, NULL, 0, 60, 0, 0, SWP_NOSIZE);


	m_pVerFileList = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("ver_file_list")));
	m_pHerVideo = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("her_video")));
	m_pFileListUI = static_cast<CFileListUI*>(m_PaintManager.FindControl(_T("list_file_ui")));

	CListTextElementUI* pEle = new CListTextElementUI();
	pEle->SetText(0,_T("111"));
	m_pFileListUI->Add(pEle);
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
		else if (msg.pSender->GetName() == _T("btn_file_list")) {
			OnFileList();
		}
		else if (msg.pSender->GetName() == _T("btn_open_file")) {
			OnOpenFile();
		}
		else if (msg.pSender->GetName() == _T("btn_play_video"))
		{
			OnBtnPlayVideo();
		}
		else if (msg.pSender->GetName() == _T("btn_stop"))
		{
			OnBtnStop();
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

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int width = LOWORD(lParam);
	int height = HIWORD(lParam);
	WindowImplBase::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void CMainWnd::OnFileList()
{
	BOOL b = m_pVerFileList->IsVisible();
	m_pVerFileList->SetVisible(!b);

	RECT rc = m_pVerFileList->GetPos();
	int width = m_pHerVideo->GetWidth();
	int height = m_pHerVideo->GetHeight();
	if (b)
	{
		SetWindowPos(m_VideoWnd, NULL, 0, rc.top, width, height, SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowPos(m_VideoWnd, NULL, rc.right, rc.top, width - (rc.right - rc.left), height, SWP_SHOWWINDOW);
	}
	
}

void CMainWnd::OnOpenFile()
{
	OPENFILENAME ofn;
	TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };
	TCHAR* p = NULL;
	int nLen = 0;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
	if (GetOpenFileName(&ofn))
	{
		m_mFileList.clear();
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);
		szPath[ofn.nFileOffset] = '\0';
		nLen = lstrlen(szPath);
		if (szPath[nLen - 1] != '\\')                        //如果选了多个文件,则必须加上'//'
		{
			lstrcat(szPath, TEXT("\\"));
		}
		p = szOpenFileNames + ofn.nFileOffset;              //把指针移到第一个文件		
		while (*p)
		{
			ZeroMemory(szFileName, sizeof(szFileName));
			lstrcat(szFileName, szPath);               //给文件名加上路径 
			lstrcat(szFileName, p);                    //加上文件名 
			//lstrcat(szFileName, TEXT("\n"));
			m_mFileList.insert(make_pair(p, szFileName));
			p += lstrlen(p) + 1;                        //移至下一个文件
		}
	}
	
}

void CMainWnd::OnBtnPlayVideo()
{
	HRESULT hr = S_OK;

	if (m_pLavSplitterSource == NULL)
	{
		VideoAddFilter(CLSID_LavSplitter_Source, L"Lav Splitter Source", &m_pLavSplitterSource);
		hr = m_pLavSplitterSource->QueryInterface(IID_IFileSourceFilter, (void **)&m_pFileSourceFilter);
		if (hr == S_OK)
		{
			hr = m_pFileSourceFilter->Load(_T("F:\\迅雷下载\\这个男人来自地球BD中英双字.rmvb"), NULL);
			if (hr == S_OK)
			{

			}
		}
		VideoAddFilter(CLSID_LavVideoDecoder, L"Lav Video Decoder", &m_pLavVideoDecoder);
		VideoConnectFilter(m_pLavSplitterSource, m_pLavVideoDecoder);
		VideoAddFilter(CLSID_LavAudioDecoder, L"Lav Audio Decoder", &m_pLavAudioDecoder);
		VideoConnectFilter(m_pLavSplitterSource, m_pLavAudioDecoder);
		VideoAddFilter(CLSID_VideoMixingRenderer9, L"Video Mixing Renderer-9", &m_pVideoRenderer);
		VideoConnectFilter(m_pLavVideoDecoder, m_pVideoRenderer);
		VideoAddFilter(CLSID_AudioRender, L"Audio render", &m_pAudioRender);
		VideoConnectFilter(m_pLavAudioDecoder, m_pAudioRender);
		m_pFileSourceFilter->Release();
		m_pFileSourceFilter = NULL;
		VideoPlayRun();
	}
}

void CMainWnd::OnBtnStop()
{
	if (m_pLavSplitterSource)
	{
		VideoPlayStop();
		VideoDelFilter(m_pAudioRender);
		VideoDelFilter(m_pVideoRenderer);
		VideoDelFilter(m_pLavAudioDecoder);
		VideoDelFilter(m_pLavVideoDecoder);
		VideoDelFilter(m_pLavSplitterSource);
		m_pLavSplitterSource = NULL;		
		m_pLavVideoDecoder = NULL;
		m_pLavAudioDecoder = NULL;
		m_pVideoRenderer = NULL;
		m_pAudioRender = NULL;
	}
	
}

void CMainWnd::OnBtnLayeredWindow()
{
	if (!IsWindow(m_LovingWnd))
	{
		m_LovingWnd.CreateDuiWindow(m_hWnd, _T("loving"), WS_POPUP);
		m_LovingWnd.ShowWindow();
	}
	else
	{
		m_LovingWnd.ShowWindow(!IsWindowVisible(m_LovingWnd));
	}
}