package com.lq.record;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

public class PcmOriginView extends View {

    private LimitQueue<Short> limitQueue = new LimitQueue<>(1000);

    private Paint paint;

    private Path path;


    public PcmOriginView(Context context) {
        super(context);
        init();
    }

    public PcmOriginView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public PcmOriginView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
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
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        int size = limitQueue.size();
        int y = getHeight() / 2;
        int x = getWidth();

        path.reset();

        path.moveTo(0, y);

        for (int i = 0; i < size; i ++) {
            path.lineTo(x * i / 1000f, y + limitQueue.get(i));
//            canvas.drawPoint(x * i / 1000f, y + limitQueue.get(i) * 2, paint);
        }
        canvas.drawPath(path, paint);
    }

    public void appendData(short value) {
        limitQueue.offer(value);
        invalidate();
    }
}
