#include "StdAfx.h"
#include "CardAnalyst.h"

CCardAnalyst::CCardAnalyst(void)
{
}

CCardAnalyst::~CCardAnalyst(void)
{
}

CString CCardAnalyst::HandleCardNo( CString& szParam )
{
	CString szFun = "";
	CString szNewParam = "";
	if(Split(szParam, szFun, szNewParam))
	{
		if(szFun.CompareNoCase("DecToHex") == 0)
		{
			return DecToHex(szNewParam);
		}
		else if(szFun.CompareNoCase("HexToDec") == 0)
		{
			return HexToDec(szNewParam);
		}
		else if(szFun.CompareNoCase("DecRevsToHex") == 0)
		{
			return DecRevsToHex(szNewParam);
		}
		else if(szFun.CompareNoCase("DecRevsToDec") == 0)
		{
			return DecRevsToDec(szNewParam);
		}
		else if(szFun.CompareNoCase("HexRevsToDec") == 0)
		{
			return HexRevsToDec(szNewParam);
		}
		else if(szFun.CompareNoCase("HexRevsToHex") == 0)
		{
			return HexRevsToHex(szNewParam);
		}
		else if(szFun.CompareNoCase("Intercept") == 0)
		{
			return Intercept(szNewParam);
		}
		else if(szFun.CompareNoCase("InterceptLeft") == 0)
		{
			return InterceptLeft(szNewParam);
		}
		else if(szFun.CompareNoCase("InterceptRight") == 0)
		{
			return InterceptRight(szNewParam);
		}
		else if(szFun.CompareNoCase("Wiegand34To26") == 0)
		{
			return Wiegand34To26(szNewParam);
		}
		else if(szFun.CompareNoCase("AddToLeft") == 0)
		{
			return AddToLeft(szNewParam);
		}
		else if(szFun.CompareNoCase("AddToRight") == 0)
		{
			return AddToRight(szNewParam);
		}
		else if(szFun.CompareNoCase("Concat") == 0)
		{
			return Concat(szNewParam);
		}
	}
	else
	{
		//printf("!!CAnalyst::Split,failed ,szParam=%s\n", szParam);
	}
	return szParam;
}

BOOL CCardAnalyst::Split( CString& source, CString& funout, CString& paramout )
{
	int nFirstIndex = source.Find('(');
	int nLastIndex = source.ReverseFind(')');
	if (nFirstIndex > 0 && nLastIndex > 0)
	{
		funout = source.Mid(0, nFirstIndex);
		paramout = source.Mid(nFirstIndex+1, nLastIndex-nFirstIndex-1);
		//printf("CAnalyst::Split,funout=%s,paramout=%s\n",funout,paramout);
		return TRUE;
	}
	return FALSE;
}

CString CCardAnalyst::DecToHex( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	//ת����16����
	unsigned long nCardNo = _atoi64(szCardNo);
	szCardNo.Format("%08x", nCardNo);
	szCardNo.MakeUpper();

	//printf("CAnalyst::DecToHex,szCardNo=%s\n", szCardNo);
	return szCardNo;
}

CString CCardAnalyst::HexToDec( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	szCardNo.MakeUpper();
	int nLength = szCardNo.GetLength();
	unsigned __int64 value = 0;
	char c = '\0';
	unsigned __int64 temp = 0;
	for (int i=0; i<nLength; i++)
	{
		c = szCardNo.GetAt(i);
		if (c >= 'A')
		{
			temp = 10 + c - 'A';
		}
		else
		{
			temp = c - '0';
		}
		value = value * 16 + temp;
	}
	char chNum[21] = {0};
	sprintf_s(chNum, "%I64u", value);
	CString szOut(chNum);
	//printf("CAnalyst::HexToDec,szOut=%s\n", szOut);
	return szOut;
}

//10�����ַ�����ת16�����ַ���
CString CCardAnalyst::DecRevsToHex( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	unsigned long nCardNo = _atoi64(szCardNo);
	nCardNo = ((nCardNo&0xFF000000)>>24) | ((nCardNo&0x00FF0000)>>8) | ((nCardNo&0x0000FF00)<<8) | ((nCardNo&0x000000FF)<<24);//��ת
	szCardNo.Format("%08x", nCardNo);
	szCardNo.MakeUpper();
	//printf("CAnalyst::DecRevsToHex,szCardNo=%s\n", szCardNo);
	return szCardNo;
}
//10�����ַ�����ת10�����ַ���
CString CCardAnalyst::DecRevsToDec( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	unsigned long nCardNo = _atoi64(szCardNo);
	nCardNo = ((nCardNo&0xFF000000)>>24) | ((nCardNo&0x00FF0000)>>8) | ((nCardNo&0x0000FF00)<<8) | ((nCardNo&0x000000FF)<<24);//��ת
	szCardNo.Format("%08x", nCardNo);
	szCardNo.MakeUpper();
	szCardNo = HexToDec(szCardNo);
	//printf("CAnalyst::DecRevsToDec,szCardNo=%s\n", szCardNo);
	return szCardNo;
}
//16�����ַ�����ת10�����ַ���
CString CCardAnalyst::HexRevsToDec( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	szCardNo = DecRevsToDec(HexToDec(szCardNo));
	//printf("CAnalyst::HexRevsToDec,szCardNo=%s\n", szCardNo);
	return szCardNo;
}
//16�����ַ�����ת16�����ַ���
CString CCardAnalyst::HexRevsToHex( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	CString lleft = szCardNo;
	if(lleft.GetLength() / 2 == 1)
	{//ǰ�油0
		lleft = "0" + lleft;
	}
	CString szNew = "";
	while(lleft.GetLength() >= 2)
	{
		CString right2 = lleft.Right(2);
		lleft = lleft.Left(lleft.GetLength() - 2);
		szNew.Append(right2);
	}
	//printf("CAnalyst::HexRevsToHex,szNew=%s\n", szNew);
	return szNew;
}
CString CCardAnalyst::Intercept( CString& szParam )
{
	//printf("CCardAnalyst::Intercept,1,szParam=%s\n", szParam);
	CString szCardNo = szParam;
	int index = szCardNo.ReverseFind(',');
	if(index > 0)
	{
		CString card = szCardNo.Left(index);
		int nLen = atoi(szCardNo.Right(szCardNo.GetLength()-index-1));

		if(card.Find('(') > 0)
		{
			card = HandleCardNo(card);
		}
		szCardNo = card.Right(nLen);
		//printf("CCardAnalyst::Intercept,2,szCardNo=%s\n", szCardNo);
		if(szCardNo.GetLength() < nLen)
		{
			CString szFormat;
			szFormat.Format("%%0%dd%%s",nLen-szCardNo.GetLength());
			CString tmp = szCardNo;
			szCardNo.Format(szFormat, 0, tmp);
		}
		//printf("CCardAnalyst::Intercept,3,szCardNo=%s\n", szCardNo);
	}
	//printf("CAnalyst::Intercept,szCardNo=%s\n", szCardNo);
	return szCardNo;
}

CString CCardAnalyst::Wiegand34To26( CString& szParam )
{
	CString szCardNo = szParam;
	if(szParam.Find('(') > 0)
	{
		szCardNo = HandleCardNo(szParam);
	}
	unsigned long lCardInt = strtoul(szCardNo,NULL,10);
	unsigned long lNewCard	= ((lCardInt % (256 * 256)) + (lCardInt / (256 * 256) % 256) * 100000);
	szCardNo.Format("%08u", lNewCard);
	return szCardNo;
}

BOOL CCardAnalyst::CheckValidate( CString& szParam )
{
	CString szFun = "";
	CString szNewParam = szParam;
	int leftkuohao = 0;
	int rightkuohao = 0;
	char pBuf[200] = {0};
	strcpy(pBuf, szParam.GetString());
	int nIndex = 0;
	//�ж���˳���Ƿ���ȷ
	while(nIndex < 200)
	{
		if(*(pBuf+nIndex) == '(')
		{
			leftkuohao += 1;
		}
		else if(*(pBuf+nIndex) == ')')
		{
			rightkuohao += 1;
		}
		nIndex++;
		if(leftkuohao < rightkuohao)
		{
			//printf("����˳�򲻶ԣ�������%d����������%d��\n", leftkuohao, rightkuohao);
			return FALSE;
		}
	}
	//�ж����Ÿ����Ƿ�ƥ��
	if(leftkuohao != rightkuohao || leftkuohao < 1)
	{
		//printf("���Ÿ������ԣ�������%d����������%d��\n", leftkuohao, rightkuohao);
		return FALSE;
	}
	//�жϺ������Լ���������Ƿ���ȷ
	while(TRUE)
	{
		if(szNewParam.Find('(') > 0)
		{
			CString szNewParam2;
			if(!Split(szNewParam, szFun, szNewParam2))
			{
				//printf("�����������szNewParam=%s\n", szNewParam);
				return FALSE;
			}
			if(szFun.CompareNoCase("DecToHex") != 0
				&& szFun.CompareNoCase("HexToDec") != 0
				&& szFun.CompareNoCase("DecRevsToHex") != 0
				&& szFun.CompareNoCase("DecRevsToDec") != 0
				&& szFun.CompareNoCase("HexRevsToDec") != 0
				&& szFun.CompareNoCase("HexRevsToHex") != 0
				&& szFun.CompareNoCase("Intercept") != 0
				&& szFun.CompareNoCase("Wiegand34To26") != 0)
			{
				//printf("δ֪�ķ����� %s\n", szFun);
				return FALSE;
			}
			if(szFun.CompareNoCase("Intercept") == 0)
			{
				int index = szNewParam2.Find(',');
				if(index > 0 && index < szNewParam2.GetLength())
				{
					szNewParam = szNewParam2.Left(index);
				}
				else
				{
					//printf("Intercept�����Ĳ�����������ȷ��szNewParam=%s\n", szNewParam);
					return FALSE;
				}
			}
			szNewParam = szNewParam2;
		}
		{
			break;
		}
	}
	return TRUE;
}

CString CCardAnalyst::AddToLeft( CString& szParam )
{
	//printf("CCardAnalyst::AddToLeft,1,szParam=%s\n", szParam);
	CString szCardNo = szParam;
	int index = szCardNo.ReverseFind(',');
	if(index > 0)
	{
		CString card = szCardNo.Left(index);
		CString part2 = szCardNo.Right(szCardNo.GetLength()-index-1);
		part2.Replace("\"","");

		if(card.Find('(') > 0)
		{
			card = HandleCardNo(card);
		}
		szCardNo.Format("%s%s", part2, card);
		//printf("CCardAnalyst::AddToLeft,2,szCardNo=%s\n", szCardNo);
	}
	//printf("CAnalyst::Intercept,szCardNo=%s\n", szCardNo);
	return szCardNo;
}

CString CCardAnalyst::AddToRight( CString& szParam )
{

	//printf("CCardAnalyst::AddToRight,1,szParam=%s\n", szParam);
	CString szCardNo = szParam;
	int index = szCardNo.ReverseFind(',');
	if(index > 0)
	{
		CString card = szCardNo.Left(index);
		CString part2 = szCardNo.Right(szCardNo.GetLength()-index-1);
		part2.Replace("\"","");

		if(card.Find('(') > 0)
		{
			card = HandleCardNo(card);
		}
		szCardNo.Format("%s%s", card, part2);
		//printf("CCardAnalyst::AddToRight,2,szCardNo=%s\n", szCardNo);
	}
	//printf("CAnalyst::Intercept,szCardNo=%s\n", szCardNo);
	return szCardNo;
}

CString CCardAnalyst::Concat( CString& szParam )
{
	//�㷨�����⣬�ݲ��ᳫʹ��
	CString szCardNo = szParam;
	char* cParam = szCardNo.GetBuffer();
	int nLeft = 0;
	int nRight = 0;
	int nIndex = 0;
	for(int i = 0; i < strlen(cParam); i++)
	{
		if(*(cParam+i) == '(')
		{
			nLeft++;
		}
		else if(*(cParam+i) == ')')
		{
			nRight++;
		}
		else if(*(cParam+i) == ',' && nLeft==nRight)
		{
			nIndex = i;
			break;
		}
	}
	szCardNo.ReleaseBuffer();
	//printf("CAnalyst::Concat,nIndex=%d\n", nIndex);
	if(nIndex > 0)
	{
		CString str1 = szCardNo.Left(nIndex);
		CString str2 = szCardNo.Right(szCardNo.GetLength()-nIndex-1);
		if(str1.Find('(') > 0)
		{
			str1 = HandleCardNo(str1);
		}
		else
		{
			str1.Replace("\"","");
		}
		if(str2.Find('(') > 0)
		{
			str2 = HandleCardNo(str2);
		}
		else
		{
			str2.Replace("\"","");
		}
		szCardNo.Format("%s%s", str1, str2);
	}
	return szCardNo;
}

//��ȡ���λ��������������ಹ��
CString CCardAnalyst::InterceptLeft( CString& szParam )
{
	//printf("CCardAnalyst::Intercept,1,szParam=%s\n", szParam);
	CString szCardNo = szParam;
	int index = szCardNo.ReverseFind(',');
	if(index > 0)
	{
		CString card = szCardNo.Left(index);
		int nLen = atoi(szCardNo.Right(szCardNo.GetLength()-index-1));

		if(card.Find('(') > 0)
		{
			card = HandleCardNo(card);
		}
		szCardNo = card.Left(nLen);
		//printf("CCardAnalyst::Intercept,2,szCardNo=%s\n", szCardNo);
		if(szCardNo.GetLength() < nLen)
		{
			CString szFormat;
			szFormat.Format("%%0%dd%%s",nLen-szCardNo.GetLength());
			CString tmp = szCardNo;
			szCardNo.Format(szFormat, 0, tmp);
		}
		//printf("CCardAnalyst::Intercept,3,szCardNo=%s\n", szCardNo);
	}
	printf("CAnalyst::Intercept,szCardNo=%s\n", szCardNo);
	return szCardNo;
}

//��ȡ�ұ�λ��������������ಹ��
CString CCardAnalyst::InterceptRight( CString& szParam )
{
	//printf("CCardAnalyst::Intercept,1,szParam=%s\n", szParam);
	CString szCardNo = szParam;
	int index = szCardNo.ReverseFind(',');
	if(index > 0)
	{
		CString card = szCardNo.Left(index);
		int nLen = atoi(szCardNo.Right(szCardNo.GetLength()-index-1));

		if(card.Find('(') > 0)
		{
			card = HandleCardNo(card);
		}
		szCardNo = card.Right(nLen);
		//printf("CCardAnalyst::Intercept,2,szCardNo=%s\n", szCardNo);
		if(szCardNo.GetLength() < nLen)
		{
			CString szFormat;
			szFormat.Format("%%0%dd%%s",nLen-szCardNo.GetLength());
			CString tmp = szCardNo;
			szCardNo.Format(szFormat, 0, tmp);
		}
		//printf("CCardAnalyst::Intercept,3,szCardNo=%s\n", szCardNo);
	}
	printf("CAnalyst::Intercept,szCardNo=%s\n", szCardNo);
	return szCardNo;
}