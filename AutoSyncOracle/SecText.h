// SecText.h: interface for the CSecText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTEXT_H__B0236F01_594E_11D5_B66E_0050BAF46898__INCLUDED_)
#define AFX_SECTEXT_H__B0236F01_594E_11D5_B66E_0050BAF46898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define _WIN32_WINNT 0x0400
#define SECRETKEY         "ISECSTAR~!@#$%^&*()"
#define FILEMODESECUREKY  ")(*&^%$#@!'"
class CSecText  
{
public:
	bool IsNumSecType(CString str);
	CString unsecretstr(CString str);
	CString secretstr(CString str);
	CString GetDefaultKey();
	CString DecryptGroupName(CString GroupName);
	CString EncryptGroupName(CString GroupName);
	void SetKey(LPCTSTR csKey);
	void GetKey(CString& csKey);
	void TransformChar(unsigned char& cTarget);
	void TransformString(LPCTSTR csKey, CString& csTarget);
	CString  unsecretstr2(CString str);
	CString  secretstr2(CString str);
	CSecText();
	virtual ~CSecText();
protected:
    
	CString m_csKey;

    unsigned long           m_LFSR_A;
    unsigned long           m_LFSR_B;
    unsigned long           m_LFSR_C;
    const unsigned long     m_Mask_A;
    const unsigned long     m_Mask_B;
    const unsigned long     m_Mask_C;
    const unsigned long     m_Rot0_A;
    const unsigned long     m_Rot0_B;
    const unsigned long     m_Rot0_C;
    const unsigned long     m_Rot1_A;
    const unsigned long     m_Rot1_B;
    const unsigned long     m_Rot1_C;
};

#endif // !defined(AFX_SECTEXT_H__B0236F01_594E_11D5_B66E_0050BAF46898__INCLUDED_)
