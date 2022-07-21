//
// Created by GE SONG on 2021/12/22.
//

#ifndef RECORDKIT_AUDIOENGINECALLBACK_H
#define RECORDKIT_AUDIOENGINECALLBACK_H


#include <jni.h>
#include "Log.h"
#include <cstdint>
#include <exception>
#include "global/GlobalConfig.h"

class AudioEngineCallback {

private:
    AudioEngineCallback() {}
    AudioEngineCallback(AudioEngineCallback&)=delete;
    AudioEngineCallback& operator=(const AudioEngineCallback&)=delete;
    static AudioEngineCallback* INSTANCE;

public:
    ~AudioEngineCallback() {}

    static AudioEngineCallback* getInstance();

    void onRecordDataCallback(int16_t *data, int32_t length) const;

    jobject callback_J;
};




#endif //RECORDKIT_AUDIOENGINECALLBACK_H
