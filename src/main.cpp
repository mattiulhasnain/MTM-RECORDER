#include <iostream>
#include "recorder.h"
#include "ui.h"

int main() {
    std::cout << "MTM Recorder - Starting application..." << std::endl;

    // Initialize the recorder
    Recorder recorder;
    recorder.initialize();

    // Initialize the UI
    UI ui;
    ui.initialize();

    // Main application loop
    while (true) {
        // Handle UI events
        ui.handleEvents();

        // Check for recording commands
        if (ui.shouldStartRecording()) {
            recorder.startRecording();
        } else if (ui.shouldStopRecording()) {
            recorder.stopRecording();
        }

        // Render the UI
        ui.render();
    }

    return 0;
}
