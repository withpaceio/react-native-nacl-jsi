# react-native-nacl-jsi

Sodium library for React Native with JSI binding.

## Features

- ⚡️ High performance integration of the Sodium library written in C++
- 🔗 Synchronous calls without the bridge, uses [JSI](https://reactnative.dev/docs/the-new-architecture/landing-page#fast-javascriptnative-interfacing)
- 🧩 Compatible with the new architecture

## Source compilation

Precompiled binaries of [libsodium](https://libsodium.org) will be linked by default.
Optionally, you can choose to compile libsodium by yourself (run **npm&nbsp;run&nbsp;rebuild** in package directory). Source code will be downloaded and verified before compilation.

### MacOS prerequisites

- libtool (macports, homebrew)
- autoconf (macports, homebrew)
- automake (macports, homebrew)
- Xcode (12 or newer)

### Android prerequisites

- Android NDK
- CMake
- LLDB

### Recompile

```sh
npm run rebuild
```

## Installation

Using Hermes on Android is required.

```sh
npm install react-native-nacl-jsi
npx pod-install
```

## Usage

## Encoding/decoding

All functions takes and returns `Uint8Array`.
There are helper functions that support encoding and decoding to and from UTF-8, hexadecimal and base64 strings.

## Example

```js
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
} from 'react-native-nacl-jsi';

const key = secretboxGenerateKey();
const encrypted = secretboxSeal(decodeUtf8('encrypt me'), key);
const decrypted = secretboxOpen(encrypted, key);

console.log(encodeUtf8(decrypted));
```

## Public-key authenticated encryption (box)

Generates a random key pair for box and returns a `KeyPair`:

```ts
function boxGenerateKey(): KeyPair;

// With KeyPair:
type KeyPair = {
  publicKey: Uint8Array;
  secretKey: Uint8Array;
};
```

Encrypts and authenticates message using the recipient's public key and the sender's secret key:

```ts
function boxSeal(
  message: Uint8Array,
  recipientPublicKey: Uint8Array,
  senderSecretKey: Uint8Array
): Uint8Array;
```

Authenticates and decrypts the encrypted message using the sender's public key and the recipient's secret key:

```ts
function boxOpen(
  encryptedMessage: Uint8Array,
  senderPublicKey: Uint8Array,
  recipientSecretKey: Uint8Array
): Uint8Array;
```

## Secret-key authenticated encryption (secretbox)

Generates a random key for secretbox:

```ts
function secretboxGenerateKey(): Uint8Array;
```

Encrypts and authenticates message using the key:

```ts
function secretboxSeal(message: Uint8Array, secretKey: Uint8Array): Uint8Array;
```

Authenticates and decrypts the encrypted message using the key:

```ts
function secretboxOpen(
  encryptedMessage: Uint8Array,
  secretKey: Uint8Array
): Uint8Array;
```

## Signature

Generates a signing key pair:

```ts
function signGenerateKey(): KeyPair;

// With KeyPair:
type KeyPair = {
  publicKey: Uint8Array;
  secretKey: Uint8Array;
};
```

Signs a message and returns the signature:

```ts
function signDetached(
  messageToSign: Uint8Array,
  secretKey: Uint8Array
): Uint8Array;
```

Verifies a signature:

```ts
function signVerifyDetached(
  message: Uint8Array,
  publicKey: Uint8Array,
  signature: Uint8Array
): boolean;
```

## Password hashing

Hash the password using the Argon2id algorithm:

```ts
function argon2idHash(
  password: Uint8Array,
  iterations: BigInt,
  memoryLimit: BigInt
): Promise<string>;
```

Verifies a hash and returns `true` if the hash matches the password:

```ts
function argon2idVerify(hash: string, password: Uint8Array): Promise<boolean>;
```

## Key derivation

Derives the key using a salt and the Argon2id algorithm:

```ts
function argon2idDeriveKey(
  key: Uint8Array,
  salt: Uint8Array,
  keyLength: number,
  iterations: BigInt,
  memoryLimit: BigInt
): Promise<Uint8Array>;
```

## AES256-GCM

Generates a random key to use for AES256-GCM encryption:

```ts
function aesGenerateKey(): Uint8Array;
```

Encrypts the message using the key and returns a `AesResult`:

```ts
function aesEncrypt(message: Uint8Array, key: Uint8Array): Promise<AesResult>;

// With AesResult:
type AesResult = {
  encrypted: Uint8Array;
  iv: Uint8Array;
};
```

Decrypts the encrypted message using the initialisation vector `iv` and the key:

```ts
function aesDecrypt(
  cipherText: Uint8Array,
  key: Uint8Array,
  iv: Uint8Array
): Promise<Uint8Array>;
```

## Generates random bytes

```ts
function getRandomBytes(size: number | BigInt): Uint8Array;
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## Author

Rémy Dautriche ([@remydautriche](https://twitter.com/remydautriche))

[PACE](https://withpace.io), a private fitness tracker, implements its cryptography using `react-native-nacl-jsi`.

<div>
  <a href="https://apps.apple.com/app/pace-privacy/id6444367013">
    <img height="40" src="/assets/img/appstore.svg">
  </a>
  <a href="https://play.google.com/store/apps/details?id=io.withpace.pace">
    <img height="40" src="/assets/img/googleplay.svg">
  </a>
</div>

## License

MIT
