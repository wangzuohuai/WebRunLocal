#pragma once

#include <exdisp.h>    // browser control definitions
#include <exdispid.h>  // browser event dispatch IDs
#include <mshtmdid.h>

enum CONTEXT_MENU_MODE		// 上下文菜单
	{
		NoContextMenu,			// 无菜单
		DefaultMenu,			// 默认菜单
		TextSelectionOnly,		// 仅文本选择菜单
		CustomMenu				// 自定义菜单
	};

class CDocHostUIHandler : 
	public IDocHostUIHandler,
	public IOleCommandTarget
{
protected:

	LONG	m_cRef;

	// Default interface pointers  
    CComQIPtr<IDocHostUIHandler, &IID_IDocHostUIHandler> m_spDefaultDocHostUIHandler;  

	CComQIPtr<IOleCommandTarget, &IID_IOleCommandTarget> m_spDefaultOleCommandTarget; 

public:
	CDocHostUIHandler()
		:m_cRef(0)
	{
	}

	void ReleaseCom()
	{
		m_cRef = 0;

		if(NULL != m_spDefaultDocHostUIHandler)
			m_spDefaultDocHostUIHandler.Release();
		if(NULL != m_spDefaultOleCommandTarget)
			m_spDefaultOleCommandTarget.Release();
	}

	HRESULT STDMETHODCALLTYPE SetHandler(CComPtr<IWebBrowser2> spWebBrowser)  
    {  
        HRESULT result = E_NOINTERFACE;  
		if(NULL == spWebBrowser)
			return E_POINTER;
        CComPtr<IDispatch> spDisp = NULL;  
        HRESULT hr = spWebBrowser->get_Document(&spDisp);  
        if (SUCCEEDED(hr) && spDisp)  
        {
            // If this is not an HTML document (e.g., it's a Word doc or a PDF), don't sink.  
            CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spHTML(spDisp);  
            if (spHTML)  
            {  
                // Get pointers to default interfaces  
                CComQIPtr<IOleObject, &IID_IOleObject> spOleObject(spDisp);  
                if (spOleObject)  
                {  
                    CComPtr<IOleClientSite> spClientSite = NULL;  
                    hr = spOleObject->GetClientSite(&spClientSite);  
                    if (SUCCEEDED(hr) && spClientSite)  
                    {  
                        m_spDefaultDocHostUIHandler = spClientSite;  
                        m_spDefaultOleCommandTarget = spClientSite;  
                    } 
                }  
                       
                // Set this class to be the IDocHostUIHandler  
                CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDisp);  
                if (spCustomDoc)   
                {  
                    spCustomDoc->SetUIHandler(this);  
                    result = S_OK;
                }
            }  
        }
		spDisp = NULL;
        return result;  
    }

	/// IDocHostUIHandler
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)  
    {  
        HRESULT result = S_OK;  
                       
        if (IsBadWritePtr(ppvObject, sizeof(LPVOID)))  
            result = E_INVALIDARG;  
                       
        if (result == S_OK)  
        {  
            *ppvObject = NULL;  
                       
            if ( IsEqualIID( riid, IID_IUnknown ) )  
                *ppvObject = this;  
            else if ( IsEqualIID( riid, IID_IDispatch ) )  
                             *ppvObject = (IDispatch *) this;  
            else if ( IsEqualIID( riid, IID_IDocHostUIHandler ) )  
                *ppvObject = (IDocHostUIHandler *) this;  
            else if ( IsEqualIID( riid, IID_IOleCommandTarget ) )  
                *ppvObject = (IOleCommandTarget *) this;  
            else  
                result = E_NOINTERFACE;  
        }  
                       
        if (result == S_OK)  
            this->AddRef();  
                       
        return result;  
    }  
                       
    ULONG STDMETHODCALLTYPE AddRef()
	{  
        InterlockedIncrement(&m_cRef);  
        return m_cRef;  
    }  
                       
    ULONG STDMETHODCALLTYPE Release() 
	{  
        ULONG ulRefCount = InterlockedDecrement(&m_cRef);  
        if(m_cRef == 0)  
        {  
            delete this;  
        }  
        return ulRefCount;  
    }  
                                             
    STDMETHOD(ShowContextMenu)(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdTarget, IDispatch FAR* pdispReserved)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->ShowContextMenu(dwID, ppt, pcmdTarget, pdispReserved);  
        return S_FALSE;  
    }  
                       
    STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject,  
                      IOleCommandTarget FAR* pCommandTarget,  
                      IOleInPlaceFrame  FAR* pFrame,  
                      IOleInPlaceUIWindow FAR* pDoc)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);  
        return S_FALSE;  
    }  
                       
    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO FAR *pInfo)  
    {  
		pInfo->cbSize = sizeof(DOCHOSTUIINFO);
        pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_ENABLE_REDIRECT_NOTIFICATION;
        pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
        pInfo->pchHostCss = 0;
        pInfo->pchHostNS = 0;
//      if(m_spDefaultDocHostUIHandler)  
 //        return m_spDefaultDocHostUIHandler->GetHostInfo(pInfo);  
        return S_OK;  
    }  
                       
    STDMETHOD(HideUI)(void)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->HideUI();  
        return S_OK;  
    }  
                       
    STDMETHOD(UpdateUI)(void)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->UpdateUI();  
        return S_OK;  
    }  
                       
    STDMETHOD(EnableModeless)(BOOL fEnable)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->EnableModeless(fEnable);  
        return S_OK;  
    }  
                       
    STDMETHOD(OnDocWindowActivate)(BOOL fActivate)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->OnDocWindowActivate(fActivate);  
        return S_OK;  
    }  
                       
    STDMETHOD(OnFrameWindowActivate)(BOOL fActivate)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->OnFrameWindowActivate(fActivate);  
        return S_OK;  
    }  
                       
    STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow, BOOL fFrameWindow)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->ResizeBorder(prcBorder, pUIWindow, fFrameWindow);  
        return S_OK;  
    }  
                       
    STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID FAR* pguidCmdGroup, DWORD nCmdID)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);  
        return E_NOTIMPL;  
    }  
                       
    STDMETHOD(GetOptionKeyPath)(LPOLESTR FAR* pchKey, DWORD dw)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->GetOptionKeyPath(pchKey, dw);  
        return E_FAIL;  
    }  
                       
    STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            m_spDefaultDocHostUIHandler->GetDropTarget(pDropTarget, ppDropTarget);  
        return S_OK;  
    }  
                       
    STDMETHOD(GetExternal)(IDispatch** ppDispatch)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->GetExternal(ppDispatch);  
        return S_FALSE;  
    }  
                       
    STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);  
        return S_FALSE;  
    }  
                       
    STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet)  
    {  
        if (m_spDefaultDocHostUIHandler)  
            return m_spDefaultDocHostUIHandler->FilterDataObject(pDO, ppDORet);  
        return S_FALSE;  
    }  
                       
    //  
    // IOleCommandTarget  
    //  
    STDMETHOD(QueryStatus)(  
        /*[in]*/ const GUID *pguidCmdGroup,   
        /*[in]*/ ULONG cCmds,  
        /*[in,out][size_is(cCmds)]*/ OLECMD *prgCmds,  
        /*[in,out]*/ OLECMDTEXT *pCmdText)  
    {  
		if(NULL == m_spDefaultOleCommandTarget)
			return E_NOINTERFACE;
        return m_spDefaultOleCommandTarget->QueryStatus(pguidCmdGroup, cCmds, prgCmds, pCmdText);  
    }

	/// 屏蔽JS脚本错误
    STDMETHOD(Exec)(
        /*[in]*/ const GUID *pguidCmdGroup,  
        /*[in]*/ DWORD nCmdID,  
        /*[in]*/ DWORD nCmdExecOpt,  
        /*[in]*/ VARIANTARG *pvaIn,  
        /*[in,out]*/ VARIANTARG *pvaOut)  
    {  
        if (nCmdID == OLECMDID_SHOWSCRIPTERROR)  
        {  
            // Don't show the error dialog, but  
            // continue running scripts on the page.  
            (*pvaOut).vt = VT_BOOL;  
            (*pvaOut).boolVal = VARIANT_TRUE;  
            return S_OK;  
        }
		if(NULL == m_spDefaultOleCommandTarget)
			return E_NOINTERFACE;
        return m_spDefaultOleCommandTarget->Exec(pguidCmdGroup, nCmdID, nCmdExecOpt, pvaIn, pvaOut);  
    }  
};