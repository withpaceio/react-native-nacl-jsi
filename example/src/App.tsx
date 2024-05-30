import React from 'react';

import { SafeAreaView, ScrollView, StyleSheet, Text, View } from 'react-native';
import {
  ARGON2ID_MEMLIMIT_INTERACTIVE,
  ARGON2ID_OPSLIMIT_SENSITIVE,
  ARGON2ID_SALTBYTES,
  aesDecrypt,
  aesEncrypt,
  aesGenerateKey,
  argon2idDeriveKey,
  argon2idHash,
  argon2idVerify,
  boxGenerateKey,
  boxOpen,
  boxSeal,
  decodeUtf8,
  encodeBase64,
  encodeUtf8,
  getRandomBytes,
  secretboxGenerateKey,
  secretboxOpen,
  secretboxSeal,
  signDetached,
  signGenerateKey,
  signVerifyDetached,
  type AesResult,
} from 'react-native-nacl-jsi';

export default function App() {
  const aesKey = aesGenerateKey();
  const aesResult = aesEncrypt(decodeUtf8('hello aes!'), aesKey) as AesResult;
  const aesDecryptedMessage = aesDecrypt(
    aesResult.encrypted,
    aesKey,
    aesResult.iv
  ) as Uint8Array;

  const secretKey = secretboxGenerateKey();
  const messageSecretbox = decodeUtf8('hello with secretbox!');
  const encryptedMessage = secretboxSeal(messageSecretbox, secretKey);
  const decryptedMessage = secretboxOpen(
    encryptedMessage as Uint8Array,
    secretKey
  );

  const recipientKeyPair = boxGenerateKey();
  const senderKeyPair = boxGenerateKey();

  const message = decodeUtf8('hello with box!');
  const boxEncryptedMessage = boxSeal(
    message,
    recipientKeyPair.publicKey,
    senderKeyPair.secretKey
  );
  const boxDecryptedMessage = boxOpen(
    boxEncryptedMessage as Uint8Array,
    senderKeyPair.publicKey,
    recipientKeyPair.secretKey
  );

  const password = 'secure_password1234)';
  const hashedPassword = argon2idHash(
    decodeUtf8(password),
    ARGON2ID_OPSLIMIT_SENSITIVE,
    ARGON2ID_MEMLIMIT_INTERACTIVE
  );
  const isVerified = argon2idVerify(
    hashedPassword as string,
    decodeUtf8(password)
  );

  const salt = getRandomBytes(ARGON2ID_SALTBYTES);
  const derivedKey = argon2idDeriveKey(
    decodeUtf8(password),
    salt,
    32,
    ARGON2ID_OPSLIMIT_SENSITIVE,
    ARGON2ID_MEMLIMIT_INTERACTIVE
  );

  const signKeyPair = signGenerateKey();
  const messageToSign = decodeUtf8('sign this message');
  const signature = signDetached(messageToSign, signKeyPair.secretKey);
  const isSignatureVerified = signVerifyDetached(
    messageToSign,
    signKeyPair.publicKey,
    signature as Uint8Array
  );

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>AES256-GCM</Text>
          <Text>Key: {encodeBase64(aesKey)}</Text>
          <Text>---</Text>
          <Text>cipher text: {encodeBase64(aesResult.encrypted)}</Text>
          <Text>---</Text>
          <Text>
            clear text: {encodeUtf8(aesDecryptedMessage as Uint8Array)}
          </Text>
        </View>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>SECRET BOX</Text>
          <Text>secret key: {encodeBase64(secretKey)}</Text>
          <Text>---</Text>
          <Text>
            nonce + cipher text: {encodeBase64(encryptedMessage as Uint8Array)}
          </Text>
          <Text>---</Text>
          <Text>clear text: {encodeUtf8(decryptedMessage as Uint8Array)}</Text>
        </View>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>BOX</Text>
          <Text>
            nonce + cipher text:{' '}
            {encodeBase64(boxEncryptedMessage as Uint8Array)}
          </Text>
          <Text>---</Text>
          <Text>
            clear text: {encodeUtf8(boxDecryptedMessage as Uint8Array)}
          </Text>
        </View>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>Argon2id</Text>
          <Text>Password: {password}</Text>
          <Text>---</Text>
          <Text>Hash: {hashedPassword}</Text>
          <Text>---</Text>
          <Text>Is verified: {isVerified ? 'true' : 'false'}</Text>
        </View>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>Argon2id key derivation</Text>
          <Text>Key: {password}</Text>
          <Text>Salt: {encodeBase64(salt)}</Text>
          <Text>---</Text>
          <Text>Derived key: {encodeBase64(derivedKey)}</Text>
        </View>
        <View style={styles.algorithmContainer}>
          <Text style={styles.algorithmName}>Signature</Text>
          <Text>Public key: {encodeBase64(signKeyPair.publicKey)}</Text>
          <Text>Secret key: {encodeBase64(signKeyPair.secretKey)}</Text>
          <Text>Signature: {encodeBase64(signature as Uint8Array)}</Text>
          <Text>Is verified: {isSignatureVerified ? 'true' : 'false'}</Text>
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    width: '100%',
    backgroundColor: '#fff',
  },
  algorithmContainer: {
    width: '100%',
    paddingBottom: 20,
    borderBottomColor: 'black',
    backgroundColor: '#eee',
    marginBottom: 10,
    padding: 10,
  },
  algorithmName: {
    alignSelf: 'center',
    fontWeight: 'bold',
    marginBottom: 10,
  },
});
