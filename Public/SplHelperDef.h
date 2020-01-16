#ifndef _SPLHELPERDEF_C71D6D41_C58E_4ff8_B39E_CF694E3DC0DF_H_
#define _SPLHELPERDEF_C71D6D41_C58E_4ff8_B39E_CF694E3DC0DF_H_

enum SplType{ST_UnKnow, ST_EMF, ST_PCL, ST_PCL_5, ST_PCL_6, ST_PS, ST_PS_3, ST_PS_2};

#define PostScript_Print_Param_PageSize			TEXT("/PageSize")				//纸型宽高大小

#define PostScript_Print_Param_Duplex			TEXT("Duplex")				//双面打印参数
#define PostScript_Print_Param_Duplex_None		TEXT("%%BeginFeature: *Duplex None")				//单面打印
#define PostScript_Print_Param_Duplex_NoTumble	TEXT("%%BeginFeature: *Duplex DuplexNoTumble")	//双面长边打印
#define PostScript_Print_Param_Duplex_Tumble	TEXT("%%BeginFeature: *Duplex DuplexTumble")	//双面短边打印
//【/Duplex true /Tumble false】	//双面：开启，纸型旋转：关闭。双面长边
#define PostScript_Print_Param_DuplexFlag	TEXT("/Duplex")	//双面打印开关，true/falae
#define PostScript_Print_Param_TumbleFlag	TEXT("/Tumble")	//纸型旋转开关，true/falae

//@PJL SET QTY = 1
#define PostScript_Print_Param_PJL_Copies	TEXT("@PJL SET QTY")

//苹果Mac打印的PS文件
#define PostScript_Print_Param_Color_On_MAC		TEXT("%%BeginFeature: *ColorModel RGB")
#define PostScript_Print_Param_Color_Off_MAC	TEXT("%%BeginFeature: *ColorModel Gray")

#define PostScript_Print_Param_Copies_L3	TEXT("%%BeginNonPPDFeature: NumCopies")
#define PostScript_Print_Param_Copies_L2	TEXT("%RBINumCopies:")
#define PostScript_Print_Param_Page_Begin	TEXT("%%BeginPageSetup")
#define PostScript_Print_Param_Page_End		TEXT("%%EndPageSetup")
#define PostScript_Print_Param_Color_On_LG	TEXT("%%BeginFeature: colorbw color")			//理光
#define PostScript_Print_Param_Color_Off_LG	TEXT("%%BeginFeature: colorbw blackandwhite")	//理光
#define PostScript_Print_Param_Color_On_LM	TEXT("@PJL SET RENDERMODE=COLOR")				//利盟
#define PostScript_Print_Param_Color_Off_LM	TEXT("@PJL SET RENDERMODE=GRAYSCALE")			//利盟
#define PostScript_Print_Param_Color_Off_HpUniversalPrinting	TEXT("@PJL SET GRAYSCALE=BLACKONLY")			//惠普通用PS驱动
#define PostScript_Print_Param_Page_End		TEXT("%%EndPageSetup")
#define PostScript_Print_Param_Pages		TEXT("%%Pages:")
#define PostScript_Print_Param_AtEnd		TEXT("(atend)")
#define SPL_PostScript_Flag					TEXT("@PJL ENTER LANGUAGE=POSTSCRIPT")	//POSTSCRIPT 文件标志
#define SPL_PCL_Flag						TEXT("@PJL ENTER LANGUAGE=PCL")	//pcl 文件标志
#define SPL_PCL6_Flag						TEXT("@PJL ENTER LANGUAGE=PCLXL")	//PCL6 文件标志


#endif