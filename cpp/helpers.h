#pragma once

#include <optional>

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
  facebook::jsi::ArrayBuffer getArrayBuffer(facebook::jsi::Runtime& jsiRuntime, double size);
  std::optional<facebook::jsi::ArrayBuffer> getArrayBuffer(facebook::jsi::Runtime& jsiRuntime, const facebook::jsi::Value& value);
}
