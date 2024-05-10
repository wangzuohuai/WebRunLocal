/**
 * @file ZMRegKey.cpp
 *
 * Copyright (c) 2013-?,成都佐罗软件
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 注册表操作封装实现
 *
 *
 *
 * @version 
 *
 * @date 2018-09-01
 *
 * @author WZH
 *
 * @history 
 *
 */
// ZMRegKey.cpp : 注册表封装实现

#include "stdafx.h"
#include "WRLRegKey.h"
#include "BaseFuncLib.h"

CWrlRegKey::CWrlRegKey(const CString& strMainRegPath,\
		const CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	m_bOpenFlag = FALSE;
	m_dwLastErr = 0;
	ATLASSERT(strMainRegPath.GetLength());
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

BOOL CWrlRegKey::NewRegPath(const CString& strMainRegPath,\
		const CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	if(m_bOpenFlag)
		Close();

	CString strRegPath;
	if(!strChildName.IsEmpty())
		strRegPath.Format(_T("%s\\%s"),strMainRegPath,strChildName);
	else
		strRegPath = strMainRegPath;

	m_dwLastErr = Open(hKey,strRegPath,samDesired);
	if(ERROR_SUCCESS != m_dwLastErr)
	{
		if(ERROR_ACCESS_DENIED != m_dwLastErr && ERROR_WRITE_PROTECT != m_dwLastErr 
			&& (KEY_WRITE == (KEY_WRITE & samDesired)))
		{
			/// 还不存在指定注册项目，自动创建
			m_dwLastErr = Create(hKey,strRegPath,NULL,0,samDesired);
		}
	}
	if(ERROR_SUCCESS == m_dwLastErr)
	{
		m_bOpenFlag = TRUE;
	}
	return m_bOpenFlag;
}

BOOL CWrlRegKey::GetRegStringVal(
		const CString& strKeyName,CString& strKeyValue)
{
	BOOL bGetFlag = FALSE;
	if(!m_bOpenFlag)
	{
		return bGetFlag;
	}
	DWORD dwSize = 0;
	if(strKeyName.IsEmpty())
		m_dwLastErr = CRegKey::QueryStringValue(NULL,NULL,&dwSize);
	else
		m_dwLastErr = CRegKey::QueryStringValue(strKeyName,NULL,&dwSize);
	if(!dwSize)
	{
		/// 没有值
		return bGetFlag;
	}
	TCHAR *szBuf = new TCHAR[dwSize+1];
	if(NULL == szBuf)
		return bGetFlag;
	memset(szBuf,0,(dwSize+1)*sizeof(TCHAR));
	if(strKeyName.IsEmpty())
		m_dwLastErr = CRegKey::QueryStringValue(NULL,szBuf,&dwSize);
	else
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

BOOL CWrlRegKey::SetRegStringVal(const CString& strKeyName,
								  const CString& strKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetStringValue(strKeyName,strKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::GetRegDwordVal(const CString& strKeyName,
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

BOOL CWrlRegKey::SetRegDwordVal(const CString& strKeyName,
								 DWORD dwKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetDWORDValue(strKeyName,dwKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CWrlRegKey::DelKeyName(const CString& strKeyName)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::DeleteValue(strKeyName);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}