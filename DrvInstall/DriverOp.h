// DriverOp.h: interface for the CDriverOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEROP_H__4CB0DDB8_F88C_4DE2_8EC4_906125D2CC73__INCLUDED_)
#define AFX_DRIVEROP_H__4CB0DDB8_F88C_4DE2_8EC4_906125D2CC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDriverOp  
{
public:

	void RepairWintMars();
	bool RepairMirrorReg();

    BOOL AddAccessRights(CHAR *lpszFileName, DWORD dwAccessMask);

	CDriverOp();
	virtual ~CDriverOp();
};

#endif // !defined(AFX_DRIVEROP_H__4CB0DDB8_F88C_4DE2_8EC4_906125D2CC73__INCLUDED_)
