// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER			0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE		0x0700
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
/// 定义使用ATL的CString
#define _CSTRING_NS	ATL
/// 不适用WTL的类型
#define _WTL_NO_WTYPES
#include <atlapp.h>

extern CAppModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlstr.h>
#include <atltypes.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

using namespace ATL;

#pragma warning( disable : 4100 )
#pragma warning( disable : 4510 )
#pragma warning( disable : 4610 )
#pragma warning( disable : 4206 )

#ifndef _WIN64
#import "..\\..\\..\\Release\\Win32\\WrlEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#import "..\\..\\..\\Release\\Win32\\WrlBase.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#else
#import "..\\..\\..\\Release\\X64\\WrlEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#import "..\\..\\..\\Release\\X64\\WrlBase.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#endif