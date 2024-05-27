#pragma once
#include <shlobj.h>
#include "BaseFuncLib.h"

class CComHelper
{
protected:

	static CThreadDataLock	m_DataLock;

	static STRINGULONG_MAP	m_mapInstance;

	static HINSTANCE GetComHandle(const CString& strModelName);

	static void FreeHandle(const CString& strModelName);

	static void SetMap(const CString& strModelName,HINSTANCE& hModule);

	static void FreeAllHandle();
};