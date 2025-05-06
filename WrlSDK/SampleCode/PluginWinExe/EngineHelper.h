#pragma once
#include "ComHelper.h"

class CEngineHelper : public CComHelper
{
protected:
	/// Ä£¿éÃû³Æ
	static CString m_strModelName;

	static CString GetLoadPath();

public:

	static void FreeDB();

	static ICxImgPtr GetCxImg();

	static IOpenEncryPtr GetOpenEncry();

	static IJsonServicePtr GetJsonService();

	static IHttpVisitPtr GetHttpVisit();

	static IConnectHelperPtr GetDBHelper();

	static IWmiDevicePtr GetWmiDevice();

	static IStreamDataPtr GetStreamData();
};