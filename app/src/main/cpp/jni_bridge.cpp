//
// Created by GE SONG on 2021/12/20.
//
#include <jni.h>
#include "AudioEngine.h"
#include "AudioEngineCallback.h"

static AudioEngine *audioEngine = nullptr;

static AudioEngineCallback *audioEngineCallback = nullptr;

extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_init(JNIEnv *env, jobject thiz, jobject l_engine_callback) {
    // TODO: implement init()

    if (audioEngine == nullptr) {
        audioEngine = new AudioEngine();
    }

    audioEngineCallback = new AudioEngineCallback();
    (*env).GetJavaVM(&audioEngineCallback->g_VM);
    audioEngineCallback->callback_J = (*env).NewGlobalRef(l_engine_callback);
    audioEngine->setDelegate(audioEngineCallback);

    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_startRecording(JNIEnv *env, jobject thiz) {

    // TODO: implement startRecording()

    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return -1;
    }
    audioEngine->start();
    return 0;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_lq_record_LEngine_stopRecording(JNIEnv *env, jobject thiz) {
    // TODO: implement stopRecording()
    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return -1;
    }
    audioEngine->stop();

    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lq_record_LEngine_saveToFile(JNIEnv *env, jobject thiz, jstring filePath) {
    // TODO: implement saveToFile()
    if (audioEngine == nullptr) {
        LOGE("audioEngine is null, you must call init() method before calling this method");
        return;
    }
    const char *path;

    path = env->GetStringUTFChars(filePath, nullptr);
    audioEngine->save(path);
}