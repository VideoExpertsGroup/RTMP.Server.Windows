#include <unistd.h>
#include <string.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

#include "export_interface.h"



struct stream_cfg {
  char* name;
  char* url;
};

void* stream_routine(void* arg) {
  struct stream_cfg* cfg = (struct stream_cfg*) arg;
  int m_firstVideoFrame = 1;
  int m_firstAudioFrame = 1;
  int32_t videoStreamIndex = -1;
  int32_t audioStreamIndex = -1;
  AVPacket packet;
  int32_t ret = 0;
  uint64_t pts = 0;

  AVRational rtmp_time_base;
  rtmp_time_base.num = 1;
  rtmp_time_base.den = 1000;

  AVFormatContext *ifmtCtx = avformat_alloc_context ();

  if ((ret = avformat_open_input(&ifmtCtx, cfg->url, 0, 0)) < 0) {
      printf("Could not open input file '%s'\n", cfg->url);
      goto end;
  }

  if ((ret = avformat_find_stream_info(ifmtCtx, 0)) < 0) {
      printf("Failed to retrieve input stream information");
      goto end;
  }

  av_log_set_level(100);
  av_dump_format(ifmtCtx, 0, cfg->url, 0);

  for (int i = 0; i < ifmtCtx->nb_streams; i++) {
    if (ifmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStreamIndex = i;
      printf("Video stream index %d found\n", videoStreamIndex);
    } else if (ifmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
      audioStreamIndex = i;
      printf("Audio stream index %d found\n", audioStreamIndex);
    }
  }


  while (1) {
    av_init_packet(&packet); 
    MediaSample sample;
    memset(&sample, 0, sizeof(MediaSample));
    ret = av_read_frame(ifmtCtx, &packet); 

    if (packet.stream_index == audioStreamIndex) {
      sample.isVideo = 0;
      sample.isKey = packet.flags & AV_PKT_FLAG_KEY;
      sample.pts = av_rescale_q(packet.pts, ifmtCtx->streams[packet.stream_index]->time_base, rtmp_time_base);
      sample.audio_extradata = ifmtCtx->streams[audioStreamIndex]->codec->extradata;
      sample.audio_extradata_size = ifmtCtx->streams[audioStreamIndex]->codec->extradata_size;
    } 
    else if (packet.stream_index == videoStreamIndex) {
      sample.isVideo = 1;
      sample.isKey = packet.flags & AV_PKT_FLAG_KEY;
      sample.pts = av_rescale_q(packet.pts, ifmtCtx->streams[packet.stream_index]->time_base, rtmp_time_base); 
      sample.video_extradata = ifmtCtx->streams[videoStreamIndex]->codec->extradata;
      sample.video_extradata_size = ifmtCtx->streams[videoStreamIndex]->codec->extradata_size;
    }
    else {
      av_free_packet(&packet);
      continue;
    }

    sample.data = packet.data;
    sample.size = packet.size;
    RTMPServer_PushMediaSample(cfg->name, &sample);
    av_free_packet(&packet);
  }

end:
  printf("Error !\n");
  return NULL;
}


int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage:\n\t./%s rtsp_stream_url\n", argv[0]);
    exit(1);
  }

  printf("rtmp url rtmp//0.0.0.0:1935/live/test\n");  
  RTMPServer_Start(1935);
  RTMPServer_AddApplication("live");
  RTMPServer_AddStream("test");

  av_register_all();


  struct stream_cfg stream = {"test", argv[1]};
  pthread_t thread;

  pthread_create(&thread, NULL, stream_routine, &stream);

  while (1) {
    sleep (1);
  }

  return 0;
}

