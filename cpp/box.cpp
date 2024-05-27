#include <optional>

#include "box.h"
#include "helpers.h"
#include "sodium.h"

using namespace facebook;

namespace react_native_nacl {
  void install_box(jsi::Runtime& jsiRuntime) {
    auto boxGenerateKey = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxGenerateKey"),
      0,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        jsi::ArrayBuffer publicKeyArrayBuffer = getArrayBuffer(jsiRuntime, crypto_box_PUBLICKEYBYTES);
        jsi::ArrayBuffer secretKeyArrayBuffer = getArrayBuffer(jsiRuntime, crypto_box_SECRETKEYBYTES);
        crypto_box_keypair(publicKeyArrayBuffer.data(jsiRuntime), secretKeyArrayBuffer.data(jsiRuntime));

        jsi::Object keyPair = jsi::Object(jsiRuntime);
        keyPair.setProperty(jsiRuntime, "publicKey", publicKeyArrayBuffer);
        keyPair.setProperty(jsiRuntime, "secretKey", secretKeyArrayBuffer);

        return keyPair;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxGenerateKey", std::move(boxGenerateKey));

    auto boxSeal = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxSeal"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> optMessage = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!optMessage.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxSeal message must be an ArrayBuffer");
        }
        auto messageSize = optMessage.value().size(jsiRuntime);
        auto messageData = optMessage.value().data(jsiRuntime);

        std::optional<jsi::ArrayBuffer> optPublicKey = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!optPublicKey.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxSeal public key must be an ArrayBuffer");
        }
        if (optPublicKey.value().size(jsiRuntime) != crypto_box_PUBLICKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxSeal wrong public key length");
        }
        auto publicKeyData = optPublicKey.value().data(jsiRuntime);

        std::optional<jsi::ArrayBuffer> optSecretKey = getArrayBuffer(jsiRuntime, arguments[2]);
        if (!optSecretKey.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxSeal secret key must be an ArrayBuffer");
        }
        if (optSecretKey.value().size(jsiRuntime) != crypto_box_SECRETKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxSeal wrong secret key length");
        }
        auto secretKeyData = optSecretKey.value().data(jsiRuntime);

        std::vector<uint8_t> nonce(crypto_box_NONCEBYTES);
        randombytes_buf(nonce.data(), crypto_box_NONCEBYTES);

				std::vector<uint8_t> cipherText;
				auto cipherTextLength = crypto_box_MACBYTES + messageSize;
				cipherText.resize(cipherTextLength);

        if (crypto_box_easy(cipherText.data(), messageData, messageSize, nonce.data(), publicKeyData, secretKeyData) != 0) {
					return jsi::Value(nullptr);
        }

        jsi::ArrayBuffer nonceCipherText = getArrayBuffer(jsiRuntime, nonce.size() + cipherText.size());
        auto data = nonceCipherText.data(jsiRuntime);

        std::move(nonce.begin(), nonce.end(), data);
        std::move(cipherText.begin(), cipherText.end(), &(data[crypto_box_NONCEBYTES]));

        return nonceCipherText;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxSeal", std::move(boxSeal));

    auto boxOpen = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxOpen"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> nonceCipherTextOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!nonceCipherTextOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxOpen encryptedMessage must be an Uint8Array");
        }
        auto nonceCipherTextData = nonceCipherTextOpt.value().data(jsiRuntime);
        auto nonceCipherTextSize = nonceCipherTextOpt.value().size(jsiRuntime);

        std::optional<jsi::ArrayBuffer> publicKeyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!publicKeyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxOpen publicKey must be an Uint8Array");
        }
        if (publicKeyOpt.value().size(jsiRuntime) != crypto_box_PUBLICKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxOpen wrong public key length");
        }
        auto publicKeyData = publicKeyOpt.value().data(jsiRuntime);

        std::optional<jsi::ArrayBuffer> secretKeyOpt = getArrayBuffer(jsiRuntime, arguments[2]);
        if (!secretKeyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxOpen secretKey must be an Uint8Array");
        }
        if (secretKeyOpt.value().size(jsiRuntime) != crypto_box_SECRETKEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] boxOpen wrong secret key length");
        }
        auto secretKeyData = secretKeyOpt.value().data(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, nonceCipherTextSize - crypto_box_NONCEBYTES - crypto_box_MACBYTES);
        uint8_t* message = arrayBuffer.data(jsiRuntime);

        if (crypto_box_open_easy(message, &nonceCipherTextData[crypto_box_NONCEBYTES], nonceCipherTextSize - crypto_box_NONCEBYTES, nonceCipherTextData, publicKeyData, secretKeyData) != 0) {
          return jsi::Value(nullptr);
        }

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxOpen", std::move(boxOpen));
  }

}
