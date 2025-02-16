#include "recorder.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace Gdiplus;
using namespace std::chrono;

Recorder::Recorder() : isRecording(false), frameRate(30), targetWindow(NULL) {
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

Recorder::~Recorder() {
    cleanup();
}

void Recorder::initialize() {
    std::cout << "Recorder initialized." << std::endl;
}

void Recorder::startRecording() {
    if (!isRecording) {
        isRecording = true;
        frameBuffer.clear();
        
        auto frameDuration = milliseconds(1000 / frameRate);
        while (isRecording) {
            auto start = high_resolution_clock::now();
            
            if (!captureFrame()) {
                std::cerr << "Failed to capture frame!" << std::endl;
                break;
            }
            
            auto end = high_resolution_clock::now();
            auto elapsed = duration_cast<milliseconds>(end - start);
            if (elapsed < frameDuration) {
                std::this_thread::sleep_for(frameDuration - elapsed);
            }
        }
        
        encodeVideo();
    }
}

void Recorder::stopRecording() {
    isRecording = false;
}

void Recorder::setOutputPath(const std::string& path) {
    outputPath = path;
}

void Recorder::setFrameRate(int fps) {
    frameRate = fps;
}

void Recorder::setCaptureArea(int x, int y, int width, int height) {
    captureRect = {x, y, x + width, y + height};
}

void Recorder::setCaptureWindow(HWND hwnd) {
    targetWindow = hwnd;
}

bool Recorder::captureFrame() {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    
    int width = captureRect.right - captureRect.left;
    int height = captureRect.bottom - captureRect.top;
    
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBitmap);
    
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 
           captureRect.left, captureRect.top, SRCCOPY);
    
    BITMAPINFOHEADER bi = {0};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  // Negative to make it top-down
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    
    std::vector<BYTE> frameData(width * height * 4);
    GetDIBits(hdcMem, hBitmap, 0, height, frameData.data(), 
             (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    
    frameBuffer.push_back(frameData);
    
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    
    return true;
}

void Recorder::encodeVideo() {
    if (frameBuffer.empty()) {
        std::cerr << "No frames to encode!" << std::endl;
        return;
    }

    // Get frame dimensions from first frame
    int width = captureRect.right - captureRect.left;
    int height = captureRect.bottom - captureRect.top;

    // Initialize FFmpeg components
    AVFormatContext* formatContext = nullptr;
    AVOutputFormat* outputFormat = nullptr;
    AVStream* videoStream = nullptr;
    AVCodecContext* codecContext = nullptr;
    SwsContext* swsContext = nullptr;
    AVFrame* frame = nullptr;
    AVPacket packet;

    // Allocate format context
    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, outputPath.c_str());
    if (!formatContext) {
        std::cerr << "Could not create output context" << std::endl;
        return;
    }

    outputFormat = formatContext->oformat;

    // Find the H.264 codec
    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        return;
    }

    // Create video stream
    videoStream = avformat_new_stream(formatContext, codec);
    if (!videoStream) {
        std::cerr << "Could not create video stream" << std::endl;
        return;
    }

    videoStream->id = formatContext->nb_streams - 1;
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        std::cerr << "Could not allocate codec context" << std::endl;
        return;
    }

    // Set codec parameters
    codecContext->codec_id = AV_CODEC_ID_H264;
    codecContext->bit_rate = 400000;
    codecContext->width = width;
    codecContext->height = height;
    videoStream->time_base = (AVRational){1, frameRate};
    codecContext->time_base = videoStream->time_base;
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codecContext->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
    }

    // Open codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        return;
    }

    // Allocate frame
    frame = av_frame_alloc();
    if (!frame) {
        std::cerr << "Could not allocate video frame" << std::endl;
        return;
    }

    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;

    if (av_frame_get_buffer(frame, 32) < 0) {
        std::cerr << "Could not allocate frame data" << std::endl;
        return;
    }

    // Write header
    if (avformat_write_header(formatContext, nullptr) < 0) {
        std::cerr << "Error occurred when opening output file" << std::endl;
        return;
    }

    // Initialize SWS context for software scaling
    swsContext = sws_getContext(width, height, AV_PIX_FMT_BGRA,
                               width, height, AV_PIX_FMT_YUV420P,
                               SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Encode frames
    for (size_t i = 0; i < frameBuffer.size(); i++) {
        // Convert frame to YUV420P
        const uint8_t* srcData[1] = { frameBuffer[i].data() };
        int srcLinesize[1] = { width * 4 };
        
        sws_scale(swsContext, srcData, srcLinesize, 0, height,
                  frame->data, frame->linesize);

        frame->pts = i;

        // Encode frame
        if (avcodec_send_frame(codecContext, frame) < 0) {
            std::cerr << "Error sending frame for encoding" << std::endl;
            break;
        }

        while (avcodec_receive_packet(codecContext, &packet) >= 0) {
            av_packet_rescale_ts(&packet, codecContext->time_base, videoStream->time_base);
            packet.stream_index = videoStream->index;
            av_interleaved_write_frame(formatContext, &packet);
            av_packet_unref(&packet);
        }
    }

    // Write trailer
    av_write_trailer(formatContext);

    // Cleanup
    if (codecContext) {
        avcodec_free_context(&codecContext);
    }
    if (formatContext) {
        avio_closep(&formatContext->pb);
        avformat_free_context(formatContext);
    }
    if (frame) {
        av_frame_free(&frame);
    }
    if (swsContext) {
        sws_freeContext(swsContext);
    }

    std::cout << "Video encoding completed: " << outputPath << std::endl;
}

void Recorder::cleanup() {
    GdiplusShutdown(gdiplusToken);
}
