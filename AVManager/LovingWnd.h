#ifndef LOVING_WND_H_
#define LOVING_WND_H_

class CLovingWnd : public WindowImplBase
{
public:
	CLovingWnd();
	~CLovingWnd();

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();

	void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CPaintManagerUI m_PaintManager;
	int m_nInc;
};

#endif