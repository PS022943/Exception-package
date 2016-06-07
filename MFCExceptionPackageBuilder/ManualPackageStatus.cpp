// ManualPackageStatus.cpp : implementation file

#include "stdafx.h"
#include "MFCExceptionPackageBuilder.h"
#include "ManualPackageStatus.h"
#include "MFCExceptionPackageBuilderDlg.h"
#include "Generic.h"
#include "afxdialogex.h"
#include <sys/stat.h>
#include "resource.h"
#include <io.h>
#include <string>
#include <wchar.h>
#include <array>
#include "ColorListBox.h"

int CManualPackageStatus::m_iProgressBar = 0;
int CManualPackageStatus::m_iShowCompleteStatus = 0;
CCriticalSection c_s;



#define WM_USER_MESSAGE (WM_USER + 0x100)
#define WM_USER_THREAD_UPDATE_PROGRESSBAR (WM_USER+0x101)
#define WM_USER_THREAD_EXECUTE_PROGRESS (WM_USER+0x102)
#define WM_USER_UPDATE_UI_MESSAGE (WM_USER+0x103)
#define PROGRESSBAR_INCREMENTCOUNTER  2.63
#define szEcosystem "C:\\Ecosystem"
#define szRefein32 "P:\\win32"
#define szRefxtremetoolkit "P:\\include_ots/"
#define szRefinclude "P:\\include"
#define szReflib "P:\\lib"
#define szRefots "P:\\include_ots"
#define szCode "C:\\Code"
#define szpgmfilescerner "C:\\Program Files\\Cerner"
#define szpgmfilesgkcerner "C:\\Program Files\\GKCerner"
//#define INTERMEDIATE_FILES_PATH "Z:\\Intermediate_Files\\"
#define INTERMEDIATE_FILES_PATH "\\\\cernerwhq1\\IPFactory_Internal\\Intermediate_Files"
#define szsvnpath "svn info \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//"
#define szsvnpath1 "svn checkout \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//"
#define szRegistercom "\\\\cernerwhq1\\india\\IndiaHomeFolders\\PS022943\\Package_document\\registerdll.bat"
#define lib "C:\\Code\\CPP\\"
IMPLEMENT_DYNAMIC(CManualPackageStatus, CDialogEx)


CManualPackageStatus::CManualPackageStatus(CWnd* pParent /*=NULL*/)
: CDialogEx(CManualPackageStatus::IDD, pParent)

{
	m_nDropListIndex1 = 0;
	m_nDropListIndex2 = 0;
	if (NULL != AfxGetApp())
		m_Icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CManualPackageStatus::~CManualPackageStatus()
{
}

void CManualPackageStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MANUALPACKAGE_PROGRESS, m_ManualPackageProgress);
	DDX_Control(pDX, IDC_COMBO_SERVICE_PACKAGE, m_ServicepackageComboBox);//(pointer to data exchange,resource id of control to be subclassed,reference of dialogbox,formview etc)
	DDX_Control(pDX, IDC_COMBO_SERVICE_PIPELINE, m_ServiceBasePipelineComboBox);
	DDX_CBIndex(pDX, IDC_COMBO_SERVICE_PIPELINE, m_nDropListIndex1);
	DDX_CBIndex(pDX, IDC_COMBO_SERVICE_PACKAGE, m_nDropListIndex2);
	DDX_Control(pDX, IDC_STATUS, m_displaystatus);
	DDX_Control(pDX, IDC_BUTTON1, m_imgbtn);
}

BEGIN_MESSAGE_MAP(CManualPackageStatus, CDialogEx)//(the class,base class)
	ON_BN_CLICKED(IDCLOSE, &CManualPackageStatus::OnBnClickedClose)//(id,member function)
	ON_BN_CLICKED(ID_START_PACKAGE_PROCESSS, &CManualPackageStatus::OnBnClickedStartPackageProcesss)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_USER_THREAD_UPDATE_PROGRESSBAR, OnThreadUpdateProgressBar)
	ON_MESSAGE(WM_USER_UPDATE_UI_MESSAGE, OnThreadManualPackageThread)
	ON_MESSAGE(WM_USER_MESSAGE, DisplayProgressStatusOnListBox)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVICE_PIPELINE, &CManualPackageStatus::OnSelchangeComboServicePipeline)//(id,member function)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVICE_PACKAGE, &CManualPackageStatus::OnSelchangeComboServicePackage)
	ON_EN_CHANGE(IDC_EDIT_PACKAGE, &CManualPackageStatus::OnChangeEditPackage)
	ON_BN_CLICKED(IDC_VERIFY_FOLDERS_BTN, &CManualPackageStatus::OnBnClickedVerifyFoldersBtn)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REGISTER_COMPONENTS, &CManualPackageStatus::OnBnClickedRegisterComponents)
END_MESSAGE_MAP()


UINT CManualPackageStatus::CopyingIncludeAndIncludeOTS(LPVOID lParam)
{
	THREADINFOSTRUCT* pManualPackageThreadlParam = (THREADINFOSTRUCT*)lParam;
	CString szManualPackageServicePackage = pManualPackageThreadlParam->szManualPackageServicePackage;
	CString szPipelinePackage = pManualPackageThreadlParam->szServicePipeline;
	CString szManualPackage = pManualPackageThreadlParam->szManualPackage;
	CString szCommandLineName;
	DWORD dExitCode = 0;
	
	_stListBoxTextInfo* listboxInfostruct = new _stListBoxTextInfo;
	
	//::----------------------------------------------------::
	//::Copy XtremeToolkit To P : \include_ots\XtremeToolkit ::
	//::----------------------------------------------------::
	CString szXtremeToolKit = "\"P:\\include_ots\"";
	CString szOtsLib = "\"P:\\lib\"";

	CString szXtremeToolKitFilesFolder = "\"";
	szXtremeToolKitFilesFolder.Append(INTERMEDIATE_FILES_PATH);
	szXtremeToolKitFilesFolder.Append("\\");
	szXtremeToolKitFilesFolder.Append(szManualPackageServicePackage);
	szXtremeToolKitFilesFolder.Append("\\include_ots\"");
	szXtremeToolKitFilesFolder.Append(" ");
	szXtremeToolKitFilesFolder.Append(szXtremeToolKit);
	szXtremeToolKitFilesFolder.Append(" ");
	szXtremeToolKitFilesFolder.Append("*.*");
	szXtremeToolKitFilesFolder.Append(" ");
	szXtremeToolKitFilesFolder.Append("/e");
	szXtremeToolKitFilesFolder.Append(" ");
	szXtremeToolKitFilesFolder.Append("/MT");

	szCommandLineName = CommandLineName("robocopy", szXtremeToolKitFilesFolder, "");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3) || dExitCode == 16)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed:could not copy every file from include_ots for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every file from include_ots for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 1, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	
	++CManualPackageStatus::m_iShowCompleteStatus;

	if (CManualPackageStatus::m_iShowCompleteStatus == 4)
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Manual packaging process completed";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_UPDATE_UI_MESSAGE, 0, 0);
	}

	return 0;
}

UINT CManualPackageStatus::CopyingApplicationExtension(LPVOID lParam)
{
	THREADINFOSTRUCT* pManualPackageThreadlParam = (THREADINFOSTRUCT*)lParam;
	CString szManualPackageServicePackage = pManualPackageThreadlParam->szManualPackageServicePackage;
	CString szPipelinePackage = pManualPackageThreadlParam->szServicePipeline;
	CString szManualPackage = pManualPackageThreadlParam->szManualPackage;
	CString szCommandLineName;
	DWORD dExitCode = 0;
	CString szLibraryFilesFolder2 = "\"";
	szLibraryFilesFolder2.Append(INTERMEDIATE_FILES_PATH);
	szLibraryFilesFolder2.Append("\\");
	szLibraryFilesFolder2.Append(szManualPackageServicePackage);
	szLibraryFilesFolder2.Append("\\winintel\"");
	szLibraryFilesFolder2.Append(" ");
	szLibraryFilesFolder2.Append("\"");
	szLibraryFilesFolder2.Append(szpgmfilesgkcerner);
	szLibraryFilesFolder2.Append("\"");
	szLibraryFilesFolder2.Append(" ");

	_stListBoxTextInfo* listboxInfostruct = new _stListBoxTextInfo;
	szCommandLineName = CommandLineName("robocopy", szLibraryFilesFolder2, "*.dll");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not copy every dll file from Winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every dll file from Winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 2, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	CString szLibraryFilesFolder3 = "\"";
	szLibraryFilesFolder3.Append(INTERMEDIATE_FILES_PATH);
	szLibraryFilesFolder2.Append("\\");
	szLibraryFilesFolder3.Append(szManualPackageServicePackage);
	szLibraryFilesFolder3.Append("\\winintel\"");
	szLibraryFilesFolder3.Append(" ");
	szLibraryFilesFolder3.Append("\"");
	szLibraryFilesFolder3.Append(szpgmfilesgkcerner);
	szLibraryFilesFolder3.Append("\"");
	szLibraryFilesFolder3.Append(" ");

	szCommandLineName = CommandLineName("robocopy", szLibraryFilesFolder3, "*.exe");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not copy every exe file from WinIntel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every exe file from WinIntel folder for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 3, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	//copy .ocx and .dll winintel to to c:/programfiles/cerner
	CString szLibraryFilesFolder1 = "\"";
	szLibraryFilesFolder1.Append(INTERMEDIATE_FILES_PATH);
	szLibraryFilesFolder2.Append("\\");
	szLibraryFilesFolder1.Append(szManualPackageServicePackage);
	szLibraryFilesFolder1.Append("\\winintel\"");
	szLibraryFilesFolder1.Append(" ");
	szLibraryFilesFolder1.Append("\"");
	szLibraryFilesFolder1.Append(szpgmfilesgkcerner);
	szLibraryFilesFolder1.Append("\"");
	szLibraryFilesFolder1.Append(" ");

	szCommandLineName = CommandLineName("robocopy", szLibraryFilesFolder1, "*.ocx");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Copying every ocx file from winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copying every ocx file from winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 4, 100);
		++CManualPackageStatus::m_iProgressBar;
	}


	//Registering the dll and ocx components
	//dExitCode = 0;
	//ExecuteCommandLineParameter(szRegistercom, dExitCode);

	//if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	//{
	//	listboxInfostruct->bFlag = false;
	//	listboxInfostruct->strDisplayMessage = "Completed : Registering required dll and ocx components";
	//	::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));

	//	return 0;
	//}
	//else
	//{
	//	listboxInfostruct->bFlag = true;
	//	listboxInfostruct->strDisplayMessage = "Completed : Registering required dll and ocx components";
	//	::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
	//	::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 5, 100);
	//	++CManualPackageStatus::m_iProgressBar;

	//}

	++CManualPackageStatus::m_iShowCompleteStatus;

	if (CManualPackageStatus::m_iShowCompleteStatus == 4)
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Manual packaging process completed";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_UPDATE_UI_MESSAGE, 0, 0);
	}

	return 0;
}

// CManualPackageStatus message handlers
UINT CManualPackageStatus::ManualPackageStatusThreadFunc(LPVOID lParam)
{
	THREADINFOSTRUCT* pManualPackageThreadlParam = (THREADINFOSTRUCT*)lParam;
	CString szManualPackageServicePackage = pManualPackageThreadlParam->szManualPackageServicePackage;
	CString szPipelinePackage = pManualPackageThreadlParam->szServicePipeline;
	CString szManualPackage = pManualPackageThreadlParam->szManualPackage;
	CString szCommandLineName;

	DWORD dExitCode = 0;
	_stListBoxTextInfo* listboxInfostruct = new _stListBoxTextInfo;

	//::-------------------------------------------------- - ::
	//	::Copy Required Cerner Millennium Files To P : \win32 ::
	//	::-------------------------------------------------- - ::
	//CString szRequiredExecutableProgramFilesFolder = "\"C:\\Program Files\\Cerner\" \"P:\\win32\" ";
	CString szRequiredExecutableProgramFilesFolder;
	szRequiredExecutableProgramFilesFolder.Append("\"");
	szRequiredExecutableProgramFilesFolder.Append(szpgmfilescerner);
	szRequiredExecutableProgramFilesFolder.Append("\"");
	szRequiredExecutableProgramFilesFolder.Append(" ");
	szRequiredExecutableProgramFilesFolder.Append("\"");
	szRequiredExecutableProgramFilesFolder.Append(szRefein32);
	szRequiredExecutableProgramFilesFolder.Append("\"");

	szCommandLineName = CommandLineName("robocopy ", szRequiredExecutableProgramFilesFolder, "Powerchart.exe");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed:could not copy powerchart";

		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	szCommandLineName = CommandLineName("robocopy", szRequiredExecutableProgramFilesFolder, "Multum.SDK.PlugIn.Interface.Net.dll");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed: Could not copy Multum.SDK.PlugIn.Interface.Net.dll ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	szCommandLineName = CommandLineName("robocopy", szRequiredExecutableProgramFilesFolder, "Multum.SDK.PlugIn.Utility.dll");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed:could not copy Multum.SDK.PlugIn.Utility.dll ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	szCommandLineName = CommandLineName("robocopy", szRequiredExecutableProgramFilesFolder, "Multum.SDK.Data.xml");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)//dExitCode=1
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed:could not copy Multum.SDK.Data.xml ";

		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	szCommandLineName = CommandLineName("robocopy", szRequiredExecutableProgramFilesFolder, "pvorderdlg.net.dll");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not copy pvorderdlg.net.dll ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	/*************************************************************************************************************************************************/

	//delete previously existing CPP,CSharp and INCLUDE folders and recreate them to get the latest files and folders from svn

	szCommandLineName = CommandLineName("del", szpgmfilescerner, "diagnosisassistant.tlb");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Deleted diagnosisassistant.tlb";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	
	CString szReleasePipeline = szManualPackageServicePackage;
	CString szLine = szReleasePipeline.Left(4);
	dExitCode = 0;

	//check if vc10 folder is present
	std::map<int, CString> szsvnpathmap;
	CString szTargetep, sztargetep, szTargetepPath, szTargetep1, szcpp, szvc10 = "";
	int def = 0;
	int vc10present;
	CString wchPath = CGeneric::GetExeFolderPath();
	szcpp = wchPath;
	szTargetepPath = wchPath;
	szcpp.Append("\\CPP.xml");
	szTargetepPath.Append("\\Targetep.xml");
	
	szCommandLineName = CommandLineName("del", szTargetepPath, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	szCommandLineName = CommandLineName("del", szcpp, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	DWORD dexitcode;
	szCommandLineName = CommandLineName("rd / s / q", szCode, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)//dExitCode=0
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Unable to remove Code folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));

		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Removed Code folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
	}

	createxml(szTargetepPath, "", szPipelinePackage, szManualPackageServicePackage, def);
	vc10present = svnxml(szTargetepPath, szsvnpathmap);
	CString szsvnpathfolder, vc10folder;
	int vc10flag = 0;

	if (vc10present == 1)
	{
		szsvnpathfolder = "";
		szsvnpathfolder.Append(szsvnpath1);
		szsvnpathfolder.Append(szPipelinePackage);
		szsvnpathfolder.Append("//");
		szsvnpathfolder.Append(szManualPackageServicePackage);
		szsvnpathfolder.Append("//");
		szsvnpathfolder.Append("VC10");
		szsvnpathfolder.Append("\"");
		szsvnpathfolder.Append(" ");
		szsvnpathfolder.Append(szCode);
		szsvnpathfolder.Append(" ");
		szsvnpathfolder.Append("-q");
		ExecuteCommandLineParameter(szsvnpathfolder, dExitCode);
		cppxml(szTargetepPath, vc10folder, szPipelinePackage, szManualPackageServicePackage, def);
		if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)//dExitCode=0
		{
			++CManualPackageStatus::m_iShowCompleteStatus;
			listboxInfostruct->bFlag = true;
			listboxInfostruct->strDisplayMessage = "Failed : Copied contents to CODE folder";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));

			return 0;
		}
		else
		{
			listboxInfostruct->bFlag = true;
			listboxInfostruct->strDisplayMessage = "Completed : Copied contents to CODE folder";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 7, 100);
			++CManualPackageStatus::m_iProgressBar;
		}

	}

	if (vc10present == 0)
	{
		//copy CPP folder to code folder
		CString strFolderName("CPP");
		dexitcode = vc10absentfolder(szTargetepPath, szPipelinePackage, szManualPackageServicePackage, def, strFolderName);
		if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
		{
			++CManualPackageStatus::m_iShowCompleteStatus;
			listboxInfostruct->bFlag = false;
			listboxInfostruct->strDisplayMessage = "Failed : Copied contents to CPP";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
			return 0;
		}
		else
		{
			listboxInfostruct->bFlag = true;
			listboxInfostruct->strDisplayMessage = "Completed : Copied contents to CPP";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		}

		//copy CSharp folder to code folder
		strFolderName = "CSHARP";
		vc10absentfolder(szTargetepPath, szPipelinePackage, szManualPackageServicePackage, def, strFolderName);
		if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
		{
			++CManualPackageStatus::m_iShowCompleteStatus;
			listboxInfostruct->bFlag = false;
			listboxInfostruct->strDisplayMessage = "Failed : Copied contents to CSharp";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));

			return 0;
		}
		else
		{
			listboxInfostruct->bFlag = true;
			listboxInfostruct->strDisplayMessage = "Completed : Copied contents to CSharp";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		}

		//copy INCLUDE folder to code folder

		strFolderName = "INCLUDE";
		vc10absentfolder(szTargetepPath, szPipelinePackage, szManualPackageServicePackage, def, strFolderName);
		if (dExitCode < 0 && dExitCode > 3)
		{
			++CManualPackageStatus::m_iShowCompleteStatus;
			listboxInfostruct->bFlag = false;
			listboxInfostruct->strDisplayMessage = "Failed : Copied contents to INCLUDE";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
			return 0;
		}
		else
		{
			listboxInfostruct->bFlag = true;
			listboxInfostruct->strDisplayMessage = "Completed : Copied contents to INCLUDE";
			::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		}
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 7, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	szsvnpathfolder = "";
	szsvnpathfolder.Append(szsvnpath1);
	szsvnpathfolder.Append("Solution Files");
	szsvnpathfolder.Append("//");
	if (szLine.CompareNoCase("2015") == 0)
	{
		szsvnpathfolder.Append("2015.01_EP");
	}
	else if (szLine.CompareNoCase("2012") == 0)
	{
		szsvnpathfolder.Append("2012.01EP");
	}
	else if (szReleasePipeline.Find("STP") > 0 && szLine.CompareNoCase("2015") == 0)
	{
		szsvnpathfolder.Append("2015.01-STP_EP");
	}

	szsvnpathfolder.Append("//");
	szsvnpathfolder.Append("\"");
	szsvnpathfolder.Append(" ");
	szsvnpathfolder.Append(szCode);
	szsvnpathfolder.Append("//");
	szsvnpathfolder.Append("SolutionFile");
	szsvnpathfolder.Append(" ");
	szsvnpathfolder.Append("-q");
	ExecuteCommandLineParameter(szsvnpathfolder, dExitCode);

	
	//::----------------------------------::
	//::Copy Include Files To P : \include :: 
	//::----------------------------------::
	CString szIncludeFilesFolder = "\"";
	szIncludeFilesFolder.Append(INTERMEDIATE_FILES_PATH);
	szIncludeFilesFolder.Append("\\");
	szIncludeFilesFolder.Append(szManualPackageServicePackage);
	szIncludeFilesFolder.Append("\\include\\.");
	szIncludeFilesFolder.Append("\"");
	szIncludeFilesFolder.Append(" ");
	szIncludeFilesFolder.Append("\"");
	szIncludeFilesFolder.Append(szRefinclude);
	szIncludeFilesFolder.Append("\"");
	szIncludeFilesFolder.Append(" ");
	szIncludeFilesFolder.Append("/e");

	szCommandLineName = CommandLineName("robocopy", szIncludeFilesFolder, "");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3) /*|| dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Could not copy every file from include for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every file from include for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 6, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	++CManualPackageStatus::m_iShowCompleteStatus;

	if (CManualPackageStatus::m_iShowCompleteStatus == 4)
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Manual packaging process completed";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_UPDATE_UI_MESSAGE, 0, 0); 
	}

	//	PostMessage
	delete pManualPackageThreadlParam;
	pManualPackageThreadlParam = NULL;

	return 0;
}

int CManualPackageStatus::svnxml(CString sztemppath, std::map<int, CString> szsvnpathmap)
{
	CString szvc10version;
	std::map<int, CString> szsvnpathmap1;
	CXmlparserHelper b;
	b.XMLParser(sztemppath, szsvnpathmap1);
	int icount = 0, ivc10present = 0;
	for (std::map<int, CString>::iterator iter = szsvnpathmap1.begin(); iter != szsvnpathmap1.end(); iter++)
	{
		CString folder = szsvnpathmap1[icount];
		if (folder == "vc10")
		{
			ivc10present = 1;
			break;
		}

		if (folder == "VC10")
		{
			ivc10present = 1;
			break;
		}
		icount++;
	}
	return ivc10present;
}

BOOL CManualPackageStatus::createxml(CString path, CString folder, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword)
{
	CString szSvnPathFolder2, abc;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE h = CreateFile(path,
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;


	szSvnPathFolder2 = "svn ls --xml \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//";
	szSvnPathFolder2.Append(szPipelinePackage);
	szSvnPathFolder2.Append("//");
	szSvnPathFolder2.Append(szManualPackageServicePackage);
	szSvnPathFolder2.Append("//");
	szSvnPathFolder2.Append(folder);
	szSvnPathFolder2.Append("\"");

	TCHAR cmd[260];
	strcpy(cmd, szSvnPathFolder2);


	dword = 0;
	dword = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	return dword;
}


void CManualPackageStatus::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(0);
	//ASSERT(AfxGetApp()->m_pMainWnd != NULL);
	//AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
	
	if (AfxGetMainWnd() != NULL){
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

int CManualPackageStatus::cppxml(CString sztemppath, CString sztemp, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword)
{
	CString cppversion, cpppath;
	CString wchPath;
	wchPath = CGeneric::GetExeFolderPath();
	cpppath = wchPath;
	cpppath = wchPath;
	cpppath.Append("\CPP.xml");

	std::map<int, CString> szsvnpathmap1;
	CXmlparserHelper b;
	b.XMLParser(sztemppath, szsvnpathmap1);
	int icount = 0, cpppresent = 0;
	for (std::map<int, CString>::iterator iter = szsvnpathmap1.begin(); iter != szsvnpathmap1.end(); iter++)
	{
		CString folder = szsvnpathmap1[icount];
		if (folder == "CPP" || folder == "cpp")
		{
			CString path;
			path.Append(sztemp);
			path.Append("//");
			path.Append(folder);
			createxml(cpppath, path, szPipelinePackage, szManualPackageServicePackage, dword);
		}
		icount++;
	}
	return cpppresent;
}


int CManualPackageStatus::vc10absentfolder(CString sztemppath, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword, CString strFoldername)
{
	CString szfolder, szCommandLineName;
	DWORD dExitCode;
	szfolder = "";
	szfolder.Append(szCode);
	szfolder.Append("\\");
	szfolder.Append(strFoldername);
	szCommandLineName = CommandLineName("md", szfolder, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	CString szSvnPathFolder1;
	szSvnPathFolder1.Append(szsvnpath1);
	szSvnPathFolder1.Append(szPipelinePackage);
	szSvnPathFolder1.Append("//");
	szSvnPathFolder1.Append(szManualPackageServicePackage);
	szSvnPathFolder1.Append("//");
	szSvnPathFolder1.Append(strFoldername);
	szSvnPathFolder1.Append("\"");
	szSvnPathFolder1.Append(" ");
	szSvnPathFolder1.Append(szCode);
	szSvnPathFolder1.Append("\\");
	szSvnPathFolder1.Append(strFoldername);
	szSvnPathFolder1.Append(" ");
	szSvnPathFolder1.Append("-q");
	szSvnPathFolder1.Append("\"");
	ExecuteCommandLineParameter(szSvnPathFolder1, dExitCode);
	if (dExitCode < 0 && dExitCode > 3)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		return 0;
	}
			
	if (strFoldername == "CPP")
	{//create a CPP.xml file
		CString abc, folder;
		DWORD def;
		def = 0;
		folder = strFoldername;
		CString szTempPath, szTempPath1, szCommandLineName;
		CString wchPath;
		wchPath = CGeneric::GetExeFolderPath();
		szTempPath = wchPath;
		szTempPath1 = wchPath;
		szTempPath1.Append("\\CPP.xml");
		CString b = szTempPath1;
		def = createxml(b, folder, szPipelinePackage, szManualPackageServicePackage, def);
	}

	return 0;
}

UINT CManualPackageStatus::CopyingSolutionFile(LPVOID lParam)
{
	THREADINFOSTRUCT* pManualPackageThreadlParam = (THREADINFOSTRUCT*)lParam;
	CString szManualPackageServicePackage = pManualPackageThreadlParam->szManualPackageServicePackage;
	CString szPipelinePackage = pManualPackageThreadlParam->szServicePipeline;
	CString szManualPackage = pManualPackageThreadlParam->szManualPackage;
	CString szCommandLineName;
	_stListBoxTextInfo* listboxInfostruct = new _stListBoxTextInfo;
	DWORD dExitCode = 0;

	Sleep(1000);
	//::-------------------------------------- - ::
	//::Copy Library Files To P : \lib          ::
	//::-------------------------------------- - ::
	CString szOtsLib = "\"P:\\lib\"";
	CString szLibraryFilesFolder = "\"";
	szLibraryFilesFolder.Append(INTERMEDIATE_FILES_PATH);
	szLibraryFilesFolder.Append("\\");
	szLibraryFilesFolder.Append(szManualPackageServicePackage);
	szLibraryFilesFolder.Append("\\winintel\\lib\"");
	szLibraryFilesFolder.Append(" ");
	szLibraryFilesFolder.Append("\"");
	szLibraryFilesFolder.Append(szReflib);
	szLibraryFilesFolder.Append("\"");
	szLibraryFilesFolder.Append(" ");

	szCommandLineName = CommandLineName("robocopy", szLibraryFilesFolder, "*.*");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if (dExitCode < 0 && dExitCode > 3)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not copy every file from lib (WinIntel) for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every file from lib (WinIntel) for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 10, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	
	szOtsLib = "\"P:\\lib\"";
	CString szWinInTelOtsFilesFolder = "\"";
	szWinInTelOtsFilesFolder.Append(INTERMEDIATE_FILES_PATH);
	szWinInTelOtsFilesFolder.Append("\\");
	szWinInTelOtsFilesFolder.Append(szManualPackageServicePackage);
	szWinInTelOtsFilesFolder.Append("\\winintel\\ots\"");
	szWinInTelOtsFilesFolder.Append(" ");
	szWinInTelOtsFilesFolder.Append(szOtsLib);
	szWinInTelOtsFilesFolder.Append(" ");
	szWinInTelOtsFilesFolder.Append("*.*");
	szWinInTelOtsFilesFolder.Append(" ");
	szWinInTelOtsFilesFolder.Append("/e");

	szCommandLineName = CommandLineName("robocopy", szWinInTelOtsFilesFolder, "");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if (dExitCode < 0 && dExitCode > 3)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not copy every  file from (OTS)Winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return 0;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Completed : Copied every  file from (OTS)Winintel for selected package ";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 11, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	++CManualPackageStatus::m_iShowCompleteStatus;

	if (CManualPackageStatus::m_iShowCompleteStatus == 4)
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Manual packaging process completed";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_UPDATE_UI_MESSAGE, 0, 0);
	}

	return 0;
}


void CManualPackageStatus::OnBnClickedStartPackageProcesss()
{
	THREADINFOSTRUCT *pManualPackageThreadInformation = new THREADINFOSTRUCT;
	pManualPackageThreadInformation->hWnd = m_hWnd;
	pManualPackageThreadInformation->szManualPackageServicePackage = m_szServicepackage;
	pManualPackageThreadInformation->szServicePipeline = m_szServiceBasePipeline;
	pManualPackageThreadInformation->szManualPackage = m_szManualpackage;

	CWnd* pBasePipelineWnd = GetDlgItem(IDC_COMBO_SERVICE_PIPELINE);
	if (NULL != pBasePipelineWnd)
	{
		::EnableWindow(pBasePipelineWnd->m_hWnd, FALSE);
	}

	CWnd* pServicePipelineWnd = GetDlgItem(IDC_COMBO_SERVICE_PACKAGE);
	if (NULL != pServicePipelineWnd)
	{
		::EnableWindow(pServicePipelineWnd->m_hWnd, FALSE);
	}

	CWnd* pVerfiyDlgWnd = GetDlgItem(IDC_VERIFY_FOLDERS_BTN);
	if (NULL != pVerfiyDlgWnd)
	{
		::EnableWindow(pVerfiyDlgWnd->m_hWnd, FALSE);
	}

	CWnd* pCloseBtnWnd = GetDlgItem(IDCLOSE);
	if (NULL != pCloseBtnWnd)
	{
		::EnableWindow(pCloseBtnWnd->m_hWnd, FALSE);
	}

	CWnd* pStartProcessBtnWnd = GetDlgItem(ID_START_PACKAGE_PROCESSS);
	if (NULL != pStartProcessBtnWnd)
	{
		::EnableWindow(pStartProcessBtnWnd->m_hWnd, FALSE);
	}

	CWnd* pRegisterComponentsBtnWnd = GetDlgItem(IDC_REGISTER_COMPONENTS);
	if (NULL != pRegisterComponentsBtnWnd)
	{
		::EnableWindow(pRegisterComponentsBtnWnd->m_hWnd, FALSE);
	}

	if (!DeleteRecreatePDriveFolders(pManualPackageThreadInformation))
	{
		MessageBox("Manual package process Stopped. \n\nUnable to created the basic folders structure.", "Process status");
		return;
	}

	CWinThread* pThread = AfxBeginThread(ManualPackageStatusThreadFunc, pManualPackageThreadInformation, THREAD_PRIORITY_NORMAL, 0, 0);

	CWinThread* pThread1 = AfxBeginThread(CopyingApplicationExtension, pManualPackageThreadInformation, THREAD_PRIORITY_NORMAL, 0, 0);

	CWinThread* pThread2 = AfxBeginThread(CopyingIncludeAndIncludeOTS, pManualPackageThreadInformation, THREAD_PRIORITY_NORMAL, 0, 0);

	CWinThread* pThread3 = AfxBeginThread(CopyingSolutionFile, pManualPackageThreadInformation, THREAD_PRIORITY_NORMAL, 0, 0);

	
}

bool CManualPackageStatus::DeleteRecreatePDriveFolders(THREADINFOSTRUCT *pManualPackageThreadInformation)
{

	THREADINFOSTRUCT *pManualPackageThreadlParam = pManualPackageThreadInformation;
	CString szCommandLineName;

	DWORD dExitCode = 0;

	//removing reference drive
	_stListBoxTextInfo* listboxInfostruct = new _stListBoxTextInfo;

	szCommandLineName = CommandLineName("subst P: /d", "", "");
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Unable to remove reference directories";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}
	else
	{
		listboxInfostruct->bFlag = true;
		listboxInfostruct->strDisplayMessage = "Manual package process started.";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_THREAD_UPDATE_PROGRESSBAR, 12, 100);
		++CManualPackageStatus::m_iProgressBar;
	}

	//removing Ecosystem folder
	szCommandLineName = CommandLineName("rd / s / q", szEcosystem, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Unable to delete folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}


	//Creating Ecosystem folder
	szCommandLineName = CommandLineName("md", szEcosystem, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed: Unable to create ecosystem folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}

	//Making P drive reference 	C:\\Ecosystem
	szCommandLineName = CommandLineName("subst P:", szEcosystem, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		return false;
	}

	
	//::----------------------::
	//	::Make P Drive Folders ::
	//	::----------------------::
	//Creating win32 Folder
	szCommandLineName = CommandLineName("md", szRefein32, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : Could not create win32 folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}

	//Creating include Folder
	szCommandLineName = CommandLineName("md", szRefinclude, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not create include folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}

	//Creating include_ots Folder
	szCommandLineName = CommandLineName("md", szRefots, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not create include_ots folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}

	//Creating lib Folder
	szCommandLineName = CommandLineName("md", szReflib, "");
	dExitCode = 0;
	ExecuteCommandLineParameter(szCommandLineName, dExitCode);

	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		++CManualPackageStatus::m_iShowCompleteStatus;
		listboxInfostruct->bFlag = false;
		listboxInfostruct->strDisplayMessage = "Failed : could not create lib folder";
		::PostMessage(pManualPackageThreadlParam->hWnd, WM_USER_MESSAGE, 0, LPARAM(listboxInfostruct));
		return false;
	}

	return true;
}

void CManualPackageStatus::VerifyComponent()
{
	//To display the set of missing components.
	CString FolderPath = lib;
	CString search_path;
	WIN32_FIND_DATA wFdStruct;
	SYSTEMTIME stStruct;
	CString FName;
	int iDay = -1;
	int iMonth = -1;
	int iFolderCount = 0;
	bool bNoMissingComponent(false);
	CString szServicePipeline;
	int iFoundComponent = 0;
	int iPipeline = 0;
	CString szName;
	CString sComponentName;
	CString sServicePackage;
	CString szTempPath;
	CString m_strFilePath;
	//wchar_t path[_MAX_PATH];
	CString sMissingComponentsMsgText;
	int iProjectCount = 0;
	std::array<CString, 100>arr;
	int iarr = 0;
	int arrsize = 1;
	search_path.Format("%s*", FolderPath);
	HANDLE hFind = ::FindFirstFile(search_path, &wFdStruct);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if ((wFdStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString strFolderName = wFdStruct.cFileName;
				int iCount = strFolderName.Find(_T("."));
				if (iCount < 0)
				{
					LocalFoldermap[iFolderCount].strFilename = wFdStruct.cFileName;
					++iFolderCount;
				}
			}
		} while (::FindNextFile(hFind, &wFdStruct));
		::FindClose(hFind);
	}
	m_ServicepackageComboBox.GetWindowText(sServicePackage);
	CString wchPath;
	wchPath = CGeneric::GetExeFolderPath();
	m_strFilePath = wchPath;
	m_XMLFileComponentmap.clear();
	m_xmlParser.XMLParser(m_strFilePath, sServicePackage, m_XMLFileComponentmap, true);
	sMissingComponentsMsgText = _T("There are few Projects missed to load. Here are the list of Projects. \n\n\n");
	for (std::map<int, CString>::iterator iterXML = m_XMLFileComponentmap.begin(); iterXML != m_XMLFileComponentmap.end(); iterXML++)
	{
		szName = iterXML->second;
		szName.Truncate(szName.GetLength() - 4);
		bool bFoundFlag(false);
		bool bElementExist(false);
		for (std::map<int, FolderInformation>::iterator iter = LocalFoldermap.begin(); iter != LocalFoldermap.end(); iter++)
		{
			bFoundFlag = false;
			if (iter->second.strFilename.CompareNoCase(szName) == 0)
			{
				bFoundFlag = true;
				break;
			}
		}
		if (!bFoundFlag)
		{
			m_bNoMissingComponent = true;
			for (int iarr = 0; iarr < arrsize; iarr++)
			{
				if (arr[iarr] == szName)
				{
					bElementExist = true;
					break;
				}
			}
			if (!bElementExist)
			{
				sMissingComponentsMsgText.Append(szName);
				sMissingComponentsMsgText.Append(_T("\n"));
				arr[iarr] = szName;
				iarr++;
				arrsize++;
			}
		}
	}
	if (m_bNoMissingComponent)
	{
		MessageBox(sMissingComponentsMsgText, _T("Missing Projects"));
	}
}

LRESULT CManualPackageStatus::OnThreadManualPackageThread(WPARAM wParam, LPARAM lParam)
{
	MessageBox("Manual package process done", "Process status");

	CWnd* pVerfiyDlgWnd = GetDlgItem(IDC_VERIFY_FOLDERS_BTN);
	if (NULL != pVerfiyDlgWnd)
	{
		::EnableWindow(pVerfiyDlgWnd->m_hWnd, TRUE);
	}

	CWnd* pCloseBtnWnd = GetDlgItem(IDCLOSE);
	if (NULL != pCloseBtnWnd)
	{
		::EnableWindow(pCloseBtnWnd->m_hWnd, TRUE);
	}

	CWnd* pRegisterComponentsBtnWnd = GetDlgItem(IDC_REGISTER_COMPONENTS);
	if (NULL != pRegisterComponentsBtnWnd)
	{
		::EnableWindow(pRegisterComponentsBtnWnd->m_hWnd, TRUE);
	}

	return 0;
}

LRESULT CManualPackageStatus::OnThreadUpdateProgressBar(WPARAM wParam, LPARAM lParam)
{
	m_ManualPackageProgress.SetPos(100 * (int)(CManualPackageStatus::m_iProgressBar * 10)/ (int)lParam);
	

	return 0;
}

LRESULT CManualPackageStatus::DisplayProgressStatusOnListBox(WPARAM wParam, LPARAM lParam)
{
	CSingleLock lock(&c_s);
	lock.Lock();

	_stListBoxTextInfo* listdatastruct = (_stListBoxTextInfo*)lParam;
	m_displaystatus.SendMessage(WM_SETFONT, (WPARAM)m_hEngFont, (LPARAM)1);
	int nCount = m_displaystatus.GetCount();

	m_displaystatus.SetCurSel(nCount);
	if (listdatastruct->bFlag == true)
	{
		m_displaystatus.AddString(listdatastruct->strDisplayMessage, RGB(0, 0, 0));
	}
	else
	{
		m_displaystatus.AddString(listdatastruct->strDisplayMessage, RGB(255, 0, 0));
	}

	lock.Unlock();
	return 0;
}


BOOL CManualPackageStatus::OnInitDialog()
{
	int iPipeline = 0;
	CString szName;
	CString szPipeline;
	CDialogEx::OnInitDialog();
	CRect rect;
	CString szTempPath2;
	CString servicepipeline = "";
	int iUniqueKey = 0;
	int iKey = 0;
	int iIndex = 0;
	CGeneric cg2;
	GetWindowRect(&rect);
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height() - 30, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_ServiceBasePipelineComboBox.ResetContent();
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	szTempPath2 = twchPath;
	szTempPath2.Append("\servicepipeline.xml");
	cg2.populatecombo(servicepipeline, szTempPath2);
	m_mapPipelineInformation = cg2.GetPopulateCombo();
	for (std::map<int, CString>::iterator iter = m_mapPipelineInformation.begin(); iter != m_mapPipelineInformation.end(); iter++)
	{
		CString strPipelineName = m_mapPipelineInformation[iKey];
		if (strPipelineName.Find("2012") != std::string::npos || strPipelineName.Find("2015") != std::string::npos || strPipelineName.Find("2017") != std::string::npos)
		{

			m_ServiceBasePipelineComboBox.InsertString(iIndex, strPipelineName);
			iIndex++;
		}
		iKey++;
	}
	populatesecondcombo();

	m_ServiceBasePipelineComboBox.SetCurSel(0);
	m_ServicepackageComboBox.SetCurSel(0);

	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	m_imgbtn.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_picture1)));
	CWnd* pVerfiyDlgWnd = GetDlgItem(IDC_VERIFY_FOLDERS_BTN);
	::EnableWindow(pVerfiyDlgWnd->m_hWnd, FALSE);

	m_hEngFont = CreateFont(20, 0, 0, 0, 100, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, "arial");

	return TRUE;
}

void CManualPackageStatus::populatesecondcombo()
{
	m_szServiceBasePipeline = "2012.01_SVC_DEP";

	CString szTempPath3;
	CString tmpwchPath;
	tmpwchPath = CGeneric::GetExeFolderPath();
	szTempPath3 = tmpwchPath;

	szTempPath3.Append("\servicepackage.xml");

	DeleteFile(szTempPath3);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD dword = 0;

	HANDLE h = CreateFile(szTempPath3,
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;

	CString szSvnPathFolder2;
	szSvnPathFolder2 = "svn ls --xml \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//";
	szSvnPathFolder2.Append(m_szServiceBasePipeline);

	szSvnPathFolder2.Append("\"");

	TCHAR cmd[260];
	strcpy(cmd, szSvnPathFolder2);



	dword = 0;
	dword = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	CString szTempPath2;
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	szTempPath2 = twchPath;

	szTempPath2.Append("\\servicepackage.xml");
	m_xmlParser.XMLParser(szTempPath2, m_map2012SVCDEPInformation);
	m_ServicepackageComboBox.ResetContent();

	int iUniqueKey = 0;
	for (std::map<int, CString>::iterator iter = m_map2012SVCDEPInformation.begin(); iter != m_map2012SVCDEPInformation.end(); iter++)
	{
		CString strPipelineName = m_map2012SVCDEPInformation[iUniqueKey];
		m_ServicepackageComboBox.InsertString(iUniqueKey, strPipelineName);
		iUniqueKey++;
	}

	CloseHandle(h);
}

HBRUSH CManualPackageStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
	{
							pDC->SetBkMode(RGB(255, 255, 255));
	}
		//	return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return 	hbr = m_brush;
	}
	return m_brush;
}


BOOL CManualPackageStatus::ExecuteCommandLineParameter(CString cmdLine, DWORD & exitCode)
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

		// if Process status is 1.
		if (exitCode == STILL_ACTIVE || ERROR_FILE_NOT_FOUND || ERROR_PATH_NOT_FOUND)
		{
			Sleep(10);
		}

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

CString CManualPackageStatus::ReadString(CString szSection, CString szKey, const CString szDefaultValue)
{
	TCHAR szResult[1024] = { 0 };
	GetPrivateProfileString(szSection, szKey,
		szDefaultValue, szResult, 255, m_szFileName.GetBuffer());
	return szResult;
}

void CManualPackageStatus::WriteString(CString szSection, CString szKey, CString szValue)
{
	WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

int CManualPackageStatus::ReadInteger(CString szSection, CString szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
	return iResult;
}

void CManualPackageStatus::OnSelchangeComboServicePipeline()
{
	UpdateData();

	if (m_nDropListIndex1 < 0)
	{
		return;
	}

	m_ServiceBasePipelineComboBox.GetLBText(m_nDropListIndex1, m_szServiceBasePipeline);
	CString szTempPath3;
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	szTempPath3 = twchPath;

	szTempPath3.Append("\servicepackage.xml");

	DeleteFile(szTempPath3);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD dword = 0;

	HANDLE h = CreateFile(szTempPath3,
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;

	CString szSvnPathFolder2;
	szSvnPathFolder2 = "svn ls --xml \"http://scm.om.cerner.corp//svn//om//branches//Targeted EPs//";
	szSvnPathFolder2.Append(m_szServiceBasePipeline);

	szSvnPathFolder2.Append("\"");

	TCHAR cmd[260];
	strcpy(cmd, szSvnPathFolder2);



	dword = 0;
	dword = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	CString szTempPath2;
	//CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	szTempPath2 = twchPath;
	szTempPath2.Append("\servicepackage.xml");
	m_xmlParser.XMLParser(szTempPath2, m_map2012SVCDEPInformation);
	m_ServicepackageComboBox.ResetContent();

	int iUniqueKey = 0;
	for (std::map<int, CString>::iterator iter = m_map2012SVCDEPInformation.begin(); iter != m_map2012SVCDEPInformation.end(); iter++)
	{
		CString strPipelineName = m_map2012SVCDEPInformation[iUniqueKey];
		m_ServicepackageComboBox.InsertString(iUniqueKey, strPipelineName);
		iUniqueKey++;
	}

	CloseHandle(h);
	m_ServicepackageComboBox.SetCurSel(0);
}


void CManualPackageStatus::OnSelchangeComboServicePackage()
{
	UpdateData();

	if (m_nDropListIndex2 < 0)
	{
		return;
	}

	m_ServicepackageComboBox.GetLBText(m_nDropListIndex2, m_szServicepackage);
}


void CManualPackageStatus::OnChangeEditPackage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//m_EditManualPackage.GetWindowTextA(m_szManualpackage);
}


CString CManualPackageStatus::CommandLineName(CString szInputCommand, CString sFolderPath, CString sCommandName)
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




void CManualPackageStatus::OnBnClickedVerifyFoldersBtn()
{
	// TODO: Add your control notification handler code here
	VerifyComponent();
}


void CManualPackageStatus::OnDestroy()
{
	if (AfxGetMainWnd() != NULL){
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}

	__super::OnDestroy();

	// TODO: Add your message handler code here
}


void CManualPackageStatus::OnBnClickedRegisterComponents()
{
	// TODO: Add your control notification handler code here
	//Registering the dll and ocx components
	DWORD dExitCode = 0;
	ExecuteCommandLineParameter(szRegistercom, dExitCode);
	if ((dExitCode < 0 && dExitCode > 3)/* || dExitCode == 16*/)
	{
		MessageBox(_T("Failed.\n\nRegistering the dll and ocx components."), "Process status");

	}
	else
	{
		MessageBox("Successfully completed registering the dll and ocx components.", "Process status");
	}
}
