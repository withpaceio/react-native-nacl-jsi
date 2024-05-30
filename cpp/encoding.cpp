#include <algorithm>
#include <optional>

#include "encoding.h"
#include "helpers.h"
#include "sodium.h"

using namespace facebook;

namespace react_native_nacl {
  void install_encoding(jsi::Runtime& jsiRuntime) {
    auto encodeBase64 = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "encodeBase64"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> optArrayBuffer = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!optArrayBuffer.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] encodeBase64 buffer to encode must be an ArrayBuffer");
        }
        auto arrayBufferData = optArrayBuffer.value().data(jsiRuntime);
        auto arrayBufferSize = optArrayBuffer.value().size(jsiRuntime);

        std::string base64;
        base64.resize(sodium_base64_encoded_len(arrayBufferSize, sodium_base64_VARIANT_ORIGINAL));

        sodium_bin2base64(base64.data(), base64.size(), arrayBufferData, arrayBufferSize, sodium_base64_VARIANT_ORIGINAL);
        if (base64.size() && base64[base64.size() - 1] == '\0') {
          base64.resize(base64.size() - 1);
        }

        return jsi::String::createFromUtf8(jsiRuntime, base64.data());
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "encodeBase64", std::move(encodeBase64));

    auto decodeBase64 = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "decodeBase64"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string toDecode = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<u_int8_t> bin;
        bin.resize(toDecode.size() / 4 * 3);
        size_t decodedLength = 0;

        if (sodium_base642bin(bin.data(), bin.size(), toDecode.data(), toDecode.size(), nullptr, &decodedLength, nullptr, sodium_base64_VARIANT_ORIGINAL) != 0) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] decodeBase64 invalid base64 input");
        }

        bin.resize(decodedLength);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, decodedLength);
        std::copy(bin.begin(), bin.end(), arrayBuffer.data(jsiRuntime));

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "decodeBase64", std::move(decodeBase64));

    auto encodeHexadecimal = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "encodeHexadecimal"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> optArrayBuffer = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!optArrayBuffer.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] encodeHexadecimal buffer to encode must be an ArrayBuffer");
        }
        auto arrayBufferData = optArrayBuffer.value().data(jsiRuntime);
        auto arrayBufferSize = optArrayBuffer.value().size(jsiRuntime);

        std::string hex;
        hex.resize(arrayBufferSize * 2 + 1);

        sodium_bin2hex((char *)hex.data(), hex.size(), arrayBufferData, arrayBufferSize);
        if (hex.size() && hex[hex.size() - 1] == '\0') {
          hex.resize(hex.size() - 1);
        }

        return jsi::String::createFromUtf8(jsiRuntime, hex.data());
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "encodeHexadecimal", std::move(encodeHexadecimal));

    auto decodeHexadecimal = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "decodeHexadecimal"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::string toDecode = arguments[0].asString(jsiRuntime).utf8(jsiRuntime);

        std::vector<uint8_t> bin;
        bin.resize(toDecode.size());

        size_t decodedLength = 0;
        if (sodium_hex2bin(bin.data(), bin.size(), toDecode.data(), toDecode.size(), nullptr, &decodedLength, nullptr) != 0) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] decodeHexadecimal invalid hex input");
        }

        bin.resize(decodedLength);

        jsi::ArrayBuffer arrayBuffer = getArrayBuffer(jsiRuntime, decodedLength);
        std::copy(bin.begin(), bin.end(), arrayBuffer.data(jsiRuntime));

        return arrayBuffer;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "decodeHexadecimal", std::move(decodeHexadecimal));

    auto encodeUtf8 = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "encodeUtf8"),
      1,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        std::optional<jsi::ArrayBuffer> optArrayBuffer = getArrayBuffer(jsiRuntime, arguments[0]);
        if (!optArrayBuffer.has_value()) {
          throw jsi::JSError(jsiRuntime, "[react-native-nacl-jsi] encodeUtf8 buffer to encode must be an ArrayBuffer");
        }
        auto arrayBufferData = optArrayBuffer.value().data(jsiRuntime);

        return jsi::String::createFromUtf8(jsiRuntime, (char *)arrayBufferData);
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "encodeUtf8", std::move(encodeUtf8));
  }
}
