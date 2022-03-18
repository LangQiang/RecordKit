package com.lq.record;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.lq.record.databinding.ActivityMainBinding;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.ConcurrentLinkedQueue;


public class MainActivity extends AppCompatActivity {

    // Used to load the 'recordkit' library on application startup.
    static {
        System.loadLibrary("recordkit");
    }

    class MyData {
        public short[] data;
        public int length;
    }

    ConcurrentLinkedQueue<MyData> queue = new ConcurrentLinkedQueue<>();

    ArrayList<MyData> arrayList = new ArrayList<>();


    private ActivityMainBinding binding;

    private final LEngine lEngine = new LEngine();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());


        File file = new File(getExternalFilesDir(null), "record2.pcm");
        Log.e("filePath", file.getAbsolutePath());


        lEngine.init(new LEngineCallback() {
            @Override
            public void onRecordDataCallback(short[] data, int length) {
                try {
//                    DataOutputStream dataOutputStream = new DataOutputStream(new FileOutputStream(file));
//                    for (short datum : data) {
//                        dataOutputStream.writeShort(datum);
//                    }
//                    dataOutputStream.close();
                } catch (Exception e) {

                }
            }
        });


        Button start = binding.start;
        Button stop = binding.stop;
        Button save = binding.save;
        Button test = binding.test;

        start.setOnClickListener(v -> {
            new Thread(lEngine::startRecording).start();
        });

        stop.setOnClickListener(v -> {
            new Thread(() -> {
                lEngine.stopRecording();
            }).start();
        });

        save.setOnClickListener(v -> {
            new Thread(() -> {
                lEngine.saveToFile(file.getAbsolutePath());
            }).start();
        });

        test.setOnClickListener(this::testOnclick);
    }

    private void testOnclick(View view) {
        LEngine.test(1);
    }

    /**
     * A native method that is implemented by the 'recordkit' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

}