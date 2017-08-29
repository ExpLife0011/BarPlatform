// Update.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Update.h"


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <TlHelp32.h>
#include <algorithm>    // std::transform
//#include <Windows.h>    // ShellExecute
#include <Shellapi.h>  // ShellExecute
//#pragma comment(lib, "IPHLPAPI.lib")

using namespace std;

#define MAX_LOADSTRING 100


// ��char *ת����wstring
std::wstring MultCharToWideCharA(const char *pStr, int nLen)
{
	//��ȡ�������Ĵ�С��������ռ䣬��������С�ǰ��ַ������
	int len = MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), NULL, 0);
	wchar_t *buffer = new wchar_t[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), buffer, len);
	buffer[len] = L'\0';//����ַ�����β
	//ɾ��������������ֵ
	std::wstring return_value;
	return_value.append(buffer);
	delete[]buffer;
	return return_value;
}

//----------------------------------------------------------------------------
// ��  ��: ��ȡ��ִ���ļ�·��(��/)
// ��  ��:
// ����ֵ: 
//   strName - ��Ž����
//----------------------------------------------------------------------------
string &GetNewPlatformPathName()
{
	static string strFilePathName;
	if (strFilePathName.empty())
	{
		string strName;
		const int BUFSIZE = 1024;
		char exec_name[BUFSIZE];
		::GetModuleFileNameA(0, exec_name, BUFSIZE - 1);
		strName = exec_name;
		strName = strName.substr(0, strName.rfind('\\'));
		strFilePathName = strName;
		strName += "config.ini";
		char szTemp[MAX_PATH] = { 0 };
		wchar_t szTempW[MAX_PATH] = { 0 };
		// ƽ̨������·����
		GetPrivateProfileStringA("local", "machinename", "\\PlatformUpdate", szTemp, MAX_PATH - 1, strName.c_str());
		strFilePathName += szTemp;
		strFilePathName += "\\BarPlatform.exe";
	}
	return strFilePathName;
}

//----------------------------------------------------------------------------
// ��  ��: ��ȡ��ǰƽ̨�ļ�ȫ��
// ��  ��:
// ����ֵ: 
//   strName - ��Ž����
//----------------------------------------------------------------------------
string &GetCurPlatformPathName()
{
	static string strName;
	if (strName.empty())
	{
		const int BUFSIZE = 1024;
		char exec_name[BUFSIZE];
		::GetModuleFileNameA(0, exec_name, BUFSIZE - 1);
		strName = exec_name;
		strName = strName.substr(0, strName.rfind('\\') + 1);
		strName += "BarPlatform.exe";
	}
	return strName;
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// ����: ����ļ��Ƿ����
//----------------------------------------------------------------------------
bool FileExists(const string& strDir)
{
	bool bResult;
	struct _stat st;
	memset(&st, 0, sizeof(st));

	if (_stat(strDir.c_str(), &st) == 0)
		bResult = ((st.st_mode & S_IFREG) == S_IFREG);
	else
		bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------
// ����: ���Ŀ¼�Ƿ����
//----------------------------------------------------------------------------
bool DirectoryExists(const string& strDir)
{
	bool bResult;
	struct _stat st;
	memset(&st, 0, sizeof(st));

	if (_stat(strDir.c_str(), &st) == 0)
		bResult = ((st.st_mode & S_IFDIR) == S_IFDIR);
	else
		bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------
// ����: ����Ŀ¼
// ʾ��: CreateDir("/home/test");
//----------------------------------------------------------------------------
bool CreateDir(const string& strDir)
{
	//MakeSureDirectoryPathExists();
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	return !!CreateDirectoryA(strDir.c_str(), &attrib);
}

//----------------------------------------------------------------------------
// ����: ǿ�ƴ���Ŀ¼
// ����: 
//   strDir - ��������Ŀ¼ (�����Ƕ༶Ŀ¼)
// ����:
//   true   - �ɹ�
//   false  - ʧ��
// ʾ��:
//   ForceDirectories("/home/user1/data");
//----------------------------------------------------------------------------
bool ForceDirectories(string strDir)
{
	if (strDir.rfind('.') == strDir.size() - 4)
	{
		int n = strDir.rfind('/');
		if (n < 0) n = strDir.rfind('\\');
		strDir.resize(n);
	}
	if (DirectoryExists(strDir))
	{
		return true;
	}
	int nLen = strDir.rfind('/');
	if (nLen < 0) nLen = strDir.rfind('\\');

	if (strDir.empty() || nLen < 0)
	{
		return false;
	}
	string strSub(strDir);
	strSub.resize(nLen);

	if (DirectoryExists(strSub))
	{
		return CreateDir(strDir);
	}
	else
	{
		return ForceDirectories(strSub);
	}

	return true;
}

// ɱָ������
bool KillProcessByProcessName(const wstring& csAppName, bool& bIsProcessExist)
{
	//
	bool bResult = true;
	bIsProcessExist = false;

	//
	PROCESSENTRY32 pe;
	memset(&pe, 0, sizeof(PROCESSENTRY32));

	//
	try
	{
		HANDLE hShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hShot)
		{
			DWORD dwErrorCode = ::GetLastError();
			//CLogError::WriteLog("ɱ����  ʧ�ܣ���ȡϵͳ���̿���ʧ��(%d)", dwErrorCode);
			return false;
		}

		pe.dwSize = sizeof(PROCESSENTRY32);	//	һ��Ҫ��ΪdwSize��ֵ
		if (::Process32First(hShot, &pe))
		{
			do
			{
				wstring csAppNameTmp = pe.szExeFile;
				wstring csAppNameP = csAppName;
				transform(csAppNameTmp.begin(), csAppNameTmp.end(), csAppNameTmp.begin(), ::tolower);
				transform(csAppNameP.begin(), csAppNameP.end(), csAppNameP.begin(), ::tolower);

				int nAt = csAppNameP.rfind(L'.');
				csAppNameP = csAppNameP.substr(0, nAt);
				int nNameLen = csAppNameP.size();
				//
				if (csAppNameTmp.substr(0, nNameLen).compare(csAppNameP) == 0)
				{
					//
					bIsProcessExist = TRUE;
					//
					HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
					if (!hProcess)
					{
						DWORD dwErrorCode = ::GetLastError();
						//CLogError::WriteLog("ɱ����  ʧ�ܣ���ָ������ʧ��(%d)", dwErrorCode);
					}
					else
					{
						bResult = ::TerminateProcess(hProcess, 0);
						//CLogError::WriteLog("ɱ����  TerminateProcess end: %d", bResult);
						if (!bResult)
						{
							DWORD dwErrorCode = ::GetLastError();
						}
						::CloseHandle(hProcess);
					}
					break;
				}
			} while (::Process32Next(hShot, &pe));
		}
		else
		{
			DWORD dwErrorCode = ::GetLastError();
			//CLogError::WriteLog("ɱ����  ʧ�ܣ���ȡ�������׽���ʧ��(%d)", dwErrorCode);
		}
		//
		::CloseHandle(hShot);
	}
	catch (...)
	{
	}

	//
	return bResult;
}

//// ȫ�ֱ���: 
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
//
//// �˴���ģ���а����ĺ�����ǰ������: 
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	string strNewPlatform = GetNewPlatformPathName();
	string strCurPlatform = GetCurPlatformPathName();
	if (::FileExists(strNewPlatform))
	{
		//CLogClient::WriteLog("���°����ڡ�");
		//CString strCurPlatformVer = CConfigFile::GetInstance()->GetCurPlatformVer().c_str();
		//CString strDownloadPlatformVer = GetFileVersionEx(CConfigFile::GetInstance()->GetDownloadPlatformPathName()).c_str();
		//if (strCurPlatformVer.CompareNoCase(strDownloadPlatformVer) != 0)
		//{
			// ɱ��ƽ̨����
			//CLogClient::WriteLog("���ز������°汾��ƽ̨��");
			//CString strCurPlatformFileName = L"BarPlatForm.exe";
			bool bIsProcessExist = false;
			wstring strCurPlatformW = MultCharToWideCharA(strCurPlatform.c_str(), strCurPlatform.size());
			wstring strCurPlatformNameW = strCurPlatformW.substr(strCurPlatformW.rfind('\\') + 1);
			if (KillProcessByProcessName(strCurPlatformNameW, bIsProcessExist))
			{
				// ����ƽ̨����
cpfile:			if (::CopyFileA(strNewPlatform.c_str(), strCurPlatform.c_str(), FALSE))
				{
					//CLogClient::WriteLog("����ƽ̨���̡�");
					// ����ƽ̨����
					HINSTANCE h = ShellExecute(NULL, L"open", strCurPlatformW.c_str(), NULL, NULL, SW_HIDE);
					if ((int)h < 32)
					{
						//CLogError::WriteLogW(L"����ƽ̨����%sʧ��: %d", CConfigFile::GetInstance()->GetCurPlatformPathName().c_str(), (int)h);
					}
					else
					{
						//CLogClient::WriteLog("����ƽ̨���̳ɹ���");
						return TRUE;
					}
				}
				else
				{
					int nErr = ::GetLastError();
					//CLogError::WriteLogW(L"����ƽ̨�ļ�ʧ��: %d, %s --> %s", nErr, CConfigFile::GetInstance()->GetDownloadPlatformPathName().c_str(), CConfigFile::GetInstance()->GetCurPlatformPathName().c_str());
					if (nErr == 32)
					{
						Sleep(100);
						goto cpfile;
					}
				}
			}
	}


	return TRUE;
	//MSG msg;
	//HACCEL hAccelTable;

	//// ��ʼ��ȫ���ַ���
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_UPDATE, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	//// ִ��Ӧ�ó����ʼ��: 
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UPDATE));

	//// ����Ϣѭ��: 
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//return (int) msg.wParam;
}



////
////  ����:  MyRegisterClass()
////
////  Ŀ��:  ע�ᴰ���ࡣ
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UPDATE));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UPDATE);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}
//
////
////   ����:  InitInstance(HINSTANCE, int)
////
////   Ŀ��:  ����ʵ�����������������
////
////   ע��: 
////
////        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
////        ��������ʾ�����򴰿ڡ�
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  Ŀ��:    ���������ڵ���Ϣ��
////
////  WM_COMMAND	- ����Ӧ�ó���˵�
////  WM_PAINT	- ����������
////  WM_DESTROY	- �����˳���Ϣ������
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// �����˵�ѡ��: 
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO:  �ڴ���������ͼ����...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// �����ڡ������Ϣ�������
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}
