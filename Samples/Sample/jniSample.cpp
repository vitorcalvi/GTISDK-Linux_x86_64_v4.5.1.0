/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

// GTI2801 simple sample code. It demonstrates how to use GTI library
// APIs to access GTI2801 Chip.
 
#include "stdafx.h"
#include "jniSample.hpp"
#include "GtiClassify.h"

#ifdef ANDROID
#include <android/log.h>
#define  LOG_TAG    "GTI_JNI"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

using std::string;

string glGtiPath = "GTISDKPATH";

// FC model and label file for gnet32 image classification
string glPicCoefDataFcGnet32    = "/data/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class.bin";
string glPicLabelGnet32         = "/data/Models/gti2801/gnet2_3/fc/gnet32_fc128_20class_label.txt"; 

// CNN model and EUSB interface configuration file
string glGnet32Coef512          = "/data/Models/gti2801/gnet2_3/cnn_3/gnet32_coef_512.dat";
string glGnet32UserInput        = "/data/Models/gti2801/gnet2_3/cnn_3/userinput.txt";

// Image file in 224x224x3 BGR plannar format
string glImagename = "/data/Image_Lite/bridge.bin";

string glDevice = "eusb"; // "eusb" or "ftdi"

unsigned long GetTickCount64()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
            return 0;

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#ifndef ANDROID
int main(int argc, _TCHAR* argv[])
{
    unsigned char bgrData[224 * 224 * 3];
    string predictions[5];
    char *gtiHome = NULL;

    gtiHome = getenv(glGtiPath.c_str());
    if (gtiHome == NULL)
    {
        printf("GTISDK home path is not set! Please see GTI SDK user guide.\n");
        exit(0);
    }

    string m_SdkPath        = string(gtiHome);

    glImagename             = m_SdkPath + glImagename;
    glPicCoefDataFcGnet32   = m_SdkPath + glPicCoefDataFcGnet32;
    glPicLabelGnet32        = m_SdkPath + glPicLabelGnet32;
    glGnet32Coef512         = m_SdkPath + glGnet32Coef512;
    glGnet32UserInput       = m_SdkPath + glGnet32UserInput;

    // Prepare image data
    FILE *fp_r = fopen(glImagename.c_str(), "rb");
    if (fp_r == NULL)
    {
        std::cout << "File is not found." << glImagename.c_str() << std::endl;
        exit(0);
    }

    fseek(fp_r,0,SEEK_END);
    int size = ftell(fp_r);
    if(size != (224 * 224 * 3))
    {
        std::cout << "---*" << glImagename.c_str() << "* has wrong image format. The image must be in 224 x 224 x 3 plannar BGR raw data format." << std::endl;
        std::cout << "--- Please use the tool under 'Tools/image_tool' to convert the format." << std::endl;
        exit(0);
    }

    fseek(fp_r,0,SEEK_SET);
    fread(bgrData, 1, 224 * 224 * 3, fp_r);
    fclose(fp_r);

// Start prediction
// This function reads image from an image file with the format
// 224 x 224 pixels x 3 channels, sends the image data to GTI chip
// and gets the CNN result data from chip, classifying the result.
// Expected output:
// 0.999955 - 17 : 17 bridge
// 4.50576e-05 - 9 : 9 unknown
// 1.57612e-10 - 16 : 16 pagoda

    JniSamples test(glGnet32Coef512.c_str(), glPicCoefDataFcGnet32.c_str(), glPicLabelGnet32.c_str(), glGnet32UserInput.c_str(), glDevice.c_str());

    test.oneFramePredict(bgrData, predictions);

    std::cout << "---------- Prediction for " << glImagename.c_str() << " ----------" << std::endl;

    std::cout << predictions[0].c_str() << std::endl;
    std::cout << predictions[1].c_str() << std::endl;
    std::cout << predictions[2].c_str() << std::endl;    

    return 0;
}
#endif

//====================================================================
// Function name: JniSamples()
// Class Samples construction.
// This function calls GTI library APIs to set CNN model, FC model,
// Classification label, model configuration, device interface, and
// initialize GTI SDK library. It also allocates memory for the sample
// code.
//====================================================================
JniSamples::JniSamples(const char *cnn, const char *fc, const char *label, const char *config, const char *deviceName)
{
    // Create GTI device
    if(strcmp(deviceName, "eusb") == 0 || strcmp(deviceName, "EUSB") == 0)
        m_Device = GtiDeviceCreate(EUSB, (char *)cnn, (char *)config);
    else if(strcmp(deviceName, "ftdi") == 0 || strcmp(deviceName, "FTDI") == 0)
        m_Device = GtiDeviceCreate(FTDI, (char *)cnn, (char *)config);
    else
    {
        std::cout << "The device name is wrong, use eusb or ftdi, exit...\n";
        #ifdef ANDROID
        ALOG("The device name is wrong,  use eusb or ftdi, exit...\n");
        #endif
        exit(0);
    }
#ifdef ANDROID
	ALOG("-CNN Model: %s\n", cnn);
	ALOG("-FC  Model: %s\n", fc);
	ALOG("-FC  Label: %s\n", label);
	ALOG("-Config: %s\n", config);
	ALOG("-Device: %s\n", deviceName);
#endif

    gClassify = GtiClassifyCreate(fc, label);
 
    // Open device
    GtiOpenDevice(m_Device, "0");
 
    // Initialize GTI m_SdkPath
    if (!GtiInitialization(m_Device))
    {
        std::cout << "GTI initialization failed." << std::endl;
#ifdef ANDROID
	ALOG("Device initialization failed.");
#endif
    }

 
    // Allocate memory for sample code use
    m_ImageOutPtr = new float[GtiGetOutputLength(m_Device)];
    if (!m_ImageOutPtr)
    {
        std::cout << "GTI allocation (m_ImageOutPtr) failed." << std::endl;      
    }

    m_Img224x224x3Buffer = new BYTE[GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * GTI_IMAGE_CHANNEL];
    if (!m_Img224x224x3Buffer)
    {
        std::cout << "GTI allocation (m_Img224x224x3Buffer) failed." << std::endl;
    } 
}

//====================================================================
// Function name: ~JniSamples()
// Class Samples destruction.
// This function releases the memory allcated in Samples(), 
// it also calls GTI library API to close device.
//====================================================================
JniSamples::~JniSamples()
{
    safeRelease(m_ImageOutPtr);
    safeRelease(m_Img224x224x3Buffer);

    // Close device
    GtiCloseDevice(m_Device);
    // Release device
    GtiDeviceRelease(m_Device);

    if(gClassify)
        GtiClassifyRelease(gClassify);
}


//====================================================================
// Function name: oneFramePredict(unsigned char *imgData, string* predictions)
// This function sends 224x224x3 BGR planar format image data into GTI device,
// reads data back from GTI device, calls Classify functions to get the image
// class name and index.
//
//====================================================================

void JniSamples::facePredict(unsigned char *imgData, int predict, string* predictions, char *name)
{
}

int showFrameRate = 0;
void JniSamples::oneFramePredict(unsigned char *imgData, string* predictions)
{
 //   ALOG("--- ENTER Samples::oneFramePredict.\n");
    ULONGLONG m_CnnTimerStart = 0;
    ULONGLONG m_CnnTimerEnd = 0;
    ULONGLONG m_FcTimerStart = 0;
    ULONGLONG m_FcTimerEnd = 0;
    float cnnProcessTime_ms;
    float fcProcessTime_ms;
    float frameRate;

    char  charbuf[256];

    m_CnnTimerStart = GetTickCount64();

    int ret = GtiHandleOneFrameFloat(m_Device,  imgData, 224 * 224 * 3, m_ImageOutPtr, GtiGetOutputLength(m_Device));
    if (!ret)
    {
        std::cout << "Handle one frame error!" << std::endl;
        #ifdef ANDROID
        ALOG("Handle one frame error!\n");
        #endif
        GtiClassifyRelease(gClassify);
        return;
    }
    m_CnnTimerEnd = GetTickCount64();
    m_FcTimerStart = m_CnnTimerEnd;

    gClassify->GtiClassify(m_ImageOutPtr, 5);
	if(showFrameRate++ % 30 == 0)
	{
		m_FcTimerEnd = GetTickCount64();

		cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
		fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);

		frameRate = (1000 / (cnnProcessTime_ms + fcProcessTime_ms));
		sprintf_s(charbuf, "Frame Rate: %5.2f, CNN: %5.2f ms, FC: %5.2f ms",
		            frameRate, cnnProcessTime_ms, fcProcessTime_ms);
		#ifdef ANDROID
		ALOG("--- %s", charbuf);
		#endif
		std::cout << charbuf << std::endl;
	}
    for (int i = 0; i < 5; ++i)
    {
        predictions[i] = gClassify->GtiGetPredicationString(i);
    }
}
