/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

#pragma once

#include <iomanip>

#include <stdio.h>
#include <stdlib.h>

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

#define GTI_IMAGE_WIDTH     224
#define GTI_IMAGE_HEIGHT    224
#define GTI_IMAGE_CHANNEL   3

class JniSamples
{
public:
    JniSamples(const char *cnn, const char *fc, const char *label, const char *config, const char *deviceName);
    ~JniSamples();

    void oneFramePredict(unsigned char *imgData, string* predictions);
    void facePredict(unsigned char *imgData, int predict, string* predictions, char *name);

protected:
    GtiDevice *m_Device;
    BYTE *m_Img224x224x3Buffer = NULL;
    float *m_ImageOutPtr = NULL;
    Classify *gClassify = NULL;


    template <class T>
    void safeRelease(T& buffer)
    {
        if (buffer)
        {
            delete [] buffer;
            buffer = NULL;
        }
    }
};

