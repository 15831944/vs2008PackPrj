#include "StdAfx.h"
#include "DingDingTalking.h"


CDingDingTalking::CDingDingTalking(void)
{
	m_szCorpId = "";
	m_szCorpSecret = "";
	m_szToken = "";
}

CDingDingTalking::~CDingDingTalking(void)
{
}
void CDingDingTalking::InitDingTalking(CString szCorpId,CString szCorpSecret)
{
	m_szCorpId = szCorpId;
	m_szCorpSecret = szCorpSecret;
	theLog.Write("CDingDingTalking::InitDingTalking,m_szCorpId=%s,m_szCorpSecret=%s",m_szCorpId,m_szCorpSecret);
	m_szToken = GetToken();
	theLog.Write("CDingDingTalking::InitDingTalking,m_szToken=%s",m_szToken);
}
CString CDingDingTalking::GetToken()
{
	CString szHttpUrl;
	CString szToken;
	szHttpUrl.Format("https://oapi.dingtalk.com/gettoken?corpid=%s&corpsecret=%s",m_szCorpId,m_szCorpSecret);
	CString szMsgBack;
	if (!GetHttpsByUrl(szHttpUrl,szMsgBack))
	{
		theLog.Write("!!CDingDingTalking::GetToken,failed");
		return "";
	}
	//theLog.Write("CDingDingTalking::GetToken,0,szHttpUrl=%s,szMsgBack=%s",szHttpUrl,szMsgBack);
	Json::Value json;
	if (PharseJson(szMsgBack.GetString(),json))
	{
		theLog.Write("CDingDingTalking::GetToken,json=%s",json.toStyledString().c_str());
		int nErrCode = json["errcode"].asInt();
		if (nErrCode != 0)
		{
			szToken = "";
			theLog.Write("!!CDingDingTalking::GetToken,1,failed,nErrCode=%d",nErrCode);
		}
		else
		{
			szToken = json["access_token"].asString().c_str();
		}
	}
	else
	{
		szToken = "";
		theLog.Write("!!CDingDingTalking::GetToken,2,failed");
	}
	return szToken;
}
BOOL CDingDingTalking::GetDingTalkingAllInfoList(Json::Value& JsonEntList,Json::Value& JsonDeptList,Json::Value& JsonUserList)
{
	BOOL bRet = TRUE;
	do 
	{
		if (m_szToken.IsEmpty())
		{
			bRet = FALSE;
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,fail");
			break;
		}
		CString szUrl;
		szUrl.Format("https://oapi.dingtalk.com/department/list?access_token=%s",m_szToken);
		CString szMsgBack;
		if (!GetHttpsByUrl(szUrl,szMsgBack))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,SendRequest,failed");
			bRet = FALSE;
			break;
		}
		Json::Value jsonall;
		if (!PharseJson(szMsgBack,jsonall))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,PharseJson,failed");
			bRet = FALSE;
			break;
		}
		else
		{
			//theLog.Write("==============jsonall=%s",jsonall.toStyledString().c_str());
			int nErrCode = jsonall["errcode"].asInt();
			if (nErrCode != 0)
			{
				bRet = FALSE;
				theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
				break;
			}
			else
			{
				try
				{
					Json::Value& JsonDeptListTmp = jsonall["department"];
					//遍历部门
					theLog.Write("部门个数,JsonDeptList.size()=%d",JsonDeptListTmp.size());
					int nSize = JsonDeptListTmp.size();
					for (int index=0; index < nSize; index++)
					{
						Json::Value& dept = JsonDeptListTmp[index];
						theLog.Write("正在获取第%d个【%s】",index+1,dept["name"].asString().c_str());
						CString szLog;
						
						if ((index+1)%2 == 0 || index == nSize-1)//每三次更新界面
						{
							szLog.AppendFormat("正在获取部门【%s】以及用户信息",dept["name"].asString().c_str());
							if (g_pIsecSyncDingTalk)
							{
								g_pIsecSyncDingTalk->OnUiLog(szLog);
							}
						}
						
						int nDeptId = dept["id"].asInt();
						int nParentId = dept["parentid"].asInt();
						if (nDeptId == 1 && nParentId == 0)
						{
							//添加企业list
							JsonEntList.append(dept);
						}
						else
						{
							//添加部门list
							JsonDeptList.append(dept);
						}
						CString szUrlUser;
						CString szJsonAllUser;
						Json::Value jsonAllUser;
						szUrlUser.Format("https://oapi.dingtalk.com/user/list?access_token=%s&department_id=%d",m_szToken,nDeptId);
						if (GetHttpsByUrl(szUrlUser,szJsonAllUser))
						{
							if (PharseJson(szJsonAllUser,jsonAllUser))
							{
								//theLog.Write("==============jsonAllUser=%s",jsonAllUser.toStyledString().c_str());
								int nErrCode = jsonAllUser["errcode"].asInt();
								if (nErrCode != 0)
								{
									bRet = FALSE;
									theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed,nErrCode=%d",nErrCode);
									break;
								}
								else
								{
									//Json::Value JsonUserListTmp;
									CString szUserListKey;
									szUserListKey.Format("dep%d_user_list",nDeptId);
									JsonUserList[szUserListKey] = jsonAllUser["userlist"];
									//添加用户list
									//JsonUserList.append(JsonUserListTmp);
									theLog.Write("######JsonUserList=%s",JsonUserList.toStyledString().c_str());
								}
							}
							else
							{
								bRet = FALSE;
								theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
								break;
							}
						}
						else
						{
							bRet = FALSE;
							theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
							break;
						
						}
						//theLog.Write("============dept=%s",dept.toStyledString().c_str());

					}
				}
				catch(...)
				{
					bRet = FALSE;
					theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
					break;

				}

			}

		}

	} while (FALSE);
	//theLog.Write("CDingDingTalking::GetDingTalkingEntList,JsonEntList=%s",JsonEntList.toStyledString().c_str());
	//theLog.Write("CDingDingTalking::GetDingTalkingEntList,JsonDeptList=%s",JsonDeptList.toStyledString().c_str());
	//theLog.Write("CDingDingTalking::GetDingTalkingEntList,JsonUserList=%s",JsonUserList.toStyledString().c_str());
	return bRet;
}
#if 0
BOOL CDingDingTalking::GetDingTalkingEntList(Json::Value& JsonEntList)
{
	BOOL bRet = TRUE;
	do 
	{
		if (m_szToken.IsEmpty())
		{
			bRet = FALSE;
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,fail");
			break;
		}
		CString szUrl;
		szUrl.Format("https://oapi.dingtalk.com/department/list?access_token=%s",m_szToken);
		CString szMsgBack;
		if (!GetHttpsByUrl(szUrl,szMsgBack))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,SendRequest,failed");
			bRet = FALSE;
			break;
		}
		Json::Value jsonall;
		if (!PharseJson(szMsgBack,jsonall))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,PharseJson,failed");
			bRet = FALSE;
			break;
		}
		else
		{
			int nErrCode = jsonall["errcode"].asInt();
			if (nErrCode != 0)
			{
				bRet = FALSE;
				theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
				break;
			}
			else
			{
				try
				{
					Json::Value& JsonDeptList = jsonall["department"];
					for (int index=0; index<JsonDeptList.size(); index++)
					{
						Json::Value& dept = JsonDeptList[index];
						int nDeptId = dept["id"].asInt();
						int nParentId = dept["parentid"].asInt();
						if (nDeptId == 1 && nParentId == 0)
						{
							JsonEntList.append(dept);
						}
						//theLog.Write("============dept=%s",dept.toStyledString().c_str());

					}
				}
				catch(...)
				{
					bRet = FALSE;
					theLog.Write("!!CDingDingTalking::GetDingTalkingEntList,2,failed");
					break;
					
				}

			}
			
		}
		
	} while (FALSE);
	theLog.Write("CDingDingTalking::GetDingTalkingEntList,JsonEntList=%s",JsonEntList.toStyledString().c_str());
	return bRet;
}
BOOL CDingDingTalking::GetDingTalkingDeptList(Json::Value& JsonDeptList)
{
	BOOL bRet = TRUE;
	do 
	{
		if (m_szToken.IsEmpty())
		{
			bRet = FALSE;
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,fail");
			break;
		}
		CString szUrl;
		szUrl.Format("https://oapi.dingtalk.com/department/list?access_token=%s",m_szToken);
		CString szMsgBack;
		if (!GetHttpsByUrl(szUrl,szMsgBack))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,SendRequest,failed");
			bRet = FALSE;
			break;
		}
		Json::Value jsonall;
		if (!PharseJson(szMsgBack,jsonall))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,PharseJson,failed");
			bRet = FALSE;
			break;
		}
		else
		{
		
			int nErrCode = jsonall["errcode"].asInt();
			if (nErrCode != 0)
			{
				bRet = FALSE;
				theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,2,failed");
				break;
			}
			else
			{
				try
				{
					Json::Value& JsonDeptListTmp = jsonall["department"];
					//theLog.Write("==========jsonall=%s",JsonDeptListTmp.toStyledString().c_str());
					for (int index=0; index<JsonDeptListTmp.size(); index++)
					{
						Json::Value& dept = JsonDeptListTmp[index];
						int nDeptId = dept["id"].asInt();
						int nParentId = dept["parentid"].asInt();
						if (nDeptId != 1 && nParentId != 0)
						{
							JsonDeptList.append(dept);
						}
						//theLog.Write("============dept=%s",dept.toStyledString().c_str());

					}
				}
				catch(...)
				{
					bRet = FALSE;
					theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,2,failed");
					break;

				}

			}

		}

	} while (FALSE);
	theLog.Write("CDingDingTalking::GetDingTalkingDeptList,JsonDeptList=%s",JsonDeptList.toStyledString().c_str());
	return bRet;
}
BOOL CDingDingTalking::GetDingTalkingUserList(Json::Value JsonDeptList,Json::Value& JsonUserList)
{
	BOOL bRet = TRUE;
	do 
	{
		if (m_szToken.IsEmpty())
		{
			bRet = FALSE;
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,fail");
			break;
		}
		CString szUrl;
		szUrl.Format("https://oapi.dingtalk.com/department/list?access_token=%s",m_szToken);
		CString szMsgBack;
		if (!GetHttpsByUrl(szUrl,szMsgBack))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,SendRequest,failed");
			bRet = FALSE;
			break;
		}
		Json::Value jsonall;
		if (!PharseJson(szMsgBack,jsonall))
		{
			theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,PharseJson,failed");
			bRet = FALSE;
			break;
		}
		else
		{

			int nErrCode = jsonall["errcode"].asInt();
			if (nErrCode != 0)
			{
				bRet = FALSE;
				theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,2,failed");
				break;
			}
			else
			{
				try
				{
					Json::Value& JsonDeptListTmp = jsonall["department"];
					//theLog.Write("==========jsonall=%s",JsonDeptListTmp.toStyledString().c_str());
					for (int index=0; index<JsonDeptListTmp.size(); index++)
					{
						Json::Value& dept = JsonDeptListTmp[index];
						int nDeptId = dept["id"].asInt();
						int nParentId = dept["parentid"].asInt();
						if (nDeptId != 1 && nParentId != 0)
						{
							JsonDeptList.append(dept);
						}
						//theLog.Write("============dept=%s",dept.toStyledString().c_str());

					}
				}
				catch(...)
				{
					bRet = FALSE;
					theLog.Write("!!CDingDingTalking::GetDingTalkingDeptList,2,failed");
					break;

				}

			}

		}

	} while (FALSE);
	theLog.Write("CDingDingTalking::GetDingTalkingDeptList,JsonDeptList=%s",JsonDeptList.toStyledString().c_str());
	return bRet;
}
#endif
BOOL CDingDingTalking::PharseJson(CString szJson,Json::Value &json)
{
	BOOL bRet = TRUE;
	//theLog.Write("CDingDingTalking::PharseJson,0,szJson=%s",szJson);
	try
	{
		CString szJson2 = CCommonFun::BslashUnicodeToMultiByte(szJson);
		szJson2.Replace("\\/", "/");

		Json::Reader reader;
		if (!reader.parse(szJson2.GetString(),json))
		{
			bRet = FALSE;
		}
	}
	catch (...)
	{
		theLog.Write("!!CDingDingTalking::PharseJson,failed,FILE=%s,FUC=%s,LINE=%s",__FILE__,__FUNCTION__,__LINE__);
		bRet = FALSE;
	}
	return bRet;
}
BOOL CDingDingTalking::GetHttpsByUrl(CString szUrl, CString &szMsgBack)
{
	CGetContenByUrl getHttps;
	if (getHttps.getHttpsContentByUrl(szUrl,szMsgBack))
	{
		return TRUE;
	}
	else
	{
		theLog.Write("!!CDingDingTalking::GetHttpsByUrl,failed");
		return FALSE;
	}
}
