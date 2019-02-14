// PluginWinExe.cpp : main source file for PluginWinExe.exe
//

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "WrlRegKey.h"

CAppModule _Module;

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
//  If you are running on NT 4.0 or higher you can use the following call instead to 
//  make the EXE free threaded. This means that calls come in on a random RPC thread.
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	hRes = CoInitializeSecurity( NULL, -1, NULL, NULL, 
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL);

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES|ICC_DATE_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	int nRet = 0;
	// BLOCK: Run application
	{
		/// 接收命令行传递的开始侦听端口，同时接收PID参数(如果有的话)
		CString strCmd(lpstrCmdLine);
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
		nFind = strCmd.Find(L"=");
		ATLASSERT(-1 != nFind);
		CString strParaName(strCmd.Left(nFind));
		strCmd.Delete(0,nFind+1);
		mapPara[strParaName] = strCmd;
		strCmd.Empty();
		strParaName.Empty();

		/// 配置浏览器
		InitBrowerConfig();

		CMainDlg dlgMain;
		dlgMain.SetListenPara(mapPara);
		nRet = (int)dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
