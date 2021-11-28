/**
 * @file WRLRegKey.h
 *
 * Copyright (c) 2013-?,成都佐罗软件有限公司
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 注册表封装
 *
 *
 *
 * @version 
 *
 * @date 2020-07-13
 *
 * @author WZH
 *
 * @history 
 *
 */
// WRLRegKey.h : 注册表操作头文件定义
//

#pragma once

#include "WRLBaseDef.h"

/// 定义产品在注册表中的安装路径
#define PRODUCT_COMMAN_INSPATH		L"InsPath"
/// 定义产品在注册表中的主版本
#define PRODUCT_COMMAN_MAINVERNAME	L"MainVer"
/// 定义产品在注册表中的次版本
#define PRODUCT_COMMAN_MINIVERNAME	L"MiniVer"
/// 定义产品在注册表中的小版本
#define PRODUCT_COMMAN_SMALLVERNAME	L"SmallVer"
/// 定义产品在注册表中的编译版本
#define PRODUCT_COMMAN_BUILDVERNAME	L"BuildVer"
/// 定义使用的语言
#define PRODUCT_COMMAN_LANGUAGE		L"Language"
/// 定义产品在注册表中的安装语言
#define PRODUCT_COMMAN_INSLANG		L"InsLang"

class CWrlRegKey : public CRegKey
{
public:
	CWrlRegKey(const CString& strMainRegPath = COMPANY_REGPATH,\
		const CString& strChildName = PRODUCT_REGNODENAME, \
		HKEY hKey = HKEY_CURRENT_USER,REGSAM samDesired = KEY_ALL_ACCESS);

	virtual ~CWrlRegKey(void)
	{
		Close();
	}

	BOOL IsOpen()
	{
		return m_bOpenFlag;
	}

	BOOL NewRegPath(const CString& strMainRegPath = COMPANY_REGPATH,\
		const CString& strChildName = PRODUCT_REGNODENAME,\
		HKEY hKey = HKEY_CURRENT_USER,REGSAM samDesired = KEY_ALL_ACCESS);

	BOOL GetRegStringVal(const CString& strKeyName,CString& strKeyValue);

	BOOL SetRegStringVal(const CString& strKeyName,const CString& strKeyValue);

	BOOL GetRegDwordVal(const CString& strKeyName,DWORD& dwKeyValue);

	BOOL SetRegDwordVal(const CString& strKeyName,DWORD dwKeyValue);

	BOOL DelKeyName(const CString& strKeyName);

	DWORD GetLastErr()
	{
		return m_dwLastErr;
	}

	virtual void Close();

protected:

	DWORD	m_dwLastErr;

	BOOL	m_bOpenFlag;
};
