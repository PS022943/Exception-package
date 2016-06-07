#pragma once
#include <map>
#include "XmlparserHelper.h"
class CGeneric
{
public:
	CGeneric();
	~CGeneric();
	CXmlparserHelper m_xmlParser;
	static CString GetExeFolderPath();
	std::map<int, CString> m_map2012SVCDEPInformation;
	std::map<int, CString> m_mapPipelineInformation;
	std::map<int, CString> GetPopulateCombo();
	void SetPopulateCombo(int iUniqueKey, int iKey);
	void populatecombo(CString servicepipeline, CString sztemppath1);
};

