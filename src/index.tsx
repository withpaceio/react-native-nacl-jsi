import { NativeModules } from 'react-native';

const g = global as any;
const NaClModule = NativeModules.NaclJsi;

if (NaClModule && typeof NaClModule.install === 'function') {
  NaClModule.install();
}

export function secretboxGenerateKey(): string {
  return g.secretboxGenerateKey();
}

export function secretboxSeal(message: string, secretKey: string): string {
  return g.secretboxSeal(message, secretKey);
}
