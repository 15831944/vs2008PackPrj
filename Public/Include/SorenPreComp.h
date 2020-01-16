/*********************************************************************
* 版权所有 (C)2011，ISafeTec。
* 
* 文件名称：  SorenPreComp.h
* 内容摘要：  需要在stdafx.h头文件前面定义的宏
* 说    明：  
* 当前版本：  1.0.0.0
* 作    者：  SongQX
* 日    期：  2011/06/30
***********************************************************************/

#if 1
#ifdef _BIND_TO_CURRENT_VCLIBS_VERSION
#undef _BIND_TO_CURRENT_VCLIBS_VERSION
#pragma message("SorenPreComp.h is included too late!\n_BIND_TO_CURRENT_VCLIBS_VERSION will not take effect.")
#endif

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1
#endif
