import { NativeModules } from 'react-native';

import type { AesResult, KeyPair } from './types';

const g = global as any;
const NaClModule = NativeModules.NaclJsi;

if (NaClModule && typeof NaClModule.install === 'function') {
  NaClModule.install();
}

export type { AesResult, KeyPair };

const constants = g.getConstants();

export const ARGON2ID_MEMLIMIT_MIN: BigInt = constants.ARGON2ID_MEMLIMIT_MIN;
export const ARGON2ID_MEMLIMIT_MAX: BigInt = constants.ARGON2ID_MEMLIMIT_MAX;
export const ARGON2ID_MEMLIMIT_INTERACTIVE: BigInt =
  constants.ARGON2ID_MEMLIMIT_INTERACTIVE;
export const ARGON2ID_MEMLIMIT_MODERATE: BigInt =
  constants.ARGON2ID_MEMLIMIT_MODERATE;
export const ARGON2ID_MEMLIMIT_SENSITIVE: BigInt =
  constants.ARGON2ID_MEMLIMIT_SENSITIVE;

export const ARGON2ID_OPSLIMIT_MIN: BigInt = constants.ARGON2ID_OPSLIMIT_MIN;
export const ARGON2ID_OPSLIMIT_MAX: BigInt = constants.ARGON2ID_OPSLIMIT_MAX;
export const ARGON2ID_OPSLIMIT_INTERACTIVE: BigInt =
  constants.ARGON2ID_OPSLIMIT_INTERACTIVE;
export const ARGON2ID_OPSLIMIT_MODERATE: BigInt =
  constants.ARGON2ID_OPSLIMIT_MODERATE;
export const ARGON2ID_OPSLIMIT_SENSITIVE: BigInt =
  constants.ARGON2ID_OPSLIMIT_SENSITIVE;

export const ARGON2ID_SALTBYTES: BigInt = constants.ARGON2ID_SALTBYTES;

export const BOX_PUBLIC_KEY_LENGTH: BigInt = constants.BOX_PUBLIC_KEY_LENGTH;
export const BOX_SECRET_KEY_LENGTH: BigInt = constants.BOX_SECRET_KEY_LENGTH;
export const BOX_NONCE_LENGTH: BigInt = constants.BOX_NONCE_LENGTH;
export const BOX_SEED_LENGTH: BigInt = constants.BOX_SEED_LENGTH;

export const SECRETBOX_KEY_LENGTH: BigInt = constants.SECRETBOX_KEY_LENGTH;
export const SECRETBOX_NONCE_LENGTH: BigInt = constants.SECRETBOX_NONCE_LENGTH;

/**
 * Encodes a buffer into a base64 string
 *
 * @throws when the buffer is not an `Uint8Array`
 */
export function encodeBase64(buffer: Uint8Array): string {
  return g.encodeBase64(buffer.buffer);
}

/**
 * Decodes a base64 string into a buffer
 *
 * @throws when the given string is not a valid base64 string
 */
export function decodeBase64(toEncode: string): Uint8Array {
  return new Uint8Array(g.decodeBase64(toEncode));
}

/**
 * Encodes a buffer into an hexadecimal string
 *
 * @throws when buffer is not an `Uint8Array`
 */
export function encodeHexadecimal(buffer: Uint8Array): string {
  return g.encodeHexadecimal(buffer.buffer);
}

/**
 * Decodes an hexadecimal string into an Uint8Array
 *
 * @throws when the given string is not a valid hexadecimal string
 */
export function decodeHexadecimal(toDecode: string): Uint8Array {
  return new Uint8Array(g.decodeHexadecimal(toDecode));
}

/**
 * Encodes a buffer into a UTF-8 string
 *
 * @throws when buffer is not an `Uint8Array`
 */
export function encodeUtf8(buffer: Uint8Array): string {
  return g.encodeUtf8(buffer.buffer);
}

/**
 * Decodes a UTF-8 string into an `Uint8Array`
 */
export function decodeUtf8(input: string): Uint8Array {
  return new TextEncoder().encode(input);
}

/**
 * Generates a key to be used with aesEncrypt and aesDecrypt
 */
export function aesGenerateKey(): Uint8Array {
  const key = g.aesGenerateKey();
  return new Uint8Array(key);
}

/**
 * Encrypts a message with AES-256-GCM using the given key
 *
 * @throws when message is not an `Uint8Array`
 * @throws when key is not an `Uint8Array`
 * @throws when key length is incorrect
 */
export function aesEncrypt(message: Uint8Array, key: Uint8Array): AesResult {
  const aesResult: { encrypted: ArrayBuffer; iv: ArrayBuffer } = g.aesEncrypt(
    message.buffer,
    key.buffer
  );

  return {
    encrypted: new Uint8Array(aesResult.encrypted),
    iv: new Uint8Array(aesResult.iv),
  };
}

/**
 * Decrypts a cipher text with AES-256-GCM using the given key and the initialization vector
 *
 * @throws when cipherText is not an `Uint8Array`
 * @throws when key is not an `Uint8Array`
 * @throws when key length is incorrect
 * @throws when iv is not an `Uint8Array`
 * @throws when iv length is not an `Uint8Array`
 * @throws when the decryption failed
 */
export function aesDecrypt(
  cipherText: Uint8Array,
  key: Uint8Array,
  iv: Uint8Array
): Uint8Array {
  const decrypted: ArrayBuffer = g.aesDecrypt(
    cipherText.buffer,
    key.buffer,
    iv.buffer
  );

  return new Uint8Array(decrypted);
}

/**
 * Generates a key pair to be used with boxSeal and boxOpen
 */
export function boxGenerateKey(): KeyPair {
  const keyPair: { publicKey: ArrayBuffer; secretKey: ArrayBuffer } =
    g.boxGenerateKey();

  return {
    publicKey: new Uint8Array(keyPair.publicKey),
    secretKey: new Uint8Array(keyPair.secretKey),
  };
}

/**
 * Encrypts a message using the recipient's public key and the sender's secret key
 *
 * @throws when message is not an `Uint8Array`
 * @throws when recipientPublicKey is not an `Uint8Array`
 * @throws when recipientPublicKey length is incorrect
 * @throws when senderSecretKey is not an `Uint8Array`
 * @throws when senderSecretKey length is incorrect
 * @throws when the encryption didn't succeed successfully
 */
export function boxSeal(
  message: Uint8Array,
  recipientPublicKey: Uint8Array,
  senderSecretKey: Uint8Array
): Uint8Array {
  const encrypted: ArrayBuffer = g.boxSeal(
    message.buffer,
    recipientPublicKey.buffer,
    senderSecretKey.buffer
  );

  return new Uint8Array(encrypted);
}

/**
 * Verifies and decrypts a ciphertext produced by `boxSeal`
 *
 * @throws when cipherText is not an `Uint8Array`
 * @throws when senderPublicKey is not an `Uint8Array`
 * @throws when senderPublicKey length is incorrect
 * @throws when recipientSecretKey is not an `Uint8Array`
 * @throws when recipientSecretKey length is incorrect
 * @throws when the verification failed
 */
export function boxOpen(
  cipherText: Uint8Array,
  senderPublicKey: Uint8Array,
  recipientSecretKey: Uint8Array
): Uint8Array {
  const decrypted: ArrayBuffer = g.boxOpen(
    cipherText.buffer,
    senderPublicKey.buffer,
    recipientSecretKey.buffer
  );

  return new Uint8Array(decrypted);
}

/**
 * Generates a key to be used with secretboxSeal and secretboxOpen
 */
export function secretboxGenerateKey(): Uint8Array {
  const secretKey = g.secretboxGenerateKey();
  return new Uint8Array(secretKey);
}

/**
 * Encrypts a message with the given secret key
 *
 * @throws when message is not a `Uint8Array`
 * @throws when secretKey is not a `Uint8Array`
 * @throws when secretKey length is incorrect
 * @throws when the encryption didn't complete successfully
 */
export function secretboxSeal(
  message: Uint8Array,
  secretKey: Uint8Array
): Uint8Array {
  const encrypted: ArrayBuffer = g.secretboxSeal(
    message.buffer,
    secretKey.buffer
  );

  return new Uint8Array(encrypted);
}

/**
 * Verifies and decrypts a ciphertext produced by `secretboxSeal`
 *
 * @throws when cipherText is not an `Uint8Array`
 * @throws when secretKey is not an `Uint8Array`
 * @throws when secretKey length is incorrect
 * @throws when the verification failed
 */
export function secretboxOpen(
  cipherText: Uint8Array,
  secretKey: Uint8Array
): Uint8Array {
  const decrypted: ArrayBuffer = g.secretboxOpen(
    cipherText.buffer,
    secretKey.buffer
  );

  return new Uint8Array(decrypted);
}

/**
 * Generates a key pair for signing and verifying signatures
 */
export function signGenerateKey(): KeyPair {
  const keyPair: { publicKey: ArrayBuffer; secretKey: ArrayBuffer } =
    g.signGenerateKey();

  return {
    publicKey: new Uint8Array(keyPair.publicKey),
    secretKey: new Uint8Array(keyPair.secretKey),
  };
}

/**
 * Signs a message with the given secret key
 *
 * @throws when message is not a `Uint8Array`
 * @throws when secretKey is not a `Uint8Array`
 * @throws when secretKey length is incorrect
 */
export function signDetached(
  message: Uint8Array,
  secretKey: Uint8Array
): Uint8Array {
  const signature: ArrayBuffer = g.signDetached(
    message.buffer,
    secretKey.buffer
  );

  return new Uint8Array(signature);
}

/**
 * Verifies that the signature is valid given the message the public key
 *
 * @throws when message is not a `Uint8Array`
 * @throws when publicKey is not a `Uint8Array`
 * @throws when publicKey length is incorrect
 * @throws when signature is not a `Uint8Array`
 */
export function signVerifyDetached(
  message: Uint8Array,
  publicKey: Uint8Array,
  signature: Uint8Array
): boolean {
  return g.signVerifyDetached(
    message.buffer,
    publicKey.buffer,
    signature.buffer
  );
}

/**
 * Hash a password using the Argon2id algorithm
 *
 * @throws when password is not a `Uint8Array`
 * @throws when the hash computation didn't complete successfully
 */
export function argon2idHash(
  password: Uint8Array,
  iterations: BigInt,
  memoryLimit: BigInt
): string {
  return g.argon2idHash(password.buffer, iterations, memoryLimit);
}

/**
 * Verifies that `hash` is a valid `password` verification string
 *
 * @throws when password is not a `Uint8Array`
 */
export function argon2idVerify(hash: string, password: Uint8Array): boolean {
  return g.argon2idVerify(hash, password.buffer);
}

/**
 * Derives a key of the given length
 *
 * @throws when password is not a `Uint8Array`
 * @throws when salt is not a `Uint8Array` or its size is not `ARGON2ID_SALTBYTES`
 */
export function argon2idDeriveKey(
  password: Uint8Array,
  salt: Uint8Array,
  keyLength: number,
  iterations: BigInt,
  memoryLimit: BigInt
): Uint8Array {
  const key = g.argon2idDeriveKey(
    password.buffer,
    salt.buffer,
    keyLength,
    iterations,
    memoryLimit
  );

  return new Uint8Array(key);
}

/**
 * Returns a buffer of the given size filled with random bytes
 *
 * @throws When the given size is incorrect
 */
export function getRandomBytes(size: number | BigInt): Uint8Array {
  const randomBytes: ArrayBuffer = g.getRandomBytes(size);
  return new Uint8Array(randomBytes);
}
