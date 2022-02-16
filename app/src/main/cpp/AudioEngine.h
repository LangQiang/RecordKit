//
// Created by GE SONG on 2021/12/20.
//

#ifndef RECORDKIT_AUDIOENGINE_H
#define RECORDKIT_AUDIOENGINE_H

#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>
#include <oboe/AudioStreamBuilder.h>
#include "Log.h"
#include "jni.h"
#include "AudioEngineCallback.h"
#include "RecordingCallBack.h"
#include "SoundRecording.h"
#include "AudioConfig.h"
#include <string>

using namespace oboe;

class AudioEngine {

public:

    void start();

    void stop();

    void save(const char *string);

    void setDelegate(AudioEngineCallback *audioEngineCallback);

private:

    SoundRecording *mSoundRecording = new SoundRecording();

    RecordingCallBack *recordingCallBack = new RecordingCallBack(mSoundRecording);

    AudioConfig *audioConfig = new AudioConfig();

    AudioStream *mRecordingStream = nullptr;

    AudioStream *mPlaybackStream = nullptr;

    AudioEngineCallback *delegate;

    void openRecordingStream();
    void startStream();

};


#endif //RECORDKIT_AUDIOENGINE_H
