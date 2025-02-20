/**
 * @file WRLBaseDef.h
 *
 * Copyright (c) 2013-?,成都佐罗软件
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 一些常量和基础类定义
 *
 * @author 汪佐怀
 *
 */

//  WRLBaseDef.h
/////////////////////////////////////////////////////////////////

#pragma once

/// 包含STL库声明
#include <vector>
#include <list>
#include <map>

/// 使用ATL的CString
#include <atlstr.h>
#include <Wtsapi32.h>
#include "AppletVersionDef.h"

///////////////////////// 公司、产品及注册表定义 /////////////////////////

/// 定义产品名称、单机版
#ifdef WRL_VRSION_STANDALONE
#ifdef WRL_VRSION_OVERSEAS
#define WRL_APPLICATION_NAME		L"PageHi"
#else
#define WRL_APPLICATION_NAME		L"PluginOK"
#endif
#endif

/// 网络版
#ifdef WRL_VRSION_NET
#ifdef WRL_VRSION_OVERSEAS
#define WRL_APPLICATION_NAME		L"PageHi"
#else
#define WRL_APPLICATION_NAME		L"BrowserApplet"
#endif
#endif

/// 定义运行程序数据路径
#define PRODUCT_COMMAN_APPRUNFILE	L"AppRunFile"

/// 定义公司名称
#ifndef WRL_VRSION_OVERSEAS
#define COMPANY_NAME_EN				L"ZorroSoft"
#define COMPANY_NAME_CHS			L"佐罗软件"
#else
#define COMPANY_NAME_EN				L"PageHi"
#define COMPANY_NAME_CHS			L"ZorroSoft"
#endif

/// 定义崩溃后的联系邮箱
#ifndef WRL_VRSION_OVERSEAS
#define	WRL_EMAIL_ADDR				L"wzh@zorrosoft.com"
#else
#define	WRL_EMAIL_ADDR				L"PageHi@gmail.com"
#endif

/// 定义崩溃后的主站地址
#ifndef WRL_VRSION_OVERSEAS
#define	WRL_WEB_ADDR				L"http://www.zorrosoft.com"
#else
#define	WRL_WEB_ADDR				L"http://www.pagehi.com"
#endif

#ifndef WRL_VRSION_OVERSEAS
#ifdef WRL_VRSION_STANDALONE
#define PRODUCT_REGNODENAME			L"PluginOK"
#define PRODUCT_SETUP_FILE			L"PluginOKIns"
#endif

#ifdef WRL_VRSION_NET
#define PRODUCT_REGNODENAME			L"BrowserApplet"
#define PRODUCT_REGPOKAUTH			L"PluginOKServer"
#define PRODUCT_SETUP_FILE			L"BrowserAppletIns"
#endif
#else
#ifdef WRL_VRSION_STANDALONE
#define PRODUCT_REGNODENAME			L"PageHi"
#define PRODUCT_SETUP_FILE			L"PageHiIns"
#endif

#ifdef WRL_VRSION_NET
#define PRODUCT_REGNODENAME			L"PageHiNet"
#define PRODUCT_REGPOKAUTH			L"PageHiServer"
#define PRODUCT_SETUP_FILE			L"PageHiNetIns"
#endif
#endif

/// 定义注册表中公司的路径
#define COMPANY_REGPATH				CString(L"Software\\") + COMPANY_NAME_EN

//////////////////////////////////////////////////////////////////////////

#define WRL_STRING_RETURN			"\r\n"
#define WRL_STRING_LRETURN			L"\r\n"
#define WRL_STRING_FILE				L"file:///"
#define WRL_STRING_FILE2			L"FILE:///"

///////////////////////////// 模块定义 //////////////////////////////////
#define WRL_SYNTDLL				L"NTdll.dll"
#define WRL_SYKERNELDLL			L"Kernel32.dll"
#define WRL_USERDLL				L"User32.dll"
#define WRL_REGSVR32			L"regsvr32.exe"

#define WRL_PSAPIDLL			L"Psapi.dll"

#define WRL_VLCOCXDLL			L"VlcOcx.dll"

/// AutoIT3程序
#define	WRL_AUTOIT3				L"IENewTab.dll"

#ifdef WRL_VRSION_STANDALONE
/// 平台核心支持库
#ifndef WRL_VRSION_OVERSEAS
#define WRL_ENGINEDLL			L"WrlEngine.dll"
#define WRL_BASEDLL				L"WrlBase.dll"
#define WRL_HTTPDLL				L"WrlHttp.dll"
#define WRL_APPLETDLL			L"WrlApplet.dll"
#define WRL_ENGINEDLL_CSHARP	L"Interop.WrlEngine.dll"
#define WRL_BASEDLL_CSHARP		L"Interop.WrlBase.dll"
#define WRL_APPLETDLL_CSHARP	L"Interop.WrlApplet.dll"
/// 浏览器HOOK支持库
#define WRL_BROWSERDLL			L"WrlBrowser.dll"
#else
#define WRL_ENGINEDLL			L"PHEngine.dll"
#define WRL_BASEDLL				L"PHBase.dll"
#define WRL_HTTPDLL				L"PHHttp.dll"
#define WRL_APPLETDLL			L"PHApplet.dll"
#define WRL_ENGINEDLL_CSHARP	L"Interop.PHEngine.dll"
#define WRL_BASEDLL_CSHARP		L"Interop.PHBase.dll"
#define WRL_APPLETDLL_CSHARP	L"Interop.PHApplet.dll"
/// 浏览器HOOK支持库
#define WRL_BROWSERDLL			L"PHBrowser.dll"
#endif
#else
#ifndef WRL_VRSION_OVERSEAS
/// 平台核心支持库
#define WRL_ENGINEDLL			L"ZbaEngine.dll"
#define WRL_BASEDLL				L"ZbaBase.dll"
#define WRL_HTTPDLL				L"ZbaHttp.dll"
#define WRL_APPLETDLL			L"ZbaApplet.dll"
#define WRL_ENGINEDLL_CSHARP	L"Interop.ZbaEngine.dll"
#define WRL_BASEDLL_CSHARP		L"Interop.ZbaBase.dll"
#define WRL_APPLETDLL_CSHARP	L"Interop.ZbaApplet.dll"
/// 浏览器HOOK支持库
#define WRL_BROWSERDLL			L"ZbaBrowser.dll"
#else
/// 平台核心支持库
#define WRL_ENGINEDLL			L"PageHiEngine.dll"
#define WRL_BASEDLL				L"PageHiBase.dll"
#define WRL_HTTPDLL				L"PageHiHttp.dll"
#define WRL_APPLETDLL			L"PageHiApplet.dll"
#define WRL_ENGINEDLL_CSHARP	L"Interop.PageHiEngine.dll"
#define WRL_BASEDLL_CSHARP		L"Interop.PageHiBase.dll"
#define WRL_APPLETDLL_CSHARP	L"Interop.PageHiApplet.dll"
/// 浏览器HOOK支持库
#define WRL_BROWSERDLL			L"PageHiBrowser.dll"
#endif
#endif

#define	WRL_7ZAPP				L"7Z.exe"
#define	WRL_BUTTRAPDLL			L"BugTrap.dll"
#define	WRL_WINDESKTOP			L"explorer.exe"
#define	WRL_WINWORD				L"winword.exe"
#define	WRL_WINEXCEL			L"excel.exe"
#define	WRL_WINPPT				L"powerpnt.exe"
#define	WRL_WINOUTLOOK			L"outlook.exe"

#define	WRL_WINWPSTEXT			L"wps.exe"
#define	WRL_WINWPSET			L"et.exe"
#define	WRL_WINWPSPPT			L"wpp.exe"
#define	WRL_WINWPSPDF			L"wpspdf.exe"

#define	WRL_WINYZTEXT			L"yozowp.exe"
#define	WRL_WINYZET				L"yozoss.exe"
#define	WRL_WINYZPPT			L"yozopg.exe"

#define	WRL_WINACAD				L"acad.exe"
#define	WRL_WINSWCAD			L"sldworks.exe"
#define	WRL_CATIACAD			L"cnext.exe"
#define	WRL_PROECAD				L"proe.exe"
#define	WRL_ZWCAD				L"zwcad.exe"

/// 浏览器这里都用小写，以下国外浏览器
#define WRL_IE					L"iexplore.exe"
#define WRL_NEWEDGE				L"msedge.exe"
#define WRL_CHROME				L"chrome.exe"
#define WRL_CHROMIUM			L"chromium.exe"
#define WRL_FIREFOX				L"firefox.exe"
#define WRL_OPERA				L"opera.exe"
#define WRL_VIVALDI				L"vivaldi.exe"
#define WRL_BRAVE				L"brave.exe"
#define WRL_YNADEX				L"browser.exe"

/// 国产浏览器
#define WRL_360CHROME			L"360chrome.exe"
#define WRL_360CHROMEX			L"360chromex.exe"
#define WRL_360SE				L"360se.exe"
#define WRL_360AI				L"360aibrowser.exe"
#define WRL_360GAME				L"360gt.exe"
#define WRL_360ENTSE			L"360ent.exe"
#define WRL_CMSBROWSER			L"cmsbrowser.exe"
#define WRL_QQ					L"qqbrowser.exe"
#define WRL_WECHATEXE			L"wechat.exe"
#define WRL_WECHATAPP			L"wechatappex.exe"
#define WRL_HUAWEI				L"huaweibrowser.exe"
#define WRL_DOUBAO				L"doubao.exe"
#define WRL_SOGOU				L"sogouexplorer.exe"
#define WRL_GTJA				L"gtjabrowser.exe"
#define WRL_QIANXIN				L"qaxbrowser.exe"
#define WRL_HAITAI				L"htbrowser.exe"
#define WRL_LENOVO				L"slbrowser.exe"
#define WRL_LIEBAO				L"liebao.exe"
#define WRL_MAXTHON				L"maxthon.exe"
#define WRL_QUARK				L"quark.exe"
#define WRL_DOUBLECORE			L"chromecore.exe"
#define WRL_GTJA				L"gtjabrowser.exe"

/// IE小程序
#define WRL_IEAPPLET			L"IEApplet.exe"
/// IE标签小程序
#define WRL_IENEWTAB			L"IENewTab.exe"

/// VLC网页播放器
#define WRL_VIDEO_VLCWEBPLAYER	L"VlcWebPlayer.exe"
/// RTSP多引擎网页播放器
#define WRL_VIDEO_RTSPWEBPLAYER	L"VideoWebPlayer.exe"

#ifndef WRL_VRSION_OVERSEAS
/// 原生网页播放小程序
#define WRL_VIDEO_HKWEBPLAYER	L"HKWebPlayer.exe"

/// 视频代理播放小程序
#ifdef WRL_VRSION_STANDALONE
#define WRL_VIDEO_PROXYPLAYER	L"VideoProxyPlayerS.exe"
#else
#define WRL_VIDEO_PROXYPLAYER	L"VideoProxyPlayer.exe"
#endif
#else
/// 原生网页播放小程序
#define WRL_VIDEO_HKWEBPLAYER	L"PHWebPlayer.exe"

/// 视频代理播放小程序
#ifdef WRL_VRSION_STANDALONE
#define WRL_VIDEO_PROXYPLAYER	L"PHVideoProxyPlayerS.exe"
#else
#define WRL_VIDEO_PROXYPLAYER	L"PHVideoProxyPlayer.exe"
#endif
#endif
/// 原生播放控件测试程序
#define WRL_VIDEO_NATIVEOCXTEST	L"VideoPlayerTest.exe"

#define	WRL_PID_FILEAPPLET		L"199BB8A7-9DD5-41F5-AE9D-2AA94300B0B3"	/// 文件操作小程序
#define	WRL_PID_NETFILEPID		L"762C3400-B3E5-4F00-8AA7-18E00F13E414"	/// 网络文件操作
#define	WRL_PID_LOCALFILEPID	L"D325C871-AB46-479B-B20E-1D861D26CA89"	/// 本地文件操作

///////////////////////////// 其它定义 ////////////////////////////////////

#ifndef GWL_USERDATA
#define GWL_USERDATA				(-21)
#endif

#ifndef GWL_HWNDPARENT
#define GWL_HWNDPARENT				(-8)
#endif

#ifndef KILO_DIGEST_LENGTH
#define KILO_DIGEST_LENGTH			1024
#endif

/// 长路径，系统默认260
#define MAX_PATH_NEW				1024

#ifndef _WIN64
/// 64位支持函数定义
typedef BOOL ( __stdcall *lpWow64RevertWow64FsRedirection)(PVOID OldValue);
typedef BOOL ( __stdcall *lpWow64DisableWow64FsRedirection)(PVOID *OldValue);
#endif

/// 获得系统信息
typedef void ( __stdcall *lpFN_GNSI)(LPSYSTEM_INFO);
/// 判断进程是否运行在64位系统
typedef BOOL ( __stdcall *lpIsWow64Process)(HANDLE, PBOOL);
/// 获得系统版本，RtlGetVersion的原型
typedef BOOL (__stdcall *lpGetVersion)(PRTL_OSVERSIONINFOW povi);
/// 获得SID长度，RtlLengthSid的原型
typedef ULONG (__stdcall *lpRtlLengthSid)(PSID Sid);
/// 注册表查询
typedef ULONG (__stdcall *lpNtQueryKey)(HANDLE,int,PVOID,ULONG,PULONG);

/// 获得类工厂创建函数定义
typedef HRESULT ( __stdcall LPGetClassObject)(REFCLSID,REFIID,LPVOID*);

typedef void (WINAPI *LPWTSFreeMemory)(PVOID pMemory);

/// 屏幕保护相关
typedef BOOL (WINAPI *LPGetWindowDisplayAffinity)(HWND hWnd,DWORD *pdwAffinity);
typedef BOOL (WINAPI *LPSetWindowDisplayAffinity)(HWND hWnd,DWORD dwAffinity);

//////////////////////////////////////////////////////////////////////////////////////////////////

/// 字符串vector
typedef std::vector		<CString>				CSTRING_VECTOR;
/// 字符串list
typedef std::list		<CString>				CSTRING_LIST;
/// 字符串属性对
typedef std::pair		<CString,CString>		CSTRING_PAIR;
/// 整数属性对
typedef std::pair		<ULONG_PTR,ULONG_PTR>	NUMBER_PAIR;
/// 整数字符串对
typedef std::pair		<ULONG_PTR,CString>		NUMBERSTRING_PAIR;

/// 字符串属性对列表
typedef std::vector		<CSTRING_PAIR>			PAIRSTRING_VECT;
/// 整数属性对列表
typedef std::vector		<NUMBER_PAIR>			PAIRNUMBER_VECT;
/// 整数字符串属性对列表
typedef std::vector		<NUMBERSTRING_PAIR>		NUMBERSTRING_VECT;

/// 字符串集合
typedef std::map		<CString,CString>		CSTRING_MAP;
/// 变体集合
typedef std::map		<CString,CComVariant>	VARIANT_MAP;
/// 字符串和句柄集合
typedef std::map		<CString,ULONG_PTR>		STRINGULONG_MAP;
/// 长整型和字符串集合
typedef std::map		<ULONG_PTR,CString>		ULONGSTRING_MAP;
/// 数值vector
typedef std::vector		<ULONG_PTR>				NUMBER_VECTOR;
/// 长整形集合
typedef std::map		<ULONG_PTR,ULONG_PTR>	LONGNUMBER_MAP;
/// 整形集合
typedef std::map		<int,int>				INTNUMBER_MAP;

////////////////////////////////////////////////////////////////////////////////////
