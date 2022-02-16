//
// Created by GE SONG on 2021/12/22.
//

#include "RecordingCallBack.h"

oboe::DataCallbackResult
RecordingCallBack::onAudioReady(oboe::AudioStream *audioStream, void *audioData,
                                int32_t numFrames) {

    auto *inputData = static_cast<int16_t *>(audioData);

    int32_t num = numFrames * audioStream->getChannelCount();

    LOGI("read(): ");
    mSoundRecording->write(inputData, num);

    return oboe::DataCallbackResult::Continue;
}



