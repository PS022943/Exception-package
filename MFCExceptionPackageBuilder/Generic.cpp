#include "stdafx.h"
#include "Generic.h"


CGeneric::CGeneric()
{
}


CGeneric::~CGeneric()
{
}

CString CGeneric::GetExeFolderPath()
{
	TCHAR AppNameTChar[MAX_PATH];
	GetModuleFileName(NULL, AppNameTChar, MAX_PATH);
	CString AppName = AppNameTChar;
	int nIndex = AppName.ReverseFind(_T('\\'));
	if (nIndex > 0)
	{
		AppName = AppName.Left(nIndex);
		AppName += '\\';
	}
	return AppName;
}

void CGeneric::populatecombo(CString servicepipeline, CString sztemppath1)
{

	CString tmppath1;
	tmppath1 = sztemppath1;

	DeleteFile(tmppath1);
	CString szSvnPathFolder2, abc;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD dword = 0;

	HANDLE h = CreateFile(sztemppath1,
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);



	BOOL ret = FALSE;
	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;


	szSvnPathFolder2 = "svn ls --xml \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//";
	szSvnPathFolder2.Append(servicepipeline);

	szSvnPathFolder2.Append("\"");

	TCHAR cmd[260];
	strcpy(cmd, szSvnPathFolder2);
	dword = 0;
	dword = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	//waitfor
	CString szTempPath10;
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	szTempPath10 = twchPath;
	szTempPath10.Append("\servicepipeline.xml");
	m_xmlParser.XMLParser(szTempPath10, m_map2012SVCDEPInformation);
	//m_ServiceBasePipelineComboBox.ResetContent();
	WaitForSingleObject(pi.hProcess, INFINITE);
	int iUniqueKey = 0;
	int iKey = 0;
	SetPopulateCombo(iUniqueKey, iKey);
	CloseHandle(h);
}
void CGeneric::SetPopulateCombo(int iUniqueKey, int iKey)
{
	for (std::map<int, CString>::iterator iter = m_map2012SVCDEPInformation.begin(); iter != m_map2012SVCDEPInformation.end(); iter++)
	{
		m_mapPipelineInformation[iKey] = m_map2012SVCDEPInformation[iUniqueKey];
		iUniqueKey++;
		iKey++;
	}
}

std::map<int, CString> CGeneric::GetPopulateCombo()
{
	return m_mapPipelineInformation;
}

