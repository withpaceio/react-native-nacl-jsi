#include "helpers.h"

using namespace facebook;

namespace react_native_nacl {
  jsi::ArrayBuffer getArrayBuffer(jsi::Runtime& jsiRuntime, double size) {
    jsi::Function arrayBufferFunction = jsiRuntime.global().getPropertyAsFunction(jsiRuntime, "ArrayBuffer");
    jsi::Object object = arrayBufferFunction.callAsConstructor(jsiRuntime, (double)size).getObject(jsiRuntime);
    jsi::ArrayBuffer arrayBuffer = object.getArrayBuffer(jsiRuntime);

    return arrayBuffer;
  }
}
