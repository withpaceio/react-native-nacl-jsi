#include "random.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_random(jsi::Runtime& jsiRuntime) {
    auto getRandomBytes = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "getRandomBytes"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        unsigned long size;
        try {
          size = arguments[0].asNumber();
        } catch (jsi::JSINativeException exception) {
          size = arguments[0].asBigInt(jsiRuntime).asUint64(jsiRuntime);
        }

        std::string encoding = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> buffer(size);
        randombytes_buf(buffer.data(), size);

        std::string encoded_buffer = encoding.compare("base64") == 0
            ? binToBase64(buffer.data(), buffer.size(), sodium_base64_VARIANT_ORIGINAL)
            : binToHex(buffer.data(), buffer.size());

        return jsi::String::createFromUtf8(jsiRuntime, encoded_buffer);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "getRandomBytes", std::move(getRandomBytes));
  }
}
