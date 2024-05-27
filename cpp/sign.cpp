#include <optional>

#include "helpers.h"
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
        jsi::ArrayBuffer publicKey = getArrayBuffer(jsiRuntime, crypto_sign_PUBLICKEYBYTES);
        jsi::ArrayBuffer secretKey = getArrayBuffer(jsiRuntime, crypto_sign_SECRETKEYBYTES);
        crypto_sign_keypair(publicKey.data(jsiRuntime), secretKey.data(jsiRuntime));

        jsi::Object keyPair = jsi::Object(jsiRuntime);
        keyPair.setProperty(jsiRuntime, "publicKey", publicKey);
        keyPair.setProperty(jsiRuntime, "secretKey", secretKey);

        return keyPair;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "signGenerateKey", std::move(signGenerateKey));

    auto signDetached = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "signDetached"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> messageOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!messageOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] signDetached message must be an ArrayBuffer");
        }
        auto messageData = messageOpt.value().data(jsiRuntime);
        auto messageSize = messageOpt.value().size(jsiRuntime);

        std::optional<jsi::ArrayBuffer> secretKeyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!secretKeyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] signDetached secretKey must be an ArrayBuffer");
        }
        if (secretKeyOpt.value().size(jsiRuntime) != crypto_sign_SECRETKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] signDetached wrong secret key length");
        }
        auto secretKeyData = secretKeyOpt.value().data(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, crypto_sign_BYTES);
        uint8_t* signature = arrayBuffer.data(jsiRuntime);

        if (crypto_sign_detached(signature, NULL, messageData, messageSize, secretKeyData) != 0) {
          return jsi::Value(nullptr);
        }

        return arrayBuffer;
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
