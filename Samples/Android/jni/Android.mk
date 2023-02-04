LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=  ../../Samples/Sample/ ../../../Include/ ../../../Include/Android/jni/include/

#-mfloat-abi=hardfp -DNDEBUG -mfpu=vfp -marm
LOCAL_CFLAGS := -std=c++11 -DLINUX -DFACE_RECOGNITION -DANDROID -O3

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../../Lib/Android/arm64-v8a/ 
TARGET_ARCH_ABI := arm64-v8a
TARGET_ABI := android-21-arm64-v8a
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../../Lib/Android/armeabi-v7a/
TARGET_ARCH_ABI := armeabi-v7a
TARGET_ABI := android-21-armeabi-v7a
endif

include ../../../Include/Android/jni/OpenCV.mk

LOCAL_MODULE    := gti_classify
LOCAL_SRC_FILES := 	../../Sample/Classify.cpp \
					../../Sample/jniSample.cpp	\
					jni_classify.cpp 
					
LOCAL_LDLIBS +=  -llog -ldl -lstdc++ -lc -lz -lm -landroid -latomic -lgti_android -lftd3xx -lstdc++ 

include $(BUILD_SHARED_LIBRARY)
