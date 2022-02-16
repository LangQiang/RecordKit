#include "SoundRecording.h"
#include <fstream>
#include <iostream>
#include "wave/WaveFormat.h"


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
    waveFormat->pcm2WaveFromData(mData, mTotalSamples, outPutFile, channels, sampleRate, bitsPerSample);

//    waveFormat->savePatch(sourceData, numSamples);
    waveFormat->savePatch(nullptr, 0);

    return 0;
}
