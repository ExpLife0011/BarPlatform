#ifndef _DATATYPEDEF_H
#define _DATATYPEDEF_H

#include <map>
#include <time.h>
#include <string>
#include <list>
#include <queue>

#include "gconst.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
// ���Ͷ���

typedef unsigned char       byte;
typedef unsigned short      word;
typedef unsigned int        dword;
typedef unsigned int        uint;
#ifdef _WINDOWS
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
typedef long long			int64;
typedef unsigned long long	uint64;
#endif

#pragma pack(1)     //1�ֽڶ���
// IP - Port �ṹ
struct NetAddr
{
	char sIp[16];           // ��������IP��ַ (�����ֽ�˳��)
	word nPort;             // ��������UDP�˿�
	NetAddr() :nPort(0) { memset(sIp, 0, 16); }
	NetAddr(const char *ip, word wport) :nPort(wport) { memcpy(sIp, ip, 15); sIp[15] = 0; }
};

#pragma pack()     //1�ֽڶ���

//// ��������
//struct CMatchDetail
//{
//	// {"KDA":"4",
//	string strKDA;
//	// "aReward":"0",
//	string strReward;
//	// "area":"��ŷ����",
//	string strArea;
//	// "assists":"0",
//	string strAssists;
//	// "beginTime":"22",
//	string strBeginTime;
//	// "damTaken":"32121",
//	string strDamTaken;
//	// "damToChan":"1213",
//	string strDamToChan;
//	// "df":"���ֵ�ȼ",
//	string strDf;
//	// "die":"12",
//	string strDie;
//	// "game":"Ӣ������",
//	string strGame;
//	// "gameName":"���ӽ�",
//	string strGameName;
//	// "killed":"1",
//	string strKilled;
//	// "mvp":"0",
//	string strMvp;
//	// "playTime":"30",
//	string strPlayTime;
//	// "qita":"",
//	string strQita;
//	// "result":"1",
//	string strResult;
//	//// "shopName":"1",
//	//string strBarName;
//	// "shopNo":"1",
//	string strBarId;
//	// "userId":"2",
//	string strUserId;
//	// "userName":"das",
//	string strUserName;
//	// "zhuangBei":"���ϴ��ϵĿռ䰡�Ǵ�"}]
//	string strZB;
//	//
//	//
//};
//// ��������û���Ϣ
//struct MatchUserInfo
//{
//	string id;		// �û���ʶ
//	string objname;//����
//	string netbarid;//����ID
//	string netbarname;//��������
//	string gamename;//��Ϸ������
//};
//
//// ��������
//struct CMatchCommonInfo
//{
//	// "id" : "", 
//	string id; //����ID
//	// "objname" : "liukun�ķ���", 
//	string objname;//��������
//	// "netbarid" : "testbarid", 
//	string netbarid;//����ID
//	// "netbarname" : "testbarname", 
//	string netbarname;//������
//	// "matchpwd" : "111111", 
//	string matchpwd;//��������
//	// "tiaozhanj" : 100,
//	int tiaozhanj = 0;//��ս��
//	// "dashangj" : 0, 
//	int dashangj = 0;//���ͽ�
//	// "pipei" : "10", 
//	string pipei;//ƥ��ģʽ
//	string dashangw;//������
//	// "youxi" : "1" }
//	string youxi;//��Ϸ
//	// "daqu" : "3109", 
//	string daqu;//����
//	string objstatus;//״̬
//	// { "createtime":"2016-06-19 13:37:51", 
//	string createtime;//����ʱ��
//	// "opt" : "", 
//	string opt = "";//���������������룬�˳���
//	// "starttime" : "", 
//	string starttime = "";//������ʼʱ��
//	// "endtime" : "", 
//	string endtime = "";//��������ʱ��
//	// "leaderid" : "testuserinfo", 
//	string leaderid = "";//����id
//	// objtype("0" - ���ɣ� ��1�� - ���)
//	string objtype;
//	// ģʽ
//	string gamemode;
//	// �����󶨵Ļid
//	string gameactiveid;
//	// �����󶨵Ļ�ǲ���Ҫ��ת
//	string gameisjump;
//	// "users" : ["testuserinfo"], 
//	list<MatchUserInfo> ltUsers;//��ԱID�б�
//};
// ��Ϸ������
struct GameTypeItem
{
	// ��Ϸ����ID(����int)
	string strId;
	// ��Ϸ��������
	string strName;
	// ��Ϸ������ID
	string strParentId;
};

// ��Ϸ��������
struct GameLibItem
{
	//// ���������һ�£����ؿ�����
	//int type;
	// ��Ϸ��ţ�������Ϸ�����Ϸ��� center_game_id
	int64  center_game_id;// game_id;
	// ��Ϸ���ƣ�����鿴
	string game_name;
	// ��Ϸ����ƴ�����������
	string name_py;
	// ��Ϸͼ�����·��
	string game_icon;
	// �����������·��
	string launcher;
	// �������·��
	string process;
	// ��Ϸ�����;
	int64  game_dp = 0;
	// ��Ϸ���࣬Ҳ����Ϸ����Ŀ¼
	string category_dir;
	// ��Ϸһ������
	string category_1;
};

// ��λ����Ϸ����
struct SortGameItem
{
	// ��Ϸ��ţ�������Ϸ�����Ϸ���
	int64 game_id;
	// ��Ϸ���ƣ�����鿴
	string game_name;
	// ��������URL�����ر���ʱ��ȥ��md5�ַ���
	string launcher;
	// ��������md5�ַ���
	string launchermd5;
	// ��ѡ��ͼ��URL��������PNG/ICO/EXE/DLL�������EXE/DLL�������Դ���ֻ�ȡ
	string icon;
	// icon md5�ַ���
	string iconmd5;
	// ��Ӧ�����ļ���
	string iconpathname;
	// ��Ӧ�����ļ���
	string launchpathname;
	// ��λ˳���
	int rank;
	// ��־λ 0 ��ʱ������ͨ��1��ʱ���Ǹ�����ʾ
	int flag = 0;
	// ����URL
	string popurl;
	// ���� md5�ַ���
	string popmd5;
	// �����Ӧ�����ļ���
	string poppathname;
};

// ��λ������
struct SortDataItem
{
	// ����������ڣ�yyyy-MM-dd��ʽ���ַ���
	string strDate;
	list<SortGameItem> ltGame;
};

// ����������
struct UpLevelDataItem
{
	// ������������
	int type;
	// �汾���е�������ڣ������ж��Ƿ���Ҫ����
	string ver;
	// ����URL
	string url;
	// ��λ�ֽڣ�B���������ļ��Ĵ�С
	int size;
	// ��Ӧ�����ļ���
	string pathname;
	// md5
	string md5;
};

// С��������
struct ProgramDataItem
{
	// С�������ͣ�Ĭ��1
	int type;
	// �汾���е�������ڣ������ж��Ƿ���Ҫ����
	string ver;
	// ����URL
	string url;
	// ��λ�ֽڣ�B���������ļ��Ĵ�С
	int size;
	// ��Ӧ�����ļ���
	string pathname;
	// md5
	string md5;
};

// ��Դ����
struct ResourceDataItem
{
	// "resources":[{"size":20984703,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/game_icons.zip","md5":"2cf0c87d74f333de1e6935c0ca82d207"}]
	// ��λ�ֽڣ�B���������ļ��Ĵ�С
	int size;
	// ��Դ���ͣ�Ĭ��1-��Ϸͼ��; 100-˳����Ϸ�⣻200-������Ϸ�⣻300-������Ϸ�⣻400-�Ƹ�����Ϸ��
	int type;
	// ����URL
	string url;
	// md5
	string md5;
	// ��Ӧ�����ļ���
	string iconpathname;
};

// ��������
struct ConfigDataItem
{
	// ��Դ�б�
	list<ResourceDataItem> ltRD;
	// ��λ��
	list<SortDataItem> ltSD;
	// �������б�
	list<UpLevelDataItem> ltUD;
	// С�����б�
	list<ProgramDataItem> ltPD;
	// �ϱ���URL
	string talking_url;
	// ��Ϸ��ݷ�ʽ������
	string game_menu;

	// ��ȡ��Ӧ��ͼ�걾���ļ�ȫ��
	string GetIconPathName(const string &strGameId, const string &strIconMd5)
	{
		string strRet;
		for (auto it : ltRD)
		{
			if (strIconMd5.empty())
			{
				//if ()
			}
		}
		return strRet;
	}
};

// ��Ϸ������
struct ConfigIIDataItem
{
	// С�������ͣ�Ĭ��1
	int type;
	// ����URL
	string url;
	// md5
	string md5;
	// ��λ�ֽڣ�B���������ļ��Ĵ�С
	int size;
	// ��Ӧ�����ļ���
	string param;
};

// ��Ϸ������
struct ConfigIIItem
{
	// ���ؽ����0 - �ɹ���
	int ret_code;
	string ret_msg;
	// �����б�
	list<ConfigIIDataItem> lt;
};


#endif
