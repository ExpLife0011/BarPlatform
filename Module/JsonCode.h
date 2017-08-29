// JsonCode.h
#pragma once
#include "gclass.h"
//#include "AppClientInfo.h"
#include "json.h"
//#include "YYBBList.h"

class CJsonEncode :public CSingleServer<CJsonEncode>
{
public:
	//// �����û���¼��Ϣ
	//wstring EnCodeUserLoginW(const CUserInfo &info, const string &strBarName, const string &strBarId, const list<CUserInfo> &ltUserInfo, const string &strFistLogin);
	//// �������̨��¼��Ϣ
	//wstring EnCodeControlLoginW(const string &strBarId, const string &strBarName, const CUserInfo &info, const list<CUserInfo> &ltUserInfo);
	//// ���������û���¼��Ϣ
	//wstring EnCodeOtherUserLoginW(const CUserInfo &info);
	//// ���������û��ǳ���Ϣ
	//wstring EnCodeOtherUserLogOutW(const string &strUserNo, const string &strMachine);
	//// ���뼴ʱ��Ϣ
	//wstring EnCodeUserPChatW(const string &strUserNo, const string &strUserMachine, const string &strMsg, const string &strUserSex);
	//// �����ָ̨����Ļ
	//wstring EnCodeUserPDChatW(const string &strUserNo, const string &strMsg);
	//wstring EnCodeUserGChatW(const string &strUserNo, const string &strMsg, const string &strUserSex);
	//// ��������������¼��Ϣ(nSex: 0 - Ů; 1 - ��; 2 - δ֪) 
	//string EnCodeUserLoginYYBB(const string &strMachine, int nSex = 0);
	//// ������񲥱���¼��Ϣ(strLevel:##) 
	//string EnCodeGodYYBB(const string &strMachine, const string &strLevel);
	//// ��������MP3������Ϣ
	//static string EnCodeYYBBMp3Message(const string &broadContent, const string &broadContent2);
	//// ��������TTS������Ϣ
	//static string EnCodeYYBBTTSMessage(const string &broadContent);
	//// ������з�������
	//wstring EnCodeCallServerW(const string &strMachineName);
	//// ����̨�����û���Ϸ����
	//wstring EnCodeGameInfoW(const string &strMachineName, const string &strTier, const string &strLevel, const string &strWin, const string &strAreaId, const string &strUserQQId, const string &strNickName, const string &strSex, const string &strUserId);
	//// ����̨�����˳���Ϸ����
	//wstring EnCodeExitGameInfoW(const string &strMachineName, const string &strGameType, const string &strGameState);

	//// ����̨�����˳���Ϸ����
	//wstring EnCodeBeginGameInfoW(const string &strMachineName, const string &strGameType, const string &strGameState);
	//
	//// �����������
	//static string EnCodeMatchDetail(const list<CMatchDetail> &ltDetail);
	//// �����������
	//static string EnCodeMatchCommonData(const string &strAction, list<CMatchCommonInfo> &ltMatchInfo);
	//// ����������� - WebSocket������������, ����Ӧ�ò�
	//static string EnCodeMatchToAppA(const string &strRecv);
	//static wstring EnCodeMatchToAppW(const string &strRecv);
	//static string EnCodeMatchToApp(const string &strActionName, const string &strRecv);
	//// �����ȡͨ�õ��ϲ�json����
	//static wstring EnCodeGetCommonToAppW(const string &strActionName, const string strRecv);

	//// ����������ӿ�����
	//// ����������Ϣ
	//static wstring EnCodeThirdBarInfo();
	//// �����û���Ϣ
	//static wstring EnCodeThirdUserInfo();
	//// ����۷���Ϣ
	//static wstring EnCodeThirdChangeMoneyInfo();

	//// 1�����ݣ��װ�����ۣ�������Ϊ������׼���˷ḻ����Ʒ����ʳ��������Ҫ�밴�����壡
	//static string EncodeDSBBMessage1();
	////2���װ�����ۣ���������ˣ��밴���ϵķ����壬��֭����ͳͳ����Ŷ��
	//static string EncodeDSBBMessage2();
	////3���װ�����ۣ����˱���裬�������������������Ա��ɱ�����Ŷ��
	//static string EncodeDSBBMessage3();
	////4���װ�����ۣ����������ò�ʱ�䣬��Ҫ��͵������밴�����壬���ǽ��߳�Ϊ������
	//static string EncodeDSBBMessage4();
	////5���װ�����ۣ����ʱ�䵽��baby�������������������壡
	//static string EncodeDSBBMessage5();
	////6���װ�����ۣ�Ϊ���������˵����彡�����벻Ҫ�����������̣��������̵Ĺ˿ͣ��뵽ָ������������лл������ף��������죡
	//static string EncodeDSBBMessage6();
	////7�������кܶ����ӣ�Ϊ�����ӵĽ��������̵�С�����ȥ�������ɡ��������Ӹ�л����Ŷ��
	//static string EncodeDSBBMessage7();
	////8���װ�����ۣ���Ϸ�����ټ��ң�ҲҪ�ǵÿ��ܺã�����Я���Ĺ�����ƷŶ��
	//static string EncodeDSBBMessage8();
	////9���װ�����ۣ��뱣�ܺ����Ĺ�����Ʒ���ֻ�Ǯ����Ҫ�������棬���ⶪʧ ��лл������ף��������죡
	//static string EncodeDSBBMessage9();
	////10���װ�����ۣ����ǿ�ͨ��Ӣ��������ȨŶ��ȫӢ��ȫƤ����10%����ӳɣ�һ�����Ҳ���������
	//static string EncodeDSBBMessage10();
	////11���װ�����ۣ��������ṩƻ���Ͱ�׿����ߣ�������Ҫ�밴������Ŷ��
	//static string EncodeDSBBMessage11();
	////12������λ��ע�⣬����λ��ע�⣬�����߽�һλƯ�����ӡ�
	//static string EncodeDSBBMessage12();
	////13���װ�����ۣ����������Ҫ��ô��ڨ�����ҷ���ԱС�������Ͱ���۵ı��׻��ɣ�
	//static string EncodeDSBBMessage13();
	////14���װ�����ۣ������ǳ���ı�Ǯ�������棬�����ƿ���ڶ��棬�Թ���˵�ã�
	//static string EncodeDSBBMessage14();
	////15���װ�����ۣ���ָ̫æû�պ�ˮ�𣿰��·����壬��Ʒ���̷��ϡ�
	//static string EncodeDSBBMessage15();
	////16��������鿨����Ϊ�˷�ֹ�ظ��۷ѣ��벻Ҫ��������������������ʣ�����ϵ��̨����Ա��
	//static string EncodeDSBBMessage16();
	////17���װ�����ۣ����������ڽ���Ӣ�����˱������з��ƷŶ���Ͽ�Լ�϶�������̨�����ɡ�
	//static string EncodeDSBBMessage17(const string &strGameName, const string &strGameType);
	////18���װ�����ۣ�����������10���Ӻ����ϵͳ�������鷳����ʱ�»�����л����������⡣
	//static string EncodeDSBBMessage18();
	////19���װ�����ۣ�����С���Ĳ�иŬ�������ڵ�����ָ���������Ҵ���������б�Ǹ����ӭС����Ǽ����ϻ����ڣ�ף�����Ŀ��ģ�
	//static string EncodeDSBBMessage19();
	////20����ӭ��������������һ���������ʡ�
	//static string EncodeDSBBMessage20();
	////21���װ�����ۣ�������ͨ��ʱ��Ϊ����24������������8�㣬������鿨����Ϊ�˷�ֹ�ظ��۷ѣ��벻Ҫ��������������������ʣ�����ϵ��̨����Ա��
	//static string EncodeDSBBMessage21(const string &strBeginTime, const string &strEndTime);
	//// ͨ�ö�ʱ����
	//static string EncodeDSBBCommMsg(const string &s);
	//// ����ǰ����
	//static string InsertYYBBPreVoice(const string &s);

private:
	CJsonEncode();
	~CJsonEncode();
	friend class CSingle<CJsonEncode>;

};

//struct LolTeamItem
//{
//	string strIndex;
//	string strTeamId;
//	string strName;
//	string strType;
//};
//
//struct UserInfoFromYServerItem
//{
//	string strUserId;
//	string strUserName;
//	string strBarId;
//	// ֤������
//	string strCertId;
//	// ֤������
//	string strCertType;
//	// �Ա�
//	string strSex;
//};
//
class CJsonDecode :public CSingleServer<CJsonDecode>
{
public:
	// ��������������
	static bool DeCodeSelfUpdateData(const string &strData, UpLevelDataItem &Item);
	// ������Ϸ��
	static bool DeCodeGameData(const string &strData, list<GameLibItem> &lt);
	// ������������
	static bool DeCodeConfigData(const string &strData, ConfigDataItem &Item);
	// ������������2
	static bool DeCodeConfigIIData(const string &strData, ConfigIIItem &Item);
	//// ������Ϸ������ȡ�����б���������
	//static bool DeCodeGetRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// ������Ϸ�������������б���������
	//static bool DeCodeCreateRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// ������Ϸ�����޸ķ����б���������
	//static bool DeCodeModifyRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// ��������������ȡ��ʱ�б���������
	//static bool DeCodeGetYYBBListRet(const string &strRecv, list<YYBBItem> &lt);
	//// ������ȡ���÷�������
	//static bool DeCodeGetBarConfigRes(const string &strR, ConfigItem &itemRes);
	//// �������ø�����Ϣ��������
	//static bool DeCodeUpdateUserRes(const string &strR);
	//// ������¼����Ӧ������
	//static void DeCodeLoginReq(const string &strInfo, string &strBarId, CUserInfo &Info);
	////Base64����
	//static string DecodeBase64(const char* Data, int DataByte);
	//////����LOL��λ
	//static string DecodeLolLevelRes(const string &strUserInfo);
	//////��������̨��Ϸ��Ϣ
	//static bool DecodeControlGameInfoRes(const string &strUserInfo, string &strUserId, string &strTier, string &strLevel, string &strWin, string &strAreaId, string &strUserQQId, string &strNickName);
	//static string DecodeMatchGameInfoRes(const string &strUserInfo, const string &strUserMatchUser);
	//static bool DecodeLolInfo(string &strAction, const string &strJson, string &strUserId, string &strArea, string &strGameName, string &strTier, string &strMathInfo, string &strTeamList, string &strWinner, list<LolTeamItem> &lt);
	//static bool DecodeLolActionAndValues(const string &strJson, string &strAction, string &strValues);
	//// �������������ȡ�����û�ID
	//static bool DeCodeGetUserIDFromYS(const string &strJson, UserInfoFromYServerItem &Item);
	//static bool DeCodeGetUserInfoFromYS(const string &strJson, UserInfoFromYServerItem &Item);
	//// ����������ȡ�����û����
	//static bool DecodeUserMoney(const string &s, int &nMoney);
private:
	CJsonDecode();
	~CJsonDecode();
	friend class CSingle<CJsonDecode>;


};

namespace Base64
{
	static std::string const base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	/// Test whether a character is a valid base64 character
	/**
	* @param c The character to test
	* @return true if c is a valid base64 character
	*/
	static inline bool is_base64(unsigned char c) {
		return (c == 43 || // +
			(c >= 47 && c <= 57) || // /-9
			(c >= 65 && c <= 90) || // A-Z
			(c >= 97 && c <= 122)); // a-z
	}

	/// Encode a char buffer into a base64 string
	/**
	* @param input The input data
	* @param len The length of input in bytes
	* @return A base64 encoded string representing input
	*/
	inline std::string base64_encode(unsigned char const * input, size_t len) {
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (len--) {
			char_array_3[i++] = *(input++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
					((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
					((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++) {
					ret += base64_chars[char_array_4[i]];
				}
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 3; j++) {
				char_array_3[j] = '\0';
			}

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
				((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
				((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++) {
				ret += base64_chars[char_array_4[j]];
			}

			while ((i++ < 3)) {
				ret += '=';
			}
		}

		return ret;
	}

	/// Encode a string into a base64 string
	/**
	* @param input The input data
	* @return A base64 encoded string representing input
	*/
	inline std::string base64_encode(std::string const & input) {
		return base64_encode(
			reinterpret_cast<const unsigned char *>(input.data()),
			input.size()
			);
	}

	/// Decode a base64 encoded string into a string of raw bytes
	/**
	* @param input The base64 encoded input data
	* @return A string representing the decoded raw bytes
	*/
	inline std::string base64_decode(std::string const & input) {
		size_t in_len = input.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (input[in_] != '=') && is_base64(input[in_])) {
			char_array_4[i++] = input[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++) {
					char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
				}

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++) {
					ret += char_array_3[i];
				}
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) {
				ret += static_cast<std::string::value_type>(char_array_3[j]);
			}
		}

		return ret;
	}
}