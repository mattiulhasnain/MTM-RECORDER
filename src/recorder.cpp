#include "recorder.h"
#include <iostream>

Recorder::Recorder() : isRecording(false) {}

void Recorder::initialize() {
    std::cout << "Recorder initialized." << std::endl;
}

void Recorder::startRecording() {
    if (!isRecording) {
        std::cout << "Recording started." << std::endl;
        isRecording = true;
    }
}

void Recorder::stopRecording() {
    if (isRecording) {
        std::cout << "Recording stopped." << std::endl;
        isRecording = false;
    }
}
