#pragma once

#define WM_PROXYSEREREVENT_NEWCONNECT		WM_USER + 2000
#define WM_PROXYSEREREVENT_RECMESSAGE		WM_USER + 2001
#define WM_PROXYSEREREVENT_RECTEXT			WM_USER + 2002
#define WM_PROXYSEREREVENT_BYTE				WM_USER + 2003
#define WM_PROXYSEREREVENT_CLOSECONN		WM_USER + 2005

#ifndef NEED_FINAL_CONSTRUCT
#define NEED_FINAL_CONSTRUCT
#endif

// CProxyServerEvent
class CProxyServerEvent : 
	public CComObjectRoot,
	//实现事件通知接口
	public _ISocketProxyEvents
{
protected:

	/// 消息窗口
	HWND		m_hMsgWnd;

	/// 当前会话ID
	CString		m_strSID;

	/// 当前请求序号
	ULONG		m_nReqID;

public:
	CProxyServerEvent()
		:m_hMsgWnd(NULL)
	{
	}

	~CProxyServerEvent()
	{
	}

	CString GetCurSID()
	{
		return m_strSID;
	}

	ULONG GetCurReqID()
	{
		return m_nReqID;
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

	STDMETHOD(RecMsgEvent)(BSTR bstrSID,ULONG nReqID,BSTR nReqName,BSTR bstrRecText);
	STDMETHOD(RecTextEvent)(BSTR bstrSID,BSTR bstrText);
	STDMETHOD(RecByte)(BSTR bstrSID,BYTE* pContent,ULONG nLen);

	STDMETHOD(CloseEvent)(BSTR bstrSID,BSTR bstrReason);
};