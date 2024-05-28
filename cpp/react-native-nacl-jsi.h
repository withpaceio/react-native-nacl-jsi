#pragma once

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  void install(facebook::jsi::Runtime& jsiRuntime);
  void cleanup();
}
