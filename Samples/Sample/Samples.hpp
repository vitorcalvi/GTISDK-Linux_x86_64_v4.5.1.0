/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

#pragma once

#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>
#include <cstring>
#include <functional>
#include "Classify.hpp"
#include "GTILib.h"

#ifdef LINUX
#include <unistd.h>
#include <sys/time.h>

#define Sleep(ms) usleep(ms * 1000)

#define min(a, b) (((a)<(b)) ? (a) : (b))

#define scanf_s scanf
#define fscanf_s fscanf
#define sprintf_s sprintf

typedef unsigned int DWORD;
typedef unsigned char BYTE;
typedef char TCHAR, _TCHAR;
typedef int  BOOL;
typedef unsigned long ULONGLONG;
#endif

using std::string;

#define FTDI                0
#define EUSB                1
#define PCIE                2

#define MAX_FILENAME_SIZE   256
#define IMAGE_OUT_SIZE      0x08000
#define GTI_IMAGE_WIDTH     224
#define GTI_IMAGE_HEIGHT    224
#define GTI_IMAGE_CHANNEL   3

#define GTI_PICTURE         0
#define GTI_VIDEO           1
#define GTI_WEBCAM          2

#define PEOPLE_TYPE         7
#define PETS_TYPE           4
#define OFFICE_TYPE         0
#define KITCHEN_TYPE        1
#define LIVINGROOM_TYPE     2

#ifdef WIN32
typedef struct dirent {
    string d_name;
} Dirent, *pDirent;

typedef HANDLE cnn_mutex_t;
#define cnn_mutex_init		WIN_mutex_init
#define cnn_mutex_lock		WIN_mutex_lock
#define cnn_mutex_unlock	WIN_mutex_unlock
#define cnn_mutex_destroy	WIN_mutex_destroy
#endif

#ifdef LINUX
typedef pthread_mutex_t cnn_mutex_t;

#define CNN_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

#define cnn_mutex_init		pthread_mutex_init
#define cnn_mutex_lock		pthread_mutex_lock
#define cnn_mutex_unlock	pthread_mutex_unlock
#define cnn_mutex_destroy	pthread_mutex_destroy
#endif

class Samples
{
public:
    Samples(int, char *, char *, char *deviceName);
    ~Samples();

    void Test_picture();
    void Test_video();
    void Video_Hierarchy(int);
    void Test_webcam();
    void Webcam_Hierarchy();
    void Slideshow_picture(int);
    void Slideshow_video();
    void Test_CnnSpeed(int demoFlag);
    void Test_CnnSpeed_video();
    void Demo_video(char *videoName, char *dispName, int x, int y, int w, int h);
    void Demo_picture(char *dispName, int x, int y, int w, int h);

    void Test_face_recognition();
    void generate_face_feature_vectors();


#ifdef ANDROID
    void oneFramePredictInit(int type);
    void oneFramePredict(unsigned char *imgData, string* predictions);
    void facePredict(unsigned char *imgData, int predict, string* predictions, char *name);
#endif

protected:
    char *m_deviceName;
    string m_coefName;
    string m_picFcname;
    string m_videoFcname;
    string m_camFcname;
    string m_camFcnameFace;
    string m_camFc_pets;
    string m_picLabelname;
    string m_videoLabelname;
    string m_camLabelname;
    string m_camLabelnameFace;
    string m_camLabel_pets;
    string m_Fc_office;
    string m_Label_office;
    string m_Fc_kitchen;
    string m_Label_kitchen;
    string m_Fc_livingroom;
    string m_Label_livingroom;

    string m_ImageName = "Image";
    string m_VideoName = "Video";
    string m_WebcamName = "Webcam";
    string m_ImageFolder = "Image_bmp";
    string m_VideoFolder = "Image_mp4";
    string m_SdkPath;
    string m_filename;
    cv::Mat m_img, m_img1;
    cv::Size m_imgSize;
    double m_width;
    double m_height;

// face recognition and prediction 
    char name[128]; // name for register
    char* dir_target_vectors; // folder of feature vector files
    char* dir_target_photos; // folder of face photos 
    int feat_vector_len; // feature vector dim
    int num_of_targets; // num of persons in pool
    std::vector<char*> filename_target_vectors;
    std::map<string, int> vector_count_by_name;
    std::vector<float*> target_vectors;
    std::vector<float> abs_of_target_vectors;
    float* feat_vector; // feature vector for inference
    string pred_name;
    // face recognition and prediction end 

    GtiDevice *m_Device;
    TCHAR *m_FileNameBuffer = NULL;
    float *m_Buffer32FC3 = NULL;
    BYTE *m_Img224x224x3Buffer = NULL;
    float *m_ImageOutPtr = NULL;
    int m_WtRdDelay = 35;        // Read/Write delay
    int m_GnetType;

    ULONGLONG m_SvicTimerStart = 0;
    ULONGLONG m_SvicTimerEnd = 0;
    ULONGLONG m_CnnTimerStart = 0;
    ULONGLONG m_CnnTimerEnd = 0;
    ULONGLONG m_FcTimerStart = 0;
    ULONGLONG m_FcTimerEnd = 0;
    ULONGLONG m_PureCnnStart = 0;
    ULONGLONG m_PureCnnEnd = 0;

#ifdef ANDROID
    Classify *gClassify = NULL;
#endif

#ifdef LINUX
    unsigned long GetTickCount64()
    {
        struct timeval tv;
        if(gettimeofday(&tv, NULL) != 0)
                return 0;

        return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    }
#endif

    template <class T>
    void safeRelease(T& buffer)
    {
        if (buffer)
        {
            delete [] buffer;
            buffer = NULL;
        }
    }
    string openFileDialog(int Mode = 0);
    void convolution(const cv::Mat& img);
    void convolution(BYTE* img, const int imgLength);

    void convOutput();
    void setLabel(cv::Mat& im, const std::string label, const cv::Point & orig, double scale);
    void displayImage(cv::Mat &img, Classify* gClassify, int mode, string str="", int resize=1, int demoFlag=0, string ext="");
    void dispDemoImage(cv::Mat &img, Classify* gClassify, char* dispName, string str, int w, int h, int demoFlag=0, string ext="");
    void showImage(cv::Mat &dispImg, Classify* gClassify, string text, string displayName, string ext, int demoFlag);
    void showImage1(cv::Mat &dispImg, Classify* gClassify, string text, string displayName, string ext, int demoFlag);
    void cvt32FloatTo8Byte(float *InBuffer, uchar *OutBuffer, int Width, int Height, int Channels);
    void cnnSvicProc32FC3(const cv::Mat& img, std::vector<cv::Mat>* input_channels);
    void cnnSvicProc8UC3(const cv::Mat& img, std::vector<cv::Mat>*input_channels);
    int sdb2BlobsFcUnmapping(const unsigned char *svbuf_ptr, float* output_blobs_data, int dataLength);
    int gtiWriteToFile(char *filename, unsigned char *dataBuff, int dataLen);

    void convolution(const cv::Mat& img, string *ptxt);
    void convolution_face(const cv::Mat& img);

    void face_recognition_init(Classify* gtiClassify);
    void face_recognition_exec(Classify* gtiClassify, cv::Mat img_face);
    void face_recognition_register(Classify* gtiClassify, unsigned char *img_face, char* name);

//    void face_recognition_register_with_detection(Classify* gtiClassify, cv::Mat img, frontal_face_detector detector, char* name);

#ifdef WIN32
    int m_OpenFlag = 0;
    WIN32_FIND_DATA m_ffd;
    Dirent m_ent;
    HANDLE opendir(const char* dirName);
    pDirent readdir(HANDLE);
    void closedir(HANDLE);
#endif
};

class HandleFile
{
public:
    HandleFile(char *FileName, char *rw);
    ~HandleFile();

    int ReadFile();
    void WriteToFile(unsigned char *dataBuff, int dataLen);
    char *GetPtr() { return m_DataPtr; }
    int GetLength() { return m_Length; }
private:
    FILE *m_fp = NULL;
    char *m_DataPtr = NULL;
    int m_Length;

    int fileSize();
};

