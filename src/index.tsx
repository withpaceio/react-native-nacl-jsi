import { NativeModules } from 'react-native';

import type { AesResult, KeyPair } from './types';

const g = global as any;
const NaClModule = NativeModules.NaclJsi;

if (NaClModule && typeof NaClModule.install === 'function') {
  NaClModule.install();
}

const constants = g.getConstants();

export const ARGON2ID_MEMLIMIT_MIN = constants.ARGON2ID_MEMLIMIT_MIN;
export const ARGON2ID_MEMLIMIT_MAX = constants.ARGON2ID_MEMLIMIT_MAX;
export const ARGON2ID_MEMLIMIT_INTERACTIVE =
  constants.ARGON2ID_MEMLIMIT_INTERACTIVE;
export const ARGON2ID_MEMLIMIT_MODERATE = constants.ARGON2ID_MEMLIMIT_MODERATE;
export const ARGON2ID_MEMLIMIT_SENSITIVE =
  constants.ARGON2ID_MEMLIMIT_SENSITIVE;

export const ARGON2ID_OPSLIMIT_MIN = constants.ARGON2ID_OPSLIMIT_MIN;
export const ARGON2ID_OPSLIMIT_MAX = constants.ARGON2ID_OPSLIMIT_MAX;
export const ARGON2ID_OPSLIMIT_INTERACTIVE =
  constants.ARGON2ID_OPSLIMIT_INTERACTIVE;
export const ARGON2ID_OPSLIMIT_MODERATE = constants.ARGON2ID_OPSLIMIT_MODERATE;
export const ARGON2ID_OPSLIMIT_SENSITIVE =
  constants.ARGON2ID_OPSLIMIT_SENSITIVE;

export function aesGenerateKey(): string {
  return g.aesGenerateKey();
}

export function aesEncrypt(message: string, key: string): AesResult {
  return g.aesEncrypt(message, key);
}

export function aesDecrypt(
  encryptedMessage: string,
  key: string,
  iv: string
): string {
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

export function argon2idHash(
  password: string,
  iterations: number,
  memoryLimit: number
): string {
  return g.argon2idHash(password, iterations, memoryLimit);
}

export function argon2idVerify(hash: string, password: string): boolean {
  return g.argon2idVerify(hash, password);
}

export function getRandomBytes(size: number): string {
  return g.getRandomBytes(size);
}
