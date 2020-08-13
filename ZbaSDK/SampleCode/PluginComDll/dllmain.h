// dllmain.h : 模块类的声明。

class CPluginComDllModule : public ATL::CAtlDllModuleT< CPluginComDllModule >
{
public :
	DECLARE_LIBID(LIBID_PluginComDllLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PLUGINCOMDLL, "{F0FFF18E-E8C7-4778-8B38-45C9CD71912E}")
};

extern class CPluginComDllModule _AtlModule;
