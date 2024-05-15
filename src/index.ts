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

export function aesGenerateKey(): string {
  return g.aesGenerateKey();
}

export function aesEncrypt(message: string, key: string): AesResult | null {
  return g.aesEncrypt(message, key);
}

export function aesDecrypt(
  encryptedMessage: string,
  key: string,
  iv: string
): string | null {
  return g.aesDecrypt(encryptedMessage, key, iv);
}

export function boxGenerateKey(): KeyPair {
  return g.boxGenerateKey();
}

export function boxSeal(
  message: string,
  recipientPublicKey: string,
  senderSecretKey: string
): string {
  return g.boxSeal(message, recipientPublicKey, senderSecretKey);
}

export function boxOpen(
  encryptedMessage: string,
  senderPublicKey: string,
  recipientSecretKey: string
): string {
  return g.boxOpen(encryptedMessage, senderPublicKey, recipientSecretKey);
}

export function secretboxGenerateKey(): string {
  return g.secretboxGenerateKey();
}

export function secretboxSeal(message: string, secretKey: string): string {
  return g.secretboxSeal(message, secretKey);
}

export function secretboxOpen(cipherText: string, secretKey: string): string {
  return g.secretboxOpen(cipherText, secretKey);
}

export function signGenerateKey(): KeyPair {
  return g.signGenerateKey();
}

export function signDetached(
  message: string,
  secretKey: string
): string | null {
  return g.signDetached(message, secretKey);
}

export function signVerifyDetached(
  message: string,
  publicKey: string,
  signature: string
): boolean {
  return g.signVerifyDetached(message, publicKey, signature);
}

export function argon2idHash(
  password: string,
  iterations: BigInt,
  memoryLimit: BigInt
): string {
  return g.argon2idHash(password, iterations, memoryLimit);
}

export function argon2idVerify(hash: string, password: string): boolean {
  return g.argon2idVerify(hash, password);
}

export function argon2idDeriveKey(
  password: string,
  salt: string,
  keyLength: number,
  iterations: BigInt,
  memoryLimit: BigInt
): string {
  return g.argon2idDeriveKey(
    password,
    salt,
    keyLength,
    iterations,
    memoryLimit
  );
}

export function getRandomBytes(
  size: number | BigInt,
  encoding: 'base64' | 'hex' = 'base64'
): string {
  return g.getRandomBytes(size, encoding);
}
