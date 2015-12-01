/// @file Wn_FacePortrait.h
/// @brief 
/// @details 
/// @author		chenghuaiyu@sina.com
/// @date		2014-10-5
/// @version	A001
/// @par Copyright (c):
///        Science And Technology Co.,Ltd.
/// @par History:          
///		version: author, date, desc\n 

#ifndef __WN_FACE_PORTRAIT_H__
#define __WN_FACE_PORTRAIT_H__

#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

typedef enum Wn_Error_T
{
	WN_ERROR_NONE,
	WN_ERROR_PARAMETER,
	WN_ERROR_FAIL,


}
Wn_Error;

#ifdef __cplusplus
extern "C" {
#endif

	/// @defgroup functions 函数定义
	/// @{

	/// @brief .
	/// @details .
	/// @param	. 
	/// @retval	0		成功
	/// @retval	其他值	错误
	/// @note
	int DecorateFace(Mat & mDst, Mat & mSrc);


	/// @}

#ifdef __cplusplus
} // extern "C"
#endif


#endif

