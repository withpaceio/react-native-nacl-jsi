import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
  boxGenerateKey,
  boxSeal,
  boxOpen,
  aesGenerateKey,
  aesEncrypt,
  aesDecrypt,
} from 'react-native-nacl-jsi';

export default function App() {
  const aesKey = aesGenerateKey();
  const { encrypted: aesEncryptedMessage, iv } = aesEncrypt(
    'hello aes!',
    aesKey
  );
  const aesDecryptedMessage = aesDecrypt(aesEncryptedMessage, aesKey, iv);

  const secretKey = secretboxGenerateKey();
  const encryptedMessage = secretboxSeal('hello with secretbox!', secretKey);
  const decryptedMessage = secretboxOpen(encryptedMessage, secretKey);

  const recipientKeyPair = boxGenerateKey();
  const senderKeyPair = boxGenerateKey();
  const boxEncryptedMessage = boxSeal(
    'hello with box!',
    recipientKeyPair.publicKey,
    senderKeyPair.secretKey
  );
  const boxDecryptedMessage = boxOpen(
    boxEncryptedMessage,
    senderKeyPair.publicKey,
    recipientKeyPair.secretKey
  );

  return (
    <View style={styles.container}>
      <View style={styles.algorithmContainer}>
        <Text style={styles.algorithmName}>AES256-GCM</Text>
        <Text>Key: {aesKey}</Text>
        <Text>---</Text>
        <Text>cipher text: {aesEncryptedMessage}</Text>
        <Text>---</Text>
        <Text>clear text: {aesDecryptedMessage}</Text>
      </View>
      <View style={styles.algorithmContainer}>
        <Text style={styles.algorithmName}>SECRET BOX</Text>
        <Text>secret key: {secretKey}</Text>
        <Text>---</Text>
        <Text>nonce + cipher text: {encryptedMessage}</Text>
        <Text>---</Text>
        <Text>clear text: {decryptedMessage}</Text>
      </View>
      <View style={styles.algorithmContainer}>
        <Text style={styles.algorithmName}>BOX</Text>
        <Text>nonce + cipher text: {boxEncryptedMessage}</Text>
        <Text>---</Text>
        <Text>clear text: {boxDecryptedMessage}</Text>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'flex-start',
    justifyContent: 'center',
    width: '100%',
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
