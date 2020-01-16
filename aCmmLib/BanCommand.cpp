// BanSomeApplication.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "BanCommand.h"
#include "shlwapi.h"
#include "SecurityTool.h"
#pragma comment(lib, "shlwapi.lib")
BanCommand::BanCommand()
{

}

BanCommand::~BanCommand()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//取消添加/删除程序
BOOL BanCommand::SetBanDIApplication(BOOL sign)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("NoAddRemovePrograms",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall NoAddRemovePrograms failed!");
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg;
	return bRet;
}

BOOL BanCommand::SetBanTaskManager(BOOL sign)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("DisableTaskmgr",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System DisableTaskmgr failed!");
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg;
	return bRet;
}

BOOL BanCommand::SetBanLinkProperty(BOOL sign)
{
	ASSERT(0);
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Policies\\Microsoft\\Windows\\Network Connections";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Policies\\Microsoft\\Windows\\Network Connections",sSID);
			theLog.Write("前用户是系统sKeyPath = %s",sKeyPath);
		}
		else
		{
			nID = 2;
			pReg = new CRegistry2(0,0);
		}
			
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Policies\\Microsoft\\Windows\\Network Connections failed!");
				break;
			}
		}
		// 将内容写入到注册表
		if (!pReg->Write("NC_EnableAdminProhibits",Num))
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Policies\\Microsoft\\Windows\\Network Connections NC_EnableAdminProhibits failed!");
			bRet = 0;
			break;
		}
		if (!pReg->Write("NC_LanProperties",0))
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Policies\\Microsoft\\Windows\\Network Connections NC_LanProperties failed");
			bRet = 0;
			break;
		}
		if (!pReg->Write("NC_LanConnect",0))
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Policies\\Microsoft\\Windows\\Network Connections NC_LanProperties failed");
			bRet = 0;
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg; 
	return bRet;
}

BOOL BanCommand::SetBanComputername(BOOL sign)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("NoPropertiesMyComputer",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer NoPropertiesMyComputer failed!");
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg;
	SendMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,0);  
	return bRet;
}

BOOL BanCommand::SetBanMenu(BOOL sign)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("NoSetTaskBar",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer NoSetTaskBar failed!");
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg;
	SendMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,0);  
	return bRet;
}

BOOL BanCommand::DelStartItem(CString path)
{
	ASSERT(0);
	BOOL bRet = TRUE;
	// 修改文件的的属性
	CFileStatus status;
	if (CFile::GetStatus(path,status))
	{
		status.m_attribute = 0x00;
		CFile::SetStatus(path,status);
		// 删除文件 
		DeleteFile(path);
	}
	else
	{
		CRegistry2* pReg = NULL;
		CString sWinUser;
		CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		CString sKeyAllPath =  path;
		int length = path.GetLength();
		int lastindex = path.ReverseFind('\\');
		if (-1 == lastindex)
			return FALSE;
		CString name = path.Right(length-lastindex-1);
		do 
		{
			// 如果是删除的公共配置
			if (sKeyAllPath.Find("HKEY_LOCAL_MACHINE") != -1)
			{
				pReg = new CRegistry2(2,0);
				if (!pReg)
				{
					theLog.Write("new CRegistry2 failed");
					bRet = 0;
					break;
				}
			}
			else if (sKeyAllPath.Find("HKEY_CURRENT_USER") != -1)
			{
				// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
				CSecurityTool::GetCurrProcessUser(sWinUser);
				if(sWinUser.CompareNoCase("system") == 0)
				{
					CString sSID;
					CSecurityTool::GetCurUserSID(sSID);
					if (sSID.IsEmpty())
					{
						theLog.Write("GetCurUserSID failed");
						bRet = 0;
						break;
					}
					pReg = new CRegistry2(3,0);
					if (!pReg)
					{
						theLog.Write("new CRegistry2 failed");
						bRet = 0;
						break;
					}
					sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",sSID);
				}
				else
				{
					pReg = new CRegistry2(0,0);
					// 如果创建失败，直接返回
					if (!pReg)
					{
						theLog.Write("new CRegistry2 failed");
						bRet = 0;
						break;
					}
				}
			}
			if (pReg)
			{
				bRet = pReg->Open(sKeyPath);
				if (!bRet)
				{
					sKeyAllPath.Format("RegOpenKeyEx %s failed",path);
					theLog.Write(sKeyAllPath);
					bRet = 0;
					break;
				}

				bRet = pReg->Delete(name);
				if (!bRet)
				{
					sKeyAllPath.Format("RegDeleteValue %s failed",path);
					theLog.Write(sKeyAllPath);
					bRet = 0;
					break;
				}
			}
		} while (0);

		// 最后释放资源
		if (pReg)
			delete pReg;
	}
	return bRet;
}
//禁止使用注册表
BOOL BanCommand::SetBanRegister(BOOL sign)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 1;
	if (!sign)
		Num = 0;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\System\\	 failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("DisableRegistryTools",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System DisableRegistryTools failed!");
			break;
		}
	} while (0);
	// 最后释放资源
	if (pReg)
		delete pReg; 
	return bRet;
}

// 显示文件后缀名
BOOL BanCommand::SetFileExtShow(BOOL bShow)
{
	BOOL bRet = TRUE;
	HKEY hKey = NULL;
	DWORD dwCret = 0;
	DWORD Num = 0;
	if (!bShow)
		Num = 1;
	CRegistry2* pReg = NULL;
	CString sKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced";
	CString sWinUser;
	int nID = 0;
	do 
	{
		// 如果当前用户是系统管理员,则将这些配置信息写到当前登录的用户下
		CSecurityTool::GetCurrProcessUser(sWinUser);
		if(sWinUser.CompareNoCase("system") == 0)
		{
			CString sSID;
			CSecurityTool::GetCurUserSID(sSID);
			if (sSID.IsEmpty())
			{
				bRet = 0;
				break;
			}
			pReg = new CRegistry2(3,0);
			nID = 3;
			sKeyPath.Format("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",sSID);
		}
		else
			pReg = new CRegistry2(0,0);
		// 如果创建失败，直接返回
		if (!pReg)
		{
			theLog.Write("new CRegistry2 failed");
			bRet = 0;
			break;
		}
		// 打开注册表操作
		bRet = pReg->Open(sKeyPath);
		if (!bRet)
		{
			// 如果打开注册表项失败，则创建之
			bRet = pReg->CreateKey(sKeyPath,nID);
			if (!bRet)
			{
				theLog.Write("CreateKey HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced	 failed!");
				break;
			}
		}
		// 将内容写入到注册表
		bRet = pReg->Write("HideFileExt",Num);
		if (!bRet)
		{
			theLog.Write("Write HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced DisableRegistryTools failed!");
			break;
		}
	} while (0);

	// 最后释放资源
	if (pReg)
		delete pReg; 
	return bRet;
}