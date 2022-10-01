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
        int ARGON2ID_MEMLIMIT_MIN = crypto_pwhash_MEMLIMIT_MIN;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MIN", ARGON2ID_MEMLIMIT_MIN);
        int ARGON2ID_MEMLIMIT_MAX = crypto_pwhash_MEMLIMIT_MAX;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MAX", ARGON2ID_MEMLIMIT_MAX);
        int ARGON2ID_MEMLIMIT_INTERACTIVE = crypto_pwhash_MEMLIMIT_INTERACTIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_INTERACTIVE", ARGON2ID_MEMLIMIT_INTERACTIVE);
        int ARGON2ID_MEMLIMIT_MODERATE = crypto_pwhash_MEMLIMIT_MODERATE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_MODERATE", ARGON2ID_MEMLIMIT_MODERATE);
        int ARGON2ID_MEMLIMIT_SENSITIVE = crypto_pwhash_MEMLIMIT_SENSITIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_MEMLIMIT_SENSITIVE", ARGON2ID_MEMLIMIT_SENSITIVE);

        int ARGON2ID_OPSLIMIT_MIN = crypto_pwhash_OPSLIMIT_MIN;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MIN", ARGON2ID_OPSLIMIT_MIN);
        int ARGON2ID_OPSLIMIT_MAX = crypto_pwhash_OPSLIMIT_MAX;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MAX", ARGON2ID_OPSLIMIT_MAX);
        int ARGON2ID_OPSLIMIT_INTERACTIVE = crypto_pwhash_OPSLIMIT_INTERACTIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_INTERACTIVE", ARGON2ID_OPSLIMIT_INTERACTIVE);
        int ARGON2ID_OPSLIMIT_MODERATE = crypto_pwhash_OPSLIMIT_MODERATE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_MODERATE", ARGON2ID_OPSLIMIT_MODERATE);
        int ARGON2ID_OPSLIMIT_SENSITIVE = crypto_pwhash_OPSLIMIT_SENSITIVE;
        constants.setProperty(jsiRuntime, "ARGON2ID_OPSLIMIT_SENSITIVE", ARGON2ID_OPSLIMIT_SENSITIVE);

        int ARGON2ID_SALTBYTES = crypto_pwhash_SALTBYTES;
        constants.setProperty(jsiRuntime, "ARGON2ID_SALTBYTES", ARGON2ID_SALTBYTES);

        int BOX_PUBLIC_KEY_LENGTH = crypto_box_PUBLICKEYBYTES;
        constants.setProperty(jsiRuntime, "BOX_PUBLIC_KEY_LENGTH", BOX_PUBLIC_KEY_LENGTH);
        int BOX_SECRET_KEY_LENGTH = crypto_box_SECRETKEYBYTES;
        constants.setProperty(jsiRuntime, "BOX_SECRET_KEY_LENGTH", BOX_SECRET_KEY_LENGTH);
        int BOX_NONCE_LENGTH = crypto_box_NONCEBYTES;
        constants.setProperty(jsiRuntime, "BOX_NONCE_LENGTH", BOX_NONCE_LENGTH);
        int BOX_SEED_LENGTH = crypto_box_SEEDBYTES;
        constants.setProperty(jsiRuntime, "BOX_SEED_LENGTH", BOX_SEED_LENGTH);

        int SECRETBOX_KEY_LENGTH = crypto_secretbox_KEYBYTES;
        constants.setProperty(jsiRuntime, "SECRETBOX_KEY_LENGTH", SECRETBOX_KEY_LENGTH);
        int SECRETBOX_NONCE_LENGTH = crypto_secretbox_NONCEBYTES;
        constants.setProperty(jsiRuntime, "SECRETBOX_NONCE_LENGTH", SECRETBOX_NONCE_LENGTH);

        return constants;
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "getConstants", std::move(getConstants));
  }
}