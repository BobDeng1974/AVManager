#include "stdafx.h"
#include "MainWnd.h"
#include <process.h>
CMainWnd::CMainWnd()
{
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("skin\\");
}


CDuiString CMainWnd::GetSkinFile()
{
	return GetSkinFolder() + _T("MainWnd_UpGrade.xml");
}


LPCTSTR CMainWnd::GetWindowClassName(void) const
{
	return _T("MainWndUpGrade");
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}


void CMainWnd::InitWindow()
{
	m_strLocalFile = _T("C:\\");
	m_strConfigFile = _T("www");
	m_strCurVersion = GetAppVersionInfo(_T(""));
	m_strNewVersion = _T("");
	unsigned int iThreadID = 0;
	CloseHandle((HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &iThreadID));
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("btn_close")) {
			Close();
		}
		else if (msg.pSender->GetName() == _T("btn_upgrade"))
		{
			OnUpgrade();
		}
		else if (msg.pSender->GetName() == _T("btn_no_upgrade"))
		{
			OnNoUpgrade();
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

unsigned int  _stdcall CMainWnd::ThreadProc(void* param)
{
	CMainWnd* pThis = static_cast<CMainWnd*>(param);
	pThis->CheckProcess();
	return 0;
}

void CMainWnd::CheckProcess()
{
	//	本地存在下载好的文件，
	if (!m_strLocalFile.empty())
	{

	}
	else
	{

	}
}

void		CMainWnd::OnUpgrade()
{

}

void		CMainWnd::OnNoUpgrade()
{

}

wstring    CMainWnd::GetAppVersionInfo(LPCTSTR lpFileName)
{
	
	TCHAR strVersion[256] = {0};
	TCHAR strFile[MAX_PATH];
	memset(strFile, 0, MAX_PATH);

	if (lpFileName == NULL)
	{
		GetModuleFileName(NULL, strFile, MAX_PATH);
	}
	else
	{
		memcpy(strFile, lpFileName, _tcslen(lpFileName) * 2);
	}

	TCHAR szVersionBuffer[1024];
	DWORD dwVerSize;
	DWORD dwHandle;
	memset(szVersionBuffer, 0, sizeof(TCHAR)* 1024);

	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
	if (dwVerSize == 0)
		return _T("");

	if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
	{
		VS_FIXEDFILEINFO * pInfo;
		unsigned int nInfoLen;

		if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
		{
			wsprintf(strVersion, _T("%d.%d.%d.%d"), HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),
				HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
		}
	}
	return strVersion;
}