#include "stdafx.h"
#include "VideoWnd.h"


CDuiString CVideoWnd::GetSkinFolder()
{
	return _T("skin\\");
}


CDuiString CVideoWnd::GetSkinFile()
{
	return GetSkinFolder() + _T("VideoWnd.xml");
}


LPCTSTR CVideoWnd::GetWindowClassName(void) const
{
	return _T("VideoWindow");
}


void CVideoWnd::InitWindow()
{

}

void CVideoWnd::Notify(TNotifyUI& msg)
{

}


LRESULT CVideoWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	return 0;
}

CControlUI* CVideoWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}