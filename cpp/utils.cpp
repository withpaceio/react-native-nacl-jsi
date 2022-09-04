#include "sodium.h"
#include "utils.h"

using namespace facebook;

namespace react_native_nacl {
	std::vector<uint8_t> base64ToBin(jsi::Runtime& jsiRuntime, const std::string &str) {
		std::vector<u_int8_t> bin;
		bin.resize(str.size() / 4 * 3);
		size_t decoded_len = 0;

		if (sodium_base642bin(bin.data(), bin.size(), str.data(), str.size(), nullptr, &decoded_len, nullptr, sodium_base64_VARIANT_ORIGINAL) != 0) {
			jsi::detail::throwJSError(jsiRuntime, "[react-native-nacl-jsi] invalid base64 input");
		}

		bin.resize(decoded_len);
		return bin;
	}

	std::string binToBase64(const uint8_t *buffer, size_t length, uint8_t variant) {
		std::string base64;
		base64.resize(sodium_base64_encoded_len(length, variant));

		sodium_bin2base64((char *)base64.data(), base64.size(), buffer, length, variant);
		if (base64.size() && base64[base64.size() - 1] == '\0') {
			base64.resize(base64.size() - 1);
		}

		return base64;
	}
}