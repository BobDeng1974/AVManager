#ifndef MAIN_WND_H_
#define MAIN_WND_H_
#include "VideoWnd.h"
#include "LovingWnd.h"
#include "FileListUI.h"

class CMainWnd : public WindowImplBase
{
public:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();

	void Notify(TNotifyUI& msg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CControlUI* CreateControl(LPCTSTR pstrClass);

	void OnFileList();
	void OnOpenFile();
	void OnBtnPlayVideo();
	void OnBtnLayeredWindow();

private:
	CVideoWnd m_VideoWnd;
	CLovingWnd m_LovingWnd;

	CVerticalLayoutUI* m_pVerFileList;
	CHorizontalLayoutUI* m_pHerVideo;
	CFileListUI * m_pFileListUI;

	map<tstring, tstring> m_mFileList;
};

#endif