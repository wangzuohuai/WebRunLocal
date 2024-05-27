#include "StdAfx.h"
#include "ComHelper.h"
#include "EngineHelper.h"

extern HINSTANCE g_hInstance;

STRINGULONG_MAP CComHelper::m_mapInstance;

CThreadDataLock		CComHelper::m_DataLock;

HINSTANCE CComHelper::GetComHandle(const CString& strModelName)
{
	HINSTANCE hHandle = NULL;
	m_DataLock.Lock(L"GetComHandle");
	STRINGULONG_MAP::iterator it = m_mapInstance.find(strModelName);
	if(it != m_mapInstance.end())
		hHandle = (HINSTANCE )it->second;
	m_DataLock.Unlock(L"GetComHandle");
	return hHandle;
}

void CComHelper::SetMap(const CString& strModelName,HINSTANCE& hModule)
{
	m_DataLock.Lock(L"SetMap");
	m_mapInstance[strModelName] = (ULONG_PTR)hModule;
	m_DataLock.Unlock(L"SetMap");
}

void CComHelper::FreeHandle(const CString& strModelName)
{
	HINSTANCE hHandle = GetComHandle(strModelName);
	m_DataLock.Lock(L"FreeHandle");
	if(NULL != hHandle)
	{
		::FreeLibrary(hHandle);
		hHandle = NULL;
	}
	m_DataLock.Unlock(L"FreeHandle");
}

void CComHelper::FreeAllHandle()
{
	if(!m_mapInstance.size())
		return;
	m_DataLock.Lock(L"FreeAllHandle");
	STRINGULONG_MAP::iterator it = m_mapInstance.begin();
	while(it != m_mapInstance.end())
	{
		HINSTANCE hHandle = (HINSTANCE)it->second;
		if(NULL != hHandle)
		{
			::FreeLibrary(hHandle);
			hHandle = NULL;
		}
		it++;
	}
	m_mapInstance.clear();
	m_DataLock.Unlock(L"FreeAllHandle");
	::CoFreeUnusedLibraries();
}
