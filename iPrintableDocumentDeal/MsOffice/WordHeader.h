#pragma once

// Office Word 2010 lib
// #if 0	//编译会出错，要示编译再次才成功，并且使用导出类的项目会编译不通过
// #import "../Lib/Office2010/MSWORD.OLB" \
// 	no_namespace raw_interfaces_only \
// 	rename("FindText", "Word_FindText") \
// 	rename("Rectangle", "Word_Rectangle") \
// 	rename("ExitWindows","Word_ExitWindows")
// #else	//msword.tlh是上面#import生成的文件
// #include "../Lib/Office2010/msword.tlh"
// #endif


//将下列包含的头文件注释import
// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSWORD.OLB" no_namespace
//可以包含Word2010下的所有头文件，目前只包含下面用到的头文件
#include "Word2010/CApplication.h"	// word程序对象
#include "Word2010/CDocuments.h"	// 文档集对象
#include "Word2010/CDocument0.h"	// 单个docx对象
#include "Word2010/CSelection.h"	// 该对象代表窗口或窗格中的当前所选内容，使用率最高
#include "Word2010/CCell.h"		// 表格单元格
#include "Word2010/CCells.h"	// 表格单元格集合
#include "Word2010/CRange.h"	// 该对象代表文档中的一个连续范围
#include "Word2010/CTable0.h"	// 单个表格
#include "Word2010/CTables0.h"	// 表格集合
#include "Word2010/CFont0.h"	// 字体
#include "Word2010/CParagraphs.h"	// 段落集合
#include "Word2010/CParagraphFormat.h"	// 段落样式
#include "Word2010/CParagraph.h"	// 单个段落
#include "Word2010/CnlineShape.h"	// Inline图形对象集合
#include "Word2010/CnlineShapes.h"	// 单个Inline图像对象
#include "Word2010/CRow.h"	// 单个行
#include "Word2010/CRows.h"	// 行集合
#include "Word2010/CFields.h"
#include "Word2010/CPane.h"	// 页眉等设置
#include "Word2010/CWindow0.h"// 页眉等设置
#include "Word2010/CView0.h"// 页眉等设置
#include "Word2010/CPageSetup.h"	// 页面设置


//下面的枚举类型是从[Lib/Office2010/msword.tlh]截取出来的,后续可以将所有的enum截取出来，不需要#import任何MSWORD.OLB
enum WdSaveFormat
{
	wdFormatDocument = 0,
	wdFormatDocument97 = 0,
	wdFormatTemplate = 1,
	wdFormatTemplate97 = 1,
	wdFormatText = 2,
	wdFormatTextLineBreaks = 3,
	wdFormatDOSText = 4,
	wdFormatDOSTextLineBreaks = 5,
	wdFormatRTF = 6,
	wdFormatUnicodeText = 7,
	wdFormatEncodedText = 7,
	wdFormatHTML = 8,
	wdFormatWebArchive = 9,
	wdFormatFilteredHTML = 10,
	wdFormatXML = 11,
	wdFormatXMLDocument = 12,
	wdFormatXMLDocumentMacroEnabled = 13,
	wdFormatXMLTemplate = 14,
	wdFormatXMLTemplateMacroEnabled = 15,
	wdFormatDocumentDefault = 16,
	wdFormatPDF = 17,
	wdFormatXPS = 18,
	wdFormatFlatXML = 19,
	wdFormatFlatXMLMacroEnabled = 20,
	wdFormatFlatXMLTemplate = 21,
	wdFormatFlatXMLTemplateMacroEnabled = 22,
	wdFormatOpenDocumentText = 23
};

enum WdMovementType
{
	wdMove = 0,
	wdExtend = 1
};

enum WdExportFormat
{
	wdExportFormatPDF = 17,
	wdExportFormatXPS = 18
};

enum WdExportOptimizeFor
{
	wdExportOptimizeForPrint = 0,
	wdExportOptimizeForOnScreen = 1
};

enum WdExportCreateBookmarks
{
	wdExportCreateNoBookmarks = 0,
	wdExportCreateHeadingBookmarks = 1,
	wdExportCreateWordBookmarks = 2
};

enum WdExportItem
{
	wdExportDocumentContent = 0,
	wdExportDocumentWithMarkup = 7
};

enum WdExportRange
{
	wdExportAllDocument = 0,
	wdExportSelection = 1,
	wdExportCurrentPage = 2,
	wdExportFromTo = 3
};
