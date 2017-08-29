/******************************************************************************
* ��Ȩ���� (C)2012, Sandongcun������
*
* �ļ����ƣ�configfile.h
* ����ժҪ������ģ��
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2014.7.28
* �޸ļ�¼��
* 1.
* �޸����ڣ�
* �� �� �ţ�
* �� �� �ˣ�
* �޸����ݣ�
******************************************************************************/
#pragma once

#include "gclass.h"

class CConfigFile :
	public CSafeSingle<CConfigFile>
{
	//public:
public:
	// ת��Ϊ����·�� 
	static void SwitchAbsPath(string &strPath);
public:
	// ��ȡ�Ƿ���������ͼ��
	bool GetIsCreateDeskIcon() { return m_bCreateDeskIcon; }
	// �Ƿ���ò˵�
	bool GetIsCallMenu() { return m_bCallMenu; }
	// �Ƿ����P2P
	bool GetIsCallP2P() { return m_bCallP2P; }
	// �Ƿ�����ͼ���
	bool GetIsDLIconLib() { return m_bDownloadIconlib; }
	// ������������ɨ�����ڣ���λ���ӣ�0 - ��ɨ�裩
	int GetDLConfigInterval() { return m_nDownloadConfigInterval; }

	// ��ȡ����Id
	string GetAgencyId() { return m_strAgencyId; }
	// ������Ϸ����
	void SetGameMenuType(const string &strGameMenuType) { m_strGameMenuType = strGameMenuType; }
	// �Ƿ񷽸���
	bool IsFGZ(){ return m_strGameMenuType.compare("500") == 0; }
	// �Ƿ��Ƹ���
	bool IsClundUpdate(){ return m_strGameMenuType.compare("400") == 0; }
	// �Ƿ�˳��
	bool IsShunwang(){ return m_strGameMenuType.compare("100") == 0; }
	// ��ȡ��Ϸ����
	string GetGameMenuType() { return m_strGameMenuType; }
	// ��ȡBarId
	string GetBarId() { return m_strBarId; }
	// ��ȡMAC
	string GetMac() { return m_strMac; }
	// ��ȡ����IP
	string GetLocalIp() { return m_strIp; }
	// ��ȡ��Ϸ�˵�����ȫ��
	string GetGameMenuProgram() { return m_strGameMenu; }
	// ��ȡ��Ϸ��Դ·��
	string GetGameResPath();
	// ��ȡ��Ϸ����
	string GetGameLibFileName() { return m_strGameLibFileName; }
	// ��ȡ��Ϸ������
	string GetGameTypeFileName() { return m_strGameTypeFileName; }
	// ��ȡ��Ϸ��������
	string GetGameSortFileName() { return m_strSortFileName; }
	// ��ȡС����·����
	string GetMinProgramFilePath();
	// ��ȡ����·����
	string GetPolicyFilePath();
	// ��ȡƽ̨������·����
	string GetPlatformUpdateFilePath();
	// ��ȡ�����ݷ�ʽ����
	string GetLinkFileName() { return m_strLinkFileName; }

	// ���ķ���UDP�����˿�
	int GetServerUdpPort() { return atoi(m_strServerUdpPort.c_str()); }
	// ���ķ������ip
	string GetServerIp() { return m_strServerIp; }
	// ���ķ���HTTP�����˿�
	string GetServerHttpPort() { return m_strServerHttpPort; }
	// ���ķ���HTTP����IP�˿�
	string GetServerHttpIpPort() {

		string s = m_strServerIp;
		if (!m_strServerHttpPort.empty())
		{
			s += ":";
			s += m_strServerHttpPort;
		}
		return	s;
	}
	// ���ķ���HTTP����IP�˿�
	string GetClientIp() {
		return	m_strClientIp;
	}

	//// ���ɷ�������˿�
	//int GetServerTcpPort(){ return atoi(m_strServerTcpPort.c_str()); }
	//string GetServerTcpPortStr(){ return m_strServerTcpPort; }
	//// ע��ӿ�URL
	//string GetRegeditAddr(){ return m_strRegeditAddr; }
	// ����˰汾
	string GetServerVer() { return m_strServerVer; }
	void SetServerVer(string &strServerVer) { m_strServerVer = strServerVer; }
	// �ͻ��˰汾
	string GetClientVer() { return m_strClientVer; }
	// �˵��汾
	string GetMenuVer() { return m_strMenuVer; }

	// ��ȡͼƬ�ļ�·��
	string GetImageFilePath() { return m_strImagePath; }
	// �������������ļ�
	void LoadUpdateConfig(const string &strUpdateFileName);

	// ��ȡ����UDP�����˿�
	word GetLocalUDPListenPort() { return m_nLocalUdpPort; }
	// ��ȡ������
	string GetLocalMachineName() { return m_strMachineName; }

	// ����ȫ·����
	std::string CreateFullPathName(std::string strFileName);
private:
	void	LoadConfig();

	// ȡMAC,Ip
	void GetSysMacAndIp();
	// ȡӲ�̱��
	void GetSysDiskNum();
private:
	// �Ƿ���ò˵�
	bool m_bCallMenu = true;
	// �Ƿ����P2P
	bool m_bCallP2P = true;
	// �Ƿ�����ͼ���
	bool m_bDownloadIconlib = true;
	// ������������ɨ�����ڣ���λ���ӣ�0 - ��ɨ�裩
	int m_nDownloadConfigInterval = 0;


	// �Ƿ���������ͼ��
	bool m_bCreateDeskIcon = true;
	// ������Ϸ��������(��֪���ؿ����ͣ�100-˳����200-���ӣ�300-���Σ�400-�Ƹ���)
	string m_strGameMenuType; 
	// ���ķ���IP
	string m_strServerIp;
	// ���ķ���UDP�����˿�
	string m_strServerUdpPort;
	// ���ķ���TCP�����˿�
	string m_strServerTcpPort;
	// ���ķ���HTTP�����˿�
	string m_strServerHttpPort;
	// ����Id
	string  m_strAgencyId; 
	// ��Ϸ�˵�����ȫ��
	string  m_strGameMenu;
	// ��Ϸ��Դ·��
	string  m_strGameMenuResPath;
	// ��Ϸ����
	string  m_strGameLibFileName;
	// ��ȡ��Ϸ������
	string  m_strGameTypeFileName; 
	// ��Ϸ��������
	string  m_strSortFileName;
	// ��ȡС����·����
	string m_strMinProgramFilePath;
	// ����·����
	string m_strPolicyFilePath;
	// ƽ̨������·����
	string m_strPlatformUpdateFilePath;
	// �����ݷ�ʽ����
	string m_strLinkFileName;

private:
	// BarId
	string	m_strBarId;
	// MAC��ַ
	string	m_strMac;
	// Ip��ַ
	string m_strIp;
	// Ӳ�̱��
	string	m_strDiskNum;
	// ����˰汾
	string  m_strServerVer;
	// �ͻ��˰汾
	string  m_strClientVer;
	// �˵��汾
	string m_strMenuVer; 
	// ͼƬ�ļ�·��
	string  m_strImagePath;
	// ����UDP�����˿�
	word m_nLocalUdpPort;
	// ������
	string m_strMachineName;
	// ��ȡ�ͻ�����Ҫ�ĵ�ַ
	string  m_strClientIp;
private:
	CConfigFile();
	~CConfigFile();

	friend class CSingle<CConfigFile>;
};

