#ifndef MAIN_WND_H_
#define MAIN_WND_H_
#include "VideoWnd.h"
#include "LovingWnd.h"

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


	void OnBtnPlayVideo();
	void OnBtnLayeredWindow();

private:
	CVideoWnd m_VideoWnd;
	CLovingWnd m_LovingWnd;
};

#endif