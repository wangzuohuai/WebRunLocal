/*	NOTE: If you running this in a none-Windows platform, 
	then you should remove the include file below.
  */
#include "stdafx.h"
#include "TextFileHelper.h"

#if PEK_TX_TECHLEVEL == 0
 //Include iostream if running in ANSI mode.
 #include <iostream>
#endif

#pragma warning( disable : 4996)

//在段模式下，没条记录允许的段数
#define ENABLESECTION_SECTION_COUNT		10

//Base constructor
CTextFileBase::CTextFileBase()
{
#if PEK_TX_TECHLEVEL > 0
	m_codepage = CP_ACP;
	m_unknownChar = 0;
#else
	m_unknownChar = '?';
#endif

#if PEK_TX_TECHLEVEL == 1
	m_hFile = INVALID_HANDLE_VALUE;
#elif PEK_TX_TECHLEVEL == 2
	m_file = NULL;
	m_closeAndDeleteFile = false;
#endif
	m_datalost = false;
	m_buffpos = -1;
}

//Base destructor
CTextFileBase::~CTextFileBase()
{
	Close();
}

//Set which character to use when conversion can't be done
void CTextFileBase::SetUnknownChar(const char unknown)
{
	m_unknownChar = unknown;
}

//True if data was lost during conversion
bool CTextFileBase::IsDataLost() const
{
	return m_datalost;
}

//Reset the data lost flag
void CTextFileBase::ResetDataLostFlag()
{
	m_datalost = false;
}

#if PEK_TX_TECHLEVEL > 0

//Make sure we have a legal value for code page to use when
//converting string. Used for debugging only.
#define IsLegalCodePage(cp) (cp == CP_ACP || cp == CP_MACCP || cp == CP_OEMCP || cp == CP_SYMBOL || cp == CP_THREAD_ACP || cp == CP_UTF7 || cp == CP_UTF8 || IsValidCodePage(cp))

//(Windows-specific) Set codepage to use when working with non-unicode strings.
void CTextFileBase::SetCodePage(const UINT codepage)
{
	ATLASSERT(IsLegalCodePage(codepage));
	m_codepage = codepage;
}

//(Windows-specific) Get codepage to use when working with non-unicode strings.
UINT CTextFileBase::GetCodePage() const
{
	return m_codepage;
}

//(Windows-specific) Convert string to wstring with current codepage
inline void CTextFileBase::CharToWstring(const char* from, wstring &to) const
{
	ConvertCharToWstring(from, to, m_codepage);
}

//(Windows-specific) Convert wstring to string with current codepage
inline void CTextFileBase::WcharToString(const wchar_t* from, string &to) 
{
	ConvertWcharToString(from, to, m_codepage, &m_datalost, m_unknownChar);
}

//(Windows-specific) Convert char* to wstring
void CTextFileBase::ConvertCharToWstring(const char* from, wstring &to, UINT codepage)
{
	to = L"";
	ATLASSERT(IsLegalCodePage(codepage));
	//Use api convert routine
	int wlen = MultiByteToWideChar(codepage,0,from,-1,NULL,0);

	//if wlen == 0, some unknown codepage was probably used.
	ATLASSERT(wlen);
	if(wlen == 0) 
		return;
	wchar_t* wbuffer = new wchar_t[wlen+2];
    MultiByteToWideChar(codepage,0,from,-1,wbuffer,wlen);
	to = wbuffer;
	delete [] wbuffer;
}

//(Windows-specific) Convert wchar_* to string
void CTextFileBase::ConvertWcharToString(const wchar_t* from, string &to, 
	UINT codepage, bool* datalost, char unknownchar)
{
	to = "";
	ATLASSERT(IsLegalCodePage(codepage));	
	int alen = 	WideCharToMultiByte(codepage,0,from,-1,NULL,0,NULL,NULL);

	//if alen == 0, some unknown codepage was probably used.
	ATLASSERT(alen);
	if(alen == 0) 
		return;
	//Use mfc convert routine
	char* abuffer = new char[alen+2]; 
	BOOL UsedDefault=FALSE;

	WideCharToMultiByte(codepage,0,from,-1,abuffer,alen, 
		(unknownchar != 0 ? &unknownchar:NULL),(datalost != NULL ? &UsedDefault:NULL));

	if( datalost != NULL && UsedDefault != FALSE)
		*datalost = true;
	
	to = abuffer;
	delete [] abuffer;
}

#else

//(None-windows-specific) Convert string to wstring 
inline void CTextFileBase::CharToWstring(const char* from, wstring &to) const
{
	ConvertCharToWstring(from, to);
}

//(None-windows-specific) Convert wstring to string
inline void CTextFileBase::WcharToString(const wchar_t* from, string &to) 
{
	ConvertWcharToString(from, to, &m_datalost, m_unknownChar);
}

//(None-windows-specific) Convert char* to wstring
void CTextFileBase::ConvertCharToWstring(const char* from, wstring &to)
{
	to = L"";
	size_t pos=0;
	wchar_t temp[1];
	while(true)
	{
		size_t len = mbtowc(temp, from+pos, MB_CUR_MAX);
		//Found end
		if(len == 0)
			return;
		else if(len == (size_t)-1)
		{
			//Unknown character, should never happen
			pos++;
		}
		else
		{
			to += temp[0];
			pos += len;
		}
	}
}

//(None-windows-specific) Convert wchar_* to string
void CTextFileBase::ConvertWcharToString(const wchar_t* from, string &to, bool* datalost, char unknownchar)
{
	to = "";
	char* temp = new char[MB_CUR_MAX];	
	while(*from != L'\0')
	{
		size_t len = wctomb(temp, *from);

		//Found end
		if(len == 0)
			break;
		else if(len == (size_t)-1)
		{
			//Replace with unknown character
			to += unknownchar;

			if(datalost != NULL)
				*datalost=true;
		}
		else
		{
			//Copy all characters
			for(size_t i=0; i<len; i++)
				to += temp[i];
		}

		from++;
	}
	delete [] temp;
}
#endif

//Return file encoding
CTextFileBase::TEXTENCODING CTextFileBase::GetEncoding() const 
{
	return m_encoding;
};

//Is file open?
int CTextFileBase::IsOpen() 
{
#if PEK_TX_TECHLEVEL == 0
	return m_file.is_open();
#elif PEK_TX_TECHLEVEL == 1
	return m_hFile != INVALID_HANDLE_VALUE;
#else
	return (m_file != NULL && m_file->m_hFile != CFile::hFileNull);
#endif
}

//Close file
void CTextFileBase::Close()
{
	if(IsOpen())
	{
#if PEK_TX_TECHLEVEL == 0
		m_file.close();
#elif PEK_TX_TECHLEVEL == 1
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
#else
		if(m_closeAndDeleteFile)
		{
			m_file->Close();
		}		
#endif
	}
#if PEK_TX_TECHLEVEL == 2
	if(m_closeAndDeleteFile)
	{
		delete m_file;
		m_file = NULL;
	}
#endif
}

USHORT IsPathExist(const CString& strPath)
{
	USHORT nRet = 0;
	if(strPath.IsEmpty())
		return nRet;
	WIN32_FIND_DATA	data;
	BOOL bFindDir = FALSE;
	CString strFind(strPath);
	if (0 == strPath.Right(1).CompareNoCase(L"\\"))
	{
		/// 查找目录
		bFindDir = TRUE;
		strFind += L"*.*";
	}		
	HANDLE hFindFile = ::FindFirstFile(strFind,&data);
	if(INVALID_HANDLE_VALUE != hFindFile)
	{
		if(bFindDir)
		{
			while(INVALID_HANDLE_VALUE != hFindFile)
			{
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
				{				
					nRet = 1;
					break;
				}
				if (!FindNextFile(hFindFile,&data))
					break;
			}
		}
		else
		{
			nRet = 1;
			if(!data.nFileSizeHigh && !data.nFileSizeLow)
				nRet = 2;
		}
		::FindClose(hFindFile);
		hFindFile = NULL;
	}
	else
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			/// 目前不支持长路径260 https://learn.microsoft.com/zh-cn/windows/win32/fileio/maximum-file-path-limitation?tabs=registry
			if(ERROR_FILE_NOT_FOUND != dwErrCode && ERROR_PATH_NOT_FOUND != dwErrCode)
				nRet = 3;
		}
	}
	return nRet;
}

BOOL CreatePath(const CString& strLocalDir)
{
	BOOL bCreateFlag = FALSE;
	CString strWorkDir,strPath=strLocalDir;
	int index = -2;
	index = strPath.Find(L'\\');
	if(index == -1)
		return FALSE;
	strWorkDir = strPath.Left(index);
	strPath.Delete(0,index+1);
	while(strPath.GetLength())
	{
		index = strPath.Find(L'\\');
		if(index == -1)
		{
			bCreateFlag = ::CreateDirectory(strLocalDir,NULL);
			strWorkDir.Empty();
			strPath.Empty();
			break;
		}
		strWorkDir += L"\\";
		strWorkDir += strPath.Left(index);
		strPath.Delete(0,index + 1);
		if(!IsPathExist(strWorkDir + L"\\"))
			bCreateFlag = ::CreateDirectory(strWorkDir,NULL);
	}
	if(!bCreateFlag)
	{
		DWORD dwErrCode = ::GetLastError();
		::Sleep(100);
	}
	return bCreateFlag;
}

//Create textfile
CTextFileWrite::CTextFileWrite(const FILENAMECHAR* filename, TEXTENCODING encoding)
{
	CString strFilPath(filename);
	int iFind = strFilPath.ReverseFind(L'\\');
	if(-1 != iFind)
	{
		/// 创建目录
		CreatePath(strFilPath.Left(iFind));
	}

#if PEK_TX_TECHLEVEL == 0
	m_file.open(filename, ios::binary | ios::out );
#elif PEK_TX_TECHLEVEL == 1
	m_hFile = ::CreateFile(filename,GENERIC_WRITE,0,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
#else
	m_file = new CFile;
	m_file->Open(filename, CFile::modeCreate | CFile::modeWrite);
	m_closeAndDeleteFile = true;
#endif
	m_buffpos = -1;
	m_buffsize = 0;
	m_encoding = encoding;
	WriteBOM();
}

#if PEK_TX_TECHLEVEL==2
//Create textfile from CFile object
CTextFileWrite::CTextFileWrite(CFile* file, TEXTENCODING encoding)
{
	ATLASSERT(file);
	m_file = file;
	m_closeAndDeleteFile = false;
	m_encoding = encoding;
	m_buffpos = -1;
	m_buffsize = 0;
	WriteBOM();
}
#endif

CTextFileWrite::~CTextFileWrite()
{
	Close();
}

void CTextFileWrite::WriteBOM()
{
	//Write BOM
	if(IsOpen())
	{
		if(m_encoding == UNI16_BE || m_encoding == UNI16_LE)
		{
			//Write BOM
			WriteWchar( 0xFEFF );
		}
		else if(m_encoding == UTF_8)
		{
			//Write UTF-8 BOM.  0xEF 0xBB 0xBF
			WriteByte(0xEF);
			WriteByte(0xBB);
			WriteByte(0xBF);
		}
	}
}

//Write one byte
void CTextFileWrite::WriteByte(const unsigned char byte)
{
	//Instead of writing, save data in buffer and write when buffer is full
	if(m_buffpos+1 >= BUFFSIZE)
		Flush();
	m_buffpos++;
	m_buf[m_buffpos] = byte;
}

//Write and empty buffer
void CTextFileWrite::Flush()
{
#if PEK_TX_TECHLEVEL==0
	m_file.write(m_buf, m_buffpos+1);
#elif PEK_TX_TECHLEVEL==1

	DWORD nWritten;
	if (!::WriteFile(m_hFile, m_buf, m_buffpos+1, &nWritten, NULL))
	{
		//Something bad has happend! Close file
		CTextFileBase::Close();

		//Throw exception
		throw CTextFileException(GetLastError());
	}
#else
	m_file->Write(m_buf, m_buffpos+1);	
#endif	
	m_buffpos = -1;
}

void CTextFileWrite::WriteWchar(const wchar_t ch)
{
	//Write HO byte first?
	if(m_encoding == UNI16_BE)
	{
		//Write HO byte
		WriteByte((unsigned char) (ch >> 8) );
		//Write LO byte
		WriteByte((unsigned char) ch);
	}
	else if(m_encoding == UNI16_LE)
	{
		//Write LO byte
		WriteByte((unsigned char) ch);
		//Write HO byte
		WriteByte((unsigned char) (ch >> 8) );
	}
	else
	{
		//http://www.cl.cam.ac.uk/~mgk25/unicode.html#examples
		//http://www.ietf.org/rfc/rfc3629.txt
		//Just a single byte?
		if(ch <= 0x7F)
		{
			//U-00000000 - U-0000007F:  0xxxxxxx  
			WriteByte( (unsigned char) ch );
		}	
		else if(ch <= 0x7FF)//Two bytes?
		{
			//U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xC0 | (ch>>6)) );
			WriteByte( (unsigned char) (0x80 | (ch&0x3F)) );
		}		
		else if(ch <= 0xFFFF)//Three bytes?
		{
			//U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xE0 | (  ch>>12)		));
			WriteByte( (unsigned char) (0x80 | ( (ch>>6)&0x3F ) ));
			WriteByte( (unsigned char) (0x80 | ( ch&0x3F )		));
		}
		/* //UPS! I did some coding for UTF-32, may be useful in the future :-)
		//Four bytes?
		else if(ch <= 0x1FFFFF)
		{
			//U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xF0 | (  ch>>18)		 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}
		
		//Five bytes bytes?
		else if(ch <= 0x3FFFFFF)
		{
			//U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xF8 | (  ch>>24)	 	 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>18)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}		
		//Five bytes bytes?
		else if(ch <= 0x7FFFFFFF)
		{
			//U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
			WriteByte( (unsigned char) (0xFC | (  ch>>30)		 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>24)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>18)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}*/
	}
}

//Write new line
void CTextFileWrite::WriteEndl()
{
	if(m_encoding == ASCII)
	{
		WriteByte(0x0D);
		WriteByte(0x0A);
	}
	else 
	{
		WriteWchar(0x0D);
		WriteWchar(0x0A);
	}
}

//Write a c-string in ASCII. 
//In versions before 1.02 this function wrote directly to file,
//no buffer was used. But sense WriteEndl() used buffer the file was
//written incorretly. Now buffer is used here too, this is the
//fastest solution.
void CTextFileWrite::WriteAsciiString(const char* s)
{	
	while(*s != '\0')
	{
		WriteByte(*s);
		s++;
	}
}

CTextFileWrite& CTextFileWrite::operator<< (const char* text)
{
	Write(text); 
	return *this;
};

CTextFileWrite& CTextFileWrite::operator << (const string& text)
{
	Write(text.c_str());
	return *this;
}

CTextFileWrite& CTextFileWrite::operator<< (const wchar_t* text)
{
	Write(text); 
	return *this;
};

CTextFileWrite& CTextFileWrite::operator << (const wstring& text)
{
	Write(text.c_str());
	return *this;
}

CTextFileWrite& CTextFileWrite::operator<< (const wchar_t wc)
{
	//Not the perfect code, but it's easy!
	wstring text;
	text=wc;
	Write(text.c_str());
	return *this;
};

CTextFileWrite& CTextFileWrite::operator<< (const char c)
{
	//Not the perfect code, but it's easy!
	string text;
	text=c;
	Write(text.c_str());
	return *this;
};

BOOL CTextFileWrite::Write(const string& text)
{
	try
	{
		Write(text.c_str());
	}
	catch ( ... )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTextFileWrite::Write(const wstring& text)
{
	try
	{
		Write(text.c_str());
	}
	catch ( ... )
	{
		return FALSE;
	}
	return TRUE;
}

//Write char*
void CTextFileWrite::Write(const char* text)
{
	//ASCIItext file format?
	if(m_encoding == ASCII)
		WriteAsciiString(text);
	else
	{
		//Convert text to unicode
		wstring utext;
		CharToWstring(text, utext);

		//OK, lets write unicode
		for(wstring::const_iterator i=utext.begin();
			i < utext.end();
			i++)
		{
			WriteWchar(*i);
		}
	}
}

//Write wcar_t*
void CTextFileWrite::Write(const wchar_t* utext)
{
	//ASCII text file format?
	if(m_encoding == ASCII)
	{
		//Convert to string and write
		string text;
		WcharToString(utext, text);
		WriteAsciiString(text.c_str());
	}
	else
	{
		while(*utext != 0)
		{
			WriteWchar(*utext);
			utext++;
		}
	}
}

//Close the file
void CTextFileWrite::Close()
{
	if(IsOpen())
		Flush();

	CTextFileBase::Close();
}

CTextFileRead::CTextFileRead(const FILENAMECHAR* filename)
{

#if PEK_TX_TECHLEVEL==0
	//If not Windows, do this
	m_file.open(filename, ios::binary | ios::in);
#elif PEK_TX_TECHLEVEL == 1
	m_hFile = ::CreateFile(	filename, GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
#else
	m_file = new CFile;
	//In windows, do this
	m_file->Open(filename, CFile::modeRead | CFile::shareDenyWrite );
	m_closeAndDeleteFile = true;
#endif

	m_firstLine = true;
	m_endoffile = (IsOpen()==0);

	//Force reading to buffer next time
	m_buffpos=-1;
	m_useExtraBuffer = false;	
	//段落信息无效
	m_section_enable = false;

	m_buf_first_section = -1;
	m_wbuf_first_section = 
	m_current_pos = 0;
	m_section_count = 0;

	m_dirty = false;
	ReadBOM();
}

#if PEK_TX_TECHLEVEL==2
CTextFileRead::CTextFileRead(CFile* file)
{
	ATLASSERT(file);

	m_file = file;
	m_closeAndDeleteFile = false;

	m_firstLine = true;
	m_endoffile = (IsOpen()==0);

	//Force reading to buffer next time
	m_buffpos=-1;

	m_useExtraBuffer=false;
	
	ReadBOM();
}
#endif

void CTextFileRead::ReadBOM()
{
	if( IsOpen() )
	{
		unsigned char bytes[2];

		//Read the first two bytes
		ReadByte(bytes[0]);
		ReadByte(bytes[1]);

		//Figure out what format the file is in
		if( bytes[0] == 0xFF && bytes[1] == 0xFE)
			m_encoding = UNI16_LE;
		else if( bytes[0] == 0xFE && bytes[1] == 0xFF)
			m_encoding = UNI16_BE;
		else if( bytes[0] == 0xEF && bytes[1] == 0xBB)
		{
			//This is probably UTF-8, check the third byte
			unsigned char temp;
			ReadByte(temp);
			if( temp == 0xBF)
				m_encoding = UTF_8;
			else
			{
				//Set text format.
				m_encoding = ASCII;
				ResetFilePointer();
			}
		}
		else
		{
			m_encoding = ASCII;

			//Set start pos
			ResetFilePointer();
		}
	}
}

//End of file?
bool CTextFileRead::Eof() const
{
	return m_endoffile;
}

//Read one byte
void CTextFileRead::ReadByte(unsigned char& ch)
{
	//Use extrabuffer if needed
	if(m_useExtraBuffer)
	{
		m_useExtraBuffer=false;
		ch = m_extraBuffer_char;
		return;
	}
	//In Windows, do this...
	//If buffer used or not read
	if(m_buffpos==-1 || m_buffpos == BUFFSIZE-1)
	{
#if PEK_TX_TECHLEVEL==0
		m_buffsize = m_file.read(m_buf, BUFFSIZE).gcount();	
#elif PEK_TX_TECHLEVEL==1

	DWORD dwRead;
	if (!::ReadFile(m_hFile, m_buf, BUFFSIZE, &dwRead, NULL))
	{
		//Couldn't read!
		Close();
		m_buffsize = 0;

		//Throw exception
		throw CTextFileException(GetLastError());
	}
	else
		m_buffsize = (int) dwRead;
#else
		m_buffsize=m_file->Read(m_buf, BUFFSIZE);
#endif
		if(m_buffsize == 0)
		{
			m_endoffile=true;
			ch = 0;
			return;
		}
		m_buffpos=0;
	}
	else
	{
		m_buffpos++;
		if(m_buffpos >= m_buffsize)
		{
			m_endoffile=true;
			ch = 0;
			return;
		}
	}

	//当前位置增加
	m_current_pos++;
	ch = m_buf[m_buffpos];
}

void CTextFileRead::ReadWchar(wchar_t& ch)
{
	if(m_useExtraBuffer)
	{
		m_useExtraBuffer=false;
		ch = m_extraBuffer_wchar;
		return;
	}

	if(m_encoding == UTF_8)
	{	
		//This is quite tricky :-/
		//http://www.cl.cam.ac.uk/~mgk25/unicode.html#examples
		unsigned char byte;
		ReadByte(byte);

		int onesBeforeZero = 0;

		{
			//Calc how many ones before the first zero...
			unsigned char temp = byte;
			while( (temp & 0x80)!=0 )
			{
				temp = (unsigned char) (temp << 1);
				onesBeforeZero++;
			}
		}

		if(onesBeforeZero==0)
		{
			ch = byte;
			return;
		}
		else if(onesBeforeZero == 2)
		{
			//U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
			unsigned char byteb;
			ReadByte(byteb);
			ch = (wchar_t)( ((0x1F & byte) << 6)| (0x3F & byteb)) ;
			return;
		}
		else if(onesBeforeZero == 3)
		{
			//U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
			unsigned char byteb, bytec;
			ReadByte(byteb);
			ReadByte(bytec);

			ch = (wchar_t)( ((0x0F & byte) << 12) | ((0x3F & byteb) << 6) | (0x3F & bytec) );
			return;
		}

		//This should never happend! It it do, something is wrong with the file.
		ch = 0xFFFD;
	}
	else
	{

		unsigned char bytes[2];
		ReadByte(bytes[0]);
		ReadByte(bytes[1]); 

		if(m_encoding == UNI16_BE)
			ch = (wchar_t) ( ((wchar_t) bytes[0] << 8) | (wchar_t) bytes[1] ) ;
		else
			ch = (wchar_t) ( ((wchar_t) bytes[1] << 8) | (wchar_t) bytes[0] );
	}
}

void CTextFileRead::ResetFilePointer()
{
	m_useExtraBuffer=false;

#if PEK_TX_TECHLEVEL==0
	m_file.clear();
	m_file.seekg(0, ios::beg);

#elif PEK_TX_TECHLEVEL==1

	::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
	m_section_position.clear();

#else
	m_file->SeekToBegin();
#endif
	//Force reread buffer
	m_buffpos=-1;
	m_current_pos = 0;

	m_firstLine = true;
	m_endoffile = false;
}

bool CTextFileRead::ReadLine(string& line,bool bRecordSection )
{
	//EOF?
	if(Eof())
		return false;

	//使用章节序号获取的话，不允许正常的获取方法了
	if(!m_section_enable)
		ATLASSERT(!bRecordSection);

	if(m_encoding == ASCII)
	{
		return ReadCharLine(line);
	}
	wstring wline;
	if(!ReadWcharLine(wline))
		return false;
	//Convert
	WcharToString(wline.c_str(), line);
	return true;
}

bool CTextFileRead::ReadLine(wstring& line,bool bRecordSection)
{
	//EOF?
	if(Eof())
		return false;
	if(m_encoding == ASCII)
	{
		string cline;
		if(!ReadCharLine(cline))
			return false;
		//Convert to wstring
		CharToWstring(cline.c_str(), line);
		return true;
	}
	return ReadWcharLine(line);
}

bool CTextFileRead::ReadLineByID(string& line,long section_id)
{
	ATLASSERT(m_section_enable);			//段落打开
	ATLASSERT(section_id >= 0);
	ATLASSERT(section_id < m_section_count);
	ATLASSERT(m_hFile);

	if(m_buf_first_section != section_id/ENABLESECTION_SECTION_COUNT * ENABLESECTION_SECTION_COUNT)
	{
		//当前段不在缓冲区，填充缓冲区
		m_section_buf.clear();
		m_buf_first_section = section_id/ENABLESECTION_SECTION_COUNT * ENABLESECTION_SECTION_COUNT;

		//设置文件位置
		::SetFilePointer(m_hFile, m_section_position[section_id/ENABLESECTION_SECTION_COUNT], NULL, FILE_BEGIN);
		m_section_position[section_id/ ENABLESECTION_SECTION_COUNT];
		m_buffpos = -1;
		m_endoffile = false;
		m_useExtraBuffer=false;

		//跳过前几个
		for(long i = m_buf_first_section;
			i < ENABLESECTION_SECTION_COUNT+m_buf_first_section && i< m_section_count; i++)
		{
			ReadLine(line);
			m_section_buf.push_back(line); 
		}
	}
	line = m_section_buf[section_id%ENABLESECTION_SECTION_COUNT];
	return true;
}

bool CTextFileRead::ReadLineByID(wstring& line,long section_id)
{
	ATLASSERT(m_section_enable);			//段落打开
	ATLASSERT(section_id >= 0);
	ATLASSERT(section_id < m_section_count);
	ATLASSERT(m_hFile);

	if(m_wbuf_first_section != section_id/ENABLESECTION_SECTION_COUNT * ENABLESECTION_SECTION_COUNT)
	{
		//当前段不在缓冲区，填充缓冲区
		m_wsection_buf.clear();
		m_wbuf_first_section = section_id/ENABLESECTION_SECTION_COUNT * ENABLESECTION_SECTION_COUNT;

		//设置文件位置
		::SetFilePointer(m_hFile, m_section_position[section_id/ENABLESECTION_SECTION_COUNT], NULL, FILE_BEGIN);
		m_section_position[section_id/ ENABLESECTION_SECTION_COUNT];
		m_buffpos = -1;
		m_endoffile = false;
		m_useExtraBuffer=false;

		//跳过前几个
		for(long i = m_wbuf_first_section;
			i < ENABLESECTION_SECTION_COUNT+m_wbuf_first_section && i< m_section_count; i++)
		{
			ReadLine(line);
			m_wsection_buf.push_back(line); 
		}
	}

	line = m_wsection_buf[section_id%ENABLESECTION_SECTION_COUNT];
	return true;
}

bool CTextFileRead::Read(string& all, const string newline)
{
	if(!IsOpen())
		return false;
	int buffpos = 0;
	int buffsize = GuessCharacterCount()+2;

	//Create buffer
	char* buffer = new char[buffsize];
	//If not possible, don't use any buffer
	if(buffer == NULL) 
		buffsize = 0;

	string temp;
	all = temp;
	all.reserve(buffsize);
	bool firstLine=true;

	while(!Eof())
	{
		if(ReadLine(temp))
		{
			//Add new line, if not first line
			if(!firstLine)
				temp.insert(0, newline.c_str());
			else
				firstLine=false;

			//Add to buffer if possible
			if(buffpos + (int) temp.size() < buffsize)
			{
				strcpy(buffer+buffpos, temp.c_str());
				buffpos += (int) temp.size();
			}
			else
			{
				//Copy to all string
				if(buffpos != 0)
				{
					all.append(buffer, buffpos);
					buffpos = 0;
				}
				all += temp;
			}
		}
	};
	//Copy to all string
	if(buffpos != 0)
	{
		all.append(buffer, buffpos);
	}
	if(buffer != NULL)
		delete [] buffer;
	return true;
}

bool CTextFileRead::Read(wstring& all, const wstring newline)
{
	if(!IsOpen())
		return false;
	int buffpos = 0;
	int buffsize = GuessCharacterCount()+2;

	//Create buffer
	wchar_t* buffer = new wchar_t[buffsize];

	//If not possible, don't use any buffer
	if(buffer == NULL) 
		buffsize = 0;

	wstring temp;
	all = temp;
	all.reserve(buffsize);
	bool firstLine=true;

	while(!Eof())
	{
		if(ReadLine(temp))
		{
			//Add new line, if not first line
			if(!firstLine)
				temp.insert(0, newline.c_str());
			else
				firstLine=false;

			//Add to buffer if possible
			if(buffpos + (int) temp.size() < buffsize)
			{
				wcscpy(buffer+buffpos, temp.c_str());
				buffpos += (int) temp.size();
			}
			else
			{
				//Copy to all string
				if(buffpos != 0)
				{
					all.append(buffer, buffpos);
					buffpos = 0;
				}

				all += temp;
			}
		}
	};

	//Copy to all string
	if(buffpos != 0)
	{
		all.append(buffer, buffpos);
	}
	if(buffer != NULL)
		delete [] buffer;
	return true;
}

int CTextFileRead::GuessCharacterCount()
{
#if PEK_TX_TECHLEVEL==2
	int bytecount = (int) m_file->GetLength();
#else
	//Code needed to get file size when not using MFC
	int bytecount = 1024*1024; //Default: 1 MB
#endif
	//If ASCII, the number of characters is the byte count.
	//If UTF-8, it can't be more than bytecount, so use byte count
	if(m_encoding == ASCII || m_encoding == UTF_8)
		return bytecount;
	//Otherwise, every two byte in one character
	return bytecount/2;
}

#if PEK_TX_TECHLEVEL == 2

bool CTextFileRead::ReadLine(CString& line)
{
#ifndef _UNICODE
	string temp;
#else
	wstring temp;
#endif

	if(!ReadLine(temp))
		return false;

	line = temp.c_str();
	return true;
}

bool CTextFileRead::Read(CString& all, const CString newline)
{
#ifndef _UNICODE
	string temp, n=newline;
#else
	wstring temp, n=newline;
#endif

	if(!Read(temp, n))
		return false;
	all = temp.c_str();
	return true;
}

#endif

//Returns false if end-of-file was reached
//(line will not be changed). If returns true,
//it means that last line ended with a line break.
bool CTextFileRead::ReadWcharLine(wstring& line)
{
	//EOF?
	if(Eof())
		return false;
	wchar_t ch=0;
	
	//Ignore 0x0D and 0x0A
	//or 0x0D 0x0D and 0x0A
	//or just 0x0D
	//or just 0x0A
	//except when we read the first line
	ReadWchar(ch);

	if(!m_firstLine)
	{
		if(ch == 0x0D) //If next is 0x0A, ignore that too
		{
			ReadWchar(ch);

			if(ch == 0x0A)
				ReadWchar(ch);
			else if(ch == 0x0D)
			{
				ReadWchar(ch);
				if(ch == 0x0A)
					ReadWchar(ch);
			}

		}
		else if(ch == 0x0A)
		{
			ReadWchar(ch);
		}
	}
	else
	{
		//Next time we reads we don't read the first line in file.
		//(then we should ignore \r\n)
		m_firstLine = false;
	}
	//Clear line
	line = L"";

	//It would be a lot easier if we didn't use a buffer, and added directly to
	//line, but that is quite slow.
	wchar_t buffer[BUFFSIZE];
	buffer[BUFFSIZE-1] = '\0';
	//Where to insert next character
	int bufpos = 0;

	//Read line
	while(ch != 0x0D && ch != 0x0A && !Eof())
	{
		//End of buffer?
		if(bufpos+1 >= BUFFSIZE)
		{
			//Add to line
			line.append(buffer, bufpos);
			bufpos=0;
		}

		buffer[bufpos] = ch;
		bufpos++;

		ReadWchar(ch);
	};

	buffer[bufpos] = L'\0';
	line += buffer;
	
	//Save currents character in extra buffer
	m_useExtraBuffer=true;
	m_extraBuffer_wchar=ch;
	return true;
}

//Returns false if end-of-file was reached
//(line will not be changed). If returns true,
//it means that last line ended with a line break.
bool CTextFileRead::ReadCharLine(string& line)
{
	//EOF?
	if(Eof())
		return false;
	unsigned char ch=0;
	
	//Ignore 0x0D and 0x0A
	//or just 0x0D
	//or just 0x0A
	//except when we read the first line
	ReadByte(ch);

	if(!m_firstLine)
	{		
		if(ch == 0x0D) //If next is 0x0A, ignore that too
		{
			ReadByte(ch);

			if(ch == 0x0A)
				ReadByte(ch);
			else if(ch == 0x0D)
			{
				ReadByte(ch);
				if(ch == 0x0A)
					ReadByte(ch);
			}
		}
		else if(ch == 0x0A)
		{
			ReadByte(ch);
		}
	}
	else
	{
		//Next time we reads we don't read the first line in file.
		//(then we should ignore \r\n)
		m_firstLine = false;
	}
	//Clear line
	line = "";

	//It would be a lot easier if we didn't use a buffer, and added directly to
	//line, but that is quite slow.
	char buffer[BUFFSIZE];
	buffer[BUFFSIZE-1] = '\0';
	//Where to insert next character
	int bufpos = 0;

	//Read line
	while(ch != 0x0D && ch != 0x0A && !Eof())
	{
		//End of buffer?
		if(bufpos+1 >= BUFFSIZE)
		{
			//Add to line
			line.append(buffer, bufpos);
			bufpos=0;
		}

		buffer[bufpos] = ch;
		bufpos++;

		ReadByte(ch);
	};

	buffer[bufpos] = L'\0';
	line += buffer;
	
	//Save currents character in extra buffer
	m_useExtraBuffer=true;
	m_extraBuffer_char=ch;
	return true;
}

long CTextFileRead::EnableSection(HANDLE hRecorder)
{
	ATLASSERT(m_hFile);
	m_section_enable = true;
	//载入成功标志
	bool load_ok = false;

	if(hRecorder)
		load_ok = LoadSectionInfo(hRecorder);

	if(!load_ok)
	{
		//载入失败，进行重建
		m_section_count = 0;

		for(;;)
		{
			long last_pos = m_current_pos;
			for(long loop = 0 ; loop < ENABLESECTION_SECTION_COUNT ; loop++)
			{
				string str;
				if(!ReadLine(str,true))
					break;
				m_section_count++;
				str.empty();
			}
			m_section_position.push_back(last_pos);
			if(Eof())
				break;
		}
		m_dirty = true;
	}
	return m_section_count;
}

bool CTextFileRead::LoadSectionInfo(HANDLE hRecorder)
{
	ATLASSERT(hRecorder);
	ATLASSERT(m_hFile);

	//获取修改
	FILETIME last_write_time;		//修改时间
	if(!GetFileTime(m_hFile,NULL,NULL,&last_write_time))
		return false;

	DWORD dwReadSize = 0;
	BOOL bResult = FALSE;

	FILETIME last_write_time_recorder;
	//读取最后修改时间
	bResult = ReadFile(hRecorder,&last_write_time_recorder,sizeof(FILETIME),&dwReadSize,NULL);

	if(!bResult || dwReadSize != sizeof(FILETIME))
		return false;
	//比较修改时间
	if(memcmp(&last_write_time,&last_write_time_recorder,sizeof(FILETIME)) != 0)
		return false;

	//读取每条记录的段落数
	long	recorder_section_count = 0;
	bResult = ReadFile(hRecorder,&recorder_section_count,sizeof(long),&dwReadSize,NULL);
	if(!bResult || dwReadSize != sizeof(long))
		return false;
	if(recorder_section_count != ENABLESECTION_SECTION_COUNT)
		return false;
	//读取段落数
	bResult = ReadFile(hRecorder,&m_section_count,sizeof(long),&dwReadSize,NULL);
	if(!bResult || dwReadSize != sizeof(long))
		return false;
	m_section_position.clear();

	long lSectionCount = m_section_count /  ENABLESECTION_SECTION_COUNT;
	if(m_section_count%ENABLESECTION_SECTION_COUNT)
		lSectionCount++;
	//读取各个段落的文件位置
	for(long section = 0; section < lSectionCount; section++)
	{
		long section_pos = 0;
		bResult = ReadFile(hRecorder,&section_pos,sizeof(long),&dwReadSize,NULL);

		if(!bResult || dwReadSize != sizeof(long))
		{
			m_section_position.clear();
			return false;
		}
		m_section_position.push_back(section_pos);
	}
	return true;
}

bool CTextFileRead::SaveSectionInfo(HANDLE hRecorder)
{
	ATLASSERT(hRecorder);
	if(!m_dirty)
		return true;/// 不需要保存
	//获取修改
	FILETIME last_write_time;		//修改时间
	if(!GetFileTime(m_hFile,NULL,NULL,&last_write_time))
		return false;
	DWORD dwReadSize = 0;
	BOOL bResult = 0;
	//写入最后修改时间
	bResult = WriteFile(hRecorder,&last_write_time,sizeof(FILETIME),&dwReadSize,NULL);
	if(!bResult || dwReadSize != sizeof(FILETIME))
		return false;
	//写入每条记录段落数
	long	recorder_section_count = ENABLESECTION_SECTION_COUNT;
	bResult = WriteFile(hRecorder,&recorder_section_count,sizeof(long),&dwReadSize,NULL);
	if(!bResult || dwReadSize != sizeof(long))
		return false;
	//写入段落数
	bResult = WriteFile(hRecorder,&m_section_count,sizeof(long),&dwReadSize,NULL);
	if(!bResult || dwReadSize != sizeof(long))
		return false;
	long lSectionCount = m_section_count / ENABLESECTION_SECTION_COUNT;
	if(m_section_count%ENABLESECTION_SECTION_COUNT)
		lSectionCount++;
	for(long section = 0; section < lSectionCount ; section++)
	{
		long section_pos = m_section_position[section];
		bResult = WriteFile(hRecorder,&section_pos,sizeof(long),&dwReadSize,NULL);

		if(!bResult || dwReadSize != sizeof(long))
			return false;
	}
	return ::FlushFileBuffers(hRecorder)?true:false; 
}

DWORD CTextFileRead::GetFileSize()
{
	ATLASSERT(m_hFile);
	return ::GetFileSize(m_hFile,NULL);
}