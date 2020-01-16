#pragma once

#include "../iPrintableDocumentDeal/Image/ImageDeal.h"
#include "../iPrintableDocumentDeal/MsOffice/WordOperate.h"
#include "../iPrintableDocumentDeal/MsOffice/ExcelOperate.h"
#include "../iPrintableDocumentDeal/MsOffice/PPTOperate.h"
#include "../iPrintableDocumentDeal/PDF/PdfWrap.h"
#include "../iPrintableDocumentDeal/PrintableDocumentDeal.h"

#ifndef Use_PrintableDocumentDeal
#define Use_PrintableDocumentDeal	1
#endif

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\iPrintableDocumentDeal.lib")
#else
#pragma comment(lib,"..\\Release\\iPrintableDocumentDeal.lib")
#endif

