package com.stardust.xdd;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

  static {
    // When you change the lib name, change also on Android.mk file
    // Both must have same name
    System.loadLibrary("stardust");
  }

  // To call onCreate, please refer to README.md
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
  }
}
