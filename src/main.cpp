#include <iostream>
#include "recorder.h"
#include "ui.h"
#include <thread>
#include <chrono>

int main() {
    std::cout << "MTM Recorder - Starting application..." << std::endl;

    // Initialize the recorder
    Recorder recorder;
    recorder.initialize();
    recorder.setOutputPath("output.mp4");
    recorder.setFrameRate(30);
    recorder.setCaptureArea(0, 0, 1920, 1080); // Full HD capture area

    // Initialize the UI
    UI ui;
    ui.initialize();

    std::cout << "Press Enter to start recording..." << std::endl;
    std::cin.get();
    
    std::thread recordingThread([&recorder]() {
        recorder.startRecording();
    });

    std::cout << "Recording started. Press Enter to stop..." << std::endl;
    std::cin.get();
    
    recorder.stopRecording();
    recordingThread.join();

    std::cout << "Recording stopped. Exiting..." << std::endl;
    return 0;
}
