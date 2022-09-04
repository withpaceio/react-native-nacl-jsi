#include <jni.h>
#include "react-native-nacl-jsi.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativenacljsi_NaclJsiModule_initialize(JNIEnv* env, jclass type, jlong jsiPtr) {
    react_native_nacl::install(*reinterpret_cast<facebook::jsi::Runtime*>(jsiPtr));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativenacljsi_NaclJsiModule_destruct(JNIEnv* env, jclass type) {
    react_native_nacl::cleanup();
}