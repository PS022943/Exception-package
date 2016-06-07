#include "stdafx.h"
#include "IniWriter.h"
#include <iostream>
#include <Windows.h> 

CIniWriter::CIniWriter()
{
}


CIniWriter::~CIniWriter()
{
}

CIniWriter::CIniWriter(CString szFileName)
{
	//memset(m_szFileName, 0x00, 255);
	//memcpy(m_szFileName, szFileName, strlen(szFileName));
}
void CIniWriter::WriteInteger(CString szSection, CString szKey, int iValue)
{
	CString szValue;
	//sprintf(szValue, "%d", iValue);
	WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteFloat(CString szSection, CString szKey, float fltValue)
{
	CString szValue;
	//sprintf(szValue, "%f", fltValue);
	WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteBoolean(CString szSection, CString szKey, bool bolValue)
{
	CString szValue;
//	sprintf(szValue, "%s", bolValue ? "True" : "False");
	WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteString(CString szSection, CString szKey, CString szValue)
{
	WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}