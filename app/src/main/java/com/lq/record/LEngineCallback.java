package com.lq.record;

public interface LEngineCallback {
    void onRecordDataCallback(short[] data, int length);
    void onFFTDataCallback(Spectrum[] fftData, int length);
}
