#include "ui.h"
#include <iostream>

UI::UI() : startRecordingRequested(false), stopRecordingRequested(false) {}

void UI::initialize() {
    std::cout << "UI initialized." << std::endl;
}

void UI::handleEvents() {
    // Simulate event handling
    // In a real application, this would handle user input
    static int eventCounter = 0;
    if (eventCounter == 0) {
        startRecordingRequested = true;
    } else if (eventCounter == 5) {
        stopRecordingRequested = true;
    }
    eventCounter++;
}

void UI::render() {
    // Simulate UI rendering
    std::cout << "Rendering UI..." << std::endl;
}

bool UI::shouldStartRecording() {
    return startRecordingRequested;
}

bool UI::shouldStopRecording() {
    return stopRecordingRequested;
}
