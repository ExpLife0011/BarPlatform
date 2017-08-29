
// BarPlatformDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BarPlatform.h"
#include "BarPlatformDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���ô�ӡ����
LONG SetPrintParam()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//MessageBox(L"SetPrintParam");
	LONG nRet = 1;
	CString strType;
	HKEY hKey = NULL;
	DWORD szType = REG_SZ;
	DWORD dwCount = 1024;
	CString strBuf;
	// HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\PageSetup
	//if (::RegOpenKey(HKEY_CURRENT_USER, L"\\Software\\Microsoft\\Internet Explorer\\PageSetup", &hKey) != ERROR_SUCCESS)
	LONG nState = ::RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\PageSetup", &hKey);
	if (nState != ERROR_SUCCESS)
	{
		////��ӡ����  
		//HKEY_Key = "Print_Background";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "yes");
		////����
		//HKEY_Shrink = "Shrink_To_Fit";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Shrink, "no");
		////����ҳü��Ϊ�գ�
		//HKEY_Key = "header";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "");
		////����ҳ�ţ�Ϊ�գ� 
		//HKEY_Key = "footer";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "");
		////������ҳ�߾ࣨ0��
		//HKEY_Key = "margin_bottom";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0.25");
		////������ҳ�߾ࣨ0��
		//HKEY_Key = "margin_left";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0");
		////������ҳ�߾ࣨ0��
		//HKEY_Key = "margin_right";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0");
		////������ҳ�߾ࣨ0.25�� 
		//HKEY_Key = "margin_top";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0.25");
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		HKEY	hSubkey;
		DWORD	dwDisposition;
		if (RegCreateKeyEx(hKey, L"Print_Background", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubkey, &dwDisposition) != ERROR_SUCCESS)
		{
			return 2;
		}
		RegCloseKey(hSubkey);
	}
	else
	{
		strBuf = L"yes";
		if (RegSetValueEx(hKey, L"Print_Background", 0, szType, (unsigned char *)strBuf.GetBuffer(), (strBuf.GetLength() + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			::RegCloseKey(hKey);
			return 3;
		}
	}
	return 0;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

#include "gfunction.h"
// CBarPlatformDlg �Ի���
CBarPlatformDlg::CBarPlatformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarPlatformDlg::IDD, pParent)
{
	//SetPrintParam();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBarPlatformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBarPlatformDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CBarPlatformDlg ��Ϣ�������

BOOL CBarPlatformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	SetWindowText(_T(""));
#ifndef _DEBUG
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	CRect rt(-100, -100, -1, -1);
	MoveWindow(&rt);
#endif
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBarPlatformDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBarPlatformDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else 
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBarPlatformDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBarPlatformDlg::OnNcPaint()
{
#ifndef _DEBUG
	ShowWindow(SW_HIDE);
#else
	CDialogEx::OnNcPaint();
#endif
}
