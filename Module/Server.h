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
#ifndef _SERVER_H
#define _SERVER_H

#include "datatypedef.h"

class CServer
{
public:
	CServer();
	~CServer();
	void Run();
private:
	// 
	bool m_bStop;
	// ��ʼ��
	void init();
	void uninit();
private:
	// ����������Ϣ
	bool LoadConfig();
};


#endif
