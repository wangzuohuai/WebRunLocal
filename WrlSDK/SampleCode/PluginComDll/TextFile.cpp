// TextFile.cpp : CTextFile 的实现

#include "stdafx.h"
#include <shlobj.h>
#include "TextFile.h"

// CTextFile
STDMETHODIMP CTextFile::RecText(BSTR bstrContent)
{
	// TODO: Add your implementation code here
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
	{
		return E_POINTER;
	}
	HRESULT hRet = m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);

	/// 请处理收到的文本信息，这里演示直接回复原内容
	hRet = m_spiSocketConnect->AsynSendText(bstrContent,NULL);
	ATLASSERT(SUCCEEDED(hRet));
	/// 也可以直接发送二进制数据流，建议把二进制流进行编码后再传输
//	BYTE szContent[] = "TestByte";
//	hRet = m_spiSocketConnect->AsynSendByte(szContent,strlen((char*)szContent),NULL);
	return hRet;
}

ATL::CString GetSpecialFolderPath(long lFoldID)
{
	TCHAR szSpecialPath[MAX_PATH];
	::memset(szSpecialPath,0,MAX_PATH*sizeof(TCHAR));
	BOOL bGetFlag = ::SHGetSpecialFolderPath(NULL,szSpecialPath,lFoldID,TRUE);
	if(!bGetFlag)
	{
		return _T("");
	}

	ATL::CString strSpecialPath(szSpecialPath);
	///路径后面都有"\"
	if(!strSpecialPath.IsEmpty() && 0 != strSpecialPath.Right(1).CompareNoCase(_T("\\")))
		strSpecialPath+=_T("\\");
	return strSpecialPath;
}

STDMETHODIMP CTextFile::RecJson(ULONG nReqID,BSTR bstrReqName,BSTR bstrContent)
{
	// TODO: Add your implementation code here
	ATLASSERT(NULL != m_spiSocketConnect);
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
		return E_POINTER;
	m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);
	IJsonServicePtr spiJsonService = NULL;
	HRESULT hRet = spiJsonService.CreateInstance(__uuidof(JsonService));
	if(NULL == spiJsonService)
		return hRet;/// 创建JSON解析器对象失败
	VARIANT_BOOL bLoadFlag = VARIANT_FALSE;
	spiJsonService->put_CodingType(CODINGTYPE_US2);
	hRet = spiJsonService->ParseString(bstrContent,&bLoadFlag);
	if(VARIANT_FALSE == bLoadFlag)
	{
		spiJsonService = NULL;
		return hRet;
	}
	/// 请处理收到的普通JSON数据包
	/// 在这里解析请求并作出回应
	CString strReqName(bstrReqName);
	if(0 == strReqName.CompareNoCase(L"Demo_WriteFile"))
	{
		CString strFilePath = GetSpecialFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"Name"),&bstrVal);;
		strFilePath += bstrVal.m_str;
		bstrVal.Empty();
		spiJsonService->GetStringValue(CComBSTR(L"Content"),&bstrVal);;
		CString strContent = bstrVal.m_str;
		bstrVal.Empty();

		CTextFileWrite WriteFile(strFilePath);
		WriteFile.Write(strContent);
		WriteFile.Close();
		m_spiSocketConnect->AsynSendText(CComBSTR(strFilePath + L" 文件写入完成"),NULL);
	}
	else if(0 == strReqName.CompareNoCase(L"Demo_ReadFile"))
	{
		CString strFilePath = GetSpecialFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"Name"),&bstrVal);;
		strFilePath += bstrVal.m_str;
		bstrVal.Empty();

		CTextFileRead ReadFile(strFilePath);
		string strLine;
		while(ReadFile.ReadLine(strLine))
		{
			if(!strLine.length())
				continue;/// 空行不发送
			m_spiSocketConnect->AsynSendText(CComBSTR(strLine.c_str()),NULL);
			strLine.clear();
		}
		ReadFile.Close();
		m_spiSocketConnect->AsynSendText(L"读取完成", NULL);
	}
	else
	{
		m_spiSocketConnect->AsynSendText(CComBSTR(CString(L"收到请求:")+bstrReqName),NULL);
		m_spiSocketConnect->AsynSendText(CComBSTR(CString(L"收到内容:") + bstrContent), NULL);
	}
	return S_OK;
}