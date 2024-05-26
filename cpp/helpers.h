#pragma once

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  facebook::jsi::ArrayBuffer getArrayBuffer(facebook::jsi::Runtime& jsiRuntime, double size);
}
