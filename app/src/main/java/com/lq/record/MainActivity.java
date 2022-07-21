package com.lq.record;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.lq.record.databinding.ActivityMainBinding;

import java.io.File;
import java.util.ArrayList;
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


        File file = new File(getExternalFilesDir(null), "record2.wav");
        Log.e("filePath", file.getAbsolutePath());

        Spectrum[] last = new Spectrum[1000];

        for (int i = 0; i < last.length; i++) {
            last[i] = new Spectrum((short) 0, 0);
        }

        lEngine.init(new LEngineCallback() {

            private long time = 0;

            private int count = 0;

            private long total = 0;

            private FrameCallback frameCallback = new FrameCallback() {
                @Override
                public void callback(short value) {
                    binding.pcmOriginView.post(() -> {
                       binding.pcmOriginView.appendData(value);
                    });
                    Log.e("ui", "value:" + value);
                }

                @Override
                public void fft(Spectrum[] values) {
                    binding.fftView.post(() -> {
                        binding.fftView.refresh(values);
                    });
                }
            };

            @Override
            public void onRecordDataCallback(short[] data, int length) {
                try {
                    long cur = System.currentTimeMillis();


                    if (cur - time >= 16) {
                        time = cur;
                        if (count != 0) {
                            frameCallback.callback((short) (total / count));
                        }
                        count = 0;
                        total = 0;
                    } else {
                        long tempT = 0;
                        for (short datum : data) {
                            tempT += datum;
                        }
                        total += (tempT / length);
                        count ++;
                    }
                } catch (Exception e) {

                }
            }
            private long fftTime = 0;

            private int fftCount = 0;

            @Override
            public void onFFTDataCallback(Spectrum[] fftData, int length) {
                long cur = System.currentTimeMillis();
                if (cur - fftTime >= 500) {
                    fftTime = cur;
                    for (int i = 0; i < length; i++) {
                        fftData[i].db = (short) ((last[i].db + fftData[i].db) / (fftCount + 1));
                        last[i].db = 0;
                    }
                    frameCallback.fft(fftData);
                    fftCount = 0;

                } else {
                    fftCount ++;
                    for (int i = 0; i < length; i++) {
                        last[i].db += fftData[i].db;
                    }
                }
            }
        });


        Button start = binding.start;
        Button stop = binding.stop;
        Button save = binding.save;
        Button test = binding.test;
        Button mix = binding.mix;

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

        mix.setOnClickListener(this::mixClick);

    }

    private void mixClick(View view) {
        String mix1 = new File(getExternalFilesDir(null), "mix1.wav").getAbsolutePath();
        String mix2 = new File(getExternalFilesDir(null), "mix2.wav").getAbsolutePath();
        lEngine.mix(mix1, mix2);//同样位深 码率 声道 先忽略参数
    }

    private void testOnclick(View view) {
        LEngine.test(1);
    }

    /**
     * A native method that is implemented by the 'recordkit' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    interface FrameCallback {
        void callback(short value);
        void fft(Spectrum[] values);
    }

}