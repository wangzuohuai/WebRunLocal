/**
 * @file BaseFuncLib.h
 *
 * Copyright (c) 2013-?,成都佐罗软件有限公司
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 基础公共函数实现
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
// BaseFuncLib.h : 基础函数库头文件定义
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcomtime.h>
#include <atltypes.h>

/// 小程序自身配置文件
#ifndef WRL_SELFCONFIG
#define	WRL_SELFCONFIG			L"Config.json"
#endif

/// 下载临时文件扩展名
#define	WRL_DOWNFILE_TEMPNAME	L".tmp"

typedef enum EWrlOSType
{
OS_UNKNOWN					=	0,
OS_WINXP					=	510000,
OS_WINXP64					=	520000,

OS_WINVISTA					=	600000,

OS_WIN7						=	610000,
///SP1
OS_WIN7601					=	617601, 
OS_WIN7801					=	618001, 

OS_WIN8						=	620000,

OS_WIN81					=	630000,

OS_WIN10					=	1000000,
/// https://docs.microsoft.com/zh-cn/windows-server/get-started/windows-server-release-info
OS_WIN2019Server			=	1017763,

OS_WIN11					=	1022000,
OS_WIN12					=	1027000,
OS_WINADV					=	1100000,

} EOSType;

#define CREATE_MYTHREAD(func,lpParam,dwID) (::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,\
	(LPVOID)lpParam,0,(dwID) ? &dwID:0))

class CBaseFuncLib
{
public:
	CBaseFuncLib(void){};

	static int Us2ToChar(const CString& strSrc, char** pDst,int nCodePage = CP_ACP);

	static int CharToUS2(const char* szSrc, WCHAR** pDst,int nCodePage = CP_ACP);

	static BOOL IsX64Exe(const CString& strAppPath);

	static BYTE toHex(const BYTE &x);

	static CString NumToStr(LONGLONG i);

	static LONG StrToNum(const CString& strTemp);
	
	static ULONG StrToUlong(const CString& strTemp);

	static ULONGLONG GetTickTime();

	static CString GetShowSize(DWORD dwFileSize,int iPoint = 2);

	static CString GetSpecialFolderPath(long lFoldID,BOOL bWriteLog = TRUE);

	static CString GetTimestamp(COleDateTime GetTime);

	static CString PercentStringDecode(const CString& strSrc,BOOL bUtf8);

	static BOOL GetRGBFromString(CString strSource,COLORREF& rgbColor);

	static CString GetStringFromRGB(COLORREF& rgbColor);

	static COleDateTime GetFileWriteTime(const CString& strFilePath);

	static BOOL IsWin6432(HMODULE& hKernel);

	static WORD GetProcArchitecture();

	static BOOL GetWindowDisplayAffinity(HWND hWnd,DWORD& dwAffinity);

	static BOOL SetWindowDisplayAffinity(HWND hWnd,DWORD dwAffinity);

	static BOOL GetOSVerEx(LPOSVERSIONINFO lpVersionInformation);
	
	static EWrlOSType GetOsType();

	static BOOL IsWindowsServer();

	static BOOL FindProName(DWORD dwPID,CString& strFileName);

	static CString GetTmpPath(const CString& strChildDir = L"");
	
	static CString GetAppCfgPath();

	static BOOL IsPathExist(const CString& strPath) throw();	

	static BOOL CreatePath(const CString& strLocalDir);

	static void CBaseFuncLib::DelDirFile(const CString &strPath,const CString &strFind,\
		BOOL bDirFlag = TRUE,BOOL bWriteLog = TRUE);

	static DWORD ReadAllData(const CString& FileName,BYTE **ppData);

	static BOOL ProcRun(ULONG nPID);

	static BOOL TerminateProc(DWORD dwPID);
	
	static DWORD FindProc(const CString& strExeFile,DWORD &dwPID,DWORD dwOtherID = 0,\
		ULONG nGetFlag = 0,const CString& strFileName = L"");

	static CString GetErrInfo(DWORD dwErrCode, HINSTANCE hLib = NULL);

	static BOOL WriteToFile(const CString& strDataFile,BYTE *pData,DWORD nlen,BOOL bEndFlag = FALSE);
	
	static HANDLE WriteToFile2(const CString& strDataFile,LPCVOID lpBuffer,DWORD nlen,BOOL bEndFlag);

	static BOOL WriteLastLogToFile(DWORD nError,const CString &strModuleName = _T(""),\
		USHORT eLogType = 0);

	static BOOL WriteLogToFile(const CString& strLogInfo,const CString &strModuleName = _T(""),\
		USHORT eLogType = 1);

	static BOOL WriteLogToFile2(const CString& strLogFile,const CString& strLogInfo,\
		const CString &strModuleName = _T(""),USHORT eLogType = 1);

	static CString GetModulePath(HMODULE hModule = NULL);

	static CString GetModuleName(HMODULE hModule = NULL,BOOL bNameFlag = TRUE);

	static ULONG GetFileSize(const CString &strFilePath);

	static COleDateTime GetMsgTime(LONGLONG ulTotalSecond);

	static int Utf8ToUS2(LPCSTR pSrc, WCHAR** pDst);

	static int US2ToUtf8(const CString& strSrc,char** pDst);

	static int Utf8ToChar(LPCSTR pSrc, char** pDst);

	static char* UrlEncodeToChar(const CString& strPara);

	static CString UrlEncode(const CString& strPara,BOOL bFlag = TRUE);

	static void UrlEncodeToFile(const CString& strPara,HANDLE hFileHandle,BOOL bTryFlag =FALSE);
	
	static CString Utf8TOWChar(const char* pStr);

	static HINSTANCE CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv );

	static HRESULT CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv );

	static void OpenUrl(const CString& strUrl);

	static BOOL ReadClipboardString(CString& strCopy,BOOL bClearFlag);

	static BOOL CopyStringToClipboard(const CString& strCopy);

	static DWORD RunExe(LPWSTR szAppPath,WORD wShowFlag,LPWSTR szCommandLine = _T(""),\
		LPWSTR szDesktopName = _T(""),BOOL bWaitFlag = FALSE);
};

class CThreadDataLock
{
public:
 	CThreadDataLock() 
		: m_bLogFlag(FALSE)
	{
		memset(m_szPreName,0,MAX_PATH*sizeof(TCHAR));

		m_pSection = new CRITICAL_SECTION();
		if(NULL != m_pSection)
		{
			SYSTEM_INFO sysInfo;
			GetSystemInfo(&sysInfo);
			if(sysInfo.dwNumberOfProcessors > 1)
				InitializeCriticalSectionAndSpinCount(m_pSection,4000);
			else
				InitializeCriticalSection(m_pSection);
		}
	}

	~CThreadDataLock()
	{
		if(NULL != m_pSection)
		{
			DeleteCriticalSection(m_pSection);
			delete m_pSection;
			m_pSection = NULL;
		}
	}

	inline BOOL Lock(TCHAR* szFuncName = NULL);

	BOOL TryLock(TCHAR* szFuncName = NULL);

	inline void Unlock(TCHAR* szFuncName = NULL);

	inline void SetLog(BOOL bLogFlag)
	{
		m_bLogFlag = bLogFlag;
	}

protected:
	CThreadDataLock(const CThreadDataLock&);

	CThreadDataLock& operator=(const CThreadDataLock&);
 
	inline BOOL IsLock()
	{
		if(NULL == m_pSection || -1 == m_pSection->LockCount)
			return FALSE;
		return TRUE;
	}
 
	/// 是否启用日志 
	BOOL			m_bLogFlag;
	/// 上一个请求名称
	TCHAR			m_szPreName[MAX_PATH];
	/// 临界区
	CRITICAL_SECTION* m_pSection;
};