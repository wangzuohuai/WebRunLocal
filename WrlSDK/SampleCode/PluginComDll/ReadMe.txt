========================================================================
    活动模板库 : PluginComDll 项目概述
========================================================================

应用程序向导已为您创建了此 PluginComDll 项目，作为编写动态链接库 (DLL) 
的起点。

本文件概要介绍组成项目的每个文件的内容。

PluginComDll.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 的版本信息，
    以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

PluginComDll.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。 
    它包含有关项目文件与筛选器之间的关联信息。 在 IDE 中，通过这种关联，
    在特定节点下以分组形式显示具有相似扩展名的文件。
    例如，“.cpp”文件与“源文件”筛选器关联。

PluginComDll.idl
    此文件包含项目中定义的类型库、接口和组件类的 IDL 定义。
    此文件将由 MIDL 编译器处理以生成：
        C++ 接口定义和 GUID 声明		(PluginComDll.h)
        GUID 定义			(PluginComDll_i.c)
        类型库				(PluginComDll.tlb)
        封送处理代码			(PluginComDll_p.c 和 
                                           dlldata.c）

PluginComDll.h
    此文件包含 PluginComDll.idl 中定义的项目的 C++ 接口定义
    和 GUID 声明。 它将在编译过程中由 MIDL 重新生成。

PluginComDll.cpp
    此文件包含对象映射和 DLL 导出的实现。

PluginComDll.rc
    这是程序使用的所有 Microsoft Windows 资源的列表。

PluginComDll.def
    此模块定义文件为链接器提供有关 DLL 所要求的导出的信息， 
    它包含用于以下内容的导出：
        DllGetClassObject
        DllCanUnloadNow
        DllRegisterServer
        DllUnregisterServer
        DllInstall

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 PluginComDll.pch 的预编译头 (PCH) 文件和
    名为 StdAfx.obj 的预编译类型文件。

Resource.h
    这是用于定义资源 ID 的标准头文件。


/////////////////////////////////////////////////////////////////////////////
