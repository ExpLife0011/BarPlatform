#include "CenterInterface.h"

#include "ConfigFile.h"
#include "CommInterface.h"
//#include "UdpServer.h"
#include "csprotocol.h"
#include "BarInfo.h"
#include "Jsoncode.h"
#include "log.h"

const int TCP_TMPRECV_BUFLEN = 2 * 1024;
int g_nSeriNumber = 1;
HMODULE g_hm = 0;

CCenterInterface::CCenterInterface()
{
}


CCenterInterface::~CCenterInterface()
{
}

// ��ȡ���°�����
string CCenterInterface::GetUpdate()
{
	CLogInterface::WriteLog("��ȡ���°����� beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["ver1"] = CConfigFile::GetInstance()->GetClientVer();
	mapParam["ver2"] = CConfigFile::GetInstance()->GetMenuVer();
	string strUrl = GetHttpDataUrl("/client/update");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("��ȡ���°����� end");
	return strRet;
}


// ��ȡ����
string CCenterInterface::GetConfig()
{
	CLogInterface::WriteLog("CCenterInterface::GetConfig() beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	string strUrl = GetHttpDataUrl("/client/strategy");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("CCenterInterface::GetConfig() end");
	return strRet;
}

// ��ȡ����
string CCenterInterface::GetConfigII()
{
	CLogInterface::WriteLog("CCenterInterface::GetConfigII() beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["ver1"] = CConfigFile::GetInstance()->GetClientVer();
	mapParam["ver2"] = CConfigFile::GetInstance()->GetMenuVer();
	mapParam["barId"] = CConfigFile::GetInstance()->GetBarId();
	string strUrl = GetHttpDataUrl("/client/barStrategy");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("CCenterInterface::GetConfigII() end");
	return strRet;
}

// ��ȡ�˵����°�
string CCenterInterface::GetMenuUpdate()
{
	CLogInterface::WriteLog("��ȡ�˵����°� beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["ver1"] = CConfigFile::GetInstance()->GetClientVer();
	mapParam["ver2"] = CConfigFile::GetInstance()->GetMenuVer();
	string strUrl = GetHttpDataUrl("/client/update2");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("��ȡ�˵����°� end");
	return strRet;
}

// ��ȡС����
string CCenterInterface::GetMinProgram()
{
	CLogInterface::WriteLog("��ȡС���� beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["ver1"] = CConfigFile::GetInstance()->GetClientVer();
	mapParam["ver2"] = CConfigFile::GetInstance()->GetMenuVer();
	string strUrl = GetHttpDataUrl("/client/update3");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("��ȡС���� end");
	return strRet;
}

// ��ȡ��Ϸ��
string CCenterInterface::GetGameLib()
{
	CLogInterface::WriteLog("��ȡ��Ϸ�� beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["type"] = CConfigFile::GetInstance()->GetGameMenuType();
	string strUrl = GetHttpDataUrl("/client/relatedGames");
	string strRet = GetHttpData(strUrl, mapParam, true);
	CLogInterface::WriteLog("��ȡ��Ϸ�� end");
	return strRet;
}

// �ϴ��������� 
string CCenterInterface::UploadInfo(const string &strUrl, const string strAction)
{
	CLogInterface::WriteLog("�ϴ��������� beg");
	map<string, string> mapParam;
	mapParam["clientId"] = CConfigFile::GetInstance()->GetMac();
	mapParam["lanIp"] = CConfigFile::GetInstance()->GetLocalIp();
	mapParam["agencyId"] = CConfigFile::GetInstance()->GetAgencyId();
	mapParam["ver1"] = CConfigFile::GetInstance()->GetClientVer();
	mapParam["ver2"] = CConfigFile::GetInstance()->GetMenuVer();
	mapParam["action"] = strAction;
	//string strUrl = "http://106.14.45.195:80/talking/start";// GetHttpDataUrl("/talking/start");
	string sUrl = strUrl;
	sUrl += "talking/start";
	string strRet = GetHttpData(sUrl, mapParam);
	CLogInterface::WriteLog("�ϴ��������� end");
	return strRet;
}

// �ӱ���������Դ�ļ�������
bool CCenterInterface::DownloadFromP2P(const string &strUrl, const string &strFileName, const string &strMd5)
{
	typedef const int (__cdecl * P2pGetContent_)(const char * lpDownUrl, const char *lpFileName, const char * lpChkSign);

	if (g_hm == 0)
	{
		string strDll = "\\lanp2p.dll";
		strDll = CConfigFile::GetInstance()->CreateFullPathName(strDll);
		g_hm = LoadLibraryA(strDll.c_str());
		if (!g_hm)
		{
			CLogError::WriteLog("����%sʧ��%d������", strDll.c_str(), GetLastError());
			return false;
		}
	}

	P2pGetContent_ P2pGetContent = (P2pGetContent_)GetProcAddress(g_hm, "P2pGetContent");
	if (P2pGetContent == NULL)
	{
		CLogError::WriteLog("����lanp2p.dll�ӿ�P2pGetContentʧ�ܣ�����");
		return false;
	}
	unsigned int uiSize;
	int nRet = P2pGetContent(strUrl.c_str(), strFileName.c_str(), strMd5.c_str());
	CLogInterface::WriteLog("DownloadFromP2P ret: %d", nRet);

	return nRet == 0;
	//char szBuffer[512] = { 0 };
	//sprintf(szBuffer, "data size: %x, %c%c%c%c", uiSize, lpCnt[0], lpCnt[1], lpCnt[2], lpCnt[3]);
	//OutputDebugStringA(szBuffer);
}
// ������Դ�ļ�������
bool CCenterInterface::DownloadFile(const string &strUrl, const string &strFileName, const string &strMd5)
{
	CLogInterface::WriteLog("������Դ�ļ������أ�%s", strUrl.c_str());
	bool bRet = false;
	if (CConfigFile::GetInstance()->GetIsCallP2P())
	{
		bRet = DownloadFromP2P(strUrl, strFileName, strMd5);
	}
	else
	{
		int nState = 0;
		int nRet = CCommInterface::DownloadDataFromServer(strUrl.c_str(), (char *)strFileName.c_str(), nState);
		CLogInterface::WriteLog("������������Դ�ļ�������State: %d, Result: %d end", nState, nRet);
		return nRet > 0;
	}
	CLogInterface::WriteLog("������Դ�ļ������� Ret: %d end", bRet);
	return bRet;
}

// ��ȡHttp����
string CCenterInterface::GetHttpData(const string &strUrl, map<string, string>mapParam, bool bBigRet, bool bIsPost)
{
	CLogInterface::WriteLog("��ȡHttp���� beg");
	string strRet;
	string sUrl(strUrl);
	string sParam;
	for (auto it : mapParam)
	{
		if (!it.first.empty())
		{
			sParam += it.first;
			sParam += "=";
			sParam += it.second;
			sParam += "&";
		}
	}
	if (!sParam.empty())
	{
		sParam = sParam.substr(0, sParam.size() - 1);
		sUrl += "?";
		sUrl += sParam;
	}
	if (bBigRet)
	{
		const int nRetLen = 10 * 1024 * 1024;
		char *szRet = new char[nRetLen];
		memset(szRet, 0, nRetLen);
		int nState(0);
		CLogInterface::WriteLog("��ȡHttp���� URL: %s", sUrl.c_str());
		int nRet = 0;
		if (bIsPost)
		{
			int nPos = sUrl.find("?");
			if (string::npos != nPos)
			{
				string strUrl = sUrl.substr(0, nPos);
				string strSource = sUrl.substr(nPos + 1);
				nRet = CCommInterface::PostHttpData(strUrl.c_str(), strSource.c_str(), strSource.size(), szRet, nRetLen, nState);
			}
		}
		else
		{
			nRet = CCommInterface::GetHttpData(sUrl.c_str(), szRet, nRetLen, nState);
		}
		CLogInterface::WriteLog("��ȡHttp���� Result Len: %d", nRet);
		if (nRet > 0)
		{
			strRet = szRet;
		}
		delete[] szRet;
	}
	else
	{
		int nRet = 0;
		const int nRetLen = 2 * 1024;
		char szRet[nRetLen] = { 0 };
		int nState(0);
		CLogInterface::WriteLog("��ȡHttp���� URL: %s", sUrl.c_str());
		if (bIsPost)
		{
			int nPos = sUrl.find("?");
			if (string::npos != nPos)
			{
				string strUrl = sUrl.substr(0, nPos);
				string strSource = sUrl.substr(nPos + 1);
				nRet = CCommInterface::PostHttpData(strUrl.c_str(), strSource.c_str(), strSource.size(), szRet, nRetLen, nState);
			}
		}
		else
		{
			nRet = CCommInterface::GetHttpData(sUrl.c_str(), szRet, nRetLen, nState);
		}
		CLogInterface::WriteLog("��ȡHttp���� Result: %s", szRet);
		if (nRet > 0)
		{
			strRet = szRet;
		}
	}
	CLogInterface::WriteLog("��ȡHttp���� end");
	return strRet;
}

// ��ȡHttp������������
string CCenterInterface::GetHttpDataUrl(const string &strPath)
{
	const int nBufLen(200);
	char szBuf[nBufLen] = { 0 };
	string s = CConfigFile::GetInstance()->GetServerHttpIpPort();
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	if (s.find("http://") == 0 || s.find("https://") == 0)
	{
		sprintf_s(szBuf, nBufLen, "%s%s", s.c_str(), strPath.c_str());
	}
	else
	{
		sprintf_s(szBuf, nBufLen, "http://%s%s", s.c_str(), strPath.c_str());
	}
	string strUrl(szBuf);
	return strUrl;
}

// �ص�����ָ������
// ������
//   pCallbackParam - �����ص��������ϲ������
//   pSreamSocket	- ������SreamSocket����ָ�룻
//   pRemoteIp		- �������е�Զ��Ip��
//   nRemotePort	- �������е�Զ�̶˿ڣ�
void WINAPI CCenterInterface::ClientProcTcpConnect(void *pCallbackParam, void *pSreamSocket, char *pSendBuf, int nBufLen)
{
	CCommonTCPPacket *p = (CCommonTCPPacket *)pSendBuf;
	int nSendLen = p->_data_len + sizeof(CCommonTCPPacket);
	p->_command_id = ::htonl(p->_command_id);
	p->_head_flag = ::htonl(p->_head_flag);
	p->_data_len = ::htonl(p->_data_len);
	p->_seq_no = ::htonl(p->_seq_no);
	CCommonTCPPacket ReHeader;
	int nRet = CCommInterface::SendTcpDataByClientConnection(pSreamSocket, p, nSendLen);
	if (nRet <= 0)
	{
		CLogInterface::WriteLog("CCenterInterface::ClientProcTcpConnect send header action: %d, false�� %d", p->_command_id, ::GetLastError());
		return;
	}
	nRet = CCommInterface::RecvTcpDataByClientConnection(pSreamSocket, &ReHeader, sizeof(ReHeader));
	if (nRet <= 0)
	{
		CLogInterface::WriteLog("CCenterInterface::ClientProcTcpConnect recv header action: %d, false�� %d", p->_command_id, ::GetLastError());
		return;
	}

	int nR = ::ntohl(ReHeader._data_len);
	if (nR <= 0)
	{
		CLogInterface::WriteLog("CCenterInterface::ClientProcTcpConnect recv Data len: %d -- %d", p->_data_len, nR);
		return;
	}
	// ��������
	char *pRecv = new char[nR + 1];
	nRet = CCommInterface::RecvTcpDataByClientConnection(pSreamSocket, pRecv, nR);
	if (nRet <= 0)
	{
		CLogInterface::WriteLog("CCenterInterface::ClientProcTcpConnect recv header action: %d, �����ļ������� false�� %d", p->_command_id, ::GetLastError());
		delete []pRecv;
		return;
	}

	if (pCallbackParam)
	{
		if (nR > TCP_TMPRECV_BUFLEN){ nR = TCP_TMPRECV_BUFLEN; }
		memcpy(pCallbackParam, pRecv, nR);
	}
	delete []pRecv;
}

