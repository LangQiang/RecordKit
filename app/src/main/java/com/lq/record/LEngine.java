package com.lq.record;

public class LEngine {

    public native int init(LEngineCallback lEngineCallback);

    public native int startRecording();

    public native int stopRecording();

    public native void saveToFile(String path);

}
