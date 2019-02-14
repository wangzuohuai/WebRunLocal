

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Feb 11 18:26:40 2019
 */
/* Compiler settings for PluginComDll.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "PluginComDll_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _PluginComDll_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } PluginComDll_MIDL_TYPE_FORMAT_STRING;

typedef struct _PluginComDll_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } PluginComDll_MIDL_PROC_FORMAT_STRING;

typedef struct _PluginComDll_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } PluginComDll_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const PluginComDll_MIDL_TYPE_FORMAT_STRING PluginComDll__MIDL_TypeFormatString;
extern const PluginComDll_MIDL_PROC_FORMAT_STRING PluginComDll__MIDL_ProcFormatString;
extern const PluginComDll_MIDL_EXPR_FORMAT_STRING PluginComDll__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITextFile_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITextFile_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISysInfo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISysInfo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const PluginComDll_MIDL_PROC_FORMAT_STRING PluginComDll__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const PluginComDll_MIDL_TYPE_FORMAT_STRING PluginComDll__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITextFile, ver. 0.0,
   GUID={0xC38672FA,0xB5C8,0x4D9D,{0x89,0xB5,0x2D,0x71,0xF0,0x76,0x06,0x61}} */

#pragma code_seg(".orpc")
static const unsigned short ITextFile_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ITextFile_ProxyInfo =
    {
    &Object_StubDesc,
    PluginComDll__MIDL_ProcFormatString.Format,
    &ITextFile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITextFile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PluginComDll__MIDL_ProcFormatString.Format,
    &ITextFile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ITextFileProxyVtbl = 
{
    0,
    &IID_ITextFile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ITextFile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ITextFileStubVtbl =
{
    &IID_ITextFile,
    &ITextFile_ServerInfo,
    7,
    &ITextFile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ISysInfo, ver. 0.0,
   GUID={0xE46E1B5B,0x2891,0x414F,{0xA2,0x1A,0x17,0xB0,0x02,0x31,0xE6,0x50}} */

#pragma code_seg(".orpc")
static const unsigned short ISysInfo_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISysInfo_ProxyInfo =
    {
    &Object_StubDesc,
    PluginComDll__MIDL_ProcFormatString.Format,
    &ISysInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISysInfo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PluginComDll__MIDL_ProcFormatString.Format,
    &ISysInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ISysInfoProxyVtbl = 
{
    0,
    &IID_ISysInfo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ISysInfo_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ISysInfoStubVtbl =
{
    &IID_ISysInfo,
    &ISysInfo_ServerInfo,
    7,
    &ISysInfo_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    PluginComDll__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _PluginComDll_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISysInfoProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ITextFileProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _PluginComDll_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISysInfoStubVtbl,
    ( CInterfaceStubVtbl *) &_ITextFileStubVtbl,
    0
};

PCInterfaceName const _PluginComDll_InterfaceNamesList[] = 
{
    "ISysInfo",
    "ITextFile",
    0
};

const IID *  const _PluginComDll_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _PluginComDll_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PluginComDll, pIID, n)

int __stdcall _PluginComDll_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _PluginComDll, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _PluginComDll, 2, *pIndex )
    
}

const ExtendedProxyFileInfo PluginComDll_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PluginComDll_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PluginComDll_StubVtblList,
    (const PCInterfaceName * ) & _PluginComDll_InterfaceNamesList,
    (const IID ** ) & _PluginComDll_BaseIIDList,
    & _PluginComDll_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

