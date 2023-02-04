/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

// GTI2801 Sample code. It demonstrates how to use GTI library
// APIs to access GTI2801 Chip.

#include "stdafx.h"
#include <mutex>
#include "Samples.hpp"
#include "GtiClassify.h"


#ifdef WIN32
#include "direct.h"
#include "strsafe.h"
#endif
#ifdef LINUX
#include <dirent.h>
#include "unistd.h"
#endif

using std::string;

#ifdef ANDROID
#include <android/log.h>
#define  LOG_TAG    "GTI_JNI"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif


#define CNN_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

std::mutex  mOpenCvMutex ;

string gl20ClassPictureFolder = "Demo Sample 196 Pics Sep2017_JPEG";

string glGtiPath = "GTISDKPATH";

#ifdef WIN32
string dataPath = "\\data\\";
// Video heirarchy demo file
string glCnnSpeedDemoFileName = dataPath + "Image_bmp\\cat.bmp";
string glCnnSpeedDemoVideoName = dataPath + "Image_demo\\VideoDemoFastestMP4.mp4";
string glVideoDemoFileName = dataPath + "Image_demo\\VideoDemoFastestMP4.mp4";
string glImageFolder = dataPath + "Image_bmp";
string glVideoFolder = dataPath + "Image_mp4";
string glVideoDemoFolder = dataPath + "Image_mp4\\";

// Model files root
string glGnet1Root = dataPath + "Models\\gti2801\\gnet1\\";
string glGnet2_3Root = dataPath + "Models\\gti2801\\gnet2_3\\";

// Files for picture
string glPicCoefDataFcFileName = glGnet1Root + "fc\\picture_coef.bin";             // Picture CoefDataFcFileName
string glPicLabelFileName = glGnet1Root + "fc\\picture_label.txt";                 // Picture Label file name
// Files for gnet32 picture
string glPicCoefDataFcGnet32 = glGnet2_3Root + "fc\\gnet32_pic_coef_1000.bin";     // Picture CoefDataFcFileName
string glPicLabelGnet32 = glGnet2_3Root + "fc\\gnet32_pic_label_1000.txt";         // Picture Label file name
// Files for video
string glVideoCoefDataFcFileName = glGnet1Root + "fc\\video_coef.bin";             // Video CoefDataFcFileName
string glVideoLabelFileName = glGnet1Root + "fc\\video_label.txt";                 // Video Label file name
// Files for gnet32 video
string glVideoCoefDataFcGnet32 = glGnet2_3Root + "fc\\gnet32_video_coef_40.bin";   // Video CoefDataFcFileName
string glVideoLabelGnet32 = glGnet2_3Root + "fc\\gnet32_video_label_40.txt";          // Video Label file name
// Files for web camera
string glWebCamCoefDataFcFileName = glGnet1Root + "fc\\webcam_coef.bin";           // WebCam CoefDataFcFileName
string glWebCamLabelFileName = glGnet1Root + "fc\\webcam_label.txt";               // WebCam Label file name
// Files for gnet32 web camera
string glWebCamCoefDataFcGnet32 = glGnet2_3Root + "fc\\gnet32_20class.bin";        // Picture CoefDataFcFileName
string glWebCamLabelGnet32 = glGnet2_3Root + "fc\\gnet32_20class_label.txt";       // Picture Label file name

// GTI coef file name
string glGtiCoefFileName = glGnet1Root + "cnn\\gnet1_coef_vgg16.dat";
string glGnet1UserInput = glGnet1Root + "cnn\\userinput.txt";
// Gnet32 coef file name for 512 output
string glGnet32Coef512 = glGnet2_3Root + "cnn_3\\gnet32_coef_512.dat";
string glGnet32UserInput = glGnet2_3Root + "cnn_3\\userinput.txt";

// Sub classifies
string glWebCamFcPets = glGnet1Root + "fc\\sub_pets\\vgg16_20class_pets.bin";
string glWebCamLabelPets = glGnet1Root + "fc\\sub_pets\\vgg16_pets_label.txt";
// gnet2,3 Sub classifies
string glGnet2_3WebCamFcPets = glGnet2_3Root + "fc\\sub_pets\\gnet32_20class_pets.bin";
string glGnet2_3WebCamLabelPets = glGnet2_3Root + "fc\\sub_pets\\gnet32_pets_label.txt";
string glGnet2_3Fc_10office = glGnet2_3Root + "fc\\sub_office\\gnet3_10office.bin";
string glGnet2_3Fc_10officeLabel = glGnet2_3Root + "fc\\sub_office\\gnet3_10office_label.txt";
string glGnet2_3Fc_10kitchen = glGnet2_3Root + "fc\\sub_kitchen\\gnet3_11kitchen.bin";
string glGnet2_3Fc_10kitchenLabel = glGnet2_3Root + "fc\\sub_kitchen\\gnet3_11kitchen_label.txt";
string glGnet2_3Fc_10livingroom = glGnet2_3Root + "fc\\sub_livingroom\\gnet3_10livingRoom.bin";
string glGnet2_3Fc_10livingroomLabel = glGnet2_3Root + "fc\\sub_livingroom\\gnet3_10livingRoom_label.txt";
string glGnet1CnnRoot = glGnet1Root + "cnn\\";
string glGnet2_3CnnRoot = glGnet2_3Root + "cnn_3\\";

#else

#ifdef ANDROID
string dataPath = "/sdcard/gti/";
// Video heirarchy demo file
string glCnnSpeedDemoFileName = dataPath + "Image_bmp/cat.bmp";
string glCnnSpeedDemoVideoName = dataPath + "Image_demo/VideoDemoFastestMP4.mp4";
string glVideoDemoFileName = dataPath + "Image_demo/VideoDemoFastestMP4.mp4";
string glImageFolder = dataPath + "Image_bmp";
string glVideoFolder = dataPath + "Image_mp4";
string glVideoDemoFolder = dataPath + "Image_mp4";

// Model files root
string glGnet1Root = dataPath + "Models/gti2801/gnet1/";
string glGnet2_3Root = dataPath + "Models/gti2801/gnet2_3/";
string glGnet1CnnRoot = glGnet1Root + "cnn/";
string glGnet2_3CnnRoot = glGnet2_3Root + "cnn_3/";
#else

string dataPath = "/data/";
// Video heirarchy demo file
string glCnnSpeedDemoFileName = dataPath + "Image_bmp/cat.bmp";
string glCnnSpeedDemoVideoName = dataPath + "Image_demo/VideoDemoFastestMP4.mp4";
string glVideoDemoFileName = dataPath + "Image_demo/VideoDemoFastestMP4.mp4";
string glImageFolder = dataPath + "Image_bmp";
string glVideoFolder = dataPath + "Image_mp4";
string glVideoDemoFolder = dataPath + "Image_mp4/";

// Model files root
string glGnet1Root = dataPath + "Models/gti2801/gnet1/";
string glGnet2_3Root = dataPath + "Models/gti2801/gnet2_3/";
string glGnet1CnnRoot = glGnet1Root + "cnn/";
string glGnet2_3CnnRoot = glGnet2_3Root + "cnn_3/";

string glGti2803Gnet1Root = dataPath + "Models/gti2803/gnet1/";
#endif

// Files for picture (2803)
string gl2803PicCoefDataFcFileName = glGti2803Gnet1Root + "fc/cnnlib_coef_fc.bin";             // Picture CoefDataFcFileName
string gl2803PicLabelFileName = glGti2803Gnet1Root + "fc/synset_words_1000class.txt";          // Picture Label file name

// Files for picture
string glPicCoefDataFcFileName = glGnet1Root + "fc/picture_coef.bin";             // Picture CoefDataFcFileName
string glPicLabelFileName = glGnet1Root + "fc/picture_label.txt";                 // Picture Label file name
// Files for gnet32 picture
string glPicCoefDataFcGnet32 = glGnet2_3Root + "fc/gnet32_pic_coef_1000.bin";     // Picture CoefDataFcFileName
string glPicLabelGnet32 = glGnet2_3Root + "fc/gnet32_pic_label_1000.txt";         // Picture Label file name
// Files for video
string glVideoCoefDataFcFileName = glGnet1Root + "fc/video_coef.bin";             // Video CoefDataFcFileName
string glVideoLabelFileName = glGnet1Root + "fc/video_label.txt";                 // Video Label file name
// Files for gnet32 video
string glVideoCoefDataFcGnet32 = glGnet2_3Root + "fc/gnet32_video_coef_40.bin";      // Video CoefDataFcFileName
string glVideoLabelGnet32 = glGnet2_3Root + "fc/gnet32_video_label_40.txt";          // Video Label file name
// Files for web camera
string glWebCamCoefDataFcFileName = glGnet1Root + "fc/webcam_coef.bin";           // WebCam CoefDataFcFileName
string glWebCamLabelFileName = glGnet1Root + "fc/webcam_label.txt";               // WebCam Label file name
// Files for gnet32 web camera
string glWebCamCoefDataFcGnet32 = glGnet2_3Root + "fc/gnet32_20class.bin";        // Picture CoefDataFcFileName
string glWebCamLabelGnet32 = glGnet2_3Root + "fc/gnet32_20class_label.txt";       // Picture Label file name

// GTI coef file name
string glGtiCoefFileName = glGnet1Root + "cnn/gnet1_coef_vgg16.dat";
string glGnet1UserInput = glGnet1Root + "cnn/userinput.txt";
// Gnet32 coef file name for 512 output
string glGnet32Coef512 = glGnet2_3Root + "cnn_3/gnet32_coef_512.dat";
string glGnet32UserInput = glGnet2_3Root + "cnn_3/userinput.txt";

// Sub classifies
string glWebCamFcPets = glGnet1Root + "fc/sub_pets/vgg16_20class_pets.bin";
string glWebCamLabelPets = glGnet1Root + "fc/sub_pets/vgg16_pets_label.txt";
// gnet2,3 Sub classifies
string glGnet2_3WebCamFcPets = glGnet2_3Root + "fc/sub_pets/gnet32_20class_pets.bin";
string glGnet2_3WebCamLabelPets = glGnet2_3Root + "fc/sub_pets/gnet32_pets_label.txt";
string glGnet2_3Fc_10office = glGnet2_3Root + "fc/sub_office/gnet3_10office.bin";
string glGnet2_3Fc_10officeLabel = glGnet2_3Root + "fc/sub_office/gnet3_10office_label.txt";
string glGnet2_3Fc_10kitchen = glGnet2_3Root + "fc/sub_kitchen/gnet3_11kitchen.bin";
string glGnet2_3Fc_10kitchenLabel = glGnet2_3Root + "fc/sub_kitchen/gnet3_11kitchen_label.txt";
string glGnet2_3Fc_10livingroom = glGnet2_3Root + "fc/sub_livingroom/gnet3_10livingRoom.bin";
string glGnet2_3Fc_10livingroomLabel = glGnet2_3Root + "fc/sub_livingroom/gnet3_10livingRoom_label.txt";
#endif


#ifdef FACE_RECOGNITION
string glWebCamCoefDataFcFileNameFace = glGnet1Root + "fc/gti_face_coef_fc.bin";           // FaceRecogntion CoefDataFcFileName
string glWebCamLabelFileNameFace = glGnet1Root + "fc/face_label.txt";               // WebCam Label file name
string glGtiCoefFileNameFace = glGnet1Root + "cnn/gti_face_coef_cnn.dat";  //CNN coef for Face
//string glGtiCoefFileNameFace = glGnet1Root +  "/cnn/YYY.dat";  //CNN coef for Face


void read_fc7_vector(char* filename, float* vector, int len)
{
   FILE *file = fopen(filename, "r" );
   if ( file != NULL )
   {
      int i = 0;
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( (fgets(line, sizeof line, file) != NULL) && (i < len)) /* read a line */
      {
         //fputs (line, stdout); /* write the line */
	 vector[i] = atof(line); 
	 i++;
      }
      fclose(file);
   }
   else
   {
      printf("Failed to open vector file!");
   }
   return;
}

float inner_product(float* a, float* b, int len)
{
    float r = 0.0;
    for(int i = 0; i < len; i++){
	r += a[i]*b[i];
    }
    return r;
}

float abs_of_vector(float* a, int len)
{
    float r = sqrt(inner_product(a, a, len));
    return r;
}


int get_max_cos_sim_idx(std::vector<float*> target_vectors, int num_of_targets, float* test_vector, int feat_vector_len, std::vector<float> abs_of_target_vectors, float abs_of_test_vector)
{
    int max_idx;
    float max_sim = -1e10;
    float s = 0;
    for(int i = 0; i < num_of_targets; i++)
	{
		s = inner_product(target_vectors[i], test_vector, feat_vector_len); 
		s /= abs_of_target_vectors[i];
		s /= abs_of_test_vector;
		    if (s > max_sim)
			{
				max_sim = s;
				max_idx = i;
			}
    }
#ifdef ANDROID
    ALOG("--- max cos similarity: %f\n", max_sim);
#else
	printf("--- max cos similarity: %f\n", max_sim);
#endif
    if(max_sim < 0.72)
        return -1;
    return max_idx;
}

void load_vectors(char* directory, std::vector<char*> &filename, int num_of_files, std::vector<float*> &target_vectors, int feat_vector_len, std::map<string, int>& vector_count_by_name)
{
    char dir_vector[256];
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory)) != NULL) {
        int i = 0;
        while (((ent = readdir (dir)) != NULL) && (i < num_of_files)) {
	    string s1 = ent->d_name;
	    string s2 = ".csv";
	    if (s1.find(s2) != std::string::npos) {
		strcpy(filename[i], ent->d_name);
                strcpy(dir_vector, directory);
                strcat(dir_vector, ent->d_name);
        	read_fc7_vector(dir_vector, target_vectors[i], feat_vector_len);
		i++;

	        s1.erase(s1.length()-9, 9); // remove _xxxx.csv
	        std::map<string, int>::iterator it = vector_count_by_name.find(s1);
	        if ( it != vector_count_by_name.end()){
	            vector_count_by_name[it->first]++;
	        }else{
	            vector_count_by_name.insert(std::pair<string, int>(s1, 0));
	        }
            }
      }
      closedir(dir);
    } else {
      printf("Failed to open directory!");
    }

    std::map<string, int>::iterator it;
    for( it = vector_count_by_name.begin(); it != vector_count_by_name.end(); it++)
	printf("name: %s, count: %d\n", it->first.c_str(), it->second);

    return;
}

int get_num_of_files(char* directory, string s)
{
    int num_of_files = 0;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(directory)) != NULL){
        /* retrieve all .csv files within directory */
        while((ent = readdir (dir)) != NULL){
	    string s1 = ent->d_name;
	    string s2 = s;
	    if(s1.find(s2) != std::string::npos){
		num_of_files++;
            }
        }
        closedir(dir);
    }else{
        printf("Failed to open directory!");
    }
    return num_of_files;
}


bool GtiResize(unsigned char* imgIn, int inHeight, int inWidth, unsigned char* imgOut, int outHeight, int outWidth)
{
    if(imgIn == NULL) return false;

    double scaleWidth =  (double)outWidth / (double)inWidth;
    double scaleHeight = (double)outHeight / (double)inHeight;

    for(int cy = 0; cy < outHeight; cy++)
    {
        for(int cx = 0; cx < outWidth; cx++)
        {
            int pixel = (cy * (outWidth *3)) + (cx*3);
            int nearestMatch =  (((int)(cy / scaleHeight) * (inWidth *3)) + ((int)(cx / scaleWidth) *3) );
            
            imgOut[pixel    ] =  imgIn[nearestMatch    ];
            imgOut[pixel + 1] =  imgIn[nearestMatch + 1];
            imgOut[pixel + 2] =  imgIn[nearestMatch + 2];
        }
    }

    return true;
}

bool GtiCvtColor(unsigned char* imgIn, int imgHeight, int imgWidth, unsigned char* imgOut)
{
    if(imgIn == NULL) return false;

    for(int cy = 0; cy < imgHeight; cy++)
    {
        for(int cx = 0; cx < imgWidth; cx++)
        {
            int pixel = (cy * (imgWidth *3)) + (cx*3);
            imgOut[cy*imgWidth + cx ] =  imgIn[pixel + 0]; // R 
            imgOut[1*imgHeight*imgWidth + cy*imgWidth + cx ] =  imgIn[pixel + 1]; // G
            imgOut[2*imgHeight*imgWidth + cy*imgWidth + cx ] =  imgIn[pixel + 2]; // B
        }
    }

    return true;
}

void get_filenames(char* directory, char** filename, int num_of_files, string s)
{
    char dir_vector[256];
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory)) != NULL) {
        int i = 0;
        while (((ent = readdir (dir)) != NULL) && (i < num_of_files)) {
	    string s1 = ent->d_name;
	    string s2 = s;
	    if (s1.find(s2) != std::string::npos) {
		strcpy(filename[i], ent->d_name);
                strcpy(dir_vector, directory);
                strcat(dir_vector, ent->d_name);
		strcpy(filename[i], dir_vector);
		i++;
            }
      }
      closedir(dir);
    } else {
      printf("Failed to open directory!");
    }
    return;
}

int update_vector_count_by_name(std::map<string, int> &vector_count_by_name, char* name)
{
    int count = 0;
    string s = string(name); //s.assign(name);
    std::map<string, int>::iterator it;
    it = vector_count_by_name.find(s);
    if(it != vector_count_by_name.end()){
	vector_count_by_name[it->first]++; 
    }else{
	vector_count_by_name.insert(std::pair<string, int>(s, 0));
    } 
    return count;
}


int get_vector_count_by_name(std::map<string, int> vector_count_by_name, char* name)
{
    int count = 0;
    string s = string(name); //s.assign(name);
    std::map<string, int>::iterator it;
    it = vector_count_by_name.find(s);
    if(it != vector_count_by_name.end()){
	count = it->second; 
    }else{
	count = 0;
    } 
    return count;
}

#endif

//====================================================================
// Function name: Samples()
// Class Samples construction.
// This function calls GTI library APIs to set device type, open device,
// initialize GTI SDK library. It also allocates memory for the sample
// code.
//====================================================================
Samples::Samples(int gnetType, char *coefFilename, char *configFilename, char *deviceName)
{
	char currentDir[1024];
    char *gtiHome = NULL;
    string userConfig;

    m_GnetType = gnetType;
    m_deviceName = deviceName;

#ifdef ANDROID
	gtiHome = "";
#else
    // Get GTISDK home
    gtiHome = getenv(glGtiPath.c_str());
    if (gtiHome == NULL)
    {
        printf("GTISDK home path is not set! Please see GTI SDK user guide.\n");
        exit(0);
    }
#endif

    m_SdkPath = string(gtiHome);

    if (m_GnetType == 0)
    {
#ifdef GTI2803
        m_coefName = m_SdkPath + glGti2803Gnet1Root + string(coefFilename);
        userConfig = m_SdkPath + glGti2803Gnet1Root + string(configFilename);
        m_picFcname = m_SdkPath + gl2803PicCoefDataFcFileName;
        m_picLabelname = m_SdkPath + gl2803PicLabelFileName;
#else        
        m_coefName = m_SdkPath + glGnet1CnnRoot + string(coefFilename);
        userConfig = m_SdkPath + glGnet1CnnRoot + string(configFilename);
        m_picFcname = m_SdkPath + glPicCoefDataFcFileName;
        m_picLabelname = m_SdkPath + glPicLabelFileName;
#endif
        m_videoFcname = m_SdkPath + glVideoCoefDataFcFileName;
        m_videoLabelname = m_SdkPath + glVideoLabelFileName;
        m_camFcname = m_SdkPath + glWebCamCoefDataFcFileName;
        m_camLabelname = m_SdkPath + glWebCamLabelFileName;
        m_camFc_pets = m_SdkPath + glWebCamFcPets;
        m_camLabel_pets = m_SdkPath + glWebCamLabelPets;
        m_Fc_office = m_SdkPath + glGnet2_3Fc_10office;
        m_Label_office = m_SdkPath + glGnet2_3Fc_10officeLabel;
        m_Fc_kitchen = m_SdkPath + glGnet2_3Fc_10kitchen;
        m_Label_kitchen = m_SdkPath + glGnet2_3Fc_10kitchenLabel;
        m_Fc_livingroom = m_SdkPath + glGnet2_3Fc_10livingroom;
        m_Label_livingroom = m_SdkPath + glGnet2_3Fc_10livingroomLabel;

#ifdef FACE_RECOGNITION
        m_coefName =  glGtiCoefFileNameFace;
        m_camFcnameFace =  glWebCamCoefDataFcFileNameFace;
        m_camLabelnameFace =  glWebCamLabelFileNameFace;
#ifdef ANDROID
        ALOG("GTI_JNI: m_coefName = %s, m_camFcnameFace = %s", m_coefName.c_str(), m_camFcnameFace.c_str());
#endif
#endif
    }
    else
    {
        m_coefName = m_SdkPath + glGnet2_3CnnRoot + string(coefFilename);
        userConfig = m_SdkPath + glGnet2_3CnnRoot + string(configFilename);
        m_picFcname = m_SdkPath + glPicCoefDataFcGnet32;
        m_picLabelname = m_SdkPath + glPicLabelGnet32;
        m_videoFcname = m_SdkPath + glVideoCoefDataFcGnet32;
        m_videoLabelname = m_SdkPath + glVideoLabelGnet32;
        m_camFcname = m_SdkPath + glWebCamCoefDataFcGnet32;
        m_camLabelname = m_SdkPath + glWebCamLabelGnet32;
        m_camFc_pets = m_SdkPath + glGnet2_3WebCamFcPets;
        m_camLabel_pets = m_SdkPath + glGnet2_3WebCamLabelPets;
        m_Fc_office = m_SdkPath + glGnet2_3Fc_10office;
        m_Label_office = m_SdkPath + glGnet2_3Fc_10officeLabel;
        m_Fc_kitchen = m_SdkPath + glGnet2_3Fc_10kitchen;
        m_Label_kitchen = m_SdkPath + glGnet2_3Fc_10kitchenLabel;
        m_Fc_livingroom = m_SdkPath + glGnet2_3Fc_10livingroom;
        m_Label_livingroom = m_SdkPath + glGnet2_3Fc_10livingroomLabel;
    }

    // Create GTI device with EUSB interface
#if defined(USE_EUSB)
	{
#ifdef ANDROID
	   ALOG("GTI_JNI: Samples::Samples create EUSB device, %s, %s.\n",(char *)m_coefName.c_str(), (char *)userConfig.c_str());
#endif    
	   m_Device = GtiDeviceCreate(EUSB, (char *)m_coefName.c_str(), (char *)userConfig.c_str());
	}
#elif defined(USE_PCIE)
	{
		m_Device = GtiDeviceCreate(PCIE, (char *)m_coefName.c_str(), (char *)userConfig.c_str());
	}
#else
	{
#ifdef ANDROID
	   ALOG("GTI_JNI: Samples::Samples create FTDI device, %s, %s.\n",(char *)m_coefName.c_str(), (char *)userConfig.c_str());
#endif    
        m_Device = GtiDeviceCreate(FTDI, (char *)m_coefName.c_str(), (char *)userConfig.c_str());
	}
#endif

    // Open device
    GtiOpenDevice(m_Device, deviceName);

    // Initialize GTI SDK
    if (!GtiInitialization(m_Device))
    {
#ifdef ANDROID
        ALOG("\n\n\nGTI initialization failed.");
#endif        

        std::cout << "GTI initialization failed." << std::endl;
    }

    // Allocate memory for sample code use
    m_ImageOutPtr = new float[GtiGetOutputLength(m_Device)];
    if (!m_ImageOutPtr)
    {
        std::cout << "GTI allocation (m_ImageOutPtr) failed." << std::endl;
#ifdef ANDROID
        ALOG("\n\n\n------- GTI allocation (m_ImageOutPtr) failed.");
#endif        
    }

//    printf("GtiGetOutputLength = %d\n", GtiGetOutputLength);

    m_Buffer32FC3 = new float[GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * GTI_IMAGE_CHANNEL];
    if (!m_Buffer32FC3)
    {
        std::cout << "GTI allocation (m_Buffer32FC3) failed." << std::endl;
    }

    m_Img224x224x3Buffer = new BYTE[GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * GTI_IMAGE_CHANNEL];
    if (!m_Img224x224x3Buffer)
    {
        std::cout << "GTI allocation (m_Img224x224x3Buffer) failed." << std::endl;
    }

    m_FileNameBuffer = new TCHAR[MAX_FILENAME_SIZE];
    if (!m_FileNameBuffer)
    {
        std::cout << "GTI allocation (m_FileNameBuffer) failed." << std::endl;
    }

#ifdef ANDROID
    ALOG("GTI_JNI: Samples::Samples complete.\n");
#endif    
}

//====================================================================
// Function name: ~Samples()
// Class Samples destruction.
// This function releases the memory allcated in Samples(), 
// it also calls GTI library API to close device.
//====================================================================
Samples::~Samples()
{

    safeRelease(m_ImageOutPtr);
    safeRelease(m_Img224x224x3Buffer);
    safeRelease(m_Buffer32FC3);
    safeRelease(m_FileNameBuffer);

    // Close device
    GtiCloseDevice(m_Device);
    // Release device
    GtiDeviceRelease(m_Device);

#ifdef ANDROID
    if(gClassify)
        GtiClassifyRelease(gClassify);    
#endif
}

#ifdef ANDROID
//====================================================================
// Function name: oneFramePredictInit()
// This function sets FC and label name for image, video or camera classifer.
//
// Input: none.
// return none.
//====================================================================
void Samples::oneFramePredictInit(int type)
{
    if(type == 0) //Image
	{
		ALOG("Samples::oneFramePredictInit, %s, %s\n", m_picFcname.c_str(), m_picLabelname.c_str());
        gClassify = GtiClassifyCreate(m_picFcname.c_str(), m_picLabelname.c_str());
	}
    else if(type == 1) //Video
	{
		ALOG("Samples::oneFramePredictInit, %s, %s\n", m_camFcname.c_str(), m_camLabelname.c_str());
     //   gClassify = new Classify (m_videoFcname, m_videoLabelname);
        gClassify = GtiClassifyCreate (m_camFcname.c_str(), m_camLabelname.c_str());
	}
    else if(type == 2) //Camera
	{
		ALOG("Samples::oneFramePredictInit, %s, %s\n", m_camFcname.c_str(), m_camLabelname.c_str());
        gClassify = GtiClassifyCreate (m_camFcname.c_str(), m_camLabelname.c_str());
	}
    else if(type == 3) //Face
    {
		ALOG("Samples::oneFramePredictInit, %s, %s\n", m_camFcnameFace.c_str(), m_camLabelnameFace.c_str());
        gClassify = GtiClassifyCreate (m_camFcnameFace.c_str(), m_camLabelnameFace.c_str());
        face_recognition_init(gClassify);
    }
    else
        ALOG("Samples::oneFramePredictInit type not supported. Type = %d", type);

    ALOG("Samples::oneFramePredictInit. Type = %d (image, video, camera)", type);

}
//====================================================================
// Function name: oneFramePredict(unsigned char *imgData, string* predictions)
// This function inputs picture image, sends the image to GTI FPGA,
// gets output from the FPGA, calls Classify functions to get the image
// class name and index.
//
// Input: none.
// return none.
//====================================================================

float frameRate = 0.0;
float avgCnnProcessTime = 0.;
float accumlatedCnnProcessTime = 0.;
float avgFcProcessTime = 0.;
float accumlatedFcProcessTime = 0.;
float cnnProcessTime_ms = 0;
float fcProcessTime_ms = 0;
char  charbuf[256];
int   frame_num = 0;
float accumlatedRecognitionProcessTime = 0.;
float avgRecognitionProcessTime = 0.;
float recognitionProcessTime = 0.;


void Samples::facePredict(unsigned char *imgData, int predict, string* predictions, char *name)
{
    if(predict == 0) //Register nwe user
    {
        face_recognition_register(gClassify, imgData, name);
        return;
    }

    else if(predict == 1)
    {
        const int FRAME_COUNT = 30;

        static int imgCnt = 0;

        char filename[30];
        if (imgCnt < 10)
        {
            sprintf(filename, "/sdcard/gti/tmp_%d.bgr", imgCnt++ % 10);

            FILE *file = fopen(filename, "wb" );

            fwrite(imgData, 1, 224 * 224 * 3, file);
            fclose(file);
        }
           
        m_CnnTimerStart = GetTickCount64();

        convolution(imgData, GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * 3);

        m_CnnTimerEnd = GetTickCount64();
        m_FcTimerStart = m_CnnTimerEnd;

        gClassify->GtiClassify(m_ImageOutPtr, 1);
        
        m_FcTimerEnd = GetTickCount64();

        cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
        fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
        accumlatedCnnProcessTime += cnnProcessTime_ms;
        accumlatedFcProcessTime += fcProcessTime_ms;

        //ALOG("CNN=%2.0fms, FC=%2.0fms", cnnProcessTime_ms, fcProcessTime_ms);
        frame_num++;
        
        // get feature vector
        float* fc7_out = GetFeatureVector(gClassify); // gtiClassify->m_Fc7_out
        for (unsigned int i = 0; i < feat_vector_len; ++i) {
            feat_vector[i] = fc7_out[i];
        }
       
        // calculate similarity and predict
        if(num_of_targets != 0){
            float abs_of_feat_vector;
            int max_idx;
            abs_of_feat_vector = abs_of_vector(feat_vector, feat_vector_len);
            max_idx = get_max_cos_sim_idx(target_vectors, num_of_targets, feat_vector, feat_vector_len, abs_of_target_vectors, abs_of_feat_vector);
            if(max_idx == -1)
                pred_name = string("");
            else
            {
                pred_name = string(filename_target_vectors[max_idx]);
                pred_name.erase(pred_name.length()-9, 9); // remove _xxxx.csv
            }
            ALOG("----------- Max_idx = %d, pred = %s\n", max_idx, pred_name.c_str());
        }else{
            pred_name = string("");
        }

        predictions[0] = pred_name;

        accumlatedRecognitionProcessTime +=  (GetTickCount64() - m_FcTimerEnd);

            // calculate frame rate every 10 frames
        if (frame_num % FRAME_COUNT == 0) 
        {
            m_SvicTimerEnd = GetTickCount64();

            avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
            accumlatedCnnProcessTime = 0.;
            avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
            accumlatedFcProcessTime = 0.;

            avgRecognitionProcessTime = accumlatedRecognitionProcessTime / FRAME_COUNT;
            accumlatedRecognitionProcessTime = 0.;           

            frameRate = (1000 / (avgCnnProcessTime + avgFcProcessTime));
            sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f ms, FC: %5.2f ms, Recognition: %5.2f ms",
                        frameRate, avgCnnProcessTime, avgFcProcessTime, avgRecognitionProcessTime);
            ALOG("--- %s", charbuf);
        }
    }
}


void Samples::oneFramePredict(unsigned char *imgData, string* predictions)
{
 //   ALOG("--- ENTER Samples::oneFramePredict.\n");
    const int FRAME_COUNT = 30;
    m_CnnTimerStart = GetTickCount64();

    convolution(imgData, GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * 3);

    m_CnnTimerEnd = GetTickCount64();
    m_FcTimerStart = m_CnnTimerEnd;

    gClassify->GtiClassify(m_ImageOutPtr, 3);

    m_FcTimerEnd = GetTickCount64();

    cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
    fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
    accumlatedCnnProcessTime += cnnProcessTime_ms;
    accumlatedFcProcessTime += fcProcessTime_ms;

    //ALOG("CNN=%2.0fms, FC=%2.0fms", cnnProcessTime_ms, fcProcessTime_ms);

    frame_num++;

// calculate frame rate every 10 frames
    if (frame_num % FRAME_COUNT == 0) 
    {
        m_SvicTimerEnd = GetTickCount64();

        avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
        accumlatedCnnProcessTime = 0.;
        avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
        accumlatedFcProcessTime = 0.;

        frameRate = (1000 / (avgCnnProcessTime + avgFcProcessTime));
        sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f ms, FC: %5.2f ms",
                    frameRate, avgCnnProcessTime, avgFcProcessTime);
        ALOG("--- %s", charbuf);
    }
 
    /* Print the top N predictions. */

    //for (int i = 0; i < gClassify->GtiGetPredicationSize(); ++i)
    for (int i = 0; i < 2; ++i)
    {
      //  char *ptext = gClassify->GtiGetPredicationString(i);
//        ALOG("--- ENTER Samples::oneFramePredict prediction[%d].\n", i);

      //  std::cout << std::fixed << std::setprecision(4) << ptext << std::endl;
        predictions[i] = gClassify->GtiGetPredicationString(i);
      //  ALOG("--- Prediction: [%s] \n", predictions[i].c_str());

        //std::to_string(m_Predications[Index].second) + string(" - ") + m_Predications[Index].first;
    }
}
#endif

//====================================================================
// Function name: Test_picture()
// This function inputs picture image, sends the image to GTI FPGA,
// gets output from the FPGA, calls Classify functions to get the image
// class name and index.
//
// Input: none.
// return none.
//====================================================================
void Samples::Test_picture()
{
    int key = 0;
    Classify* gtiClassify = GtiClassifyCreate(m_picFcname.c_str(), m_picLabelname.c_str());

    cv::namedWindow(m_ImageName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_ImageName, 800, 600);
    cv::moveWindow(m_ImageName, 400, 100);

    while (1)
    {
        m_filename = openFileDialog(GTI_PICTURE);
        if (m_filename.size() == 0)
        {
            break;
        }
        std::cout << "---------- Prediction for " << m_filename << " ----------" << std::endl;

        m_img = cv::imread(m_filename, -1);
        if (m_img.empty())
        {
            std::cout << "File is not found or not supported." << std::endl;
            break;
        }

        cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));

        convolution(m_img1);

        GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);

        displayImage(m_img, gtiClassify, GTI_PICTURE);

        /* Print the top N predictions. */
        for (int i = 0; i < 5; ++i)
        {
            char *ptext = GetPredicationString(gtiClassify, i);
            std::cout << std::fixed << std::setprecision(4) << ptext << std::endl;
        }

        key = cv::waitKey(0) & 0xff;
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }

        std::cout << "End of process image " << m_filename << std::endl;
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Slideshow_picture(int hierarchyFlag)
// This function inputs picture image from a directory, sends the image
// to GTI FPGA, gets output from the FPGA, calls Classify functions to
// get the image class name and index.
//
// Input: int hierarchyFlag - 1: hierarchy, 0: no hierarchy.
// return none.
//====================================================================
void Samples::Slideshow_picture(int hierarchyFlag)
{
    int key = 0, subType, pauseFlag = 0;
    struct dirent *ent;
#ifdef LINUX
    DIR *dir;
#endif
#ifdef WIN32
    HANDLE dir;
#endif

    Classify *gtiClassify = GtiClassifyCreate(m_picFcname.c_str(), m_picLabelname.c_str());
    Classify *petsClassify = NULL;
    Classify *officeClassify = NULL;
    Classify *kitchenClassify = NULL;
    Classify *livingroomClassify = NULL;
    if (hierarchyFlag == 1)
    {
        petsClassify = GtiClassifyCreate(m_camFc_pets.c_str(), m_camLabel_pets.c_str());
        officeClassify = GtiClassifyCreate(m_Fc_office.c_str(), m_Label_office.c_str());
        kitchenClassify = GtiClassifyCreate(m_Fc_kitchen.c_str(), m_Label_kitchen.c_str());
        livingroomClassify = GtiClassifyCreate(m_Fc_livingroom.c_str(), m_Label_livingroom.c_str());
    }

    cv::namedWindow(m_ImageName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_ImageName, 800, 600);
    cv::moveWindow(m_ImageName, 400, 100);

    m_ImageFolder = m_SdkPath + glImageFolder;
    while (1)
    {
        dir = opendir(m_ImageFolder.c_str());
        if (NULL == dir)
        {
            /* could not open directory */
            printf("Open Image_bmp folder error.\n");
            perror ("");
            break;
        }

        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            string imageFilename;

            m_filename = ent->d_name;

            string strFilename=m_filename;
            if(strFilename.compare(".")==0) continue;
            if(strFilename.compare("..")==0) continue;

            if (m_filename.size() == 0)
            {
                continue;
            }

#ifdef WIN32
            imageFilename = m_ImageFolder + "\\" + m_filename;
#else
            imageFilename = m_ImageFolder + "/" + m_filename;
#endif
            //std::cout << "---------- Prediction for " << m_filename << " ----------" << std::endl;

            m_img = cv::imread(imageFilename, -1);
            if (m_img.empty())
            {
                //std::cout << "File not found." << std::endl;
                continue;
            }

            cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
            while (1)
            {
                convolution(m_img1);

                if (hierarchyFlag == 1)
                {
                    subType = GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
                    switch(subType)
                    {
                    case PETS_TYPE:
                        GtiClassifyFC(petsClassify, m_ImageOutPtr, 5);
                        displayImage(m_img, petsClassify, GTI_PICTURE, "", 1, 0, "Pets");
                        break;
                    case OFFICE_TYPE:
                        GtiClassifyFC(officeClassify, m_ImageOutPtr, 5);
                        displayImage(m_img, officeClassify, GTI_PICTURE, "", 1, 0, "Office");
                        break;
                    case KITCHEN_TYPE:
                        GtiClassifyFC(kitchenClassify, m_ImageOutPtr, 5);
                        displayImage(m_img, kitchenClassify, GTI_PICTURE, "", 1, 0, "Kitchen");
                        break;
                    case LIVINGROOM_TYPE:
                        GtiClassifyFC(livingroomClassify, m_ImageOutPtr, 5);
                        displayImage(m_img, livingroomClassify, GTI_PICTURE, "", 1, 0, "Living room");
                        break;
                    default:
                        displayImage(m_img, gtiClassify, GTI_PICTURE);
                        break;
                    }
                }
                else
                {
                    GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
                    displayImage(m_img, gtiClassify, GTI_PICTURE, imageFilename); 
                }

                key = cv::waitKey(1000) & 0xff;
                if (key == 'q' || key == 27)    // ESC key 27
                {
                    break;
                }
                else if (key == ' ')
                {
                    pauseFlag = ~pauseFlag;
                }
                if (!pauseFlag)
                {
                    break;
                }
            }
            if (key == 'q' || key == 27)    // ESC key 27
            {
                break;
            }
            //std::cout << "End of process image " << m_filename << std::endl;
        }

        closedir (dir);

        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
    if (hierarchyFlag == 1)
    {
        GtiClassifyRelease(petsClassify);
        GtiClassifyRelease(officeClassify);
        GtiClassifyRelease(kitchenClassify);
        GtiClassifyRelease(livingroomClassify);
    }
}

#ifdef FACE_RECOGNITION

void Samples::face_recognition_init(Classify* gtiClassify)
{
    // set directory of target vectors and photos
    dir_target_vectors = new char[128];
    strcpy(dir_target_vectors, "/sdcard/gti/face/Data/vectors/");
    dir_target_photos = new char[128];
    strcpy(dir_target_photos, "/sdcard/gti/face/Data/photos/");

    // get feature vector length
    feat_vector_len = GetFeatureVectorLength(gtiClassify);

    // get number of target vectors stored
    num_of_targets = get_num_of_files(dir_target_vectors, ".csv"); // num of persons in pool

    // allocate memory for file names of target vectors 
    for(int i = 0; i < num_of_targets; i++){
	char* fn_ptr = new char[128];
        filename_target_vectors.push_back(fn_ptr);
    }    

    // allocate memory for target vectors 
    for(int i = 0; i < num_of_targets; i++){
	float* v_ptr = new float[feat_vector_len];
        target_vectors.push_back(v_ptr);
    }                                              

    // allocate memory for abs of target vectors 
    for(int i = 0; i < num_of_targets; i++){
        abs_of_target_vectors.push_back(0.0);
    }                                              

    // allocate memory for instant feature vector
    feat_vector = new float[feat_vector_len]; // feature vector

    // load target vectors
    load_vectors(dir_target_vectors, filename_target_vectors, num_of_targets, target_vectors, feat_vector_len, vector_count_by_name);

    // pre-calculate abs of target vectors
    for(int i = 0; i < num_of_targets; i++){
        abs_of_target_vectors[i] = abs_of_vector(target_vectors[i], feat_vector_len);
    }

#ifdef ANDROID
    ALOG("GTI_JNI: Face num_of_targets = %d, feat_vector_len = %d\n", num_of_targets, feat_vector_len);
#else
	printf("Face num_of_targets = %d, feat_vector_len = %d\n", num_of_targets, feat_vector_len);
#endif
    return;
}

void Samples::face_recognition_exec(Classify* gtiClassify, cv::Mat img_face)
{
   
    return;
}

void Samples::Test_face_recognition()
{
 
}

void Samples::face_recognition_register(Classify* gtiClassify, unsigned char *img_face, char* name)
{
    // face image path
    char photo_path[128];
    char fn_photo[128];

    // feature vector path
    char feat_vector_path[256];

    // CNN
    convolution(img_face, GTI_IMAGE_WIDTH * GTI_IMAGE_HEIGHT * 3);

    // FC
    GtiClassifyFC(gtiClassify, m_ImageOutPtr, 1);
    
    // get feature vector
    float* v_out = GetFeatureVector(gtiClassify); // gtiClassify->m_Fc7_out
    float* feat_vector = new float[feat_vector_len]; // feature vector
    for (int i = 0; i < feat_vector_len; ++i) {
        feat_vector[i] = v_out[i];
    }

    // insert new feature vector
    target_vectors.push_back(feat_vector); // add the new feat vector into the pool of target vectors

    // insert abs of new feature vector 
    abs_of_target_vectors.push_back(abs_of_vector(feat_vector, feat_vector_len)); // calculate the abs of the newly added vector

    // update number of target feature vectors
    num_of_targets++;

    //update vector count of each person 
    update_vector_count_by_name(vector_count_by_name, name);

    // get feature vector filename 
    char* fn_vector = new char[128];
    sprintf(fn_vector, "%s_%04d.csv", name, get_vector_count_by_name(vector_count_by_name, name));

    // insert file name of new feature vector 
    filename_target_vectors.push_back(fn_vector);

    // save feature vector into .csv file
    strcpy(feat_vector_path, dir_target_vectors);
    strcat(feat_vector_path, fn_vector);
    FILE* fp = fopen(feat_vector_path, "w");	
    for (int i = 0; i < feat_vector_len; ++i) {
        fprintf(fp, "%f\n", feat_vector[i]);
    }
    fclose(fp);

    // write face image into .jpg file
    sprintf(fn_photo, "%s_%04d.bgr", name, get_vector_count_by_name(vector_count_by_name, name));
    strcpy(photo_path, dir_target_photos);
    strcat(photo_path, fn_photo);
    
    fp = fopen(photo_path, "wb");
    if(fp)
    {
#ifdef ANRDOID		
        ALOG("GTI_JNI: Cannot create file %s", photo_path);
#else
        printf("Cannot create file %s", photo_path);
#endif
        fwrite(img_face, 1, 224 * 224 * 3, fp);
		fclose(fp);
    }
    return;
}


void Samples::generate_face_feature_vectors()
{
    return;
}

//====================================================================
// Function name: void convolution_face(const cv::Mat& img)
// This function calls GTISendImage() to send image to GTI FPGA to do
// convolution.
//
// Input: Mat& img - 224 pixel x 224 pixel x 3 channels image data.
// return: none.
//====================================================================
void Samples::convolution_face(const cv::Mat& img)
{
    int bRet;
    int ConvIn_Channels = 3;
    int ConvIn_ImgSize = 224;
    int imageInLength = (ConvIn_ImgSize * ConvIn_ImgSize) * ConvIn_Channels;
    std::vector<cv::Mat> input_image;

    //cnnSvicProc8UC3(img, &input_image);
    cnnSvicProc32FC3(img, &input_image);
    cvt32FloatTo8Byte((float *)input_image[0].data, (uchar *)m_Img224x224x3Buffer, ConvIn_ImgSize, ConvIn_ImgSize, ConvIn_Channels);

    m_PureCnnStart = GetTickCount64();
    bRet = GtiHandleOneFrameFloat(m_Device, m_Img224x224x3Buffer, imageInLength, m_ImageOutPtr, GtiGetOutputLength(m_Device));
    m_PureCnnEnd = GetTickCount64();
    if (bRet == 0)
    {
        GtiCloseDevice(m_Device);
        GtiOpenDevice(m_Device, m_deviceName);
    }
    return;
}

#endif


//====================================================================
// Function name: Demo_picture(char *dispName, int x, int y, int w, int h)
// This function inputs picture image from a directory, sends the image
// to GTI FPGA, gets output from the FPGA, calls Classify functions to
// get the image class name and index.
//
// Input: 
// return none.
//====================================================================
void Samples::Demo_picture(char *dispName, int x, int y, int w, int h)
{
    int key = 0, pauseFlag = 0;
    struct dirent *ent;
#ifdef LINUX
    DIR *dir;
#endif
#ifdef WIN32
    HANDLE dir;
#endif

    Classify *gtiClassify = GtiClassifyCreate(m_picFcname.c_str(), m_picLabelname.c_str());

    cv::namedWindow(dispName, cv::WINDOW_NORMAL);
    cv::resizeWindow(dispName, w, h);
    cv::moveWindow(dispName, x, y);

    m_ImageFolder = m_SdkPath + glImageFolder;
    while (1)
    {
        dir = opendir(m_ImageFolder.c_str());
        if (NULL == dir)
        {
            /* could not open directory */
            printf("Open Image_bmp folder error.\n");
            perror ("");
            break;
        }

        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            string imageFilename;

            m_filename = ent->d_name;

            if (m_filename.size() == 0)
            {
                continue;
            }

#ifdef WIN32
            imageFilename = m_ImageFolder + "\\" + m_filename;
#else
            imageFilename = m_ImageFolder + "/" + m_filename;
#endif
            //std::cout << "---------- Prediction for " << m_filename << " ----------" << std::endl;

            m_img = cv::imread(imageFilename, -1);
            if (m_img.empty())
            {
                //std::cout << "File not found." << std::endl;
                continue;
            }

            cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
            while (1)
            {
                convolution(m_img1);

                GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
                dispDemoImage(m_img, gtiClassify, dispName, "", w, h, 0);

                key = cv::waitKey(10) & 0xff;
                if (key == 'q' || key == 27)    // ESC key 27
                {
                    break;
                }
                else if (key == ' ')
                {
                    pauseFlag = ~pauseFlag;
                }
                if (!pauseFlag)
                {
                    break;
                }
            }
            if (key == 'q' || key == 27)    // ESC key 27
            {
                break;
            }
            //std::cout << "End of process image " << m_filename << std::endl;
        }

        closedir (dir);

        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Test_video()
// This function inputs video clip image, sends the image to GTI FPGA,
// gets output from the FPGA, calls Classify functions to get the clip
// class name and index.
//
// Input: none.
// return none.
//====================================================================
void Samples::Test_video()
{
    int key = 0, resizeFlag = 0, pauseFlag = 0;
    double width, height;
    float frameRate = 0.0;
    float avgCnnProcessTime = 0.;
    float accumlatedCnnProcessTime = 0.;
    float avgFcProcessTime = 0.;
    float accumlatedFcProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    float fcProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    char charbuf[256];

    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_videoFcname.c_str(), m_videoLabelname.c_str());

    cv::namedWindow(m_VideoName, cv::WINDOW_NORMAL);
//    cv::resizeWindow(m_VideoName, 800, 600);
    cv::moveWindow(m_VideoName, 400, 100);
    while (1)
    {
        accumlatedCnnProcessTime = 0.;
        accumlatedFcProcessTime = 0.;

        sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f, FC: %5.2f",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);

        m_filename = openFileDialog(GTI_VIDEO);
        if (m_filename.size() == 0)
        {
            break;
        }
        std::cout << "Open Video file " << m_filename << "...\n";
        bool ret = cap.open(m_filename);
        std::cout << "Open Video file " << m_filename << " status: " << ret << "\n";

        if (!cap.isOpened())
        {
            cap.release();
            std::cout << "Video Clip File " << m_filename << " is not opened!\n";
            break;
        }

        width = m_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        height = m_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        cv::resizeWindow(m_VideoName, int(width), int(height));

        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "fps: " << cap.get(CV_CAP_PROP_FPS) << std::endl;
        std::cout << "frames: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

        // cv::VideoWriter  outputVideo;

        // mp4 output
        //const string name = "cnnsvic_output.mp4";
        //cv::Size size = cv::Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        //outputVideo.open(name, CV_FOURCC('M', 'P', '4', '2'), cap.get(CV_CAP_PROP_FPS), size, true);

        m_SvicTimerStart = GetTickCount64();
        int frame_num = 0;
        while (1)
        {
            if (pauseFlag == 0)
            {
                if (FALSE == cap.read(m_img))
                {
                    // reset if video playing ended
                    cap.set(CV_CAP_PROP_POS_FRAMES, 0);
                    cap.read(m_img);
                    frame_num = 0;
                }
                cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
            }

            m_CnnTimerStart = GetTickCount64();

            convolution(m_img1);

            m_CnnTimerEnd = GetTickCount64();
            m_FcTimerStart = m_CnnTimerEnd;
            GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
            m_FcTimerEnd = GetTickCount64();

            cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
            fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
			accumlatedCnnProcessTime += cnnProcessTime_ms;
			accumlatedFcProcessTime += fcProcessTime_ms;

            //if (frame_num % 20 == 0) std::cout << ".";

            displayImage(m_img, gtiClassify, GTI_VIDEO, string(charbuf), resizeFlag);

            key = cv::waitKey(1) & 0xff;
            if (key == 'q' || key == 27 || key =='n')   // Esc key 27
            {
                break;
            }
            if (key == ' ')
            {
                pauseFlag = ~pauseFlag;
            }

            frame_num++;

            // calculate frame rate every 10 frames
            if (frame_num % FRAME_COUNT == 0) {
                m_SvicTimerEnd = GetTickCount64();

                frameRate = (float)(FRAME_COUNT * 1000. / (m_SvicTimerEnd - m_SvicTimerStart));
                // Start Svic timer
                m_SvicTimerStart = GetTickCount64();

                avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
                accumlatedCnnProcessTime = 0.;
                avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
                accumlatedFcProcessTime = 0.;
                sprintf_s(charbuf, "Frame Rate: %5.2f, Process time - CNN: %5.2f ms, FC: %5.2f ms",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);
            }
        }
        std::cout << "\n";

        if (cap.isOpened())
        {
            cap.release();
        }
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Demo_video()
// This function inputs video clip image, sends the image to GTI FPGA,
// gets output from the FPGA, calls Classify functions to get the clip
// class name and index.
//
// Input: 
// return none.
//====================================================================
int demo_key, demo_pauseFlag = 0;
void Samples::Demo_video(char *videoName, char *dispName, int x, int y, int w, int h)
{
    float frameRate = 0.0;
    float avgCnnProcessTime = 0.;
    float accumlatedCnnProcessTime = 0.;
    float avgFcProcessTime = 0.;
    float accumlatedFcProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    float fcProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    char charbuf[256];

    mOpenCvMutex.lock();
    cv::VideoCapture cap;
    mOpenCvMutex.unlock();
    Classify *gtiClassify = GtiClassifyCreate(m_videoFcname.c_str(), m_videoLabelname.c_str());

    mOpenCvMutex.lock();
    cv::namedWindow(dispName, cv::WINDOW_NORMAL);
    cv::resizeWindow(dispName, w, h);
    cv::moveWindow(dispName, x, y);
    mOpenCvMutex.unlock();
    while (1)
    {
        accumlatedCnnProcessTime = 0.;
        accumlatedFcProcessTime = 0.;

        sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f, FC: %5.2f",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);

        m_filename = m_SdkPath + glVideoDemoFolder + string(videoName);
        if (m_filename.size() == 0)
        {
            break;
        }
        std::cout << "Open Video file " << m_filename << "...\n";
        mOpenCvMutex.lock();
        bool ret = cap.open(m_filename);
        std::cout << "Open Video file " << m_filename << " status: " << ret << "\n";

        if (!cap.isOpened())
        {
            cap.release();
            std::cout << "Video Clip File " << m_filename << " is not opened!\n";
            break;
        }
        mOpenCvMutex.unlock();

        m_SvicTimerStart = GetTickCount64();
        int frame_num = 0;
        while (1)
        {
            if (demo_pauseFlag == 0)
            {
               mOpenCvMutex.lock();
               if (FALSE == cap.read(m_img))
                {
                    // reset if video playing ended
                    cap.set(CV_CAP_PROP_POS_FRAMES, 0);
                    cap.read(m_img);
                    frame_num = 0;
                }
                cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
                mOpenCvMutex.unlock();
            }

            m_CnnTimerStart = GetTickCount64();

            convolution(m_img1);

            m_CnnTimerEnd = GetTickCount64();
            m_FcTimerStart = m_CnnTimerEnd;
            GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
            m_FcTimerEnd = GetTickCount64();

            cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
            fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
			accumlatedCnnProcessTime += cnnProcessTime_ms;
			accumlatedFcProcessTime += fcProcessTime_ms;

            mOpenCvMutex.lock();
            dispDemoImage(m_img, gtiClassify, dispName, string(charbuf), w, h, 1);
            
            demo_key = cv::waitKey(1) & 0xff;
            mOpenCvMutex.unlock();

            if (demo_key == 'q' || demo_key == 27 || demo_key =='n')   // Esc key 27
            {
                break;
            }
            if (demo_key == ' ')
            {
                demo_pauseFlag = ~demo_pauseFlag;
            }

            frame_num++;

            // calculate frame rate every 10 frames
            if (frame_num % FRAME_COUNT == 0) {
                m_SvicTimerEnd = GetTickCount64();

                frameRate = (float)(FRAME_COUNT * 1000. / (m_SvicTimerEnd - m_SvicTimerStart));
                // Start Svic timer
                m_SvicTimerStart = GetTickCount64();

                avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
                accumlatedCnnProcessTime = 0.;
                avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
                accumlatedFcProcessTime = 0.;
                sprintf_s(charbuf, "Frame Rate: %5.2f, Process time - CNN: %5.2f ms, FC: %5.2f ms",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);
            }
        }
        std::cout << "\n";

        mOpenCvMutex.lock();
        if (cap.isOpened())
        {
            cap.release();
        }
        mOpenCvMutex.unlock();
        if (demo_key == 'q' || demo_key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Video_Hierarchy(int DemoFlag)
// This function inputs video clip image, sends the image to GTI FPGA,
// gets output from the FPGA, calls Classify functions with hierarchy
// to get the clip class name and index.
//
// Input: int DemoFlag - 1: no need to input file name for demo
//                       0: need to input file name.
// return none.
//====================================================================
void Samples::Video_Hierarchy(int DemoFlag)
{
    int key = 0, subType, resizeFlag = 0, pauseFlag = 0;
    double width, height;
    float frameRate = 0.0;
    float avgCnnProcessTime = 0.;
    float accumlatedCnnProcessTime = 0.;
    float avgFcProcessTime = 0.;
    float accumlatedFcProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    float fcProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    char charbuf[256];

    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_camFcname.c_str(), m_camLabelname.c_str());
    Classify *petsClassify = GtiClassifyCreate(m_camFc_pets.c_str(), m_camLabel_pets.c_str());
    Classify *officeClassify = GtiClassifyCreate(m_Fc_office.c_str(), m_Label_office.c_str());
    Classify *kitchenClassify = GtiClassifyCreate(m_Fc_kitchen.c_str(), m_Label_kitchen.c_str());
    Classify *livingroomClassify = GtiClassifyCreate(m_Fc_livingroom.c_str(), m_Label_livingroom.c_str());

    cv::namedWindow(m_VideoName, cv::WINDOW_NORMAL);
    cv::moveWindow(m_VideoName, 400, 100);
    while (1)
    {
        accumlatedCnnProcessTime = 0.;
        accumlatedFcProcessTime = 0.;

        if (DemoFlag)
        {
            sprintf_s(charbuf, "fps:%.2f", frameRate);
            m_filename = m_SdkPath + glVideoDemoFileName;
        }
        else
        {
            sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f, FC: %5.2f",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);
            m_filename = openFileDialog(GTI_VIDEO);
            if (m_filename.size() == 0)
            {
                break;
            }
        }

        std::cout << "Open Video file " << m_filename << "...\n";
        bool ret = cap.open(m_filename);
        std::cout << "Open Video file " << m_filename << " status: " << ret << "\n";

        if (!cap.isOpened())
        {
            cap.release();
            std::cout << "Video Clip File " << m_filename << " is not opened!\n";
            break;
        }

        width = m_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        height = m_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        cv::resizeWindow(m_VideoName, int(width), int(height));

        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "fps: " << cap.get(CV_CAP_PROP_FPS) << std::endl;
        std::cout << "frames: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

        // cv::VideoWriter  outputVideo;

        // mp4 output
        //const string name = "cnnsvic_output.mp4";
        //cv::Size size = cv::Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        //outputVideo.open(name, CV_FOURCC('M', 'P', '4', '2'), cap.get(CV_CAP_PROP_FPS), size, true);

        m_SvicTimerStart = GetTickCount64();
        int frame_num = 0;
        while (1)
        {
            if (pauseFlag == 0)
            {
                if (FALSE == cap.read(m_img))
                {
                    // reset if video playing ended
                    cap.set(CV_CAP_PROP_POS_FRAMES, 0);
                    cap.read(m_img);
                    frame_num = 0;
                }
                cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
            }

            m_CnnTimerStart = GetTickCount64();

            convolution(m_img1);

            m_CnnTimerEnd = GetTickCount64();
            m_FcTimerStart = m_CnnTimerEnd;
            subType = GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
            switch(subType)
            {
            case PETS_TYPE:
                GtiClassifyFC(petsClassify, m_ImageOutPtr, 5);
                m_FcTimerEnd = GetTickCount64();
                displayImage(m_img, petsClassify, GTI_VIDEO, string(charbuf), resizeFlag, DemoFlag, "Pets");
                break;
            case OFFICE_TYPE:
                GtiClassifyFC(officeClassify, m_ImageOutPtr, 5);
                m_FcTimerEnd = GetTickCount64();
                displayImage(m_img, officeClassify, GTI_VIDEO, string(charbuf), resizeFlag, DemoFlag, "Office");
                break;
            case KITCHEN_TYPE:
                GtiClassifyFC(kitchenClassify, m_ImageOutPtr, 5);
                m_FcTimerEnd = GetTickCount64();
                displayImage(m_img, kitchenClassify, GTI_VIDEO, string(charbuf), resizeFlag, DemoFlag, "Kitchen");
                break;
            case LIVINGROOM_TYPE:
                GtiClassifyFC(livingroomClassify, m_ImageOutPtr, 5);
                m_FcTimerEnd = GetTickCount64();
                displayImage(m_img, livingroomClassify, GTI_VIDEO, string(charbuf), resizeFlag, DemoFlag, "Living room");
                break;
            default:
                m_FcTimerEnd = GetTickCount64();
                displayImage(m_img, gtiClassify, GTI_VIDEO, string(charbuf), resizeFlag, DemoFlag);
                break;
            }

            cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
            fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
			accumlatedCnnProcessTime += cnnProcessTime_ms;
			accumlatedFcProcessTime += fcProcessTime_ms;

            key = cv::waitKey(1) & 0xff;
            if (key == 'q' || key == 27 || key =='n')   // Esc key 27
            {
                break;
            }
            if (key == ' ')
            {
                pauseFlag = ~pauseFlag;
            }

            frame_num++;

            // calculate frame rate every 10 frames
            if (frame_num % FRAME_COUNT == 0) {
                m_SvicTimerEnd = GetTickCount64();

                frameRate = (float)(FRAME_COUNT * 1000. / (m_SvicTimerEnd - m_SvicTimerStart));
                // Start Svic timer
                m_SvicTimerStart = GetTickCount64();

                avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
                accumlatedCnnProcessTime = 0.;
                avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
                accumlatedFcProcessTime = 0.;
                if (DemoFlag)
                {
                    sprintf_s(charbuf, "fps:%.2f", frameRate);
                }
                else
                {
                    sprintf_s(charbuf, "Frame Rate: %5.2f, Process time - CNN: %5.2f ms, FC: %5.2f ms",
                            frameRate, avgCnnProcessTime, avgFcProcessTime);
                }
            }
        }
        std::cout << "\n";

        if (cap.isOpened())
        {
            cap.release();
        }
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
    GtiClassifyRelease(petsClassify);
    GtiClassifyRelease(officeClassify);
    GtiClassifyRelease(kitchenClassify);
    GtiClassifyRelease(livingroomClassify);
}

//====================================================================
// Function name: Slideshow_video()
// This function slideshows video with FPGA output.
//
// Input: none.
// return none.
//====================================================================
void Samples::Slideshow_video()
{
    int key = 0, resizeFlag = 0, pauseFlag = 0;
    double width, height;
#ifdef LINUX
    DIR *dir;
#endif
#ifdef WIN32
    HANDLE dir;
#endif
    struct dirent *ent;
    float frameRate = 0.0;
    float avgCnnProcessTime = 0.;
    float accumlatedCnnProcessTime = 0.;
    float avgFcProcessTime = 0.;
    float accumlatedFcProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    float fcProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    char charbuf[256];

    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_videoFcname.c_str(), m_videoLabelname.c_str());

    cv::namedWindow(m_VideoName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_VideoName, 800, 600);
    cv::moveWindow(m_VideoName, 440, 120);

    sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f, FC: %5.2f",
                        frameRate, avgCnnProcessTime, avgFcProcessTime);

    m_VideoFolder = m_SdkPath + glVideoFolder;

    while (1)
    {
        dir = opendir(m_VideoFolder.c_str());
        if (NULL == dir)
        {
            /* could not open directory */
            printf("Open Image_mp4 folder error.\n");
            perror ("");
            break;
        }

        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            string imageFilename;

            accumlatedCnnProcessTime = 0.;
            accumlatedFcProcessTime = 0.;

            m_filename = ent->d_name;

            if (m_filename.size() == 0)
            {
                continue;
            }

#ifdef WIN32
            imageFilename = m_VideoFolder + "\\" + m_filename;
#else
            imageFilename = m_VideoFolder + "/" + m_filename;
#endif
            std::cout << "Open Video file " << imageFilename << "...\n";
            cap.open(imageFilename);

            if (!cap.isOpened())
            {
                std::cout << "Video Clip File " << imageFilename << " is not opened!\n";
                continue;
            }

            width = m_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
            height = m_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
            cv::resizeWindow(m_VideoName, int(width), int(height));

            m_SvicTimerStart = GetTickCount64();
            int frame_num = 0;
            while (1)
            {
                if (pauseFlag == 0)
                {
                    if (0 == cap.read(m_img))
                    {
                        break;
                    }
                    cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
                }

                m_CnnTimerStart = GetTickCount64();

                convolution(m_img1);

                m_CnnTimerEnd = GetTickCount64();
                m_FcTimerStart = m_CnnTimerEnd;
                GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
                m_FcTimerEnd = GetTickCount64();

                cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
                fcProcessTime_ms = (float)(m_FcTimerEnd - m_FcTimerStart);
			    accumlatedCnnProcessTime += cnnProcessTime_ms;
			    accumlatedFcProcessTime += fcProcessTime_ms;

                //if (frame_num % 20 == 0) std::cout << ".";

                displayImage(m_img, gtiClassify, GTI_VIDEO, string(charbuf), resizeFlag);

                key = cv::waitKey(1) & 0xff;
                if (key == 'q' || key == 27 || key =='n')   // Esc key 27
                {
                    break;
                }
                if (key == ' ')
                {
                    pauseFlag = ~pauseFlag;
                }

                frame_num++;

                // calculate frame rate every 10 frames
                if (frame_num % FRAME_COUNT == 0)
                {
                    m_SvicTimerEnd = GetTickCount64();

                    frameRate = (float)(FRAME_COUNT * 1000. / (m_SvicTimerEnd - m_SvicTimerStart));
                    // Start Svic timer
                    m_SvicTimerStart = GetTickCount64();

                    avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
                    accumlatedCnnProcessTime = 0.;
                    avgFcProcessTime = accumlatedFcProcessTime / FRAME_COUNT;
                    accumlatedFcProcessTime = 0.;
                    sprintf_s(charbuf, "Frame Rate: %5.2f, Process(ms) - CNN: %5.2f ms, FC: %5.2f ms",
                                frameRate, avgCnnProcessTime, avgFcProcessTime);
                }
            }
            cap.release();
            if (key == 'q' || key == 27)    // ESC key 27
            {
                break;
            }
        }
        closedir (dir);
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Test_webcam()
// This function inputs web camera clip image, sends the image to
// GTI FPGA, gets output from the FPGA, calls Classify functions
// to get the clip class name and index.
//
// Input: none.
// return none.
//====================================================================
void Samples::Test_webcam()
{
    int key = 0;
    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_camFcname.c_str(), m_camLabelname.c_str());
    int devIndex = 0;
	int unused;
	
	(void)(unused);

    std::cout << "************************" << std::endl;
    std::cout << "Please input web camera device port number (0, 1, 2, 3, ...): ";
    unused = scanf_s("%d", &devIndex);
    std::cout << "************************" << std::endl;

    cap.open(devIndex);
    if (!cap.isOpened())
    {
        std::cout << "Webcam Device (" << devIndex <<") is not opened!" << std::endl;
        std::cout << "Press 'Enter' to exit." << std::endl;
        return;
    }

    std::cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "fps: " << cap.get(CV_CAP_PROP_FPS) << std::endl;

    cv::namedWindow(m_WebcamName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_WebcamName, 800, 600);
    cv::moveWindow(m_WebcamName, 400, 100);

    int frame_num = 0;
    while (1)
    {
        cap.grab();
        cap.retrieve(m_img);
        cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));

        convolution(m_img1);
        GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);

        if (frame_num % 20 == 0) std::cout << ".";
        displayImage(m_img, gtiClassify, GTI_WEBCAM);

        key = cv::waitKey(1) & 0xff;
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }

        frame_num++;
    }

    GtiClassifyRelease(gtiClassify);
    std::cout << "\n";
    cap.release();
}

//====================================================================
// Function name: Webcam_Hierarchy()
// This function inputs web camera clip image, sends the image to
// GTI FPGA, gets output from the FPGA, calls Classify functions
// to get the clip class name and index. It classified sub mode
// "People" and "Pets".
//
// Input: none.
// return none.
//====================================================================
void Samples::Webcam_Hierarchy()
{
    int key = 0;
    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_camFcname.c_str(), m_camLabelname.c_str());
    Classify *petsClassify = GtiClassifyCreate(m_camFc_pets.c_str(), m_camLabel_pets.c_str());
    int devIndex = 0;
    int subType = -1;
	int unused;
	(void)((unused));

    std::cout << "************************" << std::endl;
    std::cout << "Please input web camera device port number (0, 1, 2, 3, ...): ";
    unused = scanf_s("%d", &devIndex);
    std::cout << "************************" << std::endl;

    cap.open(devIndex);
    if (!cap.isOpened())
    {
        std::cout << "Webcam Device (" << devIndex <<") is not opened!" << std::endl;
        std::cout << "Press 'Enter' to exit." << std::endl;
        return;
    }

    std::cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "fps: " << cap.get(CV_CAP_PROP_FPS) << std::endl;

    cv::namedWindow(m_WebcamName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_WebcamName, 800, 600);
    cv::moveWindow(m_WebcamName, 400, 100);

    int frame_num = 0;
    while (1)
    {
        cap.grab();
        cap.retrieve(m_img);
        cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));

        convolution(m_img1);
        subType = GtiClassifyFC(gtiClassify, m_ImageOutPtr, 5);
        switch (subType)
        {
        case PETS_TYPE:
            GtiClassifyFC(petsClassify, m_ImageOutPtr, 5);
            displayImage(m_img, petsClassify, GTI_WEBCAM);
            break;
        default:
            displayImage(m_img, gtiClassify, GTI_WEBCAM);
            break;
        }

        if (frame_num % 20 == 0)
        {
            std::cout << ".";
        }

        key = cv::waitKey(1) & 0xff;
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }

        frame_num++;
    }

    GtiClassifyRelease(gtiClassify);
    GtiClassifyRelease(petsClassify);
    std::cout << "\n";
    cap.release();
}

//====================================================================
// Function name: Test_CnnSpeed(int DemoFlag)
// This function inputs an image, sends the image to GTI FPGA,
// gets output from the FPGA, counts the CNN time, calculates the FPS.
//
// Input: int DemoFlag - 1: no need to input file name for demo
//                       0: need to input file name.
// return none.
//====================================================================
void Samples::Test_CnnSpeed(int DemoFlag)
{
    int key = 0;
    float frameRate = 0.0, cnnFrameRate;
    float avgCnnProcessTime = 0.;
    float accumlatedCnnProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    static char charbuf[256];

    Classify *gtiClassify = GtiClassifyCreate(m_picFcname.c_str(), m_picLabelname.c_str());

    cv::namedWindow(m_ImageName, cv::WINDOW_NORMAL);
    cv::resizeWindow(m_ImageName, 800, 600);
    cv::moveWindow(m_ImageName, 400, 100);
    while (1)
    {
        accumlatedCnnProcessTime = 0.;

        if (DemoFlag)
        {
            sprintf_s(charbuf, "cnn fps:%.2f", frameRate);
            m_filename = m_SdkPath + glCnnSpeedDemoFileName;
        }
        else
        {
            sprintf_s(charbuf, "CNN Frame Rate: %5.2f, Process(ms) - CNN: %5.1f",
                                frameRate, avgCnnProcessTime);
            m_filename = openFileDialog(GTI_PICTURE);
        }
        if (m_filename.size() == 0)
        {
            break;
        }
        std::cout << "---------- Prediction for " << m_filename << " ----------" << std::endl;

        m_img = cv::imread(m_filename, -1);
        if (m_img.empty())
        {
            std::cout << "File not found." << std::endl;
            break;
        }
        cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
        displayImage(m_img, gtiClassify, GTI_PICTURE, "");

        m_SvicTimerStart = GetTickCount64();
        int frame_num = 0;
        while (1)
        {
            m_CnnTimerStart = GetTickCount64();

            convolution(m_img1);

            m_CnnTimerEnd = GetTickCount64();

//            cnnProcessTime_ms = (float)(m_CnnTimerEnd - m_CnnTimerStart);
            cnnProcessTime_ms = (float)(m_PureCnnEnd - m_PureCnnStart);
            accumlatedCnnProcessTime += cnnProcessTime_ms;

            key = cv::waitKey(1) & 0xff;
            if (key == 'q' || key == 27 || key ==' ')   // Esc key 27
                break;

            frame_num++;

            // calculate frame rate every 10 frames
            if (frame_num % FRAME_COUNT == 0)
            {
                m_SvicTimerEnd = GetTickCount64();

                frameRate = (float)(FRAME_COUNT * 1000. / (m_SvicTimerEnd - m_SvicTimerStart));
                // Start Svic timer
                m_SvicTimerStart = GetTickCount64();

                avgCnnProcessTime = accumlatedCnnProcessTime / FRAME_COUNT;
                cnnFrameRate = (float)(FRAME_COUNT * 1000. / accumlatedCnnProcessTime);
                accumlatedCnnProcessTime = 0.;
                if (DemoFlag)
                {
                    sprintf_s(charbuf, "cnn fps:%.2f", cnnFrameRate);
                }
                else
                {
                    sprintf_s(charbuf, "CNN Frame Rate: %5.2f, Process time - CNN: %5.2f ms",
                            cnnFrameRate, avgCnnProcessTime);
                }
                displayImage(m_img, gtiClassify, GTI_PICTURE, string(charbuf), 1, DemoFlag);

            }
        }
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: Test_CnnSpeed_video()
// This function tests GTI chip's CNN speed with video playing.
//
// Input: none.
// return none.
//====================================================================
void Samples::Test_CnnSpeed_video()
{
    int key = 0, resizeFlag = 0, pauseFlag = 0;
    double width, height;
    float cnnFrameRate = 0.0;
    float accumlatedCnnProcessTime = 0.;
    float cnnProcessTime_ms = 0;
    const int FRAME_COUNT = 10;
    char charbuf[256];

    cv::VideoCapture cap;
    Classify *gtiClassify = GtiClassifyCreate(m_videoFcname.c_str(), m_videoLabelname.c_str());

    cv::namedWindow(m_VideoName, cv::WINDOW_NORMAL);
//    cv::resizeWindow(m_VideoName, 800, 600);
    cv::moveWindow(m_VideoName, 400, 100);
    while (1)
    {
        accumlatedCnnProcessTime = 0.;

        sprintf_s(charbuf, "cnn fps:%.2f", cnnFrameRate);
        m_filename = m_SdkPath + glCnnSpeedDemoVideoName;

        if (m_filename.size() == 0)
        {
            break;
        }
        std::cout << "Open Video file " << m_filename << "...\n";
        cap.open(m_filename);
        if (!cap.isOpened())
        {
            cap.release();
            std::cout << "Video Clip File " << m_filename << " is not opened!\n";
            break;
        }

        width = m_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        height = m_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        cv::resizeWindow(m_VideoName, int(width), int(height));

        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "fps: " << cap.get(CV_CAP_PROP_FPS) << std::endl;
        std::cout << "frames: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

        int frame_num = 0;
        while (1)
        {
            if (pauseFlag == 0)
            {
                if (FALSE == cap.read(m_img))
                {
                    // reset if video playing ended
                    cap.set(CV_CAP_PROP_POS_FRAMES, 0);
                    cap.read(m_img);
                    frame_num = 0;
                }
                cv::resize(m_img, m_img1, cv::Size(GTI_IMAGE_WIDTH, GTI_IMAGE_HEIGHT));
            }

            convolution(m_img1);

            cnnProcessTime_ms = (float)(m_PureCnnEnd - m_PureCnnStart);
			accumlatedCnnProcessTime += cnnProcessTime_ms;

            displayImage(m_img, gtiClassify, GTI_VIDEO, string(charbuf), resizeFlag, 1);

            key = cv::waitKey(1) & 0xff;
            if (key == 'q' || key == 27 || key =='n')   // Esc key 27
            {
                break;
            }
            if (key == ' ')
            {
                pauseFlag = ~pauseFlag;
            }

            frame_num++;

            // calculate frame rate every 10 frames
            if (frame_num % FRAME_COUNT == 0) {
                frame_num = 0;
                cnnFrameRate = (float)(FRAME_COUNT * 1000. / accumlatedCnnProcessTime);
                accumlatedCnnProcessTime = 0.;
                sprintf_s(charbuf, "cnn fps:%.2f", cnnFrameRate);
            }
        }
        std::cout << "\n";

        if (cap.isOpened())
        {
            cap.release();
        }
        if (key == 'q' || key == 27)    // ESC key 27
        {
            break;
        }
    }
    GtiClassifyRelease(gtiClassify);
}

//====================================================================
// Function name: string openFileDialog(int Mode)
// This function opens a file dialog. You can select the image or video
// file from the dialog, this function returns selected file name.
//
// Input: int Mode: 0 - picture only. 1 - video only. 2 - picutre and video
// return: string - file name.
//====================================================================
std::string Samples::openFileDialog(int Mode)
{
#ifdef LINUX
    TCHAR fileName[200];
    int unused __attribute__((unused));
    std::string retFileName = "Image_bmp/bicycle.bmp";

    std::cout << "************************" << std::endl;
    switch (Mode)
    {
    case GTI_PICTURE:    // picture only
        std::cout << "Please input image file name(*.bmp, *.jpg, *.png): ";
        unused = scanf_s("%s", fileName);
        getchar();
        retFileName = (std::string)&fileName[0];
        break;
    case GTI_VIDEO:      // video only
        std::cout << "Please input video file name(*.mp4): ";
        unused = scanf_s("%s", fileName);
        getchar();
        retFileName = (std::string)&fileName[0];
        break;
    default:             // unknown
        std::cout << "Unknown file mode, open image file - Image_bmp/bicycle.bmp" << std::endl;
        break;
    }
    std::cout << "************************" << std::endl;
    return retFileName;
#else   
    // Initialize OPENFILENAME
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = m_FileNameBuffer;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_FILENAME_SIZE;
    switch (Mode)
    {
    case GTI_PICTURE:     // picture only
        ofn.lpstrInitialDir = (LPCWSTR)glImageFolder.c_str();// L".\\Image_bmp\\";
        ofn.lpstrFilter = L"Image Files(*.BMP;*.JPG;*.PNG;)\0*.BMP;*.JPG;*.PNG;\0All files(*.*)\0*.*\0";
        break;
    case GTI_VIDEO:       // video only
        ofn.lpstrInitialDir = (LPCWSTR)glVideoFolder.c_str();  // L".\\Image_mp4\\";
        ofn.lpstrFilter = L"Video Files(*.mp4)\0*.mp4;\0All files(*.*)\0*.*\0";
        break;
    default:    // picture and video
        ofn.lpstrInitialDir = (LPCWSTR)glImageFolder.c_str(); // L".\\Image_bmp\\";
        ofn.lpstrFilter = L"Image Files(*.BMP;*.JPG;*.PNG;*.mp4)\0*.BMP;*.JPG;*.PNG;*.mp4;\0All files(*.*)\0*.*\0";
        break;
    }

    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = L"Open Image..."; // NULL;
    ofn.nMaxFileTitle = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::wstring ws(ofn.lpstrFile);					// wchar_t to wstring
        return std::string(ws.begin(), ws.end());	    // wstring to string
    }
    else
    {
        return std::string("");
    }
#endif
}

//====================================================================
// Function name: void setLabel(cv::Mat&im, string label, cv::Point& or, double scale)
// This function display the text to an image.
//
// Input: Mat& im - image data.
//        string label - text to be displayed.
//        Point& or - start point for text displaying.
//        double scale - text scale value.
// return: none.
//====================================================================
void Samples::setLabel(cv::Mat& im, const std::string label, const cv::Point & orig, const double scale)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::rectangle(im, orig + cv::Point(0, baseline), orig + cv::Point(text.width, -text.height), CV_RGB(50, 60, 70), CV_FILLED);
    cv::putText(im, label, orig, fontface, scale, CV_RGB(255, 255, 0), thickness, 8);
}

//====================================================================
// Function name: void displayImage(cv::Mat& img, Classify& gClassify,
//     int mode, string str, int resizeFlag, int demoFlag, string ext)
// This function displays the image and predication texts on screen.
//
// Input: Mat& img - image data.
//        Classify& gClassify - class for FC.
//        int mode - display mode, 0: picture, 1: video, 2: webcam
//        string str - extra text to be displayed on the image.
//        int resizeFlag - 0: no resize, 1: resize.
//        int demoFlag - 0: not for demo, 1: for demo.
//        string ext - extra text to add to the label.
// return: none.
//====================================================================
void Samples::displayImage(cv::Mat &img, Classify* gClassify, int mode,
                 string str, int resizeFlag, int demoFlag, string ext)
{
    // display image in 800x600 window.
    int displayWidth = 800;
    int displayHeight = 600;
    cv::Mat dispImg;
    cv::String displayName;

    switch (mode)
    {
    case GTI_PICTURE:
        displayName = m_ImageName;
        break;
    case GTI_VIDEO:
        displayName = m_VideoName;
        break;
    case GTI_WEBCAM:
    default:
        displayName = m_WebcamName;
        break;
    }

    if (resizeFlag)
    {
        cv::resize(img, dispImg, cv::Size(displayWidth, displayHeight));
        showImage(dispImg, gClassify, str, displayName, ext, demoFlag);
    }
    else
    {
        showImage(img, gClassify, str, displayName, ext, demoFlag);
    }
}

//====================================================================
// Function name: void dispDemoImage(cv::Mat& img, Classify& gClassify,
//     char*dispName, string str, int w, int h, int demoFlag, string ext)
// This function displays the image and predication texts on screen.
//
// Input: Mat& img - image data.
//        Classify& gClassify - class for FC.
//        char *dispName - display name on window
//        string str - extra text to be displayed on the image.
//        int w - window width.
//        int h - window height.
//        int demoFlag - 0: not for demo, 1: for demo.
//        string ext - extra text to add to the label.
// return: none.
//====================================================================
void Samples::dispDemoImage(cv::Mat &img, Classify* gClassify, char* dispName,
                string str, int w, int h, int demoFlag, string ext)
{
    cv::Mat dispImg;

    cv::resize(img, dispImg, cv::Size(w, h));
    showImage1(dispImg, gClassify, str, string(dispName), ext, demoFlag);
}

//====================================================================
// Function name: void showImage(cv::Mat& img, Classify& gClassify,
//            string text, string dispName, string ext, int demoFlag)
// Based on demoFlag, this function displays the image and predicated
// texts on screen.
//
// Input: Mat& img - image data.
//        Classify& gClassify - class for FC.
//        string text - text to diaplay
//        string dispName - display window tile name.
//        string ext - extra string to display.
//        int demoFlag - 0: not for demo, 1: for demo.
// return: none.
//====================================================================
void Samples::showImage(cv::Mat &dispImg, Classify* gClassify, string text,
                      string displayName, string ext, int demoFlag)
{
    cv::Point text1Org(10, 550);
    cv::Point text2Org(10, 580);
    cv::Point text3Org(10, 520);
    double scale = 0.4;

    if (demoFlag)
    {
        text3Org.x = 10;
        text3Org.y = 30;
        text1Org.x = 10;
        text1Org.y = 70;
        scale = 1;
    }
    // then put the text itself
    if (GetPredicationSize(gClassify) > 1)
    {
        string text1 = GetPredicationString(gClassify, 0);
        string text2 = GetPredicationString(gClassify, 1);

        // Yello Text on Block Blox
        if (demoFlag)
        {
            size_t lastPos = text1.find_last_of(' ', string::npos);
            string labelName = text1.substr (lastPos + 1, string::npos);
            lastPos = labelName.find_last_of('-', string::npos);
            if (lastPos != string::npos)
            {
                labelName = labelName.substr (lastPos + 1, string::npos);
            }
            if (ext.length() > 0)
            {
                labelName = ext + "-" + labelName;
            }
            setLabel(dispImg, labelName, text1Org, scale);
        }
        else
        {
            if (ext.length() > 0)
            {
                text1 = ext + "-" + text1;
                text2 = ext + "-" + text2;
            }
            setLabel(dispImg, text1, text1Org, scale);
            setLabel(dispImg, text2, text2Org, scale);
        }
    }
    if (text.length() > 0)
    {
        setLabel(dispImg, text, text3Org, scale);
    }

    cv::imshow(displayName, dispImg);
}

//====================================================================
// Function name: void showImage1(cv::Mat& img, Classify& gClassify,
//            string text, string dispName, string ext, int demoFlag)
// Based on demoFlag, this function displays the image and predicated
// texts on screen.
//
// Input: Mat& img - image data.
//        Classify& gClassify - class for FC.
//        string text - text to diaplay
//        string dispName - display window tile name.
//        string ext - extra string to display.
//        int demoFlag - 0: not for demo, 1: for demo.
// return: none.
//====================================================================
void Samples::showImage1(cv::Mat &dispImg, Classify* gClassify, string text,
                      string displayName, string ext, int demoFlag)
{
    cv::Point text1Org(10, 30);
    cv::Point text2Org(10, 50);
    cv::Point text3Org(10, 70);
    double scale = 0.4;

    if (demoFlag)
    {
        text3Org.x = 10;
        text3Org.y = 30;
        text1Org.x = 10;
        text1Org.y = 70;
        scale = 0.5;
    }
    // then put the text itself
    if (GetPredicationSize(gClassify) > 1)
    {
        string text1 = GetPredicationString(gClassify, 0);
        string text2 = GetPredicationString(gClassify, 1);

        // Yello Text on Block Blox
        if (demoFlag)
        {
            size_t lastPos = text1.find_last_of(' ', string::npos);
            string labelName = text1.substr (lastPos + 1, string::npos);
            lastPos = labelName.find_last_of('-', string::npos);
            if (lastPos != string::npos)
            {
                labelName = labelName.substr (lastPos + 1, string::npos);
            }
            if (ext.length() > 0)
            {
                labelName = ext + "-" + labelName;
            }
            setLabel(dispImg, labelName, text1Org, scale);
        }
        else
        {
            if (ext.length() > 0)
            {
                text1 = ext + "-" + text1;
                text2 = ext + "-" + text2;
            }
            setLabel(dispImg, text1, text1Org, scale);
            setLabel(dispImg, text2, text2Org, scale);
        }
    }
    if (text.length() > 0)
    {
        setLabel(dispImg, text, text3Org, scale);
    }

    cv::imshow(displayName, dispImg);
}


//====================================================================
// Function name: void convolution(const signed char* img, const int imgLength)
// This function calls GTISendImage() to send image to GTI FPGA to do
// convolution.
//
// Input: img - 224 pixel x 224 pixel x 3 channels image data.
// return: none.
//====================================================================
void Samples::convolution(BYTE* img, const int imgLength)
{
    int bRet;
#ifdef ANDROID
  //  ALOG("GTI_JNI: Samples::convolution, length = %d\n", imgLength);
#endif
    bRet = GtiHandleOneFrameFloat(m_Device, img, imgLength, m_ImageOutPtr, GtiGetOutputLength(m_Device));

    if (bRet == 0)
    {
#ifdef ANDROID
        ALOG("GTI_JNI: Samples::convolution error.");
#endif
        GtiCloseDevice(m_Device);
        GtiOpenDevice(m_Device, m_deviceName);
    }
    return;
}


//====================================================================
// Function name: void convolution(const cv::Mat& img)
// This function calls GTISendImage() to send image to GTI FPGA to do
// convolution.
//
// Input: Mat& img - 224 pixel x 224 pixel x 3 channels image data.
// return: none.
//====================================================================
void Samples::convolution(const cv::Mat& img)
{
    int bRet;
    int ConvIn_Channels = 3;
    int ConvIn_ImgSize = 224;
    int imageInLength = (ConvIn_ImgSize * ConvIn_ImgSize) * ConvIn_Channels;
    std::vector<cv::Mat> input_image;

    //cnnSvicProc8UC3(img, &input_image);
	mOpenCvMutex.lock();
    cnnSvicProc32FC3(img, &input_image);
	mOpenCvMutex.unlock();
    cvt32FloatTo8Byte((float *)input_image[0].data, (uchar *)m_Img224x224x3Buffer, ConvIn_ImgSize, ConvIn_ImgSize, ConvIn_Channels);

    m_PureCnnStart = GetTickCount64();
  //    printf("-----tiGetOutputLength(m_Device) = %d\n", GtiGetOutputLength(m_Device));
    bRet = GtiHandleOneFrameFloat(m_Device, m_Img224x224x3Buffer, imageInLength, m_ImageOutPtr, GtiGetOutputLength(m_Device));
    m_PureCnnEnd = GetTickCount64();
    if (bRet == 0)
    {
        GtiCloseDevice(m_Device);
        GtiOpenDevice(m_Device, m_deviceName);
    }
    return;
}

//====================================================================
// Function name: void convOutput()
// This function calls GTIGetOutputData() to receive GTI FPGA output data.
// The output data length is 0x8000.
//
// Input: none
// return: none.
//====================================================================
void Samples::convOutput()
{
    int imageOutLength = GtiGetOutputLength(m_Device);

    if (!m_ImageOutPtr)
    {
        std::cout << "m_ImageOutPtr is null." << std::endl;
        return;
    }

    GtiGetOutputDataFloat(m_Device, m_ImageOutPtr, imageOutLength);
}

//====================================================================
// Function name: void cvt32FloatTo8Byte(float *InBuffer, uchar *OutBuffer,
//                      int Width, int Height, int Channels)
// This function converts 32 bit float format image to 8 bit integer
// format image.
//
// Input: float *InBuffer - input 32 bits/pixel data buffer.
//        uchar *OutBuffer - output buffer to store 8 bits/pixel image data.
//        int Width - image width in pixel
//        int Height - image height in pixel
//        int Channels - image channels
// return: none.
//====================================================================
void Samples::cvt32FloatTo8Byte(float *InBuffer, uchar *OutBuffer,
                  int Width, int Height, int Channels)
{
    uchar *pOut = OutBuffer;
    float *pIn = InBuffer;
    if (pIn == NULL || pOut == NULL)
    {
        std::cout << "cvt32FloatTo8Byte: null pointer!" << std::endl;
        return;
    }

    for (int i = 0; i < Channels; i++)
    {
        for (int j = 0; j < Height; j++)
        {
            for (int k = 0; k < Width; k++)
            {
                *pOut++ = (uchar)*pIn++;
            }
        }
    }
}

//====================================================================
// Function name: void cnnSvicProc8UC3(const cv::Mat& img,
//                          std::vector<cv::Mat>* input_channels)
// This function converts input image to the float point 3 channel image.
//
// Input: const cv::Mat& img - input image.
//        std::vector<cv::Mat>* input_channels - output buffer to store
//              float point 3 channel image.
// return: none.
//====================================================================
void Samples::cnnSvicProc8UC3(const cv::Mat& img, std::vector<cv::Mat>*input_channels)
{
    int width = 224;
    int height = 224;
    int num_channels_ = 3;
    m_imgSize.width = width;
    m_imgSize.height = height;
    unsigned char *input_data = (unsigned char *)m_Buffer32FC3;

    for (int i = 0; i < num_channels_; ++i)
    {
        cv::Mat channel(height, width, CV_8UC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }

    /* Convert the input image to the input image format of the network. */
    cv::Mat sample;
    if (img.channels() == 4) // && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1) // && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else
        sample = img;

    cv::Mat sample_resized;
    if (sample.size() != m_imgSize)
        cv::resize(sample, sample_resized, m_imgSize);
    else
        sample_resized = sample;

    cv::Mat sample_uchar;
    sample_resized.convertTo(sample_uchar, CV_8UC3);

    cv::Mat sample_normalized;
    cv::subtract(sample_uchar, cv::Scalar(0., 0., 0.), sample_normalized); // Zero

    /* This operation will write the separate BGR planes directly to the
    * input layer of the network because it is wrapped by the cv::Mat
    * objects in input_channels. */
    cv::split(sample_normalized, *input_channels);
}

//====================================================================
// Function name: void cnnSvicProc32FC3(const cv::Mat& img,
//                          std::vector<cv::Mat>* input_channels)
// This function converts input image to the float point 3 channel image.
//
// Input: const cv::Mat& img - input image.
//        std::vector<cv::Mat>* input_channels - output buffer to store
//              float point 3 channel image.
// return: none.
//====================================================================
void Samples::cnnSvicProc32FC3(const cv::Mat& img, std::vector<cv::Mat>* input_channels)
{
    cv::Mat sample;
    cv::Mat sample_resized;
    cv::Mat sample_byte;
    cv::Mat sample_normalized;

    const int num_channels = GTI_IMAGE_CHANNEL;
    int width = GTI_IMAGE_WIDTH;
    int height = GTI_IMAGE_HEIGHT;
    float *input_data = m_Buffer32FC3;
    if (input_data == NULL)
    {
        std::cout << "Failed allocat memory for input_data!" << std::endl;
        return;
    }

    for (int i = 0; i < num_channels; ++i)
    {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }

    sample = img;
    switch (img.channels())
    {
    case 1:
        if (num_channels == 3)
        {
            cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
        }
        break;
    case 3:
        if (num_channels == 1)
        {
            cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
        }
        break;
    case 4:
        if (num_channels == 1)
        {
            cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
        }
        else if (num_channels == 3)
        {
            cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
        }
        break;
    default:
        break;
    }

    sample_resized = sample;

    if (num_channels == 3)
    {
        sample_resized.convertTo(sample_byte, CV_32FC3);
    }
    else
    {
        sample_resized.convertTo(sample_byte, CV_32FC1);
    }

    cv::subtract(sample_byte, cv::Scalar(0., 0., 0.), sample_normalized);

    /* This operation will write the separate BGR planes directly to the
     * input layer of the network because it is wrapped by the cv::Mat
     * objects in input_channels. */
    cv::split(sample_normalized, *input_channels);
}

#ifdef WIN32
HANDLE Samples::opendir(const char* dirName)
{
    HANDLE hDir = INVALID_HANDLE_VALUE;
    wchar_t szDir[MAX_PATH];
    size_t ret;

    mbstowcs_s(&ret, szDir, MAX_PATH, dirName, MAX_PATH);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    hDir = FindFirstFile(szDir, &m_ffd);
    if (INVALID_HANDLE_VALUE == hDir)
    {
        hDir = NULL;
    }
    m_OpenFlag = 1;
    return hDir;
}

pDirent Samples::readdir(HANDLE hDir)
{
    char fileName[200];
    int fsize;

    if (0 == m_OpenFlag)
    {
        if (0 == FindNextFile(hDir, &m_ffd))
        {
            return NULL;
        }
    }
    m_OpenFlag = 0;
    fsize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_ffd.cFileName, -1, fileName, 0, NULL, NULL);
    fsize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_ffd.cFileName, -1, fileName, fsize, NULL, NULL);
    m_ent.d_name = fileName;
    return &m_ent;
}

void Samples::closedir(HANDLE hDir)
{
    FindClose(hDir);
}
#endif

HandleFile::HandleFile(char *FileName, char *rw)
{
#ifdef WIN32
    fopen_s(&m_fp, FileName, rw);
#else
    m_fp = fopen(FileName, rw);
#endif
}

HandleFile::~HandleFile()
{
    if (m_fp)
    {
        fclose(m_fp);
    }
    if (m_DataPtr)
    {
        delete [] m_DataPtr;
    }
}

int HandleFile::ReadFile()
{
    int readValue;
    int count = 0;

    if (!m_fp)
    {
        printf("File is not opened or open file error!");
        return FALSE;
    }

    m_Length = fileSize();
    m_DataPtr = new char[m_Length];
    if (NULL == m_DataPtr)
    {
        printf("Allocate memory error!");
        return FALSE;
    }
    unsigned short* readPtr = (unsigned short *)m_DataPtr;
    while (EOF != fscanf_s(m_fp, "%x", &readValue))
    {
        *readPtr++ = (unsigned short)readValue;
        count++;
    }
    m_Length = count * 2;
    return TRUE;
}

void HandleFile::WriteToFile(unsigned char *dataBuff, int dataLen)
{
    for (int i = 0; i < dataLen; i++)
    {
        fprintf(m_fp, "%02x ", dataBuff[i]);
#if 0
        if (i != 0 && ((i+1)%7) == 0)
        {

            fprintf(fp, "\n");
        }
        if (i != 0 && ((i+1)%49) == 0)
        {
            fprintf(fp, "\n");
        }
#else
        fprintf(m_fp, "\n");
#endif
    }
}

//=======================================================================
// Function name: int fileSize()
//   Gets file size.
//
// Input: None.
//
// Return: int - file size.
//=======================================================================
int HandleFile::fileSize()
{
    int size;

    if (NULL == m_fp)
    {
        return 0;
    }
    fseek(m_fp, 0L, SEEK_END);
    size = ftell(m_fp);
    rewind(m_fp);
    return size;
}

