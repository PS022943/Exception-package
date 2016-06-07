#include "stdafx.h"
#include "XmlparserHelper.h"
#include <strsafe.h>

CXmlparserHelper::CXmlparserHelper()
{
	m_iKey = 0;
}


CXmlparserHelper::~CXmlparserHelper()
{
	m_iKey = 0;
}


bool CXmlparserHelper::XMLParser(CString szFileName, std::map<int, CString>& m_map2012SVCDEPInformation)
{
	m_map2012SVCDEPInformation.clear();
	// Initialize COM
	::CoInitialize(NULL);
	int n = 0;
	BSTR strFindText = L" ";
	m_iKey = 0;
	//Variables to store item's name, parent, text and node type:
	BSTR bstrItemText, bstrItemNode, bstrItemParent, bstrNodeType;

	//Variables to store attribute's name,type and text:	 
	BSTR bstrAttrName, bstrAttrType, bstrAttrText;

	//Create an instance of the DOMDocument object:
	m_plDomDocument.CreateInstance(__uuidof(DOMDocument30));

	// Load a document:
	_variant_t varXml(szFileName);//XML file to load
	_variant_t varResult((bool)TRUE);//result 

	if (NULL == m_plDomDocument)
	{
		return false;
	}

	varResult = m_plDomDocument->load(varXml);
	//m_plDomDocument->GetparseError();
	//ErrorExit("XMLParser");
	if ((bool)varResult == FALSE)
	{
		return false;
	}

	//Collect all or selected nodes by tag name:
	NodeListPtr = m_plDomDocument->getElementsByTagName(strFindText);

	if (NULL == m_plDomDocument->documentElement || NULL == NodeListPtr)
	{
		return false;
	}

	//Output root node:
	m_plDomDocument->documentElement->get_nodeName(&bstrItemText);

	for (int i = 0; i < (NodeListPtr->length); i++)
	{

		if (NULL != pIDOMNode)
		{
			pIDOMNode->Release();
		}

		NodeListPtr->get_item(i, &pIDOMNode);


		if (NULL != pIDOMNode)
		{

			pIDOMNode->get_nodeTypeString(&bstrNodeType);
			CString strElementtext = "element";
			CString strNodeType(bstrNodeType);

			if (strNodeType.CompareNoCase(strElementtext) == 0)
			{
				n++;//element node's number

				pIDOMNode->get_nodeName(&bstrItemNode);
				CString strNode = (CString)bstrItemNode;
				pIDOMNode->get_text(&bstrItemText);
				CString strItemText(bstrItemText);
				if (strNode.CompareNoCase("name") == 0)
				{
					m_map2012SVCDEPInformation.insert(std::pair<int, CString>(m_iKey, strItemText));
					m_iKey++;
				}

				pIDOMNode->get_parentNode(&pIParentNode);
				if (NULL == pIParentNode)
				{
					return false;
				}

				pIParentNode->get_nodeName(&bstrItemParent);

				pIDOMNode->get_childNodes(&childList);

				int j = 0;//loop-index variable
				long length = 0;// number of attributes in the collection

				DOMNamedNodeMapPtr = pIDOMNode->attributes;

				if (NULL == DOMNamedNodeMapPtr)
				{
					return false;
				}

				HRESULT hResult = DOMNamedNodeMapPtr->get_length(&length);

				if (SUCCEEDED(hResult))
				{
					//Loop through the number of attributes:
					for (j = 0; j < length; j++)
					{
						//get attribute node:							
						DOMNamedNodeMapPtr->get_item(j, &pIAttrNode);
						if (NULL == pIAttrNode)
						{
							return false;
						}

						pIAttrNode->get_nodeTypeString(&bstrAttrType);//type as string
						pIAttrNode->get_nodeName(&bstrAttrName);
						pIAttrNode->get_text(&bstrAttrText);
					}
				}
			}
		}
	}

	//Do not forget to release interfaces:
	pIDOMNode->Release();
	pIDOMNode = NULL;
	//pIParentNode->Release();
	//pIParentNode = NULL;
	::CoUninitialize();
	return false;
}

void CXmlparserHelper::ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
//	ExitProcess(dw);
}

bool CXmlparserHelper::XMLParser(CString strFilePath, CString strPipeline, std::map<int, CString>& XMLFileComponentmap, bool bIsCreator)
{
	// Initialize COM
	::CoInitialize(NULL);
	int iXMLMapFileCount = 1;

	HRESULT hr = m_plDomDocument.CreateInstance(__uuidof(DOMDocument30));

	strFilePath.Append("\\");
	strFilePath.Append("Package_Components.xml");

	_bstr_t bstrFileName;
	bstrFileName = strFilePath.AllocSysString();

	variant_t vResult;
	vResult = m_plDomDocument->load(bstrFileName);

	if (((bool)vResult) != TRUE) // success!
	{
	
		return false;;
	}

	hr = m_plDomDocument->get_documentElement(&pXMLElement);
	if (FAILED(hr))
	{
		return false;
	}

	//Retrieve a list of the child nodes.
	hr = pXMLElement->get_childNodes(&pChildList);
	if (FAILED(hr))
	{
		return false;
	}

	long lChildListCount = pChildList->Getlength();

	CString sTempPipeline, sTempVersion, sNodeText;
	CString sVersion;

	for (long lChildCount = 0; lChildCount < lChildListCount; lChildCount++)
	{
		// Retrieve the first child in the list.
		hr = pChildList->get_item(lChildCount, &pChildNode);
		if (FAILED(hr))
		{
			return false;
		}

		_bstr_t bstrText;
		MSXML2::IXMLDOMNodePtr pInfoNode = NULL;
		pInfoNode = pChildNode->selectSingleNode((LPCSTR)"VERSION");  //Able to get the text within the  child element
		bstrText = pInfoNode->text;

		/*if (lChildCount == 0)
		{
			if (!bIsCreator)
			{
				continue;
			}
		}*/

		if (lChildCount > 0)
		{
			sTempPipeline = strPipeline.Mid(0, 7);

			sNodeText = (LPCTSTR)bstrText;

			if (sTempPipeline == sNodeText.Mid(0, 7))
			{
				sTempVersion = strPipeline.Mid(8, 2);
				sVersion = sNodeText.Mid(8, 2);
				int iTempVersion, iVersion;

				if (strPipeline.GetLength() == 10)
				{
					iTempVersion = _ttoi(sTempVersion);
					iVersion = _ttoi(sVersion);

					if (iTempVersion < iVersion)
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
			else if (strPipeline.Mid(0, 11) == sNodeText.Mid(0, 11))
			{
				sTempVersion = strPipeline.Mid(11, 4);

				double dTempVersion, dVersion;
				sVersion = sNodeText.Mid(11, 4);

				dTempVersion = _ttof(sTempVersion);
				dVersion = _ttof(sVersion);

				if (dTempVersion < dVersion)
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}

		MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
		pNodeList = pChildNode->GetchildNodes();// nextSibling;

		long lNodeList = pNodeList->Getlength();
		CString sComponentName;
		_bstr_t bstrElementName;
		_bstr_t bstrElementText;
		MSXML2::IXMLDOMNodePtr pInfoListNode = NULL;

		//Add condition to load the MAP based on the version selected


		for (long lCount = 1; lCount < lNodeList; lCount++)
		{
			CString strCount;
			strCount.Format(_T("%ld"), lCount);

			bstrElementName = L"Component" + _bstr_t(strCount);

			pInfoListNode = pChildNode->selectSingleNode(bstrElementName);
			bstrElementText = pInfoListNode->text;

			sComponentName = (LPCTSTR)bstrElementText;
			XMLFileComponentmap[iXMLMapFileCount] = sComponentName.MakeLower();
			iXMLMapFileCount++;
		}
	}
	return true;
}

