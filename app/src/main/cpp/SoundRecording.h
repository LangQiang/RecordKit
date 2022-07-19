
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

    bool isRunning;

    static void *write2File(void *args);

public:
    int32_t write(const int16_t *sourceData, int32_t numSamples);
    int32_t read(int16_t *targetData, int32_t numSamples);
    int32_t save(const char *outPutFile, int32_t channels, int32_t sampleRate, int32_t bitsPerSample);

    void start();

    void stop();

    int32_t getTotalSamples() const { return mTotalSamples; };

    std::atomic<int32_t> mIteration { 1 };
    std::atomic<int32_t> mWriteIndex { 0 };
    std::atomic<int32_t> mReadIndex { 0 };
    std::atomic<int32_t> mTotalSamples { 0 };
    std::atomic<bool> mIsLooping { false };

    int16_t* mData = new int16_t[kMaxSamples]{0};
    int16_t gain_factor = 1;

    BlockingQueue<DataPatch> blockingQueue;

    void mix(const char *mix1, const char *mix2);
};

#endif //OBOE_RECORDER_SOUNDRECORDING_H
