#pragma once
class CIniWriter
{
public:
	CIniWriter();
	~CIniWriter();
	CIniWriter(CString szFileName);
	void WriteInteger(CString szSection, CString szKey, int iValue);
	void WriteFloat(CString szSection, CString szKey, float fltValue);
	void WriteBoolean(CString szSection, CString szKey, bool bolValue);
	void WriteString(CString szSection, CString szKey, CString szValue);
private:
	CString m_szFileName;
};

