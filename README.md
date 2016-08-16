# RTMPServer

## Description
RTMP Server can be used to stream live video/audio content to Adobe Flash player clients over RTMP protocol. It is similar to using Adobe's Flash Server (FMS). The server is written to be lightweight and easy to understand, while having good performance. The server gets elementary streams from custom video source and broadcast to the clients as-is. Written in C and running in single process. 

## Features
* RTMP live streaming from custom video source 
* Stream relay support for distributed streaming: push & pull models
* H264/AAC support
* Online transcoding with FFmpeg
* IP address filtering 
* Advanced buffering techniques to keep memory allocations at a minimum level for faster streaming and low memory footprint
* Proved to work with Wirecast, FMS, Wowza, JWPlayer, FlowPlayer, StrobeMediaPlayback, ffmpeg, avconv, rtmpdump, flvstreamer and many more
* Source code is compatible with embedded systems
