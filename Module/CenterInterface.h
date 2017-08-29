/******************************************************************************
* ��Ȩ���� (C)2012, Sandongcun������
*
* �ļ����ƣ�centerinterface.h
* ����ժҪ�����Ľӿ�
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
#include "WebSocket.h"
#include "CommonStruct.h"

#pragma pack(1)
typedef struct tag_WY_COMMU_HEAD
{
	UINT32	_head_flag;			//��ͷ
	INT32	_seq_no;			//�������ɣ���Ӧ��ԭֵ����  ������������ģ��������Ƿ�����ֱ������
	UINT32	_command_id;		// ָ��ID
	UINT32	_data_len;			//������ͷ���ȣ�����Ϣ��ʱ������Ϊ0
	UINT32	_checksum;			//Ԥ��checksum���Ժ�У��
}CCommonTCPPacket;

#pragma pack()	//	end #pragma pack(1)



class CCenterInterface
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ӿ�
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ȡ���°�����
	static string GetUpdate();
	// ��ȡ����
	static string GetConfig();
	// ��ȡ����
	static string GetConfigII();
	// ��ȡ�˵����°�
	static string GetMenuUpdate();
	// ��ȡС����
	static string GetMinProgram();
	// ��ȡ��Ϸ��
	static string GetGameLib();
	// �ϴ��������� ��10 - ˳���� 11 - �Ƹ��£� 12 - ���Σ� 13 - ���ӣ�14 - �����ӣ�15 - �̻���ʦ��
	static string UploadInfo(const string &strUrl, const string strAction = "1");
	// �ӱ���������Դ�ļ�������
	static bool DownloadFromP2P(const string &strUrl, const string &strFileName, const string &strMd5);
	// ������Դ�ļ�������
	static bool DownloadFile(const string &strUrl, const string &strFileName, const string &strMd5);

public:
	CCenterInterface();
	~CCenterInterface();
private:
	// ��ȡHttp����
	static string GetHttpData(const string &strUrl, map<string, string>mapParam, bool bBigRet = false, bool bIsPost = false);
	// ��ȡHttp������������
	static string GetHttpDataUrl(const string &strPath);
	// �ص�����ָ������
	// ������
	//   pCallbackParam - �����ص��������ϲ������
	//   pSreamSocket	- ������SreamSocket����ָ�룻
	//   pRemoteIp		- �������е�Զ��Ip��
	//   nRemotePort	- �������е�Զ�̶˿ڣ�
	static void WINAPI ClientProcTcpConnect(void *pCallbackParam, void *pSreamSocket, char *pSendBuf, int nBufLen);
};

