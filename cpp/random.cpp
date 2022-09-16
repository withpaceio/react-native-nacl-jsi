#include "random.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_random(jsi::Runtime& jsiRuntime) {
    auto getRandomBytes = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "getRandomBytes"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        int size = arguments[0].asNumber();

        std::vector<uint8_t> buffer(size);
        randombytes_buf(buffer.data(), size);

        return jsi::String::createFromUtf8(jsiRuntime, binToBase64(buffer.data(), buffer.size(), sodium_base64_VARIANT_ORIGINAL));
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "getRandomBytes", std::move(getRandomBytes));
  }
}