
// BarPlatform.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBarPlatformApp: 
// �йش����ʵ�֣������ BarPlatform.cpp
//

class CBarPlatformApp : public CWinApp
{
public:
	CBarPlatformApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBarPlatformApp theApp;
