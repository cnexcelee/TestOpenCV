LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

OpenCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off

OPENCV_LIB_TYPE:=SHARE

ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include E:\sourcecode\MoTuShiYiJian\OpenCV-2.4.9-android-sdk\sdk\native\jni\OpenCV.mk
else  
include $(OPENCV_MK_PATH)  
endif 


LOCAL_MODULE    := image_process 
LOCAL_SRC_FILES := com_example_testopencv_ImageProc.c \
					Wn_FacePortrait.cpp
LOCAL_LDLIBS    += -lm -llog 
			
include $(BUILD_SHARED_LIBRARY) 