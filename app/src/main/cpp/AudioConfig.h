//
// Created by GE SONG on 2021/12/27.
//

#ifndef RECORDKIT_AUDIOCONFIG_H
#define RECORDKIT_AUDIOCONFIG_H

#include <cstdint>
#include <oboe/Oboe.h>
#include <oboe/AudioStream.h>
#include <oboe/AudioStreamBuilder.h>

using namespace oboe;

class AudioConfig {
public:

    int32_t mRecordingDeviceId = VoiceRecognition;
    AudioFormat mFormat = AudioFormat::I16;
    int32_t mSampleRate = kUnspecified;
    int32_t mInputChannelCount = ChannelCount::Mono;
    int32_t mOutputChannelCount = ChannelCount::Mono;

    int32_t mPerData = 128;

    AudioApi mAudioApi = AudioApi::AAudio;

    SharingMode sharingMode = SharingMode::Exclusive;

    PerformanceMode performanceMode = PerformanceMode::LowLatency;

    char* printConfig();
    void resetConfig();
};


#endif //RECORDKIT_AUDIOCONFIG_H
