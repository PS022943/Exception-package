
// MFCExceptionPackageBuilderDlg.cpp : implementation file

#include "stdafx.h"
#include "MFCExceptionPackageBuilder.h"
#include "MFCExceptionPackageBuilderDlg.h"
#include "afxdialogex.h"
#include "ManualPackageStatus.h"
#include "PackageComponentsDlg.h"
#include "Generic.h"
#include <string>
using namespace std;


#define SVNPath "C:\\Program Files\\TortoiseSVN\\bin\\svn.exe"
#define Package_document "\\\\cernerwhq1\\india\\IndiaHomeFolders\\PS022943\\Package_document" // move to ini file
#define TempPath "C:\\Users\\Administrator.MININT-R2735LU\\AppData\\Local\\Temp"
#define MSIPath "C:\\Users\\Administrator.MININT-R2735LU\\AppData\\Local\\Temp\\TortoiseSVN-1.9.3.27038-x64-svn-1.9.3.msi" 




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCExceptionPackageBuilderDlg dialog



CMFCExceptionPackageBuilderDlg::CMFCExceptionPackageBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCExceptionPackageBuilderDlg::IDD, pParent)
	, m_Uname(_T(""))
	, m_Password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCExceptionPackageBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGIN_DETAILS_STATIC, m_GroupBoxControl);
	DDX_Control(pDX, IDC_USERNAME_EDIT, m_UsernameEdit);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_Uname);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_Password);
}

BEGIN_MESSAGE_MAP(CMFCExceptionPackageBuilderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_PASSWORD_EDIT, &CMFCExceptionPackageBuilderDlg::OnEnChangePasswordEdit)
	ON_EN_CHANGE(IDC_USERNAME_EDIT, &CMFCExceptionPackageBuilderDlg::OnEnChangeUsernameEdit)
	ON_BN_CLICKED(IDOK, &CMFCExceptionPackageBuilderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCExceptionPackageBuilderDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_InstallRadio, &CMFCExceptionPackageBuilderDlg::OnBnClickedInstaller)
	ON_BN_CLICKED(IDC_VerifyRadio, &CMFCExceptionPackageBuilderDlg::OnBnClickedVerifier)
END_MESSAGE_MAP()


// CMFCExceptionPackageBuilderDlg message handlers

DWORD CMFCExceptionPackageBuilderDlg::GetApplicationDirectory(CString& strAppDir)
{
	DWORD dwRet = NO_ERROR;
	
	// retrieve the full path of the executable file of the current process
	if (0 != ::GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH))
	{
		// remove the executable file name
		::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));
	}
	else
	{
		// see what's going wrong
		dwRet = ::GetLastError();
	}

	return dwRet;
}

BOOL CMFCExceptionPackageBuilderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//CRect rect;
	//GetWindowRect(&rect);
	//SetWindowPos(NULL, 0, 0, rect.Width()*0.82, rect.Height()*0.82, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CheckRadioButton(IDC_InstallRadio, IDC_VerifyRadio, IDC_InstallRadio);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText("Manual Exception Package Tool Login");
	
	CWnd* pWndReviewer = GetDlgItem(IDC_ReviewerRadio);
	CWnd* pWndVerifier = GetDlgItem(IDC_VerifierRadio);
	if (NULL != pWndReviewer && NULL != pWndVerifier)
	{
		pWndReviewer->ShowWindow(SW_HIDE);
		pWndVerifier->ShowWindow(SW_HIDE);
	}
	
//	CString strApplicationDir;
//	GetApplicationDirectory(strApplicationDir);
//	strApplicationDir.Append("\\servicepipeline.xml");
	
	m_UsernameEdit.SetFocus();
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCExceptionPackageBuilderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCExceptionPackageBuilderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HBRUSH CMFCExceptionPackageBuilderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
	{
		pDC->SetBkMode(RGB(255, 255, 255));
	}
	default:
		return 	hbr = m_brush;
	}
	return m_brush;
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCExceptionPackageBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCExceptionPackageBuilderDlg::OnEnChangePasswordEdit()
{
	m_PasswordEdit.GetWindowText(m_PasswordString);
}


void CMFCExceptionPackageBuilderDlg::OnEnChangeUsernameEdit()
{
	m_UsernameEdit.GetWindowText(m_UserNameString);
}


void CMFCExceptionPackageBuilderDlg::OnBnClickedOk()
{
	if ((m_UserNameString.IsEmpty()) || (m_PasswordString.IsEmpty()))
	{
		AfxMessageBox(_T("Username and Password is mandatory"));
	}
	else if (LoginAndMappedToRemoteDirectory())
	{
		if (PathFileExists(SVNPath))
		{
			ShowWindow(SW_HIDE);
			int id;
			id = GetCheckedRadioButton(IDC_InstallRadio, IDC_VerifyRadio);
			if (id == 1020)
			{
				CString strCloseRunningApps = _T("Before starting the process, please close all the running MILLENIUM application and processes. \n\nNote: If any MILLENIUM applications are running, the setup will not be completed. \n\n\nClick 'YES' if you choose to continue with the installtion process. \nClick 'NO' to quit the application");
				int msg_result;
				msg_result = AfxMessageBox(strCloseRunningApps, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
				if (msg_result == IDNO)
				{
					AfxMessageBox(_T("The application will close now."));
					CDialog::EndDialog(0); 
					return;
				}
				
				CManualPackageStatus dlgManualPackage(NULL);
				dlgManualPackage.DoModal();

			}
			else
			{

				int id = 0;

				id = GetCheckedRadioButton(IDC_ReviewerRadio, IDC_VerifierRadio);
				if (id == 1051)
				{
					int ireviewflag = 0;
					CPackageComponentsDlg packagecomponentobj;
					packagecomponentobj.flag2 = ireviewflag;
					packagecomponentobj.DoModal();
				}
				if (id == 1052)
				{
					int ireviewflag = 1;
					CPackageComponentsDlg packagecomponentobj;
					packagecomponentobj.flag2 = ireviewflag;
					packagecomponentobj.DoModal();
				}

			}
		}
		else
		{
			CString svn_msg = _T("SVN Command Line Client tool is not present.\n\nPlease click Yes to proceed installing SVN and No to install manually.");
			int msg_result;
			msg_result = AfxMessageBox(svn_msg,MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
			if (msg_result == IDYES)
			{
				AfxMessageBox(_T("Please select the SVN Command Line tool option while installing."));
				DWORD dExitCode = 0;
				CString svn_path = _T("robocopy "Package_document" "TempPath" \"TortoiseSVN-1.9.3.27038-x64-svn-1.9.3.msi\" ");
				//CString p = _T("robocopy \"\\\\cernerwhq1\\india\\IndiaHomeFolders\\GN046414\" \"C:\\Program Files\\SVN\" \"TortoiseSVN-1.9.3.27038-x64-svn-1.9.3.msi\" ");
				ExecuteCommandLineParameter(svn_path, dExitCode);
				ShellExecute(NULL, "open",MSIPath, NULL, NULL, SW_SHOWNORMAL);
			}
			else
			{
				AfxMessageBox(_T("The application will close now."));
				CDialog::EndDialog(0);
			}
				
			
		}

	}
}


void CMFCExceptionPackageBuilderDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

bool CMFCExceptionPackageBuilderDlg::LoginAndMappedToRemoteDirectory()
{
	TCHAR  szCmdPath[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	DWORD dExitcode = 0;

	// Get and display the system directory.
	GetSystemDirectory(szCmdPath, INFO_BUFFER_SIZE);
	CString szRemoteDirectoryPath;
	

	szRemoteDirectoryPath = _T("");
	szRemoteDirectoryPath.Append(szCmdPath);
	szRemoteDirectoryPath.Append(_T("\\net.exe"));
	szRemoteDirectoryPath.Append(" ");
	szRemoteDirectoryPath.Append(_T("use * /delete /yes"));
	ExecuteCommandLineParameter(szRemoteDirectoryPath, dExitcode);
	if (dExitcode > 0)
	{
		AfxMessageBox(_T("Unable to delete all mapped drive"));
		return false;
	}
	
	szRemoteDirectoryPath = _T("");
	szRemoteDirectoryPath.Append(szCmdPath);
	szRemoteDirectoryPath.Append(_T("\\net.exe"));
	szRemoteDirectoryPath.Append(_T(" "));
	szRemoteDirectoryPath.Append(_T("use Z: \\\\cernerwhq1\\ipfactory_internal"));
	szRemoteDirectoryPath.Append(_T(" "));
	szRemoteDirectoryPath.Append(m_PasswordString);
	szRemoteDirectoryPath.Append(_T(" "));
	szRemoteDirectoryPath.Append(_T("/user:whq_nt_domain\\"));
	szRemoteDirectoryPath.Append(m_UserNameString);
	szRemoteDirectoryPath.Append(_T(" "));
	szRemoteDirectoryPath.Append(_T("/persistent:no"));
	ExecuteCommandLineParameter(szRemoteDirectoryPath, dExitcode);
	if (dExitcode > 0)
	{
		AfxMessageBox(_T("Unable to login, Please enter your correct username or password"));
		return false;
	}
	else
	{
		CString szpackagecomponent;
		DWORD dExitCode;
	
		CString szApplicationPath;
		GetApplicationDirectory(szApplicationPath);
		szpackagecomponent = szApplicationPath;
		szpackagecomponent.Append(_T("\\Package_Components.xml"));
		
		CString szCommandLineName = CommandName(_T("del"), szpackagecomponent, _T(""));
		dExitCode = 0;
		ExecuteCommandLineParameter(szCommandLineName, dExitCode);

		CString scopypath = Package_document;
		scopypath.Append(_T(" "));
		scopypath.Append(szApplicationPath);
		scopypath.Append(_T(""));

		szCommandLineName = CommandName(_T("robocopy "), scopypath, _T("Package_Components.xml"));
		dExitCode = 0;
		ExecuteCommandLineParameter(szCommandLineName, dExitCode);

		//if correct login grant access to svn
		UpdateData();
		CString szSvnaccess;
		szSvnaccess.Append(szCmdPath);
		szSvnaccess.Append(_T("svn co --username="));
		szSvnaccess.Append(m_Uname);
		szSvnaccess.Append(_T(" "));
		szSvnaccess.Append(_T("--password="));
		szSvnaccess.Append(m_Password);
		szSvnaccess.Append(_T(" "));
		szSvnaccess.Append(_T("http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//"));
		ExecuteCommandLineParameter(szSvnaccess, dExitcode);

		return true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Executes the given command using CreateProcess() and WaitForSingleObject().
// Returns FALSE if the command could not be executed or if the exit code could not be determined.
BOOL CMFCExceptionPackageBuilderDlg::ExecuteCommandLineParameter(CString cmdLine, DWORD & exitCode)
{
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = cmdLine.GetLength() + 50;


	// Create the process
	BOOL bResult = CreateProcess(NULL, cmdLine.GetBuffer(nStrBuffer),
		NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
		NULL, NULL, &startupInfo, &processInformation);
	cmdLine.ReleaseBuffer();


	if (!bResult)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		// Display the error
		CString strError = (LPTSTR)lpMsgBuf;
		TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

		// Free resources created by the system
		LocalFree(lpMsgBuf);

		// We failed.
		return FALSE;
	}
	else
	{
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		// Get the exit code.
		bResult = GetExitCodeProcess(processInformation.hProcess, &exitCode);

		// Close the handles.
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (!bResult)
		{
			// Could not get exit code.
			TRACE(_T("Executed command but couldn't get exit code.\nCommand=%s\n"), cmdLine);
			return FALSE;
		}
		// We succeeded.
		return TRUE;
	}
}


void CMFCExceptionPackageBuilderDlg::DeleteRequiredFolders(CString szInputCommand, CString sFolderPath, CString sCommandName)
{
	TCHAR  szCmdPath[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the system directory.
	GetSystemDirectory(szCmdPath, INFO_BUFFER_SIZE);
	
	if (PathAppend(szCmdPath, TEXT("cmd.exe")))
	{
		// Error
	}

	CString  szCmdlinePath;
	szCmdlinePath.Append(szCmdPath);
	szCmdlinePath.Append(" ");
	szCmdlinePath.Append("/c");
	szCmdlinePath.Append(" ");
	szCmdlinePath.Append(szInputCommand);
	if (!sFolderPath.IsEmpty())
	{
		szCmdlinePath.Append(" ");
		szCmdlinePath.Append(sFolderPath);
	}
	
	if (!sCommandName.IsEmpty())
	{
		szCmdlinePath.Append(" ");
		szCmdlinePath.Append(sCommandName);
	}


	STARTUPINFO startInf;
	memset(&startInf, 0, sizeof(startInf));
	startInf.cb = sizeof(startInf);
	
	PROCESS_INFORMATION procInf;
	memset(&procInf, 0, sizeof(procInf));
	BOOL bSuccess = CreateProcess(NULL, szCmdlinePath.GetBuffer(), NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startInf, &procInf);
	DWORD dwErr = 0;
	if (bSuccess) 
	{
		WaitForSingleObject(procInf.hProcess, INFINITE);
		GetExitCodeProcess(procInf.hProcess, &dwErr);
		CloseHandle(procInf.hProcess);
	}
	else 
	{
		dwErr = GetLastError();
	}
}

CString CMFCExceptionPackageBuilderDlg::CommandName(CString szInputCommand, CString sFolderPath, CString sCommandName)
{
	TCHAR  szCmdPath[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the system directory.
	GetSystemDirectory(szCmdPath, INFO_BUFFER_SIZE);

	if (PathAppend(szCmdPath, TEXT("cmd.exe")))
	{
		// Error
	}

	CString  szCmdlinePath;
	szCmdlinePath.Append(szCmdPath);
	szCmdlinePath.Append(" ");
	szCmdlinePath.Append("/c");
	szCmdlinePath.Append(" ");
	szCmdlinePath.Append(szInputCommand);
	if (!sFolderPath.IsEmpty())
	{
		szCmdlinePath.Append(" ");
		szCmdlinePath.Append(sFolderPath);
	}

	if (!sCommandName.IsEmpty())
	{
		szCmdlinePath.Append(" ");
		szCmdlinePath.Append(sCommandName);
	}
	return szCmdlinePath;
}

void CMFCExceptionPackageBuilderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}


void CMFCExceptionPackageBuilderDlg::OnBnClickedInstaller()
{
	GetDlgItem(IDC_ReviewerRadio)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VerifierRadio)->ShowWindow(SW_HIDE);
}


void CMFCExceptionPackageBuilderDlg::OnBnClickedVerifier()
{
	GetDlgItem(IDC_ReviewerRadio)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_VerifierRadio)->ShowWindow(SW_SHOW);
	CheckRadioButton(IDC_ReviewerRadio, IDC_VerifierRadio,IDC_ReviewerRadio);

}
