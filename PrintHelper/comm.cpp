#include "stdafx.h"
#include "comm.h"
#include <math.h>


MY_COMM_NAMESPACE_BEGIN

bool isStdLeafNodeOfTable(CString sEntryOid, CString sChildOfTableOid)
{
    /*
        以下代码以长度判断, 可以以 ifEntry 来分析
        ifEntry,1.3.6.1.2.1.2.2.1
        ifIndex,1.3.6.1.2.1.2.2.1.1

    */
    sEntryOid = sEntryOid.Trim();
    sChildOfTableOid = sChildOfTableOid.Trim();

    const int nLen1 = sEntryOid.GetLength();
    const int nLen2 = sChildOfTableOid.GetLength();

    if (0 == nLen1 
        || 0 == nLen2
        || nLen1 > nLen2
        || nLen2 - nLen1 <= 2
        )
        return true;

    return false;
};
MY_COMM_NAMESPACE_END

