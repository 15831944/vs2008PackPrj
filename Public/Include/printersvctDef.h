#ifndef __GL_PRINTER_SVC____
#define __GL_PRINTER_SVC____


/************************************************************************/
/* 项目各位成员注意：add by zhandongbao 2008.12.17

1.定义一个结构体的某字段长度不定时，使用宏控制一下，如学生名称，
	#define NAME_SIZE 50
	char szName[STU_NAME_SIZE];
2.定义一个结构体要有描述，包括结构体描述和结构体的各项字段描述
2.网络上使用的结构体要注意对齐方式
3.本地使用的结构体，最好使用默认对齐方式
                                                                     */
/************************************************************************/



// #define WM_PRINT_MGR_REQ_JOB	1026	//打印管理终端请求打印列表
// #define WM_PRINT_MGR_CHECK_USER	1028	//打印管理终端验证用户
// #define WM_PRINT_MGR_REQ_JOB_FILE 1029	//打印管理终端请求打印列表
#pragma pack(push, 1)

//未添加打印站点
class CWebSubmitJob
{
public:
	int nAccount;				//用户名
	bool bDuplx;
	bool bColor;
	int  nPageType;
	char szPath[MAX_PATH + 1];	//路径
	char szSnapPath[MAX_PATH + 1];
	char szDocName[MAX_PATH + 1];
};

#pragma pack(pop)

#define WM_WEB_SUBMIT_JOB					WM_USER + 1032	//
#define WM_XPS_TOOL_SUBMIT_JOB				WM_USER + 1033	//


#endif