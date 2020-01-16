// BanCommand.h
// 用于系统控制

class _AA_DLL_EXPORT_ BanCommand
{
public:
	BanCommand();
	~BanCommand();

public:
	static BOOL SetBanDIApplication(BOOL sign);//禁止修改删除/添加程序
	static BOOL SetBanTaskManager(BOOL sign);//禁止修改任务管理器
	static BOOL SetBanLinkProperty(BOOL sign);//禁止修改本地连接
	static BOOL SetBanComputername(BOOL sign);//禁止修改计算机名称
	static BOOL SetBanMenu(BOOL sign);//禁止修改计算机名称
	static BOOL DelStartItem(CString path);//删除启动项
	static BOOL SetBanRegister(BOOL sign);//禁止使用注册表
	static BOOL SetFileExtShow(BOOL bShow);// 显示文件后缀名
};