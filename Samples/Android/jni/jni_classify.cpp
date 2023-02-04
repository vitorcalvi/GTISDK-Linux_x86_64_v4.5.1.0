/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

#include <jni.h>
#include <string>

#include "../../../Samples/Sample/jniSample.hpp"

#include <android/log.h>
#define  LOG_TAG    "GTI_JNI"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

using namespace std;

#ifdef TEST_2803_FTDI
string deviceInterface = "ftdi";
#else
string deviceInterface = "eusb"; // "eusb" or "ftdi"
#endif

JniSamples *Demo;

typedef struct 
{ 
  std::string cnnModel;
  std::string userConfiguration;
  std::string fcModel;
  std::string fcLabel;

}MODEL_INFO;

#ifdef TEST_2803_FTDI
MODEL_INFO imageClassificationModel = {
"/sdcard/gti/Models/gti2803/gnet1/cnn/gnet1_coef_vgg16.dat",
"/sdcard/gti/Models/gti2803/gnet1/cnn/userinput.txt",
"/sdcard/gti/Models/gti2803/gnet1/fc/gnet1_coef_1000.bin",
"/sdcard/gti/Models/gti2803/gnet1/fc/gnet1_label_1000.txt"
};
#else
MODEL_INFO imageClassificationModel = {
"/sdcard/gti/Models/gti2801/gnet1/cnn/gnet1_coef_vgg16.dat",
"/sdcard/gti/Models/gti2801/gnet1/cnn/userinput.txt",
"/sdcard/gti/Models/gti2801/gnet1/fc/picture_coef.bin",
"/sdcard/gti/Models/gti2801/gnet1/fc/picture_label.txt"
};
#endif

MODEL_INFO videoClassificationModel = {
"/sdcard/gti/Models/gti2801/gnet2_3/cnn_3/gnet32_coef_512.dat",
"/sdcard/gti/Models/gti2801/gnet2_3/cnn_3/userinput.txt",
"/sdcard/gti/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class.bin",
"/sdcard/gti/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class_label.txt"
};

MODEL_INFO cameraClassificationModel = {
"/sdcard/gti/Models/gti2801/gnet2_3/cnn_3/gnet32_coef_512.dat",
"/sdcard/gti/Models/gti2801/gnet2_3/cnn_3/userinput.txt",
"/sdcard/gti/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class.bin",
"/sdcard/gti/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class_label.txt"
};

MODEL_INFO faceRecognitionModel = {
"/sdcard/gti/Models/gti2801/gnet1/cnn/gti_face_coef_cnn.dat",
"/sdcard/gti/Models/gti2801/gnet1/cnn/userinput.txt",
"/sdcard/gti/Models/gti2801/gnet1/fc/gti_face_coef.bin",
"/sdcard/gti/Models/gti2801/gnet1/fc/face_label.txt"
};

std::string predictionResults[5];


extern "C" {
JNIEXPORT void JNICALL Java_gti_gtidemo_MainActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_org_gti_demo_adapter_FullScreenImageAdapter_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);


JNIEXPORT void JNICALL Java_test_cameratest_CameraTestActivity_initEngine(JNIEnv*, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_test_cameratest_CameraTestActivity_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);

JNIEXPORT void JNICALL Java_gti_facerecognition_FaceRecognitionActivity_initEngine(JNIEnv*, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_gti_facerecognition_FaceRecognitionActivity_facePredict(JNIEnv* env, jobject, jbyteArray imgData, jint predict, jstring name);

JNIEXPORT void JNICALL Java_gti_camera_GtiCameraActivity_initEngine(JNIEnv*, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_gti_camera_GtiCameraActivity_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);

JNIEXPORT void JNICALL Java_org_gti_demo_GridViewActivity_initEngine(JNIEnv*, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_org_gti_demo_FullScreenViewActivity_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);
JNIEXPORT jobjectArray JNICALL Java_org_gti_videodemo_adapter_FullScreenImageAdapter_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);
JNIEXPORT jobjectArray JNICALL Java_org_gti_videodemo_FullScreenViewActivity_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);


JNIEXPORT void JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_initEngine(JNIEnv*, jobject thiz, jint network, jint type);
JNIEXPORT jobjectArray JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_facePredict(JNIEnv* env, jobject, jbyteArray imgData, jint predict);
JNIEXPORT jobjectArray JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData);


void initEngine(JNIEnv* env, jint network, jint type);
jobjectArray oneFramePredict(JNIEnv* env, jbyteArray imgData);

std::string ConvertJString(JNIEnv* env, jstring str)
{
   const jsize len = env->GetStringUTFLength(str);
   const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

   std::string Result(strChars, len);

   env->ReleaseStringUTFChars(str, strChars);

   return Result;
}


//API to init CNN engine
void initEngine(JNIEnv* env, jint network, jint type)
{
#if 0 // For rooted phones
	int r = system("su -c \"setenforce 0 \"");
	if (r !=0) {
		ALOG("unable to setenforce");
	} else {
		ALOG("setenforce success");
	}
	
	r = system("su -c \"chmod -R 777 /dev/bus\"");
	if (r !=0) {
		ALOG("unable to chmod for /dev/bus");
	} else {
		ALOG("chmod for /dev/bus success");
	}

	r = system("su -c \"chmod -R 777 /dev/sg*\"");
	if (r !=0) {
		ALOG("unable to chmod for /dev/sg*");
	} else {
		ALOG("chmod for /dev/sg* success");
	}

	r = system("su -c \"echo \"performance\" > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor\"");
	if (r !=0) {
		ALOG("unable to change scaling_governor");
	} else {
		ALOG("scaling_governor success");
	}

	r = system("su -c \"echo \"performance\" > /sys/devices/system/cpu/cpufreq/policy4/scaling_governor\"");
	if (r !=0) {
		ALOG("unable to change scaling_governor");
	} else {
		ALOG("scaling_governor success");
	}

#endif

  ALOG("---- initEngine 1");
 // string cnnModel, userConfiguration, fcModel, fcLabel;
  MODEL_INFO modelInfo;

  if(type == 0) // Image
    modelInfo = imageClassificationModel;
  else if(type == 1) // Video
    modelInfo = videoClassificationModel;
  else if(type == 2) // Camera
    modelInfo = cameraClassificationModel;
  else if(type == 3) // Face Recognition
    modelInfo = faceRecognitionModel;
  else
    modelInfo = imageClassificationModel;

	Demo = new JniSamples(modelInfo.cnnModel.c_str(), 
                        modelInfo.fcModel.c_str(), 
                        modelInfo.fcLabel.c_str(), 
                        modelInfo.userConfiguration.c_str(), 
                        deviceInterface.c_str());  
  ALOG("---- initEngine 2");
}

// API to send one frame of image data to CNN engine and read back the classification results
jobjectArray oneFramePredict(JNIEnv* env, jbyteArray imgData)
{
    signed char* buffer = env->GetByteArrayElements(imgData, 0);

    Demo->oneFramePredict((unsigned char*)buffer, predictionResults);
 //   ALOG("predictionResults[0] = %s\n", predictionResults[0].c_str());
    jobjectArray ret;
    
    ret= (jobjectArray)env->NewObjectArray(5, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for(int i = 0; i < 2; i++)
    {
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(predictionResults[i].c_str()));
    }

    return(ret);
}

char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

// API to send one frame of image data to CNN engine and read back the classification results
jobjectArray facePredict(JNIEnv* env, jbyteArray imgData, jint predict, jstring name)
{
    jobjectArray ret;
// 	return (ret); // Disable it for now.

    signed char* buffer = env->GetByteArrayElements(imgData, 0);

    //Demo->oneFramePredict((unsigned char*)buffer, predictionResults);

	// ALOG("----- JNI facePredict.\n");
    
    Demo->facePredict((unsigned char*)buffer, predict, predictionResults, jstringToChar(env, name));

    //predictionResults[0] = "ABCD";

    ALOG("predictionResults[0] = %s\n", predictionResults[0].c_str());
    
    ret= (jobjectArray)env->NewObjectArray(5, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for(int i = 0; i < 1; i++)
    {
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(predictionResults[i].c_str()));
    }

    return(ret);
}


JNIEXPORT void JNICALL Java_org_gti_demo_GridViewActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{   
  initEngine(env, network, type);
}


JNIEXPORT void JNICALL Java_org_gti_videodemo_GridViewActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{
  initEngine(env, network, type);
}

JNIEXPORT void JNICALL Java_gti_gtidemo_MainActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{
  initEngine(env, network, type);
}

JNIEXPORT void JNICALL Java_test_cameratest_CameraTestActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{
    initEngine(env, network, type);
}

JNIEXPORT void JNICALL Java_gti_camera_GtiCameraActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{
    initEngine(env, network, type);
}


JNIEXPORT void JNICALL Java_gti_facerecognition_FaceRecognitionActivity_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{
   initEngine(env, network, type); 
}


JNIEXPORT jobjectArray JNICALL Java_gti_gtidemo_MainActivity_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}


JNIEXPORT jobjectArray JNICALL Java_org_gti_demo_FullScreenViewActivity_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}


JNIEXPORT jobjectArray JNICALL Java_org_gti_demo_adapter_FullScreenImageAdapter_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}


JNIEXPORT jobjectArray JNICALL Java_test_cameratest_CameraTestActivity_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}

JNIEXPORT jobjectArray JNICALL Java_gti_camera_GtiCameraActivity_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}


JNIEXPORT jobjectArray JNICALL Java_org_gti_videodemo_FullScreenViewActivity_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}

JNIEXPORT jobjectArray JNICALL Java_org_gti_videodemo_adapter_FullScreenImageAdapter_oneFramePredict(JNIEnv* env, jobject thiz, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData));
}

JNIEXPORT jobjectArray JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_oneFramePredict(JNIEnv* env, jobject, jbyteArray imgData)
{
  return (oneFramePredict(env, imgData)); 
}

JNIEXPORT void JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_initEngine(JNIEnv* env, jobject thiz, jint network, jint type)
{   
  initEngine(env, network, type);
}


JNIEXPORT jobjectArray JNICALL Java_com_tzutalin_dlibtest_OnGetImageListener_facePredict(JNIEnv* env, jobject, jbyteArray imgData, jint predict)
{
  //return (facePredict(env, imgData, predict, ""));
}

JNIEXPORT jobjectArray JNICALL Java_gti_facerecognition_FaceRecognitionActivity_facePredict(JNIEnv* env, jobject, jbyteArray imgData, jint predict, jstring name)
{
  return (facePredict(env, imgData, predict, name));
}
}
