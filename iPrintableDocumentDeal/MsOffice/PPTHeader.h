#pragma once

// #include "MSOfficeHeader.h"
// 
// // Office PPT 2010 lib
// #if 0	//编译会出错，要示编译再次才成功，并且使用导出类的项目会编译不通过
// #import "../Lib/Office2010/MSPPT.OLB" \
// 	no_namespace raw_interfaces_only \
// 	rename("RGB", "PPT_RGB")
// #else	//msppt.tlh是上面#import生成的文件
// #include "../Lib/Office2010/msppt.tlh"
// #endif


//将下列包含的头文件注释import
// #import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\MSPPT.OLB" no_namespace
//可以包含PPT2010下的所有头文件，目前只包含下面用到的头文件
#include "PPT2010/CApplication1.h"
#include "PPT2010/CPresentations.h"
#include "PPT2010/CPresentation.h"
#include "PPT2010/CProtectedViewWindow.h"
#include "PPT2010/CProtectedViewWindows.h"


//下面的枚举类型是从[Lib/Office2010/msppt.tlh]和[Lib/Office2010/mso.tlh]截取出来的,后续可以将所有的enum截取出来，不需要#import任何MSPPT.OLB
enum PpSaveAsFileType
{
	ppSaveAsPresentation = 1,
	ppSaveAsPowerPoint7 = 2,
	ppSaveAsPowerPoint4 = 3,
	ppSaveAsPowerPoint3 = 4,
	ppSaveAsTemplate = 5,
	ppSaveAsRTF = 6,
	ppSaveAsShow = 7,
	ppSaveAsAddIn = 8,
	ppSaveAsPowerPoint4FarEast = 10,
	ppSaveAsDefault = 11,
	ppSaveAsHTML = 12,
	ppSaveAsHTMLv3 = 13,
	ppSaveAsHTMLDual = 14,
	ppSaveAsMetaFile = 15,
	ppSaveAsGIF = 16,
	ppSaveAsJPG = 17,
	ppSaveAsPNG = 18,
	ppSaveAsBMP = 19,
	ppSaveAsWebArchive = 20,
	ppSaveAsTIF = 21,
	ppSaveAsPresForReview = 22,
	ppSaveAsEMF = 23,
	ppSaveAsOpenXMLPresentation = 24,
	ppSaveAsOpenXMLPresentationMacroEnabled = 25,
	ppSaveAsOpenXMLTemplate = 26,
	ppSaveAsOpenXMLTemplateMacroEnabled = 27,
	ppSaveAsOpenXMLShow = 28,
	ppSaveAsOpenXMLShowMacroEnabled = 29,
	ppSaveAsOpenXMLAddin = 30,
	ppSaveAsOpenXMLTheme = 31,
	ppSaveAsPDF = 32,
	ppSaveAsXPS = 33,
	ppSaveAsXMLPresentation = 34,
	ppSaveAsOpenDocumentPresentation = 35,
	ppSaveAsOpenXMLPicturePresentation = 36,
	ppSaveAsWMV = 37,
	ppSaveAsExternalConverter = 64000
};

enum PpFixedFormatType
{
	ppFixedFormatTypeXPS = 1,
	ppFixedFormatTypePDF = 2
};

enum PpFixedFormatIntent
{
	ppFixedFormatIntentScreen = 1,
	ppFixedFormatIntentPrint = 2
};

enum PpPrintHandoutOrder
{
	ppPrintHandoutVerticalFirst = 1,
	ppPrintHandoutHorizontalFirst = 2
};

enum PpPrintOutputType
{
	ppPrintOutputSlides = 1,
	ppPrintOutputTwoSlideHandouts = 2,
	ppPrintOutputThreeSlideHandouts = 3,
	ppPrintOutputSixSlideHandouts = 4,
	ppPrintOutputNotesPages = 5,
	ppPrintOutputOutline = 6,
	ppPrintOutputBuildSlides = 7,
	ppPrintOutputFourSlideHandouts = 8,
	ppPrintOutputNineSlideHandouts = 9,
	ppPrintOutputOneSlideHandouts = 10
};

enum PpPrintRangeType
{
	ppPrintAll = 1,
	ppPrintSelection = 2,
	ppPrintCurrent = 3,
	ppPrintSlideRange = 4,
	ppPrintNamedSlideShow = 5,
	ppPrintSection = 6
};

//参考mso.tlh
enum MsoTriState
{
	msoTrue = -1,
	msoFalse = 0,
	msoCTrue = 1,
	msoTriStateToggle = -3,
	msoTriStateMixed = -2
};
