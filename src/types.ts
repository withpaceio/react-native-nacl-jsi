export type AesResult = {
  encrypted: Uint8Array;
  iv: Uint8Array;
};

export type KeyPair = {
  publicKey: Uint8Array;
  secretKey: Uint8Array;
};
