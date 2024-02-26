// TextFile.cpp : CTextFile 的实现

#include "stdafx.h"
#include "TextFile.h"
#include "BaseFuncLib.h"

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

ATL::CString CTextFile::GetFileFolderPath(int nFoldID)
{
	ATL::CString strSpecialPath;
	if(NULL == m_hLoadDll)
		m_hLoadDll = LoadLibrary(L"Shell32.dll");
	if(NULL == m_hLoadDll)
		return strSpecialPath;
	if(NULL == m_pGetFolderPath)
		m_pGetFolderPath = (lpSHGetFolderPath)GetProcAddress(m_hLoadDll,"SHGetSpecialFolderPathW"); 	
	if(NULL == m_pGetFolderPath)
		return strSpecialPath;
	TCHAR szSpecialPath[MAX_PATH];
	::memset(szSpecialPath,0, MAX_PATH *sizeof(TCHAR));
	BOOL bGetFlag = m_pGetFolderPath(NULL,szSpecialPath,nFoldID,TRUE);
	if(!bGetFlag)
	{
		CBaseFuncLib::WriteLastLogToFile(::GetLastError(),L"GetFileFolderPath");
		return _T("");
	}
	strSpecialPath = szSpecialPath;
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
	if(0 == strReqName.CompareNoCase(L"Demo_SetFilePath"))
	{
		CComBSTR bstrVal;
		/// 直接设置路径
		hRet = spiJsonService->GetStringValue(CComBSTR(L"Path"),&bstrVal);
		if(bstrVal.Length())
		{
			m_strFilePath = bstrVal.m_str;
			m_strFilePath.Replace(L"/",L"\\");
			bstrVal.Empty();
		}
		/// 指定路径类型来设置路径
		CComVariant varType;
		hRet = spiJsonService->GetVariantValue(CComBSTR(L"Type"),&varType);
		if(SUCCEEDED(hRet))
		{
			varType.ChangeType(VT_I4);
			m_strFilePath = GetFileFolderPath(varType.iVal);
			varType.Clear();
		}
		CString strReturn,strPath(m_strFilePath);
		strPath.Replace(L"\\",L"/");
		strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"Path\":\"%s\"}}"), \
			nReqID,strPath);
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn),NULL);	
	}
	else if(0 == strReqName.CompareNoCase(L"Demo_WriteFile"))
	{
		CString strFilePath = m_strFilePath;
		if(strFilePath.GetLength() < 3)
			strFilePath = GetFileFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
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
		CString strReturn;
		strContent.Replace(L"\"",L"\\\"");
		strFilePath.Replace(L"\\",L"/");
		strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"Content\":\"%s\"}}"), \
			nReqID,strFilePath + L" 文件写入完成");
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn),NULL);
	}
	else if(0 == strReqName.CompareNoCase(L"Demo_ReadFile"))
	{
		CString strFilePath = m_strFilePath;
		if(strFilePath.GetLength() < 3)
			strFilePath = GetFileFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"Name"),&bstrVal);;
		strFilePath += bstrVal.m_str;
		bstrVal.Empty();

		CString strReturn;
		CTextFileRead ReadFile(strFilePath);
		string strLine;
		while(ReadFile.ReadLine(strLine))
		{
			if(!strLine.length())
				continue;/// 空行不发送
			strReturn.Empty();
			strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"ReadLine\":\"%s\"}}"), \
				nReqID,CString(strLine.c_str()));
			m_spiSocketConnect->AsynSendText(CComBSTR(strReturn),NULL);
			strLine.clear();
		}
		ReadFile.Close();
		strReturn.Empty();
		strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"End\":\"%s\"}}"), \
			nReqID,L"读取完成");
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn), NULL);
	}
	else
	{
		CString strReturn,strContent(bstrContent);
		strContent.Replace(L"\"",L"\\\"");
		strReturn.Format(L"{\"rid\":%ld,\"data\":{\"Req\":\"%s\",\"Content\":\"%s\",\"Status\":\"不识别的请求\"}}",\
			nReqID,strReqName,strContent);
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn), NULL);
	}
	return S_OK;
}