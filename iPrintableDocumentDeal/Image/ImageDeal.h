#ifndef _Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_
#define _Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

enum PicturePostion
{
	PicturePostion_Cetter	= 0,	//����
	PicturePostion_Left		= 1,	//����
	PicturePostion_Rigth	= 2,	//����
	PicturePostion_Top		= 3,	//����
	PicturePostion_Bottom	= 4		//����
};

enum PictureSize
{
	PictureSize_Original	= 0,	//ԭʼ��С�����续���ͼƬС����ȱ������ŵ�����ʾ������ͼƬ
	PictureSize_Full		= 1		//�ȱ������ŵ�����ʾ������ͼƬ
};

extern ColorMatrix colorMatrix;		//����ɫͼƬת��Ϊ�Ҷ�ͼƬ�ľ������

#define Image_Deal_Max_Dpi	300		//ͼƬ��������dpi



#endif //_Image_Deal_Def_H_39D7038C_1F92_4e76_8FD7_BEF986F65209_

