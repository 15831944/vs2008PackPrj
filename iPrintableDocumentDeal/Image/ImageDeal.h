#ifndef _Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_
#define _Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

enum PicturePostion
{
	PicturePostion_Cetter	= 0,	//居中
	PicturePostion_Left		= 1,	//居左
	PicturePostion_Rigth	= 2,	//居右
	PicturePostion_Top		= 3,	//剧上
	PicturePostion_Bottom	= 4		//居下
};

enum PictureSize
{
	PictureSize_Original	= 0,	//原始大小，假如画面比图片小，则等比例缩放到可显示的最大的图片
	PictureSize_Full		= 1		//等比例缩放到可显示的最大的图片
};

extern ColorMatrix colorMatrix;		//将彩色图片转换为灰度图片的矩阵参数

#define Image_Deal_Max_Dpi	300		//图片处理的最大dpi



#endif //_Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_

