#include <jsi/jsilib.h>
#include <jsi/jsi.h>

#ifndef REACT_NATIVE_NACL_H
#define REACT_NATIVE_NACL_H

namespace react_native_nacl {
  void install(facebook::jsi::Runtime& jsiRuntime);
  void cleanup();
}

#endif
