package com.reactnativenacljsi;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

@ReactModule(name = NaclJsiModule.NAME)
public class NaclJsiModule extends ReactContextBaseJavaModule {
  public static final String NAME = "NaclJsi";

  private static native void initialize(long jsiPtr, String docDir);

  public NaclJsiModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("cpp");

      ReactApplicationContext context = getReactApplicationContext();
      initialize(context.getJavaScriptContextHolder().get(), context.getFilesDir().getAbsolutePath());

      return true;
    } catch (Exception ignored) {
      return false;
    }
  }
}
