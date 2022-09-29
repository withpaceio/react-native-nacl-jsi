#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

namespace react_native_nacl {
	std::vector<uint8_t> base64ToBin(facebook::jsi::Runtime& jsiRuntime, const std::string &str);
	std::string binToBase64(const uint8_t *buffer, size_t length, uint8_t variant);
	std::vector<uint8_t> hexToBin(facebook::jsi::Runtime& jsiRuntime, const std::string &str);
	std::string binToHex(const uint8_t* buffer, size_t length);
}

#endif