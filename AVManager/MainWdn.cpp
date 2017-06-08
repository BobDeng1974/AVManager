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

}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("btn_close")) {
			Close();
		}
	}
}


LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	return 0;
}
