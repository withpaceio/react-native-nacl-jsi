#include <optional>

#include "aes.h"
#include "helpers.h"
#include "sodium.h"

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

        // always returns 0
        crypto_aead_aes256gcm_encrypt(cipherTextData, &cipherTextSize, messageData, messageSize, NULL, 0, NULL, nonceData, keyData);

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
        std::optional<jsi::ArrayBuffer> cipherTextOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!cipherTextOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt cipherText must be an ArrayBuffer");
        }
        auto cipherTextData = cipherTextOpt.value().data(jsiRuntime);
        auto cipherTextSize = cipherTextOpt.value().size(jsiRuntime);
        if (cipherTextSize < crypto_aead_aes256gcm_ABYTES) {
          return jsi::Value(nullptr);
        }

        std::optional<jsi::ArrayBuffer> keyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!keyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt key must be an ArrayBuffer");
        }
        if (keyOpt.value().size(jsiRuntime) != crypto_aead_aes256gcm_KEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt wrong key length");
        }
        auto keyData = keyOpt.value().data(jsiRuntime);

        std::optional<jsi::ArrayBuffer> ivOpt = getArrayBuffer(jsiRuntime, arguments[2]);
        if (!ivOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt iv must be an ArrayBuffer");
        }
        if (ivOpt.value().size(jsiRuntime) != crypto_aead_aes256gcm_NPUBBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt wrong iv length");
        }
        auto ivData = ivOpt.value().data(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, cipherTextSize - crypto_aead_aes256gcm_ABYTES);

        if (crypto_aead_aes256gcm_decrypt(arrayBuffer.data(jsiRuntime), NULL, NULL, cipherTextData, cipherTextSize, NULL, 0, ivData, keyData) != 0) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] aesDecrypt verification failed");
        }

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "aesDecrypt", std::move(aesDecrypt));
  }
}
