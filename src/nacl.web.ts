import * as base64 from '@stablelib/base64';
import * as hex from '@stablelib/hex';
import * as utf8 from '@stablelib/utf8';
import { box, randomBytes, secretbox, sign } from 'tweetnacl';

import type { AesResult, KeyPair } from './types';

export type { AesResult, KeyPair };

export const BOX_PUBLIC_KEY_LENGTH: BigInt = BigInt(box.publicKeyLength);
export const BOX_SECRET_KEY_LENGTH: BigInt = BigInt(box.secretKeyLength);
export const BOX_NONCE_LENGTH: BigInt = BigInt(box.nonceLength);
export const BOX_SEED_LENGTH: BigInt = BigInt(32);

export const SECRETBOX_KEY_LENGTH: BigInt = BigInt(secretbox.keyLength);
export const SECRETBOX_NONCE_LENGTH: BigInt = BigInt(secretbox.nonceLength);

/**
 * Encodes a buffer into a base64 string
 *
 * @throws when the buffer is not an `Uint8Array`
 */
export function encodeBase64(buffer: Uint8Array): string {
  return base64.encode(buffer);
}

/**
 * Decodes a base64 string into a buffer
 *
 * @throws when the given string is not a valid base64 string
 */
export function decodeBase64(toEncode: string): Uint8Array {
  return base64.decode(toEncode);
}

/**
 * Encodes a buffer into an hexadecimal string
 *
 * @throws when buffer is not an `Uint8Array`
 */
export function encodeHexadecimal(buffer: Uint8Array): string {
  return hex.encode(buffer);
}

/**
 * Decodes an hexadecimal string into an Uint8Array
 *
 * @throws when the given string is not a valid hexadecimal string
 */
export function decodeHexadecimal(toDecode: string): Uint8Array {
  return hex.decode(toDecode);
}

/**
 * Encodes a buffer into a UTF-8 string
 *
 * @throws when buffer is not an `Uint8Array`
 */
export function encodeUtf8(buffer: Uint8Array): string {
  return utf8.decode(buffer);
}

/**
 * Decodes a UTF-8 string into an `Uint8Array`
 */
export function decodeUtf8(input: string): Uint8Array {
  return utf8.encode(input);
}

/**
 * Generates a key to be used with aesEncrypt and aesDecrypt
 */
export function aesGenerateKey(): Uint8Array {
  return randomBytes(32);
}

/**
 * Encrypts a message with AES-256-GCM using the given key
 *
 * @throws when message is not an `Uint8Array`
 * @throws when key is not an `Uint8Array`
 * @throws when key length is incorrect
 */
export async function aesEncrypt(
  message: Uint8Array,
  key: Uint8Array
): Promise<AesResult> {
  const iv = randomBytes(12);
  const aesKey = await window.crypto.subtle.importKey(
    'raw',
    key,
    'AES-GCM',
    true,
    ['encrypt']
  );

  const encryptedMessage = await window.crypto.subtle.encrypt(
    { name: 'AES-GCM', iv },
    aesKey,
    message
  );

  return {
    iv,
    encrypted: new Uint8Array(encryptedMessage),
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
export async function aesDecrypt(
  cipherText: Uint8Array,
  key: Uint8Array,
  iv: Uint8Array
): Promise<Uint8Array> {
  const aesKey = await window.crypto.subtle.importKey(
    'raw',
    key,
    'AES-GCM',
    true,
    ['encrypt']
  );

  const decryptedMessage = await window.crypto.subtle.decrypt(
    {
      name: 'AES-GCM',
      iv,
    },
    aesKey,
    cipherText
  );

  return new Uint8Array(decryptedMessage);
}

/**
 * Generates a key pair to be used with boxSeal and boxOpen
 */
export function boxGenerateKey(): KeyPair {
  return box.keyPair();
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
  const nonceBuffer = randomBytes(box.nonceLength);
  const cipherTextBuffer = box(
    message,
    nonceBuffer,
    recipientPublicKey,
    senderSecretKey
  );

  const encrypted = new Uint8Array(
    nonceBuffer.length + cipherTextBuffer.length
  );
  encrypted.set(nonceBuffer);
  encrypted.set(cipherTextBuffer);

  return encrypted;
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
  const nonceBuffer = cipherText.slice(0, box.nonceLength);

  const decrypted = box.open(
    cipherText.slice(box.nonceLength),
    nonceBuffer,
    senderPublicKey,
    recipientSecretKey
  );

  if (decrypted === null) {
    throw new Error('[react-native-nacl-jsi] boxOpen verification failed');
  }

  return decrypted;
}

/**
 * Generates a key to be used with secretboxSeal and secretboxOpen
 */
export function secretboxGenerateKey(): Uint8Array {
  return randomBytes(secretbox.keyLength);
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
  const nonceBuffer = randomBytes(secretbox.nonceLength);
  const cipherTextBuffer = secretbox(message, nonceBuffer, secretKey);

  const encrypted = new Uint8Array(
    nonceBuffer.length + cipherTextBuffer.length
  );
  encrypted.set(nonceBuffer);
  encrypted.set(cipherTextBuffer);

  return encrypted;
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
  const decrypted = secretbox.open(
    cipherText.slice(secretbox.nonceLength),
    cipherText.slice(0, secretbox.nonceLength),
    secretKey
  );

  if (decrypted === null) {
    throw new Error(
      '[react-native-nacl-jsi] secretboxOpen verification failed'
    );
  }

  return decrypted;
}

/**
 * Generates a key pair for signing and verifying signatures
 */
export function signGenerateKey(): KeyPair {
  return sign.keyPair();
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
  const keyPair = sign.keyPair.fromSecretKey(secretKey);
  return sign.detached(message, keyPair.publicKey);
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
  return sign.detached.verify(message, signature, publicKey);
}

/**
 * Returns a buffer of the given size filled with random bytes
 *
 * @throws When the given size is incorrect
 */
export function getRandomBytes(size: number | BigInt): Uint8Array {
  return randomBytes(typeof size === 'number' ? size : Number(size));
}
