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
        std::optional<jsi::ArrayBuffer> nonceCipherTextOpt = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!nonceCipherTextOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxOpen cipherText must be an ArrayBuffer");
        }
        auto nonceCipherTextData = nonceCipherTextOpt.value().data(jsiRuntime);
        auto nonceCipherTextSize = nonceCipherTextOpt.value().size(jsiRuntime);

        std::optional<jsi::ArrayBuffer> secretKeyOpt = getArrayBuffer(jsiRuntime, arguments[1]);
        if (!secretKeyOpt.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxSeal secretKey must be an ArrayBuffer");
        }
        if (secretKeyOpt.value().size(jsiRuntime) != crypto_secretbox_KEYBYTES) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] secretboxSeal wrong key length");
        }
        auto secretKeyData = secretKeyOpt.value().data(jsiRuntime);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, nonceCipherTextSize - crypto_secretbox_NONCEBYTES - crypto_secretbox_MACBYTES);
        uint8_t* message = arrayBuffer.data(jsiRuntime);

				if (crypto_secretbox_open_easy(message, &nonceCipherTextData[crypto_secretbox_NONCEBYTES], nonceCipherTextSize - crypto_secretbox_NONCEBYTES, nonceCipherTextData, secretKeyData) != 0) {
					return jsi::Value(nullptr);
				}

        return arrayBuffer;
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxOpen", std::move(secretboxOpen));

  }
}
