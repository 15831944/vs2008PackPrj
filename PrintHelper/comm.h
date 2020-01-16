/**
    \file comm.h
    \brief 该文件主要用来存放项目中的全局变量和函数
    \author zhoumanman888@126.com
    \date 2019.01.14
*/


#ifndef COMM_H
#define COMM_H


#define MY_COMM_NAMESPACE_BEGIN namespace Xab {
#define MY_COMM_NAMESPACE_END }


MY_COMM_NAMESPACE_BEGIN

/**
    判断是否是标准的表节点, 对于非标准的表节点, 则孩子节点的实例为"表对象标识符加上.1.表的索引值"
    , 默认返回true

    SNMP中标量（叶子）对象和表对象的一些重要约定: 在标量对象中, 在一个对象类型和那个对象的一个实例之间不存在模糊性, 每一个标量对象类型只有一个对象实例. 
    为了和表对象的约定一致，并区别一个对象类型和一个对象实例，SNMP规定一个不成表的标量对象的实例标识由它的对象标识加上.0组成. 
    说具体一点就是：叶子对象的实例为该叶子对象标识符加上0, 而表对象的实例为表对象标识符加上.表的索引值
    ,例如一个叶子对象实例1.3.6.1.2.1.1.1.0 为 sysDescr 叶子对象的实例, 而一个表对象实例1.3.6.1.2.1.2.2.1.2.3为ifDescr表对象的第三个实例。

    \param sEntryOid 表的入口节点
    \param sChildOfTableOid 表的任何一个孩子实例
*/
bool isStdLeafNodeOfTable(CString sEntryOid, CString sChildOfTableOid);


MY_COMM_NAMESPACE_END

#endif