// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>
#include "ProxyServerEvent.h"
#include "BrowserHost.h"

#define WM_APP_OPENURL			WM_USER + 500
#define WRL_BROWSER_CHECKTIMER	9

typedef std::map	<CString,CString>		CSTRING_MAP;
typedef std::map	<CString,ULONG_PTR>		STRINGULONG_MAP;

int Us2ToChar(const CString& strSrc, char** pDst,int nCodePage = CP_ACP);

class CMainDlg : public CAxDialogImpl<CMainDlg>,public IDispEventImpl<IDC_WEB_BROWSER,CMainDlg>
{
protected:

	/// 嵌套IE控件
	CAxWindow					m_ctlWebWindow;
	/// 浏览器访问控件
    CComPtr<IWebBrowser2>		m_spiWebBrowser;
	/// 加载HTML页面文档对象
	CComPtr <IHTMLDocument2>	m_spiHtmlDoc;

	/// 定制行为
	CDocHostUIHandler			m_DocHostUIHandler;

	/// 定时器标识
	UINT_PTR		m_nTimerID;

	/// 浏览器类型
	EWrlBrowserType m_eBrowserType;

	/// WS服务侦听端口
	USHORT			m_nPort;
	/// HTTP服务侦听端口
	USHORT			m_nHttpPort;

	/// 侦听会话ID
	CString			m_strSID;
	/// 侦听授权信息
	CString			m_strAuthInfo;

	CSTRING_MAP		m_mapPara;

	/// 准备打开的URL地址
	CString			m_strOpenUrl;

	/// 记录所有的连接标识
	STRINGULONG_MAP	m_mapConnID;

	/// 通讯代理
	ISocketProxyPtr						m_spiSocketProxy;
	/// 事件COOKIE
	DWORD								m_nSocketProxyCookie;
	/// 任务事件对象
	CComObject<CProxyServerEvent>*		m_spiSocketProxyEvent;

	void KillCheckTimer();

	void SetCheckTimer(UINT nElapse);

	void UnAdviseSocket();

	void InitWebCtrl();

	/// 获得HTML文档对象
	CComPtr <IHTMLDocument2> GetHtmlDoc();

	/// 浏览器事件
	void __stdcall OnNewWindow3(LPDISPATCH* ppDisp, VARIANT_BOOL * Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
	void __stdcall BeforeNavigate2(IDispatch *pDisp,
          CComVariant &URL, CComVariant &Flags, CComVariant &TargetFrameName,
          CComVariant &PostData, CComVariant &Headers, VARIANT_BOOL* Cancel);

	void __stdcall NavigateComplete2(IDispatch *pDisp, VARIANT *pvarURL);
	void __stdcall DocumentComplete(IDispatch *pDisp, VARIANT *pvarURL);

public:
	CMainDlg()
		:m_nTimerID(0)
		,m_nPort(12900)
		,m_nHttpPort(0)
		,m_eBrowserType(BROWSERTYPE_UNKNOWN)
		,m_spiWebBrowser(NULL)
		,m_spiHtmlDoc(NULL)
		,m_spiSocketProxy(NULL)
		,m_spiSocketProxyEvent(NULL)
		,m_nSocketProxyCookie(0)
	{
	}

	enum { IDD = IDD_MAINDLG };

	void SetListenPara(CSTRING_MAP& mapPara)
	{
		m_mapPara = mapPara;
		CSTRING_MAP::iterator it = m_mapPara.find(L"PORT");
		ATLASSERT(it != m_mapPara.end());
		if(it != m_mapPara.end())
		{
			char* szCmd = NULL;
			Us2ToChar(it->second,&szCmd);
			if(NULL != szCmd)
			{
				m_nPort = atoi(szCmd);
				delete []szCmd;
				szCmd = NULL;
			}
			ATLASSERT(m_nPort > 0 && m_nPort < 65536);
		}
		if(!m_nPort)
			m_nPort = 12800;
		it = m_mapPara.find(L"SID");
		ATLASSERT(it != m_mapPara.end());
		if(it != m_mapPara.end())
			m_strSID = it->second;
		it = m_mapPara.find(L"AI");
		if(it != m_mapPara.end())
			m_strAuthInfo = it->second;
	}

	BEGIN_SINK_MAP(CMainDlg)
        SINK_ENTRY(IDC_WEB_BROWSER,DISPID_NEWWINDOW3, OnNewWindow3)
	    SINK_ENTRY(IDC_WEB_BROWSER,DISPID_DOCUMENTCOMPLETE, DocumentComplete)
		SINK_ENTRY(IDC_WEB_BROWSER,DISPID_BEFORENAVIGATE2, BeforeNavigate2)
		SINK_ENTRY(IDC_WEB_BROWSER,DISPID_NAVIGATECOMPLETE2, NavigateComplete2)
    END_SINK_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_APP, OnInitConn)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_APP_OPENURL, OnOpenUrl)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_NEWCONNECT, OnNewConnect)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_RECMESSAGE, OnRecMessage)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_RECTEXT, OnRecText)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_CLOSECONN, OnCloseConnect)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_BYTE, OnRecByte)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_HTTPREQ, OnHttpReq)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_HTTPPORT, OnHttpPort)
		MESSAGE_HANDLER(WM_PROXYSEREREVENT_RECERR, OnRecError)

		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

//  Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnInitConn(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
	LRESULT OnOpenUrl(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnNewConnect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRecMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRecText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRecByte(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseConnect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHttpReq(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHttpPort(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRecError(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
