#include "box.h"
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
        std::vector<uint8_t> public_key(crypto_box_PUBLICKEYBYTES);
        std::vector<uint8_t> secret_key(crypto_box_SECRETKEYBYTES);
        crypto_box_keypair(public_key.data(), secret_key.data());

        jsi::Object key_pair = jsi::Object(jsiRuntime);
        key_pair.setProperty(jsiRuntime, "publicKey", binToBase64(public_key.data(), public_key.size(), sodium_base64_VARIANT_ORIGINAL));
        key_pair.setProperty(jsiRuntime, "secretKey", binToBase64(secret_key.data(), secret_key.size(), sodium_base64_VARIANT_ORIGINAL));

        return key_pair;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "boxGenerateKey", std::move(boxGenerateKey));

    auto boxSeal = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "boxSeal"),
      3,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
				std::string message_string = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);
				std::string recipient_public_key_string = arguments[1].asString(jsiRuntime).utf8(jsiRuntime);
				std::string sender_secret_key_string = arguments[2].asString(jsiRuntime).utf8(jsiRuntime);

				std::vector<uint8_t> recipient_public_key = base64ToBin(jsiRuntime, recipient_public_key_string);
				if (recipient_public_key.size() != crypto_box_PUBLICKEYBYTES) {
					jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy wrong public key length");
				}

				std::vector<uint8_t> sender_secret_key = base64ToBin(jsiRuntime, sender_secret_key_string);
				if (sender_secret_key.size() != crypto_box_SECRETKEYBYTES) {
					jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy wrong secret key length");
				}

        std::vector<uint8_t> nonce(crypto_box_NONCEBYTES);
        randombytes_buf(nonce.data(), crypto_box_NONCEBYTES);

				std::vector<uint8_t> cipher_text;
				unsigned long long cipher_text_length = crypto_box_MACBYTES + message_string.size();
				cipher_text.resize(cipher_text_length);

        if (crypto_box_easy(cipher_text.data(), (uint8_t *)message_string.data(), message_string.size(), nonce.data(), recipient_public_key.data(), sender_secret_key.data()) != 0) {
					return jsi::Value(nullptr);
        }

				std::vector<uint8_t> nonce_cipher_text;
				nonce_cipher_text.resize(nonce.size() + cipher_text.size());
				std::move(nonce.begin(), nonce.end(), nonce_cipher_text.begin());
				std::move(cipher_text.begin(), cipher_text.end(), nonce_cipher_text.begin() + crypto_box_NONCEBYTES);

				return jsi::String::createFromUtf8(jsiRuntime, binToBase64(nonce_cipher_text.data(), nonce_cipher_text.size(), sodium_base64_VARIANT_ORIGINAL));
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
					jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy_open wrong public key length");
        }

        std::vector<uint8_t> recipient_secret_key = base64ToBin(jsiRuntime, recipient_public_key_string);
        if (recipient_secret_key.size() != crypto_box_SECRETKEYBYTES) {
					jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] crypto_box_easy_open wrong secret key length");
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