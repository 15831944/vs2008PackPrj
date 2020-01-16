#pragma once



/* paper selections */
// #define DMPAPER_FIRST                DMPAPER_LETTER
// #define DMPAPER_LETTER               1  /* Letter 8 1/2 x 11 in               */

static TCHAR *pagerSizetab[] = {
		_T("FIRST"),
		_T("Letter"),				/* Letter 8 1/2 x 11 in               */
		_T("Letter Small"),/* Letter Small 8 1/2 x 11 in         */
		_T("Tabloid"),/* Tabloid 11 x 17 in                 */
		_T("Ledger"),/* Ledger 17 x 11 in                  */
		_T("Legal"),/* Legal 8 1/2 x 14 in                */
		_T("Statement"),/* Statement 5 1/2 x 8 1/2 in         */
		_T("Executive"),/* Executive 7 1/4 x 10 1/2 in        */
		_T("A3"),/* A3 297 x 420 mm                    */
		_T("A4"),/* A4 210 x 297 mm                    */
		_T("A4 Small"),/* A4 Small 210 x 297 mm              */
		_T("A5"),/* A5 148 x 210 mm                    */
		_T("B4 (JIS)"),/* B4 (JIS) 250 x 354                 */
		_T("B5 (JIS)"),/* B5 (JIS) 182 x 257 mm              */
		_T("Folio"),/* Folio 8 1/2 x 13 in                */
		_T("Quarto"),/* Quarto 215 x 275 mm                */
		_T("10x14"),  /* 10x14 in                           */
		_T("11x17"),  /* 11x17 in                           */
		_T("Note"),  /* Note 8 1/2 x 11 in                 */
		_T("Envelope #9"),  /* Envelope #9 3 7/8 x 8 7/8          */
		_T("Envelope #10"),  /* Envelope #10 4 1/8 x 9 1/2         */
		_T("Envelope #11"),  /* Envelope #11 4 1/2 x 10 3/8        */
		_T("Envelope #12"),  /* Envelope #12 4 \276 x 11           */
		_T("Envelope #14"),  /* Envelope #14 5 x 11 1/2            */
		_T("C size sheet"),  /* C size sheet                       */
		_T("D size sheet"),  /* D size sheet                       */
		_T("E size sheet"),  /* E size sheet                       */
		_T("Envelope DL"),  /* Envelope DL 110 x 220mm            */
		_T("Envelope C5"),  /* Envelope C5 162 x 229 mm           */
		_T("Envelope C3"),  /* Envelope C3  324 x 458 mm          */
		_T("Envelope C4"),  /* Envelope C4  229 x 324 mm          */
		_T("Envelope C6"),  /* Envelope C6  114 x 162 mm          */
		_T("Envelope C65"),  /* Envelope C65 114 x 229 mm          */
		_T("Envelope B4"),  /* Envelope B4  250 x 353 mm          */
		_T("Envelope B5"),  /* Envelope B5  176 x 250 mm          */
		_T("Envelope B6"),  /* Envelope B6  176 x 125 mm          */
		_T("Envelope"),  /* Envelope 110 x 230 mm              */
		_T("Envelope Monarch"),  /* Envelope Monarch 3.875 x 7.5 in    */
		_T("Envelope Personal"),  /* 6 3/4 Envelope 3 5/8 x 6 1/2 in    */
		_T("US Std Fanfold"),  /* US Std Fanfold 14 7/8 x 11 in      */
		_T("German Std Fanfold"),  /* German Std Fanfold 8 1/2 x 12 in   */
		_T("German Legal Fanfold"),  /* German Legal Fanfold 8 1/2 x 13 in */
		_T("B4 (ISO)"),  /* B4 (ISO) 250 x 353 mm              */
		_T("Japanese Postcard"),  /* Japanese Postcard 100 x 148 mm     */
		_T("9 x 11"),  /* 9 x 11 in                          */
		_T("10 x 11"),  /* 10 x 11 in                         */
		_T("15 x 11"),  /* 15 x 11 in                         */
		_T("Envelope Invite"),  /* Envelope Invite 220 x 220 mm       */
		_T("RESERVED_48"),  /* RESERVED--DO NOT USE               */
		_T("RESERVED_49"),  /* RESERVED--DO NOT USE               */
		_T("Letter Extra"),  /* Letter Extra 9 \275 x 12 in        */
		_T("Legal Extra"),  /* Legal Extra 9 \275 x 15 in         */
		_T("Tabloid Extra"),  /* Tabloid Extra 11.69 x 18 in        */
		_T("A4 Extra"),  /* A4 Extra 9.27 x 12.69 in           */
		_T("Letter Transverse"),  /* Letter Transverse 8 \275 x 11 in   */
		_T("A4 Transverse"),  /* A4 Transverse 210 x 297 mm         */
		_T("Letter Extra Transverse"), /* Letter Extra Transverse 9\275 x 12 in */
		_T("Super A"),  /* SuperA/SuperA/A4 227 x 356 mm      */
		_T("Super B"),  /* SuperB/SuperB/A3 305 x 487 mm      */
		_T("Letter Plus"),  /* Letter Plus 8.5 x 12.69 in         */
		_T("A4 Plus"),  /* A4 Plus 210 x 330 mm               */
		_T("A5 Transverse"),  /* A5 Transverse 148 x 210 mm         */
		_T("B5 (JIS) Transverse"),  /* B5 (JIS) Transverse 182 x 257 mm   */
		_T("A3 Extra"),  /* A3 Extra 322 x 445 mm              */
		_T("A5 Extra"),  /* A5 Extra 174 x 235 mm              */
		_T("B5 (ISO) Extra"),  /* B5 (ISO) Extra 201 x 276 mm        */
		_T("A2"),  /* A2 420 x 594 mm                    */
		_T("A3 Transverse"),  /* A3 Transverse 297 x 420 mm         */
		_T("A3 Extra Transverse"),  /* A3 Extra Transverse 322 x 445 mm   */
		_T("Japanese Double Postcard"), /* Japanese Double Postcard 200 x 148 mm */
		_T("A6"),  /* A6 105 x 148 mm                 */
		_T("Japanese Envelope Kaku #2"),  /* Japanese Envelope Kaku #2       */
		_T("Japanese Envelope Kaku #3"),  /* Japanese Envelope Kaku #3       */
		_T("Japanese Envelope Chou #3"),  /* Japanese Envelope Chou #3       */
		_T("Japanese Envelope Chou #4"),  /* Japanese Envelope Chou #4       */
		_T("Letter Rotated"),  /* Letter Rotated 11 x 8 1/2 11 in */
		_T("A3 Rotated"),  /* A3 Rotated 420 x 297 mm         */
		_T("A4 Rotated"),  /* A4 Rotated 297 x 210 mm         */
		_T("A5 Rotated"),  /* A5 Rotated 210 x 148 mm         */
		_T("B4 (JIS) Rotated"),  /* B4 (JIS) Rotated 364 x 257 mm   */
		_T("B5 (JIS) Rotated"),  /* B5 (JIS) Rotated 257 x 182 mm   */
		_T("Japanese Postcard Rotated"), /* Japanese Postcard Rotated 148 x 100 mm */
		_T("Double Japanese Postcard Rotated"), /* Double Japanese Postcard Rotated 148 x 200 mm */
		_T("A6 Rotated"),  /* A6 Rotated 148 x 105 mm         */
		_T("Japanese Envelope Kaku #2 Rotated"),  /* Japanese Envelope Kaku #2 Rotated */
		_T("Japanese Envelope Kaku #3 Rotated"),  /* Japanese Envelope Kaku #3 Rotated */
		_T("Japanese Envelope Chou #3 Rotated"),  /* Japanese Envelope Chou #3 Rotated */
		_T("Japanese Envelope Chou #4 Rotated"),  /* Japanese Envelope Chou #4 Rotated */
		_T("B6 (JIS)"),  /* B6 (JIS) 128 x 182 mm           */
		_T("B6 (JIS) Rotated"),  /* B6 (JIS) Rotated 182 x 128 mm   */
		_T("12 x 11"),  /* 12 x 11 in                      */
		_T("Japanese Envelope You #4 "),  /* Japanese Envelope You #4        */
		_T("Japanese Envelope You #4 Rotated"),  /* Japanese Envelope You #4 Rotated*/
		_T("PRC 16K"),  /* PRC 16K 146 x 215 mm            */
		_T("PRC 32K"),  /* PRC 32K 97 x 151 mm             */
		_T("PRC 32K(Big)"),  /* PRC 32K(Big) 97 x 151 mm        */
		_T("PRC Envelope #1"),  /* PRC Envelope #1 102 x 165 mm    */
		_T("PRC Envelope #2"),  /* PRC Envelope #2 102 x 176 mm    */
		_T("PRC Envelope #3"),  /* PRC Envelope #3 125 x 176 mm    */
		_T("PRC Envelope #4"),  /* PRC Envelope #4 110 x 208 mm    */
		_T("PRC Envelope #5"), /* PRC Envelope #5 110 x 220 mm    */
		_T("PRC Envelope #6"), /* PRC Envelope #6 120 x 230 mm    */
		_T("PRC Envelope #7"), /* PRC Envelope #7 160 x 230 mm    */
		_T("PRC Envelope #8"), /* PRC Envelope #8 120 x 309 mm    */
		_T("PRC Envelope #9"), /* PRC Envelope #9 229 x 324 mm    */
		_T("PRC Envelope #10"), /* PRC Envelope #10 324 x 458 mm   */
		_T("PRC 16K Rotated"), /* PRC 16K Rotated                 */
		_T("PRC 32K Rotated"), /* PRC 32K Rotated                 */
		_T("PRC 32K(Big) Rotated"), /* PRC 32K(Big) Rotated            */
		_T("PRC Envelope #1 Rotated"), /* PRC Envelope #1 Rotated 165 x 102 mm */
		_T("PRC Envelope #2 Rotated"), /* PRC Envelope #2 Rotated 176 x 102 mm */
		_T("PRC Envelope #3 Rotated"), /* PRC Envelope #3 Rotated 176 x 125 mm */
		_T("PRC Envelope #4 Rotated"), /* PRC Envelope #4 Rotated 208 x 110 mm */
		_T("PRC Envelope #5 Rotated"), /* PRC Envelope #5 Rotated 220 x 110 mm */
		_T("PRC Envelope #6 Rotated"), /* PRC Envelope #6 Rotated 230 x 120 mm */
		_T("PRC Envelope #7 Rotated"), /* PRC Envelope #7 Rotated 230 x 160 mm */
		_T("PRC Envelope #8 Rotated"), /* PRC Envelope #8 Rotated 309 x 120 mm */
		_T("PRC Envelope #9 Rotated"), /* PRC Envelope #9 Rotated 324 x 229 mm */
		_T("PRC Envelope #10 Rotated"), /* PRC Envelope #10 Rotated 458 x 324 mm */
		_T("LAST")			//DMPAPER_PENV_10_ROTATED
};

//长度和宽度的单位是：十分之一毫米（0.1mm）
#define Paper_Size_Amend_Value	1	//修正实际大小的数值
static int pagerSizetab2[] = {
		0,
		2159*2794,	/* Letter 8 1/2 x 11 in               */
		2159*2794,	/* Letter Small 8 1/2 x 11 in         */
		2794*4318,	/* Tabloid 11 x 17 in                 */
		4318*2794,	/* Ledger 17 x 11 in                  */
		2159*3556,	/* Legal 8 1/2 x 14 in                */
		1397*2159,	/* Statement 5 1/2 x 8 1/2 in         */
		1841*2667,	/* Executive 7 1/4 x 10 1/2 in        */
		2970*4200,	/* A3 297 x 420 mm                    */
		2100*2970,	/* A4 210 x 297 mm                    */
		2100*2970,	/* A4 Small 210 x 297 mm              */
		1480*2100,	/* A5 148 x 210 mm                    */
#ifndef Paper_Size_Amend_Value
		2500*3540,	/* B4 (JIS) 250 x 354                 */
#else
		2570*3640,	/* B4 (JIS) 257 x 364                 */
#endif
		1820*2570,	/* B5 (JIS) 182 x 257 mm              */
		2159*3302,	/* Folio 8 1/2 x 13 in                */
		2150*2750,	/* Quarto 215 x 275 mm                */
		2540*3556,  /* 10x14 in                           */
		2794*4318,  /* 11x17 in                           */
		2159*2794,  /* Note 8 1/2 x 11 in                 */
		984*2254,  /* Envelope #9 3 7/8 x 8 7/8           */
		1084*2413,  /* Envelope #10 4 1/8 x 9 1/2         */
		1143*2635,  /* Envelope #11 4 1/2 x 10 3/8        */
		1206*2794,  /* Envelope #12 4 \276 x 11           */
		1270*2921,  /* Envelope #14 5 x 11 1/2            */
		4318*5588,  /* C size sheet  17 x 22 in           */
		5588*8636,  /* D size sheet   22 x 34 in          */
		8636*11176,  /* E size sheet    34 x 44 in        */
		1100*2200,  /* Envelope DL 110 x 220mm            */
		1620*2290,  /* Envelope C5 162 x 229 mm           */
		3240*4580,  /* Envelope C3  324 x 458 mm          */
		2290*3240,  /* Envelope C4  229 x 324 mm          */
		1140*1620,  /* Envelope C6  114 x 162 mm          */
		1140*2290,  /* Envelope C65 114 x 229 mm          */
		2500*3530,  /* Envelope B4  250 x 353 mm          */
		1760*2500,  /* Envelope B5  176 x 250 mm          */
		1760*1250,  /* Envelope B6  176 x 125 mm          */
		1100*2300,  /* Envelope 110 x 230 mm              */
		984*1905,   /* Envelope Monarch 3.875 x 7.5 in    */
		921*1651,   /* 6 3/4 Envelope 3 5/8 x 6 1/2 in    */
		3778*2794,  /* US Std Fanfold 14 7/8 x 11 in      */
		2159*3048,  /* German Std Fanfold 8 1/2 x 12 in   */
		2159*3302,  /* German Legal Fanfold 8 1/2 x 13 in */
		2500*3530,  /* B4 (ISO) 250 x 353 mm              */
		1000*1480,  /* Japanese Postcard 100 x 148 mm     */
		2286*2794,  /* 9 x 11 in                          */
		2540*2794,  /* 10 x 11 in                         */
		3810*2794,  /* 15 x 11 in                         */
		2200*2200,  /* Envelope Invite 220 x 220 mm       */
		0,  /* RESERVED--DO NOT USE               */
		0,  /* RESERVED--DO NOT USE               */
		2413*3048,  /* Letter Extra 9 \275 x 12 in        */
		2413*3810,  /* Legal Extra 9 \275 x 15 in         */
		2970*4572,  /* Tabloid Extra 11.69 x 18 in        */
		2354*3223,  /* A4 Extra 9.27 x 12.69 in           */
		2159*2794,  /* Letter Transverse 8 \275 x 11 in   */
		2100*2970,  /* A4 Transverse 210 x 297 mm         */
		2413*3048,  /* Letter Extra Transverse 9\275 x 12 in */
		2270*3560,  /* SuperA/SuperA/A4 227 x 356 mm      */
		3050*4870,  /* SuperB/SuperB/A3 305 x 487 mm      */
		2159*3223,  /* Letter Plus 8.5 x 12.69 in         */
		2100*3300,  /* A4 Plus 210 x 330 mm               */
		1480*2100,  /* A5 Transverse 148 x 210 mm         */
		1820*2570,  /* B5 (JIS) Transverse 182 x 257 mm   */
		3220*4450,  /* A3 Extra 322 x 445 mm              */
		1740*2350,  /* A5 Extra 174 x 235 mm              */
		2010*2760,  /* B5 (ISO) Extra 201 x 276 mm        */
		4200*5940,  /* A2 420 x 594 mm                    */
		2970*4200,  /* A3 Transverse 297 x 420 mm         */
		3220*4450,  /* A3 Extra Transverse 322 x 445 mm   */
		2000*1480,  /* Japanese Double Postcard 200 x 148 mm */
		1050*1480,  /* A6 105 x 148 mm                 */
		2400*3320,  /* Japanese Envelope Kaku #2       */
		2160*2770,  /* Japanese Envelope Kaku #3       */
		1200*2350,  /* Japanese Envelope Chou #3       */
		900*2050,   /* Japanese Envelope Chou #4       */
		2794*2159,  /* Letter Rotated 11 x 8 1/2 11 in */
		4200*2970,  /* A3 Rotated 420 x 297 mm         */
		2970*2100,  /* A4 Rotated 297 x 210 mm         */
		2100*1480,  /* A5 Rotated 210 x 148 mm         */
		3640*2570,  /* B4 (JIS) Rotated 364 x 257 mm   */
		2570*1820,  /* B5 (JIS) Rotated 257 x 182 mm   */
		1480*1000,  /* Japanese Postcard Rotated 148 x 100 mm */
		1480*2000,  /* Double Japanese Postcard Rotated 148 x 200 mm */
		1480*1050,  /* A6 Rotated 148 x 105 mm         */
		3320*2400,  /* Japanese Envelope Kaku #2 Rotated */
		2770*2160,  /* Japanese Envelope Kaku #3 Rotated */
		2350*1200,  /* Japanese Envelope Chou #3 Rotated */
		2050*900,   /* Japanese Envelope Chou #4 Rotated */
		1280*1820,  /* B6 (JIS) 128 x 182 mm           */
		1820*1280,  /* B6 (JIS) Rotated 182 x 128 mm   */
		3049*2795,  /* 12 x 11 in                      */
		1050*2350,  /* Japanese Envelope You #4        */
		2350*1050,  /* Japanese Envelope You #4 Rotated*/
#ifndef Paper_Size_Amend_Value
		1460*2150,  /* PRC 16K 146 x 215 mm            */
		970*1510,  /* PRC 32K 97 x 151 mm             */
		970*1510,  /* PRC 32K(Big) 97 x 151 mm        */
#else
		1840*2600,  /* PRC 16K 184 x 260 mm            */
		1300*1840,  /* PRC 32K 130 x 184 mm            */
		1400*2030,  /* PRC 32K(Big) 140 x 203 mm       */
#endif
		1020*1650,  /* PRC Envelope #1 102 x 165 mm    */
		1020*1760,  /* PRC Envelope #2 102 x 176 mm    */
		1250*1760,  /* PRC Envelope #3 125 x 176 mm    */
		1100*2080,  /* PRC Envelope #4 110 x 208 mm    */
		1100*2200,  /* PRC Envelope #5 110 x 220 mm    */
		1200*2300,  /* PRC Envelope #6 120 x 230 mm    */
		1600*2300,  /* PRC Envelope #7 160 x 230 mm    */
		1200*3090,  /* PRC Envelope #8 120 x 309 mm    */
		2290*3240,  /* PRC Envelope #9 229 x 324 mm    */
		3240*4580,  /* PRC Envelope #10 324 x 458 mm   */
#ifndef Paper_Size_Amend_Value
		2150*1460, /* PRC 16K Rotated 215 x 146 mm           */
		1510*970, /* PRC 32K Rotated 151 x 97 mm             */
		1510*970, /* PRC 32K(Big) Rotated  151 x 97 mm       */
#else
		2600*1840,  /* PRC 16K Rotated 260 x 184 mm	*/
		1840*1300,  /* PRC 32K Rotated 184 x 130 mm */
		2030*1400,  /* PRC 32K(Big) Rotated 203 x 140 mm	*/
#endif
		1650*1020,  /* PRC Envelope #1 Rotated 165 x 102 mm */
		1760*1020,  /* PRC Envelope #2 Rotated 176 x 102 mm */
		1760*1250,  /* PRC Envelope #3 Rotated 176 x 125 mm */
		2080*1100,  /* PRC Envelope #4 Rotated 208 x 110 mm */
		2200*1100,  /* PRC Envelope #5 Rotated 220 x 110 mm */
		2300*1200,  /* PRC Envelope #6 Rotated 230 x 120 mm */
		2300*1600,  /* PRC Envelope #7 Rotated 230 x 160 mm */
		3090*1200,  /* PRC Envelope #8 Rotated 309 x 120 mm */
		3240*2290,  /* PRC Envelope #9 Rotated 324 x 229 mm */
		4580*3240,  /* PRC Envelope #10 Rotated 458 x 324 mm */
		0			//DMPAPER_PENV_10_ROTATED
};

//长度和宽度的单位是：十分之一毫米（0.1mm）
static int pagerSizetab3[] = {
	0,0,
	2159,2794,	/* Letter 8 1/2 x 11 in               */
	2159,2794,	/* Letter Small 8 1/2 x 11 in         */
	2794,4318,	/* Tabloid 11 x 17 in                 */
	4318,2794,	/* Ledger 17 x 11 in                  */
	2159,3556,	/* Legal 8 1/2 x 14 in                */
	1397,2159,	/* Statement 5 1/2 x 8 1/2 in         */
	1841,2667,	/* Executive 7 1/4 x 10 1/2 in        */
	2970,4200,	/* A3 297 x 420 mm                    */
	2100,2970,	/* A4 210 x 297 mm                    */
	2100,2970,	/* A4 Small 210 x 297 mm              */
	1480,2100,	/* A5 148 x 210 mm                    */
#ifndef Paper_Size_Amend_Value
	2500,3540,	/* B4 (JIS) 250 x 354                 */
#else
	2570,3640,	/* B4 (JIS) 257 x 364                 */
#endif
	1820,2570,	/* B5 (JIS) 182 x 257 mm              */
	2159,3302,	/* Folio 8 1/2 x 13 in                */
	2150,2750,	/* Quarto 215 x 275 mm                */
	2540,3556,  /* 10x14 in                           */
	2794,4318,  /* 11x17 in                           */
	2159,2794,  /* Note 8 1/2 x 11 in                 */
	984,2254,  /* Envelope #9 3 7/8 x 8 7/8           */
	1084,2413,  /* Envelope #10 4 1/8 x 9 1/2         */
	1143,2635,  /* Envelope #11 4 1/2 x 10 3/8        */
	1206,2794,  /* Envelope #12 4 \276 x 11           */
	1270,2921,  /* Envelope #14 5 x 11 1/2            */
	4318,5588,  /* C size sheet  17 x 22 in           */
	5588,8636,  /* D size sheet   22 x 34 in          */
	8636,11176,  /* E size sheet    34 x 44 in        */
	1100,2200,  /* Envelope DL 110 x 220mm            */
	1620,2290,  /* Envelope C5 162 x 229 mm           */
	3240,4580,  /* Envelope C3  324 x 458 mm          */
	2290,3240,  /* Envelope C4  229 x 324 mm          */
	1140,1620,  /* Envelope C6  114 x 162 mm          */
	1140,2290,  /* Envelope C65 114 x 229 mm          */
	2500,3530,  /* Envelope B4  250 x 353 mm          */
	1760,2500,  /* Envelope B5  176 x 250 mm          */
	1760,1250,  /* Envelope B6  176 x 125 mm          */
	1100,2300,  /* Envelope 110 x 230 mm              */
	984,1905,   /* Envelope Monarch 3.875 x 7.5 in    */
	921,1651,   /* 6 3/4 Envelope 3 5/8 x 6 1/2 in    */
	3778,2794,  /* US Std Fanfold 14 7/8 x 11 in      */
	2159,3048,  /* German Std Fanfold 8 1/2 x 12 in   */
	2159,3302,  /* German Legal Fanfold 8 1/2 x 13 in */
	2500,3530,  /* B4 (ISO) 250 x 353 mm              */
	1000,1480,  /* Japanese Postcard 100 x 148 mm     */
	2286,2794,  /* 9 x 11 in                          */
	2540,2794,  /* 10 x 11 in                         */
	3810,2794,  /* 15 x 11 in                         */
	2200,2200,  /* Envelope Invite 220 x 220 mm       */
	0,0,  /* RESERVED--DO NOT USE               */
	0,0,  /* RESERVED--DO NOT USE               */
	2413,3048,  /* Letter Extra 9 \275 x 12 in        */
	2413,3810,  /* Legal Extra 9 \275 x 15 in         */
	2970,4572,  /* Tabloid Extra 11.69 x 18 in        */
	2354,3223,  /* A4 Extra 9.27 x 12.69 in           */
	2159,2794,  /* Letter Transverse 8 \275 x 11 in   */
	2100,2970,  /* A4 Transverse 210 x 297 mm         */
	2413,3048,  /* Letter Extra Transverse 9\275 x 12 in */
	2270,3560,  /* SuperA/SuperA/A4 227 x 356 mm      */
	3050,4870,  /* SuperB/SuperB/A3 305 x 487 mm      */
	2159,3223,  /* Letter Plus 8.5 x 12.69 in         */
	2100,3300,  /* A4 Plus 210 x 330 mm               */
	1480,2100,  /* A5 Transverse 148 x 210 mm         */
	1820,2570,  /* B5 (JIS) Transverse 182 x 257 mm   */
	3220,4450,  /* A3 Extra 322 x 445 mm              */
	1740,2350,  /* A5 Extra 174 x 235 mm              */
	2010,2760,  /* B5 (ISO) Extra 201 x 276 mm        */
	4200,5940,  /* A2 420 x 594 mm                    */
	2970,4200,  /* A3 Transverse 297 x 420 mm         */
	3220,4450,  /* A3 Extra Transverse 322 x 445 mm   */
	2000,1480,  /* Japanese Double Postcard 200 x 148 mm */
	1050,1480,  /* A6 105 x 148 mm                 */
	2400,3320,  /* Japanese Envelope Kaku #2       */
	2160,2770,  /* Japanese Envelope Kaku #3       */
	1200,2350,  /* Japanese Envelope Chou #3       */
	900,2050,   /* Japanese Envelope Chou #4       */
	2794,2159,  /* Letter Rotated 11 x 8 1/2 11 in */
	4200,2970,  /* A3 Rotated 420 x 297 mm         */
	2970,2100,  /* A4 Rotated 297 x 210 mm         */
	2100,1480,  /* A5 Rotated 210 x 148 mm         */
	3640,2570,  /* B4 (JIS) Rotated 364 x 257 mm   */
	2570,1820,  /* B5 (JIS) Rotated 257 x 182 mm   */
	1480,1000,  /* Japanese Postcard Rotated 148 x 100 mm */
	1480,2000,  /* Double Japanese Postcard Rotated 148 x 200 mm */
	1480,1050,  /* A6 Rotated 148 x 105 mm         */
	3320,2400,  /* Japanese Envelope Kaku #2 Rotated */
	2770,2160,  /* Japanese Envelope Kaku #3 Rotated */
	2350,1200,  /* Japanese Envelope Chou #3 Rotated */
	2050,900,   /* Japanese Envelope Chou #4 Rotated */
	1280,1820,  /* B6 (JIS) 128 x 182 mm           */
	1820,1280,  /* B6 (JIS) Rotated 182 x 128 mm   */
	3049,2795,  /* 12 x 11 in                      */
	1050,2350,  /* Japanese Envelope You #4        */
	2350,1050,  /* Japanese Envelope You #4 Rotated*/
#ifndef Paper_Size_Amend_Value
	1460,2150,  /* PRC 16K 146 x 215 mm            */
	970,1510,  /* PRC 32K 97 x 151 mm             */
	970,1510,  /* PRC 32K(Big) 97 x 151 mm        */
#else
	1840,2600,  /* PRC 16K 184 x 260 mm            */
	1300,1840,  /* PRC 32K 130 x 184 mm            */
	1400,2030,  /* PRC 32K(Big) 140 x 203 mm       */
#endif
	1020,1650,  /* PRC Envelope #1 102 x 165 mm    */
	1020,1760,  /* PRC Envelope #2 102 x 176 mm    */
	1250,1760,  /* PRC Envelope #3 125 x 176 mm    */
	1100,2080,  /* PRC Envelope #4 110 x 208 mm    */
	1100,2200,  /* PRC Envelope #5 110 x 220 mm    */
	1200,2300,  /* PRC Envelope #6 120 x 230 mm    */
	1600,2300,  /* PRC Envelope #7 160 x 230 mm    */
	1200,3090,  /* PRC Envelope #8 120 x 309 mm    */
	2290,3240,  /* PRC Envelope #9 229 x 324 mm    */
	3240,4580,  /* PRC Envelope #10 324 x 458 mm   */
#ifndef Paper_Size_Amend_Value
	2150,1460, /* PRC 16K Rotated 215 x 146 mm           */
	1510,970, /* PRC 32K Rotated 151 x 97 mm             */
	1510,970, /* PRC 32K(Big) Rotated  151 x 97 mm       */
#else
	2600,1840,  /* PRC 16K Rotated 260 x 184 mm	*/
	1840,1300,  /* PRC 32K Rotated 184 x 130 mm */
	2030,1400,  /* PRC 32K(Big) Rotated 203 x 140 mm	*/
#endif
	1650,1020,  /* PRC Envelope #1 Rotated 165 x 102 mm */
	1760,1020,  /* PRC Envelope #2 Rotated 176 x 102 mm */
	1760,1250,  /* PRC Envelope #3 Rotated 176 x 125 mm */
	2080,1100,  /* PRC Envelope #4 Rotated 208 x 110 mm */
	2200,1100,  /* PRC Envelope #5 Rotated 220 x 110 mm */
	2300,1200,  /* PRC Envelope #6 Rotated 230 x 120 mm */
	2300,1600,  /* PRC Envelope #7 Rotated 230 x 160 mm */
	3090,1200,  /* PRC Envelope #8 Rotated 309 x 120 mm */
	3240,2290,  /* PRC Envelope #9 Rotated 324 x 229 mm */
	4580,3240,  /* PRC Envelope #10 Rotated 458 x 324 mm */
	0,0			//DMPAPER_PENV_10_ROTATED
};
