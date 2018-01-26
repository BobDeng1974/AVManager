#pragma once

//每次读取的字节数
#define READ_BUFFER_SIZE		4096


#include "IHttpInterface.h"

#define DOWNLOAD_BUFFER_SIZE	8*1024*1024


void MyParseUrlW(LPCWSTR lpUrl, wstring& strHostName, wstring& strPage, WORD& sPort);
void MyParseUrlA(LPCSTR lpUrl, string& strHostName, string& strPage, WORD& sPort);

class IHttp
{
public:
	IHttp()
		: m_lpParam(NULL)
		, m_error(Hir_Success)
		, m_pCallback(NULL)
	{

	}
protected:
	void*	m_lpParam;
	HttpInterfaceError	m_error;
	IHttpCallback*	m_pCallback;
};

class CWininetHttp
	: public IWininetHttp
	, public IHttp
{
public:
	CWininetHttp(void);
	virtual ~CWininetHttp(void);
	//	返回utf-8编码
	virtual string	Request(LPCSTR pUrl, HttpRequest type, LPCSTR pPostData = NULL, LPCSTR pHeader=NULL);
	virtual string	Request(LPCWSTR pUrl, HttpRequest type, LPCSTR pPostData = NULL, LPCWSTR pHeader=NULL);
	virtual bool	DownloadFile(LPCWSTR lpUrl, LPCWSTR lpFilePath);
	virtual bool	DownloadToMem(LPCWSTR lpUrl, OUT void** ppBuffer, OUT int* nSize);
	virtual void	SetDownloadCallback(IHttpCallback* pCallback, void* pParam)		{ m_pCallback = pCallback; m_lpParam = pParam; }
	virtual HttpInterfaceError GetErrorCode()										{ return m_error;	}
	
protected:
	//关闭句柄
	void	ReleaseHandle(HINTERNET& hInternet);
	void	Release();
private:
	HINTERNET	m_hSession;
	HINTERNET	m_hConnect;
	HINTERNET	m_hRequest;
};
