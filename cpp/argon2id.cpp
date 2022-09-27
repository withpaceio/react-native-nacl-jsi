#include "argon2id.h"
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
				std::string password_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
				int iterations = arguments[1].asNumber();
        int memory_limit = arguments[2].asNumber();

        char hashed_password[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(hashed_password, password_string.data(), password_string.size(), iterations, memory_limit) != 0) {
					throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_pwhash_str out of memory");
        }

				return jsi::String::createFromUtf8(jsiRuntime, hashed_password);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idHash", std::move(argon2idHash));

    auto argon2idVerify = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "argon2idVerify"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string hash_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string password_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);

        bool result = crypto_pwhash_str_verify(hash_string.data(), password_string.data(), password_string.size()) == 0;
        return jsi::Value(result);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idVerify", std::move(argon2idVerify));

    auto argon2idDeriveKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "argon2idDeriveKey"),
      5,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string password_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string salt_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);
        int key_length = arguments[2].asNumber();
        int iterations = arguments[3].asNumber();
        int memory_limit = arguments[4].asNumber();

        std::vector<uint8_t> key(key_length);
        std::vector<uint8_t> salt = base64ToBin(jsiRuntime, salt_string);

        if (crypto_pwhash(key.data(), key_length, password_string.data(), password_string.size(), salt.data(), iterations, memory_limit, crypto_pwhash_ALG_ARGON2ID13) != 0) {
          jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_pwhash out of memory");
        }

        return jsi::String::createFromUtf8(jsiRuntime, binToBase64(key.data(), key.size(), sodium_base64_VARIANT_ORIGINAL));
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "argon2idDeriveKey", std::move(argon2idDeriveKey));
  }
}