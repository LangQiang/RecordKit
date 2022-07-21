//
// Created by GE SONG on 2021/12/22.
//

#ifndef RECORDKIT_RECORDINGCALLBACK_H
#define RECORDKIT_RECORDINGCALLBACK_H


#include <oboe/AudioStreamCallback.h>
#include <oboe/AudioStream.h>
#include "Log.h"
#include "SoundRecording.h"

class RecordingCallBack : public oboe::AudioStreamCallback {

private:

    SoundRecording* mSoundRecording = nullptr;

public:
    RecordingCallBack() = default;

    explicit RecordingCallBack(SoundRecording* recording) {
        mSoundRecording = recording;
    }
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream,
                                          void *audioData,
                                          int32_t numFrames);

};


#endif //RECORDKIT_RECORDINGCALLBACK_H
