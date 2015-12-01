package com.example.testopencv;

import org.opencv.core.Mat;

/**
* @ClassName: NdkJniHeadConversion
* @Description: TODO JNI 调用c/c++代码 头像转换类
* @author lijia
* @date 2015-11-27 下午5:40:56
*
 */
public class ImageProc {
	
//    static{  
//        System.loadLibrary("opencv_java");  
//        System.loadLibrary("image_process");  
//    }  

	/**
	 * 头像图片转化成漫画形势
	 * @param mMat
	 * @param srcMat
	 * @return
	 */
	public native int DecorateFace(Mat mMat,Mat srcMat);
	
}
