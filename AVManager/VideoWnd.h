#ifndef VIDEO_WND_H_
#define VIDEO_WND_H_



class CVideoWnd : public WindowImplBase
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