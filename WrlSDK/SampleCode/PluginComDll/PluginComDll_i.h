

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PluginComDll_i_h__
#define __PluginComDll_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITextFile_FWD_DEFINED__
#define __ITextFile_FWD_DEFINED__
typedef interface ITextFile ITextFile;
#endif 	/* __ITextFile_FWD_DEFINED__ */


#ifndef __ISysInfo_FWD_DEFINED__
#define __ISysInfo_FWD_DEFINED__
typedef interface ISysInfo ISysInfo;
#endif 	/* __ISysInfo_FWD_DEFINED__ */


#ifndef ___ITextFileEvents_FWD_DEFINED__
#define ___ITextFileEvents_FWD_DEFINED__
typedef interface _ITextFileEvents _ITextFileEvents;
#endif 	/* ___ITextFileEvents_FWD_DEFINED__ */


#ifndef __TextFile_FWD_DEFINED__
#define __TextFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class TextFile TextFile;
#else
typedef struct TextFile TextFile;
#endif /* __cplusplus */

#endif 	/* __TextFile_FWD_DEFINED__ */


#ifndef ___ISysInfoEvents_FWD_DEFINED__
#define ___ISysInfoEvents_FWD_DEFINED__
typedef interface _ISysInfoEvents _ISysInfoEvents;
#endif 	/* ___ISysInfoEvents_FWD_DEFINED__ */


#ifndef __SysInfo_FWD_DEFINED__
#define __SysInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class SysInfo SysInfo;
#else
typedef struct SysInfo SysInfo;
#endif /* __cplusplus */

#endif 	/* __SysInfo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITextFile_INTERFACE_DEFINED__
#define __ITextFile_INTERFACE_DEFINED__

/* interface ITextFile */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITextFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C38672FA-B5C8-4D9D-89B5-2D71F0760661")
    ITextFile : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITextFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextFile * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITextFileVtbl;

    interface ITextFile
    {
        CONST_VTBL struct ITextFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextFile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextFile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextFile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextFile_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextFile_INTERFACE_DEFINED__ */


#ifndef __ISysInfo_INTERFACE_DEFINED__
#define __ISysInfo_INTERFACE_DEFINED__

/* interface ISysInfo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISysInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E46E1B5B-2891-414F-A21A-17B00231E650")
    ISysInfo : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISysInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISysInfo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISysInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISysInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISysInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISysInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISysInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISysInfo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISysInfoVtbl;

    interface ISysInfo
    {
        CONST_VTBL struct ISysInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISysInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISysInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISysInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISysInfo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISysInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISysInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISysInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISysInfo_INTERFACE_DEFINED__ */



#ifndef __PluginComDllLib_LIBRARY_DEFINED__
#define __PluginComDllLib_LIBRARY_DEFINED__

/* library PluginComDllLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_PluginComDllLib;

#ifndef ___ITextFileEvents_DISPINTERFACE_DEFINED__
#define ___ITextFileEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITextFileEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ITextFileEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("40756D41-E6C7-4648-80C4-AB70A325F831")
    _ITextFileEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ITextFileEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ITextFileEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ITextFileEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ITextFileEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ITextFileEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ITextFileEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ITextFileEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ITextFileEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ITextFileEventsVtbl;

    interface _ITextFileEvents
    {
        CONST_VTBL struct _ITextFileEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITextFileEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ITextFileEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ITextFileEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ITextFileEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ITextFileEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ITextFileEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ITextFileEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ITextFileEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TextFile;

#ifdef __cplusplus

class DECLSPEC_UUID("40613676-C8A5-4879-A59B-9CE6406476F6")
TextFile;
#endif

#ifndef ___ISysInfoEvents_DISPINTERFACE_DEFINED__
#define ___ISysInfoEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISysInfoEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ISysInfoEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FAC6FC19-9B22-4904-A656-581FCB35EC98")
    _ISysInfoEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISysInfoEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISysInfoEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISysInfoEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISysInfoEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ISysInfoEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ISysInfoEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ISysInfoEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ISysInfoEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ISysInfoEventsVtbl;

    interface _ISysInfoEvents
    {
        CONST_VTBL struct _ISysInfoEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISysInfoEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ISysInfoEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ISysInfoEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ISysInfoEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ISysInfoEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ISysInfoEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ISysInfoEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISysInfoEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SysInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("6D7595BD-BFB0-4D2C-906B-247028691A50")
SysInfo;
#endif
#endif /* __PluginComDllLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


