
// PackageComponentsDlg.h : header file
//

#pragma once
#include <map>
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "Generic.h"

// CPackageComponentsDlg dialog
class CPackageComponentsDlg : public CDialogEx, public CGeneric
{
// Construction
public:
	CPackageComponentsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PACKAGECOMPONENTS_DIALOG };

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
		CEdit m_EditFolderPath;
		CStatic m_StaticPackageResult;
		int m_nServicePipiline;
		CStatic  m_StaticListResults;
		CStatic  m_StaticMissingText;
		CStatic  m_StaticAdditionalFilesText;
		CListCtrl m_ListMissingFiles;
		CImageList m_MissingImagelist;
		CListCtrl m_ListAdditionalFiles;
		CButton	 m_VerifyButton;
		HIMAGELIST m_hList;
		CButton	 m_DeleteButton;
		CButton	 m_ResetButton;
		CStatic m_Path1;
		CButton m_button1;
		CStatic m_Path2;
		CEdit m_strFolderPath;
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBnClickedFileselector();
		afx_msg void OnBnClickedToVerifyComponent();
		afx_msg void OnClickedRadio2012();
		afx_msg void OnBnClickedRadio2015();
		afx_msg void OnBnClickedReset();
		afx_msg void OnCbnSelchangeCombo1();
		afx_msg void OnCbnSelchangeCombo2();
		afx_msg void OnBnClickedDeleteAdditionalFiles();
		void LoadServicePackagePipeline(CString strServicePipeLine);

		bool IsComponentDateValid(int iPackageBuildDay, int iPackageBuildMonth);

		void populatesecondcombo();
public:
	struct stPackageInformation
	{
		CString strFilename;
		int		iPackageBuildDay;
		int		iPackageBuildMonth;
		bool	bAdditionalFile;
	};
	//int		m_nDropListIndex1;
	//int		m_nDropListIndex2;
	int flag2;
	int m_iKey;
	CString m_szFileName;
	CString m_strFilePath;
	CBrush m_brush;
	CXmlparserHelper m_xmlParser;
	CComboBox m_ServicepackageComboBox;
	CComboBox m_BaseLineComboBox;
	CGeneric cg;
	CRect m_rect;
	bool m_bNoMissingComponent;
	bool m_bNoAdditionalComponent;
	bool  m_CheckFolderPathSelect;
	std::map<int, CString> m_map2012SVCDEPInformation;
	std::map<int, CString> m_XMLFileComponentmap;
	std::map<int, stPackageInformation> LocalFolderComponentmap;
	std::map<int, stPackageInformation> PvOrderPoeComponentmap;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	enum Month
	{
		eJanuary = 1,
		eFebruary,
		eMarch,
		eApril,
		eMay,
		eJune,
		eJuly,
		eAugust,
		eSeptember,
		eOctober,
		eNovember,
		eDecember
	};
};
