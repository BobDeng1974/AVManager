#ifndef MAIN_WND_H_
#define MAIN_WND_H_



class CMainWnd : public WindowImplBase
{
public:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();

	void Notify(TNotifyUI& msg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	CControlUI* CreateControl(LPCTSTR pstrClass);
};

#endif