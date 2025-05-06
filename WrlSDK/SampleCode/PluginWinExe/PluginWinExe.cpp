// PluginWinExe.cpp : main source file for PluginWinExe.exe
//

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "WrlRegKey.h"

#ifdef WRL_VRSION_OVERSEAS
CString		g_strLang = L"ENG";
#else
CString		g_strLang = L"CHS";
#endif

CAppModule _Module;

extern HINSTANCE g_hInstance;

void InitBrowerConfig()
{
	/// 使用API函数IsHungAppWindow，可以判断窗口是否已经停止响应
	TCHAR szPath[MAX_PATH];
	::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	CString strFileName(szPath);
	int nFind = strFileName.ReverseFind(_T('\\'));
	if(-1 != nFind)
		strFileName.Delete(0,nFind+1);

	CString strRegPath(_T("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl"));
	CWrlRegKey reg(strRegPath,_T("FEATURE_TABBED_BROWSING"),HKEY_CURRENT_USER,KEY_WRITE);
	DWORD dwRet = reg.DelKeyName(strFileName);

	/// 解决登录后获取数据失败问题，设置支持IE9，解决淘宝等问题
	reg.NewRegPath(strRegPath,_T("FEATURE_BROWSER_EMULATION"),HKEY_CURRENT_USER,KEY_WRITE);
	reg.SetRegDwordVal(strFileName,9000);

	reg.NewRegPath(strRegPath,_T("FEATURE_LOCALMACHINE_LOCKDOWN"),HKEY_CURRENT_USER,KEY_WRITE);
	dwRet = reg.DelKeyName(strFileName);

	/// 启用缩放
	dwRet = -1;
	reg.NewRegPath(_T("Software\\Microsoft\\Internet Explorer"),_T("Zoom"),HKEY_CURRENT_USER,KEY_WRITE|KEY_READ);
	if(reg.GetRegDwordVal(_T("ZoomDisabled"),dwRet))
		reg.SetRegDwordVal(_T("ZoomDisabled"),0);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	HMODULE hKernel = NULL;
	BOOL bIsX6432 = CBaseFuncLib::IsWin6432(hKernel);
	TCHAR szRunPath[MAX_PATH];
	::memset(szRunPath,0,MAX_PATH * sizeof(TCHAR));
	::GetModuleFileName(NULL,szRunPath,MAX_PATH);
	CString strRunPath(szRunPath);
	/// 接收命令行传递的开始侦听端口，同时接收其它参数
	CString strCmd(lpstrCmdLine);
	if(L'\"' == strCmd.GetAt(0))
		strCmd.Delete(0);
	if(strCmd.Right(1) == L'\"')
		strCmd.Delete(strCmd.GetLength()-1);
	CSTRING_MAP mapPara;
	int nFind = strCmd.Find(L"&");
	while(-1 != nFind)
	{
		CString strPara(strCmd.Left(nFind));
		strCmd.Delete(0,nFind+1);
		nFind = strPara.Find(L"=");
		ATLASSERT(-1 != nFind);
		CString strParaName(strPara.Left(nFind));
		strPara.Delete(0,nFind+1);
		mapPara[strParaName] = strPara;
		strPara.Empty();
		strParaName.Empty();
		nFind = strCmd.Find(L"&");
	}
	if(strCmd.GetLength())
	{
		nFind = strCmd.Find(L"=");
		ATLASSERT(-1 != nFind);
		CString strParaName(strCmd.Left(nFind));
		strCmd.Delete(0,nFind+1);
		mapPara[strParaName] = strCmd;
		strCmd.Empty();
		strParaName.Empty();
	}

#ifndef _DEBUG
	/// 64位系统，判断安装的版本位数
	if(bIsX6432 && !CBaseFuncLib::IsX64Exe(strRunPath))
	{
		CString strComPath;
		/// 如果在这个进程里需要加载64位版本的控件(先获取strComPath的实际路径，再调用IsX64Exe判断是否为64位版本程序)，需要在这里启动64位版本的主程序
		if(strComPath.GetLength() && TRUE == CBaseFuncLib::IsX64Exe(strComPath))
		{
			/// 启用64位运行
			int nFind = strRunPath.ReverseFind(L'\\');
			CString strPath = strRunPath.Left(nFind + 1);
			CString strFileName(strRunPath.Right(strRunPath.GetLength()-nFind-1));
			strPath += L"X64\\";
			strPath += strFileName;
			if(1 == CBaseFuncLib::IsPathExist(strPath))
			{
				DWORD dwPID = 0;
				SHELLEXECUTEINFO execinfo;
				memset(&execinfo,0,sizeof(execinfo));
				execinfo.lpFile         = strPath;
				execinfo.cbSize         = sizeof(execinfo);
				execinfo.fMask          = SEE_MASK_NOCLOSEPROCESS;
				execinfo.nShow			= SW_NORMAL;
				execinfo.lpParameters   = lpstrCmdLine;
				ShellExecuteEx(&execinfo);
				/// 启动64位版本的程序运行，退出32位版本
				CBaseFuncLib::WriteLogToFile(strPath + L" 64Runing->");
				::OleUninitialize();
				strRunPath.Empty();
				strFileName.Empty();
				return FALSE;
			}
		}
	}
	strRunPath.Empty();
#endif

//  If you are running on NT 4.0 or higher you can use the following call instead to 
//  make the EXE free threaded. This means that calls come in on a random RPC thread.
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	hRes = CoInitializeSecurity( NULL, -1, NULL, NULL, 
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL);
	ATLASSERT(SUCCEEDED(hRes));
	AtlInitCommonControls(ICC_BAR_CLASSES|ICC_DATE_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	g_hInstance = hInstance;
	ATLASSERT(SUCCEEDED(hRes));
	CSTRING_MAP::iterator itFind = mapPara.find(L"LANG");
	if(itFind != mapPara.end())
		g_strLang = itFind->second;
	if(0 == g_strLang.CompareNoCase(L"AUTO"))
	{
		g_strLang.Empty();
		LANGID lang_id = GetUserDefaultUILanguage();
		switch(lang_id)
		{
		case 1028:
		case 2052:
		case 3076:
		case 4100:
		case 5124:
			g_strLang = L"CHS";
			break;
		default:
			g_strLang = L"ENG";
			break;
		}
	}
	AtlAxWinInit();

	int nRet = 0;
	// BLOCK: Run application
	{
		/// 配置浏览器
		InitBrowerConfig();
		CMainDlg dlgMain;
		dlgMain.SetListenPara(mapPara);
		nRet = (int)dlgMain.DoModal();
	}
	AtlAxWinTerm();
	_Module.Term();
	::CoUninitialize();
	CBaseFuncLib::WriteLogToFile(L"Quit OK");
	return nRet;
}
