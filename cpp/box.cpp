#include <optional>

#include "box.h"
#include "helpers.h"
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
        auto secretKeyData = optPublicKey.value().data(jsiRuntime);

        std::vector<uint8_t> nonce(crypto_box_NONCEBYTES);
        randombytes_buf(nonce.data(), crypto_box_NONCEBYTES);

				std::vector<uint8_t> cipher_text;
				auto cipher_text_length = crypto_box_MACBYTES + messageSize;
				cipher_text.resize(cipher_text_length);

        if (crypto_box_easy(cipher_text.data(), messageData, messageSize, nonce.data(), publicKeyData, secretKeyData) != 0) {
					return jsi::Value(nullptr);
        }
        
        jsi::ArrayBuffer nonceCipherText = getArrayBuffer(jsiRuntime, nonce.size() + cipher_text.size());
        auto data = nonceCipherText.data(jsiRuntime);
        
        std::move(nonce.begin(), nonce.end(), data);
        std::move(cipher_text.begin(), cipher_text.end(), &(data[crypto_box_NONCEBYTES]));
        
        return nonceCipherText;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxSeal", std::move(boxSeal));

    auto boxOpen = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxOpen"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string nonce_cipher_text_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
        std::string sender_public_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);
        std::string recipient_public_key_string = arguments[2].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> sender_public_key = base64ToBin(jsiRuntime, sender_public_key_string);
        if (sender_public_key.size() != crypto_box_PUBLICKEYBYTES) {
					throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy_open wrong public key length");
        }

        std::vector<uint8_t> recipient_secret_key = base64ToBin(jsiRuntime, recipient_public_key_string);
        if (recipient_secret_key.size() != crypto_box_SECRETKEYBYTES) {
					throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy_open wrong secret key length");
        }

				std::vector<uint8_t> nonce_cipher_text = base64ToBin(jsiRuntime, nonce_cipher_text_string);
				std::vector<uint8_t> nonce(crypto_box_NONCEBYTES);
				std::move(nonce_cipher_text.begin(), nonce_cipher_text.begin() + crypto_box_NONCEBYTES, nonce.begin());
				std::vector<uint8_t> cipher_text(nonce_cipher_text.size() - nonce.size());
				std::move(nonce_cipher_text.begin() + crypto_box_NONCEBYTES, nonce_cipher_text.end(), cipher_text.begin());

				std::vector<uint8_t> message(cipher_text.size());
        if (crypto_box_open_easy(message.data(), cipher_text.data(), cipher_text.size(), nonce.data(), sender_public_key.data(), recipient_secret_key.data()) != 0) {
          return jsi::Value(nullptr);
        }

				return jsi::String::createFromUtf8(jsiRuntime, message.data(), message.size());
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxOpen", std::move(boxOpen));
  }

}