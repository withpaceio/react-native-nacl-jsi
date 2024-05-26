export type AesResult = {
  encrypted: string;
  iv: string;
};

export type KeyPair = {
  publicKey: Uint8Array;
  secretKey: Uint8Array;
};
