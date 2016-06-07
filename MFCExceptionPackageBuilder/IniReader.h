#pragma once
class CIniReader
{
public:
	CIniReader();
	~CIniReader();
	CIniReader(CString szFileName);
	int ReadInteger(CString szLoggerFilePath, CString szSection, CString szKey, int iDefaultValue);
	float ReadFloat(CString szSection, CString szKey, float fltDefaultValue);
	bool ReadBoolean(CString szSection, CString szKey, bool bolDefaultValue);
	CString ReadString(CString szLoggerFilePath, CString szSection, CString szKey, const CString szDefaultValue);
private:
	CString m_szFileName;
};