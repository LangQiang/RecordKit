package com.lq.record;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;

import java.util.Arrays;

public class FFTView extends View {

    Spectrum[] values = {};

    private Paint paint;

    private Path path;

    private Rect rect;


    public FFTView(Context context) {
        super(context);
        init();
    }

    public FFTView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public FFTView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        paint = new Paint();
        paint.setAntiAlias(true);
        paint.setColor(0xff196bc8);
        paint.setStrokeWidth(1);
        paint.setStyle(Paint.Style.STROKE);


        path = new Path();

        rect = new Rect();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        int size = values.length;
        if (size == 0) {
            return;
        }
        int y = getHeight();
        float x = getWidth();

        path.reset();

        path.moveTo(0, y);

        int interval = getWidth() / size;

        for (int i = 0; i < size; i ++) {
            rect.left = i * interval;
            rect.bottom = y;
            rect.right = rect.left + interval;
            rect.top = y - values[i].db;
            canvas.drawRect(rect, paint);
//            path.lineTo(x * i / size, y + values[i] / 300f * y);
        }
//        canvas.drawPath(path, paint);
    }

    public void refresh(Spectrum[] values) {
        this.values = values;
        Log.e("test", Arrays.toString(values));
        invalidate();
    }
}
