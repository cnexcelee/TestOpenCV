#include <com_example_testopencv_ImageProc.h>
#include <Wn_FacePortrait.h>



/*
 * Class:     com_example_testopencv_ImageProc
 * Method:    DecorateFace
 * Signature: (Lorg/opencv/core/Mat;Lorg/opencv/core/Mat;)I
 */
JNIEXPORT jint JNICALL Java_com_example_testopencv_ImageProc_DecorateFace
  (JNIEnv *env, jobject obj, jobject mat, jobject srcMat){

	jint back = DecorateFace(&mat,&srcMat);
	return back;
}

