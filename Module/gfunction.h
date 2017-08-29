#ifndef _GFUNCTION_H
#define _GFUNCTION_H

#pragma once

#include <string>
#include <stdarg.h> // va_list
#include <iostream>		// cout, cin
#include "datatypedef.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
// �ַ�����غ���--------------------------------------------------------------- 
// �Ƿ����������� 
bool IsInt(const char *pStr);
// �ַ���ת�������� 
int StrToInt(const char *pStr, int nDefault = 0);
int LStrToInt(const char *pStr, int nDefault = 0);
// ��ʽ���ַ��� 
void FormatStringV(string& strResult, const char *pFormatString, va_list argList);
void FormatString(string& strResult, const char *pFormatString, ...);
void FormatStringWV(wstring& strResult, const wchar_t *pFormatString, va_list argList);
void FormatStringW(wstring& strResult, const wchar_t *pFormatString, ...);
// ����ת�ַ��� 
string IntToStr(int nValue);
// �ַ����滻 
int StringReplace(char *pStr, int nSize, char *pFrom, char *pTo);
// �ָ��ַ���
int StringSplitToInt(const char *pStr, const char *pSplitter, int pIntList[], int nListSize, int *nCount);
// ɾ��ָ���ַ�
void EraseChar(string& s, char ch);
// ����ָ���ַ��ָ��ַ���
void ParseStringBySpecChar(const string &strSource, string &strLeft, string &strRight, char ch = PROTOCOL_COMMON_SPLIT_CHAR);
// ת��UTF8�����ر���
string Utf8ToLocal(const string &strSource);
// ��char *ת����wstring
wstring MultCharToWideCharA(const char *pStr, int nLen);
wstring MultCharToWideCharA(const string &strSource);
// ��wchar_t *ת����string  
string WideCharToMultiCharW(const wchar_t *pStr, int nLen);
string WideCharToMultiCharW(const wstring &strSource);
// ���ֽڱ���ת����utf8���ֽڱ���
std::string w2m_utf8(const wstring &s);
// gb2312����ת����utf8���ֽڱ���
std::string m2m_utf8(const char *pStr, int nLen);
// �汾�Ƚ�
// nRet��ֵ��
// >0 strVer1 > strVer2
// =0 strVer1 > strVer2
// <0 strVer1 < strVer2
bool CompareVersion(const string &cstrVer1, const string &cstrVer2, int &nRet);
//����ʱ���ַ���
bool ParseHMString(const string &strHM, int &nHour, int &nMin);


////////////////////////////////////////////////////////////////////////////////
//�ļ���Ŀ¼--------------------------------------------------------------------
bool FileExists(const string& strDir);
bool DirectoryExists(const string& strDir);
bool DirectoryExistsW(const wstring& strDir);
bool DirectoryOrFileExists(const string& strDirOrFile);
bool CreateDir(const string& strDir);
string ExtractFilePath(const string& strFileName);
bool ForceDirectories(string strDir);
// ��·���л�ȡ�ļ���
string GetFileNameByPath(string strDir);
// ���ļ������ڴ�
char *GetFileMemBuf(string strLocalPath, int &nLen);
//���纯��
string IpToString(int nIp);
int StringToIp(const string& strString);
string GetLocalIp();

////////////////////////////////////////////////////////////////////////////////
// ϵͳ��غ���----------------------------------------------------------------- 
// ��ȡ��ִ���ļ�·��(��/)
string &GetModulePath(); 
// ��ȡ��ִ���ļ���
wstring &GetAppPathName();
// ��ȡ�ļ��汾
wstring GetFileVersionEx(const wstring &strFileName);
string GetFileVersionEx(const string &strFileName);
// ��ȡȫ�ֵı������
int GetCurPacketNumber();
// ����ȫ�ֵı������
int AllocCurPacketNumber();

// �������֤�����е��Ա�
int GetSexById(const string &strIDNo);

//#ifdef _XP_TEST
#define WGetTickCount() ::GetTickCount()
//#else
//#define WGetTickCount() ::GetTickCount64()
//#endif
#endif
