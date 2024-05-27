#include <optional>

#include "aes.h"
#include "helpers.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_aes(jsi::Runtime& jsiRuntime) {
    auto aesGenerateKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "aesGenerateKey"),
      0,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        jsi::ArrayBuffer key = getArrayBuffer(jsiRuntime, crypto_aead_aes256gcm_KEYBYTES);
        crypto_aead_aes256gcm_keygen(key.data(jsiRuntime));

        return key;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "aesGenerateKey", std::move(aesGenerateKey));

    auto aesEncrypt = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "aesEncrypt"),
      2,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> messageOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!messageOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesEncrypt message must be an ArrayBuffer");
        }
        auto messageSize = messageOpt.value().size(jsiRuntime);
        auto messageData = messageOpt.value().data(jsiRuntime);
        
        std::optional<jsi::ArrayBuffer> keyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!keyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesEncrypt key must be an ArrayBuffer");
        }
        if (keyOpt.value().size(jsiRuntime) != crypto_aead_aes256gcm_KEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesEncrypt wrong key length");
        }
        auto keyData = keyOpt.value().data(jsiRuntime);
        
        jsi::ArrayBuffer nonceArrayBuffer = getArrayBuffer(jsiRuntime, crypto_aead_aes256gcm_NPUBBYTES);
        auto nonceData = nonceArrayBuffer.data(jsiRuntime);
        randombytes_buf(nonceData, crypto_aead_aes256gcm_NPUBBYTES);
        
        jsi::ArrayBuffer cipherTextArrayBuffer = getArrayBuffer(jsiRuntime, messageSize + crypto_aead_aes256gcm_ABYTES);
        auto cipherTextData = cipherTextArrayBuffer.data(jsiRuntime);
        unsigned long long cipherTextSize = cipherTextArrayBuffer.size(jsiRuntime);

        if (crypto_aead_aes256gcm_encrypt(cipherTextData, &cipherTextSize, messageData, messageSize, NULL, 0, NULL, nonceData, keyData) != 0) {
          jsi::Value(nullptr);
        }

        jsi::Object aesResult = jsi::Object(jsiRuntime);
        aesResult.setProperty(jsiRuntime, "encrypted", cipherTextArrayBuffer);
        aesResult.setProperty(jsiRuntime, "iv", nonceArrayBuffer);

        return aesResult;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "aesEncrypt", std::move(aesEncrypt));

    auto aesDecrypt = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "aesDecrypt"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string cipher_text_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);
        std::string nonce_string = arguments[2].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> key = base64ToBin(jsiRuntime, key_string);
        if (key.size() != crypto_aead_aes256gcm_KEYBYTES) {
					throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_aes256gcm_decrypt wrong key length");
        }

        std::vector<uint8_t> nonce = base64ToBin(jsiRuntime, nonce_string);
        if (nonce.size() != crypto_aead_aes256gcm_NPUBBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_aes256gcm_decrypt wrong nonce length");
        }

        std::vector<uint8_t> cipher_text = base64ToBin(jsiRuntime, cipher_text_string);
        if (cipher_text.size() < crypto_aead_aes256gcm_ABYTES) {
          return jsi::Value(nullptr);
        }

        std::vector<uint8_t> message(cipher_text.size());
        if (crypto_aead_aes256gcm_decrypt(message.data(), NULL, NULL, cipher_text.data(), cipher_text.size(), NULL, 0, nonce.data(), key.data()) != 0) {
          return jsi::Value(nullptr);
        }

				return jsi::String::createFromUtf8(jsiRuntime, message.data(), message.size());
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "aesDecrypt", std::move(aesDecrypt));
  }
}
