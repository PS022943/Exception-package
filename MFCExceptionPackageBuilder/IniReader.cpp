#include "stdafx.h"
#include "IniReader.h"
#include <iostream>
#include <Windows.h>

CIniReader::CIniReader()
{
}


CIniReader::~CIniReader()
{
}

CIniReader::CIniReader(CString szFileName)
{
	//memset(m_szFileName, 0x00, 255);
	//memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CIniReader::ReadInteger(CString szLoggerFilePath, CString szSection, CString szKey, int iDefaultValue)
{
	m_szFileName.Empty();
	m_szFileName.Append(szLoggerFilePath);
	int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
	return iResult;
}
float CIniReader::ReadFloat(CString szSection, CString szKey, float fltDefaultValue)
{
	CString szResult;
	CString szDefault;
	float fltResult = 0.0;
//	sprintf(szDefault, "%f", fltDefaultValue);
	//GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	//fltResult = atof(szResult);
	return fltResult;
}
bool CIniReader::ReadBoolean(CString szSection, CString szKey, bool bolDefaultValue)
{
	bool bolResult = false;
	/*char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf(szDefault, "%s", bolDefaultValue ? "True" : "False");
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	bolResult = (strcmp(szResult, "True") == 0 ||
		strcmp(szResult, "true") == 0) ? true : false;*/
	return bolResult;
}
CString CIniReader::ReadString(CString szLoggerFilePath, CString szSection, CString szKey, const CString szDefaultValue)
{
	m_szFileName.Empty();
	m_szFileName.Append(szLoggerFilePath);
	TCHAR szResult[1024] = { 0 };
	GetPrivateProfileString(szSection, szKey,
		szDefaultValue, szResult, 255, m_szFileName);
	return szResult;
}