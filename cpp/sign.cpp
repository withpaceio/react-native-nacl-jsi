#include "sign.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_sign(jsi::Runtime& jsiRuntime) {
    auto signGenerateKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "signGenerateKey"),
      0,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::vector<uint8_t> public_key(crypto_sign_PUBLICKEYBYTES);
        std::vector<uint8_t> secret_key(crypto_sign_SECRETKEYBYTES);
        crypto_sign_keypair(public_key.data(), secret_key.data());

        jsi::Object keypair = jsi::Object(jsiRuntime);
        keypair.setProperty(jsiRuntime, "publicKey", binToBase64(public_key.data(), public_key.size(), sodium_base64_VARIANT_ORIGINAL));
        keypair.setProperty(jsiRuntime, "secretKey", binToBase64(secret_key.data(), secret_key.size(), sodium_base64_VARIANT_ORIGINAL));

        return keypair;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "signGenerateKey", std::move(signGenerateKey));

    auto signDetached = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "signDetached"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string message_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string secret_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> secret_key = base64ToBin(jsiRuntime, secret_key_string);
        if (secret_key.size() != crypto_sign_SECRETKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_sign_detached wrong secret key length");
        }

        std::vector<uint8_t> signature(crypto_sign_BYTES);
        if (crypto_sign_detached(signature.data(), NULL, (uint8_t *)message_string.data(), message_string.size(), secret_key.data()) != 0) {
          return jsi::Value(nullptr);
        }

        return jsi::String::createFromUtf8(jsiRuntime, binToBase64(signature.data(), signature.size(), sodium_base64_VARIANT_ORIGINAL));
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "signDetached", std::move(signDetached));

    auto signVerifyDetached = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "signVerifyDetached"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string message_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string public_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);
        std::string signature_string = arguments[2].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> public_key = base64ToBin(jsiRuntime, public_key_string);
        if (public_key.size() != crypto_sign_PUBLICKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_sign_verify_detached wrong public key length");
        }

        std::vector<uint8_t> signature = base64ToBin(jsiRuntime, signature_string);

        bool result = crypto_sign_verify_detached(signature.data(), (uint8_t *)message_string.data(), message_string.size(), public_key.data()) == 0;
        return jsi::Value(result);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "signVerifyDetached", std::move(signVerifyDetached));
  }
}