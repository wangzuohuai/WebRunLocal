#include "stdafx.h"
#include "BaseFuncLib.h"
#include "EngineHelper.h"
#include "BaseHelper.h"

extern CString GetComLoadPath(const CString& strExePath);

CString CBaseHelper::m_strModelName = WRL_BASEDLL;

CString CBaseHelper::GetLoadPath()
{
	TCHAR szRunPath[MAX_PATH];
	::memset(szRunPath,0,MAX_PATH * sizeof(TCHAR));
	::GetModuleFileName(NULL,szRunPath,MAX_PATH);
	CString strLoadFile = GetComLoadPath(szRunPath) + CBaseHelper::m_strModelName;
	return strLoadFile;
}

ISocketProxyPtr CBaseHelper::GetSocketProxy()
{
	ISocketProxyPtr spiSocketProxy = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface(hHandle,__uuidof(SocketProxy),__uuidof(ISocketProxy),(VOID **)&spiSocketProxy);
		ATLASSERT(spiSocketProxy);
		return spiSocketProxy;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance(strDllPath,__uuidof(SocketProxy),__uuidof(ISocketProxy),(VOID **)&spiSocketProxy);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiSocketProxy);
	return spiSocketProxy;
}

IFileToolPtr CBaseHelper::GetFileTool()
{
	IFileToolPtr spiFileTool = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface(hHandle,__uuidof(FileTool),__uuidof(IFileTool),(VOID **)&spiFileTool);
		ATLASSERT(spiFileTool);
		return spiFileTool;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance(strDllPath,__uuidof(FileTool),__uuidof(IFileTool),(VOID **)&spiFileTool);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiFileTool);
	return spiFileTool;
}

ISocketFunctionPtr CBaseHelper::GetWrlSocket()
{
	ISocketFunctionPtr spiSocketFunction = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(SocketFunction),__uuidof(ISocketFunction),(VOID **)&spiSocketFunction);
		ATLASSERT(spiSocketFunction);
		return spiSocketFunction;
	}
	CString strDllPath(GetLoadPath());
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(SocketFunction),__uuidof(ISocketFunction),(VOID **)&spiSocketFunction);
	if(NULL != hHandle)
		SetMap(m_strModelName,hHandle);
	strDllPath.Empty();
	ATLASSERT(spiSocketFunction);
	return spiSocketFunction;
}
