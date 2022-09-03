#include <jni.h>
#include "react-native-nacl-jsi.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_reactnativenacljsi_NaclJsiModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
    return example::multiply(a, b);
}
