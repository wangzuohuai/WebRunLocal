// WrlRegKey.cpp : 注册表封装实现

#include "stdafx.h"
#include "WrlRegKey.h"

CWrlRegKey::CWrlRegKey(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	m_bOpenFlag = FALSE;
	m_dwLastErr = 0;
	if(!strMainRegPath.IsEmpty())
		NewRegPath(strMainRegPath,strChildName,hKey,samDesired);
}

void CWrlRegKey::Close()
{
	if(!m_bOpenFlag)
		return;
	CRegKey::Close();
	m_dwLastErr = 0;
	m_bOpenFlag = FALSE;
}

BOOL CWrlRegKey::NewRegPath(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	Close();

	ATL::CString strRegPath;
	if(!strChildName.IsEmpty())
	{
		if(strMainRegPath.GetLength())
			strRegPath.Format(_T("%s\\%s"),strMainRegPath,strChildName);
		else
			strRegPath = strChildName;
	}
	else
		strRegPath = strMainRegPath;

	m_dwLastErr = Open(hKey,strRegPath,samDesired);
	if(ERROR_SUCCESS != m_dwLastErr)
	{
		if(ERROR_ACCESS_DENIED != m_dwLastErr)
		{
			/// 不存在，自动创建
			m_dwLastErr = Create(hKey,strRegPath,NULL,0,samDesired);
		}
	}
	if(ERROR_SUCCESS == m_dwLastErr)
	{
		m_bOpenFlag = TRUE;
	}
	return m_bOpenFlag;
}

BOOL CWrlRegKey::GetRegStringVal(const ATL::CString& strKeyName,
								  ATL::CString& strKeyValue)
{
	BOOL bGetFlag = FALSE;
	if(!m_bOpenFlag)
		return bGetFlag;
	DWORD dwSize = 0;
	m_dwLastErr = CRegKey::QueryStringValue(strKeyName,NULL,&dwSize);

	TCHAR *szBuf = new TCHAR[dwSize+1];
	if(NULL == szBuf)
		return bGetFlag;
	memset(szBuf,0,(dwSize+1)*sizeof(TCHAR));
	m_dwLastErr = CRegKey::QueryStringValue(strKeyName,szBuf,&dwSize);
	if(m_dwLastErr != ERROR_SUCCESS)
	{
		if(NULL != szBuf)
		{
			delete []szBuf;
			szBuf = NULL;
		}
		return bGetFlag;
	}
	bGetFlag = TRUE;
	strKeyValue = szBuf;
	if(NULL != szBuf)
	{
		delete []szBuf;
		szBuf = NULL;
	}
	return bGetFlag;
}

BOOL CWrlRegKey::SetRegStringVal(const ATL::CString& strKeyName,
								  const ATL::CString& strKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetStringValue(strKeyName,strKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::GetRegDwordVal(const ATL::CString& strKeyName,
								 DWORD& dwKeyValue)
{
	dwKeyValue = 0;
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::QueryDWORDValue(strKeyName,dwKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::SetRegDwordVal(const ATL::CString& strKeyName,
								 DWORD dwKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetDWORDValue(strKeyName,dwKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::GetRegBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes)
{
	if(!m_bOpenFlag)
		return FALSE;
	ATLASSERT(ppValue && pnBytes);
	m_dwLastErr = CRegKey::QueryBinaryValue(strKeyName,NULL,pnBytes);
	if(0 == *pnBytes || m_dwLastErr != ERROR_SUCCESS)
	{
		*ppValue = NULL;
		return FALSE;
	}
	ULONG nLen = (*pnBytes)+1;
	*ppValue = new BYTE[nLen];
	memset(*ppValue,0,nLen*sizeof(BYTE));
	m_dwLastErr = CRegKey::QueryBinaryValue(strKeyName,*ppValue,pnBytes);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::SetRegBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetBinaryValue(strKeyName,Value,nBytes);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::GetRegGuidValue(const ATL::CString &strKeyName,GUID &Value)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::QueryGUIDValue(strKeyName,Value)==ERROR_SUCCESS;
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::SetRegGuidValue(const ATL::CString &strKeyName,GUID Value)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetGUIDValue(strKeyName,Value);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::DelKeyName(const ATL::CString& strKeyName)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::DeleteValue(strKeyName);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}
