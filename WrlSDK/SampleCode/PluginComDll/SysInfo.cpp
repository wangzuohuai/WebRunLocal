// SysInfo.cpp : CSysInfo 的实现

#include "stdafx.h"
#include "SysInfo.h"
#include <atlcomtime.h>

// CSysInfo
STDMETHODIMP CSysInfo::RecText(BSTR bstrContent)
{
	// TODO: Add your implementation code here
	ATLASSERT(NULL != m_spiSocketConnect);
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
		return E_POINTER;
	m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);
	/// 请处理收到的文本信息，这里演示直接回复原内容
	ULONG nReqID = 0;
	m_spiSocketConnect->AsynSendText(bstrContent,&nReqID);
	return S_OK;
}

STDMETHODIMP CSysInfo::RecJson(ULONG nReqID,BSTR bstrReqName,BSTR bstrContent)
{
	// TODO: Add your implementation code here
	ATLASSERT(NULL != m_spiSocketConnect);
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
		return E_POINTER;
	/// 请处理收到的普通JSON数据包
	return S_OK;
}
