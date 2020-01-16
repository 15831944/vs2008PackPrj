//�ƴ�ӡ��������������Ϣ

#ifndef _CloudPrintNetDef_h_415C7ED2_E174_4AD1_9093_28119F8184A5_
#define _CloudPrintNetDef_h_415C7ED2_E174_4AD1_9093_28119F8184A5_

#include "CloudPrintConfig.h"

#define WebServiceDomain						TEXT("www.pantumcloudprint.com")		//�ƴ�ӡWebService��������IP
#define WebServicePort							(443)								//�ƴ�ӡWebService�Ķ˿�
#if Enable_Http_Interface
#define WebServiceURL							TEXT("https://www.pantumcloudprint.com/cloudprint/serviceHttp/serviceHttp.php")		//�ƴ�ӡHTTP�ӿڵ�����URL
#define WebServiceURLFormat						TEXT("https://%s:%d/cloudprint/serviceHttp/serviceHttp.php")						//�ƴ�ӡHTTP�ӿڵ�����URL�ĸ�ʽ��
#else
#define WebServiceURL							TEXT("https://www.pantumcloudprint.com/cloudprint/webservices/service.php")			//�ƴ�ӡWebService������URL
#define WebServiceURLFormat						TEXT("https://%s:%d/cloudprint/webservices/service.php")							//�ƴ�ӡWebService������URL�ĸ�ʽ��
#endif
#define WebServiceTimeout						(1000*15)							//�ƴ�ӡWebService�ĵ��ó�ʱʱ�䣬����
#define ServerDomain							TEXT("www.pantumcloudprint.com")	//�ƴ�ӡ���ķ�������������IP
#define ServerPort								(5040)								//�ƴ�ӡ���ķ������Ķ˿�

//�ͻ��������������������
#define ScanQRCodeURL							TEXT("https://www.pantumcloudprint.com/cloudprint/mobile/index.php")		//�ƴ�ӡ�ͻ���ɨ���¼�Ĳ�������URL
#define ScanQRCodeFormat						TEXT("https://%s:%d/cloudprint/mobile/index.php")		//�ƴ�ӡ�ͻ���ɨ���¼�Ĳ�������URL
#define ServerBDomain							TEXT("www.pantumcloudprint.com")	//�ƴ�ӡ���ķ�����B��������IP
#define ServerBPort								(5041)								//�ƴ�ӡ���ķ�����B�Ķ˿�

#define	UserRegisterURL							TEXT("https://www.pantumcloudprint.com/cloudprint/view/pc/user/userregister.php")//�ƴ�ӡ�û�ע��

#endif //_CloudPrintNetDef_h_415C7ED2_E174_4AD1_9093_28119F8184A5_