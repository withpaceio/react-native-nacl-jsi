#include <optional>

#include "argon2id.h"
#include "helpers.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_argon2id(jsi::Runtime& jsiRuntime) {
    auto argon2idHash = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "argon2idHash"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> passwordOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!passwordOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idHash password must be an ArrayBuffer");
        }
        auto passwordData = passwordOpt.value().data(jsiRuntime);
        auto passwordSize = passwordOpt.value().size(jsiRuntime);

        auto iterations = arguments[1].asBigInt(jsiRuntime).asUint64(jsiRuntime);
        auto memoryLimit = arguments[2].asBigInt(jsiRuntime).asUint64(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, crypto_pwhash_STRBYTES);
        uint8_t* hashedPassword = arrayBuffer.data(jsiRuntime);

        if (crypto_pwhash_str((char *)hashedPassword, (const char* const)passwordData, passwordSize, iterations, memoryLimit) != 0) {
          return jsi::Value(nullptr);
        }

        return jsi::String::createFromUtf8(jsiRuntime, (char*)hashedPassword);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idHash", std::move(argon2idHash));

    auto argon2idVerify = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "argon2idVerify"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string hash = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);

        std::optional<jsi::ArrayBuffer> passwordOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!passwordOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idVerify password must be an ArrayBuffer");
        }
        auto passwordData = passwordOpt.value().data(jsiRuntime);
        auto passwordSize = passwordOpt.value().size(jsiRuntime);

        bool result = crypto_pwhash_str_verify(hash.data(), (const char* const)passwordData, passwordSize) == 0;
        return jsi::Value(result);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idVerify", std::move(argon2idVerify));

    auto argon2idDeriveKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "argon2idDeriveKey"),
      5,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> passwordOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!passwordOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idDeriveKey password must be an ArrayBuffer");
        }
        auto passwordData = passwordOpt.value().data(jsiRuntime);
        auto passwordSize = passwordOpt.value().size(jsiRuntime);

        std::optional<jsi::ArrayBuffer> saltOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!saltOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idDeriveKey salt must be an ArrayBuffer");
        }
        if (saltOpt.value().size(jsiRuntime) != crypto_pwhash_SALTBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idDeriveKey salt length is incorrect");
        }
        auto saltData = saltOpt.value().data(jsiRuntime);

        auto keyLength = arguments[2].asNumber();
        auto iterations = arguments[3].asBigInt(jsiRuntime).asUint64(jsiRuntime);
        auto memoryLimit = arguments[4].asBigInt(jsiRuntime).asUint64((jsiRuntime));

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, keyLength);
        uint8_t* keyData = arrayBuffer.data(jsiRuntime);

        if (crypto_pwhash((unsigned char*)keyData, keyLength, (char *)passwordData, passwordSize, saltData, iterations, memoryLimit, crypto_pwhash_ALG_ARGON2ID13) != 0) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] argon2idDeriveKey out of memory");
        }

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idDeriveKey", std::move(argon2idDeriveKey));
  }
}
