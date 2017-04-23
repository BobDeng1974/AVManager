#ifndef PUBLIC_H_
#define PUBLIC_H_

#include <string>
#include <vector>
using namespace std;

#define MAX_STRING_LEN 1024

namespace utility
{
	//最长支持1024个字符
	static string replace_str(const char *src, const char *sMatchStr, const char *sReplaceStr)
	{

		int  StringLen;

		char result[MAX_STRING_LEN];
		char NewString[MAX_STRING_LEN];

		strcpy(result, src);

		const char *FindPos = strstr(result, sMatchStr);

		if( (!FindPos) || (!sMatchStr) )
			return result;


		while( FindPos )

		{

			memset(NewString, 0, sizeof(NewString));

			StringLen = FindPos - result;

			strncpy(NewString, result, StringLen);

			strcat(NewString, sReplaceStr);

			strcat(NewString, FindPos + strlen(sMatchStr));

			strcpy(result, NewString);

			FindPos = strstr(result, sMatchStr);

		}


		return result;
	}

	static void split_str(const char* str, char split, vector<string>& v)
	{
		char sub[260];
		const char* pbegin = str;
		const char* pos = NULL;
		do 
		{
			pos = strchr(pbegin, split);
			if (pos != NULL)
			{
				memset(sub, 0, sizeof(sub));
				strncpy(sub, pbegin, pos - pbegin);
				pbegin = pos + 1;
				v.push_back(sub);
			}

		} while (pos);
	}

	static string str_ltrim(const string& str) 
	{ 
		return str.substr(str.find_first_not_of(" \n\r\t")); 
	} 

	static string str_rtrim(const string& str) 
	{ 
		return str.substr(0,str.find_last_not_of(" \n\r\t") + 1); 
	} 

	static string str_trim(const string& str) 
	{ 
		return str_ltrim(str_rtrim(str)); 
	}

	static string str_toupper(const char* c)
	{
		char* r = _strdup(c);
		for (int i = 0; r[i] != 0; i++)
		{
			r[i] = toupper(r[i]);
		}
		string result = r;
		free(r);
		return result;
	}

	static string str_tolower(const char* c)
	{
		char* r = _strdup(c);
		for (int i = 0; r[i] != 0; i++)
		{
			r[i] = tolower(r[i]);
		}
		string result = r;
		free(r);
		return result;
	}

	static string byte_tran_char(unsigned long long ubyte)
	{
		char tran_char[64];
		if (ubyte / (1024 * 1024 * 1024) > 0)
		{
			sprintf(tran_char, "%.1f GB", ubyte / (1024 * 1024 * 1024.0));
		}
		else if (ubyte / (1024 * 1024) > 0)
		{
			sprintf(tran_char, "%.1f MB", ubyte / (1024 * 1024.0));
		}
		else if (ubyte / 1024 > 0)
		{
			sprintf(tran_char, "%.1f KB", ubyte / (1024.0));
		}
		else
		{
			sprintf(tran_char, "%u B", ubyte);
		}

		return tran_char;
	}

	static wstring Utf8ToUnicode(const char* buf)
	{
		int len = ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
		if (len == 0) return L"";

		std::vector<wchar_t> unicode(len);
		::MultiByteToWideChar(CP_UTF8, 0, buf, -1, &unicode[0], len);

		return &unicode[0];	
	}

	static string UnicodeToUtf8(const wchar_t* buf)
	{
		int len = ::WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
		if (len == 0) return "";

		std::vector<char> utf8(len);
		::WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);

		return &utf8[0];
	}

	static wstring AnsiToUnicode(const char* buf)
	{
		int len = ::MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
		if (len == 0) return L"";

		std::vector<wchar_t> unicode(len);
		::MultiByteToWideChar(CP_ACP, 0, buf, -1, &unicode[0], len);

		return &unicode[0];
	}

	static string UnicodeToAnsi(const wchar_t* buf)
	{
		wstring wstr = buf;
		int len = ::WideCharToMultiByte(CP_ACP, 0, buf, wstr.length(), NULL, 0, NULL, NULL);
		if (len == 0) return "";

		std::vector<char> utf8(len + 1);
		::WideCharToMultiByte(CP_ACP, 0, buf, wstr.length() + 1, &utf8[0], len + 1, NULL, NULL);
		utf8[len] = '\0';
		string str = &utf8[0];
		return &utf8[0];
	}

	static string AnsiToUtf8(const string& src)
	{
		return UnicodeToUtf8(AnsiToUnicode(src.c_str()).c_str());
	}

	static string Utf8ToAnsi(const string& src)
	{
		return UnicodeToAnsi(Utf8ToUnicode(src.c_str()).c_str());
	}
	//Format:
	static string color_itostr(unsigned int iColor)
	{
		string strRGBText;

		char szcolor[24] = {0};
		int iRed = GetRValue(iColor);
		int iGreen = GetGValue(iColor);
		int iBlue = GetBValue(iColor);
		sprintf(szcolor, "#%02X%02X%02X", iRed, iGreen, iBlue);
		strRGBText = szcolor;
		return strRGBText;
	}

	//Format: #FFFFFF
	static unsigned int	 color_strtoi(string str)
	{
		if (str.empty()) return 0;
		unsigned int  dColor = 0;

		sscanf_s(str.substr(1,str.length() - 1).c_str(),"%X",&dColor);
		return dColor;
	}
}

#endif