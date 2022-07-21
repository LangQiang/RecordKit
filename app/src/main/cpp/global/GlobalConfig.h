//
// Created by 神强 on 2022/2/27.
//

#ifndef RECORDKIT_GLOBALCONFIG_H
#define RECORDKIT_GLOBALCONFIG_H

#include "stdint.h"
#include <jni.h>

extern JavaVM *g_VM;
extern int64_t mainThreadId;
extern jobject spectrumObj;
const static char *tempPath = "/storage/emulated/0/Android/data/com.lq.record/files/test.pcm";


#endif //RECORDKIT_GLOBALCONFIG_H
