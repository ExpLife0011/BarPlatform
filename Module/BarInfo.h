#pragma once

#include "gclass.h"
#include "CommonStruct.h"
#include "netbase.h"
typedef struct tag_MEM_CONFIG_INFO_STRUCT
{	
	// clientId
	char clientId[20];
	// ����Ip
	char localIp[16];
	// �������
	char agencyId[40];
	// �ϴ�URL
	char szUploadUrl[512];
	// �˵�����
	char szMenuName[40];
	//char szMoney[40];              
}CLIENT_CONFIG_INFO, *LP_CLIENT_CONFIG_INFO, *P_CLIENT_CONFIG_INFO;

class CBarInfo : public CSafeSingle<CBarInfo>
{
public:
	// ��ȡ��Ϸ��
	void GetGameLib();
	// �Ƿ�ȡ����Ϸ��
	bool IsGetGameLib(){ CSafeLock<CBarInfo> lk(this); return m_bGetGameLib; }
	// ������Ϸ��
	void SetGetGameLib(list<GameLibItem> &ltGame){ CSafeLock<CBarInfo> lk(this); m_ltGame = ltGame; }
	// ��ȡ����
	void GetConfig();
	// ɨ��������Ϸ�˵��ϴ�
	void ScanGameMenuAndUpload();
	// �Ƿ�ȡ������
	bool IsGetConfig(){ CSafeLock<CBarInfo> lk(this); return m_bGetConfig; }
	// ��������
	ConfigDataItem GetLocalConfig()
	{
		CSafeLock<CBarInfo> lk(this);
		return m_cfg;
	}
	// ��������
	void SetConfig(const ConfigDataItem &cfg)
	{
		CSafeLock<CBarInfo> lk(this);
		m_cfg = cfg;
		m_bGetConfig = true;
	}
	string GetTalkUrl()
	{
		CSafeLock<CBarInfo> lk(this);
		return m_cfg.talking_url;
	}
private:
	// �Ƹ����Ƿ���֤·�����أ�true - ��֤��
	bool m_bCloudSWitch = true;
	// �Ƹ��������̷���-ʵ���̷�����ֵ��
	map<string, string> m_mapDiskName;
	// �Ƹ�����Ϸ��-��Ϸ����ID��ֵ��
	map<string, string> m_mapLocalId;
	// �Ƹ�����Ϸ��-·������ֵ��
	map<string, string> m_mapNamePath;
	// �Ƿ�ȡ����Ϸ��
	bool m_bGetGameLib = false;
	// ��Ϸ����
	list<GameTypeItem> m_ltGameType;
	// ��Ϸ��
	list<GameLibItem> m_ltGame;
	// ��������
	ConfigDataItem m_cfg;
	// �����ļ��ڴ���
	HANDLE m_hConfigMapping = 0;
	// ��������
	// �Ƿ�ȡ������
	bool m_bGetConfig = false;
	// ����������
	ConfigItem m_configItem;
	// ����������Ϣ���ڴ�
	void SaveInfoToMem(const ConfigDataItem &cfg);
	// ��ȡ����
	void GetConfigI();
	// ��ȡ����
	void GetConfigII();
	// ��ȡ�˵�
	void GetMenu();
	// ��ȡС����
	void GetMinProgram();
	// ��ȡ��Ϸ��
	void GetGameLibI();
	// ɨ����Ϸ���»�����
	void ScanGameMenuEnv();
	// ���潫��Ϸ����Ϣ���ļ�
	void SaveGameLibInfoToFile();
	// ���潫��Ϸ������Ϣ���ļ�
	void SaveGameTypeInfoToFile();
	// ������Ϸ��
	void FilterGameLib();
	// ��ȡ��Ϸ��������
	string GetGameTypeLevel1Id(const string &strType); 
	// �Ƿ����ָ������Ϸ���
	bool IsExistGameId(int64 nGameId);
	// ���ļ��н�������Ϸ������
	void GetGameLibFromDownloadFile(const string &strFileName, string &strRet);
	// ���Ƹ���ע����н�������Ϸ������
	void GetGameLibFromCloudUpdate();
	// �����Ƹ�����Ϸ��
	void FilterCloudUpdateGameLib();
	// ����˳����Ϸ��
	void FilterShunwangGameLib();
	// ȡƴ��
	string GetCharSpellCode(const string &strSource);
	string GetCharSpellCodeI(wchar_t wChar);
	// ���ص������ؼ�
	void LoadThreadPlug();
	// ��ȡ������
	wstring GetProcessNameById(DWORD dwId);
private:
	CBarInfo();
	~CBarInfo();
	friend class CSingle<CBarInfo>;
};

