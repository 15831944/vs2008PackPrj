// ListCtrl2.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrl2.h"
#include "aCmmLib.h"
#include "FileLog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef UNICODE
#define stoi   atoi
#else
#define stoi   _wtoi
#endif
// CListCtrl2

IMPLEMENT_DYNAMIC(CListCtrl2, CListCtrl)

CListCtrl2::CListCtrl2()
{
	m_fAsc=TRUE;
	m_edit.m_hWnd = NULL;
	m_hLinkCursor = LoadCursor(NULL, IDC_HAND);
	m_bReadOnly = FALSE;
	m_bSort = TRUE;
	m_bUseCheckBox = FALSE;
	m_sString = "";
	m_bToolTips = FALSE;
	m_nMainItem = -1;
	m_nSubItem = -1;
	m_bUseColorBelt = TRUE;
	//	r226,g237,b244

	m_crBelt1 = RGB(226, 237, 244);
	m_crBelt2 = RGB(255, 255, 255);
	m_bGridDrag2MultiSel = false;
	m_nStartIndex = -1;

	//载入向上和向下两个图标资源
	HINSTANCE hIns = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	m_bmpArrowUp.LoadBitmap(IDB_BITMAP_ASC);
	m_bmpArrowDown.LoadBitmap(IDB_BITMAP_DESC);
	AfxSetResourceHandle(hIns);
	m_bEnableColumnPicker = FALSE;
	m_bEnableCategoryGroups = FALSE;
	m_nCurrentGroupColumn = -1;
}

CListCtrl2::~CListCtrl2()
{
	// 	for (POSITION pos = m_LinkStaticList.GetHeadPosition(); pos != NULL;)
	// 	{
	// 		CLinkStatic* pStatic = m_LinkStaticList.GetNext(pos);
	// 		delete pStatic;
	// 		pStatic = NULL;
	// 	}
}

BEGIN_MESSAGE_MAP(CListCtrl2, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PARENTNOTIFY()
	ON_MESSAGE(WM_P_USER_EDIT_END,OnEditEnd)
	ON_MESSAGE(LVM_DELETECOLUMN, OnDeleteColumn)
	ON_MESSAGE(LVM_INSERTCOLUMN, OnInsertColumn)
	ON_MESSAGE(LVM_SETCOLUMNWIDTH, OnSetColumnWidth)
	ON_NOTIFY_EX(HDN_BEGINTRACKA, 0, OnHeaderBeginResize)
	ON_NOTIFY_EX(HDN_BEGINTRACKW, 0, OnHeaderBeginResize)
	//ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKA, 0, OnHeaderDividerDblClick)
	ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKW, 0, OnHeaderDividerDblClick)
	ON_WM_CONTEXTMENU()	// OnContextMenu
	ON_WM_KEYDOWN()		// OnKeyDown
	ON_WM_LBUTTONUP()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()



// CListCtrl2 消息处理程序


bool IsNumber2( LPCTSTR pszText )
{
	//ASSERT_VALID_STRING( pszText );
	int nLen = lstrlen( pszText );
	if(0 >= nLen)
	{//肯定不是数字
		return false;
	}

	for( int i = 0; i < nLen; i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;

	return true;
}


int NumberCompare2( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	//ASSERT_VALID_STRING( pszNumber1 );
	//ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = stoi( pszNumber1 );
	const int iNumber2 = stoi( pszNumber2 );

	if( iNumber1 < iNumber2 )
		return -1;

	if( iNumber1 > iNumber2 )
		return 1;

	return 0;
}


bool IsDate2( LPCTSTR pszText )
{
	//ASSERT_VALID_STRING( pszText );

	// format should be 99/99/9999.

	if( lstrlen( pszText ) != 10 )
		return false;

	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& _istdigit( pszText[ 2 ] )
		&& _istdigit( pszText[ 3 ] )
		&& pszText[ 4 ] ==_T('/')
		&& _istdigit( pszText[ 5 ] )
		&& _istdigit( pszText[ 6 ] )
		&& pszText[ 7 ] ==_T('/')
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}


int DateCompare2( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = stoi( strDate1.Mid( 0, 4 ) );
	const int iYear2 = stoi( strDate2.Mid( 0, 4 ) );

	if( iYear1 < iYear2 )
		return -1;

	if( iYear1 > iYear2 )
		return 1;

	const int iMonth1 = stoi( strDate1.Mid( 5, 2 ) );
	const int iMonth2 = stoi( strDate2.Mid( 5, 2 ) );

	if( iMonth1 < iMonth2 )
		return -1;

	if( iMonth1 > iMonth2 )
		return 1;

	const int iDay1 = stoi( strDate1.Mid( 8, 2 ) );
	const int iDay2 = stoi( strDate2.Mid( 8, 2 ) );

	if( iDay1 < iDay2 )
		return -1;

	if( iDay1 > iDay2 )
		return 1;

	return 0;
}
//*******************************************************************
//
// 排序函数的实现
//
//*******************************************************************
int CALLBACK ListCompare2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl2* pV=(CListCtrl2*)lParamSort; 
	CString strItem1, strItem2; 
	LVFINDINFO info; 
	int nIndex; 
	info.flags = LVFI_PARAM;                       //设置为LVFI_PARAM，后面的FindItem才能有效，详情参考MSDN的LVFINDINFO 
	info.lParam = lParam1;                         //由item data(lParam1)得到对应的item text 
	if ( (nIndex=pV->FindItem(&info)) != -1) 
		strItem1 = pV->GetItemText(nIndex, pV->m_nSortedCol); 
	info.lParam = lParam2;                         //由item data(lParam2)得到对应的item text 
	if ( (nIndex=pV->FindItem(&info)) != -1)
		strItem2 = pV->GetItemText(nIndex, pV->m_nSortedCol); 
	if( IsNumber2( strItem1 ) )
		return pV->m_fAsc ? NumberCompare2( strItem1, strItem2 ) : NumberCompare2( strItem2, strItem1 );
	else if( IsDate2( strItem1 ) )
		return pV->m_fAsc ? DateCompare2( strItem1, strItem2 ) : DateCompare2( strItem2, strItem1 );
	else
		// text.
		return pV->m_fAsc ? lstrcmp( strItem1, strItem2 ) : lstrcmp( strItem2, strItem1 );
}

bool IsNumber( LPCTSTR pszText )
{
	//ASSERT_VALID_STRING( pszText );
	int nLen = lstrlen( pszText );
	if(0 >= nLen)
	{
		return false;
	}

	for( int i = 0; i < nLen; i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;

	return true;
}


int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	//ASSERT_VALID_STRING( pszNumber1 );
	//ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = stoi( pszNumber1 );
	const int iNumber2 = stoi( pszNumber2 );

	if( iNumber1 < iNumber2 )
		return -1;

	if( iNumber1 > iNumber2 )
		return 1;

	return 0;
}


bool IsDate( LPCTSTR pszText )
{
	//ASSERT_VALID_STRING( pszText );

	// format should be 99/99/9999.

	if( lstrlen( pszText ) != 10 )
		return false;

	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& _istdigit( pszText[ 2 ] )
		&& _istdigit( pszText[ 3 ] )
		&& pszText[ 4 ] ==_T('/')
		&& _istdigit( pszText[ 5 ] )
		&& _istdigit( pszText[ 6 ] )
		&& pszText[ 7 ] ==_T('/')
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}


int DateCompare( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = stoi( strDate1.Mid( 0, 4 ) );
	const int iYear2 = stoi( strDate2.Mid( 0, 4 ) );

	if( iYear1 < iYear2 )
		return -1;

	if( iYear1 > iYear2 )
		return 1;

	const int iMonth1 = stoi( strDate1.Mid( 5, 2 ) );
	const int iMonth2 = stoi( strDate2.Mid( 5, 2 ) );

	if( iMonth1 < iMonth2 )
		return -1;

	if( iMonth1 > iMonth2 )
		return 1;

	const int iDay1 = stoi( strDate1.Mid( 8, 2 ) );
	const int iDay2 = stoi( strDate2.Mid( 8, 2 ) );

	if( iDay1 < iDay2 )
		return -1;

	if( iDay1 > iDay2 )
		return 1;

	return 0;
}

//add by zfq,判断一个字符串是否为浮点数
bool IsFloatNumber( LPCTSTR pszText )
{
	//ASSERT_VALID_STRING( pszText );

	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) && '.' != pszText[i])
			return false;

	return true;
}


//add by zfq,判断某字符串是否是大小, 0 字节, 1.2 KB, 30.5 kb, 
static bool IsSize(CString szText)
{
	int nLen = szText.GetLength();
	if(2 >= nLen)
	{//肯定不是大小
		return false;
	}

	szText.TrimLeft();
	szText.TrimRight();
	CStringArray ary;
	CCommonFun::StringSplit(szText, &ary, " ");	//用空格分离开
	if(2 != ary.GetCount())
	{
		return false;
	}

	CString szTmp = ary.GetAt(0);
	if(!IsFloatNumber(szTmp))
	{
		return false;
	}

	szTmp = ary.GetAt(1);
	bool bRet = false;
	if(0 == szTmp.CompareNoCase(CFFS(IDS_BYTE))
		|| 0 == szTmp.CompareNoCase("Byte")
		|| 0 == szTmp.CompareNoCase("KB")
		|| 0 == szTmp.CompareNoCase("MB")
		|| 0 == szTmp.CompareNoCase("GB")
		)
	{
		bRet = true;
	}

	return bRet;
}

//szText:入参, 5.26 MB
//fSize: 出参, 5.26
//szUnit:出参, MB
static bool SplitSize(CString szText, float &fSize, CString &szUnit)
{
	szText.TrimLeft();
	szText.TrimRight();
	CStringArray ary;
	CCommonFun::StringSplit(szText, &ary, " ");	//用空格分离开
	if(2 != ary.GetCount())
	{
		return false;
	}

	CString szTmp1 = ary.GetAt(0);
	if(!IsFloatNumber(szTmp1))
	{
		return false;
	}

	CString szTmp2 = ary.GetAt(1);
	bool bRet = false;
	if(0 == szTmp2.CompareNoCase(CFFS(IDS_BYTE))
		|| 0 == szTmp2.CompareNoCase("Byte")
		|| 0 == szTmp2.CompareNoCase("KB")
		|| 0 == szTmp2.CompareNoCase("MB")
		|| 0 == szTmp2.CompareNoCase("GB")
		)
	{
		bRet = true;
	}

	if(bRet)
	{
		fSize = (float)atof(szTmp1);
		szUnit = szTmp2;
	}

	return bRet;
}

//add by zfq,返回单位的类型
//字节 返回 1
//KB  返回 2
//MB  返回 3
//GB  返回 4
//其他 返回 0
static int GetSizeType(CString szUint)
{
	int nUint = 1;
	if(0 == szUint.CompareNoCase(CFFS(IDS_BYTE))
		|| 0 == szUint.CompareNoCase(_T("Byte")))
	{
		nUint = 1;
	}
	else if(0 == szUint.CompareNoCase(_T("KB")))
	{
		nUint = 2;
	}
	else if(0 == szUint.CompareNoCase(_T("MB")))
	{
		nUint = 3;
	}
	else if(0 == szUint.CompareNoCase(_T("GB")))
	{
		nUint = 4;
	}
	else 
	{
		ASSERT(0);
		nUint = 0;
	}

	return nUint;
}

//add by zfq,大小判断,2.36 KB, 5.78 MB
static int SizeCompare(  CString szSize1, CString szSize2 )
{
	int nRet = 0;

	CString szUnit1, szUnit2;
	float fSize1 = 0.01, fSize2 = 0.01;
	
	if(!SplitSize(szSize1, fSize1, szUnit1)
		|| !SplitSize(szSize2, fSize2, szUnit2)	)
	{
		ASSERT(0);
		return 0;
	}

	int nUint1 = GetSizeType(szUnit1);
	if(0 == nUint1)
	{
		ASSERT(0);
		return 0;
	}

	int nUint2 = GetSizeType(szUnit2);
	if(0 == nUint2)
	{
		ASSERT(0);
		return 0;
	}

	if(nUint1 < nUint2)
	{//第一个单位小
		return -1;
	}
	else if(nUint1 > nUint2)
	{//第一个单位大
		return 1;
	}

	//单位相等
	nRet = 0;
	if(fSize1 < fSize2)
	{
		nRet = -1;
	}
	else if(fSize1 > fSize2)
	{
		nRet = 1;
	}
	return nRet;
}


/////////////////////////////////////////////////////////////////////////////
// CListCtrl2 message handlers

//*******************************************************************
//
// 排序函数的实现
//
//*******************************************************************
int CALLBACK CListCtrl2::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl2* pV=(CListCtrl2*)lParamSort; 
	CSortItem * i1 = (CSortItem *) lParam1;
	CSortItem * i2 = (CSortItem *) lParam2;	
	CString strItem1 = i1->m_strText;
	CString strItem2 = i2->m_strText; 
	if( IsNumber( strItem1 ) )
		return pV->m_fAsc ? NumberCompare( strItem1, strItem2 ) : NumberCompare( strItem2, strItem1 );
	else if( IsDate( strItem1 ) )
		return pV->m_fAsc ? DateCompare( strItem1, strItem2 ) : DateCompare( strItem2, strItem1 );
	//add by zfq,start
	else if(IsSize(strItem1))
		return pV->m_fAsc ? SizeCompare( strItem1, strItem2 ) : SizeCompare( strItem2, strItem1 );
	//add by zfq,end
	else
		// text.
		return pV->m_fAsc ? lstrcmp( strItem1, strItem2 ) : lstrcmp( strItem2, strItem1 );
}
int CALLBACK CListCtrl2::GroupCompare(INT Group1_ID, INT Group2_ID, void *pvData)
{
	CMap<int, int, CString, CString>&  GroupMap = *(CMap<int, int, CString, CString>*)pvData;
	CString left =   GroupMap[Group1_ID];
	CString right = GroupMap[Group2_ID];

	//组一直降序
	if (1)
		return _tcscmp( left, right );
	else
		return _tcscmp( right, left );	
	return 0;
}
//*******************************************************************
//
// 处理顶部按钮被按下的消息
//
//*******************************************************************
void CListCtrl2::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nCol = pNMListView->iSubItem;
#if (_WIN32_WINNT >= 0x0501)
	if (CCommonFun::IsWinXpOrLater() && IsGroupViewEnabled())
	{
		//点击表头表示更换按组的列
		if (nCol == m_nCurrentGroupColumn)
			return;
		if (ColumnCanNOTGroup(nCol))
			return;
		GroupByColumn(nCol);
		return;
	}
#endif
	if (!m_bSort)
	{
		return;
	}
	RemoveSortImage(m_nSortedCol, m_bSort);

	if( nCol == m_nSortedCol )
		m_fAsc = !m_fAsc;
	else
	{
		m_fAsc = TRUE;
		m_nSortedCol = pNMListView->iSubItem;
	}
	Sort(m_nSortedCol, m_fAsc);
	SetSortImage(m_nSortedCol, m_fAsc);
}
void CListCtrl2::Begin(int iCol)
{
	int max = GetItemCount();	
	for(int i = 0; i < max; i++)	
	{			
		DWORD dw = GetItemData(i);
		CString txt = GetItemText(i, iCol);
		SetItemData(i, (DWORD)new CSortItem(dw, txt));
	}
}

void CListCtrl2::End()
{
	int max = GetItemCount();
	for(int i = 0; i < max; i++)
	{
		CSortItem* pItem = (CSortItem *)GetItemData(i);
		ASSERT(pItem);
		SetItemData(i, pItem->m_dwData);
		delete pItem;
	}
}
CListCtrl2::CSortItem::CSortItem(const DWORD _dw, const CString & _txt)
{
	m_dwData = _dw;
	m_strText = _txt;
}

CListCtrl2::CSortItem::~CSortItem()
{
}

//设置可以在格子中间进行拖动多选
void CListCtrl2::SetGridDrag2MultiSel(bool bEnable)
{
	m_bGridDrag2MultiSel = bEnable;
}

void CListCtrl2::Sort(int nCol /* = 0 */, BOOL bAsc /* = TRUE */)
{
	m_nSortedCol = nCol;
	m_fAsc = bAsc;
	Begin(m_nSortedCol);
	SortItems( ListCompare, reinterpret_cast<DWORD>( this ));
	End();
}

//排列组
void CListCtrl2::SortGroup(int nCol, BOOL bAsc /* = TRUE */)
{
	CMap<int, int, CString, CString> GroupMap;
	for(int i = 0 ; i < GetItemCount() ; i++)
	{
		int nGroupId = GetRowGroupId(i);
		if (nGroupId != -1 && GroupMap[nGroupId].IsEmpty())
			GroupMap[nGroupId] = GetGroupHeader(nGroupId);
	}
	ListView_SortGroups(m_hWnd, GroupCompare, &GroupMap);
}

BOOL CListCtrl2::DisableSort(BOOL bDisable /* = TRUE */)
{
	m_bSort = !bDisable;
	return m_bSort;
}
int CListCtrl2::InsertItem(int nItem, LPCTSTR lpszItem, CheckState nCheck /*= CHECK_BOX_UNCHECK*/)
{
	int nRet  = CListCtrl::InsertItem(nItem, lpszItem);
	if (m_bUseCheckBox)
		SetCheck(nRet, nCheck);
	SetItemData(nRet, nRet);
	return nRet;
}

int CListCtrl2::InsertItem(int nItem, DWORD dwData, LPCTSTR lpszItem, CheckState nCheck /*= CHECK_BOX_UNCHECK*/)
{
	int nRet = InsertItem(nItem, lpszItem, nCheck);
	SetItemData(nRet, dwData);
	return nRet;
}

int CListCtrl2::InsertItem(int nItem, LPCTSTR lpszItem, int nImage, DWORD dwData, CheckState nCheck /* = CHECK_BOX_UNCHECK */)
{
	int nRet = InsertItem(nItem, lpszItem, nImage, nCheck);
	SetItemData(nRet, dwData);
	return nRet;
}

int CListCtrl2::InsertItem(int nItem, LPCTSTR lpszItem, int nImage, CheckState nCheck /* = CHECK_BOX_UNCHECK  */)
{
	int nRet = CListCtrl::InsertItem(nItem, lpszItem, nImage);
	if (m_bUseCheckBox)
		SetCheck(nRet, nCheck);
	SetItemData(nRet, nRet);
	return nRet;
}

int CListCtrl2::InsertItem(const LVITEM* pItem, CheckState nCheck /*= CHECK_BOX_UNCHECK*/)
{
	if (!pItem)
		return -1;

	int nRet = CListCtrl::InsertItem(pItem);
	if (nRet == -1)
		return -1;

	if (pItem->mask & LVIF_PARAM)
		;
	else
		SetItemData(nRet, nRet);

	if (m_bUseCheckBox)
		SetCheck(nRet, nCheck);
	return nRet;
}

int CListCtrl2::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */, BOOL bEdit /* = FALSE */)
{
	int iRet = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
	m_EditMap[nCol] = bEdit;
	return iRet;
}



/*

void CListCtrl2::ShowLink(int nItem,int nSubItem,CString strText)
{
CRect rcCtrl;
m_pLinkStatic = new CLinkStatic();
GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);
m_pLinkStatic->Create(NULL, WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_STATIC + (++m_nLinkCount));

CFont tpFont;
tpFont.CreateStockObject(DEFAULT_GUI_FONT);
m_pLinkStatic->SetFont(&tpFont);
tpFont.DeleteObject();

//CString strItem = CListCtrl::GetItemText(nItem,nIndex);
m_pLinkStatic->SetWindowText(strText);
m_pLinkStatic->SetItemData(GetParent(), nItem, nSubItem, strText);
m_LinkStaticList.AddTail(m_pLinkStatic);
//::SetFocus(m_Link.GetSafeHwnd());
//m_Link.SetSel(-1);  
//m_Link.SetCtrlData(MAKEWPARAM(nIndex,nItem));	
}
*/
/*

BOOL CListCtrl2::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText, BOOL bLink / * = FALSE * /)
{
BOOL bRet;
if (!bLink || (strlen(lpszText) == 0))
{
return CListCtrl::SetItemText(nItem, nSubItem, lpszText);
}
else
{
//listctrl中不显示内容，放在static中显示
bRet = CListCtrl::SetItemText(nItem, nSubItem, _T(""));
//ShowLink(nItem, nSubItem, lpszText);
}
return bRet;
}*/

void CListCtrl2::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	int nItem=lpDIS->itemID;
	if(nItem == -1)
		return ;
	CRect rcCol = lpDIS->rcItem;
	CString sText;

	CDC* pDC=CDC::FromHandle(lpDIS->hDC);
	int nOldDCMode=pDC->SaveDC();

	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));
	item.iItem = nItem;
	item.iSubItem = 0;
	item.mask = LVIF_IMAGE | LVIF_STATE;
	item.stateMask = 0XFFFF;
	GetItem(&item);
	BOOL bSelected = item.state & LVIS_SELECTED;

	COLORREF color = ::GetSysColor(COLOR_WINDOW);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	//列顺序有可能被改变
	int nColCount = GetHeaderCtrl()->GetItemCount();
	int* pOrderArray = new int[nColCount];
	GetColumnOrderArray(pOrderArray, nColCount);
	//CCommonFun::WriteFileLog("nColcount = %d", nColCount);
	//rcCol.right = rcCol.left; 
	//rcCol.right = rcCol.left + GetColumnWidth(nCol);
	for(int nCol = 0; GetColumn(pOrderArray[nCol], &lvc) && (nCol < nColCount); nCol++)
	{
	//	CCommonFun::WriteFileLog("nCol = %d", nCol);
		rcCol.right = rcCol.left + GetColumnWidth(pOrderArray[nCol]);
		//HPEN hOldPen = (HPEN)::SelectObject(lpDIS->hDC, ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT)));
		//HBRUSH hOldBrush = (HBRUSH)::SelectObject(lpDIS->hDC, ::CreateSolidBrush(color));
		//::DeleteObject(SelectObject(lpDIS->hDC, hOldBrush));
		//::DeleteObject(SelectObject(lpDIS->hDC, hOldPen));

		//sText=MakeShortString(pDC,GetItemText(nItem,nCol),rcCol.Width());
		if(bSelected )
		{	
			HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
			::FillRect(lpDIS->hDC, rcCol, hBrush);
			DeleteObject(hBrush);
			pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			color=::GetSysColor(COLOR_HIGHLIGHT);
		}
		else
		{	
			HBRUSH hBrush;
			if (m_bUseColorBelt && (nItem % 2 == 0))
				hBrush = CreateSolidBrush(m_crBelt1);
			else
				hBrush = CreateSolidBrush(m_crBelt2);
			::FillRect(lpDIS->hDC, rcCol, hBrush);
			DeleteObject(hBrush);
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		}
		if (pOrderArray[nCol] == 0)
		{
			CRect rcDraw = rcCol;
			rcDraw.right = rcDraw.left;
			//rcDraw.right = rcDraw.left + 16;
#if 0
			//画系统checkbox
			if (GetExtendedStyle() & LVS_EX_CHECKBOXES)
			{
				DrawFrameControl(pDC->m_hDC, &rcDraw, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
				rcDraw.left = rcDraw.right;
				rcDraw.right = rcDraw.left + 16;
			}
#endif
			//画自己的checkbox
			CImageList* pImageList = GetImageList(LVSIL_STATE);
			if (pImageList && item.state & LVIS_STATEIMAGEMASK)
			{
				rcDraw.left = rcDraw.right;
				rcDraw.right = rcDraw.left + 16;
				int nStateImage = ((item.state & LVIS_STATEIMAGEMASK) >> 12) - 1;
				pImageList->Draw(pDC, nStateImage, CPoint(rcDraw.left, rcDraw.top), ILD_TRANSPARENT);
			}
			//画icon
			pImageList = GetImageList(LVSIL_SMALL);
			if (pImageList)
			{
				rcDraw.left = rcDraw.right;
				rcDraw.right = rcDraw.left + 8;
				pImageList->Draw(pDC, item.iImage, CPoint(rcDraw.left, rcDraw.top), ILD_TRANSPARENT);
			}
			rcCol.left  = rcDraw.right;
		}

		sText = GetItemText(nItem, pOrderArray[nCol]);
		if (IsLink(nItem, nCol))
			pDC->SetTextColor(RGB(0, 0, 255));
		pDC->DrawText(sText, -1, CRect::CRect(rcCol.left + 8, rcCol.top, rcCol.right, rcCol.bottom), DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE);
		rcCol.left = rcCol.right;
	}
	delete [] pOrderArray;
	pDC->RestoreDC(nOldDCMode);	
}
BOOL CListCtrl2::IsLink(int nItem, int nSubItem)
{
	for (POSITION pos = m_LinkList.GetHeadPosition(); pos !=NULL;)
	{
		CPoint pt = m_LinkList.GetNext(pos);
		if (GetDataItem(pt.x) == nItem && pt.y == nSubItem)
			return TRUE;
	}
	return FALSE;
}

void CListCtrl2::SetLink(int nItem, int nSubItem)
{
	//显示链接要设置owner draw fixed属性！
	ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & LVS_OWNERDRAWFIXED);

	CPoint pt(nItem, nSubItem);
	if (GetItemText(nItem, nSubItem).GetLength() > 0)
	{
		pt.x = (LONG)GetItemData(nItem);//排序时item会变化
		m_LinkList.AddTail(pt);
	}
}
void CListCtrl2::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt;
	CRect rcCtrl;
	int nItem, nSubItem;
	for(POSITION pos = m_LinkList.GetHeadPosition(); pos != NULL;)
	{
		pt = m_LinkList.GetNext(pos);
		nItem = GetDataItem(pt.x);
		nSubItem = pt.y;
		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);
		if (rcCtrl.PtInRect(point))
		{
			SetCursor(m_hLinkCursor);
		}
	}

	// Added by jarry 09/08/14
	if (m_bToolTips)
	{
		LVHITTESTINFO lvhti;
		lvhti.pt = point;
		SubItemHitTest(&lvhti);
		if (lvhti.iItem != -1 && lvhti.iSubItem != -1)
		{
			if (lvhti.iItem != m_nMainItem || lvhti.iSubItem != m_nSubItem)
			{
				m_nMainItem = lvhti.iItem;
				m_nSubItem = lvhti.iSubItem;
				m_sString = GetItemText(m_nMainItem,m_nSubItem);
				m_sString.TrimLeft(' ');
				if (m_pToolTipCtrl.AddTool(this,m_sString))
					m_pToolTipCtrl.Pop();
			}
		}
		else
		{
			m_nMainItem = -1;
			m_nSubItem = -1;
			m_pToolTipCtrl.AddTool(this,"");
			m_pToolTipCtrl.Pop();
		}
	}

	//add by zfq,start
	if(m_bGridDrag2MultiSel
		&& (MK_LBUTTON & nFlags))
	{//允许在格子中间拖动多选,且当前左键是按下的
		int nEndIndex = HitTest(point);
		if(0 <= m_nStartIndex && 0 <= nEndIndex)
		{
			//add by song
			if (GetFocus() != this)
			{
				SetFocus();
			}

			int nMin = 0, nMax = 0;
			if(m_nStartIndex < nEndIndex)
			{
				nMin = m_nStartIndex;
				nMax = nEndIndex;
			}//if(m_nStartIndex < nEndIndex)
			else
			{
				nMin = nEndIndex;
				nMax = m_nStartIndex;
			}//else

			int nCount = GetItemCount();
			int i = 0;
			for(i = 0; i < nCount; i++)
			{
				if(i >= nMin && i <= nMax)
				{//在范围之内,选中
					SetItemState(i,LVNI_SELECTED,LVNI_SELECTED);
				}
				else
				{
					SetItemState(i, NULL, LVIS_SELECTED);
				}
			}//for(i = 0; i < nCount; i++)
		}//if(0 < m_nStartIndex && 0 < nEndIndex)
	}//if(MK_LBUTTON == nFlags)
	else
	{
		m_nStartIndex = -1;
	}
	//end

	CListCtrl::OnMouseMove(nFlags, point);
}

void CListCtrl2::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bReadOnly)
	{
		CRect rcCtrl;
		LVHITTESTINFO lvhti;
		lvhti.pt = point;
		int nItem = CListCtrl::SubItemHitTest(&lvhti);
		if(nItem == -1)
		{
			CListCtrl::OnLButtonDown(nFlags, point);
			return;
		}
		int nSubItem = lvhti.iSubItem;
		if (m_EditMap[nSubItem])
		{
			CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl); 
			if (IsWindow(m_edit.GetSafeHwnd()) && m_edit.IsWindowVisible())
			{
				CRect rc;
				m_edit.GetWindowRect(rc);
				ScreenToClient(&rc);
				if (rc != rcCtrl)
				{
					OnEditEnd(TRUE, MAKELPARAM(TRUE, TRUE));
				}
			}
			ShowEdit(TRUE,nItem,nSubItem,rcCtrl);
			for (int i = 0; i < GetItemCount(); i++)
			{
				SetItemState(i, ((i == nItem) ? LVIS_SELECTED : ~LVIS_SELECTED), LVIS_SELECTED);
			}
			return;
		}
	}

	if (GetStyle() & LVS_OWNERDRAWFIXED && GetExtendedStyle() & LVS_EX_CHECKBOXES)
	{
		UINT flags = 0;
		int nRet = HitTest(point, &flags);
		if (flags == LVHT_ONITEMSTATEICON)
		{
			SetCheck(nRet, !GetCheck(nRet));
		}
	}

	int nItem, nSubItem;
	CRect rcCtrl;
	CPoint pt;
	for(POSITION pos = m_LinkList.GetHeadPosition(); pos != NULL;)
	{
		pt = m_LinkList.GetNext(pos);
		nItem = GetDataItem(pt.x);
		nSubItem = pt.y;
		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);
		if (rcCtrl.PtInRect(point))
		{
			SetCursor(m_hLinkCursor);
			::PostMessage(GetParent()->m_hWnd, WM_P_CLICK_LINK, GetDlgCtrlID(), MAKELPARAM(nItem, nSubItem));
		}
	}

	m_nStartIndex = HitTest(point);

	CListCtrl::OnLButtonDown(nFlags, point);
}

void CListCtrl2::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonUp(nFlags, point);
}


int CListCtrl2::GetDataItem(DWORD dwData)
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetItemData(i) == dwData)
			return i;
	}
	return -1;
}
BOOL CListCtrl2::DeleteAllItems()
{
	m_LinkList.RemoveAll();
#if (_WIN32_WINNT >= 0x0501)
	if (m_bEnableCategoryGroups)
	{
		RemoveAllGroups();
		EnableGroupView(FALSE);
	}
#endif
	return CListCtrl::DeleteAllItems();
}

void CListCtrl2::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
#if 0
	//点一下就编辑
	if (!m_bReadOnly)
	{
		CRect rcCtrl;
		LVHITTESTINFO lvhti;
		lvhti.pt = point;
		int nItem = CListCtrl::SubItemHitTest(&lvhti);
		if(nItem == -1)
			goto RET;
		int nSubItem = lvhti.iSubItem;
		if (m_EditMap[nSubItem])
		{
			CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl); 
			ShowEdit(TRUE,nItem,nSubItem,rcCtrl);
		}
	}
#endif
	int nGroupId = GroupHitTest(point);
	if (nGroupId!=-1)
	{
		if (HasGroupState(nGroupId, LVGS_COLLAPSED))
			SetGroupState(nGroupId, LVGS_NORMAL);
		else
			SetGroupState(nGroupId, LVGS_COLLAPSED);
	}
	CListCtrl::OnLButtonDblClk(nFlags, point);
}
void CListCtrl2::ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rcCtrl)
{
	if(m_edit.m_hWnd == NULL)
	{
		m_edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_LISTEDIT);
		m_edit.ShowWindow(SW_HIDE);

		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}
	if(bShow == TRUE)
	{
		CString strItem = CListCtrl::GetItemText(nItem,nIndex);
		m_edit.MoveWindow(rcCtrl);
		m_edit.ShowWindow(SW_SHOW);
		m_edit.SetWindowText(strItem);
		::SetFocus(m_edit.GetSafeHwnd());
		m_edit.SetSel(0, -1);  
		m_edit.SetCtrlData((DWORD)MAKEWPARAM(nIndex,nItem));	
	}
	else
		m_edit.ShowWindow(SW_HIDE);
}
//LPARAM高位表示是否通知父窗口，低位表示是否显示edit
LRESULT CListCtrl2::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TRUE)
	{
		CString strText(_T(""));
		m_edit.GetWindowText(strText);
		DWORD dwData = m_edit.GetCtrlData();
		int nItem= dwData>>16;
		int nIndex = dwData&0x0000ffff;
		CString strTemp = CListCtrl::GetItemText(nItem, nIndex);
		if (strTemp != strText)
		{
			CListCtrl::SetItemText(nItem, nIndex, strText);
			if (HIWORD(lParam))
				::PostMessage(GetParent()->m_hWnd, WM_P_EDIT_END,
				GetDlgCtrlID(), MAKELPARAM(nItem, nIndex));
		}
	}
	else
	{

	}

	if(LOWORD(lParam) == FALSE)
		m_edit.ShowWindow(SW_HIDE);
	return true;
}

void CListCtrl2::OnParentNotify(UINT message, LPARAM lParam) 
{
	CListCtrl::OnParentNotify(message, lParam);
	//////////////////////////////////////////////////////////////////////////
	CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
	if(pHeaderCtrl == NULL)
		return;
	CRect rcHeader;
	pHeaderCtrl->GetWindowRect(rcHeader);
	ScreenToClient(rcHeader);
	//The x coordinate is in the low-order word and the y coordinate is in the high-order word.

	CPoint pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	if(rcHeader.PtInRect(pt) && message == WM_LBUTTONDOWN)
	{
		if(m_edit.m_hWnd != NULL)
		{
			DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
				m_edit.ShowWindow(SW_HIDE);
			}
		}	
	}	
}
BOOL CListCtrl2::PreTranslateMessage(MSG* pMsg) 
{
	if (::IsWindow(m_pToolTipCtrl.GetSafeHwnd()))
		m_pToolTipCtrl.RelayEvent(pMsg);

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_TAB && m_edit.m_hWnd!= NULL)
		{
			DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
				OnEditEnd(TRUE, MAKELPARAM(TRUE, FALSE));
				CRect rcCtrl;		 
				int nItem;
				int nSub;
				if(FALSE == Key_Ctrl(nItem,nSub))
					Key_Shift(nItem,nSub);
				CListCtrl::GetSubItemRect(nItem,nSub,LVIR_LABEL,rcCtrl); 
				CPoint pt(rcCtrl.left+1,rcCtrl.top+1);
				OnLButtonDown(0,pt);
				POSITION pos = CListCtrl::GetFirstSelectedItemPosition();
				if (pos == NULL)
				{
				}
				else
				{
					while (pos)
					{
						int ntpItem = CListCtrl::GetNextSelectedItem(pos);
						CListCtrl::SetItemState(ntpItem,0,LVIS_SELECTED);
					}
				}
				CListCtrl::SetItemState(nItem,  LVIS_SELECTED,  LVIS_SELECTED);
				return TRUE;
			}
		}
	}
	else if (LVM_HITTEST == pMsg->message)
	{
		TRACE("abc");
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
BOOL CListCtrl2::Key_Shift(int& nItem,int& nSub)
{
	int nItemCount = CListCtrl::GetItemCount();
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;

	CHeaderCtrl* pHeader = CListCtrl::GetHeaderCtrl();
	if(pHeader == NULL)
		return FALSE;

	short sRet = GetKeyState(VK_SHIFT);
	int nSubcCount = pHeader->GetItemCount();
	sRet = sRet >>15;
	if(sRet == 0)
	{
		nSub += 1;
		if(nSub >= nSubcCount)
		{
			if(nItem == nItemCount-1)
			{
				nItem = 0;
				nSub  = 0;
			}
			else
			{
				nSub = 0;
				nItem += 1;
			}
		}
		if(nItem >= nItemCount)
			nItem = nItemCount-1;
		return FALSE;
	}
	else
	{
		nSub -= 1;
		if(nSub < 0)
		{

			nSub = nSubcCount -1;
			nItem --;
		}

		if(nItem < 0)
			nItem = nItemCount-1;
		return TRUE;

	}
	return FALSE;
}
BOOL CListCtrl2::Key_Ctrl(int& nItem,int &nSub)
{
	short sRet = GetKeyState(VK_CONTROL);
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;

	sRet = sRet >>15;
	int nItemCount = CListCtrl::GetItemCount();
	if(sRet == 0)
	{

	}
	else
	{
		nItem = nItem >=nItemCount-1? 0:nItem+=1;
		return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CListCtrlEdit::CListCtrlEdit()
{
}

CListCtrlEdit::~CListCtrlEdit()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEdit, CEdit)
	//{{AFX_MSG_MAP(CListCtrlEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEdit message handlers
void CListCtrlEdit::SetCtrlData(DWORD dwData)
{
	m_dwData = dwData;
}

DWORD CListCtrlEdit::GetCtrlData()
{
	return m_dwData;
}

void CListCtrlEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CWnd* pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(), WM_P_USER_EDIT_END,
		m_bExchange, MAKELPARAM(FALSE, TRUE));
}
BOOL CListCtrlEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd* pParent = this->GetParent();
			m_bExchange = TRUE;
			::PostMessage(pParent->GetSafeHwnd(), WM_P_USER_EDIT_END,
				m_bExchange, MAKELPARAM(FALSE, FALSE));
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			CWnd* pParent = this->GetParent();
			m_bExchange = FALSE;
			::PostMessage(pParent->GetSafeHwnd(), WM_P_USER_EDIT_END,
				m_bExchange, MAKELPARAM(FALSE, FALSE));
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CListCtrlEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	m_bExchange = TRUE;	
}

void CListCtrl2::SetReadOnly(BOOL bReadOnly /* = TRUE */)
{
	m_bReadOnly = bReadOnly;
}

void CListCtrl2::UseCheckBox(BOOL bUse /* = TRUE */)
{
	m_bUseCheckBox = bUse;
	if (m_bUseCheckBox)
	{
		m_imgList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 0);
		HICON hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_UNCHECK);
		m_imgList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_CHECK);
		m_imgList.Add(hIcon);
		hIcon = LoadIcon(theApp.m_hInstance, (LPCTSTR)IDI_DISABLECHECK);
		m_imgList.Add(hIcon);
		SetImageList(&m_imgList, LVSIL_STATE);
	}
	else
	{
		if (m_imgList.m_hImageList)
		{
			m_imgList.DeleteImageList();
			SetImageList(NULL, LVSIL_STATE);
		}
	}
}
int CListCtrl2::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_bUseCheckBox)
		return -1;
	CString str;
	UINT   flag; 
	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR; 
	int   nItem   =   HitTest(pNMListView->ptAction,   &flag) ;
	if(flag   == LVHT_ONITEMSTATEICON)   //当前用户点击的是CheckBox   
	{  	
		ReverseState(nItem);
		return nItem;
	}
	*pResult = 0;
	return -1;
}
void CListCtrl2::ReverseState(int nItem)
{
	if (nItem == -1)
		return;
	CheckState cs = GetCheckState(nItem);
	if (cs == CHECK_BOX_CHECKED)
		SetCheck(nItem, CHECK_BOX_UNCHECK);
	else if (cs == CHECK_BOX_UNCHECK)
		SetCheck(nItem, CHECK_BOX_CHECKED);
}
BOOL CListCtrl2::GetCheck(int nItem )const
{
	if (!m_bUseCheckBox)
		return CListCtrl::GetCheck(nItem);
	return (GetCheckState(nItem) == CHECK_BOX_CHECKED);
}
CListCtrl2::CheckState CListCtrl2::GetCheckState(int nItem)const
{
	if (!m_bUseCheckBox)
	{
		ASSERT(0);
		return CHECK_BOX_UNCHECK;
	}
	UINT nState = (GetItemState(nItem, LVIS_STATEIMAGEMASK) & LVIS_STATEIMAGEMASK); 
	nState >>= 12;
	nState -= 1;
	return (CheckState)(nState);
}
int	 CListCtrl2::GetCheckCount()
{
	int nTotal = GetItemCount();
	if (nTotal == 0)
		return 0;
	int nCount = 0;

	for (int i = 0; i < nTotal; i++)
		if (GetCheck(i))
			nCount++;
	return nCount;
}
BOOL CListCtrl2::SetCheck(int nItem, CheckState fCheck /* = CHECK_BOX_CHECKED */)
{
	if (!m_bUseCheckBox)
		return CListCtrl::SetCheck(nItem, (fCheck == CHECK_BOX_CHECKED));

	if (GetCheckState(nItem) == fCheck)
	{
		return TRUE;
	}
	return SetItemState(nItem, (fCheck + 1)<<12, LVIS_STATEIMAGEMASK);
}

BOOL CListCtrl2::SetCheck(int nItem, int nCheck /* = TRUE */)
{
	if (m_bUseCheckBox)
	{
		if (GetCheckState(nItem) == CHECK_BOX_DISABLED)
			return TRUE;
		return SetCheck(nItem, nCheck ? CHECK_BOX_CHECKED : CHECK_BOX_UNCHECK);
	}
	return CListCtrl::SetCheck(nItem, nCheck);
}

BOOL CListCtrl2::EnableTips()
{
	EnableToolTips(TRUE);
	if (m_bToolTips)
		return m_bToolTips;

	if (m_pToolTipCtrl.Create(this,TTS_ALWAYSTIP))
	{
		m_pToolTipCtrl.Activate(TRUE);
		m_pToolTipCtrl.SetDelayTime(TTDT_INITIAL,0);
		m_bToolTips = TRUE;
	}
	return m_bToolTips;
}

int CListCtrl2::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult, BOOL bMultiCheck /*= FALSE*/)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if (!m_bUseCheckBox)
	{
		return -1;
	}
	if (pLVKeyDow->wVKey == VK_SPACE)
	{
		int nIndex = -1;
		for (POSITION pos = GetFirstSelectedItemPosition(); pos != NULL;)
		{
			nIndex = GetNextSelectedItem(pos);
			if (bMultiCheck)
				ReverseState(nIndex);
		}
		if (bMultiCheck)
			return nIndex;
		ReverseState(nIndex);
		return nIndex;
	}
	*pResult = 0;
	return -1;
}

void CListCtrl2::SetColorBelt(COLORREF cr1, COLORREF cr2)
{
	m_bUseColorBelt = TRUE;
	m_crBelt1 = cr1;
	m_crBelt2 = cr2;
	if (!IsWindow(GetSafeHwnd()))
		return;
	ModifyStyle(NULL, LVS_OWNERDRAWFIXED, NULL);
	ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & LVS_OWNERDRAWFIXED);
}
BOOL IsCommCtrlVersion6()
{
	BOOL isCommCtrlVersion6 = -1;
	if (isCommCtrlVersion6 != -1)
		return isCommCtrlVersion6;

	//The default value
	isCommCtrlVersion6 = FALSE;

	HINSTANCE commCtrlDll = LoadLibrary(_T("comctl32.dll"));
	if (commCtrlDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(commCtrlDll, "DllGetVersion");

		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi = {0};
			dvi.cbSize = sizeof(DLLVERSIONINFO);
			(*pDllGetVersion)(&dvi);

			isCommCtrlVersion6 = (dvi.dwMajorVersion == 6);
		}

		FreeLibrary(commCtrlDll);
	}

	return isCommCtrlVersion6;
}
void CListCtrl2::RemoveSortImage(int nCol, BOOL bAsc)	//移除指定列的排序图标
{
	CHeaderCtrl* pHdrCtrl = GetHeaderCtrl();
	HD_ITEM hditem;	
	hditem.mask=HDI_FORMAT;
	pHdrCtrl->GetItem(nCol,&hditem);
	hditem.mask=HDI_FORMAT;	
	//if (bAsc)
	hditem.fmt &= ~HDF_SORTUP;
	//else
	hditem.fmt &= ~HDF_SORTDOWN;
	pHdrCtrl->SetItem(nCol,&hditem);

}

void CListCtrl2::SetSortImage(int nCol,BOOL bAsc)
{
	CHeaderCtrl* pHdrCtrl = GetHeaderCtrl();
	HD_ITEM hditem;	
	hditem.mask=HDI_FORMAT;
	pHdrCtrl->GetItem(nCol,&hditem);
	hditem.mask=HDI_FORMAT;	
	if (bAsc)
		hditem.fmt |= HDF_SORTUP;
	else
		hditem.fmt |= HDF_SORTDOWN;
	pHdrCtrl->SetItem(nCol,&hditem);
}

void CListCtrl2::EnableColumnPicker(BOOL bEnable /* = TRUE */)
{
	ASSERT(bEnable);
	m_bEnableColumnPicker = bEnable;
	//if (IsWindow(m_hWnd) && !(GetExtendedStyle() & LVS_EX_HEADERDRAGDROP))
	//	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
}

int CListCtrl2::GetFirstVisibleColumn()
{
	int nColCount = GetHeaderCtrl()->GetItemCount();
	for(int i = 0; i < nColCount; ++i)
	{
		int nCol = GetHeaderCtrl()->OrderToIndex(i);
		if (IsColumnVisible(nCol))
		{
			return nCol;
		}
	}
	return -1;
}

BOOL CListCtrl2::ShowColumn(int nCol, bool bShow)
{
	ColumnState& columnState = GetColumnState(nCol);

	//重复设置会有问题
	if (columnState.m_Visible == bShow)
		return TRUE;
	SetRedraw(FALSE);
	int nColCount = GetHeaderCtrl()->GetItemCount();
	int* pOrderArray = new int[nColCount];
	GetColumnOrderArray(pOrderArray, nColCount);
	if (bShow)
	{
		// Restore the position of the column
		int nCurIndex = -1;
		for(int i = 0; i < nColCount ; ++i)
		{
			if (pOrderArray[i]==nCol)
				nCurIndex = i;
			else
				if (nCurIndex!=-1)
				{
					// We want to move it to the original position,
					// and after the last hidden column
					if ( (i <= columnState.m_OrgPosition)
						|| !IsColumnVisible(pOrderArray[i])
						)
					{
						pOrderArray[nCurIndex] = pOrderArray[i];
						pOrderArray[i] = nCol;
						nCurIndex = i;
					}
				}
		}
	}
	else
	{
		// Move the column to the front of the display order list
		int nCurIndex(-1);
		for(int i = nColCount-1; i >=0 ; --i)
		{
			if (pOrderArray[i]==nCol)
			{
				// Backup the current position of the column
				columnState.m_OrgPosition = i;
				nCurIndex = i;
			}
			else
				if (nCurIndex!=-1)
				{
					pOrderArray[nCurIndex] = pOrderArray[i];
					pOrderArray[i] = nCol;
					nCurIndex = i;
				}
		}
	}

	SetColumnOrderArray(nColCount, pOrderArray);
	delete [] pOrderArray;

	if (bShow)
	{
		// Restore the column width
		columnState.m_Visible = true;
		SetColumnWidth(nCol, columnState.m_OrgWidth);
	}
	else
	{
		// Backup the column width
		int orgWidth = GetColumnWidth(nCol);
		SetColumnWidth(nCol, 0);
		columnState.m_Visible = false;
		if (orgWidth != 0)
			columnState.m_OrgWidth = orgWidth;
	}
	SetRedraw(TRUE);
	Invalidate(FALSE);
	return TRUE;
}

BOOL CListCtrl2::SetColumnWidthAuto(int nCol, bool includeHeader)
{
	if (nCol == -1)
	{
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount() ; ++i)
		{
			SetColumnWidthAuto(i, includeHeader);
		}
		return TRUE;
	}
	else
	{
		if (includeHeader)
			return SetColumnWidth(nCol, LVSCW_AUTOSIZE_USEHEADER);
		else
			return SetColumnWidth(nCol, LVSCW_AUTOSIZE);
	}
}

CListCtrl2::ColumnState& CListCtrl2::GetColumnState(int nCol)
{
	VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
	return m_ColumnStates[nCol];
}

bool CListCtrl2::IsColumnVisible(int nCol)
{
	return GetColumnState(nCol).m_Visible;
}

int CListCtrl2::GetColumnStateCount()
{
	return m_ColumnStates.GetSize();
}

void CListCtrl2::InsertColumnState(int nCol, bool bVisible, int nOrgWidth)
{
	VERIFY( nCol >=0 && nCol <= m_ColumnStates.GetSize() );

	ColumnState columnState;
	columnState.m_OrgWidth = nOrgWidth;
	columnState.m_Visible = bVisible;

	if (nCol == m_ColumnStates.GetSize())
	{
		// Append column picker to the end of the array
		m_ColumnStates.Add(columnState);
	}
	else
	{
		// Insert column in the middle of the array
		CSimpleArray<ColumnState> newArray;
		for(int i=0 ; i < m_ColumnStates.GetSize(); ++i)
		{
			if (i == nCol)
				newArray.Add(columnState);
			newArray.Add(m_ColumnStates[i]);
		}
		m_ColumnStates = newArray;
	}
}

void CListCtrl2::DeleteColumnState(int nCol)
{
	VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
	m_ColumnStates.RemoveAt(nCol);
}

void CListCtrl2::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);

	CRect headerRect;
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return;
	pHeaderCtrl->GetClientRect(&headerRect);
	if (headerRect.PtInRect(pt))//在表头右击
	{
		CMenu menu;
		if (!menu.CreatePopupMenu())
			return;

		if (m_bEnableColumnPicker)
		{
			// Show context-menu with the option to show hide columns
			for( int i = GetColumnStateCount() - 1 ; i >= 0; --i)
			{
				UINT uFlags = MF_BYPOSITION | MF_STRING;

				// Put check-box on context-menu
				if (IsColumnVisible(i))
					uFlags |= MF_CHECKED;
				else
					uFlags |= MF_UNCHECKED;

				// Retrieve column-title
				LVCOLUMN lvc = {0};
				lvc.mask = LVCF_TEXT;
				TCHAR sColText[256];
				lvc.pszText = sColText;
				lvc.cchTextMax = sizeof(sColText)-1;
				GetColumn(i, &lvc);
				if (strlen(lvc.pszText) > 0)
					menu.InsertMenu(0, uFlags, IDM_COLUMN_SHOW_START + i, lvc.pszText);
			}
		}
#if (_WIN32_WINNT >= 0x0501)
		if (m_bEnableCategoryGroups)
		{
			CMenu subMenu;
			if (subMenu.CreatePopupMenu())
			{
				for( int i = 0 ; i < pHeaderCtrl->GetItemCount(); i++)
				{
					if (ColumnCanNOTGroup(i))
						continue;
					// Put check-box on context-menu
					UINT uFlags = MF_BYPOSITION | MF_STRING;
					// Retrieve column-title
					LVCOLUMN lvc = {0};
					lvc.mask = LVCF_TEXT;
					TCHAR sColText[256];
					lvc.pszText = sColText;
					lvc.cchTextMax = sizeof(sColText)-1;
					GetColumn(i, &lvc);
					if (strlen(lvc.pszText) > 0)
						subMenu.AppendMenu(uFlags, IDM_GROUP_BY_START + i, lvc.pszText);
				}
				if (m_nCurrentGroupColumn != -1)
					subMenu.CheckMenuRadioItem(IDM_GROUP_BY_START, IDM_GROUP_BY_END, IDM_GROUP_BY_START + m_nCurrentGroupColumn, MF_BYCOMMAND);
				if (CCommonFun::IsWinXpOrLater())
				{
					if (menu.GetMenuItemCount() > 0)
						menu.AppendMenu(MF_SEPARATOR, NULL);
					menu.AppendMenu(MF_POPUP, (UINT)subMenu.Detach(), m_strGroupBy);
					if (IsGroupViewEnabled())
						menu.AppendMenu(MF_BYPOSITION | MF_STRING, IDM_CANCEL_GROUP, m_strCancelGroups);
				}
			}
		}//分组

		if (menu.GetMenuItemCount() <= 0)
			return;
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, 0);
#endif
	}//在表头单击
}

void CListCtrl2::OnItemCountChange()
{
	if (m_bEnableCategoryGroups)
	{
		if (m_nCurrentGroupColumn != -1)
			GroupByColumn(m_nCurrentGroupColumn); 
	}
}

void CListCtrl2::DisableGroupColumn(int nColumn)
{
	m_NoGroupColumnAry.Add(nColumn);
}
// Handle context-menu event for showing / hiding columns
BOOL CListCtrl2::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0)
	{
		int nCmd = LOWORD(wParam);
		if (nCmd >= IDM_COLUMN_SHOW_START && nCmd <= IDM_COLUMN_SHOW_END)
		{
			int nCol = nCmd - IDM_COLUMN_SHOW_START;
			ShowColumn(nCol, !IsColumnVisible(nCol));
		}
		else if (nCmd >= IDM_GROUP_BY_START && nCmd <= IDM_GROUP_BY_END)
		{
			GroupByColumn(nCmd - IDM_GROUP_BY_START); 
		}
		else if (nCmd == IDM_CANCEL_GROUP)
			CancelGroups();

	}
	return TRUE;
}
void CListCtrl2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_ADD:	// CTRL + NumPlus (Auto size all columns)
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				// Special handling to avoid showing "hidden" columns
				SetColumnWidthAuto(-1);
				return;
			}
		} break;
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CListCtrl2::OnHeaderBeginResize(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	// Check that column is allowed to be resized
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	int nCol = (int)pNMH->iItem;
	if (!IsColumnVisible(nCol))
	{
		*pResult = TRUE;	// Block resize
		return TRUE;		// Block event
	}
	return FALSE;
}

LRESULT CListCtrl2::OnSetColumnWidth(WPARAM wParam, LPARAM lParam)
{
	// Check that column is allowed to be resized
	int nCol = (int)wParam;
	if (!IsColumnVisible(nCol))
	{
		return FALSE;
	}
	// Let the CListCtrl handle the event
	return DefWindowProc(LVM_SETCOLUMNWIDTH, wParam, lParam);
}

BOOL CListCtrl2::OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	if (pNMH->pitem->mask & HDI_ORDER)
	{
		// Correct iOrder so it is just after the last hidden column
		int nColCount = GetHeaderCtrl()->GetItemCount();
		int* pOrderArray = new int[nColCount];
		GetColumnOrderArray(pOrderArray, nColCount);

		for(int i = 0; i < nColCount ; ++i)
		{
			if (IsColumnVisible(pOrderArray[i]))
			{
				pNMH->pitem->iOrder = max(pNMH->pitem->iOrder,i);
				break;
			}
		}
		delete [] pOrderArray;
	}
	return FALSE;
}

BOOL CListCtrl2::OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pNMH = (NMHEADER*)pNMHDR;
	SetColumnWidthAuto(pNMH->iItem);
	return TRUE;	// Don't let parent handle the event
}

LRESULT CListCtrl2::OnDeleteColumn(WPARAM wParam, LPARAM lParam)
{
	// Let the CListCtrl handle the event
	LRESULT lRet = DefWindowProc(LVM_DELETECOLUMN, wParam, lParam);
	if (lRet == FALSE)
		return FALSE;

	// Book keeping of columns
	DeleteColumnState((int)wParam);
	return lRet;
}

LRESULT CListCtrl2::OnInsertColumn(WPARAM wParam, LPARAM lParam)
{
	// Let the CListCtrl handle the event
	LRESULT lRet = DefWindowProc(LVM_INSERTCOLUMN, wParam, lParam);
	if (lRet == -1)
		return -1;

	int nCol = (int)lRet;

	// Book keeping of columns
	if (GetColumnStateCount() < GetHeaderCtrl()->GetItemCount())
		InsertColumnState((int)nCol, true);	// Insert as visible

	return lRet;
}

void CListCtrl2::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	if (m_bEnableColumnPicker)
	{
		// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
		SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
		//SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
		//SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
		//SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
	}

	//可以多选的都可以拖动
	if (GetStyle() & LVS_SINGLESEL)
	{
		//单选
		m_bGridDrag2MultiSel = FALSE;
	}
	else
	{
		m_bGridDrag2MultiSel = TRUE;
	}

	//都有颜色间隔
	if (!(GetExtendedStyle() & LVS_OWNERDRAWFIXED))
		ModifyStyle(NULL, LVS_OWNERDRAWFIXED, NULL);
	//ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & LVS_OWNERDRAWFIXED);
}

void CListCtrl2::EnableCategoryGroups(CString strGroups, CString strCancelGroups, BOOL bAutoHide, BOOL bEnable /* = TRUE */)
{
#if (_WIN32_WINNT >= 0x501)
	ASSERT(bEnable);
	ASSERT(strGroups.GetLength() > 0);
	ASSERT(strCancelGroups.GetLength() > 0);
	m_bEnableCategoryGroups = bEnable;
	m_strCancelGroups = strCancelGroups;
	m_strGroupBy = strGroups;
	m_bAutoHide = bAutoHide;
#endif
}


LRESULT CListCtrl2::InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader, DWORD dwState /* = LVGS_NORMAL */, DWORD dwAlign /*= LVGA_HEADER_LEFT*/)
{
#if (_WIN32_WINNT >= 0x501)
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupID;
	lg.state = dwState;
	lg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lg.uAlign = dwAlign;

	// Header-title must be unicode (Convert if necessary)
#ifdef UNICODE
	lg.pszHeader = strHeader.GetBuffer();
	lg.cchHeader = strHeader.GetLength();
#else
	CComBSTR header = strHeader;
	lg.pszHeader = header;
	lg.cchHeader = header.Length();
#endif
	return InsertGroup(nIndex, (PLVGROUP)&lg );
#else
	return 0;
#endif
}

BOOL CListCtrl2::SetRowGroupId(int nRow, int nGroupID)
{
#if (_WIN32_WINNT >= 0x501)
	//OBS! Rows not assigned to a group will not show in group-view
	LVITEM lvItem = {0};
	lvItem.mask = LVIF_GROUPID;
	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.iGroupId = nGroupID;
	return SetItem( &lvItem );
#else
	return FALSE;
#endif
}

int CListCtrl2::GetRowGroupId(int nRow)
{
#if (_WIN32_WINNT >= 0x501)
	LVITEM lvi = {0};
	lvi.mask = LVIF_GROUPID;
	lvi.iItem = nRow;
	GetItem(&lvi);
	return lvi.iGroupId;
#else
	return -1;
#endif
}

CString CListCtrl2::GetGroupHeader(int nGroupID)
{
#if (_WIN32_WINNT >= 0x501)
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupID;
	lg.mask = LVGF_HEADER | LVGF_GROUPID;
	GetGroupInfo(nGroupID, (PLVGROUP)&lg);
#ifdef UNICODE
	return lg.pszHeader;
#else
	CComBSTR header( lg.pszHeader );
	return (LPCTSTR)COLE2T(header);
#endif
#else
	return "";
#endif
}

BOOL CListCtrl2::IsGroupStateEnabled()
{
#if (_WIN32_WINNT >= 0x0501)
	if (!IsGroupViewEnabled())
		return FALSE;

	OSVERSIONINFO osver = {0};
	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx(&osver);
	WORD fullver = MAKEWORD(osver.dwMinorVersion, osver.dwMajorVersion);
	if (fullver < 0x0600)
		return FALSE;
	return TRUE;
#else
	return FALSE;
#endif
}

// Vista SDK - ListView_GetGroupState / LVM_GETGROUPSTATE
BOOL CListCtrl2::HasGroupState(int nGroupID, DWORD dwState)
{
#if (_WIN32_WINNT >= 0x501)
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_STATE;
	lg.stateMask = dwState;
	if ( GetGroupInfo(nGroupID, (PLVGROUP)&lg) == -1)
		return FALSE;
	return lg.state == dwState;
#else
	return FALSE;
#endif
}

// Vista SDK - ListView_SetGroupState / LVM_SETGROUPINFO
BOOL CListCtrl2::SetGroupState(int nGroupID, DWORD dwState)
{
#if (_WIN32_WINNT >= 0x501)
	if (!IsGroupStateEnabled())
		return FALSE;

	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_STATE;
	lg.state = dwState;
	lg.stateMask = dwState;

#ifdef LVGS_COLLAPSIBLE
	if (HasGroupState(nGroupID, LVGS_COLLAPSIBLE))
		lg.state |= LVGS_COLLAPSIBLE;
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;
	return TRUE;
#else
	return FALSE;
#endif
}

int CListCtrl2::GroupHitTest(const CPoint& point)
{
#if (_WIN32_WINNT >= 0x501)
	if (!IsGroupViewEnabled())
		return -1;

	if (HitTest(point)!=-1)
		return -1;

	if (IsGroupStateEnabled())
	{
#ifdef ListView_HitTestEx
#ifdef LVHT_EX_GROUP_HEADER
		LVHITTESTINFO lvhitinfo = {0};
		lvhitinfo.pt = point;
		ListView_HitTestEx(m_hWnd, &lvhitinfo);
		if ((lvhitinfo.flags & LVHT_EX_GROUP)==0)
			return -1;
#endif
#endif

#ifdef ListView_GetGroupCount
#ifdef ListView_GetGroupRect
#ifdef ListView_GetGroupInfoByIndex
		bool foundGroup = false;
		LRESULT groupCount = ListView_GetGroupCount(m_hWnd);
		if (groupCount <= 0)
			return -1;
		for(int i = 0 ; i < groupCount; ++i)
		{
			LVGROUP lg = {0};
			lg.cbSize = sizeof(lg);
			lg.mask = LVGF_GROUPID;
			ListView_GetGroupInfoByIndex(m_hWnd, i, &lg) ;

			CRect rect(0,0,0,0);
			ListView_GetGroupRect(m_hWnd, lg.iGroupId, 0, &rect) ;

			if (rect.PtInRect(point))
				return lg.iGroupId;
		}
		// Don't try other ways to find the group
		if (groupCount > 0)
			return -1;
#endif
#endif
#endif
	}	// IsGroupStateEnabled()

	// We require that each group contains atleast one item
	if (GetItemCount()==0)
		return -1;

	CRect gridRect(0,0,0,0);
	GetClientRect(&gridRect);

	int nRowAbove = -1, nRowBelow = 0;
	for(nRowBelow = GetTopIndex(); nRowBelow < GetItemCount(); nRowBelow++)
	{
		GetRowGroupId(nRowBelow);

		CRect rectRowBelow;
		if (GetItemRect(nRowBelow, rectRowBelow, LVIR_BOUNDS)==FALSE)
			continue;	// Found invisible row

		rectRowBelow.right = gridRect.right;
		if (rectRowBelow.PtInRect(point))
			return -1;	// Hit a row
		if (rectRowBelow.top > point.y)
			break;		// Found row just below the point

		nRowAbove = nRowBelow;
	}

	if (nRowBelow < GetItemCount())
	{
		// Probably hit the group just above this row
		return GetRowGroupId(nRowBelow);
	}
#endif
	return -1;
}

void CListCtrl2::CheckEntireGroup(int nGroupId, bool bChecked)
{
#if (_WIN32_WINNT >= 0x501)
	for (int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		if (GetRowGroupId(nRow) == nGroupId)
		{
			SetCheck(nRow, bChecked ? TRUE : FALSE);
		}
	}
#endif
}
void CListCtrl2::CancelGroups()
{
#if (_WIN32_WINNT >= 0x501)
	RemoveAllGroups();
	EnableGroupView(FALSE);
	if (m_bAutoHide && m_nCurrentGroupColumn != -1)
		ShowColumn(m_nCurrentGroupColumn, true);
	m_nCurrentGroupColumn = -1;
#endif
}
void CListCtrl2::DeleteEntireGroup(int nGroupId)
{
#if (_WIN32_WINNT >= 0x501)
	for (int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		if (GetRowGroupId(nRow) == nGroupId)
		{
			DeleteItem(nRow);
			nRow--;
		}
	}
	RemoveGroup(nGroupId);
#endif
}

BOOL CListCtrl2::GroupByColumn(int nCol)
{
#if (_WIN32_WINNT >= 0x501)
	if (m_nSortedCol != -1)
		RemoveSortImage(m_nSortedCol, m_bSort);
	//显示前一列
	if (m_bAutoHide && m_nCurrentGroupColumn != -1)
		ShowColumn(m_nCurrentGroupColumn, TRUE);

	m_nCurrentGroupColumn = nCol;

	//隐藏现在列
	if (m_bAutoHide)
		ShowColumn(nCol, FALSE);
	RemoveAllGroups();

	EnableGroupView( GetItemCount() > 0 );

	if (IsGroupViewEnabled())
	{
		CSimpleMap<CString,CSimpleArray<int> > groups;

		// Loop through all rows and find possible groups
		for(int nRow=0; nRow<GetItemCount(); ++nRow)
		{
			const CString& cellText = GetItemText(nRow, nCol);

			int nGroupId = groups.FindKey(cellText);
			if (nGroupId==-1)
			{
				CSimpleArray<int> rows;
				rows.Add(nRow);
				groups.Add(cellText, rows);
			}
			else
				groups.GetValueAt(nGroupId).Add(nRow);
		}

		// Look through all groups and assign rows to group
		for(int nGroupId = 0; nGroupId < groups.GetSize(); ++nGroupId)
		{
			const CSimpleArray<int>& groupRows = groups.GetValueAt(nGroupId);
			DWORD dwState = LVGS_NORMAL;

#ifdef LVGS_COLLAPSIBLE
			if (IsGroupStateEnabled())
				dwState = LVGS_COLLAPSIBLE;
#endif

			InsertGroupHeader(nGroupId, nGroupId, groups.GetKeyAt(nGroupId), dwState);
			SetGroupTask(nGroupId, _T("Task: Check Group"));
			CString subtitle;
			subtitle.Format(_T("Subtitle: %i rows"), groupRows.GetSize());
			SetGroupSubtitle(nGroupId, subtitle );
			SetGroupFooter(nGroupId, _T("Group Footer"));

			for(int groupRow = 0; groupRow < groupRows.GetSize(); ++groupRow)
			{
				SetRowGroupId(groupRows[groupRow], nGroupId);
			}
		}
		SortGroup(nCol, TRUE);
		return TRUE;
	}
#endif
	return FALSE;
}

void CListCtrl2::CollapseAllGroups()
{
	// Loop through all rows and find possible groups
	for(int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		int nGroupId = GetRowGroupId(nRow);
		if (nGroupId!=-1)
		{
			if (!HasGroupState(nGroupId,LVGS_COLLAPSED))
			{
				SetGroupState(nGroupId,LVGS_COLLAPSED);
			}
		}
	}
}

void CListCtrl2::ExpandAllGroups()
{
	// Loop through all rows and find possible groups
	for(int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		int nGroupId = GetRowGroupId(nRow);
		if (nGroupId!=-1)
		{
			if (HasGroupState(nGroupId,LVGS_COLLAPSED))
			{
				SetGroupState(nGroupId,LVGS_NORMAL);
			}
		}
	}
}


BOOL CListCtrl2::SetGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign /*= LVGA_FOOTER_CENTER*/)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_FOOTER;
#ifdef UNICODE
	lg.pszFooter = footer.GetBuffer();
	lg.cchFooter = footer.GetLength();
#else
	CComBSTR bstrFooter = footer;
	lg.pszFooter = bstrFooter;
	lg.cchFooter = bstrFooter.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl2::SetGroupTask(int nGroupID, const CString& task)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_TASK;
#ifdef UNICODE
	lg.pszTask = task.GetBuffer();
	lg.cchTask = task.GetLength();
#else
	CComBSTR bstrTask = task;
	lg.pszTask = bstrTask;
	lg.cchTask = bstrTask.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl2::SetGroupSubtitle(int nGroupID, const CString& subtitle)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_SUBTITLE;
#ifdef UNICODE
	lg.pszSubtitle = subtitle.GetBuffer();
	lg.cchSubtitle = subtitle.GetLength();
#else
	CComBSTR bstrSubtitle = subtitle;
	lg.pszSubtitle = bstrSubtitle;
	lg.cchSubtitle = bstrSubtitle.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl2::SetGroupTitleImage(int nGroupID, int nImage, const CString& topDesc, const CString& bottomDesc)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_TITLEIMAGE;
	lg.iTitleImage = nImage;	// Index of the title image in the control imagelist.

#ifdef UNICODE
	if (!topDesc.IsEmpty())
	{
		// Top description is drawn opposite the title image when there is
		// a title image, no extended image, and uAlign==LVGA_HEADER_CENTER.
		lg.mask |= LVGF_DESCRIPTIONTOP;
		lg.pszDescriptionTop = topDesc;
		lg.cchDescriptionTop = topDesc.GetLength();
	}
	if (!bottomDesc.IsEmpty())
	{
		// Bottom description is drawn under the top description text when there is
		// a title image, no extended image, and uAlign==LVGA_HEADER_CENTER.
		lg.mask |= LVGF_DESCRIPTIONBOTTOM;
		lg.pszDescriptionBottom = bottomDesc;
		lg.cchDescriptionBottom = bottomDesc.GetLength();
	}
#else
	CComBSTR bstrTopDesc = topDesc;
	CComBSTR bstrBottomDesc = bottomDesc;
	if (!topDesc.IsEmpty())
	{
		lg.mask |= LVGF_DESCRIPTIONTOP;
		lg.pszDescriptionTop = bstrTopDesc;
		lg.cchDescriptionTop = bstrTopDesc.Length();
	}
	if (!bottomDesc.IsEmpty())
	{
		lg.mask |= LVGF_DESCRIPTIONBOTTOM;
		lg.pszDescriptionBottom = bstrBottomDesc;
		lg.cchDescriptionBottom = bstrBottomDesc.Length();
	}
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl2::OnGroupTaskClick(NMHDR* pNMHDR, LRESULT* pResult)
{
#if _WIN32_WINNT >= 0x0600
	NMLVLINK* pLinkInfo = (NMLVLINK*)pNMHDR;
	int nGroupId = pLinkInfo->iSubItem;
	CheckEntireGroup(nGroupId, true);
#endif
	return FALSE;
}
void CListCtrl2::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMeasureItemStruct->itemHeight = 10;
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
BOOL CListCtrl2::ColumnCanNOTGroup(int nColumn)
{
	for (int t = 0; t < m_NoGroupColumnAry.GetCount(); t++)
	{
		if (m_NoGroupColumnAry.GetAt(t) == nColumn)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CListCtrl2::HitTest(CPoint pt, UINT* pFlags)
{
	ASSERT(::IsWindow(m_hWnd));
	LVHITTESTINFO hti;
	hti.pt = pt;
	int nRes = HitTest(&hti);
	if (pFlags != NULL)
		*pFlags = hti.flags;
	return nRes;
}
int CListCtrl2::HitTest(LVHITTESTINFO* pHitTestInfo)
{
	ASSERT(::IsWindow(m_hWnd));
	int nRes = (int) ::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)pHitTestInfo);
	CRect rc;
	if (GetStyle() & LVS_OWNERDRAWFIXED)
	{
		if (pHitTestInfo->flags & LVHT_ONITEM)
		{
			GetSubItemRect(nRes, 0, LVIR_BOUNDS, rc);
			rc.right = rc.left + 16;
			if (PtInRect(&rc, pHitTestInfo->pt))
			{
				if (GetImageList(LVSIL_STATE) != NULL)
				{
					pHitTestInfo->flags = LVHT_ONITEMSTATEICON;
				}
			}
		}
	}
	return nRes;
}
int CListCtrl2::GetFirstSelectedItem()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos)
	{
		return GetNextSelectedItem(pos);
	}
	return -1;
}