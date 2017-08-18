#ifndef FILE_LIST_UI_H_
#define FILE_LIST_UI_H_

class CFileListUI : public CListUI
{
public:
	CFileListUI(CPaintManagerUI& pm);
	~CFileListUI();

	void AddNode(tstring name);
private:
	CPaintManagerUI* m_pm;
};

#endif