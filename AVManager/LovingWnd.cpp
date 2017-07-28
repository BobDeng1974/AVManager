#include "stdafx.h"
#include "LovingWnd.h"



CLovingWnd::CLovingWnd()
{
	m_nInc = 2;
}

CLovingWnd::~CLovingWnd()
{

}


void CLovingWnd::Notify(TNotifyUI& msg)
{

}

CDuiString CLovingWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CLovingWnd::GetSkinFile()
{
	return GetSkinFolder() + _T("LovingWnd.xml");
}

CControlUI* CLovingWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LPCTSTR CLovingWnd::GetWindowClassName() const
{
	return _T("LovingWnd");
}

void CLovingWnd::InitWindow()
{

	SetWindowLong(*this, GWL_EXSTYLE, GetWindowLong(*this, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(*this, RGB(255, 255, 255), 0, LWA_COLORKEY);
	SetTimer(m_hWnd, 1, 100, NULL);
}

LRESULT CLovingWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_TIMER)
	{
		if (wParam == 1)
		{	

			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			rc.left += m_nInc;
			rc.top += m_nInc;
			rc.bottom += m_nInc;
			rc.right += m_nInc;

			MoveWindow(m_hWnd, rc.left , rc.top, rc.right-rc.left, rc.bottom-rc.left, TRUE);
		}
	}
	return 0;
}