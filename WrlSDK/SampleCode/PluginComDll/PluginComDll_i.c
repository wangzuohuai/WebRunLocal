

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITextFile,0xC38672FA,0xB5C8,0x4D9D,0x89,0xB5,0x2D,0x71,0xF0,0x76,0x06,0x61);


MIDL_DEFINE_GUID(IID, IID_ISysInfo,0xE46E1B5B,0x2891,0x414F,0xA2,0x1A,0x17,0xB0,0x02,0x31,0xE6,0x50);


MIDL_DEFINE_GUID(IID, LIBID_PluginComDllLib,0xA22E18F1,0x95F8,0x4FDB,0x99,0xD2,0x18,0x8E,0x5F,0xB1,0x2B,0x23);


MIDL_DEFINE_GUID(IID, DIID__ITextFileEvents,0x40756D41,0xE6C7,0x4648,0x80,0xC4,0xAB,0x70,0xA3,0x25,0xF8,0x31);


MIDL_DEFINE_GUID(CLSID, CLSID_TextFile,0x40613676,0xC8A5,0x4879,0xA5,0x9B,0x9C,0xE6,0x40,0x64,0x76,0xF6);


MIDL_DEFINE_GUID(IID, DIID__ISysInfoEvents,0xFAC6FC19,0x9B22,0x4904,0xA6,0x56,0x58,0x1F,0xCB,0x35,0xEC,0x98);


MIDL_DEFINE_GUID(CLSID, CLSID_SysInfo,0x6D7595BD,0xBFB0,0x4D2C,0x90,0x6B,0x24,0x70,0x28,0x69,0x1A,0x50);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



