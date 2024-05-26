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
        int64_t size;
        if (arguments[0].isNumber()) {
          size = arguments[0].asNumber();
        } else if (arguments[0].isBigInt()) {
          size = arguments[0].asBigInt(jsiRuntime).asInt64(jsiRuntime);
        } else {
          throw new jsi::JSError(jsiRuntime, "Size must be a number or a BigInt");
        }

        jsi::Function arrayBufferFunction = jsiRuntime.global().getPropertyAsFunction(jsiRuntime, "ArrayBuffer");
        jsi::Object object = arrayBufferFunction.callAsConstructor(jsiRuntime, (double)size).getObject(jsiRuntime);
        jsi::ArrayBuffer arrayBuffer = object.getArrayBuffer(jsiRuntime);
        uint8_t* data = arrayBuffer.data(jsiRuntime);
        randombytes_buf(data, size);

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "getRandomBytes", std::move(getRandomBytes));
  }
}
