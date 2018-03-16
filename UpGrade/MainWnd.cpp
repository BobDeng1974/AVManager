#include "stdafx.h"
#include "MainWnd.h"
#include "Utils.h"
#include "WininetHttp.h"
#include <process.h>
#include <commdlg.h>
#include<Shlobj.h>  


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
	m_pBtnUpgrade = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_upgrade")));
	m_pBtnNoUpgrade = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_no_upgrade")));

	CUtils util;
	// 工作目录
	m_strPath = util.GetCurDirectory(); 
	m_strAppFile = m_strPath + _T("AVManager.exe");

	// 本地安装文件路径、网络配置文件url
	TCHAR szConfigUrl[MAX_PATH] = { 0 };
	tstring strConfigPath = m_strPath + _T("upgrade");
	tstring strIniFile = m_strPath + _T("\\upgrade\\upgrade.ini");
	if (!util.FloderIsExist(strConfigPath.c_str()))
	{ 
		CreateDirectory(strConfigPath.c_str(), NULL);
	}	
	GetPrivateProfileString(_T("APPUPGRADE"), _T("LocalInstallFile"), _T(""), szConfigUrl, MAX_PATH, strIniFile.c_str());
	
	m_strConfigUrl = szConfigUrl;
	if (m_strConfigUrl.empty()) m_strConfigUrl = _T("");
	m_strCurVersion = util.GetAppVersionInfo(m_strPath.c_str());
	m_strNewVersion = _T("");

	unsigned int iThreadID = 0;
	//CloseHandle((HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &iThreadID));
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
	CUtils util;
	tstring strDownLoadUrl = _T("");

	// 获取Configurattion
	string strResponse;
	CWininetHttp http;
	strResponse = http.Request(m_strConfigUrl.c_str(), HttpRequest::Hr_Get);

	// 最新版本和当前版本相等
	if (m_strCurVersion == m_strNewVersion)
	{

	}
	else
	{
		m_pBtnUpgrade->SetVisible(TRUE);
	}
	
}

void		CMainWnd::DownLoadFile()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区  
	bi.lpszTitle = TEXT("选择一个文件夹");//标题  
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer)){
		MessageBox(NULL, szBuffer, TEXT("你选择的文件夹"), 0);
	}
	else{
		MessageBox(NULL, TEXT("请选择一个文件夹"), NULL, MB_ICONERROR);
	}
}

void		CMainWnd::OnUpgrade()
{
	DownLoadFile();
}

void		CMainWnd::OnNoUpgrade()
{

}