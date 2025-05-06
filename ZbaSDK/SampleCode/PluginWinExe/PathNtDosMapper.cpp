#include "stdafx.h"
#include "PathNtDosMapper.h"

CPathNtDosMapper::CPathNtDosMapper(void)
{
	Refresh();
}

CPathNtDosMapper::~CPathNtDosMapper(void)
{
}

void CPathNtDosMapper::Refresh()
{
	memset(m_devices, 0, sizeof(m_devices));
	WCHAR drive[3]={L'A', L':', L'\0'};
	for(int i = L'A', j=0; i <= L'Z'; i++, j++)
	{
		drive[0]=i;
		QueryDosDeviceW(drive, m_devices[j].m_szDevice, _countof(m_devices[j].m_szDevice));
		if(!m_devices[j].m_szDevice[0])
			continue;
		m_devices[j].len=_tcslen(m_devices[j].m_szDevice);
	}
}

static const TCHAR sPrefixGlobalroot[]=_T("\\\\?\\Globalroot");
static const TCHAR sPrefixQuestions[]=_T("\\\\?\\Globalroot");
static const TCHAR sPrefixDevice[]=_T("\\Device");
static const TCHAR sPrefixDeviceLanman[]=_T("\\Device\\LanmanRedirector");
static const TCHAR sPrefixDeviceMup[]=_T("\\Device\\Mup");

CString CPathNtDosMapper::DosToNt(LPCTSTR pSrc)
{
	TCHAR drive[3]={_T('A'), _T(':'), _T('\0')};
	LPCTSTR buf = pSrc;

	if(_tcsnicmp(pSrc, sPrefixGlobalroot, _countof(sPrefixGlobalroot)-1) == 0)
		buf += _countof(sPrefixGlobalroot)-1;
	if(_tcsnicmp(pSrc, sPrefixQuestions, _countof(sPrefixQuestions)-1) == 0)
		buf += _countof(sPrefixQuestions)-1;

	try
	{
		for(int i=0; i<2; i++)
		{
			for(int j = 0; j < _countof(m_devices); j++)
			{
				if(!m_devices[j].len)
					continue;
				drive[0]=j+L'A';
				if(0==_tcsnicmp(buf, drive, 2))
				{
					CString strPath(m_devices[j].m_szDevice);
					strPath+= (buf + 2);
					return strPath;
				}
			}
			//not mapped? try refresh and try map again
			//Refresh();
			break;
		}
	}
	catch(...)
	{
	}
	return CString(buf);
}

CString CPathNtDosMapper::NtToDos(LPCTSTR pSrc)
{
	TCHAR drive[3]={_T('A'), _T(':'), _T('\0')};
	LPCTSTR buf = pSrc;

	if(_tcsnicmp(pSrc, sPrefixGlobalroot, _countof(sPrefixGlobalroot)-1) == 0)
		buf += _countof(sPrefixGlobalroot)-1;

	try
	{
		for(int i=0; i<2; i++)
		{
			for(int j = 0; j < _countof(m_devices); j++)
			{
				if(!m_devices[j].len)
					continue;
				drive[0]=j+L'A';
				if(0==_tcsnicmp(buf, m_devices[j].m_szDevice, m_devices[j].len) && buf[m_devices[j].len] == _T('\\'))
				{
					CString strPath(drive);
					strPath+= (buf + m_devices[j].len);
					return strPath;
				}
			}
			//not mapped? try refresh and try map again
			//Refresh();
			break;
		}
	}
	catch(...)
	{
	}
	return CString(buf);
}

CString CPathNtDosMapper::NtToDosEx(LPCTSTR pSrc)
{
	LPCTSTR buf = pSrc;
	try
	{
		if(_tcsnicmp(pSrc, sPrefixGlobalroot, _countof(sPrefixGlobalroot)-1) == 0)
			buf += _countof(sPrefixGlobalroot)-1;

		if(_tcsnicmp(buf, sPrefixDevice, _countof(sPrefixDevice)-1))
			return CString(buf);

		TCHAR drive[3]={_T('A'), _T(':'), _T('\0')};
		for(int j = 0; j < _countof(m_devices); j++)
		{
			if(!m_devices[j].len)
				continue;
			drive[0]=j+L'A';
			if(0==_tcsnicmp(buf, m_devices[j].m_szDevice, m_devices[j].len) && buf[m_devices[j].len] == _T('\\'))
			{
				CString strPath(drive);
				strPath+= (buf + m_devices[j].len);
				return strPath;
			}
		}

		size_t iShift = 0;

		if(_tcsnicmp(buf, sPrefixDeviceLanman, _countof(sPrefixDeviceLanman)-1) == 0)
			iShift = _countof(sPrefixDeviceLanman)-1;
		else if(_tcsnicmp(buf, sPrefixDeviceMup, _countof(sPrefixDeviceMup)-1) == 0)
			iShift = _countof(sPrefixDeviceMup)-1;

		if(iShift)
		{
			CString strPath(drive);
			strPath.Format(_T("\\%s"), buf + iShift);
			return strPath;
		}
	}
	catch(...)
	{
		//
		//
		//
	}
	return CString(buf);
}

CString CPathNtDosMapper::NtToDosExpandEnvironment(LPCTSTR pSrc)
{
	ATLASSERT(pSrc);

	//const size_t lenght=_UE_MAX_PATH; //_UE_MAX_PATH==296
	const size_t lenght=_MAX_PATH+36;
	CString path;
	{
		CStrBuf buff(path, (int)lenght);
		ExpandEnvironmentStrings(pSrc, buff, (DWORD)lenght);
	}
	return NtToDos(path);
}


