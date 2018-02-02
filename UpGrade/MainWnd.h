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
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Notify(TNotifyUI& msg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	

	CControlUI* CreateControl(LPCTSTR pstrClass);

	void		CheckProcess();
	void		OnUpgrade();
	void		OnNoUpgrade();
	wstring GetAppVersionInfo(LPCTSTR lpFileName);
	static  unsigned int  _stdcall ThreadProc(void* param);
private:
	wstring m_strLocalFile;
	wstring m_strConfigFile;
	wstring m_strCurVersion;
	wstring m_strNewVersion;
};

#endif