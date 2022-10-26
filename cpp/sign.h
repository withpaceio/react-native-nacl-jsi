#ifndef SIGN_H
#define SIGN_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  void install_sign(facebook::jsi::Runtime& jsiRuntime);
}

#endif