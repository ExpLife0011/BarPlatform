// csprotocol.h
#ifndef _CSPROTOCOL_H
#define _CSPROTOCOL_H

#include "gconst.h"
#include "datatypedef.h"
//////////////////////////////////////////////////////////////////////////////

#define RET_SUCCESS         0
#define RET_FAILD           -1

// �Ѽ�������
#define RET_ALREAD_ADDNET   100
// �ȴ���������
#define RET_WAITTING_ADDNET   10

//----------------------------------------------------------------------------
//-- ��������:

//�ͻ��� -> �ͻ���: (ռ�ô����: 1 .. 100)
const int CN_DEBUG = 1;						// ����							
const int CN_LOGIN = 2;						// ��������							
const int NC_LOGIN = 3;						// ��Ӧ��������	����ͨ�ڵ��Ӧ��¼��						
const int CC_NOTIFYINSERT = 6;				// ֪ͨ��Χ�������½ڵ��������						
const int CC_KEEPALIVE = 8;					// 						
const int CC_REKEEPALIVE = 9;				// 						
const int CN_NOTIFYCHANGEMACHINE = 10;      // ֪ͨ���л�������ڵ������ı�						
const int NC_NOTIFYCHANGEMACHINE = 11;      // ֪ͨ���л�������ڵ������ı�ظ�						
const int CN_LOGINSERVER = 12;				// ֪ͨ��¼���ķ�����							
const int NC_LOGINSERVER = 13;				// ֪ͨ��¼���ķ�����							
const int CS_LOGINSERVER = 14;				// ֪ͨ��¼�Ƿ�ɹ�							
const int SC_LOGINSERVER = 14;				// ֪ͨ��¼�Ƿ�ɹ�							
const int CN_CHAT = 15;						// ����������Ϣ							
const int CC_CHAT = 16;						// ˽��������Ϣ		
const int CC_DCHAT = 17;					// ��ָ̨����Ļ
const int CC_GAMEINFO = 18;                 //��̨�ɼ���Ϸ��Ϣ
const int SC_GAMEINFO = 19;       
const int SC_JIONGAME = 20;                    //������˽��뷿��
const int CC_GAMEINFOEX = 21;                    // ��Ϸ��Ϣ(�ͻ��˴򿪡��˳���Ϸ���ģ��)

const int CN_COMMON_UPDATEDATA = 30;			// �����ݸ��£� �磺����������Ϣ����

const int CCN_LOGINSERVER = 50;				// ����̨֪ͨ��¼���ķ�����							
const int CCN_CALLSERVER = 51;				// ����̨֪ͨ��¼���ķ�����							

// ͨ��Ӧ��
const int CC_ACK = 1000;      // ͨ��Ӧ��						

#pragma pack(1)     //1�ֽڶ���
//----------------------------------------------------------------------------
//-- ���ݰ��ײ�:

struct CPacketHeader
{
    word nActionCode;       // ��������
    byte nProtoVer;         // Э��汾
	word nSeri;				// �������
    word nPacketLen;        // ����ȫ��
    uint nCheckSum;          // У���

private:
    int GetCheckSum()
	{
		int nChek = 0;// nProtoVer ^ nActionCode ^ nSeri ^ nPacketLen;
		for (int i = 0; i < sizeof(CPacketHeader)-4; i++)
		{
			unsigned char *p = (unsigned char *)this;
			nChek ^= *(p + i);
		}
		for (int i = sizeof(CPacketHeader); i < nPacketLen; i++)
		{
			unsigned char *p = (unsigned char *)this;
			nChek ^= *(p + i);
		}
		return nChek;
	}
public:
	void InitHeader(word nActionCodeA, word nSeriA, word nPacketLenA, int nProtoVerA = 0)
	{
		nProtoVer = nProtoVerA;
		nActionCode = nActionCodeA;
		nSeri = nSeriA;
		nPacketLen = nPacketLenA;
		nCheckSum = GetCheckSum();
	}
	bool CheckHeaderIsValid(){ return (GetCheckSum() == nCheckSum) && (nPacketLen >= sizeof(CPacketHeader)); }
};

#pragma pack()



#endif
