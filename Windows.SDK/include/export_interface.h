#ifndef __EXPORT_INTERFACE_H
#define __EXPORT_INTERFACE_H

#include <stdint.h>
#include <string>


using namespace std;

struct MediaSample {
  uint8_t* data;
  size_t size;
  bool isVideo;
  bool isKey;

  uint64_t pts;

  unsigned char* video_extradata;
  size_t video_extradata_size;
  unsigned char* audio_extradata;
  size_t audio_extradata_size;
};

typedef bool (*GetMediaSampleCb_t)(string streamName, struct MediaSample& sample);

#ifdef _WIN32 

#include <windows.h>

/* You should define ADD_EXPORTS *only* when building the DLL. */
#ifdef ADD_EXPORTS
  #define ADDAPI __declspec(dllexport)
#else
  #define ADDAPI __declspec(dllimport)
#endif
 #define ADDCALL __cdecl

#else

#define ADDAPI
#define ADDCALL

#endif

#ifdef __cplusplus
extern "C" {  
#endif  







ADDAPI bool ADDCALL RTMPServer_Start(int port);
ADDAPI bool ADDCALL RTMPServer_AddApplication(char* name);
//ADDAPI bool ADDCALL RTMPServer_DelApplication(char* name);
ADDAPI bool ADDCALL RTMPServer_AddStream(char* name);
//ADDAPI bool ADDCALL RTMPServer_DelStream(char* name);
ADDAPI bool ADDCALL RTMPServer_PushMediaSample(char* streamName, MediaSample* sample);
ADDAPI bool ADDCALL RTMPServer_Stop();

#ifdef __cplusplus  
} // extern "C"  
#endif

#endif
