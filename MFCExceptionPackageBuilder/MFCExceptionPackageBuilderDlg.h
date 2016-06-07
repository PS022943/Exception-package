
// MFCExceptionPackageBuilderDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "XmlparserHelper.h"


// CMFCExceptionPackageBuilderDlg dialog
class CMFCExceptionPackageBuilderDlg : public CDialogEx
{
// Construction
public:
	CMFCExceptionPackageBuilderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCEXCEPTIONPACKAGEBUILDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_UsernameEdit;
	CEdit m_PasswordEdit;
	CString m_UserNameString;
	CString m_PasswordString;
	CStatic m_GroupBoxControl;
	CString m_Uname;
	CString m_Password;
	bool LoginAndMappedToRemoteDirectory();
	void DeleteRequiredFolders(CString szInputCommand, CString sFolderName, CString sCommandName);
	CString CommandName(CString szInputCommand, CString sFolderPath, CString sCommandName);
	afx_msg void OnBnClickedInstaller();
	afx_msg void OnBnClickedVerifier();
	afx_msg void OnEnChangePasswordEdit();
	afx_msg void OnEnChangeUsernameEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	CBrush m_brush;
	CXmlparserHelper m_xmlParser;
	std::map<int, CString> m_map2012SVCDEPInformation;
	std::map<int, CString> m_mapPipelineInformation;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	void populatecombo(CString servicepipeline, CString sztemppath1);
	BOOL ExecuteCommandLineParameter(CString cmdLine, DWORD & exitCode);
	DWORD GetApplicationDirectory(CString& strAppDir);

};

