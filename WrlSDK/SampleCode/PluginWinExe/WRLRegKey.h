// WRLRegKey.h : 注册表操作头文件定义
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CWrlRegKey : public CRegKey
{
public:
	CWrlRegKey(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName, \
		HKEY hKey,REGSAM samDesired);

	virtual ~CWrlRegKey(void)
	{
		/// 销毁时自动关闭，避免句柄泄露
		Close();
	}

	/// 判断是否打开了注册表
	BOOL IsOpen()
	{
		return m_bOpenFlag;
	}

	/**
	 * @brief 设置新路径，重用类实例
	 *
	 *
	 * @param strMainRegPath 注册表主位置
	 * @param strChildName 注册表子目录名称
	 * @param bCurrentUser 是否为用户注册表，否则为系统注册表
	 * @return 
	 */
	BOOL NewRegPath(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName,\
		HKEY hKey,REGSAM samDesired);

	/**
	 * @brief 根据键名称获得字符串类型键值
	 *
	 *
	 * @param strKeyName  键名
	 * @param strKeyValue 键值
	 * @return 是否成功获取
	 */
	BOOL GetRegStringVal(const ATL::CString& strKeyName,ATL::CString& strKeyValue);
	
	/**
	 * @brief 设置注册表字符串类型值
	 *
	 *
	 * @param strKeyName  键名
	 * @param strKeyValue 键值
	 * @return 是否成功设置
	 */
	BOOL SetRegStringVal(const ATL::CString& strKeyName,const ATL::CString& strKeyValue);

	/**
	 * @brief 获得注册表DWORD类型值
	 *
	 *
	 * @param strKeyName 键名
	 * @param dwKeyValue 键值
	 * @return 是否获取成功
	 */
	BOOL GetRegDwordVal(const ATL::CString& strKeyName,DWORD& dwKeyValue);

	/**
	 * @brief 设置注册表DWORD类型值
	 *
	 *
	 * @param strKeyName 键名
	 * @param dwKeyValue 键值
	 * @return 设置是否成功
	 */
	BOOL SetRegDwordVal(const ATL::CString& strKeyName,DWORD dwKeyValue);

	/**
	 * @brief 获得注册表二进制值
	 *
	 *
	 * @param strKeyName 键名
	 * @param ppValue	键值
	 * @param pnBytes    返回长度
	 * @return 是否获取成功
	 */
	BOOL GetRegBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes);

	/**
	 * @brief 设置二进制值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @param pnBytes    返回长度
	 * @return 是否设置成功
	 */
	BOOL SetRegBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes);

	/**
	 * @brief 获得GUID类型注册表值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @return 返回是否获取成功
	 */
	BOOL GetRegGuidValue(const ATL::CString &strKeyName,GUID &Value);

	/**
	 * @brief 设置GUID类型注册表值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @return 返回是否设置成功
	 */
	BOOL SetRegGuidValue(const ATL::CString &strKeyName,GUID Value);

	/**
	 * @brief 删除指定的键值
	 *
	 *
	 * @param strKeyName 键名
	 * @return 
	 */
	BOOL DelKeyName(const ATL::CString& strKeyName);

	DWORD GetLastErr()
	{
		return m_dwLastErr;
	}

	/**
	 * @brief 关闭注册表句柄
	 *
	 *
	 */
	virtual void Close();

protected:

	/// 错误码
	DWORD	m_dwLastErr;

	/// 是否打开了注册表
	BOOL	m_bOpenFlag;
};
