import { NativeModules } from 'react-native';

import type { AesResult, KeyPair } from './types';

const g = global as any;
const NaClModule = NativeModules.NaclJsi;

if (NaClModule && typeof NaClModule.install === 'function') {
  NaClModule.install();
}

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
