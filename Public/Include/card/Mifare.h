//int rBuffer[22];          	//接收缓冲区 整型
//int sBuffer[22];           //发送缓冲区 整型

#ifdef __cplusplus
extern "C" {
#endif

extern "C" _declspec(dllimport) int StrHexToByte(char* Value,int len,int* ByteArray);
extern "C" _declspec(dllimport) int StrHexToChar(char* Value,int len,char* CharArray);

extern "C" _declspec(dllimport) int OpenPort(int Port);     //打开串口
extern "C" _declspec(dllimport) int ClosePort(int Port);    //关闭串口
extern "C" _declspec(dllimport) int Call(int Port);         //点名
extern "C" _declspec(dllimport) int BeepEx(int Port);       //蜂鸣     

extern "C" _declspec(dllimport) int LoadAPassword(int Port,int Block,char* Psw);  //装载密码
extern "C" _declspec(dllimport) int ReadData(int Port,int Block,char* Data);      //读块数据
extern "C" _declspec(dllimport) int WriteData(int Port,int Block,char* Data);     //写块数据
extern "C" _declspec(dllimport) int GetCardNo(int Port,char* CardNo);             //取卡号
//对指定块写入定长字符串
extern "C" _declspec(dllimport) int WriteDataEx(int Port,int Block,char* Psw,char* pData,int len);
//对指定块写入定长字符串
extern "C" _declspec(dllimport) int ReadDataEx(int Port,int Block,char* Psw,char* pData,int len);
//修改密码
extern "C" _declspec(dllimport) int ChangePassword(int Port,int Sector,char* OldPsw,char* NewPsw);

#ifdef __cplusplus
}
#endif