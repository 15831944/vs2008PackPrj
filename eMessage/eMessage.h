// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� EMESSAGE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// EMESSAGE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef EMESSAGE_EXPORTS
#define EMESSAGE_API __declspec(dllexport)
#else
#define EMESSAGE_API __declspec(dllimport)
#endif

// �����Ǵ� eMessage.dll ������
class EMESSAGE_API CeMessage {
public:
	CeMessage(void);
	
	// TODO: �ڴ�������ķ�����
};

extern EMESSAGE_API int neMessage;

EMESSAGE_API int fneMessage(void);
