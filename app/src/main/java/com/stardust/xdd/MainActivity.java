package com.stardust.xdd;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("stardust");
    }

    // JNI методы для передачи тач-событий в C++
    public static native void nativeTouchDown(float x, float y);
    public static native void nativeTouchUp(float x, float y);
    public static native void nativeTouchMove(float x, float y);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Вешаем touch listener на корневой View
        View rootView = getWindow().getDecorView().getRootView();
        rootView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                float x = event.getRawX();
                float y = event.getRawY();
                switch (event.getActionMasked()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_POINTER_DOWN:
                        nativeTouchDown(x, y);
                        break;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_POINTER_UP:
                    case MotionEvent.ACTION_CANCEL:
                        nativeTouchUp(x, y);
                        break;
                    case MotionEvent.ACTION_MOVE:
                        nativeTouchMove(x, y);
                        break;
                }
                // Возвращаем false чтобы игра тоже получала события
                return false;
            }
        });
    }
}
