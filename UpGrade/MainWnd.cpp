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
	// ����Ŀ¼
	m_strPath = util.GetCurDirectory(); 
	m_strAppFile = m_strPath + _T("AVManager.exe");

	// ���ذ�װ�ļ�·�������������ļ�url
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

	// ��ȡConfigurattion
	string strResponse;
	CWininetHttp http;
	strResponse = http.Request(m_strConfigUrl.c_str(), HttpRequest::Hr_Get);

	// ���°汾�͵�ǰ�汾���
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
	bi.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����  
	bi.lpszTitle = TEXT("ѡ��һ���ļ���");//����  
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer)){
		MessageBox(NULL, szBuffer, TEXT("��ѡ����ļ���"), 0);
	}
	else{
		MessageBox(NULL, TEXT("��ѡ��һ���ļ���"), NULL, MB_ICONERROR);
	}
}

void		CMainWnd::OnUpgrade()
{
	DownLoadFile();
}

void		CMainWnd::OnNoUpgrade()
{

}