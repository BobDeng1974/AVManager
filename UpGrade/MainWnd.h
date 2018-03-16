#ifndef MAIN_WND_H_
#define MAIN_WND_H_


class CMainWnd : public WindowImplBase
{
public:
	CMainWnd();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();

	void Notify(TNotifyUI& msg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	

	CControlUI* CreateControl(LPCTSTR pstrClass);

	void		CheckProcess();
	void		DownLoadFile();
	void		OnUpgrade();
	void		OnNoUpgrade();
	static  unsigned int  _stdcall ThreadProc(void* param);
private:
	tstring m_strPath;
	tstring m_strAppFile;
	tstring m_strConfigUrl;
	tstring m_strCurVersion;
	tstring m_strNewVersion;

	CButtonUI *m_pBtnUpgrade;
	CButtonUI *m_pBtnNoUpgrade;
};

#endif