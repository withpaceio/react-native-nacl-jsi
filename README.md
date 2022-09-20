# react-native-nacl-jsi

Sodium library for React Native with JSI binding.

## Installation

```sh
npm install react-native-nacl-jsi
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

### `boxGenerateKey(): KeyPair`

Generates a random key pair for box and returns a `KeyPair`.

`type KeyPair = { publicKey: string; secretKey: string }`

### `boxSeal(message: string, recipientPublicKey: string, senderSecretKey: string): string`

Encrypts and authenticates message using the recipient's public key and the sender's secret key.

### `boxOpen(encryptedMessage: string, senderPublicKey: string, recipientSecretKey): string`

Authenticates and decrypts the encrypted message using the sender's public key and the recipient's secret key.

## Secret-key authenticated encryption (secretbox)

### `secretboxGenerateKey(): string`

Generates a random key for secretbox.

### `secretboxSeal(message: string, secretKey: string): string`

Encrypts and authenticates message using the key.

### `secretboxOpen(encryptedMessage: string, secretKey: string): string`

Authenticates and decrypts the encrypted message using the key.

## Password hashing

### `argon2idHash(password: string, iterations: number, memoryLimit: number): string`

Hash the password using the Argon2id algorithm.

### `argon2idVerify(hash: string, password: string): boolean`

Returns `true` if the hash matches the password.

## AES256-GCM

### `aesGenerateKey(): string`

Generates a random key to use for AES256-GCM encryption

### `aesEncrypt(message: string, key: string): AesResult`

Encrypts the message using the key and returns a `AesResult`.

`type AesResult = { encrypted: string; iv: string }`

### `aesDecrypt(encryptedMessage: string, key: string, iv: string)`

Decrypts the encrypted message using the initialisation vector `iv` and the key.

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
