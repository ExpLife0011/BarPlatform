/******************************************************************************
* ��Ȩ���� (C)2012, Sandongcun������
*
* �ļ����ƣ�imserver.cpp
* ����ժҪ����������
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2012.6.10
* �޸ļ�¼��
* 1.
* �޸����ڣ�
* �� �� �ţ�
* �� �� �ˣ�
* �޸����ݣ�
******************************************************************************/
// Server.cpp : ����Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include "Server.h"

#include "server.h"
#include "configfile.h"
#include "configfileconst.h"
#include "udpserver.h"
//#include "ChannelServer.h"
#include "JsonCode.h"
#include "CenterInterface.h"
#include "BarInfo.h"
//#include "SendRecvServer.h"
//#include "SendApplicationServer.h"

//1.pre_server		�� ������������			UDP���������˿�-6000; UDP���������˿�-7070
#define PRESERVER_LINSTEN_UDPINNERPORT		6000
#define PRESERVER_LINSTEN_UDPOUTERPORT		7070
//2.state_server	�� ״̬��������			UDP���������˿�-6001;
#define STATESERVER_LINSTEN_UDPINNERPORT	6001
//3.group_server	�� Ⱥ���߷�������		UDP���������˿�-6002;
#define GROUPSERVER_LINSTEN_UDPINNERPORT	6002
//4.operator_server	�� ҵ���������			UDP���������˿�-6003; UDP���������˿�-7071 TCP���������˿�7071
#define OPERATORSERVER_LINSTEN_UDPINNERPORT	6003
#define OPERATORSERVER_LINSTEN_UDPOUTERPORT	7071
#define OPERATORSERVER_LINSTEN_TCPOUTERPORT	7071
//5.file_server		�� �ļ���������			UDP���������˿�-6004; TCP���������˿�-7072
#define FILESERVER_LINSTEN_UDPINNERPORT		6004
#define FILESERVER_LINSTEN_TCPOUTERPORT		7072


#include <iostream>
using namespace std;

//// �� - IP��ַ�� ֵ - 
//map<string, ContactPosItem *> m_mapItem;
//ContactPosItem m_cpItem;
//
void WINAPI WebSockClientCallbackFun(const void *pCallbackParam, const string &strRecv)
{
	strRecv;
	//char *szBuf = "test";
	//int nRet = CCommInterface::SendDataByWebSockClient(pWebSocketClient, szBuf, sizeof(szBuf));
	//char szRet[100];
	//int nFlag(0);
	//nRet = CCommInterface::RecvDataFromWebSockClient(pWebSocketClient, szRet, sizeof(szRet), nFlag);
}

void WINAPI WebSockClientCallbackFun2(const void *pCallbackParam, const void *pWebSocketClient)
{
	//char *szBuf = "test";
	//int nRet = CCommInterface::SendDataByWebSockClient(pWebSocketClient, szBuf, sizeof(szBuf));
//	char szRet[100];
	//int nFlag(0);
	//nRet = CCommInterface::RecvDataFromWebSockClient(pWebSocketClient, szRet, sizeof(szRet), nFlag);
}

//#include "MatchAppClient.h"
void test()
{
	//CMatrix::GetInstance()->Init();
	//string strMachine = "001"; //"W6SYL9UKUJMBG6D";
	//CNetAddr addrFrom("192.166.0.244", 7000);
	//// �����ھ�
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// �����ھ�
	//strMachine = "002";// "WYOFFICE_";
	//addrFrom.SetIp("92.168.5.52");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// �����ھ�
	//strMachine = "034";
	//addrFrom.SetIp("192.166.0.34");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// �����ھ�
	//strMachine = "041";
	//addrFrom.SetIp("192.166.0.41");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	////// �����ھ�
	////strMachine = "W6SYL9UKUJMBG6D";
	////addrFrom.SetIp("192.166.0.244");
	////CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);



	string strAction;
	string strMatchInfo;
	string strUserId;
	string strArea;
	string strGameName;
	string strTier;
	string strWinner;
	string strRecv = "{\"ActionName\":\"GetLolMatchInfo\",\"Values\":{\"killlist\":[{\"name\":\"Ҫ����ұ�����\",\"kill\":\"2\"}], \"win\":\"0\"}}";
	//if (CJsonDecode::DecodeLolInfo(strAction, strRecv, strUserId, strArea, strGameName, strTier, strMatchInfo, strWinner))
	//{
	//	if (!strMatchInfo.empty())
	//	{
	//		// �����ķ�����������
	//		CCenterInterface::FinishMatch(strMatchInfo, strWinner);
	//	}
	//}
	//	
		
	string strRet;
	//strRet = CCenterInterface::GetScreenShow();
	//strRet = CCenterInterface::CreateScreenShow("asdfsdfaasdf", "8:00", "asdf", "3|5|7|8|", "0");
	//strRet = CCenterInterface::ModifyScreenShow("11bce23cfce14a6fbdfc366f2a763f53", "aaaaaaaaaa", "9:00", "asdf", "3|5|7|8|9", "1");
	//strRet = CCenterInterface::SetScreenShowSwitch("11bce23cfce14a6fbdfc366f2a763f53", "2");
	//strRet = CCenterInterface::DelScreenShow("11bce23cfce14a6fbdfc366f2a763f53");
	//// {"createtime":"2016-06-21 15:57:09","daqu":"","dashangj":0,"endtime":"","id":"02823c2d5dda4b61bca15ee3e00f9fe2","leaderid":"testuserinfo","matchpwd":"111","modifytime":"2016-06-21 15:57:09","netbarid":"testbarid","netbarname":"testbarName","objname":"111","objstatus":"0","opt":"create","pipei":"6","starttime":"","tiaozhanj":1000,"users":[{"id":"testuserinfo","netbarid":"testbarid","netbarname":"testbarName","objname":"testbarName-004"}],"youxi":""}
	//string s = "{\"createtime\":\"2016-06-21 15:57:09\",\"daqu\":\"\",\"dashangj\":0,\"endtime\":\"\",\"id\":\"02823c2d5dda4b61bca15ee3e00f9fe2\",\"leaderid\":\"testuserinfo\",\"matchpwd\":\"111\",\"modifytime\":\"2016-06-21 15:57:09\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"111\",\"objstatus\":\"0\",\"opt\":\"create\",\"pipei\":\"6\",\"starttime\":\"\",\"tiaozhanj\":1000,\"users\":[{\"id\":\"testuserinfo\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"testbarName-004\"}],\"youxi\":\"\"}";
	////wstring s = L"{ \"ActionName\":\"CreateRoom\", \"Values\" : {\"Area\":\"\", \"Game\" : \"\", \"IsLocalBar\" : \"0\", \"MatchMoney\" : \"1000\", \"PersonNum\" : \"8\", \"RoomName\" : \"111\", \"RoomPass\" : \"111\"} }";
	//       s = "{\"createtime\":\"2016 - 06 - 21 16:32 : 04\",\"daqu\":\"\",\"dashangj\":0,\"endtime\":\"\",\"id\":\"608e0a5920e04e3a8e9a7da7b0973a6a\",\"leaderid\":\"UserTest\",\"matchpwd\":\"111\",\"modifytime\":\"2016 - 06 - 21 16 : 32 : 04\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"111\",\"objstatus\":\"0\",\"opt\":\"create\",\"pipei\":\"6\",\"starttime\":\"\",\"tiaozhanj\":1000,\"users\":[{\"id\":\"UserTest\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"testbarName - 004\"}],\"youxi\":\"\"}";
	//CMatchCommonInfo item;
	//CJsonDecode::DeCodeMatchCommonData(s, item);
	//CMatchAppClient ct(22, L"xxxxx");
	//ct.ProcData(s);
	// ��ȡ�����б�
	//string strInfo = CCenterInterface::GetMatchList("", "", "", "");
	//// ����
	//// ���������ӿ�
	//CCenterInterface::CreateRoom("1", "3109", "liukun�ķ���", "111111", "10", "100", 0, &WebSockClientCallbackFun);
	//Sleep(6000);
	//// ���뷿��ӿ�
	//CCenterInterface::EnterRoom("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// �˳�����ӿ�
	//CCenterInterface::QuitRoom("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// ���ͼ�¼�ӿ�
	//CCenterInterface::SetReward("100", "4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// ��ʼ�����ӿ�
	//CCenterInterface::StartMatch("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// �޸ı����ӿ�
	//CMatchCommonInfo item;
	//item.id = "c85ba5e912f8445a88b3ed59625238b4";
	//item.youxi = "1";
	//item.daqu = "3901";
	//item.objname = "liukun�ķ���";
	//item.matchpwd = "222222";
	//item.dashangj = 100;
	//item.tiaozhanj = 100;
	//item.pipei = "8";
	//CCenterInterface::ModiMatch(item, 0, &WebSockClientCallbackFun);
	//Sleep(1000000);
	//CCenterInterface::GetBroadcastList();
	// �༭��ʱ��������
	//string strRet;// = CCenterInterface::ModifyBroadcastRecord("cb5e90555bc548d9861a163ad8eafde2", "15:00", "0", "1");
	//strRet = CCenterInterface::SetBroadcastSwitch("cb5e90555bc548d9861a163ad8eafde2", "0");
	//// ��ȡ
	//CCenterInterface::GetMatchList("", "", "", "");
	// 
	//const char *sUrl = "ws://192.168.102.186:8080/ybing/websocket";
	//CCommInterface::CreateWebSockClient(sUrl, 0, WebSockClientCallbackFun2);
	//string strRet = CCenterInterface::CreateMatch("LOL001", "3601", "liukun", "111111", 10, "100");
	//int nSex = GetSexById("360103197811194418");
	// [{"id":"111", "jsonrpc":"2.0","method":"play_mp3","params":["Ů��ݵ�.mp3"]},{"jsonrpc":"2.0","method":"play_mp3","params":["1.mp3"],"id":"12255"},{"id":"111","jsonrpc":"2.0","method":"play_mp3","params":["�Ż�.mp3"]},{"id":"111", "jsonrpc":"2.0","method":"play_mp3","params":["Ů�������ǳ�.mp3"]}]
	//strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"��ʾ9.mp3\"],\"id\":\"12255\"},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"��ʾ3.mp3\"],\"id\":\"12256\"}]"; //
	//string strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"��ӭ������������\"],\"id\":\"12255\"}]"; //
	strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_tts\",\"params\":[\"1�Ż���Ҫ���ܰ���\"],\"id\":\"12255\"}]"; //
	//strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB(CConfigFile::GetInstance()->GetLocalMachineName(), 0);
	//strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB("223", 0);
	//strRet = "�װ�����ۣ�������ͨ��ʱ��Ϊ����24������������8�㣬������鿨�����";
	//strRet = CJsonEncode::EnCodeYYBBTTSMessage(strRet);

	const int nRetLen = 10 * 1024;
	char *szRet = new char[nRetLen];
	int nState(0); 
	string s("�װ�����ۣ�������ͨ��ʱ��Ϊ����24������������8�㣬������鿨����Ϊ�˷�ֹ�ظ��۷ѣ��벻Ҫ��������������������ʣ�����ϵ��̨����Ա��");

	////s = "�װ�����ۣ����������ڽ���Ӣ�����˱������з��ƷŶ���Ͽ�Լ�϶�������̨�����ɡ�";
	//strRet = CJsonEncode::EnCodeYYBBMp3Message(s, "");
	////strRet = "[{\"id\":\"\", \"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"50.������ͨ��.mp3\"]},{\"id\":\"\", \"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"51.����.mp3\"]},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"20.mp3\"],\"id\":\"\"},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"4.mp3\"],\"id\":\"\"}]";
	CCommInterface::PostHttpData("http://192.168.102.82:8080/voice_api", strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//CCommInterface::PostHttpData("192.168.101.157", strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//CCommInterface::PostHttpData(CConfigFile::GetInstance()->getYYBBServerUrl().c_str(), strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//string strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB("2", 0);
	//m_cpItem.strMachineName = "003";
	//ContactPosItem *pItem = new ContactPosItem;
	//pItem->strMachineName = "001";
	//ContactPosItem *pItem2 = new ContactPosItem;
	//pItem2->strMachineName = "002";
	//ContactPosItem *pItem4 = new ContactPosItem;
	//pItem4->strMachineName = "004";
	//ContactPosItem *pItem5 = new ContactPosItem;
	//pItem5->strMachineName = "005";
	//m_mapItem["192.168.102.168:7000"] = pItem;
	//m_mapItem["192.168.101.243:7000"] = pItem2;
	//m_mapItem["192.168.102.169:7000"] = pItem4;
	//m_mapItem["192.168.101.244:7000"] = pItem5;
	//CalcNeighbourRef();
	exit(0);
}

CServer::CServer()
//:m_serTcp(OPERATORSERVER_LINSTEN_UDPOUTERPORT)
{
//#ifdef _DEBUG
//	test();
//#endif
	m_bStop = false;
	init();
}

CServer::~CServer()
{
	uninit();
}

// �������� 
void CServer::Run()
{
	// ��ֹ�ظ����г���
	SECURITY_DESCRIPTOR sd;
	if (0 == InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) || 0 == SetSecurityDescriptorDacl(&sd, TRUE, (PACL)0, FALSE))
	{
		return;
	}
	else
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;
		HANDLE hSem = CreateSemaphore(&sa, 1, 1, _T("BarPlatform_Semaphore"));
		//HANDLE hSem = CreateSemaphore(NULL, 1, 1, _T("DW_Bar_Client"));
		DWORD dwCheckThread = GetLastError();
		if (dwCheckThread == ERROR_ALREADY_EXISTS)
		{
			return;
		}
	}
	if (!LoadConfig())
	{
		return;
	}
	////CYYBBClient::GetInstance()->Run();
	//CUserOnlineInfo::GetInstance();
	// ����UDP�������� 
	CUdpServer::GetInstance()->Run();
	//// ����TCP�������� 
	//m_serTcp.Run();

	//CMatrix::GetInstance()->Init();

	//CChannelServer::GetInstance()->StartServer();
	//CSendRecvServer::GetInstance()->Run();
	//CSendAppServer::GetInstance()->Run();
	//CCenterInterface::UploadInfo();
}

// ��ʼ��
void CServer::init()
{
	// ��ǰ��ʼ������ֹ���̳߳��η��ʵ����³���(ʵ�������̷߳�����queue.size() = -64 < 0)
	InitLogs();

	//CUserInfoList::GetInstance();
	CBarInfo::GetInstance();
	CConfigFile::GetInstance();
	CJsonEncode::GetInstance();
	CJsonDecode::GetInstance();
	//CCallServer::GetInstance();
	//CAppClientFactory::GetInstance()->Run();
	//CWebSocket::GetInstance();

}
void CServer::uninit()
{
	//CWebSocket::FreeInstance();
	//CSendAppServer::FreeInstance();
	//CSendRecvServer::FreeInstance();
	//CChannelServer::FreeInstance();
	//CMatrix::FreeInstance();
	//// ֹͣTCP����
	//m_serTcp.Stop();
	// ֹͣUDP����
	CUdpServer::FreeInstance();
	//CCallServer::FreeInstance();
	//CAppClientFactory::FreeInstance();
	CJsonDecode::FreeInstance();
	CJsonEncode::FreeInstance();
	CConfigFile::FreeInstance();
	CBarInfo::FreeInstance();
	//CUserInfoList::FreeInstance();
	//CUserOnlineInfo::FreeInstance();
	FreeLogs();
}

// ����������Ϣ
bool CServer::LoadConfig()
{
	//CConfigFile *p = CConfigFile::GetInstance();
	//if(p->GetCount() != CONFIGFILE_PARAMCOUNT)
	//{
	//	return false;
	//}
	//// ��������IP
	//m_stInnerIp = p->GetStringValueA(CONFIGFILE_INDEX_INNERADDRIP);
	//if(m_stInnerIp.empty())
	//{
	//	return false;
	//}
	return true;
}

