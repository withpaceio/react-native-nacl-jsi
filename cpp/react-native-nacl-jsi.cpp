#include "react-native-nacl-jsi.h"

using namespace facebook;

namespace react_native_nacl {
	void install(jsi::Runtime& jsiRuntime) {
		auto helloJsi = jsi::Function::createFromHostFunction(
			jsiRuntime,
			jsi::PropNameID::forAscii(jsiRuntime, "helloJsi"),
			0,
			[](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
				return jsi::Value(jsi::String::createFromUtf8(jsiRuntime, "Hello JSI!"));
			}
		);

		jsiRuntime.global().setProperty(jsiRuntime, "helloJsi", std::move(helloJsi));
	}

	void cleanup() {
	}
}
