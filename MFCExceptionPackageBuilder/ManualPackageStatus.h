#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "XmlparserHelper.h"
#include "Generic.h"
#include "ColorListBox.h"
// CManualPackageStatus dialog

typedef struct _stListBoxTextInfo
{
	bool bFlag;
	CString strDisplayMessage;
}_stListBoxTextInfo;

class CManualPackageStatus : public CDialogEx,public CGeneric
{
	DECLARE_DYNAMIC(CManualPackageStatus)

public:
	CManualPackageStatus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualPackageStatus();

// Dialog Data
	enum { IDD = IDD_DIALOG_PACKAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int		m_nDropListIndex1;
	int		m_nDropListIndex2;
	CString m_szServiceBasePipeline;
	CString m_szServicepackage;
	CString m_szManualpackage;
	CProgressCtrl m_ManualPackageProgress;
	CColorListBox m_displaystatus;
	CButton m_imgbtn;
	void VerifyComponent();
	afx_msg void OnChangeEditPackage();
	afx_msg void OnBnClickedVerifyFoldersBtn();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedStartPackageProcesss();
	afx_msg LRESULT OnThreadManualPackageThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadUpdateProgressBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT DisplayProgressStatusOnListBox(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComboServicePipeline();
	afx_msg void OnSelchangeComboServicePackage();
	static UINT ManualPackageStatusThreadFunc(LPVOID lParam);
	static UINT CopyingApplicationExtension(LPVOID lParam);
	static UINT CopyingIncludeAndIncludeOTS(LPVOID lParam);
	static UINT CopyingSolutionFile(LPVOID lParam);
	
	static BOOL createxml(CString path, CString folder, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword);
	//CWinThread* pThread;
	//CWinThread* pThread1;
	//CWinThread* pThread2;
	static int m_iProgressBar;
	static int m_iShowCompleteStatus;
public:
	struct FolderInformation
	{
		CString strFilename;
		int		iPackageBuildDay;
		int		iPackageBuildMonth;
		bool	bAdditionalFile;
	};

	typedef struct THREADINFOSTRUCT {
		HWND hWnd;
		CString szManualPackageStatusData;
		CString szManualPackageServicePackage;
		CString szServicePipeline;
		CString szManualPackage;
	} THREADINFOSTRUCT;

	HICON m_Icon;
	CBitmap Bmp1;
	CBrush m_brush;
	int m_iKey;
	CXmlparserHelper m_xmlParser;
	CComboBox m_ServiceBasePipelineComboBox;
	CComboBox m_ServicepackageComboBox;
	CString m_szFileName;
	bool m_bNoMissingComponent;
	bool m_bNoAdditionalComponent;
	std::map<int, CString> m_map2012SVCDEPInformation;
	std::map<int, CString> m_XMLFileComponentmap;
	std::map<int, FolderInformation> LocalFoldermap;
	std::map<int, CString> m_mapPipelineInformation;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	static int svnxml(CString sztemppath, std::map<int, CString> szsvnpathmap);
	static int cppxml(CString sztemppath, CString sztemp, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword);
	static int vc10absentfolder(CString sztemppath, CString szPipelinePackage, CString szManualPackageServicePackage, DWORD dword,CString strFolder);
	static BOOL ExecuteCommandLineParameter(CString cmdLine, DWORD & exitCode);
	static CString CommandLineName(CString szInputCommand, CString sFolderPath, CString sCommandName);
	CString ReadString(CString szSection, CString szKey, const CString szDefaultValue);
	void WriteString(CString szSection, CString szKey, CString szValue);
	void populatesecondcombo();
	int ReadInteger(CString szSection, CString szKey, int iDefaultValue);
	afx_msg void OnDestroy();
	//CFont *m_FontObj;
	HFONT m_hEngFont;

	bool DeleteRecreatePDriveFolders(THREADINFOSTRUCT *pManualPackageThreadInformation);
	afx_msg void OnBnClickedRegisterComponents();
};
