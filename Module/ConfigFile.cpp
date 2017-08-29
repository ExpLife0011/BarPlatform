#include "ConfigFile.h"
#include "gfunction.h"

//#include "diskseri.h"
//#include <atlutil.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32")

// �������������ļ���
#define CONFIG_FILENAME					"config.ini"
// ��������
#define CONFIG_LOCALAPP					"local"
// ��ȡͬ����Ӧ������
#define CONFIG_LOCAL_IMAPPCLIENTNAME	"imapp"
// ��ȡ����Ӧ������
#define CONFIG_LOCAL_MATCHAPPCLIENTNAME "matchapp"
// ��ȡ����̨Ӧ������
#define CONFIG_LOCAL_CONTROLAPPCLIENTNAME "controlapp"
// ȡ�����ƶ˰汾
#define CONFIG_LOCAL_CONTROLVER			"1.0.0.1"
// ȡ���ͻ��˰汾
#define CONFIG_LOCAL_CLIENTVER			"1.0.0.1"
// tcp��������˿�
#define CONFIG_LOCAL_TCPLISTENKEY		"tcpport"
// udp��������˿�
#define CONFIG_LOCAL_UDPLISTENKEY		"udpport"
// ȡ�����ķ���IP
#define CONFIG_LOCAL_UDPIP              "serverip"
// ��ͻ��˷��ͷ��ʵ�ַ
#define CONFIG_LOCAL_ClIENTIP     "serverClientIp"
// ȡ�����ķ���http�˿�
#define CONFIG_LOCAL_HTTPPORT           "serverport"
// ͼƬ�ļ�·��
#define CONFIG_LOCAL_IMAGEPATH		    "imagepath"
// ����UDP�����˿�
#define CONFIG_LOCAL_LOCALUDPLISTENPORT "udplistenport"
// �������
#define CONFIG_LOCAL_MACHINENAME		"machinename"
// ��������URL
#define CONFIG_LOCAL_YYBBURL            "yybburl"
// ���������IP��ַ
#define CONFIG_LOCAL_YSERVER            "yserver"

// BarId
#define CONFIG_LOCAL_NETBARID			"barid"
// ����Id
#define CONFIG_LOCAL_AGENCYID			"agencyid"
// ��Ϸ�˵�����
#define CONFIG_LOCAL_GAMEMENU			"gamemenu"
// ��Ϸ��Դ·��
#define CONFIG_LOCAL_GAMERESPATH		"respath"
// ��Ϸ����
#define CONFIG_LOCAL_GAMERLIB			"libname"
// ��Ϸ������
#define CONFIG_LOCAL_GAMERTYPE			"typename"
// ��Ϸ��������
#define CONFIG_LOCAL_GAMERSORT			"sortname"
// С����·����
#define CONFIG_LOCAL_MINPROGRAM			"minprogram"
// ��ȡ����·����
#define CONFIG_LOCAL_POLICYPATH			"policy"
// ƽ̨������·����
#define CONFIG_LOCAL_PLATFORMUPDATE		"platformupdate"
// �����ݷ�ʽ����
#define CONFIG_LOCAL_LINKFILENAME		"linkfilename"

// �Ƿ���������ͼ��
#define CONFIG_LOCAL_CREATEDESKICON		"createdeskicon"
// �Ƿ���ò˵�
#define CONFIG_LOCAL_CALLMENU			"callmenu"
// �Ƿ����P2P
#define CONFIG_LOCAL_CALLP2P			"callp2p"
// �Ƿ�����ͼ���
#define CONFIG_LOCAL_DLICONLIB			"downloadiconlib"
// ������������ɨ�����ڣ���λ���ӣ�0 - ��ɨ�裩
#define CONFIG_LOCAL_DLCONFIGINTERVAL	"downloadconfiginterval"
// ��Ϸ�˵�����
#define CONFIG_LOCAL_MENUTYPE			"gamemenutype"

// ����˰汾
const char *g_constServerVer = "1.0.0.0";

template <> CConfigFile *CSingle<CConfigFile>::m_pInstance = 0;

CConfigFile::CConfigFile()
{
	LoadConfig();
}


CConfigFile::~CConfigFile()
{
}

std::string GetIpFromUrl(const std::string &strUrl)
{
	std::string strRet;

	/////* ��ַ: http://www.google.cn/ */
	//wstring strwUrl = MultCharToWideCharA(strUrl.c_str(), strUrl.size());
	//CUrl url;
	//url.CrackUrl(strwUrl.c_str());
	//wstring strwHost = url.GetHostName();
	//string strHost = WideCharToMultiCharW(strwHost.c_str(), strwHost.size());

	//unsigned long nIp = inet_addr(strHost.c_str());
	//if (nIp == INADDR_NONE)
	//{
	//	WSADATA WSAData;
	//	///*******************************************************************
	//	//ʹ��Socket�ĳ�����ʹ��Socket֮ǰ�������WSAStartup������
	//	//�ú����ĵ�һ������ָ����������ʹ�õ�Socket�汾��
	//	//���и�λ�ֽ�ָ�����汾����λ�ֽ�ָ�����汾��
	//	//����ϵͳ���õڶ����������������Socket�İ汾��Ϣ��
	//	//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬
	//	//Ȼ����ҵ���Socket�⵽��Ӧ�ó����С�
	//	//�Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ�
	//	//�ú���ִ�гɹ��󷵻�0��
	//	//*****************************************************************/
	//	int WSA_return = WSAStartup(0x0101, &WSAData);
	//	if (WSA_return == 0)
	//	{
	//		/* ��Ҫ������������������ */
	//		HOSTENT *host_entry = gethostbyname(strHost.c_str());
	//		int nErr = GetLastError();
	//		if (host_entry != 0)
	//		{
	//			strRet = host_entry->h_name;
	//			FormatString(strRet, "%d.%d.%d.%d", host_entry->h_addr_list[0][0] & 0x00ff, host_entry->h_addr_list[0][1] & 0x00ff, host_entry->h_addr_list[0][2] & 0x00ff, host_entry->h_addr_list[0][3] & 0x00ff);
	//			//(host_entry->h_addr_list[0][0] & 0x00ff),
	//			//(host_entry->h_addr_list[0][1] & 0x00ff),
	//			//(host_entry->h_addr_list[0][2] & 0x00ff),
	//			//(host_entry->h_addr_list[0][3] & 0x00ff));
	//		}
	//	}
	//	WSACleanup();
	//}
	//else
	//{
	//	strRet = strHost;
	//}
	return strRet;
}

// ת��Ϊ����·��
void CConfigFile::SwitchAbsPath(string &strPath)
{
	string s(strPath);
	strPath = CConfigFile::GetInstance()->CreateFullPathName(s);
}
// ��ȡ��Ϸ��Դ·��
string CConfigFile::GetGameResPath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strGameMenuResPath))
		{
			if (!ForceDirectories(m_strGameMenuResPath))
			{
				::MessageBox(0, L"��ȡ��Ϸ��Դ·��ʧ�ܣ�", L"����", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strGameMenuResPath;
}

// ��ȡС����·����
string CConfigFile::GetMinProgramFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strMinProgramFilePath))
		{
			if (!ForceDirectories(m_strMinProgramFilePath))
			{
				::MessageBox(0, L"��ȡС����·��ʧ�ܣ�", L"����", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strMinProgramFilePath;
}

// ��ȡ����·����
string CConfigFile::GetPolicyFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strPolicyFilePath))
		{
			if (!ForceDirectories(m_strPolicyFilePath))
			{
				::MessageBox(0, L"��ȡ����·��ʧ�ܣ�", L"����", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strPolicyFilePath;
}

// ��ȡƽ̨������·����
string CConfigFile::GetPlatformUpdateFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strPlatformUpdateFilePath))
		{
			if (!ForceDirectories(m_strPlatformUpdateFilePath))
			{
				::MessageBox(0, L"��ȡƽ̨������·��ʧ�ܣ�", L"����", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strPlatformUpdateFilePath;
}

void CConfigFile::LoadConfig()
{
	char szTemp[MAX_PATH] = { 0 };
	wchar_t szTempW[MAX_PATH] = { 0 };
	string strFileName = CreateFullPathName(CONFIG_FILENAME);
	// ��ȡ������
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MACHINENAME, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strMachineName = szTemp;
#ifdef _DEBUG
	m_strMachineName = "035";
#endif
	if (m_strMachineName.empty())
	{
		// ������
		std::string strName;
		char szComputerName[51] = { 0 };
		DWORD dwSize = 50;
		::GetComputerNameA(szComputerName, &dwSize);
		m_strMachineName = szComputerName;
		size_t nPos = m_strMachineName.find("PC");
		if (string::npos != nPos)
		{
			m_strMachineName.erase(nPos, 2);
		}
		//m_strMachineName.erase('C');
	}

	// ��ȡ����UDP�����˿�
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_LOCALUDPLISTENPORT, "7001", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_nLocalUdpPort = StrToInt(szTemp, 7001);
	// ȡ���ͻ��˰汾
	wstring strTemp = GetAppPathName();
	m_strClientVer = GetFileVersionEx(WideCharToMultiCharW(strTemp));
	//GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CLIENTVER, "1.0.0.1", szTemp, MAX_PATH - 1, strFileName.c_str());
	//m_strClientVer = szTemp;
	// ȡ��tcp��������˿�
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_TCPLISTENKEY, "7070", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerTcpPort = szTemp;
	// ȡ��udp��������˿�
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_UDPLISTENKEY, "5050", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerUdpPort = szTemp;
	// ȡ�����ķ���IP
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_UDPIP, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerIp = szTemp;
	// ȡ��HTTP��������˿�
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_HTTPPORT, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerHttpPort = szTemp;
	// ��ȡ�ͻ�����Ҫ�ĵ�ַ
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_ClIENTIP, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strClientIp = szTemp;
	// ͼƬ�ļ�·��
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_IMAGEPATH, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strImagePath = szTemp;
	if (m_strImagePath.empty())
	{
		m_strImagePath = "\\image";
		m_strImagePath = CreateFullPathName(m_strImagePath);
	}
	// ȡMAC,Ip
	GetSysMacAndIp();
	// ȡӲ�̱��
	GetSysDiskNum();

	// BarId
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_NETBARID, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strBarId = szTemp;

	// ����Id
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_AGENCYID, "agencyid", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strAgencyId = szTemp;

	// ��Ϸ�˵�����ȫ��
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMEMENU, "\\GameMenu\\myoubox.exe", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenu = szTemp;
	m_strGameMenu = CreateFullPathName(m_strGameMenu);
	// �˵��汾
	m_strMenuVer = GetFileVersionEx(m_strGameMenu);

	// ��Ϸ��Դ·��
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERESPATH, "\\GameMenu\\Game", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenuResPath = szTemp;
	m_strGameMenuResPath = CreateFullPathName(m_strGameMenuResPath);

	// ��Ϸ����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERLIB, "\\GameMenu\\Game\\game.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameLibFileName = szTemp;
	m_strGameLibFileName = CreateFullPathName(m_strGameLibFileName);

	// ��Ϸ������
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERTYPE, "\\GameMenu\\Game\\type.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameTypeFileName = szTemp;
	m_strGameTypeFileName = CreateFullPathName(m_strGameTypeFileName);

	// ��Ϸ��������	
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERSORT, "\\GameMenu\\Game\\Index.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strSortFileName = szTemp;
	m_strSortFileName = CreateFullPathName(m_strSortFileName);

	// ��ȡС����·����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MINPROGRAM, "\\MinProgram", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strMinProgramFilePath = szTemp;
	m_strMinProgramFilePath = CreateFullPathName(m_strMinProgramFilePath);

	// ��ȡ����·����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_POLICYPATH, "\\Policy", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strPolicyFilePath = szTemp;
	m_strPolicyFilePath = CreateFullPathName(m_strPolicyFilePath);

	// ƽ̨������·����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_PLATFORMUPDATE, "\\PlatformUpdate", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strPlatformUpdateFilePath = szTemp;
	m_strPlatformUpdateFilePath = CreateFullPathName(m_strPlatformUpdateFilePath);

	// �����ݷ�ʽ����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_LINKFILENAME, "myoubox", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strLinkFileName = szTemp;

	// �Ƿ���������ͼ��
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CREATEDESKICON, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCreateDeskIcon = StrToInt(szTemp, 1) == 0 ? false : true;
	
	// �Ƿ���ò˵�
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CALLMENU, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCallMenu = StrToInt(szTemp, 1) == 0 ? false : true;

	// �Ƿ����P2P
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CALLP2P, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCallP2P = StrToInt(szTemp, 1) == 0 ? false : true;

	// �Ƿ�����ͼ���
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_DLICONLIB, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bDownloadIconlib = StrToInt(szTemp, 1) == 0 ? false : true;

	// ������������ɨ�����ڣ���λ���ӣ�0 - ��ɨ�裩
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_DLCONFIGINTERVAL, "0", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_nDownloadConfigInterval = StrToInt(szTemp, 0);

	// ��Ϸ�˵�����
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MENUTYPE, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenuType = szTemp;
}

// ����ȫ·����
std::string CConfigFile::CreateFullPathName(std::string strFileName)
{
	std::string strPath = strFileName;
	if (strPath.find(':') != 0)
	{
		// ���·��
		int nOff(0);
		if (strFileName[0] != '\\' && strFileName[0] != '/')
		{
			nOff = 1;
		}
		char szName[MAX_PATH] = { 0 };
		::GetModuleFileNameA(0, szName, MAX_PATH - 1);
		strPath = szName;
		int nIndex = strPath.rfind('\\');
		if (nIndex < 0) nIndex = strPath.rfind('/');
		strPath = strPath.substr(0, nIndex + nOff);
		strPath += strFileName;
	}
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] == '\\' || strPath[strPath.size() - 1] == '/')
		{
			strPath = strPath.substr(0, strPath.size() - 1);
		}
	}
	return strPath;
}

// ȡMAC
void CConfigFile::GetSysMacAndIp()
{
	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	DWORD AdapterInfoSize;
	int nResult;
	AdapterInfoSize = 0;
	if ((nResult = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
	{
		if (nResult != ERROR_BUFFER_OVERFLOW)
		{
			return;
		}
	}
	if ((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
	{
		return;
	}
	if ((nResult = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	{
		GlobalFree(pAdapterInfo);
		return;
	}
	pAdapt = pAdapterInfo;
	while (pAdapt)
	{
		std::string strIpTemp = pAdapt->IpAddressList.IpAddress.String;
		if (strIpTemp.compare("0.0.0.0") == 0)
		{
			pAdapt = pAdapt->Next;
			continue;
		}
		if (pAdapt->Type == MIB_IF_TYPE_ETHERNET)
		{
			//������������
			std::string strDescription;
			strDescription = pAdapt->Description;
			std::transform(strDescription.begin(), strDescription.end(), strDescription.begin(), tolower);
			if ((strDescription.find("virtual") != std::string::npos) && MIB_IF_TYPE_ETHERNET != pAdapt->Type)
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			int nFindIndex = strDescription.find("vmware");
			if (nFindIndex >= 0)
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			std::string strLocalIP = pAdapt->IpAddressList.IpAddress.String;
			if (strLocalIP == "0.0.0.0")
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			//����ͷ��������MACΪ0
			BYTE btBuffer[6] = { 0 };
			if (memcmp(pAdapt->Address, btBuffer, 6) == 0)
			{
				pAdapt = pAdapt->Next;
				continue;
			}

			int iAddressLength = min(pAdapt->AddressLength, 6);
			for (int j = 0; j < iAddressLength; j++)
			{
				btBuffer[j] = pAdapt->Address[j];
			}
			char szMac[13] = { 0 };
			sprintf_s(szMac, 13, "%02x%02x%02x%02x%02x%02x", pAdapt->Address[0], pAdapt->Address[1], pAdapt->Address[2], pAdapt->Address[3], pAdapt->Address[4], pAdapt->Address[5]);
			m_strMac = szMac;
			m_strIp = pAdapt->IpAddressList.IpAddress.String;
			break;
		}
		else if (71 == pAdapt->Type)
		{
			char szMac[13] = { 0 };
			sprintf_s(szMac, 13, "%02x%02x%02x%02x%02x%02x", pAdapt->Address[0], pAdapt->Address[1], pAdapt->Address[2], pAdapt->Address[3], pAdapt->Address[4], pAdapt->Address[5]);
			m_strMac = szMac;
			m_strIp = pAdapt->IpAddressList.IpAddress.String;
			break;
		}
		pAdapt = pAdapt->Next;
	}
	GlobalFree(pAdapterInfo);
}

// ȡӲ�̱��
void CConfigFile::GetSysDiskNum()
{
	//m_strDiskNum = diskseri::getHardDriveComputerID();
	//EraseChar(m_strDiskNum, ' ');
}


