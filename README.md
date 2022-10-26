# react-native-nacl-jsi

Sodium library for React Native with JSI binding.

Precompiled binaries of [libsodium](https://libsodium.org) will be linked by default.
Optionally, you can choose to compile libsodium by yourself (run **npm&nbsp;run&nbsp;rebuild** in package directory). Source code will be downloaded and verified before compilation.

## Source compilation

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

```js
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
} from 'react-native-nacl-jsi';

const key = secretboxGenerateKey();
const encrypted = secretboxSeal('encrypt me', key);
const decrypted = secretboxOpen(encrypted, key);
```

## Public-key authenticated encryption (box)

Generates a random key pair for box and returns a `KeyPair`:

```ts
function boxGenerateKey(): KeyPair;

// With KeyPair:
type KeyPair = {
  publicKey: string;
  secretKey: string;
};
```

Encrypts and authenticates message using the recipient's public key and the sender's secret key:

```ts
function boxSeal(
  message: string,
  recipientPublicKey: string,
  senderSecretKey: string
): string;
```

Authenticates and decrypts the encrypted message using the sender's public key and the recipient's secret key:

```ts
function boxOpen(
  encryptedMessage: string,
  senderPublicKey: string,
  recipientSecretKey
): string;
```

## Secret-key authenticated encryption (secretbox)

Generates a random key for secretbox:

```ts
function secretboxGenerateKey(): string;
```

Encrypts and authenticates message using the key:

```ts
function secretboxSeal(message: string, secretKey: string): string;
```

Authenticates and decrypts the encrypted message using the key:

```ts
function secretboxOpen(encryptedMessage: string, secretKey: string): string;
```

## Signature

Generates a signing key pair:

```ts
function signGenerateKey(): KeyPair;

// With KeyPair:
type KeyPair = {
  publicKey: string;
  secretKey: string;
};
```

Signs a message and returns the signature:

```ts
function signDetached(messageToSign: string, secretKey: string): string;
```

Verifies a signature:

```ts
function signVerifyDetached(
  message: string,
  publicKey: string,
  signature: string
): boolean;
```

## Password hashing

Hash the password using the Argon2id algorithm:

```ts
function argon2idHash(
  password: string,
  iterations: number,
  memoryLimit: number
): string;
```

Verifies a hash and returns `true` if the hash matches the password:

```ts
function argon2idVerify(hash: string, password: string): boolean;
```

## Key derivation

Derives the key using a salt and the Argon2id algorithm:

```ts
function argon2idDeriveKey(
  key: string,
  salt: string,
  keyLength: number,
  iterations: number,
  memoryLimit: number
): string;
```

## AES256-GCM

Generates a random key to use for AES256-GCM encryption:

```ts
function aesGenerateKey(): string;
```

Encrypts the message using the key and returns a `AesResult`:

```ts
function aesEncrypt(message: string, key: string): AesResult;

// With AesResult:
type AesResult = {
  encrypted: string;
  iv: string;
};
```

Decrypts the encrypted message using the initialisation vector `iv` and the key:

```ts
function aesDecrypt(encryptedMessage: string, key: string, iv: string): string;
```

## Generates random bytes

```ts
function getRandomBytes(
  size: number,
  encoding: 'base64' | 'hex' = 'base64'
): string;
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
