#ifndef UTILS_H_
#define UTILS_H_


class CUtils
{
public:
	//	获取当前目录
	tstring GetCurDirectory();
	tstring GetModuleName();
	tstring GetAppVersionInfo(LPCTSTR lpFileName);

	BOOL	FloderIsExist(LPCTSTR lpPath);
	BOOL	FileIsExist(LPCTSTR lpPath);

	// 字符串转换
	tstring AnsiStringToUnicodeString(const char* pIn);
	string	UnicodeStringToAnsiString(const wchar_t* lpin);
	tstring Utf8StringToUnicodeString(const char* strUtf8);
	string	UnicodeStringToUtf8String(const wchar_t* lpin);
	string	UTF8ToGBK(const string& strUTF8);

	//	进程管理
	DWORD	GetProcessidFromName(LPCTSTR name);
	int		KillProcess(DWORD processID);
	BOOL	StartProcess(tstring strFilePath);
private:
	tstring m_strCurrentDir;
	tstring m_strModuleName;
	tstring m_strAppVersion;
};


#endif