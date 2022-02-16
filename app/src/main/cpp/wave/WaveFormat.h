//
// Created by GE SONG on 2021/12/27.
//

#ifndef RECORDKIT_WAVEFORMAT_H
#define RECORDKIT_WAVEFORMAT_H

#include <cstdint>
#include <fstream>



//44
typedef struct WaveHeader {

    // RIFF Chunk ID
    char riffCkID[4] = {'R', 'I', 'F', 'F'};

    // RIFF Chunk Size
    int32_t riffCkSize;

    // Format
    char format[4] = {'W', 'A', 'V', 'E'};

    // Format Chunk ID
    char fmtCkID[4] = {'f', 'm', 't', ' '};

    // Format Chunk Size
    int32_t fmtCkSize = 16;

    // 音频格式 1=PCM 3=Floating Point
    int16_t audioFormat = 1;

    // 声道数
    int16_t channels;

    // 采样率
    int32_t sampleRate;

    // 字节率
    int32_t byteRate = sampleRate * blockAlign;

    int16_t blockAlign = (channels * bitsPerSample) >> 3;

    // 位深
    int16_t bitsPerSample;

    // Data Chunk ID
    char dataCkID[4] = {'d', 'a', 't', 'a'};

    // Data Chunk Size
    int32_t dataCkSize;

} WaveHeader;

class WaveFormat {
private:

    int writeHeader(std::ofstream *outFile, WaveHeader *waveHeader);

public:
    int pcm2WaveFromFile(const char *inputFile, const char *outPutFile, int32_t channels, int32_t sampleRate, int32_t bitsPerSample);

    int pcm2WaveFromData(int16_t *data, int32_t dataLength, const char *outPutFile, int32_t channels, int32_t sampleRate, int32_t bitsPerSample);

    int savePatch(const int16_t *data, int32_t length);
};


#endif //RECORDKIT_WAVEFORMAT_H
