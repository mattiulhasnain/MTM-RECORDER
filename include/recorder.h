#ifndef RECORDER_H
#define RECORDER_H

class Recorder {
public:
    Recorder();
    void initialize();
    void startRecording();
    void stopRecording();

private:
    bool isRecording;
};

#endif // RECORDER_H
