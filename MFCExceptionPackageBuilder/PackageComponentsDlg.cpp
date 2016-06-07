
// PackageComponentsDlg.cpp : implementation file

#include "stdafx.h"
#include "PackageComponents.h"
#include "PackageComponentsDlg.h"
#include "afxdialogex.h"
#include "IniReader.h"
#include "IniWriter.h"
#include <string>
#include "resource.h"
#include "Generic.h"
#include <stdlib.h>
#include <array>

#define component "\\\\cernerwhq1\\iptools\\Symbol_Store\\Package\\"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define IDC_MFCEDITBROWSE1 201


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	//enum { IDD = IDD_ABOUTBOX };

	//protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
//protected:
//	DECLARE_MESSAGE_MAP()
};

CPackageComponentsDlg::CPackageComponentsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPackageComponentsDlg::IDD, pParent), m_nServicePipiline(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_nDropListIndex1 = 0;
	//m_nDropListIndex2 = 0;
}

void CPackageComponentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPONENT_PATH, m_EditFolderPath);
	DDX_Control(pDX, IDC_STATUS_RESULT, m_StaticPackageResult);
	DDX_Control(pDX, IDC_STATIC_RESULTS, m_StaticListResults);
	DDX_Control(pDX, IDC_STATIC_MISSING_FILES, m_StaticMissingText);
	DDX_Control(pDX, IDC_STATIC_ADDITIONAL_FILES, m_StaticAdditionalFilesText);
	DDX_Control(pDX, IDOK, m_VerifyButton);
	DDX_Control(pDX, IDC_PIPELINE_VERSION, m_ServicepackageComboBox);
	DDX_Control(pDX, IDC_BASE_PIPELINE, m_BaseLineComboBox);
	//DDX_CBIndex(pDX, IDC_BASE_PIPELINE, m_nDropListIndex1);
	//DDX_CBIndex(pDX, IDC_PIPELINE_VERSION, m_nDropListIndex2);
	DDX_Control(pDX, IDC_MISSING_COMPONENT_LIST, m_ListMissingFiles);
	DDX_Control(pDX, IDC_LIST_ADDITIONAL_FILES, m_ListAdditionalFiles);
	DDX_Control(pDX, IDC_DELETE_ADDITIONAL_FILES, m_DeleteButton);
	DDX_Control(pDX, IDRESET, m_ResetButton);
	DDX_Control(pDX, IDC_STATIC_COMPONENT_PATH, m_Path1);
	DDX_Control(pDX, IDC_FILESELECTOR, m_button1);
	DDX_Control(pDX, IDC_PACKAGE_COMPONENT, m_Path2);
	DDX_Control(pDX, IDC_EDIT_PACKAGE, m_strFolderPath);
}

BEGIN_MESSAGE_MAP(CPackageComponentsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPackageComponentsDlg::OnBnClickedToVerifyComponent)
	
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDRESET, &CPackageComponentsDlg::OnBnClickedReset)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_BASE_PIPELINE, &CPackageComponentsDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_PIPELINE_VERSION, &CPackageComponentsDlg::OnCbnSelchangeCombo2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DELETE_ADDITIONAL_FILES, &CPackageComponentsDlg::OnBnClickedDeleteAdditionalFiles)
	ON_BN_CLICKED(IDC_FILESELECTOR, &CPackageComponentsDlg::OnBnClickedFileselector)
	ON_BN_CLICKED(IDCANCEL, &CPackageComponentsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPackageComponentsDlg message handlers

BOOL CPackageComponentsDlg::OnInitDialog()
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

	if (flag2 == 0)
	{//reviewer
		m_EditFolderPath.ShowWindow(FALSE);
		m_button1.ShowWindow(FALSE);
		m_Path1.ShowWindow(SW_HIDE);
		m_DeleteButton.EnableWindow(FALSE);
	}

	if (flag2 == 1)
	{//verifier
		m_Path2.ShowWindow(SW_HIDE);
		m_strFolderPath.ShowWindow(FALSE);
	}

	m_strFilePath.Empty();
	m_strFilePath.Append(cg.GetExeFolderPath());
	m_BaseLineComboBox.ResetContent();
	CString szTempPath2;
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
		szTempPath2 = twchPath;
	szTempPath2.Append("\servicepipeline.xml");
	CString servicepipeline;
	servicepipeline = "";
	int iUniqueKey = 0;
	int iKey = 0;
	int iIndex = 0;
	//Populating the BaseLine ComboBox
	CGeneric cg3;
	cg3.populatecombo(servicepipeline, szTempPath2);
	m_mapPipelineInformation = cg3.GetPopulateCombo();
	for (std::map<int, CString>::iterator iter = m_mapPipelineInformation.begin(); iter != m_mapPipelineInformation.end(); iter++)
	{
		CString strPipelineName = m_mapPipelineInformation[iKey];
		if (strPipelineName.Find("2012") != std::string::npos || strPipelineName.Find("2015") != std::string::npos || strPipelineName.Find("2017") != std::string::npos)
		{

			m_BaseLineComboBox.InsertString(iIndex, strPipelineName);
			iIndex++;
		}
		iKey++;
	}

	populatesecondcombo();

	//CString szServicePackage = "\servicepackage.xml";
	//m_BaseLineComboBox.GetLBText(0, szServicePackage);
	
	//LoadServicePackagePipeline(szServicePackage);
	
	m_BaseLineComboBox.SetCurSel(0);
	m_ServicepackageComboBox.SetCurSel(0);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_StaticPackageResult.ShowWindow(FALSE);
	m_StaticListResults.ShowWindow(FALSE);
	m_StaticMissingText.ShowWindow(FALSE);
	m_StaticAdditionalFilesText.ShowWindow(FALSE);
	m_ListMissingFiles.ShowWindow(FALSE);
	m_ListAdditionalFiles.ShowWindow(FALSE);
	m_DeleteButton.ShowWindow(FALSE);
	m_ResetButton.EnableWindow(FALSE);
		
	CRect rect_init;
	GetWindowRect(&m_rect);
	rect_init = m_rect;
	SetWindowPos(NULL, 0, 0, rect_init.Width()*0.62, rect_init.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPackageComponentsDlg::LoadServicePackagePipeline(CString strServicePipeLine)
{
	//Populating the ServicePackage ComboBox
	//m_nDropListIndex1 = 0;
	//m_nDropListIndex2 = 0;

	CString szTempPath3;
	CString tmpwchPath;
	tmpwchPath = CGeneric::GetExeFolderPath();
	szTempPath3 = tmpwchPath;

	szTempPath3.Append("\\servicepackage.xml");

	DeleteFile(szTempPath3);
	Sleep(100);

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
		CREATE_ALWAYS,
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
	szSvnPathFolder2.Append(strServicePipeLine);

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
	m_ServicepackageComboBox.SetCurSel(0);
	CloseHandle(0);
}

void CPackageComponentsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPackageComponentsDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPackageComponentsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPackageComponentsDlg::OnBnClickedToVerifyComponent()
{
	//Check if the package component exists.
	if (flag2 == 0)		//for reviewer
	{
		//If the package component exists, takes the latest version and compares.
		int iFolderCount = 0;
		CEdit* EDIT_FOLDER_PATH = (CEdit *)GetDlgItem(IDC_EDIT_PACKAGE);
		CString str;
		EDIT_FOLDER_PATH->GetWindowText(str);
		int iFolder_name_length = str.GetLength();
		CString FolderPath = component;
		CString search_path;
		search_path.Format("%s*", FolderPath + str);
		WIN32_FIND_DATA wFdStruct;
		SYSTEMTIME stStruct;
		CString text;
		CString FName;
		CString FolderName;
		if (iFolder_name_length >= 5)
		{
			std::array<int, 10>arr;
			int max = arr[0];
			int iarr = 0;
			HANDLE hFind = ::FindFirstFile(search_path, &wFdStruct);
			if (hFind != INVALID_HANDLE_VALUE)
			{
				do {
					if ((wFdStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						CString strFolderName = wFdStruct.cFileName;
						++iFolderCount;
						int Index = strFolderName.ReverseFind(_T('_'));
						int y = strFolderName.GetLength();
						FolderName = strFolderName.Left(Index);
						strFolderName = strFolderName.Mid(Index + 1, y);
						int x = atoi(strFolderName);
						arr[iarr] = x;
						iarr++;
					}
				} while (::FindNextFile(hFind, &wFdStruct));
				for (int iFolder = 0; iFolder<iFolderCount; iFolder++)
				{
					if (arr[iFolder] > max)
						max = arr[iFolder];
				}
				text.Format(_T("%d"), max);
				FolderPath.Append(FolderName);
				FolderPath.Append("_");
				FolderPath.Append(text);
				::FindClose(hFind);
			}
		}
		else
		{
			CString sMessage;
			sMessage.Append("Please enter the correct package component. ");
			MessageBox(sMessage, "Package Path information", MB_ICONERROR);
			return;
		}
			CString spath;
			int key = 0;
			spath.Format("%s/*.*", FolderPath);
			WIN32_FIND_DATA FStruct;
			int iCount = 1;
			int iDay = -1;
			int iMonth = -1;
			HANDLE hFolder = ::FindFirstFile(spath, &FStruct);
			if (hFolder != INVALID_HANDLE_VALUE)
			{
				do {
					if (!(FStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						CString FName = FStruct.cFileName;
						FileTimeToSystemTime(&FStruct.ftLastWriteTime, &stStruct);
						iMonth = stStruct.wMonth;
						iDay = stStruct.wDay;
						LocalFolderComponentmap[iCount].iPackageBuildMonth = iMonth;
						LocalFolderComponentmap[iCount].iPackageBuildDay = iDay;
						LocalFolderComponentmap[iCount].strFilename = FName;
						LocalFolderComponentmap[iCount].bAdditionalFile = true;
						if (FName.CompareNoCase("PvOrderPoe.dll") == 0)
						{
							PvOrderPoeComponentmap[0].iPackageBuildMonth = iMonth;
							PvOrderPoeComponentmap[0].iPackageBuildDay = iDay;
							PvOrderPoeComponentmap[0].strFilename = FName;
						}
						++iCount;
					}
				} while (::FindNextFile(hFolder, &FStruct));
			}
		}
	else
	{
		CString sPath;
		m_EditFolderPath.GetWindowText(sPath);
		if (sPath.IsEmpty())
		{
			CString sMessage;
			sMessage.Append("Provide the valid path for verification. ");
			MessageBox(sMessage, "Package Path information", MB_ICONERROR);
			return;
		}
	}
		

		bool bNoMissingComponent(false);
		CString szServicePipeline;

		int iFoundComponent = 0;
		int iPipeline = 0;
		CString szName;
		CString sComponentName;

		CString sServicePackage;
		m_ServicepackageComboBox.GetWindowText(sServicePackage);

		m_XMLFileComponentmap.clear();
		m_xmlParser.XMLParser(m_strFilePath, sServicePackage, m_XMLFileComponentmap);

		m_ListMissingFiles.ShowWindow(TRUE);
		m_ListAdditionalFiles.ShowWindow(TRUE);
		m_DeleteButton.ShowWindow(TRUE);

		m_ListMissingFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		m_ListMissingFiles.ShowWindow(SW_HIDE);

		LVCOLUMN column;
		column.mask = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH | LVCF_IMAGE;
		column.fmt = LVCFMT_LEFT | LVCFMT_IMAGE;
		column.cx = 256;
		column.iImage = -1;

		m_ListMissingFiles.InsertColumn(0, &column);

		m_ListAdditionalFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
		m_ListAdditionalFiles.ShowWindow(SW_HIDE);

		column.pszText = (LPSTR)_T("Additional component list.");

		m_ListAdditionalFiles.InsertColumn(0, &column);

		m_hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
		m_MissingImagelist.Attach(m_hList);


		// Load the large icons
		CBitmap cBmp;
		cBmp.LoadBitmap(IDB_BITMAP1);
		m_MissingImagelist.Add(&cBmp, RGB(255, 0, 255));
		cBmp.DeleteObject();

		m_ListMissingFiles.SetImageList(&m_MissingImagelist, LVSIL_SMALL);

		int iItemMissing = 0;

		int iPOEPackageBuildDay = PvOrderPoeComponentmap[0].iPackageBuildDay;
		int iPOEPackageBuildMonth = PvOrderPoeComponentmap[0].iPackageBuildMonth;

		LVITEM listMissingItem;
		for (std::map<int, CString>::iterator iterXML = m_XMLFileComponentmap.begin(); iterXML != m_XMLFileComponentmap.end(); iterXML++)
		{
			szName = iterXML->second;

			bool bFoundFlag(false);
			for (std::map<int, stPackageInformation>::iterator iter = LocalFolderComponentmap.begin(); iter != LocalFolderComponentmap.end(); iter++)
			{
				//Comparing the LocalFolder with the xml file
				if ((iter->second.strFilename == szName) && IsComponentDateValid(iter->second.iPackageBuildDay, iter->second.iPackageBuildMonth))
				{
					bFoundFlag = true;
					iter->second.bAdditionalFile = false;
					break;
				}
			}

			if (!bFoundFlag)
			{
				szName.Truncate(szName.GetLength() - 4);

				LVFINDINFO info;
				int nIndex = 0;

				info.flags = LVFI_PARTIAL | LVFI_STRING;
				info.psz = szName.GetBuffer();

				nIndex = m_ListMissingFiles.FindItem(&info, -1);

				if (nIndex == -1)
				{
					memset(&listMissingItem, 0, sizeof(LVITEM));
					listMissingItem.mask = LVIF_IMAGE | LVIF_TEXT;
					listMissingItem.mask |= LVIF_STATE;
					listMissingItem.state = LVIS_SELECTED | LVIS_FOCUSED;
					listMissingItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

					listMissingItem.cchTextMax = 256;
					listMissingItem.iItem = iItemMissing;
					listMissingItem.iSubItem = 0;
					listMissingItem.iImage = 0;
					listMissingItem.pszText = szName.GetBuffer();

					m_ListMissingFiles.InsertItem(&listMissingItem);

					::SendMessage(m_ListMissingFiles.m_hWnd, LVM_SETITEM, 0, (LPARAM)&listMissingItem);
					iItemMissing++;
				}
				m_bNoMissingComponent = true;
			}

		}

		LVITEM listItem;
		int iItem = 0;
		for (std::map<int, stPackageInformation>::iterator iter = LocalFolderComponentmap.begin(); iter != LocalFolderComponentmap.end(); iter++)
		{

			if (iter->second.bAdditionalFile == true)
			{
				memset(&listItem, 0, sizeof(LVITEM));
				listItem.mask = LVIF_IMAGE | LVIF_TEXT;
				listItem.state = LVIS_SELECTED | LVIS_FOCUSED;
				listItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

				listItem.cchTextMax = 256;
				listItem.iItem = iItem;
				listItem.iSubItem = 0;
				listItem.iImage = 0;
				listItem.pszText = iter->second.strFilename.GetBuffer();

				m_ListAdditionalFiles.InsertItem(&listItem);

				::SendMessage(m_ListAdditionalFiles.m_hWnd, LVM_SETITEM, 0, (LPARAM)&listItem);
				m_ListAdditionalFiles.SetCheck(iItem, TRUE);

				iItem++;
				m_bNoAdditionalComponent = true;
			}
		}

		CString sFinalResult;

		if (!m_bNoMissingComponent && !m_bNoAdditionalComponent)
		{
			MessageBox("There are no missing component(s) in the package", "Missing component in package", MB_ICONINFORMATION);
			m_StaticPackageResult.SetWindowText("Complete Package");
			m_StaticPackageResult.ShowWindow(TRUE);

			m_VerifyButton.EnableWindow(FALSE);
			m_ResetButton.EnableWindow(TRUE);
		}
		else
		{
			m_StaticPackageResult.SetWindowText("Incomplete Package");

			SetWindowPos(NULL, 0, 0, m_rect.Width(), m_rect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			m_StaticListResults.ShowWindow(TRUE);
			m_StaticMissingText.ShowWindow(TRUE);
			m_StaticAdditionalFilesText.ShowWindow(TRUE);
			m_ListMissingFiles.ShowWindow(TRUE);
			m_ListAdditionalFiles.ShowWindow(TRUE);

			m_StaticPackageResult.ShowWindow(TRUE);

			m_VerifyButton.EnableWindow(FALSE);
			m_ResetButton.EnableWindow(TRUE);
		}

}


void CPackageComponentsDlg::OnBnClickedFileselector()
{
	LocalFolderComponentmap.clear();
	PvOrderPoeComponentmap.clear();
	CString strFolderPath;
	BROWSEINFO StructBrowseInfo;
	memset(&StructBrowseInfo, 0, sizeof(StructBrowseInfo));

	StructBrowseInfo.ulFlags = BIF_USENEWUI;
	StructBrowseInfo.lpszTitle = "Orders and Plans package components.";

	::OleInitialize(NULL);

	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&StructBrowseInfo);

	if (pIDL != NULL)
	{
		TCHAR buffer[_MAX_PATH] = { '\0' };
		if (::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			strFolderPath = buffer;
		}

		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();


	m_EditFolderPath.SetWindowText(strFolderPath);

	CString search_path;
	search_path.Format("%s\\*.*", strFolderPath);
	WIN32_FIND_DATA wFdStruct;
	int iFileCount = 1;
	SYSTEMTIME stStruct;
	int iDay = -1;
	int iMonth = -1;
	HANDLE hFind = ::FindFirstFile(search_path, &wFdStruct);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (!(wFdStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				//Mapping contents of LocalFolder selected.
				CString strFileName = wFdStruct.cFileName;
				FileTimeToSystemTime(&wFdStruct.ftLastWriteTime, &stStruct);
				iMonth = stStruct.wMonth;
				iDay = stStruct.wDay;
				LocalFolderComponentmap[iFileCount].iPackageBuildMonth = iMonth;
				LocalFolderComponentmap[iFileCount].iPackageBuildDay = iDay;
				LocalFolderComponentmap[iFileCount].strFilename = strFileName.MakeLower();
				LocalFolderComponentmap[iFileCount].bAdditionalFile = true;
				if (strFileName.CompareNoCase("PvOrderPoe.dll") == 0)
				{	
					PvOrderPoeComponentmap[0].iPackageBuildMonth = iMonth;
					PvOrderPoeComponentmap[0].iPackageBuildDay = iDay;
					PvOrderPoeComponentmap[0].strFilename = strFileName.MakeLower();
				}
				++iFileCount;
			}
		} while (::FindNextFile(hFind, &wFdStruct));
		::FindClose(hFind);
	}
	m_CheckFolderPathSelect = true;
}


HBRUSH CPackageComponentsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CPackageComponentsDlg::OnBnClickedReset()
{
	//szServicePackage = "2012.01_SVC_DEP";
	m_strFolderPath.SetWindowText("");
	m_StaticPackageResult.SetWindowText("");
	m_EditFolderPath.SetWindowText("");
	CString szServicePackage;
	m_BaseLineComboBox.GetLBText(0, szServicePackage);
	LoadServicePackagePipeline("2012.01_SVC_DEP");
	m_BaseLineComboBox.SetCurSel(0);
	m_ServicepackageComboBox.SetCurSel(0);

	CRect rect_reset;
	rect_reset = m_rect;
	SetWindowPos(NULL, 0, 0, rect_reset.Width()*0.62, rect_reset.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_StaticListResults.ShowWindow(FALSE);
	m_StaticMissingText.ShowWindow(FALSE);
	m_StaticAdditionalFilesText.ShowWindow(FALSE);
	m_ListMissingFiles.ShowWindow(FALSE);
	m_ListAdditionalFiles.ShowWindow(FALSE);
	m_DeleteButton.ShowWindow(FALSE);

	m_VerifyButton.EnableWindow(TRUE);
	m_ResetButton.EnableWindow(FALSE);

	
	m_ListMissingFiles.DeleteColumn(0);
	m_ListMissingFiles.DeleteAllItems();
	m_ListAdditionalFiles.DeleteColumn(0);
	m_ListAdditionalFiles.DeleteAllItems();

	if (NULL != m_MissingImagelist.m_hImageList)
	{
		m_MissingImagelist.Detach();
		ImageList_Destroy(m_hList);
	}
	m_StaticPackageResult.ShowWindow(FALSE);
}


void CPackageComponentsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}


void CPackageComponentsDlg::OnCbnSelchangeCombo1()
{
	UpdateData();
	CString sServiceBasaePipeline = _T("");
	int iCurrentSelection = m_BaseLineComboBox.GetCurSel();
	m_BaseLineComboBox.GetLBText(iCurrentSelection, sServiceBasaePipeline);
	/*CString sServiceBasaePipeline = _T("");
	CString sComboSelection = _T("");
	m_BaseLineComboBox.GetWindowTextA(sComboSelection);
	m_BaseLineComboBox.GetLBText(iCurrentSelection, sServiceBasaePipeline);
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
	szSvnPathFolder2.Append(sServiceBasaePipeline);

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
	//m_ServicepackageComboBox.SetCurSel(0);
	CString twchPath;
	twchPath = CGeneric::GetExeFolderPath();
	////szTempPath2 = twchPath;

	twchPath.Append("\\servicepackage.xml");
	*/
	
	LoadServicePackagePipeline(sServiceBasaePipeline);
	
}


void CPackageComponentsDlg::OnCbnSelchangeCombo2()
{
	UpdateData();

	/*if (m_nDropListIndex2 < 0)
	{
		return;
	}*/
	/*int iComboSelection = m_ServicepackageComboBox.GetCurSel();
	CString szServicePackage;
	m_ServicepackageComboBox.GetLBText(iComboSelection, szServicePackage);*/
}



void CPackageComponentsDlg::OnBnClickedDeleteAdditionalFiles()
{	
	CString sPathName = _T("");

	m_EditFolderPath.GetWindowText(sPathName);

	int iAddtionalFilesCount = m_ListAdditionalFiles.GetItemCount();
	for (int iItemCount = 0; iItemCount < iAddtionalFilesCount; iItemCount++)
	{
		CString sFile = m_ListAdditionalFiles.GetItemText(iItemCount, 0);
		if (TRUE == m_ListAdditionalFiles.GetCheck(iItemCount))
		{
			CString sCompletePath = _T("");
			sCompletePath += sPathName + _T("\\");
			sCompletePath.Append(m_ListAdditionalFiles.GetItemText(iItemCount, 0));

			DeleteFile(sCompletePath);

			m_ListAdditionalFiles.DeleteItem(iItemCount);
			iItemCount--;
		}
	}
}





void CPackageComponentsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	ASSERT(AfxGetApp()->m_pMainWnd != NULL);
	AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
}

bool CPackageComponentsDlg::IsComponentDateValid(int iPackageBuildDay, int iPackageBuildMonth)
{
	int iPOEPackageBuildDay = PvOrderPoeComponentmap[0].iPackageBuildDay;
	int iPOEPackageBuildMonth = PvOrderPoeComponentmap[0].iPackageBuildMonth;

	if (iPackageBuildDay == iPOEPackageBuildDay 
		&& iPackageBuildMonth == iPOEPackageBuildMonth)
	{
		return true;
	}

	//For day equal to 1st
	if (iPOEPackageBuildDay == 1)
	{
		if ((iPOEPackageBuildMonth == eFebruary || iPOEPackageBuildMonth == eApril || iPOEPackageBuildMonth == eJune || iPOEPackageBuildMonth == eAugust || iPOEPackageBuildMonth == eSeptember || iPOEPackageBuildMonth == eNovember) &&
			((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay + 1))
			|| ((iPackageBuildMonth == iPOEPackageBuildMonth - 1) && iPackageBuildDay == 31)))
		{
			return true;
		}
		else if (iPOEPackageBuildMonth == eMarch &&
			((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay + 1))
				|| ((iPackageBuildMonth == iPOEPackageBuildMonth - 1) && iPackageBuildDay == 28)))
		{
			return true;
		}
		else if (iPOEPackageBuildMonth == eJanuary &&
			((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay + 1))
			|| ((iPackageBuildMonth == eDecember) && iPackageBuildDay == 31)))
		{
			return true;
		}
		else if ((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay + 1))
				|| ((iPackageBuildMonth == iPOEPackageBuildMonth - 1) && iPackageBuildDay == 30))
		{
			return true;
		}

		return false;
	}

	//For 28th February
	if (iPOEPackageBuildDay == 28 && iPOEPackageBuildMonth == eFebruary)
	{
		if ((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay - 1))
			|| ((iPackageBuildMonth == iPOEPackageBuildMonth + 1) && iPackageBuildDay == 1))
		{
			return true;
		}

		return false;
	}

	//For Day less than or equal to 29th, ignoring the february month
	if (iPOEPackageBuildDay <= 29 && iPOEPackageBuildDay > 1)
	{
		if (iPackageBuildMonth == iPOEPackageBuildMonth &&
			((iPackageBuildDay == iPOEPackageBuildDay - 1) || (iPackageBuildDay == iPOEPackageBuildDay + 1)))
		{
			return true;
		}

		return false;
	}

	//For Day equal to 30th
	if (iPOEPackageBuildDay == 30)
	{
		if (iPOEPackageBuildMonth == eApril || iPOEPackageBuildMonth == eJune || iPOEPackageBuildMonth == eSeptember || iPOEPackageBuildMonth == eNovember)
		{
			if ((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay - 1))
				|| ((iPackageBuildMonth == iPOEPackageBuildMonth + 1) && iPackageBuildDay == 1))
			{
				return true;
			}
		}
		else
		{
			if (iPackageBuildMonth == iPOEPackageBuildMonth &&
				((iPackageBuildDay == iPOEPackageBuildDay - 1) || (iPackageBuildDay == iPOEPackageBuildDay + 1)))
			{
				return true;
			}
		}

		return false;
	}

	//For Day equal to 31st
	if (iPOEPackageBuildDay == 31)
	{
		if (iPOEPackageBuildMonth != eDecember
			&& ((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay - 1))
			|| ((iPackageBuildMonth == iPOEPackageBuildMonth + 1) && iPackageBuildDay == 1)))
		{
			return true;
		}
		else if (iPOEPackageBuildMonth == eDecember
			&& ((iPackageBuildMonth == iPOEPackageBuildMonth && (iPackageBuildDay == iPOEPackageBuildDay - 1))
			|| ((iPackageBuildMonth == eJanuary) && iPackageBuildDay == 1)))
		{
			return true;
		}

		return false;
	}

	return false;
}

void CPackageComponentsDlg::populatesecondcombo()
{
	CString sServiceBasePipeline = "2012.01_SVC_DEP";

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
	szSvnPathFolder2.Append(sServiceBasePipeline);

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