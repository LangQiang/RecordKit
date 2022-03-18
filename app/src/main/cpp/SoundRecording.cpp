#include "SoundRecording.h"
#include <fstream>
#include <iostream>
#include "Log.h"
#include <oboe/Definitions.h>
#include <ios>
#include "wave/WaveFormat.h"
#include "AudioEngine.h"

int32_t SoundRecording::write(const int16_t *sourceData, int32_t numSamples) {


    // Check that data will fit, if it doesn't then create new array,
    // copy over old data to new array then delete old array and
    // point mData to new array.
    if (mWriteIndex + numSamples > (kMaxSamples * mIteration)) {

        mIteration++;

        int32_t newSize = kMaxSamples * mIteration;

        auto * newData = new int16_t[newSize] { 0 };
        std::copy(mData, mData + mTotalSamples, newData);

        delete[] mData;
        mData = newData;

    }

    for (int i = 0; i < numSamples; ++i) {
        mData[mWriteIndex++] = sourceData[i] * gain_factor;
    }

    mTotalSamples += numSamples;


    struct DataPatch dataPatch;
    dataPatch.data = new int16_t[numSamples] { 0 };
    for (int i = 0; i < numSamples; ++i) {
        dataPatch.data[i] = sourceData[i] * gain_factor;
    }
    dataPatch.numSamples = numSamples;
    blockingQueue.push(dataPatch);

    return numSamples;
}

int32_t SoundRecording::read(int16_t *targetData, int32_t numSamples) {

    int32_t framesRead = 0;

    while (framesRead < numSamples && mReadIndex < mTotalSamples) {

        targetData[framesRead++] = mData[mReadIndex++];

        if (mIsLooping && mReadIndex == mTotalSamples) mReadIndex = 0;
    }

    return framesRead;
}

int32_t SoundRecording::save(const char *outPutFile, int32_t channels, int32_t sampleRate,
                             int32_t bitsPerSample) {
    LOGI("%s", outPutFile);
    WaveFormat::pcm2WaveFromData(mData, mTotalSamples, outPutFile, channels, sampleRate, bitsPerSample);

//    waveFormat->savePatch(sourceData, numSamples);
    WaveFormat::savePatch(nullptr, 0);

    return 0;
}

int32_t SoundRecording::write2() {
    std::ofstream outFile;
    outFile.open("/storage/emulated/0/Android/data/com.lq.record/files/test.pcm", std::ios::out | std::ios::binary | std::ios::app);

    if (!outFile.is_open()) {
        LOGE("pcm2WaveFromData open file fail");
    }
    while (isRunning) { //todo lq 可能会出现停止之后队列里仍然有数据未被保存

        DataPatch dataPatch = blockingQueue.pop();
        int16_t *sourceData = dataPatch.data;
        int32_t samples = dataPatch.numSamples;
        outFile.write((char *)sourceData, static_cast<std::streamsize>(samples * sizeof (int16_t)));
    }

    outFile.close();
    return 0;
}

