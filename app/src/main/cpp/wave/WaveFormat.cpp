//
// Created by GE SONG on 2021/12/27.
//

#include "WaveFormat.h"
#include <pthread.h>
#include <unistd.h>
#include "../Log.h"
#include "../utils/SysUtils.h"

int WaveFormat::pcm2WaveFromFile(const char *file, const char *outPutFile, int32_t channels, int32_t sampleRate,
                                 int32_t bitsPerSample) {
    remove(outPutFile);
    std::ifstream ifs( file , std::ios::binary | std::ios::in );     //  要处理的文件
    ifs.seekg( 0 , std::ios::end);                          //  文件指针指向末尾
    int length  =  ifs.tellg();                            //  得到文件的长度
    LOGE("2 length:%d", length);
    ifs.seekg ( 0 , std::ios::beg);

    auto *waveHeader = new WaveHeader;
    waveHeader->sampleRate = sampleRate;
    waveHeader->channels = static_cast<int16_t>(channels);
    waveHeader->bitsPerSample = static_cast<int16_t>(bitsPerSample);
    waveHeader->dataCkSize = static_cast<int32_t>(length);
    waveHeader->riffCkSize = static_cast<int32_t>(waveHeader->dataCkSize + sizeof (WaveHeader) - 4 - 4);

    std::ofstream outFile;
    outFile.open(outPutFile, std::ios::out | std::ios::binary);

    if (!outFile.is_open()) {
        LOGE("pcm2WaveFromData open file fail");
    }

    writeHeader(&outFile, waveHeader);

    while (!ifs.eof()) {
        char buff[1024] = {'\0'};
        ifs.read(buff, sizeof (char ) * 1024);
        int realRead = ifs.gcount();
        outFile.write(buff, realRead);
    }

    outFile.close();
    ifs.close();
    delete waveHeader;
    return 0;
}

int WaveFormat::pcm2WaveFromData(int16_t *data, int32_t dataLength, const char *outPutFile, int32_t channels, int32_t sampleRate, int32_t bitsPerSample) {

    remove(outPutFile);


    LOGE("1 length:%lu", sizeof (int16_t) * dataLength);

    auto *waveHeader = new WaveHeader;
    waveHeader->sampleRate = sampleRate;
    waveHeader->channels = static_cast<int16_t>(channels);
    waveHeader->bitsPerSample = static_cast<int16_t>(bitsPerSample);
    waveHeader->dataCkSize = static_cast<int32_t>(sizeof (int16_t) * dataLength);
    waveHeader->riffCkSize = static_cast<int32_t>(waveHeader->dataCkSize + sizeof (WaveHeader) - 4 - 4);

    std::ofstream outFile;
    outFile.open(outPutFile, std::ios::out | std::ios::binary);

    if (!outFile.is_open()) {
        LOGE("pcm2WaveFromData open file fail");
    }

    writeHeader(&outFile, waveHeader);

    outFile.write((char *)data, static_cast<std::streamsize>(dataLength * sizeof (int16_t)));

    outFile.close();

    delete waveHeader;

    return 0;
}

int WaveFormat::writeHeader(std::ofstream *outFile, WaveHeader *waveHeader) {
    (*outFile).write((char *)(&waveHeader->riffCkID), sizeof (waveHeader->riffCkID));
    (*outFile).write((char *)(&waveHeader->riffCkSize), sizeof (waveHeader->riffCkSize));
    (*outFile).write((char *)(&waveHeader->format), sizeof (waveHeader->format));
    (*outFile).write((char *)(&waveHeader->fmtCkID), sizeof (waveHeader->fmtCkID));
    (*outFile).write((char *)(&waveHeader->fmtCkSize), sizeof (waveHeader->fmtCkSize));
    (*outFile).write((char *)(&waveHeader->audioFormat), sizeof (waveHeader->audioFormat));
    (*outFile).write((char *)(&waveHeader->channels), sizeof (waveHeader->channels));
    (*outFile).write((char *)(&waveHeader->sampleRate), sizeof (waveHeader->sampleRate));
    (*outFile).write((char *)(&waveHeader->byteRate), sizeof (waveHeader->byteRate));
    (*outFile).write((char *)(&waveHeader->blockAlign), sizeof (waveHeader->blockAlign));
    (*outFile).write((char *)(&waveHeader->bitsPerSample), sizeof (waveHeader->bitsPerSample));
    (*outFile).write((char *)(&waveHeader->dataCkID), sizeof (waveHeader->dataCkID));
    (*outFile).write((char *)(&waveHeader->dataCkSize), sizeof (waveHeader->dataCkSize));
    return 0;
}

void *test(void *arg) {
    int32_t time = 100;
    while (time-- > 0) {

        LOGE("test thread invoke%d",  SysUtils::isMainThread());
        sleep(1);
    }

    return arg;
}

