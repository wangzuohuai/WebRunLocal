#include "stdafx.h"
#include "BaseFuncLib.h"
#include "EngineHelper.h"
#include "WRLRegKey.h"

/// 获取中间件的数据文件存放路径，其它小程序的数据文件等也可以存放到此目录，中间件已确保此目录普通用户权限可写
/// 前端可通过Wrl_Version协议取到中间件的安装目录，然后再拼接这个Data子目录的路径，即可得到自己的文件全路径
CString GetAppDataPath(const CString& strExePath)
{
	CString strDataPath;
	DWORD dwFlag = KEY_READ;
#ifdef _WIN64
	/// 避免64位访问注册表错误
	dwFlag += KEY_WOW64_32KEY;
#endif
	CWrlRegKey Key(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,dwFlag);
	if(Key.IsOpen())
	{
		Key.GetRegStringVal(PRODUCT_COMMAN_INSPATH,strDataPath);
		Key.Close();
	}
	if(strDataPath.GetLength() < 4)
	{
		strDataPath = strExePath;
		int nFind = strDataPath.Find(L"\\Plugins\\");
		if(-1 != nFind)
			strDataPath = strDataPath.Left(nFind + 1);
		nFind = strDataPath.Find(L"\\plugins\\");
		if(-1 != nFind)
			strDataPath = strDataPath.Left(nFind + 1);
	}
	strDataPath += L"Data\\";
	return strDataPath;
}

CString GetComLoadPath(const CString& strExePath)
{
	CString strLoadDll;
	/// 先从注册表中获取
	DWORD dwFlag = KEY_READ;
#ifdef _WIN64
	/// 避免64位访问注册表错误
	dwFlag += KEY_WOW64_32KEY;
#endif
#ifndef _DEBUG
	CWrlRegKey Key(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,dwFlag);
	if(Key.IsOpen())
	{
		Key.GetRegStringVal(PRODUCT_COMMAN_INSPATH,strLoadDll);
		Key.Close();
	}
	if(strLoadDll.GetLength() < 4)
	{
		/// 再从小程序运行路径向上回溯路径
		strLoadDll = strExePath;
		int nFind = strLoadDll.Find(L"\\Plugins\\");
		if(-1 != nFind)
			strLoadDll = strLoadDll.Left(nFind + 1);
		nFind = strLoadDll.Find(L"\\plugins\\");
		if(-1 != nFind)
			strLoadDll = strLoadDll.Left(nFind + 1);
	}
#else
	strLoadDll = strExePath;
	int nFind = strLoadDll.Find(L"\\Plugins\\");
	if(-1 != nFind)
		strLoadDll = strLoadDll.Left(nFind + 1);
#endif
	if(strLoadDll.GetLength() && CBaseFuncLib::IsX64Exe(strExePath))
	{
		/// 64位版本运行
		if(-1 == strLoadDll.Find(L"\\X64\\") && -1 == strLoadDll.Find(L"\\x64\\"))
			strLoadDll += L"X64\\";
	}
	if(L'\\' != strLoadDll.Right(1))
	{
		int nFind = strLoadDll.ReverseFind(L'\\');
		if(-1 != nFind)
			strLoadDll = strLoadDll.Left(nFind + 1);
	}
	return strLoadDll;
}

CString CEngineHelper::m_strModelName = WRL_ENGINEDLL;

CString CEngineHelper::GetLoadPath()
{
	TCHAR szRunPath[MAX_PATH];
	::memset(szRunPath,0,MAX_PATH * sizeof(TCHAR));
	::GetModuleFileName(NULL,szRunPath,MAX_PATH);
	CString strLoadFile = GetComLoadPath(szRunPath) + CEngineHelper::m_strModelName;
	return strLoadFile;
}

void CEngineHelper::FreeDB(void)
{
	FreeAllHandle();
}

IJsonServicePtr CEngineHelper::GetJsonService()
{
	IJsonServicePtr spiJsonService = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spiJsonService);
		ATLASSERT(spiJsonService);
		return spiJsonService;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spiJsonService);
	if(NULL != hHandle)
	{
		SetMap(m_strModelName,hHandle);
	}
	strDllPath.Empty();
	ATLASSERT(spiJsonService);
	return spiJsonService;
}

ICxImgPtr CEngineHelper::GetCxImg()
{
	ICxImgPtr spiCxImg = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(CxImg),__uuidof(ICxImg),(VOID **)&spiCxImg);
		ATLASSERT(spiCxImg);
		return spiCxImg;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(CxImg),__uuidof(ICxImg),(VOID **)&spiCxImg);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiCxImg);
	return spiCxImg;
}

IOpenEncryPtr CEngineHelper::GetOpenEncry()
{
	IOpenEncryPtr spiOpenEncry = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(OpenEncry),__uuidof(IOpenEncry),(VOID **)&spiOpenEncry);
		ATLASSERT(spiOpenEncry);
		return spiOpenEncry;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(OpenEncry),__uuidof(IOpenEncry),(VOID **)&spiOpenEncry);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiOpenEncry);
	return spiOpenEncry;
}

IHttpVisitPtr CEngineHelper::GetHttpVisit()
{
	IHttpVisitPtr spiHttpVisit = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(HttpVisit),__uuidof(IHttpVisit),(VOID **)&spiHttpVisit);
		ATLASSERT(spiHttpVisit);
		return spiHttpVisit;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(HttpVisit),__uuidof(IHttpVisit),(VOID **)&spiHttpVisit);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiHttpVisit);
	return spiHttpVisit;
}

IConnectHelperPtr CEngineHelper::GetDBHelper()
{
	IConnectHelperPtr spiConnectHelper = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(ConnectHelper),__uuidof(IConnectHelper),(VOID **)&spiConnectHelper);
		ATLASSERT(spiConnectHelper);
		return spiConnectHelper;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(ConnectHelper),__uuidof(IConnectHelper),(VOID **)&spiConnectHelper);
	if(NULL != hHandle)
	{
		SetMap(m_strModelName,hHandle);
	}
	strDllPath.Empty();
	ATLASSERT(spiConnectHelper);
	return spiConnectHelper;
}

IWmiDevicePtr CEngineHelper::GetWmiDevice()
{
	IWmiDevicePtr spiWmiDevice = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(WmiDevice),__uuidof(IWmiDevice),(VOID **)&spiWmiDevice);
		ATLASSERT(spiWmiDevice);
		return spiWmiDevice;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(WmiDevice),__uuidof(IWmiDevice),(VOID **)&spiWmiDevice);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiWmiDevice);
	return spiWmiDevice;
}

IStreamDataPtr CEngineHelper::GetStreamData()
{
	IStreamDataPtr spiStreamData = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(StreamData),__uuidof(IStreamData),(VOID **)&spiStreamData);
		ATLASSERT(spiStreamData);
		return spiStreamData;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(StreamData),__uuidof(IStreamData),(VOID **)&spiStreamData);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiStreamData);
	return spiStreamData;
}