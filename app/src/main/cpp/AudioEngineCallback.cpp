//
// Created by GE SONG on 2021/12/22.
//

#include "AudioEngineCallback.h"
#include "fft/FFTHelper.h"

void AudioEngineCallback::onRecordDataCallback(short *data, int32_t length) {
    try {
        //fft
        short fftData[length];
        FFTHelper::fft(data, length, fftData);
        JNIEnv *env;
        int mNeedDetach = 0;
        int getEnvStat = (*g_VM).GetEnv((void **)&env,JNI_VERSION_1_4);
        if (getEnvStat == JNI_EDETACHED) {
            //如果没有， 主动附加到jvm环境中，获取到env
            if ((*g_VM).AttachCurrentThread(&env, nullptr) != 0) {
                return;
            }
            mNeedDetach = JNI_TRUE;
        }
        //通过全局变量g_obj 获取到要回调的类
        jclass javaClass = (*env).GetObjectClass(callback_J);

        if (javaClass == nullptr) {
            LOGE("Unable to find class: callBack");
            (*g_VM).DetachCurrentThread();
            return;
        }

        //获取要回调的方法ID
        jmethodID javaCallbackId = (*env).GetMethodID(javaClass,"onRecordDataCallback",
                                                      "([SI)V");
        if (javaCallbackId == nullptr) {
            LOGE("Unable to find method:%s", "onRecordDataCallback");
            return;
        }

        jshortArray inputArray = (*env).NewShortArray(length);
        (*env).SetShortArrayRegion(inputArray, 0, length, data);

        (*env).CallVoidMethod(callback_J, javaCallbackId, inputArray, length);

        //获取要回调的方法ID
        jmethodID fftMethodId = (*env).GetMethodID(javaClass,"onFFTDataCallback",
                                                      "([SI)V");
        if (fftMethodId == nullptr) {
            LOGE("Unable to find method:%s", "onFFTDataCallback");
            return;
        }

        jshortArray fftArray = (*env).NewShortArray(length);
        (*env).SetShortArrayRegion(fftArray, 0, length, fftData);

        (*env).CallVoidMethod(callback_J, fftMethodId, fftArray, length);

        (*env).DeleteLocalRef(javaClass);
        (*env).DeleteLocalRef(inputArray);
        (*env).DeleteLocalRef(fftArray);

        //释放当前线程
        if(mNeedDetach) {
            (*g_VM).DetachCurrentThread();
        }
        env = nullptr;
    } catch (const std::exception& e) {
        LOGE("onRecordDataCallback %s", e.what());
    }

}
