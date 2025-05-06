#pragma once

class CPathNtDosMapper
{
	struct CDevice
	{
		TCHAR m_szDevice[_MAX_PATH];
		size_t len;
	};
	CDevice m_devices['Z'-'A'+1];
	void Refresh();
public:
	CPathNtDosMapper(void);
	virtual ~CPathNtDosMapper(void);

	CString DosToNt(LPCTSTR pSrc);
	CString NtToDos(LPCTSTR pSrc);
	CString NtToDosEx(LPCTSTR pSrc);
	CString NtToDosExpandEnvironment(LPCTSTR pSrc);
};
