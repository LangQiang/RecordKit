//
// Created by GE SONG on 2021/12/20.
//

#include "AudioEngine.h"
#include "wave/WaveFormat.h"


void AudioEngine::start() {

    mSoundRecording->start();

    openRecordingStream();

    if (mRecordingStream != nullptr) {
        startStream();
    } else {
        LOGE("startRecording(): Failed to create recording (%p) stream", mRecordingStream);
    }
}

void AudioEngine::stop() {

    mSoundRecording->stop();

    LOGD("stop");

    if (mRecordingStream) {
        //stop
        oboe::Result result = mRecordingStream->stop(0L);
        if (result != oboe::Result::OK) {
            LOGE("Error stopping stream. %s", oboe::convertToText(result));
        }
        LOGI("stopStream(): mTotalSamples = %d", mSoundRecording->getTotalSamples());

        //close
        result = mRecordingStream->close();
        if (result != oboe::Result::OK) {
            LOGE("Error closing stream. %s", oboe::convertToText(result));
        } else {
            mRecordingStream = nullptr;
        }

        LOGI("closeStream(): mTotalSamples = %d", mSoundRecording->getTotalSamples());
    }

//    delegate->onRecordDataCallback(recordingCallBack->getTotalData(), recordingCallBack->getTotalSamples());
}

void AudioEngine::openRecordingStream() {
    AudioStreamBuilder builder;
    LOGD("setUpRecordingStreamParameters(): ");

    (&builder)->setAudioApi(audioConfig->mAudioApi)
            ->setFormat(audioConfig->mFormat)
            ->setSharingMode(audioConfig->sharingMode)
            ->setPerformanceMode(audioConfig->performanceMode)
            ->setDeviceId(audioConfig->mRecordingDeviceId)
            ->setSampleRate(audioConfig->mSampleRate)
            ->setChannelCount(audioConfig->mInputChannelCount)
            ->setDirection(Direction::Input)
            ->setCallback(recordingCallBack)
            ->setFramesPerCallback(128)
            ;

    Result result = builder.openStream(&mRecordingStream);
    if (result == Result::OK && mRecordingStream) {

        assert(mRecordingStream->getChannelCount() == audioConfig->mInputChannelCount);

        audioConfig->mSampleRate = mRecordingStream->getSampleRate();
        audioConfig->mFormat = mRecordingStream->getFormat();
        LOGI("openRecordingStream(): mSampleRate = %d", audioConfig->mSampleRate);

        LOGI("openRecordingStream(): mFormat = %s", convertToText(audioConfig->mFormat));

    } else {
        LOGE("Failed to create recording stream. Error: %s",
             convertToText(result));
    }
}

void AudioEngine::startStream() {
    LOGD("startStream(): ");

    assert(mRecordingStream);
    Result result = mRecordingStream->requestStart();
    if (result != Result::OK) {
        LOGE("Error starting stream. %s", convertToText(result));
    }
}


void AudioEngine::save(const char *outFileName) {

    mSoundRecording->save(outFileName,
                          audioConfig->mInputChannelCount,
                          audioConfig->mSampleRate,
                          16);
}

void AudioEngine::setDelegate(AudioEngineCallback *audioEngineCallback) {
    recordingCallBack->setDelegate(audioEngineCallback);
}

void AudioEngine::mix(const char *mix1, const char *mix2) {
    mSoundRecording->mix(mix1, mix2);
}
