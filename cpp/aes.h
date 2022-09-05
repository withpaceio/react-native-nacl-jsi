#ifndef AES_H
#define AES_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  void install_aes(facebook::jsi::Runtime& jsiRuntime);
}

#endif