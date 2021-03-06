#include "BarInfo.h"
#include "CommInterface.h"
#include "gfunction.h"
#include "MD5.h"
#include "CenterInterface.h"
#include "JsonCode.h"
#include "ConfigFile.h"
#include <TlHelp32.h>

#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

template <> CBarInfo *CSingle<CBarInfo>::m_pInstance = 0;
int g_nGameTypeId = 1;

wstring GetDesktopPath()
{
	wstring strRet;
	wchar_t szBuf[255] = {0};
	LPITEMIDLIST  ppidl = NULL;
	if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
	{
		BOOL flag = SHGetPathFromIDList(ppidl, szBuf);
		CoTaskMemFree(ppidl);
	}
	strRet = szBuf;
	return strRet;
}

bool CreateLinkFile(LPCTSTR szStartAppPath, LPCTSTR szAddCmdLine, LPCTSTR szDestLnkPath, LPCTSTR szIconPath)
{
	bool bRet = false;
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink *pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(szStartAppPath);
			wstring strTmp = szStartAppPath;
			int nStart = strTmp.find_last_of(_T("/\\"));
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			pShellLink->SetArguments(szAddCmdLine);
			if (szIconPath)
			{
				pShellLink->SetIconLocation(szIconPath, 0);
			}
			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save(szDestLnkPath, FALSE);
				if (SUCCEEDED(hr))
				{
					bRet = true;
				}
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return bRet;
}

BOOL TaskbarPin(LPTSTR lpPath, LPTSTR lpFileName, BOOL bIsPin = TRUE)
{
	CoInitialize(nullptr);
	BOOL bRet = FALSE;
	HMENU hmenu = NULL;
	LPSHELLFOLDER pdf = NULL;
	LPSHELLFOLDER psf = NULL;
	LPITEMIDLIST pidl = NULL;
	LPITEMIDLIST pitm = NULL;
	LPCONTEXTMENU pcm = NULL;

	if (SUCCEEDED(SHGetDesktopFolder(&pdf))
		&& SUCCEEDED(pdf->ParseDisplayName(NULL, NULL, lpPath, NULL, &pidl, NULL))
		&& SUCCEEDED(pdf->BindToObject(pidl, NULL, IID_IShellFolder, (void **)&psf))
		&& SUCCEEDED(psf->ParseDisplayName(NULL, NULL, lpFileName, NULL, &pitm, NULL))
		&& SUCCEEDED(psf->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST *)&pitm, IID_IContextMenu, NULL, (void **)&pcm))
		&& (hmenu = CreatePopupMenu()) != NULL
		&& SUCCEEDED(pcm->QueryContextMenu(hmenu, 0, 1, INT_MAX, CMF_NORMAL)))
	{
		CMINVOKECOMMANDINFO ici = { sizeof(CMINVOKECOMMANDINFO), 0 };
		ici.hwnd = NULL;
		ici.lpVerb = bIsPin ? "taskbarpin" : "taskbarunpin";
		pcm->InvokeCommand(&ici);
		bRet = TRUE;
	}

	if (hmenu)
		DestroyMenu(hmenu);
	if (pcm)
		pcm->Release();
	if (pitm)
		CoTaskMemFree(pitm);
	if (psf)
		psf->Release();
	if (pidl)
		CoTaskMemFree(pidl);
	if (pdf)
		pdf->Release();

	CoUninitialize();

	return bRet;
}

CBarInfo::CBarInfo()
{
#ifdef _DEBUG
	//DirectoryOrFileExists("D:\\我的游戏");
	//GetGameLibFromCloudUpdate();

	//ScanGameMenuAndUpload();
	//TaskbarPin(_T("D:\\personalproject\\游戏菜单\\code\\BarPlatform\\Debug\\GameMenu"), _T("游戏导航菜单.lnk"));

	////string s = "linux是一个出色的操作系统";
	//wstring s = L"倩女幽魂";
	////string sR = GetCharSpellCode(s);
	////wstring strDest = GetDesktopPath();
	////strDest += L"\\游戏菜单.lnk";
	////CreateLinkFile(L"D:\\personalproject\\游戏菜单\\code\\BarPlatform\\Debug\\GameMenu\\myoubox.exe", L"", strDest.c_str(), 0);
	//LPITEMIDLIST lpItemIdList;
	//SHGetSpecialFolderLocation(0, CSIDL_APPDATA, &lpItemIdList); 
	//char szBuf[MAX_PATH] = {0};
	//wchar_t wszBuf[MAX_PATH] = {0};
	//SHGetPathFromIDList(lpItemIdList, wszBuf);
	//if (DirectoryExistsW(wszBuf))
	//{
	//	s = wszBuf;
	//	s += L"\\Microsoft\\Internet Explorer\\Quick Launch\\游戏菜单.lnk";
	//	CreateLinkFile(L"D:\\personalproject\\游戏菜单\\code\\BarPlatform\\Debug\\GameMenu\\myoubox.exe", L"", s.c_str(), 0);
	//}


#endif
	string strType = CConfigFile::GetInstance()->GetGameMenuType();
	if (strType.empty())
	{
		// 扫描游戏更新环境，
		ScanGameMenuEnv();
	}
	else
	{
		CLogClient::WriteLog("已配置游戏库： %s", strType.c_str());
	}
}

CBarInfo::~CBarInfo()
{
	if (m_hConfigMapping)
	{
		CloseHandle(m_hConfigMapping);
	}
}

// 获取游戏库
void CBarInfo::GetGameLib()
{
	if (!CBarInfo::GetInstance()->IsGetGameLib())
	{
		CLogClient::WriteLog("获取游戏库 begin");
		// 获取游戏库
		CBarInfo::GetInstance()->GetGameLibI();
		CLogClient::WriteLog("获取游戏库 end");
	}
}

// 获取配置
void CBarInfo::GetConfig()
{
	static int64 nTime = 0;
	// {"data":[{"date":"2016-12-30","games":[{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":10000,"game_id":11001,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":998,"game_id":11002,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":995,"game_id":11003,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":30000,"game_id":11004,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":999,"game_id":11005,"launcher":""},{"game_name":"QQ华夏","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":50000,"game_id":100644,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"},{"game_name":"QQ西游","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":20000,"game_id":100679,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"}]}],"ret_msg":"success","ret_code":0,"config":{"resources":[{"size":20984703,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/game_icons.zip","md5":"2cf0c87d74f333de1e6935c0ca82d207"}],"talking_url":"http://106.14.45.195:80/","update_packs":[{"ver":"20161101000000","size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/update1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}],"programs":[{"size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/smallprogram1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}]}}
	if (!CBarInfo::GetInstance()->IsGetConfig())
	{
		CLogClient::WriteLog("获取配置 begin");
		// 获取菜单
		CBarInfo::GetInstance()->GetMenu();
		// 获取配置
		CBarInfo::GetInstance()->GetConfigI();
		// 获取策略
		CBarInfo::GetInstance()->GetConfigII();
		// 获取小程序
		CBarInfo::GetInstance()->GetMinProgram();
		// 加载第三方控件
		LoadThreadPlug();
		nTime = ::GetTickCount64();
		CLogClient::WriteLog("获取配置 end");
	}
	else
	{
		// 已经获取配置
		int nInterval = CConfigFile::GetInstance()->GetDLConfigInterval();
		if (nInterval > 0 && nTime)
		{
			if (::GetTickCount64() - nTime > nInterval * 60 * 1000)
			{
				CLogClient::WriteLog("重新获取配置 begin :%d", nInterval);
				//// 获取菜单
				//CBarInfo::GetInstance()->GetMenu();
				// 获取配置
				CBarInfo::GetInstance()->GetConfigI();
				//// 获取小程序
				//CBarInfo::GetInstance()->GetMinProgram();
				//// 加载第三方控件
				//LoadThreadPlug();
				nTime = ::GetTickCount64();
				CLogClient::WriteLog("重新获取配置 end");
			}
		}
	}
}

// 扫描其他游戏菜单上传
void CBarInfo::ScanGameMenuAndUpload()
{
	if (CBarInfo::GetInstance()->IsGetConfig())
	{
		static HWND hOld = 0;
		HWND h = ::GetForegroundWindow();
		if (hOld == h)
		{
			return;
		}
		hOld = h;
		string strType;
		DWORD dwId = 0;
		GetWindowThreadProcessId(h, &dwId);
		if (dwId)
		{
			wstring strName = GetProcessNameById(dwId);
			transform(strName.begin(), strName.end(), strName.begin(), tolower);
			if (strName.compare(L"barclientview.exe") == 0)
			{
				// 顺网
				strType = "10";
			}
			else if (strName.compare(L"knbmenu.exe") == 0)
			{
				// 云更新
				strType = "11";
			}
			else if (strName.compare(L"recreation.exe") == 0)
			{
				// 易游
				strType = "12";
			}
			else if (strName.compare(L"hintclient.exe") == 0)
			{
				// 信佑
				strType = "13";
			}
			//else if (strName.compare(L"barclientview.exe") == 0)
			//{
			//	// 方格子
			//	strType = "14";
			//}
			//else if (strName.compare(L"barclientview.exe") == 0)
			//{
			//	// 绿化大师
			//	strType = "15";
			//}
		}

		if (!strType.empty())
		{
			// 上传启动数据 （10 - 顺网； 11 - 云更新； 12 - 易游； 13 - 信佑；14 - 方格子；15 - 绿化大师）
			string strUrl = GetTalkUrl();
			CCenterInterface::UploadInfo(strUrl, strType);
		}
	}
}

void CBarInfo::GetConfigI()
{
	string s = CCenterInterface::GetConfig();
	ConfigDataItem cfg;
	CJsonDecode::DeCodeConfigData(s, cfg); 
	// 下载资源
	for (list<ResourceDataItem>::iterator it = cfg.ltRD.begin(); it != cfg.ltRD.end(); it++)
	{
		if (it->url.find("http") == 0)
		{
			if (it->type == 1 && CConfigFile::GetInstance()->GetIsDLIconLib())
			{
				// 下载资源(图标)
				string strUrl = it->url;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					string strDestPath = strFileName;
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
					{
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							it->iconpathname = strDestPath;
						}
					}
				}
			}
			else if (it->type == StrToInt(CConfigFile::GetInstance()->GetGameMenuType().c_str()))
			{
				// 下载资源(游戏库)
				string strUrl = it->url;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					string strDestPath = strFileName;
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
					{
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							strFileName.replace(strFileName.size() - 4, 4, ".json");
							string sTemp;
							GetGameLibFromDownloadFile(strFileName, sTemp);
							CJsonDecode::DeCodeGameData(sTemp, m_ltGame);
							// 过滤游戏库
#ifndef _DEBUG
							FilterGameLib();
#endif
							// 保存将游戏库信息到文件
							SaveGameLibInfoToFile();
							// 保存将游戏类型信息到文件
							SaveGameTypeInfoToFile();

							it->iconpathname = strDestPath;
						}
					}
				}
			}
		}
	}
	CLogClient::WriteLog("资源下载完成。");
	// 下载排序库
	for (list<SortDataItem>::iterator it = cfg.ltSD.begin(); it != cfg.ltSD.end(); it++)
	{
		for (list<SortGameItem>::iterator its = it->ltGame.begin(); its != it->ltGame.end(); its++)
		{
			string sId = IntToStr(its->game_id);
			its->iconpathname = CConfigFile::GetInstance()->GetGameResPath() + "\\";
			its->poppathname = its->iconpathname;
			its->iconpathname += sId;
			if (its->icon.find("http") == 0)
			{
				// 下载图标
				its->iconpathname += its->icon.substr(its->icon.size() - 4);
				if (!CCenterInterface::DownloadFile(its->icon, its->iconpathname, its->iconmd5))
				{
					CLogError::WriteLog("下载图标失败：%d, Url：%s", ::GetLastError(), its->icon.c_str());
				}
			}
			else
			{
				string sT = its->iconpathname + ".png";
				if (!FileExists(sT))
				{
					sT = its->iconpathname + ".ico";
				}
				its->iconpathname = sT;
			}
			if (its->popurl.find("http") == 0)
			{
				// 下载浮层动态图片
				its->poppathname += its->popurl.substr(its->popurl.rfind('/') + 1);
				if (!CCenterInterface::DownloadFile(its->popurl, its->poppathname, its->popmd5))
				{
					CLogError::WriteLog("下载浮层动态图片失败：%d, Url：%s", ::GetLastError(), its->popurl.c_str());
				}
			}
			else
			{
				its->poppathname = "";
			}
			if (its->launcher.find("http") == 0)
			{
				// 下载登陆程序
				string strUrl = its->launcher;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, its->launchermd5))
					{
						//CCommInterface::UnzipFileToPath("D:\\personalproject\\游戏菜单\\code\\BarPlatform\\Debug\\GameMenu\\Game\\10002_HJS.zip", "D:\\personalproject\\游戏菜单\\code\\BarPlatform\\Debug\\GameMenu\\Game\\10002_HJS");
						string strDestPath = strFileName.substr(0, strFileName.size() - 4);
						if (!DirectoryExists(strDestPath))
						{
							if (!CreateDir(strDestPath))
							{
								CLogError::WriteLog("创建目录失败：%d, 目录：%s", ::GetLastError(), strDestPath.c_str());
							}
						}
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							nPos = strDestPath.rfind('_');
							if (nPos != string::npos)
							{
								its->launchpathname = strDestPath;
								its->launchpathname += "\\";
								its->launchpathname += strDestPath.substr(nPos + 1);
								its->launchpathname += ".exe";
							}
							else
							{
								strDestPath += "\\launcher.exe";
								its->launchpathname = strDestPath;
							}
						}
					}
				}
			}
		}
	}
	CLogClient::WriteLog("排序库下载完成。");
	//// 下载升级包
	//for (list<UpLevelDataItem>::iterator it = cfg.ltUD.begin(); it != cfg.ltUD.end(); it++)
	//{
	//	string strOldVer = CConfigFile::GetInstance()->GetMenuVer();
	//	if (strOldVer.compare(it->ver) == 0)
	//	{
	//		break;
	//	}
	//	if (it->url.find("http") == 0)
	//	{
	//		// 下载升级包
	//		string strUrl = it->url;
	//		string strName;
	//		int nPos = strUrl.rfind('/');
	//		if (nPos != string::npos)
	//		{
	//			strName = strUrl.substr(nPos + 1);
	//		}
	//		if (!strName.empty())
	//		{
	//			string strFileName = CConfigFile::GetInstance()->GetGameMenuProgram();
	//			strFileName = strFileName.substr(0, strFileName.rfind('\\'));
	//			string strDestPath = strFileName;
	//			strFileName += "\\";
	//			strFileName += strName;
	//			if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
	//			{
	//				if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
	//				{
	//					it->pathname = strDestPath;
	//				}
	//			}
	//		}
	//	}
	//}
	//CLogClient::WriteLog("升级包下载完成。");
	//// 下载小程序
	//for (list<ProgramDataItem>::iterator it = cfg.ltPD.begin(); it != cfg.ltPD.end(); it++)
	//{
	//	if (it->url.find("http") == 0)
	//	{
	//		// 下载小程序
	//		string strUrl = it->url;
	//		string strName;
	//		int nPos = strUrl.rfind('/');
	//		if (nPos != string::npos)
	//		{
	//			strName = strUrl.substr(nPos + 1);
	//		}
	//		if (!strName.empty())
	//		{
	//			string strFileName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//			string strDestPath = strFileName;
	//			strFileName += "\\";
	//			strFileName += strName;
	//			CLogClient::WriteLog("小程序下载全路径名：%s", strFileName.c_str());
	//			if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
	//			{
	//				if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
	//				{
	//					it->pathname = strDestPath;
	//				}
	//			}
	//			//strFileName += "\\ThreePlug.exe";
	//			//if (FileExists(strFileName.c_str()))
	//			//{
	//			//	HINSTANCE h = ShellExecuteA(NULL, "open", strFileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//			//	CLogClient::WriteLog("运行小程序结果：%s,%d--%d", strFileName.c_str(), h, ::GetLastError());
	//			//}

	//		}
	//	}
	//}
	//CLogClient::WriteLog("小程序下载完成。");
	SaveInfoToMem(cfg);
	SetConfig(cfg);
	//string strPathName = CConfigFile::GetInstance()->GetGameMenuProgram();
	//CLogClient::WriteLog("运行菜单目录：%s", strPathName.c_str());
	//if (!strPathName.empty())
	//{
	//	if (FileExists(strPathName.c_str()))
	//	{
	//		HINSTANCE h = ShellExecuteA(NULL, "open", strPathName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//		CLogClient::WriteLog("运行菜单结果：%d--%d", h, ::GetLastError());
	//		wstring strDest = GetDesktopPath();
	//		wstring strDest2 = strDest;
	//		wstring strShutKey;
	//		strDest += L"\\";
	//		if (cfg.game_menu.empty())
	//		{
	//			//strDest += L"游戏菜单";
	//			strShutKey = MultCharToWideCharA(CConfigFile::GetInstance()->GetLinkFileName());
	//			strDest += strShutKey;
	//		}
	//		else
	//		{
	//			strShutKey = MultCharToWideCharA(cfg.game_menu);
	//			strDest += strShutKey;
	//		}
	//		strShutKey += L".lnk";
	//		strDest += L".lnk";
	//		if (CreateLinkFile(MultCharToWideCharA(strPathName).c_str(), L"", strDest.c_str(), 0))
	//		{
	//			TaskbarPin((LPTSTR)strDest2.c_str(), (LPTSTR)strShutKey.c_str());
	//		}
	//	}
	//}

	//// 运行小程序
	//string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//strMinProgramName += "\\ThreePlug.exe";
	//CLogClient::WriteLog("小程序全名：%s", strMinProgramName.c_str());
	//if (FileExists(strMinProgramName.c_str()))
	//{
	//	HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//	CLogClient::WriteLog("运行小程序结果：%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	//}

}

// 获取策略
void CBarInfo::GetConfigII()
{
	string strConfig = CCenterInterface::GetConfigII();
	CLogClient::WriteLog("获取策略返回信息： %s", strConfig.c_str());
	if (!strConfig.empty())
	{
		ConfigIIItem Item;
		string strFileName;
		string strParam;
		CJsonDecode::DeCodeConfigIIData(strConfig, Item);
		for (auto it : Item.lt)
		{
			strFileName = CConfigFile::GetInstance()->GetPolicyFilePath();
			strFileName += "\\";
			strFileName += it.url.substr(it.url.rfind('/') + 1);
			string strDestPath = strFileName.substr(0, strFileName.rfind('.'));
			if (!ForceDirectories(strDestPath))
			{
				CLogError::WriteLog("创建策略文件夹失败： %s", strDestPath.c_str());
				continue;
			}
			if (!CCenterInterface::DownloadFile(it.url, strFileName, it.md5))
			{
				CLogError::WriteLog("下载策略文件失败： %s - %s", it.url.c_str(), it.md5.c_str());
				continue;
			}

			if (!CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
			{
				CLogError::WriteLog("解压策略文件失败： %s - %s", strFileName.c_str(), strDestPath.c_str());
				continue;
			}
			//CFileFind find;
			//CString strZipTarget = szTempPath;
			//BOOL bWorking = find.FindFile(strZipTarget + "\\*.*");
			//strDestPath += "\\*.Exe";
			WIN32_FIND_DATAA FindData;
			HANDLE hError = FindFirstFileA((strDestPath + "\\*.exe").c_str(), &FindData);
			if (hError != INVALID_HANDLE_VALUE)
			{
				// 调用
				string strNumIcon = it.param.substr(it.param.rfind('=') + 1);
				//FormatString(strParam, "{\"iconNum\":%s}", strNumIcon.c_str());
				FormatString(strParam, "{\"iconNum\":%s, \"barID\":\"%s\"}", strNumIcon.c_str(), CConfigFile::GetInstance()->GetBarId().c_str());
				strParam = Base64::base64_encode(strParam);
				strFileName = strDestPath + "\\" + FindData.cFileName;
				CLogClient::WriteLog("运行策略文件全名：%s - %s", strFileName.c_str(), strParam.c_str());
				if (FileExists(strFileName.c_str()))
				{
					HINSTANCE h = ShellExecuteA(NULL, "open", strFileName.c_str(), strParam.c_str(), NULL, SW_SHOWNORMAL);
					string strUrl = GetTalkUrl();
					string strAction;
					FormatString(strAction, "%d", atoi(strNumIcon.c_str()) + 200);
					CCenterInterface::UploadInfo(strUrl, strAction);
					CLogClient::WriteLog("运行策略文件结果：%s,%d--%d", strFileName.c_str(), h, ::GetLastError());
				}
			}
		}
	}
}


// 杀指定进程
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
			//CLogError::WriteLog("杀进程  失败，获取系统进程快照失败(%d)", dwErrorCode);
			return false;
		}

		pe.dwSize = sizeof(PROCESSENTRY32);	//	一定要先为dwSize赋值
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
						//CLogError::WriteLog("杀进程  失败，打开指定进程失败(%d)", dwErrorCode);
					}
					else
					{
						bResult = ::TerminateProcess(hProcess, 0);
						//CLogError::WriteLog("杀进程  TerminateProcess end: %d", bResult);
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
			//CLogError::WriteLog("杀进程  失败，获取快照中首进程失败(%d)", dwErrorCode);
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

// 获取菜单
void CBarInfo::GetMenu()
{
	string strUpdate = CCenterInterface::GetMenuUpdate();
	CLogClient::WriteLog("获取菜单返回信息： %s", strUpdate.c_str());
	if (!strUpdate.empty())
	{
		UpLevelDataItem Item;
		CJsonDecode::DeCodeSelfUpdateData(strUpdate, Item);
		if (!Item.url.empty())
		{
			CLogClient::WriteLog("有新版本菜单。");
			Item.pathname = CConfigFile::GetInstance()->GetGameMenuProgram();
			string strMenuName = Item.pathname.substr(Item.pathname.rfind('\\') + 1);
			Item.pathname = Item.pathname.substr(0, Item.pathname.rfind('\\'));
			string strPath = Item.pathname;
			Item.pathname += "\\";
			Item.pathname += Item.url.substr(Item.url.rfind('/') + 1);
			bool bIsProcessExist = false;
			if (KillProcessByProcessName(MultCharToWideCharA(strMenuName), bIsProcessExist))
			{
				bool b = CCenterInterface::DownloadFile(Item.url, Item.pathname, Item.md5);
				if (b)
				{
					if (CCommInterface::UnzipFileToPath(Item.pathname.c_str(), strPath.c_str()))
					{
						CLogClient::WriteLog("新版本菜单更新成功。");
					}
				}
			}
		}
	}
}
// 获取小程序
void CBarInfo::GetMinProgram()
{
	string strUpdate = CCenterInterface::GetMinProgram();
	CLogClient::WriteLog("获取小程序返回信息： %s", strUpdate.c_str());
	if (!strUpdate.empty())
	{
		UpLevelDataItem Item;
		CJsonDecode::DeCodeSelfUpdateData(strUpdate, Item);
		if (!Item.url.empty())
		{
			CLogClient::WriteLog("有小程序。");
			Item.pathname = CConfigFile::GetInstance()->GetMinProgramFilePath();
			Item.pathname += "\\";
			Item.pathname += Item.url.substr(Item.url.rfind('/') + 1);
			bool b = CCenterInterface::DownloadFile(Item.url, Item.pathname, Item.md5);
			if (b)
			{
				if (CCommInterface::UnzipFileToPath(Item.pathname.c_str(), CConfigFile::GetInstance()->GetMinProgramFilePath().c_str()))
				{
					CLogClient::WriteLog("获取小程序下载成功。");
				}
			}
		}
	}

	//// 运行小程序
	//string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//strMinProgramName += "\\ThreePlug.exe";
	//CLogClient::WriteLog("小程序全名：%s", strMinProgramName.c_str());
	//if (FileExists(strMinProgramName.c_str()))
	//{
	//	HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//	CLogClient::WriteLog("运行小程序结果：%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	//}


}

// 保存配置信息到内存
void CBarInfo::SaveInfoToMem(const ConfigDataItem &cfg)
{
	//	将配置信息放入内存文件
	if (!m_hConfigMapping)
	{
		m_hConfigMapping = (HANDLE)::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, sizeof(CLIENT_CONFIG_INFO), L"BarPlatform Config Struct Map");
	}
	if (m_hConfigMapping)
	{
		//string strMoney = IntToStr(CUserOnlineInfo::GetInstance()->GetUserMoney());
		CLIENT_CONFIG_INFO *lpClientConfigInfo = (CLIENT_CONFIG_INFO*)::MapViewOfFile(m_hConfigMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		memset(lpClientConfigInfo, 0, sizeof(CLIENT_CONFIG_INFO));
		memcpy(lpClientConfigInfo->clientId, CConfigFile::GetInstance()->GetMac().c_str(), CConfigFile::GetInstance()->GetMac().size());
		memcpy(lpClientConfigInfo->localIp, CConfigFile::GetInstance()->GetLocalIp().c_str(), CConfigFile::GetInstance()->GetLocalIp().size());
		memcpy(lpClientConfigInfo->agencyId, CConfigFile::GetInstance()->GetAgencyId().c_str(), CConfigFile::GetInstance()->GetAgencyId().size());
		memcpy(lpClientConfigInfo->szUploadUrl, cfg.talking_url.c_str(), cfg.talking_url.size());
		memcpy(lpClientConfigInfo->szMenuName, cfg.game_menu.c_str(), cfg.game_menu.size());
		//string strUserId = CUserOnlineInfo::GetInstance()->GetUserId().c_str();
		//CMD5 md5;
		//strUserId = md5.MD5(strUserId.c_str());
		//strUserId += "m";
		//memcpy(lpClientConfigInfo->szUserId, CUserOnlineInfo::GetInstance()->GetUserId().c_str(), CUserOnlineInfo::GetInstance()->GetUserId().size());
		//memcpy(lpClientConfigInfo->szMoney, strMoney.c_str(), strMoney.size());
		::UnmapViewOfFile(lpClientConfigInfo);
	}
	// 保存参数
	string strPFileName = CConfigFile::GetInstance()->GetGameMenuProgram();
	strPFileName = strPFileName.substr(0, strPFileName.rfind('\\') + 1);
	strPFileName += "param.ini";
	CLogClient::WriteLog("本地参数文件：%s", strPFileName.c_str());
	WritePrivateProfileStringA("platform", "clientid", CConfigFile::GetInstance()->GetMac().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "localip", CConfigFile::GetInstance()->GetLocalIp().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "agencyid", CConfigFile::GetInstance()->GetAgencyId().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "uploadurl", cfg.talking_url.c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "menuname", cfg.game_menu.c_str(), strPFileName.c_str());
	//ofstream ofP(strPFileName.c_str(), ios_base::out | ios_base::binary);
	//string strPInfo;
	//FormatString(strPInfo, "%lld|%s|%s|%s|%d|%d", its.game_id, its.game_name.c_str(), its.launchpathname.c_str(), its.iconpathname.c_str(), its.rank, its.flag);
	//ofP << strPInfo << "\r\n";
	//ofP.close();


	// 保存排位库
	for (auto it : cfg.ltSD)
	{
		if (it.ltGame.size() > 0)
		{
			string strFileName = CConfigFile::GetInstance()->GetGameSortFileName();
			ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
			for (auto its : it.ltGame)
			{
				if (!IsExistGameId(its.game_id))
				{
					CLogClient::WriteLog("本地不存在此游戏：%s", its.game_name.c_str());
				}
				string strInfo;
				FormatString(strInfo, "%lld|%s|%s|%s|%d|%d|%s", its.game_id, its.game_name.c_str(), 
					its.launchpathname.c_str(), its.iconpathname.c_str(), its.rank, its.flag, its.poppathname.c_str());
				of << strInfo << "\r\n";
			}
			of.close();
		}
	}
}

// 获取游戏库
void CBarInfo::GetGameLibI()
{
	string s = CCenterInterface::GetGameLib();
	CJsonDecode::DeCodeGameData(s, m_ltGame);
	// 过滤游戏库
	FilterGameLib();
	// 保存将游戏库信息到文件
	SaveGameLibInfoToFile();
	// 保存将游戏类型信息到文件
	SaveGameTypeInfoToFile();

}

// 扫描游戏更新环境，
void CBarInfo::ScanGameMenuEnv()
{
	//CConfigFile::GetInstance()->SetGameMenuType("100");
	//return;
	CLogClient::WriteLog("扫描游戏更新环境");
	string strType;
	HKEY hKey = NULL;
	// 方格子
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\FgzPnp", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "500";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("方格子");
	}
	if (!strType.empty()){ return; }

	// 顺网游戏大师
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\iCafe8", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"BarId", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "100";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		CLogClient::WriteLog("顺网游戏大师");
		::RegCloseKey(hKey);
	}
	if (!strType.empty()){ return; }

	// 云更新
	//if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\knbclient", KEY_READ | KEY_WOW64_64KEY, NULL, &hKey) == ERROR_SUCCESS)
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\knbclient", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "400";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		CLogClient::WriteLog("云更新");
		::RegCloseKey(hKey);
	}
	//if (!strType.empty())
	//{ 
	//	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\GINFO", &hKey) == ERROR_SUCCESS)
	//	{
	//		DWORD szType = REG_SZ;
	//		for (int i = 0; 1; i++)
	//		{
	//			DWORD dwCount = 1024;
	//			DWORD dwKeyLen = 1024;
	//			char szKey[1024] = { 0 };
	//			char szBuf[1024] = { 0 };
	//			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
	//			{
	//				string s = szBuf;
	//				GameLibItem item;
	//				s = s.substr(0, s.find_first_of('|'));
	//				if (!s.empty())
	//				{
	//					m_mapNameIcon[s] = szKey;
	//				}
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		::RegCloseKey(hKey);
	//	}
	//	return;
	//}
	//// 云更新
	//if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}", &hKey) == ERROR_SUCCESS)
	//{
	//	DWORD szType = REG_SZ;
	//	DWORD dwCount = 1024;
	//	TCHAR szBuf[1024];
	//	memset(szBuf, 0, 1024);
	//	if (RegQueryValueEx(hKey, L"netbarname", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
	//	{
	//		strType = "400";
	//		CConfigFile::GetInstance()->SetGameMenuType(strType);
	//	}
	//	::RegCloseKey(hKey);
	//}
	if (!strType.empty()){ return; }
	// 信佑
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Hintsoft1\\XunShanPro", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"PowerSaving", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "200";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("信佑");
	}
	if (!strType.empty()){ return; }

	// 易游
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\YileyooClient", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"HadInstall", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "300";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("易游");
	}
	if (!strType.empty()){ return; }

	// 绿化大师
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\zhdisk", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "600";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("绿化大师");
	}
	if (!strType.empty()){ return; }
}

// 保存将游戏库信息到文件
void CBarInfo::SaveGameLibInfoToFile()
{
	if (m_ltGame.size() == 0){ return; }
	// 将游戏库信息放入文件
	string strFileName = CConfigFile::GetInstance()->GetGameLibFileName();
	ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
	for (auto it : m_ltGame)
	{
		string strInfo;
		if (it.game_icon.empty())
		{
			it.game_icon = CConfigFile::GetInstance()->GetGameResPath() + "\\";
			it.game_icon += IntToStr(it.center_game_id);
			it.game_icon += ".png";
		} 
		if (CConfigFile::GetInstance()->IsFGZ())
		{
			// 方格子
			if (!::FileExists(it.game_icon))
			{
				string sPath = it.launcher;
				sPath = sPath.substr(0, sPath.rfind('\\'));
				sPath += "\\game.png";
				it.game_icon = sPath;
			}
		}
		// 游戏编号，中心游戏库的游戏编号 center_game_id | 游戏名称，方便查看 | 启动程序绝对路径 | 游戏图标绝对路径 | 游戏点击量（排序值） | 游戏分类 | 游戏名称拼音，方便查找 | 主程名 | 游戏显示名
		//FormatString(strInfo, "%lld|%s|%s|%s|%lld|%s|%s|%s", it.center_game_id, it.game_name.c_str(), it.launcher.c_str(), it.game_icon.c_str(), it.game_dp, it.category_1.c_str(), it.name_py.c_str(), it.process.c_str());
		FormatString(strInfo, "%lld|%s|%s|%s|%lld|%s|%s|%s|%s", it.center_game_id, it.game_name.c_str(), it.launcher.c_str(), it.game_icon.c_str(), it.game_dp, 
			it.category_1.c_str(), GetCharSpellCode(it.game_name).c_str(), it.process.c_str(), it.game_showname.c_str());
		of << strInfo << "\r\n";
	}
	of.close();
}

// 保存将游戏类型信息到文件
void CBarInfo::SaveGameTypeInfoToFile()
{
	if (m_ltGame.size() == 0){ return; }
	// 将游戏类型信息放入文件
	string strFileName = CConfigFile::GetInstance()->GetGameTypeFileName();
	ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
	string strAllType;
	string strInfo;
	for (auto it : m_ltGameType)
	{
		if (strAllType.empty())
		{
			strAllType = it.strId;
		}
		else
		{
			strAllType += ",";
			strAllType += it.strId;
		}
		// 游戏类型编号 | 游戏类型名称 | 游戏父类型编号
		FormatString(strInfo, "%s|%s|%s", it.strId.c_str(), it.strName.c_str(), it.strParentId.c_str());
		of << strInfo << "\r\n";
	}
	// g_nGameTypeId
	FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "所有游戏", strAllType.c_str());
	of << strInfo << "\r\n";
	//FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "最热", strAllType.c_str());
	//of << strInfo << "\r\n";
	//FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "好评", strAllType.c_str());
	//of << strInfo << "\r\n";
	of.close();
}

// 过滤游戏库
void CBarInfo::FilterGameLib()
{
	CLogClient::WriteLog("过滤游戏库");
	if (CConfigFile::GetInstance()->IsClundUpdate())
	{
		// 过滤云更新游戏库
		FilterCloudUpdateGameLib();
		return;
	}
	if (CConfigFile::GetInstance()->IsShunwang())
	{
		// 过滤顺网游戏库
		FilterShunwangGameLib();
		return;
	}
	char buf[100] = { 0 };
	list<string> ltHeader;
	DWORD len = GetLogicalDriveStringsA(100, buf);
	for (char *p = buf; *p; p += strnlen(p, 4) + 1)
	{
		string s = p;
		CLogClient::WriteLog("扫描到盘符： %s", s.c_str());
		ltHeader.push_back(s);
	}
	string strFileName;
	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		for (list<string>::iterator ith = ltHeader.begin(); ith != ltHeader.end(); ith++)
		{
			strFileName = *ith;
			strFileName += it->category_dir;
			strFileName += "\\";
			strFileName += it->game_name;
			//strFileName += "\\";
			string strLanuch = strFileName;
			strLanuch += "\\";
			if (!it->process.empty())
			{
				strFileName += "\\";
			}
			strFileName += it->process; 
			//if (FileExists(strFileName) || DirectoryExists(strFileName))
			if (DirectoryOrFileExists(strFileName))
			{
				it->launcher = strLanuch + it->launcher;
				it->process = strFileName;
				it->category_1 = GetGameTypeLevel1Id(it->category_dir);
				m_ltGame.push_back(*it);
				break;
			}
		}
	}
}

// 是否存在指定的游戏序号
bool CBarInfo::IsExistGameId(int64 nGameId)
{
	for (auto it : m_ltGame)
	{
		if (it.center_game_id == nGameId)
		{
			return true;
		}
	}
	return false;
}

// 获取游戏分类的序号
string CBarInfo::GetGameTypeLevel1Id(const string &strType)
{
	string strRet;
	for (list<GameTypeItem>::iterator it = m_ltGameType.begin(); it != m_ltGameType.end(); it++)
	{
		if (it->strName.compare(strType) == 0)
		{
			strRet = it->strId;
			break;
		}
	}
	if (strRet.empty())
	{
		GameTypeItem item;
		item.strId = IntToStr(g_nGameTypeId++);
		item.strName = strType;
		item.strParentId = "0";
		m_ltGameType.push_back(item);
		strRet = item.strId;
	}
	return strRet;
}

// 从文件中解析出游戏库数据
void CBarInfo::GetGameLibFromDownloadFile(const string &strFileName, string &strRet)
{
	//string s("通用工具");
	fstream f(strFileName.c_str(), ios::in);
	if (f.is_open())
	{
		const int nBufLen = 10 * 1024;
		f.seekg(0, std::ios::end);
		int nLen = f.tellg();
		f.seekg(0, std::ios::beg);
		char *pBuf = new char[nLen + 1];
		f.read(pBuf, nLen);
		pBuf[nLen] = 0;
		strRet = pBuf;
		strRet = Utf8ToLocal(strRet);
		delete[]pBuf;
		f.close();
	}
}

// 从云更新注册表中解析出游戏库数据
void CBarInfo::GetGameLibFromCloudUpdate()
{
	CLogClient::WriteLog("从云更新注册表中解析出游戏库数据");
	// 云更新
	m_mapDiskName.clear();
	string strName;
	HKEY hKey = NULL;
	// 加载盘符对应关系
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\VDINFO", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		for (int i = 0; 1; i++)
		{
			DWORD dwCount = 1024;
			DWORD dwKeyLen = 1024;
			char szKey[1024] = { 0 };
			char szBuf[1024] = { 0 };
			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
			{
				strName = szKey;
				string s = szBuf;
				if (!strName.empty() && !s.empty())
				{
					m_mapDiskName[strName] = s;
					CLogClient::WriteLog("扫描云更新盘符： %s--%s", strName.c_str(), m_mapDiskName[strName].c_str()); 
					s += ":";
					if (GetDriveTypeA(s.c_str()) == DRIVE_NO_ROOT_DIR)
					{
						CLogClient::WriteLog("扫描云更新盘符： %s不存在", s.c_str());
						m_bCloudSWitch = false;
					}
					else
					{
						CLogClient::WriteLog("扫描云更新盘符： %s存在", s.c_str());
					}
				}
			}
			else
			{
				break;
			}
		}
		::RegCloseKey(hKey);
	}

	// 加载游戏
	m_mapNamePath.clear();
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\GINFO", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		for (int i = 0; 1; i++)
		{
			DWORD dwCount = 1024;
			DWORD dwKeyLen = 1024;
			char szKey[1024] = { 0 };
			char szBuf[1024] = { 0 };
			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
			{
				string s = szBuf;
				GameLibItem item;
				string sField = s.substr(0, s.find_first_of('|'));
				if (!sField.empty())
				{
					strName = sField;
				}
				s = s.substr(s.find_first_of('|') + 1);
				sField = s.substr(0, s.find_first_of('|') - 1);
				//sField += strName;
				if (!sField.empty())
				{
					string strDiskName = sField.substr(0, 1);
					if (m_mapDiskName.count(strDiskName) > 0)
					{
						sField[0] = m_mapDiskName[strDiskName][0];
						if (!DirectoryExists(sField))
						{
							for (auto it : m_mapDiskName)
							{
								if (DirectoryExists(sField))
								{
									break;
								}
								sField[0] = it.second[0];
							}
						}
					}
					else
					{
						//string sTempF = sField;
						for (auto it : m_mapDiskName)
						{
							if (!m_bCloudSWitch)
							{
								break;
							}
							if (DirectoryExists(sField))
							{
								break;
							}
							sField[0] = it.second[0];
						}
					}
					m_mapNamePath[strName] = sField;// +".exe";
					m_mapLocalId[strName] = string(szKey);
					//CLogClient::WriteLog("扫描云更新游戏： %s--%s", strName.c_str(), m_mapNamePath[strName].c_str());
				}
			}
			else
			{
				break;
			}
		}
		::RegCloseKey(hKey);
	}
	CLogClient::WriteLog("从云更新注册表中解析出游戏库数据: %d", m_mapNamePath.size());
}

// 过滤顺网游戏库
void CBarInfo::FilterShunwangGameLib()
{
	char buf[100] = { 0 };
	list<string> ltHeader;
	DWORD len = GetLogicalDriveStringsA(100, buf);
	for (char *p = buf; *p; p += strnlen(p, 4) + 1)
	{
		string s = p;
		string st = s;
		st += "我的游戏";
		if (DirectoryOrFileExists(st))
		{
			st += "\\";
			CLogClient::WriteLog("扫描到文件夹： %s", st.c_str());
			ltHeader.push_back(st);
		}
		CLogClient::WriteLog("扫描到盘符： %s", s.c_str());
		ltHeader.push_back(s);
	}
	string strFileName;
	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		for (list<string>::iterator ith = ltHeader.begin(); ith != ltHeader.end(); ith++)
		{
			strFileName = *ith;
			strFileName += it->category_dir;
			strFileName += "\\";
			strFileName += it->game_name;
			//strFileName += "\\";
			string strLanuch = strFileName;
			strLanuch += "\\";
			if (!it->process.empty())
			{
				strFileName += "\\";
			}
			strFileName += it->process;
			//if (FileExists(strFileName) || DirectoryExists(strFileName))
			if (DirectoryOrFileExists(strFileName))
			{
				it->launcher = strLanuch + it->launcher;
				it->process = strFileName;
				it->category_1 = GetGameTypeLevel1Id(it->category_dir);
				m_ltGame.push_back(*it);
				break;
			}
		}
	}
}

// 过滤云更新游戏库
void CBarInfo::FilterCloudUpdateGameLib()
{
	CLogClient::WriteLog("过滤云更新游戏库");
	// 从云更新注册表中解析出游戏库数据
	GetGameLibFromCloudUpdate();
	string strFileName;
	string strIconName;
	string strType;
	string strLanuch;

	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		strFileName = it->game_name;
		//CLogClient::WriteLog("检测云更新游戏目录： %s, m_mapNamePath.count: %d", strFileName.c_str(), m_mapNamePath.size());
		if (m_mapNamePath.count(strFileName) > 0)
		{
			//CLogClient::WriteLog("检测云更新游戏目录： %s", m_mapNamePath[strFileName].c_str());
			if (DirectoryExists(m_mapNamePath[strFileName]) || !m_bCloudSWitch)
			{
				strLanuch = m_mapNamePath[strFileName] + "\\";
				it->launcher = strLanuch + it->launcher;
				strType = m_mapNamePath[strFileName].substr(3);
				strType = strType.substr(0, strType.find('\\'));
				it->process = strLanuch + it->process;
				it->category_1 = GetGameTypeLevel1Id(strType);

				it->game_icon = CConfigFile::GetInstance()->GetGameResPath() + "\\";
				it->game_icon += IntToStr(it->center_game_id);
				it->game_icon += ".png";
				if (!FileExists(it->game_icon))
				{
				    CLogClient::WriteLog("检测云更新游戏不存在图标目录： %s", it->game_icon.c_str());
					strIconName = strLanuch + m_mapLocalId[strFileName]+".ico";
					if (FileExists(strIconName))
					{
						it->game_icon = strIconName;
					}
				}
				m_ltGame.push_back(*it);
			}
			else
			{
				CLogClient::WriteLog("检测云更新游戏不存在游戏目录： %s", m_mapNamePath[strFileName].c_str());
			}
		}
		else
		{
			CLogClient::WriteLog("检测云更新游戏本地不存在游戏目录： %s", strFileName.c_str());
		}
	}
}

string CBarInfo::GetCharSpellCode(const string &strSource)
{
	string strRet;
	wstring sw = MultCharToWideCharA(strSource);
	for (int i = 0; i < sw.size(); i++)
	{
		strRet += GetCharSpellCodeI(sw[i]);
	}
	return strRet;
}

string GetFirstLetter(string strChar)
{

	string ls_second_eng = "CJWGNSPGCGNESYPBTYYZDXYKYGTDJNNJQMBSGZSCYJSYYQPGKBZGYCYWJKGKLJSWKPJQHYTWDDZLSGMRYPYWWCCKZNKYDGTTNGJEYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJZXDTTSQZYCDXXHGCKBPHFFSSWYBGMXLPBYLLLHLXSPZMYJHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZCDJZWQJBDZBXGZNZCPWHKXHQKMWFBPBYDTJZZKQHYLYGXFPTYJYYZPSZLFCHMQSHGMXXSXJJSDCSBBQBEFSJYHWWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPYCJNDQJWXQXFYYFJLEJBZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQTRBCJTHZTQFRXQ"
		"HXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSGLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWYWSBHTRXXGLHQHFNMNYKLDYXZPWLGGTMTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHZXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKOHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPCJOQLCDHJJYSPRCHNWJNLHLYYQYYWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMYDJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYDCFCXYHLXCHYZJQSFQAGMNYXPFRKSSB"
		"JLYXYSYGLNSCMHCWWMNZJJLXXHCHSYDSTTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLYSDCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQHZPQSQSCFYMMDMGBWHWLGSSLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZPMGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHBBGDMYCNQQWLQHJJZYWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSLXHTZKZJECXJCJNMFBYCSFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJXJYZGWKYP"
		"SGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJBPMLMSXLZJQQHZYJCZYDJWBMJKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHHJLJAXFGFJZSLCFDQZLCLGJDJCSNCLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTXYXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKYFCJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJS"
		"BBSGGHFJLWPMZJNLYYWDQSHZXTYYWHMCYHYWDBXBTLMSYYYFSXJCSDXXLHJHFSSXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMGYYDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGSQQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSPRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJJGYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTTSSXTBNPDKLEYCJNYCDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLGBDJLSLYGKDZPZXJYYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZB"
		"YTDKYXJYYCNRJMDJGKLCLJBCTBJDDBBLBLCZQRPXJCGLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRPHQQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYJDJJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZKHHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQJYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZM"
		"ALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNDSZFDEQFHBSAQTGLLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ";
	string ls_second_ch = "亍丌兀丐廿卅丕亘丞鬲孬噩丨禺丿匕乇夭爻卮氐囟胤馗毓睾鼗丶亟"
		"鼐乜乩亓芈孛啬嘏仄厍厝厣厥厮靥赝匚叵匦匮匾赜卦卣刂刈刎刭刳刿剀剌剞剡剜蒯剽劂劁劐劓冂罔亻仃仉仂仨仡仫仞伛仳伢佤仵伥伧伉伫佞佧攸佚佝佟佗伲伽佶佴侑侉侃侏佾佻侪佼侬侔俦俨俪俅俚俣俜俑俟俸倩偌俳倬倏倮倭俾倜倌倥倨偾偃偕偈偎偬偻傥傧傩傺僖儆僭僬僦僮儇儋仝氽佘佥俎龠汆籴兮巽黉馘冁夔勹匍訇匐凫夙兕亠兖亳衮袤亵脔裒禀嬴蠃羸冫冱冽冼凇冖冢冥讠讦讧讪讴讵讷诂诃诋诏诎诒诓诔诖诘诙诜诟诠诤诨诩诮诰诳诶诹诼诿谀谂谄谇谌谏谑谒谔谕谖谙谛谘谝谟谠谡谥谧谪谫谮谯谲谳谵谶卩卺阝阢阡阱阪阽阼"
		"陂陉陔陟陧陬陲陴隈隍隗隰邗邛邝邙邬邡邴邳邶邺邸邰郏郅邾郐郄郇郓郦郢郜郗郛郫郯郾鄄鄢鄞鄣鄱鄯鄹酃酆刍奂劢劬劭劾哿勐勖勰叟燮矍廴凵凼鬯厶弁畚巯坌垩垡塾墼壅壑圩圬圪圳圹圮圯坜圻坂坩垅坫垆坼坻坨坭坶坳垭垤垌垲埏垧垴垓垠埕埘埚埙埒垸埴埯埸埤埝堋堍埽埭堀堞堙塄堠塥塬墁墉墚墀馨鼙懿艹艽艿芏芊芨芄芎芑芗芙芫芸芾芰苈苊苣芘芷芮苋苌苁芩芴芡芪芟苄苎芤苡茉苷苤茏茇苜苴苒苘茌苻苓茑茚茆茔茕苠苕茜荑荛荜茈莒茼茴茱莛荞茯荏荇荃荟荀茗荠茭茺茳荦荥荨茛荩荬荪荭荮莰荸莳莴莠莪莓莜莅荼莶莩荽莸荻"
		"莘莞莨莺莼菁萁菥菘堇萘萋菝菽菖萜萸萑萆菔菟萏萃菸菹菪菅菀萦菰菡葜葑葚葙葳蒇蒈葺蒉葸萼葆葩葶蒌蒎萱葭蓁蓍蓐蓦蒽蓓蓊蒿蒺蓠蒡蒹蒴蒗蓥蓣蔌甍蔸蓰蔹蔟蔺蕖蔻蓿蓼蕙蕈蕨蕤蕞蕺瞢蕃蕲蕻薤薨薇薏蕹薮薜薅薹薷薰藓藁藜藿蘧蘅蘩蘖蘼廾弈夼奁耷奕奚奘匏尢尥尬尴扌扪抟抻拊拚拗拮挢拶挹捋捃掭揶捱捺掎掴捭掬掊捩掮掼揲揸揠揿揄揞揎摒揆掾摅摁搋搛搠搌搦搡摞撄摭撖摺撷撸撙撺擀擐擗擤擢攉攥攮弋忒甙弑卟叱叽叩叨叻吒吖吆呋呒呓呔呖呃吡呗呙吣吲咂咔呷呱呤咚咛咄呶呦咝哐咭哂咴哒咧咦哓哔呲咣哕咻咿哌哙哚哜咩"
		"咪咤哝哏哞唛哧唠哽唔哳唢唣唏唑唧唪啧喏喵啉啭啁啕唿啐唼唷啖啵啶啷唳唰啜喋嗒喃喱喹喈喁喟啾嗖喑啻嗟喽喾喔喙嗪嗷嗉嘟嗑嗫嗬嗔嗦嗝嗄嗯嗥嗲嗳嗌嗍嗨嗵嗤辔嘞嘈嘌嘁嘤嘣嗾嘀嘧嘭噘嘹噗嘬噍噢噙噜噌噔嚆噤噱噫噻噼嚅嚓嚯囔囗囝囡囵囫囹囿圄圊圉圜帏帙帔帑帱帻帼帷幄幔幛幞幡岌屺岍岐岖岈岘岙岑岚岜岵岢岽岬岫岱岣峁岷峄峒峤峋峥崂崃崧崦崮崤崞崆崛嵘崾崴崽嵬嵛嵯嵝嵫嵋嵊嵩嵴嶂嶙嶝豳嶷巅彳彷徂徇徉後徕徙徜徨徭徵徼衢彡犭犰犴犷犸狃狁狎狍狒狨狯狩狲狴狷猁狳猃狺狻猗猓猡猊猞猝猕猢猹猥猬猸猱獐獍獗獠獬獯獾"
		"舛夥飧夤夂饣饧饨饩饪饫饬饴饷饽馀馄馇馊馍馐馑馓馔馕庀庑庋庖庥庠庹庵庾庳赓廒廑廛廨廪膺忄忉忖忏怃忮怄忡忤忾怅怆忪忭忸怙怵怦怛怏怍怩怫怊怿怡恸恹恻恺恂恪恽悖悚悭悝悃悒悌悛惬悻悱惝惘惆惚悴愠愦愕愣惴愀愎愫慊慵憬憔憧憷懔懵忝隳闩闫闱闳闵闶闼闾阃阄阆阈阊阋阌阍阏阒阕阖阗阙阚丬爿戕氵汔汜汊沣沅沐沔沌汨汩汴汶沆沩泐泔沭泷泸泱泗沲泠泖泺泫泮沱泓泯泾洹洧洌浃浈洇洄洙洎洫浍洮洵洚浏浒浔洳涑浯涞涠浞涓涔浜浠浼浣渚淇淅淞渎涿淠渑淦淝淙渖涫渌涮渫湮湎湫溲湟溆湓湔渲渥湄滟溱溘滠漭滢溥溧溽溻溷滗溴滏溏滂"
		"溟潢潆潇漤漕滹漯漶潋潴漪漉漩澉澍澌潸潲潼潺濑濉澧澹澶濂濡濮濞濠濯瀚瀣瀛瀹瀵灏灞宀宄宕宓宥宸甯骞搴寤寮褰寰蹇謇辶迓迕迥迮迤迩迦迳迨逅逄逋逦逑逍逖逡逵逶逭逯遄遑遒遐遨遘遢遛暹遴遽邂邈邃邋彐彗彖彘尻咫屐屙孱屣屦羼弪弩弭艴弼鬻屮妁妃妍妩妪妣妗姊妫妞妤姒妲妯姗妾娅娆姝娈姣姘姹娌娉娲娴娑娣娓婀婧婊婕娼婢婵胬媪媛婷婺媾嫫媲嫒嫔媸嫠嫣嫱嫖嫦嫘嫜嬉嬗嬖嬲嬷孀尕尜孚孥孳孑孓孢驵驷驸驺驿驽骀骁骅骈骊骐骒骓骖骘骛骜骝骟骠骢骣骥骧纟纡纣纥纨纩纭纰纾绀绁绂绉绋绌绐绔绗绛绠绡绨绫绮绯绱绲缍绶绺绻绾缁缂缃"
		"缇缈缋缌缏缑缒缗缙缜缛缟缡缢缣缤缥缦缧缪缫缬缭缯缰缱缲缳缵幺畿巛甾邕玎玑玮玢玟珏珂珑玷玳珀珉珈珥珙顼琊珩珧珞玺珲琏琪瑛琦琥琨琰琮琬琛琚瑁瑜瑗瑕瑙瑷瑭瑾璜璎璀璁璇璋璞璨璩璐璧瓒璺韪韫韬杌杓杞杈杩枥枇杪杳枘枧杵枨枞枭枋杷杼柰栉柘栊柩枰栌柙枵柚枳柝栀柃枸柢栎柁柽栲栳桠桡桎桢桄桤梃栝桕桦桁桧桀栾桊桉栩梵梏桴桷梓桫棂楮棼椟椠棹椤棰椋椁楗棣椐楱椹楠楂楝榄楫榀榘楸椴槌榇榈槎榉楦楣楹榛榧榻榫榭槔榱槁槊槟榕槠榍槿樯槭樗樘橥槲橄樾檠橐橛樵檎橹樽樨橘橼檑檐檩檗檫猷獒殁殂殇殄殒殓殍殚殛殡殪轫轭轱轲轳轵轶"
		"轸轷轹轺轼轾辁辂辄辇辋辍辎辏辘辚軎戋戗戛戟戢戡戥戤戬臧瓯瓴瓿甏甑甓攴旮旯旰昊昙杲昃昕昀炅曷昝昴昱昶昵耆晟晔晁晏晖晡晗晷暄暌暧暝暾曛曜曦曩贲贳贶贻贽赀赅赆赈赉赇赍赕赙觇觊觋觌觎觏觐觑牮犟牝牦牯牾牿犄犋犍犏犒挈挲掰搿擘耄毪毳毽毵毹氅氇氆氍氕氘氙氚氡氩氤氪氲攵敕敫牍牒牖爰虢刖肟肜肓肼朊肽肱肫肭肴肷胧胨胩胪胛胂胄胙胍胗朐胝胫胱胴胭脍脎胲胼朕脒豚脶脞脬脘脲腈腌腓腴腙腚腱腠腩腼腽腭腧塍媵膈膂膑滕膣膪臌朦臊膻臁膦欤欷欹歃歆歙飑飒飓飕飙飚殳彀毂觳斐齑斓於旆旄旃旌旎旒旖炀炜炖炝炻烀炷炫炱烨烊焐焓焖焯焱"
		"煳煜煨煅煲煊煸煺熘熳熵熨熠燠燔燧燹爝爨灬焘煦熹戾戽扃扈扉礻祀祆祉祛祜祓祚祢祗祠祯祧祺禅禊禚禧禳忑忐怼恝恚恧恁恙恣悫愆愍慝憩憝懋懑戆肀聿沓泶淼矶矸砀砉砗砘砑斫砭砜砝砹砺砻砟砼砥砬砣砩硎硭硖硗砦硐硇硌硪碛碓碚碇碜碡碣碲碹碥磔磙磉磬磲礅磴礓礤礞礴龛黹黻黼盱眄眍盹眇眈眚眢眙眭眦眵眸睐睑睇睃睚睨睢睥睿瞍睽瞀瞌瞑瞟瞠瞰瞵瞽町畀畎畋畈畛畲畹疃罘罡罟詈罨罴罱罹羁罾盍盥蠲钅钆钇钋钊钌钍钏钐钔钗钕钚钛钜钣钤钫钪钭钬钯钰钲钴钶钷钸钹钺钼钽钿铄铈铉铊铋铌铍铎铐铑铒铕铖铗铙铘铛铞铟铠铢铤铥铧铨铪铩铫铮铯铳铴铵铷铹铼"
		"铽铿锃锂锆锇锉锊锍锎锏锒锓锔锕锖锘锛锝锞锟锢锪锫锩锬锱锲锴锶锷锸锼锾锿镂锵镄镅镆镉镌镎镏镒镓镔镖镗镘镙镛镞镟镝镡镢镤镥镦镧镨镩镪镫镬镯镱镲镳锺矧矬雉秕秭秣秫稆嵇稃稂稞稔稹稷穑黏馥穰皈皎皓皙皤瓞瓠甬鸠鸢鸨鸩鸪鸫鸬鸲鸱鸶鸸鸷鸹鸺鸾鹁鹂鹄鹆鹇鹈鹉鹋鹌鹎鹑鹕鹗鹚鹛鹜鹞鹣鹦鹧鹨鹩鹪鹫鹬鹱鹭鹳疒疔疖疠疝疬疣疳疴疸痄疱疰痃痂痖痍痣痨痦痤痫痧瘃痱痼痿瘐瘀瘅瘌瘗瘊瘥瘘瘕瘙瘛瘼瘢瘠癀瘭瘰瘿瘵癃瘾瘳癍癞癔癜癖癫癯翊竦穸穹窀窆窈窕窦窠窬窨窭窳衤衩衲衽衿袂裆袷袼裉裢裎裣裥裱褚裼裨裾裰褡褙褓褛褊褴褫褶襁襦疋胥皲皴矜耒"
		"耔耖耜耠耢耥耦耧耩耨耱耋耵聃聆聍聒聩聱覃顸颀颃颉颌颍颏颔颚颛颞颟颡颢颥颦虍虔虬虮虿虺虼虻蚨蚍蚋蚬蚝蚧蚣蚪蚓蚩蚶蛄蚵蛎蚰蚺蚱蚯蛉蛏蚴蛩蛱蛲蛭蛳蛐蜓蛞蛴蛟蛘蛑蜃蜇蛸蜈蜊蜍蜉蜣蜻蜞蜥蜮蜚蜾蝈蜴蜱蜩蜷蜿螂蜢蝽蝾蝻蝠蝰蝌蝮螋蝓蝣蝼蝤蝙蝥螓螯螨蟒蟆螈螅螭螗螃螫蟥螬螵螳蟋蟓螽蟑蟀蟊蟛蟪蟠蟮蠖蠓蟾蠊蠛蠡蠹蠼缶罂罄罅舐竺竽笈笃笄笕笊笫笏筇笸笪笙笮笱笠笥笤笳笾笞筘筚筅筵筌筝筠筮筻筢筲筱箐箦箧箸箬箝箨箅箪箜箢箫箴篑篁篌篝篚篥篦篪簌篾篼簏簖簋簟簪簦簸籁籀臾舁舂舄臬衄舡舢舣舭舯舨舫舸舻舳舴舾艄艉艋艏艚艟艨衾袅袈裘裟襞羝羟"
		"羧羯羰羲籼敉粑粝粜粞粢粲粼粽糁糇糌糍糈糅糗糨艮暨羿翎翕翥翡翦翩翮翳糸絷綦綮繇纛麸麴赳趄趔趑趱赧赭豇豉酊酐酎酏酤酢酡酰酩酯酽酾酲酴酹醌醅醐醍醑醢醣醪醭醮醯醵醴醺豕鹾趸跫踅蹙蹩趵趿趼趺跄跖跗跚跞跎跏跛跆跬跷跸跣跹跻跤踉跽踔踝踟踬踮踣踯踺蹀踹踵踽踱蹉蹁蹂蹑蹒蹊蹰蹶蹼蹯蹴躅躏躔躐躜躞豸貂貊貅貘貔斛觖觞觚觜觥觫觯訾謦靓雩雳雯霆霁霈霏霎霪霭霰霾龀龃龅龆龇龈龉龊龌黾鼋鼍隹隼隽雎雒瞿雠銎銮鋈錾鍪鏊鎏鐾鑫鱿鲂鲅鲆鲇鲈稣鲋鲎鲐鲑鲒鲔鲕鲚鲛鲞鲟鲠鲡鲢鲣鲥鲦鲧鲨鲩鲫鲭鲮鲰鲱鲲鲳鲴鲵鲶鲷鲺鲻鲼鲽鳄鳅鳆鳇鳊鳋鳌鳍鳎鳏鳐鳓鳔"
		"鳕鳗鳘鳙鳜鳝鳟鳢靼鞅鞑鞒鞔鞯鞫鞣鞲鞴骱骰骷鹘骶骺骼髁髀髅髂髋髌髑魅魃魇魉魈魍魑飨餍餮饕饔髟髡髦髯髫髻髭髹鬈鬏鬓鬟鬣麽麾縻麂麇麈麋麒鏖麝麟黛黜黝黠黟黢黩黧黥黪黯鼢鼬鼯鼹鼷鼽鼾齄";
	string return_py = "";
	if ((unsigned char)strChar[0] >= 215) //二级汉字
	{
		int nPos = ls_second_ch.find(strChar);
		if (string::npos != nPos)
		{
			return_py = ls_second_eng[nPos / 2];
		}
	}
	return return_py;
}

string CBarInfo::GetCharSpellCodeI(wchar_t wChar)
{
	string strRet;
	std::string sx = WideCharToMultiCharW(&wChar, 1);
	if (sx.size() == 1)
	{
		std::transform(sx.begin(), sx.end(), sx.begin(), toupper);
		return sx;
	}
	//sx = m2m_utf8(sx.c_str(), sx.size());
	unsigned char h = sx[0];
	unsigned char l = sx[1];
	unsigned short tmp = (sx[0] & 0xff) << 8;
	tmp |= (sx[1] & 0xff);
	//unsigned short tmp = htons(*((unsigned short *)sx.c_str()));
	if (tmp >= 45217 && tmp <= 45252) strRet = "A";
	else if (tmp >= 45253 && tmp <= 45760) strRet = "B";
	else if (tmp >= 45761 && tmp <= 46317) strRet = "C";
	else if (tmp >= 46318 && tmp <= 46825) strRet = "D";
	else if (tmp >= 46826 && tmp <= 47009) strRet = "E";
	else if (tmp >= 47010 && tmp <= 47296) strRet = "F";
	else if (tmp >= 47297 && tmp <= 47613) strRet = "G";
	else if (tmp >= 47614 && tmp <= 48118) strRet = "H";
	else if (tmp >= 48119 && tmp <= 49061) strRet = "J";
	else if (tmp >= 49062 && tmp <= 49323) strRet = "K";
	else if (tmp >= 49324 && tmp <= 49895) strRet = "L";
	else if (tmp >= 49896 && tmp <= 50370) strRet = "M";
	else if (tmp >= 50371 && tmp <= 50613) strRet = "N";
	else if (tmp >= 50614 && tmp <= 50621) strRet = "O";
	else if (tmp >= 50622 && tmp <= 50905) strRet = "P";
	else if (tmp >= 50906 && tmp <= 51386) strRet = "Q";
	else if (tmp >= 51387 && tmp <= 51445) strRet = "R";
	else if (tmp >= 51446 && tmp <= 52217) strRet = "S";
	else if (tmp >= 52218 && tmp <= 52697) strRet = "T";
	else if (tmp >= 52698 && tmp <= 52979) strRet = "W";
	//else if (tmp >= 52980 && tmp <= 53640) strRet = "X";
	else if (tmp >= 52980 && tmp <= 53688) strRet = "X";
	else if (tmp >= 53689 && tmp <= 54480) strRet = "Y";
	else if (tmp >= 54481 && tmp <= 55289) strRet = "Z";
	else strRet = GetFirstLetter(sx);
	return strRet;
}

// 加载第三方控件
void CBarInfo::LoadThreadPlug()
{
	// 运行菜单
	ConfigDataItem cfg = GetLocalConfig();
	string strPathName = CConfigFile::GetInstance()->GetGameMenuProgram();
	CLogClient::WriteLog("运行菜单目录：%s", strPathName.c_str());
	if (!strPathName.empty() && CConfigFile::GetInstance()->GetIsCallMenu())
	{
		if (FileExists(strPathName.c_str()))
		{
			HINSTANCE h = ShellExecuteA(NULL, "open", strPathName.c_str(), NULL, NULL, SW_SHOWNORMAL);
			CLogClient::WriteLog("运行菜单结果：%d--%d", h, ::GetLastError());
			wstring strDest = GetDesktopPath();
			wstring strDest2 = strDest;
			wstring strShutKey;
			strDest += L"\\";
			if (cfg.game_menu.empty())
			{
				//strDest += L"游戏菜单";
				strShutKey = MultCharToWideCharA(CConfigFile::GetInstance()->GetLinkFileName());
				strDest += strShutKey;
			}
			else
			{
				strShutKey = MultCharToWideCharA(cfg.game_menu);
				strDest += strShutKey;
			}
			strShutKey += L".lnk";
			strDest += L".lnk";
			if (CConfigFile::GetInstance()->GetIsCreateDeskIcon())
			{
				if (CreateLinkFile(MultCharToWideCharA(strPathName).c_str(), L"", strDest.c_str(), 0))
				{
					TaskbarPin((LPTSTR)strDest2.c_str(), (LPTSTR)strShutKey.c_str());
				}
			}
		}
	}

	// 运行小程序
	string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	strMinProgramName += "\\ThreePlug.exe";
	CLogClient::WriteLog("小程序全名：%s", strMinProgramName.c_str());
	if (FileExists(strMinProgramName.c_str()))
	{
		HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
		CLogClient::WriteLog("运行小程序结果：%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	}

	// 加载第三方接口模块
	int nRet = CCommInterface::LoadThreePlug();
	if (nRet == 0)
	{
		CLogClient::WriteLog("加载接口成功");
	}
	else
	{
		CLogError::WriteLog("加载接口失败： %d", nRet);
	}
}

wstring CBarInfo::GetProcessNameById(DWORD dwId)
{
	HANDLE h;
	PROCESSENTRY32 pe;
	wstring strRet;
	// 获取进程快照
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE != h)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(h, &pe))
		{
			do{
				if (dwId == pe.th32ProcessID)
				{
					strRet = pe.szExeFile;
					break;
				}
			} while (Process32Next(h, &pe));
		}
		CloseHandle(h);
	}
	return strRet;
}

/*

函数功能：对指定文件在指定的目录下创建其快捷方式

函数参数：

lpszFileName    指定文件，为NULL表示当前进程的EXE文件。

lpszLnkFileDir  指定目录，不能为NULL。

lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。

wHotkey         为0表示不设置快捷键

pszDescription  备注

iShowCmd        运行方式，默认为常规窗口

*/
//
//BOOL CreateFileShortcut(LPCWSTR lpszFileName, LPCWSTR lpszLnkFileDir, LPCWSTR lpszLnkFileName, LPCWSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL)
//{
//	if (lpszLnkFileDir == NULL)
//		return FALSE;
//	HRESULT hr;
//	IShellLink     *pLink;  //IShellLink对象指针
//	IPersistFile   *ppf; //IPersisFil对象指针
//	//创建IShellLink对象
//	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
//	if (FAILED(hr))
//		return FALSE;
//	//从IShellLink对象中获取IPersistFile接口
//	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
//	if (FAILED(hr))
//	{
//		pLink->Release();
//		return FALSE;
//	}
//	//目标
//	pLink->SetPath(lpszFileName);
//	//工作目录
//	if (lpszWorkDir != NULL)
//		pLink->SetPath(lpszWorkDir);
//	//快捷键
//	if (wHotkey != 0)
//		pLink->SetHotkey(wHotkey);
//	//备注
//	if (lpszDescription != NULL)
//		pLink->SetDescription(lpszDescription);
//	//显示方式
//	pLink->SetShowCmd(iShowCmd);
//	//快捷方式的路径 + 名称
//	char szBuffer[MAX_PATH];
//	if (lpszLnkFileName != NULL) //指定了快捷方式的名称
//		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, lpszLnkFileName);
//	else
//	{
//		//没有指定名称，就从取指定文件的文件名作为快捷方式名称。
//		char *pstr;
//		if (lpszFileName != NULL)
//			pstr = strrchr(lpszFileName, '\\');
//		else
//			pstr = strrchr(_pgmptr, '\\');
//		if (pstr == NULL)
//		{
//			ppf->Release();
//			pLink->Release();
//			return FALSE;
//		}
//		//注意后缀名要从.exe改为.lnk
//		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, pstr);
//		int nLen = strlen(szBuffer);
//		szBuffer[nLen - 3] = 'l';
//		szBuffer[nLen - 2] = 'n';
//		szBuffer[nLen - 1] = 'k';
//	}
//	//保存快捷方式到指定目录下
//	WCHAR  wsz[MAX_PATH];  //定义Unicode字符串
//	MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, wsz, MAX_PATH);
//	hr = ppf->Save(wsz, TRUE);
//	ppf->Release();
//	pLink->Release();
//	return SUCCEEDED(hr);
//}
//
//void CreateLinkFile(const string &strStartAppPath, const string &strDestLnkPath)
//{
//	//CFileLink  link;
//	//TChar     szLink[260] = { 0 };
//	//TChar     szPath[260] = { _T("要创建快捷方式的文件路径") };
//
//	SHGetSpecialFolderPath(NULL, szLink, CSIDL_DESKTOP, FALSE);
//	_tcscat(szLink, _T("ExePath"));//"快捷方式在桌面显示的名字"
//
//	_tcscat(szLink, _T(".lnk"));
//	link.CreateLink(szLink);
//	link.SetLinkInfo(szPath);
//}
//
