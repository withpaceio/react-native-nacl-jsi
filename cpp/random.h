#ifndef RANDOM_H
#define RANDOM_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  void install_random(facebook::jsi::Runtime& jsiRuntime);
}

#endif