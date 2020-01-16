#pragma once
class CDingDingTalking
{
public:
	CDingDingTalking(void);
	~CDingDingTalking(void);
public:
	void InitDingTalking(CString szCorpId,CString szCorpSecret);
	CString GetToken();
	BOOL GetDingTalkingAllInfoList(Json::Value& JsonEntList,Json::Value& JsonDeptList,Json::Value& JsonUserList);

	BOOL GetDingTalkingList(Json::Value& JsonEntList);
	BOOL GetDingTalkingDeptList(Json::Value& JsonDeptList);
	BOOL GetDingTalkingUserList(Json::Value JsonDeptList,Json::Value& JsonUserList);
	BOOL PharseJson(CString szJson,Json::Value &json);
protected:
	BOOL GetHttpsByUrl(CString url, CString &szMsgBack);
protected:
	CString m_szCorpId;
	CString m_szCorpSecret;
	CString m_szToken;
};
