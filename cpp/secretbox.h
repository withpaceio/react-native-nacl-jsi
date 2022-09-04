#ifndef SECRET_BOX_H
#define SECRET_BOX_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  void install_secret_box(facebook::jsi::Runtime& jsiRuntime);
}

#endif