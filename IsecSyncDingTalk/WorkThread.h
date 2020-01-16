#pragma once
#include "DingDingTalking.h"


// CWorkThread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWorkThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	void SetHandleOfMsgQueOK(HANDLE hThMsgQueOK);
protected:
	void OnEndTh(WPARAM wp,LPARAM lp);
	void OnMsgQueOK(WPARAM wp,LPARAM lp);
	void KillThis();
	void OnStartSyncDingTalk(WPARAM wp,LPARAM lp);
protected:
	
protected:
	HANDLE m_hThMsgQueOK;


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


