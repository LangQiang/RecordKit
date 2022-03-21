//
// Created by GE SONG on 2021/12/20.
//
#include <jni.h>
#include "AudioEngine.h"
#include "AudioEngineCallback.h"
#include "utils/SysUtils.h"
#include "global/GlobalConfig.h"

#include "Test.h"

static AudioEngine *audioEngine = nullptr;

static AudioEngineCallback *audioEngineCallback = nullptr;


JavaVM *g_VM = nullptr;
int64_t mainThreadId = 0;

extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_init(JNIEnv *env, jobject jObj, jobject l_engine_callback) {

    if (audioEngine == nullptr) {
        audioEngine = new AudioEngine();
    }

    mainThreadId = SysUtils::getCurrentThreadId();
    LOGD("mainThreadId:%ld", mainThreadId);
    (*env).GetJavaVM(&g_VM);

    audioEngineCallback = new AudioEngineCallback();
    audioEngineCallback->callback_J = (*env).NewGlobalRef(l_engine_callback);
    audioEngine->setDelegate(audioEngineCallback);

    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_startRecording(JNIEnv *env, jobject jObj) {

    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return -1;
    }
    audioEngine->start();
    return 0;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_stopRecording(JNIEnv *env, jobject jObj) {
    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return -1;
    }
    audioEngine->stop();

    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lq_record_LEngine_saveToFile(JNIEnv *env, jobject jObj, jstring filePath) {
    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return;
    }
    const char *path;

    path = env->GetStringUTFChars(filePath, nullptr);
    audioEngine->save(path);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lq_record_LEngine_test(JNIEnv *env, jclass jCl, jint test_type) {
    int32_t type = test_type;
    if (type == 1) {
        Test::test_BlockingQueue();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lq_record_LEngine_mix(JNIEnv *env, jobject jObj, jstring mix1, jstring mix2) {
    const char *cMix1 = env->GetStringUTFChars(mix1, nullptr);
    const char *cMix2 = env->GetStringUTFChars(mix2, nullptr);
    audioEngine->mix(cMix1, cMix2);
}