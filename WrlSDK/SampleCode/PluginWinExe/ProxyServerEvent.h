#pragma once
#include <map>
#include "BaseFuncLib.h"

#define WM_PROXYSEREREVENT_NEWCONNECT	WM_USER + 2000
#define WM_PROXYSEREREVENT_RECMESSAGE	WM_USER + 2001
#define WM_PROXYSEREREVENT_RECTEXT		WM_USER + 2002
#define WM_PROXYSEREREVENT_BYTE			WM_USER + 2003
#define WM_PROXYSEREREVENT_CLOSECONN	WM_USER + 2005
#define WM_PROXYSEREREVENT_HTTPREQ		WM_USER + 2006
#define WM_PROXYSEREREVENT_HTTPPORT		WM_USER + 2007
#define WM_PROXYSEREREVENT_RECERR		WM_USER + 2008

#ifndef NEED_FINAL_CONSTRUCT
#define NEED_FINAL_CONSTRUCT
#endif

struct HttpReqData 
{
	CString strUri;
	CString strPara;
	CString strRet;

	CString strProtocol;
};

struct CRecData
{
	CString	strReqName;
	CString	strRecText;

	// 接收的二进制内容
	BYTE* pbContent;
};

typedef std::map	<CString,CRecData*>	RECDATA_MAP;

// CProxyServerEvent
class CProxyServerEvent : 
	public CComObjectRoot,
	//实现事件通知接口
	public _ISocketProxyEvents
{
protected:

	/// 消息窗口
	HWND			m_hMsgWnd;

	CThreadDataLock	m_DataLock;

	RECDATA_MAP		m_CatchData;

	void FreeAll()
	{
		m_DataLock.Lock(L"FreeAll");
		RECDATA_MAP::iterator it = m_CatchData.begin();
		while(it != m_CatchData.end())
		{
			if(NULL != it->second)
				delete it->second;
			it++;
		}
		m_CatchData.clear();
		m_DataLock.Unlock(L"FreeAll");
	}

public:
	CProxyServerEvent()
		:m_hMsgWnd(NULL)
	{
	}

	~CProxyServerEvent()
	{
		FreeAll();
	}

	void SetCallPara(HWND hMsgWnd)
	{
		if(NULL != hMsgWnd)
		{
			if(!::IsWindow(hMsgWnd))
				return;
		}
		m_hMsgWnd = hMsgWnd;
	}

	CRecData* GetCatchData(const CString& strSID,ULONG nReqID,BOOL bRemove = TRUE)
	{
		CRecData* pRecData = NULL;
		CString strKey;
		strKey.Format(L"%ld_%s",nReqID,strSID);
		m_DataLock.Lock(L"GetCatchData");
		RECDATA_MAP::iterator it = m_CatchData.find(strKey);
		if(it != m_CatchData.end())
		{
			pRecData = it->second;
			if(bRemove)
				m_CatchData.erase(it);
		}
		m_DataLock.Unlock(L"GetCatchData");
		return pRecData;
	}

BEGIN_COM_MAP(CProxyServerEvent)
	COM_INTERFACE_ENTRY(_ISocketProxyEvents)
END_COM_MAP()

public:

	STDMETHODIMP GetTypeInfoCount(UINT*)
    {
       return E_NOTIMPL;
    }

    STDMETHODIMP GetTypeInfo( UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
    { 
       return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames( REFIID riid,LPOLESTR *rgszNames,UINT cNames,
                    LCID lcid,DISPID *rgDispId)
    { 
      return E_NOTIMPL;
    }

	STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,
               VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);
	
	//  _ISocketProxyEvents
	STDMETHOD(NewConn)(BSTR bstrSID);

	STDMETHOD(RecMsgEvent)(BSTR bstrSID,ULONG nReqID,BSTR bstrReqName,BSTR bstrRecText);
	STDMETHOD(RecTextEvent)(BSTR bstrSID,BSTR bstrText);
	STDMETHOD(RecByte)(BSTR bstrSID,BYTE* pContent,ULONG nLen);

	STDMETHOD(RecErrEvent)(ULONG nSocketID,ULONG nReqID,BSTR bstrErrInfo);
	STDMETHOD(CloseEvent)(BSTR bstrSID,BSTR bstrReason);
};