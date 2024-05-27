#include <optional>

#include "helpers.h"
#include "secretbox.h"
#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
  void install_secret_box(jsi::Runtime& jsiRuntime) {
		auto secretboxGenerateKey = jsi::Function::createFromHostFunction(
			jsiRuntime,
			jsi::PropNameID::forAscii(jsiRuntime, "secretboxGenerateKey"),
			0,
			[](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        jsi::ArrayBuffer secretKey = getArrayBuffer(jsiRuntime, crypto_secretbox_KEYBYTES);
				crypto_secretbox_keygen(secretKey.data(jsiRuntime));

        return secretKey;
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxGenerateKey", std::move(secretboxGenerateKey));

		auto secretboxSeal = jsi::Function::createFromHostFunction(
			jsiRuntime,
			jsi::PropNameID::forAscii(jsiRuntime, "secretboxSeal"),
			2,
			[](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> messageOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!messageOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxSeal message must be an ArrayBuffer");
        }
        auto messageData = messageOpt.value().data(jsiRuntime);
        auto messageSize = messageOpt.value().size(jsiRuntime);

        std::optional<jsi::ArrayBuffer> secretKeyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!secretKeyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxSeal secretKey must be an ArrayBuffer");
        }
        if (secretKeyOpt.value().size(jsiRuntime) != crypto_secretbox_KEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxSeal wrong key length");
        }
        auto secretKeyData = secretKeyOpt.value().data(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, messageSize + crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES);
        uint8_t* nonceCipherText = arrayBuffer.data(jsiRuntime);

        randombytes_buf(nonceCipherText, crypto_secretbox_NONCEBYTES);

				if (crypto_secretbox_easy(&nonceCipherText[crypto_secretbox_NONCEBYTES], messageData, messageSize, nonceCipherText, secretKeyData) != 0) {
					return jsi::Value(nullptr);
				}

        return arrayBuffer;
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxSeal", std::move(secretboxSeal));

		auto secretboxOpen = jsi::Function::createFromHostFunction(
			jsiRuntime,
			jsi::PropNameID::forAscii(jsiRuntime, "secretboxOpen"),
			2,
			[](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
				std::string nonce_cipher_text_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
				std::string secret_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);

				std::vector<u_int8_t> secret_key = base64ToBin(jsiRuntime, secret_key_string);
				if (secret_key.size() != crypto_secretbox_KEYBYTES) {
					throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] crypto_secretbox_open_easy wrong key length");
				}

				std::vector<uint8_t> nonce_cipher_text = base64ToBin(jsiRuntime, nonce_cipher_text_string);
				std::vector<uint8_t> nonce(crypto_secretbox_NONCEBYTES);
				std::move(nonce_cipher_text.begin(), nonce_cipher_text.begin() + crypto_secretbox_NONCEBYTES, nonce.begin());
				std::vector<uint8_t> cipher_text(nonce_cipher_text.size() - nonce.size());
				std::move(nonce_cipher_text.begin() + crypto_secretbox_NONCEBYTES, nonce_cipher_text.end(), cipher_text.begin());

				std::vector<uint8_t> message(cipher_text.size());
				if (crypto_secretbox_open_easy(message.data(), cipher_text.data(), message.size(), nonce.data(), secret_key.data()) != 0) {
					return jsi::Value(nullptr);
				}

				return jsi::String::createFromUtf8(jsiRuntime, message.data(), message.size());
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxOpen", std::move(secretboxOpen));

  }
}
