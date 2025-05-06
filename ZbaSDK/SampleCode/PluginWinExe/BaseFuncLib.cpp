// BaseFuncLib.cpp : 基础函数库实现

#include "stdafx.h"
#include <shlobj.h>
#include <shellapi.h>
#include <atltypes.h>
#include <WtsApi32.h>
#include <tlhelp32.h>
#include <string>
#include "WRLRegKey.h"
#include "WRLBaseDef.h"
#include "BaseFuncLib.h"
#include "BaseHelper.h"
#include "PathNtDosMapper.h"

#pragma warning( disable : 4819 )

#pragma comment(lib, "Version.lib")

#define NORMAL_SIZE				8192

#ifndef MAX_PATH_NEW
/// 长路径，系统默认260
#define MAX_PATH_NEW			1024
#endif

#ifndef WRL_PSAPIDLL
#define WRL_PSAPIDLL			L"Psapi.dll"
#endif

#define KeyNameInformation_		3

#define STATUS_BUFFER_TOO_SMALL ((LONG)0xC0000023L)

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE       (-6)
#endif

extern CString g_strLang;

typedef ULONGLONG ( __stdcall *lpGetTickCount64)(VOID);

typedef DWORD ( __stdcall *lpGetProcessImageFileName)(HANDLE hProcess,\
	LPWSTR lpFilename,DWORD nSize);

typedef void (__stdcall * LPWTSFreeMemory)(PVOID pMemory);

typedef BOOL (WINAPI *LPWTSSendMessage)(
	IN HANDLE hServer,IN DWORD SessionId,LPWSTR pTitle,
	IN DWORD TitleLength,LPWSTR pMessage,IN DWORD MessageLength,
	IN DWORD Style,IN DWORD Timeout,DWORD *pResponse,IN BOOL bWait);

typedef BOOL(__stdcall * LPWTSQuerySessionInformation)(HANDLE hServer, DWORD SessionId, WTS_INFO_CLASS WTSInfoClass, LPTSTR* ppBuffer, DWORD* pBytesReturned);

typedef DWORD ( __stdcall *lpGetProcessImageFileName)(HANDLE hProcess,LPWSTR lpFilename,DWORD nSize);

/// 全局模块句柄
HINSTANCE		g_hInstance = NULL;

HMODULE			g_hWtsLib = NULL;

static CWrlThreadLock	g_WriteLogLock;

/// 日志文件
CString	g_strLogFile = L"";

struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

//取软件版本号
CString GetFileInfo(const CString& strPathFile,const CString& strNode)
{
	CString strVersion;
	DWORD dwHandle = 0;
	DWORD InfoSize = GetFileVersionInfoSize(strPathFile,&dwHandle);
	if(!InfoSize)
		return strVersion;
	InfoSize++;
	TCHAR *InfoBuf = new TCHAR[InfoSize];
	if(NULL == InfoBuf)
		return strVersion;
	memset(InfoBuf,0,InfoSize*sizeof(TCHAR));
	GetFileVersionInfo(strPathFile,0,InfoSize - 1,InfoBuf);
	unsigned int cbTranslate = 0;
	BOOL bRet = VerQueryValue(InfoBuf,L"\\VarFileInfo\\Translation",(LPVOID*)&lpTranslate,&cbTranslate);
	unsigned int nSize = (cbTranslate/sizeof(struct LANGANDCODEPAGE));
	for(unsigned int i = 0;i < nSize;i++)
	{
		TCHAR SubBlock[MAX_PATH];
		wsprintf(SubBlock,L"\\StringFileInfo\\%04x%04x\\%s",\
			lpTranslate[i].wLanguage,lpTranslate[i].wCodePage,strNode);
		void *lpBuffer = NULL;
		unsigned int dwBytes = 0;
		bRet = VerQueryValue(InfoBuf,SubBlock,&lpBuffer,&dwBytes);
#ifdef _DEBUG
		if(!bRet)
		{
			CBaseFuncLib::WriteLastLogToFile(::GetLastError(),L"GetFileInfo");
		}
#endif
		if(dwBytes)
		{
			strVersion += (TCHAR*)lpBuffer;
			lpBuffer = NULL;
		}
		if(strVersion.GetLength())
			break;
	}
	delete []InfoBuf;
	InfoBuf = NULL;
	return strVersion;
}

CString GetFileVer(const CString& strPathFile)
{
	DWORD dummy = 0;
	CString strVersion;
    // 获取文件版本信息所需的缓冲区大小
    DWORD versionInfoSize = GetFileVersionInfoSize(strPathFile,&dummy);
    if(!versionInfoSize)
        return strVersion;
    // 分配缓冲区来存储版本信息
    std::vector<BYTE> versionInfo(versionInfoSize);
    if(!GetFileVersionInfo(strPathFile,0,versionInfoSize,versionInfo.data()))
       return strVersion;
    UINT translationSize = 0;
    VS_FIXEDFILEINFO* fixedInfo = NULL;
    UINT fixedInfoSize = sizeof(VS_FIXEDFILEINFO);

    // 获取翻译信息
    LPVOID translation = NULL;
    if(!VerQueryValueW(versionInfo.data(),L"\\VarFileInfo\\Translation",&translation,&translationSize))
        return strVersion;
    // 构建翻译字符串
    std::wstring translationString;
    translationString.resize(8);
    swprintf_s(&translationString[0],9,L"%04x%04x",LOWORD(*(DWORD*)translation),HIWORD(*(DWORD*)translation));
    // 获取固定文件信息
    if(!VerQueryValueW(versionInfo.data(),L"\\",(LPVOID*)&fixedInfo,&fixedInfoSize))
         return strVersion;
	strVersion.Format(L"%d.%d.%d.%d",HIWORD(fixedInfo->dwFileVersionMS),\
		LOWORD(fixedInfo->dwFileVersionMS),HIWORD(fixedInfo->dwFileVersionLS),LOWORD(fixedInfo->dwFileVersionLS));
	return strVersion;
}

/// 用户层显示提示信息 Wtsapi32.dll
DWORD UIShowMsgBox(const CString& strMessage,const CString& strTitle,DWORD dwStyle,DWORD dwWait)
{
	/// https://docs.microsoft.com/zh-cn/windows/desktop/api/wtsapi32/nf-wtsapi32-wtssendmessagea
    DWORD dwResponse = 0;/// dwReesp返回值IDABORT IDCANCEL IDCONTINUE IDIGNORE
	// IDNO IDOK IDRETRY IDTRYAGAIN IDYES IDASYNC IDTIMEOUT
	CString strSysPath = CBaseFuncLib::GetSpecialFolderPath(CSIDL_SYSTEM);
	HINSTANCE hModule = LoadLibrary(strSysPath + L"wtsapi32.dll");
	if(NULL == hModule)
	{
		/// XP系统，直接弹出窗口
		::MessageBox(NULL,strMessage,strTitle,MB_OK | MB_SERVICE_NOTIFICATION);
		return dwResponse;
	}
	/// VISTA才支持
	LPWTSSendMessage lpWtsSendMessage = (LPWTSSendMessage)GetProcAddress(hModule,"WTSSendMessageW");
	if(NULL != lpWtsSendMessage)
	{
		IFileToolPtr spiFileTool = CBaseHelper::GetFileTool();
		ULONG dwSessionID = 0;
		if(NULL != spiFileTool)
		{
			spiFileTool->get_SessionID((ULONG)hModule,&dwSessionID);
			spiFileTool = NULL;
		}
		lpWtsSendMessage(WTS_CURRENT_SERVER_HANDLE,dwSessionID,(LPTSTR)(LPCTSTR)strTitle,
			 2*strTitle.GetLength(),(LPTSTR)(LPCTSTR)strMessage, 
			 2*strMessage.GetLength(),dwStyle,dwWait,&dwResponse,TRUE);
		lpWtsSendMessage = NULL;
	}
	else
	{
		/// XP系统，直接弹出窗口
		::MessageBox(NULL,strMessage,strTitle,MB_OK | MB_SERVICE_NOTIFICATION);
	}
	FreeLibrary(hModule);
	hModule = NULL;
	return dwResponse;
}

BOOL CBaseFuncLib::IsX64Exe(const CString& strAppPath)
{
	BOOL bX64 = FALSE;
	HANDLE hFileOpen = ::CreateFile(strAppPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
		return bX64;
	DWORD m = 0;
	BYTE szBuff[512];
	memset(szBuff,0,512);
	::ReadFile(hFileOpen,szBuff,512,&m,NULL);
	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	CString strBuff;
	for(int n = 0;n < 512;n++)
	{
		if('\0' != szBuff[n])
			strBuff += szBuff[n];
	}
	int nFind = strBuff.Find(L"PE");
	if(-1 != nFind)
		strBuff.Delete(0,nFind+2);
	if(strBuff.GetAt(0) == L'L')
		return bX64;
	bX64 = TRUE;
	return bX64;
}

BOOL CBaseFuncLib::IsWin6432(HMODULE& hKernel)
{
	BOOL b6432Flag = FALSE;
	if(NULL == hKernel)
		hKernel	= GetModuleHandle(WRL_SYKERNELDLL);
	if(NULL == hKernel)
		hKernel = ::LoadLibrary(WRL_SYKERNELDLL);
	if(NULL != hKernel)
	{
		lpIsWow64Process IsWow64Process = NULL;
		IsWow64Process = (lpIsWow64Process)GetProcAddress(hKernel,"IsWow64Process"); 	
		if(NULL != IsWow64Process)
		{
			HANDLE hProcHandle = ::GetCurrentProcess();
			IsWow64Process(hProcHandle,&b6432Flag);
			::CloseHandle(hProcHandle);
			hProcHandle = NULL;
			IsWow64Process = NULL;
		}
	}
	return b6432Flag;
}

WORD CBaseFuncLib::GetProcArchitecture()
{
	WORD wProcArchitecture = 0;
	BOOL b64Flag = FALSE,bFreeFlag = FALSE;
	HMODULE	hKernel	= ::GetModuleHandle(WRL_SYKERNELDLL);
	if(NULL == hKernel)
	{
		hKernel	= ::LoadLibrary(WRL_SYKERNELDLL);
		if(NULL == hKernel)
			return wProcArchitecture;
		b64Flag = TRUE;
	}
	lpFN_GNSI lpGetNativeSystemInfo = NULL;
	lpGetNativeSystemInfo = (lpFN_GNSI)GetProcAddress(\
		hKernel,"GetNativeSystemInfo"); 
	SYSTEM_INFO si;
	memset(&si,0,sizeof(SYSTEM_INFO));
	if(NULL != lpGetNativeSystemInfo)
	{
		lpGetNativeSystemInfo(&si);
		lpGetNativeSystemInfo = NULL;
	}
	wProcArchitecture = si.wProcessorArchitecture;
	if(b64Flag)
		::FreeLibrary(hKernel);
	hKernel = NULL;
	return wProcArchitecture;
}

BOOL CBaseFuncLib::GetWindowDisplayAffinity(HWND hWnd,DWORD& dwAffinity)
{
	dwAffinity = 0;
	BOOL bFreeFlag = FALSE,bGetFlag = FALSE;
	if(GetOsType() < OS_WIN7 || NULL == hWnd || !::IsWindow(hWnd))
		return bGetFlag;
	HMODULE hUserDll = ::GetModuleHandle(WRL_USERDLL); /// 获取dll的句柄
	if(NULL == hUserDll)
	{
		bFreeFlag = TRUE;
		hUserDll = ::LoadLibrary(WRL_USERDLL);
	}
    LPGetWindowDisplayAffinity lpGetWindowDisplayAffinity = NULL;
    if(hUserDll)
    {
        lpGetWindowDisplayAffinity = (LPGetWindowDisplayAffinity)GetProcAddress(hUserDll,"GetWindowDisplayAffinity");
    }
    if(lpGetWindowDisplayAffinity)
    {
		bGetFlag = lpGetWindowDisplayAffinity(hWnd,&dwAffinity);
		lpGetWindowDisplayAffinity = NULL;
		if(bFreeFlag)
			::FreeLibrary(hUserDll);
        return bGetFlag;
    }
    return bGetFlag;
}

BOOL CBaseFuncLib::SetWindowDisplayAffinity(HWND hWnd,DWORD dwAffinity)
{
	BOOL bFreeFlag = FALSE,bSetFlag = FALSE;
	if(GetOsType() < OS_WIN7)
		return bSetFlag;
	HMODULE hUserDll = ::GetModuleHandle(WRL_USERDLL); /// 获取dll的句柄
	if(NULL == hUserDll)
	{
		bFreeFlag = TRUE;
		hUserDll = ::LoadLibrary(WRL_USERDLL);
	}
    LPSetWindowDisplayAffinity lpSetWindowDisplayAffinity = NULL;
    if(hUserDll)
    {
        lpSetWindowDisplayAffinity = (LPSetWindowDisplayAffinity)GetProcAddress(hUserDll,"SetWindowDisplayAffinity");
    }
    if(lpSetWindowDisplayAffinity)
    {
		bSetFlag = lpSetWindowDisplayAffinity(hWnd,dwAffinity);
		lpSetWindowDisplayAffinity = NULL;
		if(bFreeFlag)
			::FreeLibrary(hUserDll);
        return bSetFlag;
    }
    return bSetFlag;
}

BOOL CBaseFuncLib::GetOSVerEx(LPOSVERSIONINFO lpVersionInformation)
{
	BOOL bFreeFlag = FALSE;
	HMODULE hNtDll = ::GetModuleHandle(WRL_SYNTDLL); /// 获取ntdll.dll的句柄
	if(NULL == hNtDll)
	{
		bFreeFlag = TRUE;
		hNtDll = ::LoadLibrary(WRL_SYNTDLL);
	}
    lpGetVersion RtlGetVersion = NULL;
    if(hNtDll)
    {
        RtlGetVersion = (lpGetVersion)GetProcAddress(hNtDll, "RtlGetVersion"); /// 获取RtlGetVersion地址
   }
    if(RtlGetVersion)
    {
		BOOL bRet = RtlGetVersion((POSVERSIONINFO)lpVersionInformation) >= 0; /// 调用RtlGetVersion
		if(bFreeFlag)
			::FreeLibrary(hNtDll);
		RtlGetVersion = NULL;
 		hNtDll = NULL;
        return bRet;
    }
 	hNtDll = NULL;
    return FALSE;
}

BOOL CBaseFuncLib::IsWindowsServer()
{
    OSVERSIONINFOEXW osvi = { sizeof(osvi),0,0,0,0,{0},0,0,0,VER_NT_WORKSTATION };
    DWORDLONG const dwlConditionMask = VerSetConditionMask(0,VER_PRODUCT_TYPE,VER_EQUAL);
    return !VerifyVersionInfo(&osvi,VER_PRODUCT_TYPE,dwlConditionMask);
}

EWrlOSType CBaseFuncLib::GetOsType()
{
	EWrlOSType eType = OS_WINXP;
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);		
	GetOSVerEx((OSVERSIONINFO *)&osvi);
	if(osvi.dwMajorVersion == 5)
	{
		if(osvi.dwMinorVersion == 1)
			eType = (EWrlOSType)(OS_WINXP + osvi.dwBuildNumber);
		else if(osvi.dwMinorVersion >= 2)
			eType = (EWrlOSType)(OS_WINXP64 + osvi.dwBuildNumber);
	}
 	else if(osvi.dwMajorVersion == 6)
	{
		if(osvi.dwMinorVersion == 0)
		{
			eType = (EWrlOSType)(OS_WINVISTA + osvi.dwBuildNumber);
		}
		else if(osvi.dwMinorVersion == 1)
		{
			eType = (EWrlOSType)(OS_WIN7 + osvi.dwBuildNumber);
		}
		else if(osvi.dwMinorVersion == 2)
		{
			eType = (EWrlOSType)(OS_WIN8 + osvi.dwBuildNumber);
 		}
		else if(osvi.dwMinorVersion >= 3)
		{
			eType = (EWrlOSType)(OS_WIN81 + osvi.dwBuildNumber);
		}
	}
 	else if(osvi.dwMajorVersion >= 10)
	{
		if(osvi.dwMinorVersion == 0)
		{
			if(osvi.dwBuildNumber < 22000)
				eType = (EWrlOSType)(OS_WIN10 + osvi.dwBuildNumber);
			else
			{
				eType = (EWrlOSType)(OS_WIN10 + osvi.dwBuildNumber);
				//eType = OS_WIN11;/// WIN 11 and WIN 12
			}
		}
		else if(osvi.dwMinorVersion == 1)
		{
			eType = (EWrlOSType)(OS_WINADV + osvi.dwBuildNumber);
		}
		else
		{
			WriteLogToFile(NumToStr(osvi.dwMinorVersion) + L" MinorVersion");
			//WriteLogToFile(NumToStr(osvi.dwBuildNumber) + L" BuildNumber");
			eType = (EWrlOSType)(OS_WINADV + osvi.dwBuildNumber);
		}
	}
	return eType;
}

ULONGLONG CBaseFuncLib::GetTickTime()
{
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetOSVerEx((OSVERSIONINFO*)&osvi);
	ULONGLONG ullTickCount = GetTickCount();
	if(osvi.dwMajorVersion >= 6)
	{
		/// 判断如果是高版本，取64位版本来使用
		HINSTANCE hModule = ::GetModuleHandle(WRL_SYKERNELDLL);
		if(NULL == hModule)
			hModule = ::LoadLibrary(WRL_SYKERNELDLL);
		if(NULL != hModule)
		{
			lpGetTickCount64 lpGetTickCount = (lpGetTickCount64)GetProcAddress(hModule, "GetTickCount64");
			if(lpGetTickCount)
			{
				ullTickCount = lpGetTickCount();
				lpGetTickCount = NULL;
			}
			::FreeLibrary(hModule);
			hModule = NULL;
		}
	}
	return ullTickCount;
}

CString CBaseFuncLib::GetTimestamp(COleDateTime GetTime)
{
	CString strTimeStamp;
	TIME_ZONE_INFORMATION tzi;
    GetTimeZoneInformation(&tzi);
	int zone = - tzi.Bias/60;
	COleDateTime beginTime(1970,1,1,zone,0,0);
	COleDateTimeSpan spanTime = GetTime - beginTime;
	strTimeStamp = NumToStr((LONGLONG)spanTime.GetTotalSeconds());
	return strTimeStamp;
}

int CBaseFuncLib::Utf8ToChar(LPCSTR pSrc, char** pDst)
{
	WCHAR*  strW=NULL;
	int iLength=0;
    iLength=::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,NULL,0);
	if(!iLength)
		return iLength;
	iLength++;
    strW = new WCHAR[iLength];
	if(strW==NULL)
		return 0;
 	::memset(strW,0,iLength*sizeof(WCHAR));
   ::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,strW,iLength-1);
    iLength=::WideCharToMultiByte(CP_ACP,0,strW,-1,NULL,0,NULL,NULL);
 	if(!iLength)
	{
		delete []strW;
		strW=NULL;
		return 0;
	}
	iLength++;
    char *strA = NULL;
	strA = new char[iLength];
	if(strA!=NULL)
	{
	 	::memset(strA,0,iLength*sizeof(char));
		iLength = ::WideCharToMultiByte(CP_ACP,0,strW,-1,strA,iLength-1,NULL,NULL);
		*pDst=strA;
    }
	else
		iLength = 0;
	delete []strW;
	strW=NULL;
	return iLength;
}

CString CBaseFuncLib::PercentStringDecode(const CString& strSrc,BOOL bUtf8)
{
	char *szBuf = NULL;
	char cTem[NORMAL_SIZE] = {};
	int nLen = strSrc.GetLength(),nC = 0;
	for(int nIndex = 0;nIndex < nLen;)
	{
		CString strVal(_T(""));
		WCHAR wc = strSrc.GetAt(nIndex);
		if(wc != _T('%'))
		{
			cTem[nC] = (char)wc;
			nC++;
			nIndex++;
			continue;
		}
		nIndex++;
		if(nIndex < nLen)
		{
			strVal += strSrc.GetAt(nIndex);
			nIndex++;
		}
		if(nIndex < nLen)
		{
			strVal += strSrc.GetAt(nIndex);
			nIndex++;
		}
		Us2ToChar(strVal,&szBuf);
		cTem[nC] = (char )strtoul(szBuf, NULL, 16);
		nC++;
		delete []szBuf;
		szBuf = NULL;
		if(nC >= NORMAL_SIZE)
			break;
	}
	if(bUtf8)
	{
		Utf8ToChar(cTem,&szBuf);
		CString strReturn(szBuf);
		delete []szBuf;
		szBuf = NULL;
		return strReturn;
	}
	else
	{
		CString strReturn(cTem);
		return strReturn;
	}
}

BOOL CBaseFuncLib::GetFileWriteTime(const CString& strFilePath,COleDateTime &WriteTime)
{
	WIN32_FIND_DATA fData;
	HANDLE hFind = FindFirstFile(strFilePath,&fData);
	if(hFind != INVALID_HANDLE_VALUE) 
	{
		FindClose(hFind);
		SYSTEMTIME sysTime;
		::FileTimeToSystemTime(&(fData.ftLastWriteTime),&sysTime);
		WriteTime = sysTime;
		TIME_ZONE_INFORMATION   tzi;  
		GetTimeZoneInformation(&tzi);  
		int zone = - tzi.Bias/60; /// 时区，如果是中国标准时间则得到8
		COleDateTimeSpan spanTime(0,zone,0,0);
		WriteTime += spanTime;
		return TRUE;
	}
	return FALSE;
}

int CBaseFuncLib::Us2ToChar(const CString& strSrc, char** pDst,int nCodePage)
{
	int iLength = ::WideCharToMultiByte(nCodePage,0,strSrc,-1,NULL,0,NULL,NULL);
 	if(!iLength)
		return iLength;
	iLength++;
    char *strA = NULL;
	strA = new char[iLength];
	if(strA == NULL)
		return 0;
	::memset(strA,0,iLength*sizeof(char));
	int iLength1 = 0;
	if(nCodePage)
		iLength1 = ::WideCharToMultiByte(nCodePage,WC_COMPOSITECHECK,strSrc,-1,strA,iLength-1,NULL,NULL);
	else
		iLength1 = ::WideCharToMultiByte(nCodePage,0,strSrc,-1,strA,iLength-1,NULL,NULL);
	*pDst = strA;
	return iLength1;
}

int CBaseFuncLib::CharToUS2(const char* szSrc, WCHAR** pDst,int nCodePage)
{
	WCHAR* strW = NULL;
	int iLength=0;
	
	iLength = MultiByteToWideChar(nCodePage,0,szSrc,-1,NULL,0);
	if(!iLength)
		return iLength;
	iLength++;
	strW = new WCHAR[iLength];
	if(strW == NULL)
		return 0;
	::memset(strW,0,iLength*sizeof(WCHAR));
	int iLength1 = 0;
	if(nCodePage)
		iLength1 = ::MultiByteToWideChar(nCodePage,MB_COMPOSITE,szSrc,-1,strW,iLength-1);
	else
		iLength1 = ::MultiByteToWideChar(nCodePage,0,szSrc,-1,strW,iLength-1);
	*pDst=strW;
	return iLength1;
}

BYTE CBaseFuncLib::toHex(const BYTE &x)
{
   return x > 9 ? x + 55: x + 48;
}

CString CBaseFuncLib::NumToStr(LONGLONG i)
{
	CString strTemp(_T(""));	
	if (i == 0)
	{
		strTemp = _T("0");
		return strTemp;
	}
	LONGLONG dwNumber = i;
	if(i < 0)
		dwNumber = -i;
	while(dwNumber)
	{
		TCHAR Temp = (TCHAR)(dwNumber % 10 + '0');
		strTemp.Insert(0,Temp);
		dwNumber = dwNumber / 10;
	}
	if(i < 0)
		strTemp.Insert(0,L"-");
	return strTemp;
}

LONG CBaseFuncLib::StrToNum(const CString& strTemp)
{
	LONG nValue = 0;
	if(strTemp.IsEmpty())
		return nValue;
	BOOL bLoss = FALSE;
	if(strTemp.GetAt(0) == '-')
		bLoss = TRUE;
	int j = 0;
	if(bLoss)
		j++;
	for (;j < strTemp.GetLength();j++)
	{
		TCHAR c = strTemp[j];
		if('.' == c)
		{
			if(j + 1 < strTemp.GetLength())
			{
				c = strTemp[j+1];
				if (c <= '9' && c >= '5')
					nValue++;/// 四舍五入
			}
			break;
		}
		if(c <= '9' && c >= '0')
		{
			nValue *= 10;
			nValue = (nValue + (c - '0')) ; 
		}
	}
	if(bLoss)
		return -nValue;
	return nValue;
}

ULONG CBaseFuncLib::StrToUlong(const CString& strTemp)
{
	ULONG nValue = 0;
	if(strTemp.IsEmpty())
		return nValue;
	int j = 0;
	if(strTemp.GetAt(0) == '-')
		j = 1;
	for(;j < strTemp.GetLength();j++)
	{
		TCHAR c = strTemp[j];
		if('.' == c)
			break;
		if (c <= '9' && c >= '0')
		{
			nValue *= 10;
			nValue = (nValue + (c - '0')) ; 
		}
	}
	return nValue;
}

CString CBaseFuncLib::GetStringFromRGB(COLORREF& rgbColor)
{
	CString strColor;
	BYTE Red	= GetRValue(rgbColor);	///得到红颜色 
	BYTE Green	= GetGValue(rgbColor);	///得到绿颜色 
	BYTE Blue	= GetBValue(rgbColor);	///得到兰颜色
	strColor.Format(L"#%02X%02X%02X",Red,Green,Blue);
	return strColor;
}

BOOL CBaseFuncLib::GetRGBFromString(CString strSource,COLORREF& rgbColor)
{
    rgbColor = 0;
	int iLength = strSource.GetLength();
	if (iLength < 7)
        return FALSE;
    BYTE Red = 0,Green = 0,Blue = 0;
	CString strColor(strSource),strHex;

	if(0 == strColor.Left(1).CompareNoCase(L"\""))
		strColor.Delete(0);
	if(L'#' != strColor.GetAt(0))
		return FALSE;
	strColor.Delete(0);
	strHex = strColor.Left(2);
	strColor.Delete(0,2);
    Red = (BYTE)_tcstoul(strHex,NULL,16);
	strHex = strColor.Left(2);
	strColor.Delete(0,2);
    Green = (BYTE)_tcstoul(strHex,NULL,16);
	strHex = strColor.Left(2);
    Blue = (BYTE)_tcstoul(strHex,NULL,16);

    rgbColor = RGB(Red,Green,Blue);
    return TRUE;
}

CString CBaseFuncLib::GetShowSize(DWORD dwFileSize,int iPoint)
{
	CString strShowSize;
	if(dwFileSize<KILO_DIGEST_LENGTH)
	{
		strShowSize.Format(L"%dB",dwFileSize);
		return strShowSize;
	}
	int iKSize=(int )dwFileSize/KILO_DIGEST_LENGTH;
	if(iKSize<KILO_DIGEST_LENGTH)
	{
		int iTem = dwFileSize%KILO_DIGEST_LENGTH;
		if(3 == iPoint)
		{
			iTem = (int)(1000*iTem/KILO_DIGEST_LENGTH);
			strShowSize.Format(L"%d.%03d",iKSize,iTem);
		}
		else if(2 == iPoint)
		{
			iTem = (int)(100*iTem/KILO_DIGEST_LENGTH);
			strShowSize.Format(L"%d.%02d",iKSize,iTem);
		}
		else if(1 == iPoint)
		{
			iTem = (int)(10*iTem/KILO_DIGEST_LENGTH);
			strShowSize.Format(L"%d.%d",iKSize,iTem);
		}
		else
			strShowSize.Format(L"%d",iKSize);
		strShowSize += L"K";
		return strShowSize;
	}
	int iMSize=(int )dwFileSize/(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH);
	int iTem = dwFileSize%(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH);
	if(3 == iPoint)
	{
		iTem = (int)(1000*iTem/(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH));
		strShowSize.Format(L"%d.%03d",iMSize,iTem);
	}
	else if(2 == iPoint)
	{
		iTem = (int)(100*iTem/(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH));
		strShowSize.Format(L"%d.%02d",iMSize,iTem);
	}
	else if(1 == iPoint)
	{
		iTem = (int)(10*iTem/(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH));
		strShowSize.Format(L"%d.%d",iMSize,iTem);
	}
	else
		strShowSize.Format(L"%d",iMSize);
	strShowSize += L"M";
	return strShowSize;
}

ULONG CBaseFuncLib::GetFileSize(const CString &strFilePath)
{
	WIN32_FIND_DATA	data;
	ULONG dwFileSize = 0;	
	HANDLE hFindFile = ::FindFirstFile(strFilePath,&data);
	if(INVALID_HANDLE_VALUE != hFindFile && NULL != hFindFile)
	{
		dwFileSize = data.nFileSizeLow + data.nFileSizeHigh*MAXDWORD + data.nFileSizeHigh;
		::FindClose(hFindFile);
		hFindFile = NULL;
	}
	return dwFileSize;
}

COleDateTime CBaseFuncLib::GetMsgTime(LONGLONG ulTotalSecond)
{
	TIME_ZONE_INFORMATION   tzi;  
    GetTimeZoneInformation(&tzi);  
	int zone = - tzi.Bias/60; /// 时区，如果是中国标准时间则得到8

	COleDateTime MsgTime(1970,1,1,zone,0,0);

	LONGLONG nTotalSecond = ulTotalSecond;
	LONG nDays = 0,nHours = 0,nMinutes = 0;
	nDays = (LONG)(nTotalSecond/(60*60*24));
	nTotalSecond -= nDays*60*60*24;
	nHours = (LONG)(nTotalSecond/(3600));
	nTotalSecond -= nHours*3600;
	nMinutes = (LONG)(nTotalSecond/60);
	nTotalSecond -= nMinutes*60;
	COleDateTimeSpan spanTime(nDays,(int)nHours,(int)nMinutes,(int)nTotalSecond);
	return (MsgTime + spanTime);
}

BOOL CBaseFuncLib::IsSessionLocked()
{
    LONG dwFlags = 0;
    BOOL bRet = FALSE;
    WTSINFOEXW * pInfo = NULL;
    LPTSTR ppBuffer = NULL;
    DWORD dwBytesReturned = 0;
    LPWTSFreeMemory pWTSFreeMemory = NULL;
    WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
    LPWTSQuerySessionInformation pWTSQuerySessionInformation = NULL;
	if(NULL == g_hWtsLib)
		g_hWtsLib = LoadLibrary(L"wtsapi32.dll");
	if(NULL == g_hWtsLib)
        return FALSE;
    pWTSQuerySessionInformation = (LPWTSQuerySessionInformation)GetProcAddress(g_hWtsLib,"WTSQuerySessionInformationW");
    if(NULL != pWTSQuerySessionInformation)
    {
        pWTSFreeMemory = (LPWTSFreeMemory)GetProcAddress(g_hWtsLib,"WTSFreeMemory");
        if(pWTSFreeMemory != NULL)
        {
            DWORD dwSessionID = WTSGetActiveConsoleSessionId();
            if(pWTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,dwSessionID,wtsic,&ppBuffer,&dwBytesReturned))
            {
                if(dwBytesReturned > 0)
                {
                    pInfo = (WTSINFOEXW*)ppBuffer;
                    if(pInfo->Level == 1)
                    {
                        dwFlags = pInfo->Data.WTSInfoExLevel1.SessionFlags;
                    }
                    if(dwFlags == WTS_SESSIONSTATE_LOCK)
                        bRet = TRUE;
                }
                pWTSFreeMemory(ppBuffer);
                ppBuffer = NULL;
            }
        }
    }
    return bRet;
}

BOOL CBaseFuncLib::FindProName(DWORD dwPID,CString& strFileName)
{
	BOOL bFindFlag = FALSE;
	/// 在Windows 10 2019年版本中可能存在问题，调用耗费200毫秒之多，CPU飙升
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);
	DWORD dwGetProcessID = dwPID;
	if(!dwGetProcessID)
		dwGetProcessID = ::GetCurrentProcessId();
	for(;fok ;fok = Process32Next(hSnapshot,&pe))
	{
		if(pe.th32ProcessID == dwGetProcessID)
		{
			strFileName = _wcsupr(pe.szExeFile);
			strFileName.MakeLower();
			bFindFlag = TRUE;
			break;
		}
	}	
	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return bFindFlag;
}

CString CBaseFuncLib::GetAppCfgPath()
{
	CString strCfgPath = CBaseFuncLib::GetTmpPath(L"Temp");
	return strCfgPath;
}

CString CBaseFuncLib::GetTmpPath(const CString& strChildDir)
{
	CString strTempPath;
	DWORD dwFlag = KEY_READ;
#ifdef _WIN64
	/// 避免64位访问注册表错误
	dwFlag += KEY_WOW64_32KEY;
#endif
	CWrlRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,dwFlag);
	regKey.GetRegStringVal(PRODUCT_COMMAN_APPRUNFILE,strTempPath);
	ATLASSERT(strTempPath.GetLength());
	int nFind = strTempPath.ReverseFind(L'\\');
	strTempPath = strTempPath.Left(nFind + 1);
	if(strChildDir.GetLength())
	{
		strTempPath += strChildDir;
		::DeleteFile(strTempPath);
		CreatePath(strTempPath);
		strTempPath += L"\\";
	}
	else
	{
		if(L'\\' != strTempPath.Right(1))
			strTempPath += L"\\";
	}
	return strTempPath;
}

USHORT CBaseFuncLib::IsPathExist(const CString& strPath)
{
	USHORT nRet = 0;
	if(strPath.IsEmpty())
		return nRet;
	BOOL bFindDir = FALSE;
	CString strFind(strPath);
	if(0 == strPath.Right(1).CompareNoCase(L"\\"))
	{
		/// 查找目录
		bFindDir = TRUE;
		strFind += L"*.*";
	}
	WIN32_FIND_DATA	data;
	HANDLE hFindFile = ::FindFirstFile(strFind,&data);
	if(INVALID_HANDLE_VALUE != hFindFile && NULL != hFindFile)
	{
		if(bFindDir)
		{
			while(INVALID_HANDLE_VALUE != hFindFile)
			{
				if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{				
					nRet = 1;
					break;
				}
				if(!FindNextFile(hFindFile,&data))
					break;
			}
		}
		else
		{
			nRet = 1;
			if(!data.nFileSizeHigh && !data.nFileSizeLow)
				nRet = 2;
		}
		::FindClose(hFindFile);
		hFindFile = NULL;
	}
	else
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			/// 目前不支持长路径260 https://learn.microsoft.com/zh-cn/windows/win32/fileio/maximum-file-path-limitation?tabs=registry
			if(ERROR_FILE_NOT_FOUND != dwErrCode && ERROR_PATH_NOT_FOUND != dwErrCode)
			{
				CBaseFuncLib::WriteLastLogToFile(dwErrCode,strFind);
				nRet = 3;
			}
		}
	}
	return nRet;
}

CString CBaseFuncLib::GetUrlFileName(const CString &strUrl)
{
	CString strNameFile;
	int nFind = strUrl.ReverseFind(L'/');
	ATLASSERT(-1 != nFind);
	if(-1 == nFind)
	{
		nFind = strUrl.ReverseFind(L'\\');
		if(-1 != nFind)
			strNameFile = strUrl.Right(strUrl.GetLength() - nFind -1);
		else
			strNameFile = strUrl;
		return strNameFile;
	}
	strNameFile = strUrl.Right(strUrl.GetLength() - nFind -1);
	nFind = strNameFile.Find(L'.');
	int nNewFind = strNameFile.Find(L"?");
	if(nNewFind > nFind && -1 != nFind)
		strNameFile = strNameFile.Left(nNewFind);
	else
		strNameFile.Delete(0,nNewFind + 1);
	nFind = strNameFile.ReverseFind(L'=');
	if(-1 != nFind && (strNameFile.GetLength() - nFind) > 4)
		strNameFile.Delete(0,nFind + 1);
	strNameFile.Replace(L"/",L"");
	strNameFile.Replace(L"\\",L"");
	return strNameFile;
}

CString CBaseFuncLib::GetSpecialFolderPath(long lFoldID,BOOL bWriteLog)
{
	/// 此函数需要初始化COM环境
	CString strSpecialPath;
	TCHAR szSpecialPath[MAX_PATH];
	::memset(szSpecialPath,0,MAX_PATH*sizeof(TCHAR));
	HRESULT hRet = ::SHGetFolderPath(NULL,lFoldID,NULL,SHGFP_TYPE_DEFAULT,szSpecialPath);
	strSpecialPath = szSpecialPath;
	if(strSpecialPath.GetLength() < 3)
		return strSpecialPath;
	strSpecialPath = szSpecialPath;
	///路径后面都有"\"
	if(!strSpecialPath.IsEmpty() && 0 != strSpecialPath.Right(1).CompareNoCase(L"\\"))
		strSpecialPath += L"\\";
	return strSpecialPath;
}

BOOL CBaseFuncLib::CreatePath(const CString& strLocalDir)
{
	BOOL bCreateFlag = FALSE;
	CString strWorkDir,strPath = strLocalDir;
	int index = -2;
	index = strPath.Find(L"\\\\");
	if(0 == index)
		strPath.Delete(0,2);/// 局域网路径
	index = strPath.Find(L'\\');
	if(index == -1)
		return FALSE;
	strWorkDir = strPath.Left(index);
	strPath.Delete(0,index+1);
	while(strPath.GetLength())
	{
		index=strPath.Find(L'\\');
		if(index == -1)
		{
			bCreateFlag = ::CreateDirectory(strLocalDir,NULL);
			strWorkDir.Empty();
			strPath.Empty();
			break;
		}
		strWorkDir += L"\\";
		strWorkDir+=strPath.Left(index);
		strPath.Delete(0,index + 1);
		if(!IsPathExist(strWorkDir + L"\\"))
		{
			bCreateFlag = ::CreateDirectory(strWorkDir,NULL);
		}
	}
	if(!bCreateFlag)
	{
		DWORD dwErrCode = ::GetLastError();
		::Sleep(10);
	}
	return bCreateFlag;
}

struct handle_data 
{
	HWND	hWnd;
	HWND	hExistWnd;
	USHORT	nFlag;
	unsigned long hIns;
	unsigned long nPID;
};
 
BOOL IsMainWindow(HWND handle,USHORT nFlag,const CString& strClassName)
{   
	if(NULL == handle)
		return FALSE;
	CRect rcWnd;
	::GetWindowRect(handle,&rcWnd);
	if(rcWnd.Width() < 6 && rcWnd.Height() < 6)
		return FALSE;/// 大小不合适
	HWND hOwnerWnd = NULL;
	BOOL bShowFlag = TRUE;
	LONG_PTR nStyle = ::GetWindowLongPtr(handle,GWL_STYLE);
	if(2 != (2 & nFlag))
	{
		bShowFlag = (nStyle & WS_VISIBLE) != 0;
		if(bShowFlag)
		{
			if(-1 != strClassName.Find(L"hidden"))
				bShowFlag = FALSE;
		}
		//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(bShowFlag) + L" ShowFlag",\
		//	CBaseFuncLib::NumToStr(nFlag) + L" Flag");
	}
	if(bShowFlag)
	{
		hOwnerWnd = GetWindow(handle,GW_OWNER);
		if(NULL != hOwnerWnd)
		{
			if(WS_SYSMENU == (nStyle & WS_SYSMENU))
				hOwnerWnd = NULL;
		}
	}
	//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hOwnerWnd) + L" OwnerWnd",strClassName);
	return bShowFlag && (hOwnerWnd == (HWND)0);
}

int GetMonitorLeft(HWND hGetWnd)
{
	int nMonitorLeft = 0;
	if(::GetSystemMetrics(SM_CMONITORS) > 1)
	{
		/// 处理外接监视器显示
		HMONITOR hMonitor = NULL;
		if(NULL != hGetWnd && ::IsWindow(hGetWnd))
			hMonitor = ::MonitorFromWindow(hGetWnd,MONITOR_DEFAULTTOPRIMARY);
		if(NULL != hMonitor)
		{
			MONITORINFO mi = {};
			mi.cbSize = sizeof(MONITORINFO);
			if(NULL != hMonitor && ::GetMonitorInfo(hMonitor,&mi))
			{
				nMonitorLeft = mi.rcMonitor.left;
			}
			hMonitor = NULL;
		}
	}
	return nMonitorLeft;
}

BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
	ATLASSERT(NULL != hWnd);
	unsigned long nPID = 0;
	handle_data& data = *(handle_data*)lParam;
	if(data.hExistWnd == hWnd)
		return TRUE;
	CString strName;
	if(data.nPID)
	{
		DWORD nRet = GetWindowThreadProcessId(hWnd,&nPID);
		if(data.nPID != nPID)
			return TRUE;
		TCHAR tcName[MAX_PATH];
		::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
		GetClassName(hWnd,tcName,MAX_PATH);
		strName = tcName;
		if(-1 != strName.Find(L"CSpNotify ") || -1 != strName.Find(L"SoPY_") || -1 != strName.Find(L"ComboLBox")
			 || -1 != strName.Find(L"Sogou_") || -1 != strName.Find(L"tooltips_class") || -1 != strName.Find(L"SplashWindowClass")
			|| -1 != strName.Find(L"IME") || -1 != strName.Find(L"HardwareMonitorWindow"))
			return TRUE;/// 特殊处理
		//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindWnd",strName);
		if(1 == (1 & data.nFlag))
		{
			if(0 != strName.CompareNoCase(L"MozillaWindowClass"))
				return TRUE;/// Firefox浏览器特殊处理
		}
		if(4 == (4 & data.nFlag))
		{
			if(0 == strName.CompareNoCase(L"ClipBoardWindow")
				|| 0 == strName.CompareNoCase(L"SunAwtToolkit")
				|| -1 != strName.Find(L"_Status"))
				return TRUE;/// 特殊处理
		}
		if(8 == (8 & data.nFlag))
		{
			if(0 != strName.CompareNoCase(L"OpusApp"))
				return TRUE;/// 微软Office中的Word特殊处理
			//::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
			//::GetWindowText(hWnd,tcName,MAX_PATH);
			//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindName",tcName);
		}
		if(16 == (16 & data.nFlag))
		{
			if(0 != strName.CompareNoCase(L"XLMAIN") && -1 == strName.Find(L"FrameClass"))
				return TRUE;/// 微软Office中的Excel及PPT特殊处理
			//::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
			//::GetWindowText(hWnd,tcName,MAX_PATH);
			//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindName",tcName);
		}
		if(32 == (32 & data.nFlag))
		{
			if(0 == strName.CompareNoCase(L"Edit"))
				return TRUE;/// Adobe Reader
			//::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
			//::GetWindowText(hWnd,tcName,MAX_PATH);
			//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindName",tcName);
		}
		if(512 == (512 & data.nFlag))
		{
			if(0 != strName.CompareNoCase(L"CASCADIA_HOSTING_WINDOW_CLASS")
				&& 0 != strName.CompareNoCase(L"ConsoleWindowClass"))
				return TRUE;/// 命令行窗口句柄
			//::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
			//::GetWindowText(hWnd,tcName,MAX_PATH);
			//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindName",tcName);
		}
	}
	else
	{
		if(data.hIns)
		{
			LONG_PTR nRet = GetWindowLongPtr(hWnd,GWL_HINSTANCE);
			if(nRet != data.hIns)
				return TRUE;
		}
	}
	if(!IsMainWindow(hWnd,data.nFlag,strName))
	{
#ifdef _DEBUG
		CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" NotMainClassWnd",strName);
#endif
		return TRUE;
	}
	data.hWnd = hWnd;
	if(128 == (128 & data.nFlag))
	{
		/// 需要显示到副屏，查询副屏幕的窗口
		int nLeft = GetMonitorLeft(hWnd);
		if(nLeft)
			return FALSE;
		return TRUE;
	}
#ifdef _DEBUG
	TCHAR tcName[MAX_PATH];
	::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
	::GetWindowText(hWnd,tcName,MAX_PATH);
	CBaseFuncLib::WriteLogToFile(strName + L" WndClass",CString(tcName) + L" WndowText");
	CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr((ULONG)hWnd) + L" FindMainWnd");
#endif
	return FALSE;
}

HWND FindMainWindow(unsigned long nPID,USHORT nFlag,HDESK hDesk,HWND hExistWnd)
{
	handle_data data;
	data.nPID = nPID;
	data.nFlag = nFlag;
	data.hExistWnd = hExistWnd;
	data.hWnd = 0;
	data.hIns = 0;
	/// 获取所有顶层可见窗口及他们的子窗口 在系统服务中调用获取不到所有窗口
	if(NULL == hDesk)
		EnumWindows(EnumWindowsCallback,(LPARAM)&data);
	else
		EnumDesktopWindows(hDesk,EnumWindowsCallback,(LPARAM)&data);
	return data.hWnd;
}

lpGetModuleFileNameEx CBaseFuncLib::GetMyModuleFileName(HINSTANCE &hPsModule)
{
	/// WIN 2000不支持
	lpGetModuleFileNameEx pGetModuleFileNameEx = NULL;
	if(NULL == hPsModule)
		hPsModule = ::LoadLibrary(WRL_PSAPIDLL);
	if(NULL != hPsModule)
		pGetModuleFileNameEx = (lpGetModuleFileNameEx)GetProcAddress(hPsModule,"GetModuleFileNameExW");
	if(NULL != pGetModuleFileNameEx)
		return pGetModuleFileNameEx;
	HINSTANCE hModule = ::GetModuleHandle(WRL_SYKERNELDLL);
	if(NULL == hModule)
	{
		hModule = ::LoadLibrary(WRL_SYKERNELDLL);
		if(NULL != hModule)
			hPsModule = hModule;
	}
	if(NULL != hModule)
	{
		pGetModuleFileNameEx = (lpGetModuleFileNameEx)GetProcAddress(hModule,"GetModuleFileNameExW");
		hModule = NULL;
	}
	return pGetModuleFileNameEx;
}

void CBaseFuncLib::FindMapProc(const CString& strExeFile,ULONGSTRING_MAP &mapInfo)
{
	if(strExeFile.IsEmpty())
		return;
	HANDLE hSnapshot = NULL;
	/// 在Windows 10 2019年版本中可能存在问题，调用耗费200毫秒之多，CPU飙升
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);
	if(NULL == hSnapshot)
		return;
	CString strExePath,strExeName;
	int nFind = strExeFile.ReverseFind(L'\\');
	if(-1 != nFind)
	{
		strExePath = strExeFile.Left(nFind+1);
		strExeName = strExeFile.Right(strExeFile.GetLength()-nFind-1);
	}
	else
		strExeName = strExeFile;

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);

	BOOL bGetFlag = FALSE;
	LONGNUMBER_MAP mapApp;
	HINSTANCE hPsModule = NULL;
	lpGetModuleFileNameEx pGetModuleFileNameEx = NULL;
	for(;fok ;fok = Process32Next(hSnapshot,&pe))
	{
		if(0 == strExeName.CompareNoCase(pe.szExeFile))
		{
			//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(pe.th32ProcessID) + L" PID",strExePath);
			/// 可能存在多个执行文件名称是一样的，需要加程序路径来区分
			if(strExePath.GetLength())
			{
				/// 判断具体的运行路径
				HMODULE hProcess = (HMODULE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,\
					FALSE,pe.th32ProcessID);
				if(NULL != hProcess && INVALID_HANDLE_VALUE != hProcess)
				{
					DWORD dwRet = 0;
					TCHAR szPath[MAX_PATH];
					memset(szPath,0,MAX_PATH*sizeof(TCHAR));
					if(!bGetFlag && NULL == pGetModuleFileNameEx)
					{
						pGetModuleFileNameEx = GetMyModuleFileName(hPsModule);
						bGetFlag = TRUE;
					}
					if(NULL != pGetModuleFileNameEx)
					{
						dwRet = pGetModuleFileNameEx(hProcess,NULL,szPath,MAX_PATH_NEW);
						if(dwRet)
						{
							CString strGetPath(szPath);
							if(0 != strExeFile.CompareNoCase(strGetPath))
								strGetPath.Replace(L"\\SysWOW64\\",L"\\");/// 32位下获取可能是错误的
							//CBaseFuncLib::WriteLogToFile(strExeFile + L" ExeFile",strGetPath);
							if(0 == strExeFile.CompareNoCase(strGetPath))
							{
								/// 必须是同路径
								mapInfo[pe.th32ProcessID] = pe.szExeFile;
							}
						}
						else
						{
							/// Windows 7 64位下32位程序打开64位进程会报无效句柄错误
							DWORD dwErrCode = ::GetLastError();
							if(ERROR_ACCESS_DENIED == dwErrCode || ERROR_INVALID_HANDLE == dwErrCode)
								mapInfo[pe.th32ProcessID] = pe.szExeFile;
							else
								CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(dwErrCode) + L" GetModuleFileNameEx");
						}
					}
					else
					{
						DWORD dwErrCode = ::GetLastError();
						CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(dwErrCode) + L" GetMyModuleFileName");
					}
					::CloseHandle(hProcess);
					hProcess = NULL;
				}
				else
				{
					DWORD dwErrCode = ::GetLastError();
					if(ERROR_ACCESS_DENIED == dwErrCode)
						mapInfo[pe.th32ProcessID] = pe.szExeFile;
					else
						CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(dwErrCode) + L" FindProcOpenProc",strExePath);
				}
			}
			else
			{
				mapInfo[pe.th32ProcessID] = pe.szExeFile;
			}
		}
	}
	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	if(NULL != hPsModule)
	{
		::FreeLibrary(hPsModule);
		hPsModule = NULL;
	}
	mapApp.clear();
}

DWORD CBaseFuncLib::FindProc(const CString& strExeFile,DWORD &dwPID,\
	DWORD dwOtherID,ULONG nGetFlag,const CString& strFileName)
{
	dwPID = 0;
	DWORD dwThread = 0;
	if(strExeFile.IsEmpty())
		return dwThread;
	HANDLE hSnapshot = NULL;
	/// 在Windows 10 2019年版本中可能存在问题，调用耗费200毫秒之多，CPU飙升
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);
	if(NULL == hSnapshot)
		return dwThread;
	CString strExePath,strExeName;
	int nFind = strExeFile.ReverseFind(L'\\');
	if(-1 != nFind)
	{
		strExePath = strExeFile.Left(nFind+1);
		strExeName = strExeFile.Right(strExeFile.GetLength()-nFind-1);
	}
	else
		strExeName = strExeFile;

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);

	BOOL bGetFlag = FALSE;
	LONGNUMBER_MAP mapApp;
	HINSTANCE hPsModule = NULL;
	lpGetModuleFileNameEx pGetModuleFileNameEx = NULL;
	for(;fok ;fok = Process32Next(hSnapshot,&pe))
	{
		if(0 == strExeName.CompareNoCase(pe.szExeFile) && dwOtherID != pe.th32ProcessID)
		{
			/// 取主窗口
			if(1 == (1 & nGetFlag))
			{
				SHORT nHideFlag = 0;
				if(4 == (4 & nGetFlag))
					nHideFlag = 2;/// 支持隐藏程序
				if(8 == (8 & nGetFlag))
					nHideFlag += 8;
				HWND hFindWnd = FindMainWindow(pe.th32ProcessID,nHideFlag,NULL,NULL);
				if(NULL == hFindWnd)
				{
					//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(pe.th32ProcessID) + L" ProcessID NULL",strFileName);
					continue;
				}
				/// 取窗口类名称判断去掉无关进程ID
				if(2 == (2 & nGetFlag))
				{
					TCHAR tcName[MAX_PATH];
					::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
					GetClassName(hFindWnd,tcName,MAX_PATH);
					CString strName(tcName);
					if(0 == strName.CompareNoCase(L"Alternate Owner"))
						continue;/// IE特殊处理
				}
				//CBaseFuncLib::WriteLogToFile(CBaseFuncLib::NumToStr(pe.th32ProcessID) + L" ProcessID",strFileName);
				if(strFileName.GetLength())
				{
					TCHAR tcName[MAX_PATH];
					::memset(tcName,0,MAX_PATH*sizeof(TCHAR));
					::GetWindowText(hFindWnd,tcName,MAX_PATH);
					CString strName(tcName);
					if(strName.IsEmpty())
					{
						continue;
					}
					//CBaseFuncLib::WriteLogToFile(strName + L" WindowText",strFileName);
					if(8 == (8 & nGetFlag))
					{
						if(strName.GetLength())
						{
							if(-1 == strName.Find(strFileName))
							{
								int nFind = strFileName.ReverseFind(L'.');
								if(-1 != nFind && -1 == strName.ReverseFind(L'.'))
								{
									CString strTempName = strFileName.Left(nFind);/// 部分电脑不显示扩展名
									if(-1 == strName.Find(strTempName))
										continue;/// 文件名不匹配
								}
								else
									continue;/// 文件名不匹配
							}
						}
					}
					else
					{
						if(-1 == strName.Find(strFileName))
							continue;/// 文件名不匹配
					}
				}
			}
			/// 可能存在多个执行文件名称是一样的，需要加程序路径来区分
			if(strExePath.GetLength())
			{
				/// 判断具体的运行路径
				HMODULE hProcess = (HMODULE)OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pe.th32ProcessID);
				if(NULL != hProcess)
				{
					DWORD dwRet = 0;
					TCHAR szPath[MAX_PATH];
					memset(szPath,0,MAX_PATH*sizeof(TCHAR));
					/// WIN 2000不支持
					if(!bGetFlag && NULL == pGetModuleFileNameEx)
					{
						pGetModuleFileNameEx = GetMyModuleFileName(hPsModule);
						bGetFlag = TRUE;
					}
					if(NULL != pGetModuleFileNameEx)
					{
						dwRet = pGetModuleFileNameEx(hProcess,NULL,szPath,MAX_PATH_NEW);
						if(dwRet)
						{
							CString strGetPath(szPath);
							if(0 != strExeFile.CompareNoCase(strGetPath))
								strGetPath.Replace(L"\\SysWOW64\\",L"\\");/// 32位下获取可能是错误的
							//CBaseFuncLib::WriteLogToFile(strExeFile + L" ExeFile",strGetPath);
							if(0 == strExeFile.CompareNoCase(strGetPath))
							{
								/// 必须是同路径
								mapApp[pe.th32ProcessID] = pe.cntThreads;
							}
						}
						else
						{
							if(NULL != hPsModule)
							{
								lpGetProcessImageFileName pGetProcessImageFileName = NULL;
								pGetProcessImageFileName = (lpGetProcessImageFileName)GetProcAddress(hPsModule,"GetProcessImageFileNameW");
								if(NULL != pGetProcessImageFileName)
								{
									dwRet = pGetProcessImageFileName(hProcess,szPath,MAX_PATH_NEW);
									if(dwRet)
									{
										CString strFullPath(szPath);
										CPathNtDosMapper PathMap;
										int nFind = strFullPath.Find(_T('.'));
										if(-1 != nFind)
										{
											int nFind1 = strFullPath.ReverseFind(_T('.'));
											if(nFind1 != nFind)
												strFullPath = strFullPath.Left(nFind1+4);
											else
												strFullPath = strFullPath.Left(nFind+4);
										}
										CString strWindowPath = CBaseFuncLib::GetSpecialFolderPath(CSIDL_WINDOWS,FALSE);
										if(-1 != strFullPath.Find(_T("\\SystemRoot\\")))
											strFullPath.Replace(_T("\\SystemRoot\\"),strWindowPath);
										strFullPath = PathMap.NtToDosEx(strFullPath);
										if(0 == strFullPath.Left(1).CompareNoCase(_T("\\")))
											strFullPath.Insert(0,strWindowPath.Left(2));
										if(1 != CBaseFuncLib::IsPathExist(strFullPath))
										{
											nFind = strFullPath.Find(_T("\\??\\"));
											if(-1 != nFind)
												strFullPath.Delete(0,nFind+4);
											nFind = strFullPath.Find(_T('.'));
											if(-1 != nFind)
												strFullPath = strFullPath.Left(nFind+4);
										}
										if(0 == strExeFile.CompareNoCase(strFullPath))
										{
											/// 必须是同路径
											mapApp[pe.th32ProcessID] = pe.cntThreads;
										}
									}
								}
							}
						}
					}
					::CloseHandle(hProcess);
					hProcess = NULL;
				}
				else
				{
					//WriteLastLogToFile(::GetLastError(),L"FindProcOpenProcess");
					DWORD dwErrCode = ::GetLastError();
					if(ERROR_ACCESS_DENIED == dwErrCode || ERROR_INVALID_HANDLE == dwErrCode)
						mapApp[pe.th32ProcessID] = pe.cntThreads;// 无效句柄及无权限访问的也算上
				}
			}
			else
			{
				mapApp[pe.th32ProcessID] = pe.cntThreads;
			}
		}
	}
	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	if(NULL != hPsModule)
	{
		::FreeLibrary(hPsModule);
		hPsModule = NULL;
	}
	/// 比较线程大小，取最大的那个
	LONGNUMBER_MAP::iterator it = mapApp.begin();
	while(it != mapApp.end())
	{
		if(dwThread < it->second)
		{
			dwPID = (DWORD )(it->first);
			dwThread = (DWORD )(it->second);
		}
		it++;
	}
	mapApp.clear();
	return dwThread;
}

BOOL CBaseFuncLib::ProcRun(ULONG nPID)
{
	BOOL bRetFlag = FALSE;
	HANDLE hProc = NULL;
	hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,nPID);
	if(NULL == hProc)
		return bRetFlag;
	/// 判断此进程ID是否为指定的执行程序
	HANDLE hSnapshot = NULL;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);
	if(NULL == hSnapshot)
		return bRetFlag;
	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);
	for(;fok ;fok = Process32Next(hSnapshot,&pe))
	{
		if(pe.th32ProcessID == nPID)
		{
			bRetFlag = TRUE;
			break;
		}
	}		
	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return bRetFlag;
}

BOOL CBaseFuncLib::TerminateProc(DWORD dwPID)
{
	HANDLE hProc = NULL;
	hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,dwPID);
	if(NULL == hProc)
	{
		DWORD dwErrCode = ::GetLastError();
		if(ERROR_INVALID_PARAMETER != dwErrCode && ERROR_ACCESS_DENIED != dwErrCode)
		{
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("杀进程 %ld 时获取句柄失败，错误码 %ld"),dwPID,dwErrCode);
			WriteLogToFile(strLogInfo);
		}
		return FALSE;
	}
	BOOL bRetFlag = ::TerminateProcess(hProc,1);
	if(!bRetFlag)
	{
		DWORD dwErrCode = ::GetLastError();
		if(ERROR_ACCESS_DENIED != dwErrCode)
		{
			CString strLogInfo(_T(""));
			if(0 == g_strLang.CompareNoCase(L"ENG"))
				strLogInfo.Format(_T("强制杀进程 %ld 失败，错误码 %ld"),dwPID,dwErrCode);
			else
				strLogInfo.Format(_T("强制杀进程 %ld 失败，错误码 %ld"),dwPID,dwErrCode);
			WriteLogToFile(strLogInfo);
		}
	}
	::CloseHandle(hProc);
	hProc = NULL;
	return bRetFlag;
}

CString CBaseFuncLib::GetErrInfo(DWORD dwErrCode, HINSTANCE hLib)
{
	CString strErrInfo;
	LPVOID lpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		hLib, dwErrCode, 0, // Default language
		(LPTSTR)&lpMsgBuf, 0, NULL);
	if(NULL != lpMsgBuf)
	{
		strErrInfo = (LPTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);
		lpMsgBuf = NULL;
	}
	/// \r\n换行替换掉，避免加到JSON中解析失败
	strErrInfo.Replace(L"\r\n",L"");
	return strErrInfo;
}

BOOL CBaseFuncLib::WriteLastLogToFile(DWORD nError,const CString &strModuleName,USHORT eLogType)
{
	if(!nError)
		return FALSE;
	BOOL bWriteFlag = WriteLogToFile(NumToStr(nError) + CString(L":") + GetErrInfo(nError),\
		strModuleName,eLogType);
	return bWriteFlag;
}

CString GetLogFile2(USHORT nNodeID = 0)
{
	/// 日志文件保存路径
	CString strLogFile = CBaseFuncLib::GetTmpPath(L"Temp");
	if(strLogFile.GetAllocLength() < 5)
	{
		TCHAR szPath[MAX_PATH];
		::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
		::GetTempPath(MAX_PATH,szPath);
		strLogFile = szPath;
	}
	if(0 != strLogFile.Right(1).CompareNoCase(_T("\\")))
		strLogFile += L"\\";
	CString strRunFile = CBaseFuncLib::GetModuleName(NULL,FALSE);
	int iFind = strRunFile.ReverseFind(L'\\');
	if(-1 != iFind)
		strRunFile = strRunFile.Right(strRunFile.GetLength()-iFind-1);
	if(-1 != iFind)
	{
		iFind = strRunFile.ReverseFind(L'.');
		strLogFile += strRunFile.Left(iFind);
	}
	else
		strLogFile += WRL_APPLICATION_NAME;
	if(nNodeID)
	{
		strLogFile += L"_";
		strLogFile += CBaseFuncLib::NumToStr(nNodeID);
	}
	strLogFile += L".txt";
	return strLogFile;
}

CString GetLogFile(USHORT nNodeID = 0)
{
	if (g_strLogFile.GetLength())
		return g_strLogFile;
	g_strLogFile = GetLogFile2(nNodeID);
	return g_strLogFile;
}

DWORD CBaseFuncLib::WriteLogToFile2(const CString& strLogFile,\
		const CString& strLogInfo,const CString &strModuleName,USHORT eLogType)
{
	if(strLogFile.IsEmpty() || strLogInfo.IsEmpty())
		return -1;
	g_WriteLogLock.Lock();
	DWORD dwFileSize = 5*KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH;/// 5MB
	if(GetFileSize(strLogFile) > dwFileSize)
	{
		unsigned _int64 nFreeBytesAvailable = 0;
		ULARGE_INTEGER nTotalNumberOfBytes;
		ULARGE_INTEGER nTotalNumberOfFreeBytes;
		BOOL bResult = ::GetDiskFreeSpaceEx(strLogFile.Left(1),(PULARGE_INTEGER)&nTotalNumberOfBytes,\
			(PULARGE_INTEGER)&nTotalNumberOfFreeBytes,(PULARGE_INTEGER)&nFreeBytesAvailable);
		if(bResult)
		{
			/// 判断磁盘空间，小于100MB时自动清理
			if(nFreeBytesAvailable/(KILO_DIGEST_LENGTH*KILO_DIGEST_LENGTH) < 100)
			{
				int nFind = strLogFile.ReverseFind(L'\\');
				DelDirFile(strLogFile.Left(nFind + 1),L".bak",FALSE,FALSE);
			}
		}
		int nIndex = 1;
		CString strMoreLog(_T(""));
		strMoreLog.Format(_T("%s%d.bak"),strLogFile,nIndex);
		while(IsPathExist(strMoreLog))
		{
			strMoreLog.Empty();
			nIndex++;
			strMoreLog.Format(_T("%s%d.bak"),strLogFile,nIndex);
		}
		if(nIndex > 20)
		{
			/// 避免太多日志产生
			for(int n = 1;n <= 15;n++)
			{
				nIndex = n;
				strMoreLog.Format(_T("%s%d.bak"),strLogFile,nIndex);
				::DeleteFile(strMoreLog);
				strMoreLog.Empty();
			}
			nIndex = 1;
			strMoreLog.Format(_T("%s%d.bak"),strLogFile,nIndex);
		}
		::MoveFile(strLogFile,strMoreLog);/// 备份日志输出
	}
	g_WriteLogLock.UnLock();

	CString strModule(strModuleName);
	if(strModule.IsEmpty())
		strModule = GetModuleName((HMODULE)g_hInstance,TRUE);

	CString strLog(_T(""));
	DWORD dwThreadID = ::GetCurrentThreadId();
	COleDateTime curTime = COleDateTime::GetCurrentTime();
	strLog.Format(_T("*%s*%02d-%02d %02d:%02d:%02d Tick:%s TID:%ld PID:%ld %s"),\
		strModule,curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),\
		curTime.GetSecond(),NumToStr(GetTickTime()),dwThreadID,::GetCurrentProcessId(),strLogInfo);
	strModule.Empty();

	if(0 != strLog.Right(2).CompareNoCase(WRL_STRING_LRETURN))
		strLog += WRL_STRING_LRETURN;
	char *szLog = NULL;
	int nLen = US2ToUtf8(strLog,&szLog) - 1;
	if(NULL == szLog)
	{
		strLog.Empty();
		return -2;
	}
	DWORD nWriteFlag = WriteToFile(strLogFile,(BYTE *)szLog,nLen,TRUE);
	delete []szLog;
	szLog = NULL;
	strLog.Empty();
	return nWriteFlag;
}

BOOL CBaseFuncLib::WriteLogToFile(const CString& strLogInfo,
		const CString &strModuleName,USHORT eLogType)
{
	if(strLogInfo.IsEmpty())
		return FALSE;
	if(g_strLogFile.IsEmpty())
		g_strLogFile = GetLogFile();
	return WriteLogToFile2(g_strLogFile,strLogInfo,strModuleName,eLogType);
}

void CBaseFuncLib::DelDirFile(const CString &strPath,\
	const CString &strFind,BOOL bDirFlag,BOOL bWriteLog)
{
	if(strPath.GetLength() < 3)
		return;/// 避免删除错误
	CString strFindPath(strPath);
	if(0 != strFindPath.Right(1).CompareNoCase(L"\\"))
		strFindPath += L"\\";
	CString strProcDir = GetSpecialFolderPath(CSIDL_WINDOWS,bWriteLog);
	if(0 == strProcDir.CompareNoCase(strPath))
		return;
	strProcDir.Empty();
	strProcDir = GetSpecialFolderPath(CSIDL_SYSTEM,bWriteLog);
	if(0 == strProcDir.CompareNoCase(strPath))
		return;
	strProcDir.Empty();
	strProcDir = GetSpecialFolderPath(CSIDL_PROGRAM_FILES,bWriteLog);
	if(0 == strProcDir.CompareNoCase(strPath))
		return;
	strProcDir.Empty();
	strProcDir = GetSpecialFolderPath(CSIDL_PROGRAM_FILES_COMMON,bWriteLog);
	if(0 == strProcDir.CompareNoCase(strPath))
		return;
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strFindPath + strFind,&fd);
	while(hFind != INVALID_HANDLE_VALUE && NULL != hFind)
	{
		if(FILE_ATTRIBUTE_DIRECTORY != (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString strFile;
			strFile.Format(L"%s%s",strFindPath,fd.cFileName);
			::SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);
			::DeleteFile(strFile);
			strFile.Empty();
		}
		else
		{
			CString strFileName(fd.cFileName);
			if(0 != strFileName.CompareNoCase(L".")
				&& 0 != strFileName.CompareNoCase(L".."))
				DelDirFile(strFindPath + fd.cFileName,strFind,bDirFlag,bWriteLog);
		}
		if(!FindNextFile(hFind,&fd))
			break;
	}
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		hFind = NULL;
	}
	if(bDirFlag)
		::RemoveDirectory(strPath);
}

HANDLE CBaseFuncLib::WriteToFile2(const CString& strDataFile,LPCVOID lpBuffer,DWORD nlen,BOOL bEndFlag)
{
	if(NULL == lpBuffer || !nlen)
		return NULL;
	HANDLE hFileOpen = ::CreateFile(strDataFile,GENERIC_WRITE,FILE_SHARE_WRITE,\
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	DWORD dwWriteLen = 0;
	if(!bEndFlag)
	{
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_BEGIN);
		::SetEndOfFile(hFileOpen);
	}
	else
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_END);
	::WriteFile(hFileOpen,lpBuffer,nlen,&dwWriteLen,NULL);
	ATLASSERT(dwWriteLen == nlen);
	return hFileOpen;
}

DWORD CBaseFuncLib::WriteToFile(const CString& strDataFile,BYTE *pData,DWORD nlen,BOOL bEndFlag)
{
	BOOL bLockFlag = FALSE;
	if(0 == g_strLogFile.CompareNoCase(g_strLogFile))
		bLockFlag = TRUE;
	if(bLockFlag)
		g_WriteLogLock.Lock();
	HANDLE hFileOpen = NULL;
	hFileOpen = ::CreateFile(strDataFile,GENERIC_WRITE,FILE_SHARE_READ,\
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastErr = ::GetLastError();
		int nIndex = 5;
		/// 不是拒绝访问
		while(ERROR_ACCESS_DENIED != dwLastErr && ERROR_INVALID_ACCESS != dwLastErr
			&& nIndex > 0 && hFileOpen == INVALID_HANDLE_VALUE)
		{
			nIndex--;
			::Sleep(100);
			/// 重新尝试打开
			hFileOpen = ::CreateFile(strDataFile,GENERIC_WRITE,FILE_SHARE_WRITE,\
				NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		}
		if(hFileOpen == INVALID_HANDLE_VALUE)
		{
			if(bLockFlag && ERROR_ACCESS_DENIED == dwLastErr)
			{
				/// 拒绝访问，写入临时目录
				int nFind = strDataFile.ReverseFind(L'\\');
				CString strFile;
				TCHAR szPath[MAX_PATH];
				memset(szPath,0,MAX_PATH*sizeof(TCHAR));
				::GetTempPath(MAX_PATH,szPath);
				CString strWritePath = szPath;
				if(0 != strWritePath.Right(1).CompareNoCase(_T("\\")))
					strWritePath += _T("\\");
				strWritePath += PRODUCT_REGNODENAME;
				::DeleteFile(strWritePath);
				CreatePath(strWritePath);
				strWritePath += _T("\\");
				strWritePath += strDataFile.Right(strDataFile.GetLength()-nFind-1);
				hFileOpen = ::CreateFile(strWritePath,GENERIC_WRITE,FILE_SHARE_READ,\
					NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				if(hFileOpen != INVALID_HANDLE_VALUE)
				{
					if(!bEndFlag)
					{
						::SetFilePointer(hFileOpen,NULL,NULL,FILE_BEGIN);
						::SetEndOfFile(hFileOpen);
					}
					else
						::SetFilePointer(hFileOpen,NULL,NULL,FILE_END);
					DWORD dwWriteLen = 0;
					::WriteFile(hFileOpen,pData,nlen,&dwWriteLen,NULL);
					::CloseHandle(hFileOpen);
					hFileOpen = NULL;
				}
			}
			if(bLockFlag)
				g_WriteLogLock.UnLock();
			return dwLastErr;
		}
	}
	if(!bEndFlag)
	{
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_BEGIN);
		::SetEndOfFile(hFileOpen);
	}
	else
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_END);
	DWORD dwLastErr = ::GetLastError();
	DWORD dwWriteLen = 0;
	BOOL bWriteFlag = ::WriteFile(hFileOpen,pData,nlen,&dwWriteLen,NULL);
	if(!bWriteFlag)
	{
		dwLastErr = ::GetLastError();
		if(ERROR_DISK_FULL == dwLastErr)
		{
			/// 磁盘空间不足，删除临时文件
			int nFind = strDataFile.ReverseFind(L'\\');
			DelDirFile(strDataFile.Left(nFind + 1),L"*.bak",FALSE,FALSE);
			bWriteFlag = ::WriteFile(hFileOpen,pData,nlen,&dwWriteLen,NULL);
		}
	}
	if(!bWriteFlag)
		dwLastErr = ::GetLastError();
	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	if(bLockFlag)
		g_WriteLogLock.UnLock();
	return dwLastErr;
}

DWORD CBaseFuncLib::ReadAllData(const CString& FileName,BYTE **ppData)
{
	DWORD m = 0;
	ATLASSERT(ppData);
	CString strCopyFile;
	HANDLE hFileOpen = ::CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		DWORD dwErrCode = ::GetLastError();
		if(ERROR_SHARING_VIOLATION != dwErrCode)
		{
			WriteLastLogToFile(dwErrCode,FileName + L" read error");
			return m;
		}
		/// 访问冲突，复制一份来操作
		strCopyFile = FileName + WRL_DOWNFILE_TEMPNAME;
		if(IsPathExist(strCopyFile))
		{
			::SetFileAttributes(strCopyFile,FILE_ATTRIBUTE_NORMAL);
			::DeleteFile(strCopyFile);
		}
		::CopyFile(FileName,strCopyFile,FALSE);
		hFileOpen = ::CreateFile(strCopyFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
		if(hFileOpen == INVALID_HANDLE_VALUE)
		{
			WriteLastLogToFile(::GetLastError(),FileName + L" read error");
			::DeleteFile(strCopyFile);
			return m;
		}
	}
	DWORD len = ::GetFileSize(hFileOpen,NULL);
	len++;
	*ppData = new BYTE[len];
	ATLASSERT(*ppData);
	if(NULL == *ppData)
	{
		if(strCopyFile.GetLength())
			::DeleteFile(strCopyFile);
		return m;
	}
	memset(*ppData,0,len);
	::ReadFile(hFileOpen,*ppData,len-1,&m,NULL);
	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	if(strCopyFile.GetLength())
		::DeleteFile(strCopyFile);
	return m;
}

int CBaseFuncLib::Utf8ToUS2(LPCSTR pSrc, WCHAR** pDst)
{
	WCHAR *strW = NULL;
	int iLength = ::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,NULL,0);
 	if(!iLength)
		return 0;
	iLength++;
    strW = new WCHAR[iLength];
	if(NULL == strW)
		return 0;
	::memset(strW,0,iLength*sizeof(WCHAR));
    int iLength1=::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,strW,iLength-1);
	*pDst=strW;
	return iLength1;
}

int CBaseFuncLib::US2ToUtf8(const CString& strSrc,char** pDst)
{
	char* cUTF8 = NULL;
	int iLength = 0;

	iLength = ::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,NULL,0,NULL,NULL);
	if(!iLength)
		return iLength;
	iLength++;
	cUTF8 = new char[iLength];
	if(cUTF8!=NULL)
	{
		::memset(cUTF8,0,iLength*sizeof(char));
		iLength = ::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,cUTF8,iLength-1,NULL,NULL);
		*pDst = cUTF8;
	}
	else
		iLength = 0;
	return iLength;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z')
		y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')
		y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')
		y = x - '0';
    else
		ATLASSERT(0);
    return y;
}

CString CBaseFuncLib::UrlEncode(const CString& strPara,BOOL bFlag)
{
	if(!bFlag)
	{
		std::string strTemp = "";
		int length = (int)strPara.GetLength();
		for (int i = 0; i < length; i++)
		{
			if (strPara[i] == '+')
				strTemp += ' ';
			else if (strPara[i] == '%')
			{
				ATLASSERT(i + 2 < length);
				unsigned char high = FromHex((unsigned char)strPara[++i]);
				unsigned char low = FromHex((unsigned char)strPara[++i]);
				strTemp += (high*16 + low);
			}
			else
				strTemp += (char)strPara[i];
		}
		WCHAR *szText = NULL;
		Utf8ToUS2(strTemp.c_str(),&szText);

		CString strReturn;
		if(NULL != szText)
		{
			strReturn = szText;
			delete []szText;
			szText = NULL;
		}
		return strReturn;
	}
	char *inbuffer = UrlEncodeToChar(strPara);
	if(NULL == inbuffer)
		return strPara;
	CString out(_T(""));
	int nLen = (int)strlen(inbuffer);
	for(int nIndex = 0;nIndex < nLen;nIndex++)
	{
		BYTE ch = inbuffer[nIndex];
		if(ch >= 0x80)
		{
			CString strTem;
			strTem.Format(_T("%%%02X"),ch);
			out+=strTem;
			strTem.Empty();
		}
		else
			out += ch;
	}

	delete []inbuffer;
	inbuffer = NULL;
	return out;
}

void CBaseFuncLib::UrlEncodeToFile(const CString& strPara,HANDLE hFileHandle,BOOL bTryFlag)
{
	CString strParaTem=strPara;
	strParaTem.Replace(_T("%"),_T("%25"));
	strParaTem.Replace(_T(" "),_T("%20"));
	strParaTem.Replace(_T("!"),_T("%21"));
	strParaTem.Replace(_T("\""),_T("%22"));
	strParaTem.Replace(_T("#"),_T("%23"));
	strParaTem.Replace(_T("$"),_T("%24"));
	strParaTem.Replace(_T("&"),_T("%26"));
	strParaTem.Replace(_T("'"),_T("%27"));
	if(bTryFlag)
		strParaTem.Replace(_T("+"),_T("%252B"));
	else
		strParaTem.Replace(_T("+"),_T("%2B"));
	strParaTem.Replace(_T("-"),_T("%2D"));
	strParaTem.Replace(_T("."),_T("%2E"));
	strParaTem.Replace(_T("/"),_T("%2F"));
	strParaTem.Replace(_T(":"),_T("%3A"));
	strParaTem.Replace(_T(";"),_T("%3B"));
	strParaTem.Replace(_T("<"),_T("%3C"));
	strParaTem.Replace(_T("="),_T("%3D"));
	strParaTem.Replace(_T(">"),_T("%3E"));
	strParaTem.Replace(_T("?"),_T("%3F"));
	strParaTem.Replace(_T("@"),_T("%40"));
	strParaTem.Replace(_T("["),_T("%5B"));
	strParaTem.Replace(_T("\\"),_T("%5C"));
	strParaTem.Replace(_T("]"),_T("%5D"));

	char *inbuffer=NULL;
	int nLen = US2ToUtf8(strParaTem,&inbuffer);
	if(NULL == inbuffer)
		return;
	DWORD dwWriteLen = 0;
	for(int nIndex = 0;nIndex<nLen;nIndex++)
	{
		BYTE bc = inbuffer[nIndex];
		if(bc >= 0x80)
		{
			CString strTem;
			strTem.Format(_T("%%%02X"),bc);
			BYTE *szBuff = NULL;
			int nWriteLen = Us2ToChar(strTem,(char **)&szBuff);
			ATLASSERT(szBuff);
			if(NULL != szBuff)
			{
				::WriteFile(hFileHandle,szBuff,nWriteLen-1,&dwWriteLen,NULL);
				delete []szBuff;
				szBuff = NULL;
			}
			strTem.Empty();
		}
		else if(bc)
		{
			::WriteFile(hFileHandle,&bc,1,&dwWriteLen,NULL);
		}
	}

	delete []inbuffer;
	inbuffer = NULL;
}

char* CBaseFuncLib::UrlEncodeToChar(const CString& strPara)
{
	CString strParaTem = strPara;
	strParaTem.Replace(L"%",L"%25");
	strParaTem.Replace(L"\"",L"%22");
	strParaTem.Replace(L"@",L"%40");
	strParaTem.Replace(L";",L"%3B");
	strParaTem.Replace(L" ",L"%20");
	strParaTem.Replace(L"'",L"%27");
	strParaTem.Replace(L"[",L"%5B");
	strParaTem.Replace(L"]",L"%5D");
	strParaTem.Replace(L"\\",L"%5C");
	strParaTem.Replace(L"\r",L"%5Cr");
	strParaTem.Replace(L"\n",L"%5Cn");
	strParaTem.Replace(_T("#"),_T("%23"));
	strParaTem.Replace(_T("&"),_T("%26"));
	strParaTem.Replace(_T("+"),_T("%2B"));
	strParaTem.Replace(_T("-"),_T("%2D"));
	strParaTem.Replace(_T(":"),_T("%3A"));
	strParaTem.Replace(_T("?"),_T("%3F"));
	strParaTem.Replace(_T("/"),_T("%2F"));
	strParaTem.Replace(_T("="),_T("%3D"));

	char *inbuffer = NULL;
	US2ToUtf8(strParaTem,&inbuffer);
	return inbuffer;
}

CString CBaseFuncLib::Utf8TOWChar(const char* pStr)
{
	CString strTxt(_T(""));
	/// 取得所需缓存的多少
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	if(!nBufferSize)
		return strTxt;
	/// 申请缓存空间
	wchar_t *pBuffer = new wchar_t[nBufferSize * sizeof(wchar_t)+1];
	if(NULL == pBuffer)
		return strTxt;
	memset(pBuffer,0,nBufferSize*sizeof(wchar_t)+1);
	/// 转码
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1 , pBuffer, nBufferSize*sizeof(wchar_t));
	strTxt = pBuffer;
	/// 释放缓存
	delete [] pBuffer;		
	pBuffer = NULL;
	return strTxt;
}

CString CBaseFuncLib::GetModulePath(HMODULE hModule)
{
	CString strPath(GetModuleName(hModule,FALSE));
	int iFind = strPath.ReverseFind(_T('\\'));
	if(-1 != iFind)
		strPath = strPath.Left(iFind+1);
	return strPath;
}

CString CBaseFuncLib::GetModuleName(HMODULE hModule,BOOL bNameFlag)
{
	CString strPath;
	TCHAR szPath[MAX_PATH];
	::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	DWORD dwLen = ::GetModuleFileName(hModule,szPath,MAX_PATH);
	if(dwLen > 5)
	{
		strPath = szPath;
		if(bNameFlag)
		{
			int iFind = strPath.ReverseFind(_T('\\'));
			if(-1 != iFind)
				strPath.Delete(0,iFind+1);
		}
	}
	return strPath;
}

HINSTANCE CBaseFuncLib::CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv )
{
	/// 整个路径默认最长支持260长度，文件名的长度最大不超过255，超过会报错
	HINSTANCE hInst = ::LoadLibraryEx(lpszFilename,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	if(NULL == hInst)
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			CString strLogInfo;
			if(0 == g_strLang.CompareNoCase(L"ENG"))
				strLogInfo.Format(L"%s Loading failed, error code:%ld,desc:%s",lpszFilename,dwErrCode, GetErrInfo(dwErrCode,g_hInstance));
			else
				strLogInfo.Format(L"%s 载入失败，错误码：%ld，描述：%s",lpszFilename,dwErrCode, GetErrInfo(dwErrCode,g_hInstance));
			WriteLogToFile(strLogInfo);
			ATLASSERT(hInst);
			strLogInfo.Empty();
		}
		return hInst;
	}
	HRESULT hRet = CreateInterface(hInst,clsid,iid,ppv);
	if(NULL == (*ppv))
	{
		FreeLibrary(hInst);
		hInst= NULL;
		if(FAILED(hRet))
		{
			CString strLogInfo;
			if(0 == g_strLang.CompareNoCase(L"ENG"))
				strLogInfo.Format(L"%s CreateInterface fails,error code:%ld",lpszFilename,hRet);
			else
				strLogInfo.Format(L"%s 创建接口失败,错误码:%ld",lpszFilename,hRet);
			WriteLogToFile(strLogInfo);
			ATLASSERT( hInst );
			strLogInfo.Empty();
		}
	}
	return hInst;
}

HRESULT CBaseFuncLib::CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv )
{
	HRESULT hRet(E_FAIL);
	if(hInst == NULL)
		return hRet;
	LPGetClassObject *lp = (LPGetClassObject *)::GetProcAddress(hInst,"DllGetClassObject");
	ATLASSERT( lp );
	if(lp == NULL)
		return hRet;
	IClassFactory* pCF = NULL;
	hRet = (*lp)(clsid,__uuidof(IClassFactory),(LPVOID *)&pCF);
	if(pCF != NULL)
	{
		hRet = pCF->CreateInstance( NULL,iid,ppv);
		pCF->Release();
		pCF = NULL;
	}
	return hRet;
}

void CBaseFuncLib::OpenUrl(const CString& strUrl)
{
	ShellExecute(NULL,L"open",strUrl,NULL,NULL,SW_SHOW);
}

BOOL CBaseFuncLib::ReadClipboardString(CString& strCopy,BOOL bClearFlag)
{
	BOOL bRetFlag = FALSE;
	if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		if(bClearFlag)
		{
			bRetFlag = ::OpenClipboard(NULL);
			if(bRetFlag)
			{
				::EmptyClipboard();
				::CloseClipboard();
			}
		}
		return bRetFlag;
	}
	bRetFlag = ::OpenClipboard(NULL);
	if(!bRetFlag)
	{
		CBaseFuncLib::WriteLastLogToFile(::GetLastError(),L"OpenReadClipboardError");
		return bRetFlag;
	}
	bRetFlag = FALSE;
	//读取数据  
    HANDLE hClip = ::GetClipboardData(CF_UNICODETEXT);
	if(NULL != hClip)
	{
		strCopy = CString((LPCTSTR)::GlobalLock(hClip));
		bRetFlag = ::GlobalUnlock(hClip);
		hClip = NULL;
		if(bClearFlag)
			::EmptyClipboard();
	}
	::CloseClipboard();
	return bRetFlag;
}

BOOL CBaseFuncLib::CopyStringToClipboard(const CString& strCopy)
{
	HANDLE hGlobalMemory = GlobalAlloc(GHND | GMEM_SHARE,(strCopy.GetLength() + 1)*sizeof(TCHAR)); 
	ATLASSERT(hGlobalMemory);
	if(NULL == hGlobalMemory)
		return FALSE;
	TCHAR *pBuf = (TCHAR*)GlobalLock(hGlobalMemory); 
	ATLASSERT(pBuf);
	if(NULL == pBuf)
	{
		::GlobalFree(hGlobalMemory);
		return FALSE;
	}
	lstrcpy(pBuf, strCopy); 
	GlobalUnlock(hGlobalMemory); // 解锁内存块
	BOOL bOpenFlag = ::OpenClipboard(NULL);
	if(!bOpenFlag)
		return bOpenFlag;
	::EmptyClipboard();
	::SetClipboardData(CF_UNICODETEXT, hGlobalMemory); // 将内存中的数据放置到剪贴板
	::CloseClipboard();
	return TRUE;
}

DWORD CBaseFuncLib::RunExe(LPWSTR szAppPath,WORD wShowFlag,\
	LPWSTR szCommandLine,LPWSTR szDesktopName,BOOL bWaitFlag)
{
	ATLASSERT(NULL != szAppPath || NULL != szCommandLine);

	STARTUPINFO stStartUpInfo;
	::SecureZeroMemory(&stStartUpInfo,sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow = wShowFlag;
	if(NULL != szDesktopName)
		stStartUpInfo.lpDesktop = szDesktopName; /// 指定运行桌面名称
	PROCESS_INFORMATION stProcessInfo;
	::memset(&stProcessInfo,0,sizeof(PROCESS_INFORMATION));
	
	if(CreateProcess(szAppPath,szCommandLine, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS, NULL,NULL, &stStartUpInfo, &stProcessInfo))
	{
		if(bWaitFlag)
		{
			//等待结束
			WaitForSingleObject(stProcessInfo.hProcess,INFINITE);
			::CloseHandle(stProcessInfo.hProcess);
			stProcessInfo.hThread = NULL;
			stProcessInfo.hProcess = NULL;
			stProcessInfo.dwThreadId = 0;
		}
	}
	else
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			CString strErrInfo(GetErrInfo(dwErrCode));
			if (strErrInfo.GetLength())
			{
#ifdef _DEBUG
				::MessageBox(NULL, strErrInfo,L"运行错误提示", MB_OK);
#else
				WriteLogToFile(CString(szCommandLine),strErrInfo);
#endif
				strErrInfo.Empty();
			}
			else
			{
				if(0 == g_strLang.CompareNoCase(L"ENG"))
					WriteLogToFile(NumToStr(dwErrCode) + L" Error code for program run");
				else
					WriteLogToFile(NumToStr(dwErrCode) + L" 程序运行错误码");
			}
		}
	}
	return stProcessInfo.dwProcessId;
}

///////////////////////////////////		CWrlThreadLock	//////////////////////////////////////////////////
inline BOOL CWrlThreadLock::Lock(TCHAR* szFuncName)
{
	BOOL bLockFlag = FALSE;
	if(!IsInit())
		Init();
	if(!IsInit())
		return bLockFlag;
	if(IsLock())
	{
		if(m_bLogFlag && NULL != szFuncName)
		{
			CString strLockInfo;
			if(0 == g_strLang.CompareNoCase(L"ENG"))
			{
				if(NULL != m_szPreName)
					strLockInfo.Format(L"%s Locked! New Call->%s",CString(m_szPreName),CString(szFuncName));
				else
					strLockInfo.Format(L"New Call->%s",CString(szFuncName));
			}
			else
			{
				if(NULL != m_szPreName)
					strLockInfo.Format(L"%s 已锁定！新执行->%s",CString(m_szPreName),CString(szFuncName));
				else
					strLockInfo.Format(L"新执行->%s",CString(szFuncName));
			}
			CBaseFuncLib::WriteLogToFile(strLockInfo);
			strLockInfo.Empty();
		}
	}
	else
	{
		if(m_bLogFlag && NULL != szFuncName)
		{
			CString strLockInfo;
			if(0 == g_strLang.CompareNoCase(L"ENG"))
				strLockInfo.Format(L"Lock->%s",CString(szFuncName));
			else
				strLockInfo.Format(L"锁定->%s",CString(szFuncName));
			CBaseFuncLib::WriteLogToFile(strLockInfo);
			strLockInfo.Empty();
		}
	}
	if(m_bLogFlag && NULL != m_szPreName && NULL != szFuncName)
	{
		memset(m_szPreName,0,MAX_PATH*sizeof(TCHAR));
		memcpy(m_szPreName,szFuncName,sizeof(TCHAR)*::_tcslen(szFuncName));
	}
	::EnterCriticalSection(m_pSection);
	bLockFlag = TRUE;
	return bLockFlag;
}

BOOL CWrlThreadLock::TryLock(TCHAR* szFuncName)
{
	BOOL bLockFlag = FALSE;
	if(!IsInit())
		Init();
	if(!IsInit())
		return bLockFlag;
	bLockFlag = ::TryEnterCriticalSection(m_pSection);
	if(bLockFlag && m_bLogFlag && NULL != szFuncName)
	{
		if(NULL != m_szPreName)
		{
			memset(m_szPreName,0,MAX_PATH*sizeof(TCHAR));
			memcpy(m_szPreName,szFuncName,sizeof(TCHAR)*::_tcslen(szFuncName));
		}
		CString strLockInfo;
		if(0 == g_strLang.CompareNoCase(L"ENG"))
			strLockInfo.Format(L"TryLock->%s",CString(szFuncName));
		else
			strLockInfo.Format(L"尝试锁定->%s",CString(szFuncName));
		CBaseFuncLib::WriteLogToFile(strLockInfo);
		strLockInfo.Empty();
	}
	return bLockFlag;
}

inline void CWrlThreadLock::UnLock(TCHAR* szFuncName)
{
	if(!IsInit())
		return;
	if(IsLock())
	{
		if(m_bLogFlag && NULL != szFuncName)
		{
			CString strLockInfo;
			if(0 == g_strLang.CompareNoCase(L"ENG"))
				strLockInfo.Format(L"%s UnLock->",CString(szFuncName));
			else
				strLockInfo.Format(L"%s 解锁->",CString(szFuncName));
			CBaseFuncLib::WriteLogToFile(strLockInfo);
			strLockInfo.Empty();
		}
		::LeaveCriticalSection(m_pSection);
		if(m_bLogFlag && NULL != m_szPreName)
			memset(m_szPreName,0,MAX_PATH*sizeof(TCHAR));
	}
}
