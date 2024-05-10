// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CComVariant	g_varWebHeader = CComVariant(L"Accept: */*\r\nAccept-Language: zh-CN\r\nAccept-Charset: utf-8\r\nContent-Type:  application/x-www-form-urlencoded\r\n");

int Us2ToChar(const CString& strSrc, char** pDst,int nCodePage)
{
	int iLength=::WideCharToMultiByte(nCodePage,0,strSrc,-1,NULL,0,NULL,NULL);
 	if(!iLength)
		return iLength;
	iLength++;
    char *strA=NULL;
	strA= new char[iLength];
	if(strA==NULL)
		return 0;
	::memset(strA,0,iLength*sizeof(char));
	int iLength1 = 0;
	if(nCodePage)
		iLength1 = ::WideCharToMultiByte(nCodePage,WC_COMPOSITECHECK,strSrc,-1,strA,iLength-1,NULL,NULL);
	else
		iLength1 = ::WideCharToMultiByte(nCodePage,0,strSrc,-1,strA,iLength-1,NULL,NULL);
	*pDst=strA;
	return iLength1;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_spiWebBrowser = NULL;
	m_spiHtmlDoc = NULL;

	m_nSocketProxyCookie = 0;
	m_spiSocketProxy = NULL;
	m_spiSocketProxyEvent = NULL;
	//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)m_hWnd) + L" MainWnd");
	this->PostMessage(WM_APP,0,0);	/// 异步初始化侦听服务
	SetCheckTimer(500);
	return TRUE;
}

BOOL FindProName(DWORD dwPID,CString& strFileName)
{
	BOOL bFindFlag = FALSE;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);
	DWORD dwGetProcessID = dwPID;
	if(!dwGetProcessID)
		dwGetProcessID = ::GetCurrentProcessId();
	for(;fok ;fok = Process32Next(hSnapshot , &pe))
	{
		if(pe.th32ProcessID == dwGetProcessID)
		{
			strFileName = _wcsupr(pe.szExeFile);
			strFileName.MakeLower();
			bFindFlag = TRUE;
			break;
		}
	}	
	
	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return bFindFlag;
}

LRESULT CMainDlg::OnTimer ( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	if(wParam == WRL_BROWSER_CHECKTIMER)
	{
		DWORD dwPID = 0;
		/// 检查顶层窗口是否为启动的浏览器，如果是，启动的弹窗应用置于前端，此操作非必要，根据自己需要来做
		HWND hWnd = ::GetForegroundWindow();
		if(NULL == hWnd)
			return 0;
		::GetWindowThreadProcessId(hWnd,&dwPID);
		if(dwPID != ::GetCurrentProcessId())
		{
			/// 根据进程ID获得执行程序文件名，判断是否为对应的浏览器
			CString strExeFile;
			FindProName(dwPID,strExeFile);
			CString strExeName;
			switch(m_eBrowserType)
			{
			case BROWSERTYPE_IE:
				strExeName = L"iexplore.exe";
				break;
			case BROWSERTYPE_EDGE:
				strExeName = L"msedge.exe";
				break;
			case BROWSERTYPE_FIREFOX:
				strExeName = L"firefox.exe";
				break;
			case BROWSERTYPE_OPERA:
				strExeName = L"opera.exe";
				break;
			case BROWSERTYPE_BRAVE:
				strExeName = L"brave.exe";
				break;
			case BROWSERTYPE_VIVALDI:
				strExeName = L"vivaldi.exe";
				break;
			case BROWSERTYPE_360:
				strExeName = L"360chrome.exe";
				break;
			case BROWSERTYPE_360X:
				strExeName = L"360chromex.exe";
				break;
			case BROWSERTYPE_360SE:
				strExeName = L"360se.exe";
				break;
			case BROWSERTYPE_360ENT:
				strExeName = L"360ent.exe";
				break;
			case BROWSERTYPE_QQ:
				strExeName = L"qqbrowser.exe";
				break;
			case BROWSERTYPE_SOGOU:
				strExeName = L"sogouexplorer.exe";
				break;
			default:
				strExeName = L"chrome.exe";
				break;
			}
			if(-1 != strExeFile.CompareNoCase(strExeName))
			{
				::BringWindowToTop(m_hWnd);
				::SetForegroundWindow(m_hWnd);
			}
		}
	}
	return 0;
}

void CMainDlg::InitWebCtrl()
{
	CRect rcClient;
	this->GetClientRect(&rcClient);
	rcClient.left += 5;
	rcClient.top += 5;
	rcClient.right -= 5;
	rcClient.bottom -= 150;
	
	if(!m_ctlWebWindow.IsWindow())
	{
		HWND hWndWB = m_ctlWebWindow.Create(m_hWnd,rcClient,NULL,
            WS_CHILD|WS_VISIBLE,0,IDC_WEB_BROWSER);
        ATLASSERT(hWndWB);
		if(!::IsWindow(hWndWB))
			this->MessageBox(L"初始化IE控件窗口失败！");
	}
	m_ctlWebWindow.SetWindowPos(this->m_hWnd,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),SWP_SHOWWINDOW);

	if(NULL == m_spiWebBrowser)
	{
		//Create the WebBrowser control(IE) with CLSID
		CComPtr<IUnknown> spUnkWebBrowser = NULL;
		HRESULT hr = spUnkWebBrowser.CoCreateInstance(CLSID_WebBrowser);
		ATLASSERT(SUCCEEDED(hr));
		//Attach the WebBrowser control to host window
		if(NULL != spUnkWebBrowser)
		{
			m_ctlWebWindow.AttachControl(spUnkWebBrowser,NULL);
			spUnkWebBrowser = NULL;
		}

		//Advise sink map
		hr = AtlAdviseSinkMap(this,true);
		ATLASSERT(SUCCEEDED(hr));

		//Query interface
		hr = m_ctlWebWindow.QueryControl(&m_spiWebBrowser);
		if(FAILED(hr))
			this->MessageBox(L"获取IE控件对象失败！");

		if(NULL != m_spiWebBrowser)
		{
			m_spiWebBrowser->put_Silent(VARIANT_TRUE);
			CComVariant varVal;
			varVal.Clear();
			m_spiWebBrowser->Navigate(CComBSTR(L"about:blank"),&varVal,&varVal,&varVal,&g_varWebHeader);
		}
	}
}

void CMainDlg::SetCheckTimer(UINT nElapse)
{
	if(m_nTimerID)
		return;
	m_nTimerID = ::SetTimer(this->m_hWnd,WRL_BROWSER_CHECKTIMER,nElapse,NULL);
}

void CMainDlg::KillCheckTimer()
{
	if(!m_nTimerID)
		return;
	::KillTimer(this->m_hWnd,m_nTimerID);
	m_nTimerID = 0;
}

LRESULT CMainDlg::OnOpenUrl(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiWebBrowser)
	{
		this->MessageBox(L"还未初始化IE控件！");
		return 0;
	}
	if(m_strOpenUrl.IsEmpty())
	{
		this->MessageBox(L"打开URL为空！");
		return 0;
	}
	CComVariant varVal;
	varVal.Clear();
	HRESULT hRet = m_spiWebBrowser->Navigate(CComBSTR(m_strOpenUrl),\
		&varVal,&varVal,&varVal,&g_varWebHeader);
	if(SUCCEEDED(hRet))
		m_ctlWebWindow.SetFocus();
	else
	{
		CString strErrCode;
		strErrCode.Format(L"打开网站失败，错误码:%ld",hRet);
		this->MessageBox(strErrCode);
	}
	return 1;
}

void CMainDlg::NavigateComplete2(IDispatch *pDisp, VARIANT *pvarURL)
{
	if(NULL == pvarURL)
		return;
	if(NULL == m_spiHtmlDoc)
		m_spiHtmlDoc = this->GetHtmlDoc();
	if(NULL != m_spiHtmlDoc)
	{
		m_DocHostUIHandler.SetHandler(m_spiWebBrowser);
	}
}

void CMainDlg::BeforeNavigate2(IDispatch *pDisp,
          CComVariant &URL, CComVariant &Flags, CComVariant &TargetFrameName,
          CComVariant &PostData, CComVariant &Headers, VARIANT_BOOL* Cancel)
{
	(*Cancel) = VARIANT_FALSE;
	CString strUrl(URL.bstrVal);
}

void CMainDlg::OnNewWindow3(LPDISPATCH* ppDisp, VARIANT_BOOL * Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
{
	/// 取消新窗口
	(*Cancel) = VARIANT_TRUE;

	if(NULL == m_spiWebBrowser)
		return;
	HRESULT hRet(E_FAIL);  
	if(NULL == m_spiHtmlDoc)
	    m_spiHtmlDoc = GetHtmlDoc();
    if (NULL == m_spiHtmlDoc)
        return;
    CComPtr <IHTMLElement> spiHtmlElement = NULL;
    hRet = m_spiHtmlDoc->get_activeElement(&spiHtmlElement);
	if(NULL == spiHtmlElement)
		return;
    CComPtr <IHTMLAnchorElement> spiAnchorElement = NULL;  
    hRet = spiHtmlElement->QueryInterface(IID_IHTMLAnchorElement, (void**)&spiAnchorElement);  
    if (NULL == spiAnchorElement)  
    {
		spiHtmlElement = NULL;
		return;
	}
    CComBSTR bstrElementHref;
	/// 获取当前点击的链接地址  
    hRet = spiAnchorElement->get_href(&bstrElementHref);
	spiAnchorElement = NULL;
	spiHtmlElement = NULL;

	ATLASSERT(bstrElementHref.Length());
	CString strReqUrl(bstrElementHref.m_str);
	bstrElementHref.Empty();
	if(0 == strReqUrl.Left(2).CompareNoCase(L"//"))
	{
		strReqUrl.Insert(0,L"http:");
	}
	CComVariant varVal;
	varVal.Clear();
	hRet = m_spiWebBrowser->Navigate(CComBSTR(strReqUrl),&varVal,&varVal,&varVal,&g_varWebHeader);
	strReqUrl.Empty();
}

CComPtr <IHTMLDocument2> CMainDlg::GetHtmlDoc()
{
	CComPtr <IHTMLDocument2> spiHtmlDoc = NULL;
	if(NULL == m_spiWebBrowser)
		return spiHtmlDoc;
    CComPtr< IDispatch > spdispDoc = NULL;
    HRESULT hRet = m_spiWebBrowser->get_Document(&spdispDoc);
    if (FAILED(hRet) || NULL == spdispDoc)  
        return spiHtmlDoc;
    CComPtr <IHTMLDocument2> spDoc = NULL;
	hRet = spdispDoc->QueryInterface(IID_IHTMLDocument2,(void **)&spiHtmlDoc);
	spdispDoc = NULL;
	return spiHtmlDoc;
}

void CMainDlg::DocumentComplete(IDispatch *pDisp, VARIANT *pvarURL)
{
	CString strUrl(pvarURL->bstrVal);
}

LRESULT CMainDlg::OnInitConn(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(NULL != m_spiSocketProxy)
		return 0;/// 已经初始化
	HRESULT hRet(E_FAIL);
	hRet = m_spiSocketProxy.CreateInstance(__uuidof(SocketProxy));
	if(NULL == m_spiSocketProxy)
	{
		InitWebCtrl();
		this->MessageBox(L"Web Socket组件还未正常注册成功！");
		return 0;
	}
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CProxyServerEvent>::CreateInstance(&(m_spiSocketProxyEvent));
#else
	m_spiSocketProxyEvent = new CComObject< CProxyServerEvent >;
#endif
	ATLASSERT(m_spiSocketProxyEvent);
	if(NULL != m_spiSocketProxyEvent)
	{
		m_spiSocketProxyEvent->SetCallPara(this->m_hWnd);
		//建立事件连接
		hRet = AtlAdvise(m_spiSocketProxy,this->m_spiSocketProxyEvent->GetUnknown(),
			DIID__ISocketProxyEvents,&this->m_nSocketProxyCookie);
		ATLASSERT(SUCCEEDED(hRet));
		if(!m_nPort)
			m_nPort = 12800;
		USHORT nTruePort = 0;
		hRet = m_spiSocketProxy->Listen(m_nPort,CComBSTR(m_strSID),CComBSTR(m_strAuthInfo),&nTruePort);
		if(FAILED(hRet))
		{
			CComBSTR bstrErr;
			m_spiSocketProxy->get_LastErrInfo(NULL,&bstrErr);
			this->MessageBox(bstrErr.m_str);
			bstrErr.Empty();
		}
		else
		{
			::BringWindowToTop(m_hWnd);
			::SetForegroundWindow(m_hWnd);
		}
	}
	InitWebCtrl();
	return 1;
}

LRESULT CMainDlg::OnNewConnect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	BSTR bstrSID = (BSTR)lParam;
	/// 这里记录连接标识，然后可以调用通知发送信息
	m_mapConnID[bstrSID] = NULL;
	this->GetDlgItem(IDC_EDIT_LOG).SetWindowText(CString(L"收到新连接通知：") + bstrSID);
	return 0;
}

LRESULT CMainDlg::OnHttpReq(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    /// 收到HTTP协议请求，主要用于前端同步请求，比如前端需要等待请求完成浏览器才能继续操作
	HttpReqData* phd = (HttpReqData*)lParam;
	if(NULL != phd)
	{
		/// 这里执行阻塞操作，比如弹出模态对话框
		this->MessageBox(L"阻塞执行",L"提示",MB_OK);
		/// 设置返回值
		phd->strRet = L"{\"ret\":0,\"data\":{\"Ret\":0,\"Code\":1}}";
	}
	return 0;
}

LRESULT CMainDlg::OnHttpPort(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	m_nHttpPort = (USHORT)lParam;
	return 0;
}

LRESULT CMainDlg::OnRecError(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMainDlg::OnRecMessage(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiSocketProxyEvent)
		return 0;
	IJsonServicePtr spiJsonService = NULL;
	HRESULT hRet = spiJsonService.CreateInstance(__uuidof(JsonService));
	if(NULL == spiJsonService)
		return 0;
	BSTR bstrSID = (BSTR)wParam;
	ULONG nReqID = (ULONG)lParam;
	CRecData* pRecData = m_spiSocketProxyEvent->GetCatchData(bstrSID,nReqID,TRUE);
	if(NULL == pRecData)
	{
		spiJsonService = NULL;
		return 0;
	}
	CString strReqName = pRecData->strReqName;

	CString strMsg;
	strMsg.Format(L"收到请求为 %s 的JSON数据包:%s",strReqName,pRecData->strRecText);
	this->GetDlgItem(IDC_EDIT_LOG).SetWindowText(strMsg);
	strMsg.Empty();

	ULONG nSendID = 0;
	m_spiSocketProxy->AsynSendText(bstrSID,\
		CComBSTR(CString(L"桌面APP收到请求参数:")+pRecData->strRecText),&nSendID);
	/// 解析收到的命令
	VARIANT_BOOL bLoadFlag = VARIANT_FALSE;
	spiJsonService->put_CodingType(CODINGTYPE_US2);
	spiJsonService->ParseString(CComBSTR(pRecData->strRecText),&bLoadFlag);
	if(VARIANT_FALSE == bLoadFlag)
	{
		spiJsonService = NULL;
		delete pRecData;
		pRecData = NULL;
		return 0;
	}
	if(0 == strReqName.CompareNoCase(L"Demo_OpenUrl"))
	{
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"url"),&bstrVal);
		m_strOpenUrl.Empty();
		m_strOpenUrl = bstrVal.m_str;
		bstrVal.Empty();
		/// 通知打开网站
		this->PostMessage(WM_APP_OPENURL,0,0);
	}
	else if(0 == strReqName.CompareNoCase(L"Demo_HttpPort"))
	{
		/// 请求返回HTTP服务的侦听端口
		nSendID = 0;
		CString strRetInfo;
		strRetInfo.Format(L"{\"ret\":0,\"data\":{\"Port\":%ld}}",m_nHttpPort);
		m_spiSocketProxy->AsynSendText(bstrSID,\
			CComBSTR(strRetInfo),&nSendID);
		strRetInfo.Empty();
	}
	spiJsonService = NULL;
	delete pRecData;
	pRecData = NULL;
	return 0;
}

LRESULT CMainDlg::OnRecText(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	BSTR bstrContent = (BSTR)lParam;
	CString strMsg;
	strMsg.Format(L"收到文本数据包:%s",(CString)bstrContent);
	this->GetDlgItem(IDC_EDIT_LOG).SetWindowText(strMsg);
	strMsg.Empty();
	ULONG nReqID = 0;
	m_spiSocketProxy->AsynSendText((BSTR)wParam,CComBSTR(CString(L"桌面APP收到内容:")+bstrContent),&nReqID);
	return 0;
}

LRESULT CMainDlg::OnCloseConnect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	BSTR bstrSID = (BSTR)wParam;
	BSTR bstrContent = (BSTR)lParam;
	CString strMsg;
	strMsg.Format(L"收到标识为 %s 的连接通知关闭:%s",(CString)bstrSID,(CString)bstrContent);
	this->GetDlgItem(IDC_EDIT_LOG).SetWindowText(strMsg);
	strMsg.Empty();
	return 0;
}

LRESULT CMainDlg::OnRecByte(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	/// 暂未实现，通知收到字节流
	return 0;
}

void CMainDlg::UnAdviseSocket()
{
	if(0 == m_nSocketProxyCookie)
		return;
	AtlUnadvise(m_spiSocketProxy,DIID__ISocketProxyEvents,m_nSocketProxyCookie);
	m_nSocketProxyCookie = 0;
	m_spiSocketProxyEvent = NULL;

	m_spiSocketProxy->Close();
	m_spiSocketProxy = NULL;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	if(NULL == m_spiSocketProxy)
		return 0;
	VARIANT_BOOL bIsReady = VARIANT_FALSE;
	m_spiSocketProxy->get_Ready(&bIsReady);
	/// 通过命令行传进来端口
	if(VARIANT_TRUE != bIsReady)
	{
		this->MessageBox(L"侦听服务还未就绪！");
		return 0;
	}
	CComBSTR bstrContent;
	this->GetDlgItem(IDC_EDIT_CONTENT).GetWindowText(&bstrContent);
	ULONG nReqID = 0;
	CString strContent(bstrContent.m_str);
	if(!m_mapConnID.size())
	{
		this->MessageBox(L"还未有活动连接！");
		return 0;
	}
	if(strContent.IsEmpty())
		return 0;
	CString strReturn;
	strContent.Replace(L"\"",L"\\\"");
	strReturn.Format(_T("{\"rid\":%ld,\"req\":\"Send\",\"data\":{\"Content\":\"%s\"}}"), \
		nReqID,strContent);
	HRESULT hRet = m_spiSocketProxy->AsynSendText(CComBSTR(m_mapConnID.begin()->first),\
		CComBSTR(strReturn),&nReqID);
	if(!nReqID || FAILED(hRet))
	{
		CComBSTR bstrErrInfo;
		m_spiSocketProxy->get_LastErrInfo(CComBSTR(m_mapConnID.begin()->first),&bstrErrInfo);
		this->MessageBox(bstrErrInfo.m_str);
		bstrErrInfo.Empty();
	}
	else
	{
		this->GetDlgItem(IDC_EDIT_CONTENT).SetWindowText(L"");
	}
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UnAdviseSocket();

	m_spiHtmlDoc = NULL;
	m_DocHostUIHandler.ReleaseCom();
	if(NULL != m_spiWebBrowser)
	{
		AtlAdviseSinkMap(this,false);
		/// 调用退出
		m_spiWebBrowser->Quit();
		m_spiWebBrowser = NULL;
	}

	if(m_ctlWebWindow.IsWindow())
		m_ctlWebWindow.DestroyWindow();

	EndDialog(wID);
	return 0;
}
