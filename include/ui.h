#ifndef UI_H
#define UI_H

class UI {
public:
    UI();
    void initialize();
    void handleEvents();
    void render();
    bool shouldStartRecording();
    bool shouldStopRecording();

private:
    bool startRecordingRequested;
    bool stopRecordingRequested;
};

#endif // UI_H
