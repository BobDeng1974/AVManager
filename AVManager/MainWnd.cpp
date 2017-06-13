#include "stdafx.h"
#include "MainWnd.h"



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
	m_VideoWnd.CreateDuiWindow(m_hWnd, _T("สำฦต"), WS_CHILD);
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
	}
}


LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	return 0;
}


void CMainWnd::OnBtnPlayVideo()
{
	
	VideoPlayStart(_T("G:\\1.avi"));
}