/**
    \file comm.h
    \brief ���ļ���Ҫ���������Ŀ�е�ȫ�ֱ����ͺ���
    \author zhoumanman888@126.com
    \date 2019.01.14
*/


#ifndef COMM_H
#define COMM_H


#define MY_COMM_NAMESPACE_BEGIN namespace Xab {
#define MY_COMM_NAMESPACE_END }


MY_COMM_NAMESPACE_BEGIN

/**
    �ж��Ƿ��Ǳ�׼�ı�ڵ�, ���ڷǱ�׼�ı�ڵ�, ���ӽڵ��ʵ��Ϊ"������ʶ������.1.�������ֵ"
    , Ĭ�Ϸ���true

    SNMP�б�����Ҷ�ӣ�����ͱ�����һЩ��ҪԼ��: �ڱ���������, ��һ���������ͺ��Ǹ������һ��ʵ��֮�䲻����ģ����, ÿһ��������������ֻ��һ������ʵ��. 
    Ϊ�˺ͱ�����Լ��һ�£�������һ���������ͺ�һ������ʵ����SNMP�涨һ�����ɱ�ı��������ʵ����ʶ�����Ķ����ʶ����.0���. 
    ˵����һ����ǣ�Ҷ�Ӷ����ʵ��Ϊ��Ҷ�Ӷ����ʶ������0, ��������ʵ��Ϊ������ʶ������.�������ֵ
    ,����һ��Ҷ�Ӷ���ʵ��1.3.6.1.2.1.1.1.0 Ϊ sysDescr Ҷ�Ӷ����ʵ��, ��һ�������ʵ��1.3.6.1.2.1.2.2.1.2.3ΪifDescr�����ĵ�����ʵ����

    \param sEntryOid �����ڽڵ�
    \param sChildOfTableOid ����κ�һ������ʵ��
*/
bool isStdLeafNodeOfTable(CString sEntryOid, CString sChildOfTableOid);


MY_COMM_NAMESPACE_END

#endif