#include "stdafx.h"
#include "Utils.h"

#include <TlHelp32.h>
#include <shellapi.h>

tstring CUtils::GetCurDirectory()
{	
	if (!m_strCurrentDir.empty()) return m_strCurrentDir;
	TCHAR pBuf[256];
	GetModuleFileName(NULL, pBuf, sizeof(pBuf) / sizeof(wchar_t));
	m_strCurrentDir = pBuf;
	m_strCurrentDir = m_strCurrentDir.substr(0,m_strCurrentDir.find_last_of('\\') + 1);

	return m_strCurrentDir;
}


tstring CUtils::GetModuleName()
{
	if (!m_strModuleName.empty()) return m_strModuleName;
	TCHAR pBuf[256];
	GetModuleFileName(NULL, pBuf, sizeof(pBuf) / sizeof(wchar_t));
	m_strModuleName = pBuf;
	m_strModuleName = m_strModuleName.substr(m_strModuleName.find_last_of('\\') + 1);

	return m_strModuleName;
}


tstring CUtils::GetFullModuleName()
{
	if (!m_strFullFileName.empty()) return m_strFullFileName;
	TCHAR pBuf[256];
	GetModuleFileName(NULL, pBuf, sizeof(pBuf) / sizeof(wchar_t));
	m_strFullFileName = pBuf;
	return m_strFullFileName;
}

BOOL CUtils::FloderIsExist(LPCTSTR lpPath)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL RetValue = fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
	FindClose(hFind);
	return RetValue;
}

BOOL CUtils::FileIsExist(LPCTSTR lpPath)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL RetValue = fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
	FindClose(hFind);
	return RetValue;
}

tstring CUtils::GetFileName(LPCTSTR lpPath)
{
	const TCHAR *pos, *pos1, *pos2;
	pos1 = _tcsrchr(lpPath, '\\');
	pos2 = _tcsrchr(lpPath, '/');
	pos = pos1 > pos2 ? pos1 : pos2;

	return pos == NULL ? lpPath : &pos[1];
}

tstring CUtils::GetExtName(LPCTSTR lpPath)
{
	const TCHAR* pos = _tcsrchr(lpPath, '.');

	return pos == NULL ? _T("") : &pos[1];
}

int64_t CUtils::GetFileSize(LPCTSTR lpFileName)
{
	HANDLE hFile = NULL;
	LARGE_INTEGER large;
	hFile = ::CreateFile(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return -1;
	GetFileSizeEx(hFile, &large);
	CloseHandle(hFile);
	return large.QuadPart;
}

tstring CUtils::GetAppVersionInfo(LPCTSTR lpFileName)
{
	if (!m_strAppVersion.empty()) return m_strAppVersion;

	TCHAR strFile[MAX_PATH] = { 0 };
	TCHAR szVersion[128] = { 0 };

	if (lpFileName == NULL)
	{
		GetModuleFileName(NULL, strFile, MAX_PATH);
	}
	else
	{
		_tcscpy_s(strFile, lpFileName);
	}

	TCHAR szVersionBuffer[1024];
	DWORD dwVerSize;
	DWORD dwHandle;
	memset(szVersionBuffer, 0, sizeof(TCHAR)* 1024);

	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
	if (dwVerSize == 0)	return _T("");

	if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
	{
		VS_FIXEDFILEINFO * pInfo;
		unsigned int nInfoLen;

		if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
		{
			_stprintf_s(szVersion, _T("%d.%d.%d.%d"), HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),
				HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
			m_strAppVersion = szVersion;
		}
	}
	return m_strAppVersion;
}

tstring CUtils::AnsiStringToUnicodeString(const char* pIn)
{
	tstring strTemp;
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, (char*)pIn, -1, NULL, 0);
	wchar_t *pChar = new wchar_t[dwMinSize];
	memset(pChar, 0, sizeof(wchar_t)* (dwMinSize));

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar(CP_ACP, 0, (char*)pIn, -1, pChar, dwMinSize);
	strTemp = pChar;
	delete[] pChar;
	return strTemp;
}

string CUtils::UnicodeStringToAnsiString(const wchar_t* lpin)
{
	int dwNum;
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, lpin, -1, NULL, 0, NULL, FALSE);

	char* psText = new char[dwNum];
	memset(psText, 0, dwNum);
	if (psText == NULL)
	{
		delete[] psText;
		return "";
	}
	WideCharToMultiByte(CP_OEMCP, NULL, lpin, -1, psText, dwNum, NULL, FALSE);
	int nLen = strlen(psText);

	ASSERT(nLen <= dwNum);
	string s = psText;
	delete[] psText;
	return s;
}

tstring CUtils::Utf8StringToUnicodeString(const char* strUtf8)
{

	ASSERT(strUtf8 != NULL);

	tstring strResult = _T("");
	int i, k;
	int nlengUtf8 = strlen(strUtf8);
	WCHAR* wResult = new WCHAR[sizeof(WCHAR)*(nlengUtf8 + 1)];
	memset(wResult, 0, sizeof(WCHAR)* (nlengUtf8 + 1));

	if (wResult)
	{
		k = 0;
		for (i = 0; i<nlengUtf8; i++)
		{
			WORD n = strUtf8[i];
			n = n << 8;
			n += strUtf8[i + 1];

			wResult[i] = n;
			k++;
		}
	}

	if (nlengUtf8 % 2 != 0)
	{
		wResult[k++] = 0;
	}
	strResult = wResult;
	return strResult;
}

string CUtils::UnicodeStringToUtf8String(const wchar_t* lpin)
{
	DWORD dwNum;
	dwNum = WideCharToMultiByte(CP_UTF8, NULL, lpin, -1, NULL, 0, NULL, FALSE);

	if (dwNum == 0) return "";

	char* psText = new char[dwNum];
	memset(psText, 0, dwNum);
	if (psText == NULL)
	{
		delete[] psText;
		return "";
	}
	WideCharToMultiByte(CP_UTF8, NULL, lpin, -1, psText, dwNum, NULL, NULL);
	string str = psText;
	delete[] psText;
	return str;

}

string CUtils::UTF8ToGBK(const string& strUTF8)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[nLen + 1];
	memset(wszGBK, 0, nLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wszGBK, nLen);

	nLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[nLen + 1];
	memset(szGBK, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, nLen, NULL, NULL);

	string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

DWORD CUtils::GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
	{
		return 0;
	}

	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
		{
			break;
		}
		if (lstrcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}

	CloseHandle(hSnapshot);

	return id;
}

int CUtils::KillProcess(DWORD processID)
{
	HANDLE hProcessHandle;

	hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, processID);

	return ::TerminateProcess(hProcessHandle, 4);
}

BOOL CUtils::StartProcess(tstring strFilePath)
{
	SHELLEXECUTEINFO ShExecInfo /*= {0}*/;
	tstring strTemp, strResult;
	BOOL bRet = FALSE, bZHJDRet = FALSE;
	HANDLE hProcessHandle = NULL;
	tstring strError;

	memset(&ShExecInfo, 0, sizeof(ShExecInfo));
	ShExecInfo.cbSize = sizeof(ShExecInfo);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = strFilePath.c_str();
	ShExecInfo.lpParameters = _T("");
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	hProcessHandle = ShExecInfo.hProcess;

	bRet = ShellExecuteEx(&ShExecInfo);
	return bRet;
}