
#ifndef OBOE_RECORDER_SOUNDRECORDING_H
#define OBOE_RECORDER_SOUNDRECORDING_H


#include <cstdint>
#include <array>
#include <atomic>
#include "BlockingQueue.h"



constexpr int kMaxSamples = 480000; // 10s of audio data @ 48kHz

struct DataPatch {
    int16_t *data;
    int32_t numSamples;
};

class SoundRecording {

public:
    int32_t write(const int16_t *sourceData, int32_t numSamples);
    int32_t write2();
    int32_t read(int16_t *targetData, int32_t numSamples);
    int32_t save(const char *outPutFile, int32_t channels, int32_t sampleRate, int32_t bitsPerSample);

    int32_t getTotalSamples() const { return mTotalSamples; };

    bool isRunning;

    std::atomic<int32_t> mIteration { 1 };
    std::atomic<int32_t> mWriteIndex { 0 };
    std::atomic<int32_t> mReadIndex { 0 };
    std::atomic<int32_t> mTotalSamples { 0 };
    std::atomic<bool> mIsLooping { false };

    int16_t* mData = new int16_t[kMaxSamples]{0};
    int16_t gain_factor = 2;

    BlockingQueue<DataPatch> blockingQueue;
};

#endif //OBOE_RECORDER_SOUNDRECORDING_H
