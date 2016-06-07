#pragma once

#include <map>

class CXmlparserHelper
{
public:
	CXmlparserHelper();
	~CXmlparserHelper();
	//bool XMLDOMNodeParser(CString szFileName, std::map<int, CString>& m_map2012SVCDEPInformation);
	bool XMLParser(CString szFileName, std::map<int, CString>& m_map2012SVCDEPInformation);
	bool XMLParser(CString strFilePath, CString strPipeline, std::map<int, CString>& XMLFileComponentmap, bool bIsCreator = false);
	//int flag1;
private:
	MSXML2::IXMLDOMDocumentPtr m_plDomDocument;
	MSXML2::IXMLDOMElementPtr m_pDocRoot;
	MSXML2::IXMLDOMNodeListPtr NodeListPtr;




	MSXML2::IXMLDOMNodePtr DOMNodePtr;//pointer to the node

	MSXML2::IXMLDOMNode *pIDOMNode = NULL;//pointer to element's node
	MSXML2::IXMLDOMNode *pIParentNode = NULL;//pointer to parent node
	MSXML2::IXMLDOMNode *pIAttrNode = NULL;//pointer to attribute node
	MSXML2::IXMLDOMNamedNodeMapPtr DOMNamedNodeMapPtr;//iteration through the collection of attribute nodes
	MSXML2::IXMLDOMNodeList *childList = NULL;//node list containing the child nodes
	MSXML2::IXMLDOMNode *pIDOMChildNode = NULL;//pointer to element's node
	int m_iKey;

	

	//MSXML2::IXMLDOMDocumentPtr pXMLDom = NULL;
	

	MSXML2::IXMLDOMNamedNodeMapPtr m_pDomNodeMap;

	MSXML2::IXMLDOMElement*      pXMLElement;
	MSXML2::IXMLDOMNamedNodeMap* pXMLNamedNodeMap;
	MSXML2::IXMLDOMNode*         pChildNode;
	MSXML2::IXMLDOMNode*         pXMLNode;
	MSXML2::IXMLDOMNodeList*     pChildList;
	MSXML2::IXMLDOMAttribute* pIXMLDOMAttribute = NULL;
	void ErrorExit(LPTSTR lpszFunction);
};

