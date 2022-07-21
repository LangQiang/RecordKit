#include "SoundRecording.h"
#include <fstream>
#include <iostream>
#include "Log.h"
#include <oboe/Definitions.h>
#include <ios>
#include "wave/WaveFormat.h"
#include "AudioEngine.h"
#include "global/GlobalConfig.h"
#include "pthread.h"
#include "speex/speex_preprocess.h"

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


    DataPatch dataPatch {
            dataPatch.data = new int16_t[numSamples] { 0 },
            dataPatch.numSamples = numSamples
    };
    for (int i = 0; i < numSamples; ++i) {
        dataPatch.data[i] = sourceData[i] * gain_factor;
    }

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
    WaveFormat::pcm2WaveFromFile(tempPath, "/storage/emulated/0/Android/data/com.lq.record/files/test.wav", channels, sampleRate, bitsPerSample);

    return 0;
}

SpeexPreprocessState *preprocess_state = speex_preprocess_state_init(128, 48000);


void *SoundRecording::write2File(void *args) {
    std::ofstream outFile;
    outFile.open(tempPath, std::ios::out | std::ios::binary | std::ios::app);

    if (!outFile.is_open()) {
        LOGE("pcm2WaveFromData open file fail");
    }
    while (((SoundRecording * )args)->isRunning || !((SoundRecording * )args)->blockingQueue.isEmpty()) {

        try {
            DataPatch dataPatch = ((SoundRecording * )args)->blockingQueue.pop();
            int16_t *sourceData = dataPatch.data;
            int32_t samples = dataPatch.numSamples;
            LOGE("test NumSamples:%d", dataPatch.numSamples);
            speex_preprocess_run(preprocess_state, sourceData);

            outFile.write((char *)sourceData, static_cast<std::streamsize>(samples * sizeof (int16_t)));
        } catch (char const*) {
            continue;
        }

    }

    outFile.close();
    return nullptr;
}
void SoundRecording::start() {
    isRunning = true;
    blockingQueue.reset();
    remove(tempPath);
    pthread_t pid;
    pthread_create(&pid, nullptr, write2File, this);
}

void SoundRecording::stop() {
    isRunning = false;
    blockingQueue.interrupt();
}

short remixLiner(int16_t buffer1,int16_t buffer2) { //线性叠加求平均
    int value = buffer1 + buffer2;
    return (int16_t)(value/2);
}

int16_t remix(int16_t data1,int16_t data2) {
    int16_t date_mix;
    if( data1 < 0 && data2 < 0)
        date_mix = data1 + data2 - (data1 * data2 / -(pow(2,16-1)-1));
    else
        date_mix = data1 + data2 - (data1 * data2 / (pow(2,16-1)-1));
    return date_mix;
}



//todo 测试混合效果
void SoundRecording::mix(const char *mix1, const char *mix2) {

    std::ifstream inStream1(mix1, std::ios::binary | std::ios::in);
//    inStream1.seekg(44, std::ios::beg);

    std::ifstream inStream2(mix2, std::ios::binary | std::ios::in);
    inStream2.seekg(44, std::ios::beg);

    std::ofstream outStream("/storage/emulated/0/Android/data/com.lq.record/files/mix.wav");

    char header[44] = {'\0'};

    inStream1.read(header, sizeof (char ) * 44);
    outStream.write(header, inStream1.gcount());

    SpeexPreprocessState *preprocess_state = speex_preprocess_state_init(1024, 48000);


    while (!inStream1.eof() /*&& !inStream2.eof()*/) {
        int16_t data1;
        int16_t data2;

        spx_int16_t spxBuff[1024] = {0};
        inStream1.read((char *)spxBuff, sizeof (spx_int16_t) * 1024);

        speex_preprocess_run(preprocess_state, spxBuff);



//        inStream1.read((char *) &data1, sizeof (data1));
//        inStream2.read((char *) &data2, sizeof (data2));
//        int16_t ret = remixLiner(data1, data2);
//        int16_t ret = remix(data1, data2);
        outStream.write((char *)&spxBuff, sizeof (spxBuff));
    }
    inStream1.close();
    inStream2.close();
    outStream.close();
}





