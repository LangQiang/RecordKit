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
public:

    void onRecordDataCallback(short *data, int32_t length);

    jobject callback_J;
};


#endif //RECORDKIT_AUDIOENGINECALLBACK_H
