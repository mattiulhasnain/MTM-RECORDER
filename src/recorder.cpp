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
    // TODO: Implement video encoding using FFmpeg or similar
    std::cout << "Encoding video..." << std::endl;
}

void Recorder::cleanup() {
    GdiplusShutdown(gdiplusToken);
}
