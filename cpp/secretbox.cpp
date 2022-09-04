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
				std::vector<uint8_t> secret_key(crypto_secretbox_KEYBYTES);
				crypto_secretbox_keygen(secret_key.data());

				return jsi::String::createFromUtf8(jsiRuntime, binToBase64(secret_key.data(), secret_key.size(), sodium_base64_VARIANT_ORIGINAL));
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxGenerateKey", std::move(secretboxGenerateKey));

		auto secretboxSeal = jsi::Function::createFromHostFunction(
			jsiRuntime,
			jsi::PropNameID::forAscii(jsiRuntime, "secretboxSeal"),
			2,
			[](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
				std::string message_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
				std::string secret_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);

				std::vector<uint8_t> secret_key = base64ToBin(jsiRuntime, secret_key_string);
				if (secret_key.size() != crypto_secretbox_KEYBYTES) {
					jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] crypto_secretbox_easy wrong key length");
				}

				std::vector<uint8_t> nonce(crypto_secretbox_NONCEBYTES);
				randombytes_buf(nonce.data(), crypto_secretbox_NONCEBYTES);

				std::vector<uint8_t> cipher_text;
				unsigned long long cipher_text_length = crypto_secretbox_MACBYTES + message_string.size();
				cipher_text.resize(cipher_text_length);

				if (crypto_secretbox_easy(cipher_text.data(), (uint8_t *)message_string.data(), cipher_text_length, nonce.data(), secret_key.data()) != 0) {
					return jsi::Value(nullptr);
				}

				std::vector<uint8_t> nonce_cipher_text;
				nonce_cipher_text.resize(nonce.size() + cipher_text.size());
				std::move(nonce.begin(), nonce.end(), nonce_cipher_text.begin());
				std::move(cipher_text.begin(), cipher_text.end(), nonce_cipher_text.begin() + nonce.size());

				return jsi::String::createFromUtf8(jsiRuntime, binToBase64(nonce_cipher_text.data(), nonce_cipher_text.size(), sodium_base64_VARIANT_ORIGINAL));
			}
		);
		jsiRuntime.global().setProperty(jsiRuntime, "secretboxSeal", std::move(secretboxSeal));
  }
}