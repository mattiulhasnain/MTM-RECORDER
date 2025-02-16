#ifndef RECORDER_H
#define RECORDER_H

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

class Recorder {
public:
    Recorder();
    ~Recorder();
    void initialize();
    void startRecording();
    void stopRecording();
    void setOutputPath(const std::string& path);
    void setFrameRate(int fps);
    void setCaptureArea(int x, int y, int width, int height);
    void setCaptureWindow(HWND hwnd);

private:
    bool captureFrame();
    void saveFrame(const std::vector<BYTE>& frameData);
    void encodeVideo();
    void cleanup();

    bool isRecording;
    int frameRate;
    std::string outputPath;
    RECT captureRect;
    HWND targetWindow;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    std::vector<std::vector<BYTE>> frameBuffer;
};

#endif // RECORDER_H
