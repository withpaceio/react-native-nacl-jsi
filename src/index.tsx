import { NativeModules } from 'react-native';

const g = global as any;
const NaClModule = NativeModules.NaclJsi;
console.log(NaClModule);

if (NaClModule && typeof NaClModule.install === 'function') {
  NaClModule.install();
}

export function helloJsi(): string {
  return g.helloJsi();
}
