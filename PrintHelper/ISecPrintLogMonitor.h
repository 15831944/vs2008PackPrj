#ifndef ISEC_PRINT_LOG_MONITOR
#define ISEC_PRINT_LOG_MONITOR

#include <string>
#include <list>
#include <vector>
#include "SnmpInterface.h"

/* 
    打印日志分析
*/
class IsecPrintLogMonitor
{
public:
	static IsecPrintLogMonitor* getInstance();
	static void release();
private:
	IsecPrintLogMonitor();
	IsecPrintLogMonitor(const IsecPrintLogMonitor&);
	IsecPrintLogMonitor& operator=(const IsecPrintLogMonitor&);
	~IsecPrintLogMonitor();
	static IsecPrintLogMonitor* _instance;

public:
	virtual bool init(std::string cmd);
	virtual void saveLogToPath(std::string path);
	virtual bool getLog();
    virtual void getPrintJobList(SCP_JobInfo_List& jobList);

	bool isInitOk() {
        return m_bInitOk;
    }

protected:
    bool m_bInitOk;
	std::string m_sCmd;         //UTF-8 codec
	std::string m_sSavePath;    //UTF-8 codec

};


#endif