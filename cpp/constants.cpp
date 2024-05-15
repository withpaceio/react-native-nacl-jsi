#include "constants.h"
#include "sodium.h"

using namespace facebook;

namespace react_native_nacl {
  void install_constants(jsi::Runtime& jsiRuntime) {
    auto getConstants = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "getConstants"),
      0,
      [](jsi::Runtime& jsiRuntime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
        jsi::Object constants = jsi::Object(jsiRuntime);
        unsigned long ARGON2ID_MEMLIMIT_MIN = crypto_pwhash_MEMLIMIT_MIN;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MIN", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_MEMLIMIT_MIN));
        unsigned long ARGON2ID_MEMLIMIT_MAX = crypto_pwhash_MEMLIMIT_MAX;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MAX", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_MEMLIMIT_MAX));
        unsigned long ARGON2ID_MEMLIMIT_INTERACTIVE = crypto_pwhash_MEMLIMIT_INTERACTIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_INTERACTIVE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_MEMLIMIT_INTERACTIVE));
        unsigned long ARGON2ID_MEMLIMIT_MODERATE = crypto_pwhash_MEMLIMIT_MODERATE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MODERATE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_MEMLIMIT_MODERATE));
        unsigned long ARGON2ID_MEMLIMIT_SENSITIVE = crypto_pwhash_MEMLIMIT_SENSITIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_SENSITIVE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_MEMLIMIT_SENSITIVE));

        unsigned long ARGON2ID_OPSLIMIT_MIN = crypto_pwhash_OPSLIMIT_MIN;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MIN", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_OPSLIMIT_MIN));
        unsigned long ARGON2ID_OPSLIMIT_MAX = crypto_pwhash_OPSLIMIT_MAX;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MAX", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_OPSLIMIT_MAX));
        unsigned long ARGON2ID_OPSLIMIT_INTERACTIVE = crypto_pwhash_OPSLIMIT_INTERACTIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_INTERACTIVE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_OPSLIMIT_INTERACTIVE));
        unsigned long ARGON2ID_OPSLIMIT_MODERATE = crypto_pwhash_OPSLIMIT_MODERATE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MODERATE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_OPSLIMIT_MODERATE));
        unsigned long ARGON2ID_OPSLIMIT_SENSITIVE = crypto_pwhash_OPSLIMIT_SENSITIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_SENSITIVE", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_OPSLIMIT_SENSITIVE));

        unsigned long ARGON2ID_SALTBYTES = crypto_pwhash_SALTBYTES;
        constants.setProperty(jsiRuntime, "ARGON2ID_SALTBYTES", jsi::BigInt::fromUint64(jsiRuntime, ARGON2ID_SALTBYTES));

        unsigned long BOX_PUBLIC_KEY_LENGTH = crypto_box_PUBLICKEYBYTES;
        constants.setProperty(jsiRuntime, "BOX_PUBLIC_KEY_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, BOX_PUBLIC_KEY_LENGTH));
        unsigned long BOX_SECRET_KEY_LENGTH = crypto_box_SECRETKEYBYTES;
        constants.setProperty(jsiRuntime, "BOX_SECRET_KEY_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, BOX_SECRET_KEY_LENGTH));
        unsigned long BOX_NONCE_LENGTH = crypto_box_NONCEBYTES;
        constants.setProperty(jsiRuntime, "BOX_NONCE_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, BOX_NONCE_LENGTH));
        unsigned long BOX_SEED_LENGTH = crypto_box_SEEDBYTES;
        constants.setProperty(jsiRuntime, "BOX_SEED_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, BOX_SEED_LENGTH));

        unsigned long SECRETBOX_KEY_LENGTH = crypto_secretbox_KEYBYTES;
        constants.setProperty(jsiRuntime, "SECRETBOX_KEY_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, SECRETBOX_KEY_LENGTH));
        unsigned long SECRETBOX_NONCE_LENGTH = crypto_secretbox_NONCEBYTES;
        constants.setProperty(jsiRuntime, "SECRETBOX_NONCE_LENGTH", jsi::BigInt::fromUint64(jsiRuntime, SECRETBOX_NONCE_LENGTH));

        return constants;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "getConstants", std::move(getConstants));
  }
}
