#include "box.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_box(jsi::Runtime& jsiRuntime) {
    auto boxGenerateKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxGenerateKey"),
      0,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::vector<uint8_t> public_key(crypto_box_PUBLICKEYBYTES);
        std::vector<uint8_t> secret_key(crypto_box_SECRETKEYBYTES);
        crypto_box_keypair(public_key.data(), secret_key.data());

        jsi::Object key_pair = jsi::Object(jsiRuntime);
        key_pair.setProperty(jsiRuntime, "publicKey", binToBase64(public_key.data(), public_key.size(), sodium_base64_VARIANT_ORIGINAL));
        key_pair.setProperty(jsiRuntime, "secretKey", binToBase64(secret_key.data(), secret_key.size(), sodium_base64_VARIANT_ORIGINAL));

        return key_pair;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxGenerateKey", std::move(boxGenerateKey));
  }
}