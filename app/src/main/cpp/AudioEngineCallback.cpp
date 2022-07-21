//
// Created by GE SONG on 2021/12/22.
//

#include "AudioEngineCallback.h"
#include "fft/FFTHelper.h"

AudioEngineCallback* AudioEngineCallback::INSTANCE = nullptr;

void originDataCallback(jobject callback_J, int16_t *data, int32_t length) {
    JNIEnv *env;
    int mNeedDetach = 0;
    try {
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


        (*env).DeleteLocalRef(javaClass);
        (*env).DeleteLocalRef(inputArray);

        //释放当前线程
        if(mNeedDetach) {
            (*g_VM).DetachCurrentThread();
        }
        env = nullptr;
    } catch (const std::exception& e) {
        //释放当前线程
        if(mNeedDetach) {
            (*g_VM).DetachCurrentThread();
        }
        env = nullptr;
        LOGE("onRecordDataCallback %s", e.what());
    }
}


void fftDataCallback(jobject callback_J, Spectrum *fftData, int32_t length) {
    JNIEnv *env;
    int mNeedDetach = 0;
    try {
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
        jmethodID fftMethodId = (*env).GetMethodID(javaClass,"onFFTDataCallback",
                                                   "([Lcom/lq/record/Spectrum;I)V");

        if (fftMethodId == nullptr) {
            LOGE("Unable to find method:%s", "onFFTDataCallback");
            //释放当前线程
            if(mNeedDetach) {
                (*g_VM).DetachCurrentThread();
            }
            return;
        }

        if (spectrumObj == nullptr) {
            LOGE("Unable to find class:%s", "com/lq/record/Specstrum");

            //释放当前线程
            if(mNeedDetach) {
                (*g_VM).DetachCurrentThread();
            }
            return;
        }
        jclass speCl = env->GetObjectClass(spectrumObj);
        jmethodID conId = (*env).GetMethodID(speCl, "<init>", "(SI)V");

        jobjectArray spectrums = (*env).NewObjectArray(length, speCl, nullptr);
        for (int i = 0; i < length; ++i) {
            jobject ele = (*env).NewObject(speCl, conId, fftData[i].amplitude, fftData[i].frequency);
            (*env).SetObjectArrayElement(spectrums, i, ele);
            env->DeleteLocalRef(ele);
        }


        (*env).CallVoidMethod(callback_J, fftMethodId, spectrums, length);

        (*env).DeleteLocalRef(javaClass);
        (*env).DeleteLocalRef(spectrums);

        //释放当前线程
        if(mNeedDetach) {
            (*g_VM).DetachCurrentThread();
        }
        env = nullptr;
    } catch (const std::exception& e) {
        LOGE("onRecordDataCallback %s", e.what());
        //释放当前线程
        if(mNeedDetach) {
            (*g_VM).DetachCurrentThread();
        }
        env = nullptr;
    }
}

void AudioEngineCallback::onRecordDataCallback(int16_t *data, int32_t length) const {
    originDataCallback(callback_J, data, length);

    //fft
    Spectrum spectrumArr[length];
    FFTHelper::fft(data, length, spectrumArr);
    fftDataCallback(callback_J, spectrumArr, length);

}

AudioEngineCallback *AudioEngineCallback::getInstance() {
    if(INSTANCE==nullptr){
        INSTANCE = new AudioEngineCallback;
    }
    return INSTANCE;
}
