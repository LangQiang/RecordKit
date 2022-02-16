//
// Created by GE SONG on 2021/12/27.
//

#include "AudioConfig.h"

char *AudioConfig::printConfig() {
    return nullptr;
}

void AudioConfig::resetConfig() {
    mRecordingDeviceId = VoiceRecognition;
    mFormat = AudioFormat::I16;
    mSampleRate = kUnspecified;
    mInputChannelCount = ChannelCount::Mono;
    mOutputChannelCount = ChannelCount::Mono;
    mAudioApi = AudioApi::AAudio;
    sharingMode = SharingMode::Exclusive;
    performanceMode = PerformanceMode::LowLatency;
}
