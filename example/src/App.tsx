import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
  boxGenerateKey,
  boxSeal,
  boxOpen,
} from 'react-native-nacl-jsi';

export default function App() {
  const secretKey = secretboxGenerateKey();
  const encryptedMessage = secretboxSeal('hello!', secretKey);
  const decryptedMessage = secretboxOpen(encryptedMessage, secretKey);

  const recipientKeyPair = boxGenerateKey();
  const senderKeyPair = boxGenerateKey();
  const boxEncryptedMessage = boxSeal(
    'hello with box encryption!',
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
      <Text>SECRET BOX</Text>
      <Text>---</Text>
      <Text>secret key: {secretKey}</Text>
      <Text>---</Text>
      <Text>nonce + cipher text: {encryptedMessage}</Text>
      <Text>---</Text>
      <Text>clear text: {decryptedMessage}</Text>
      <Text>---</Text>
      <Text>BOX</Text>
      <Text>---</Text>
      <Text>nonce + cipher text: {boxEncryptedMessage}</Text>
      <Text>---</Text>
      <Text>clear text: {boxDecryptedMessage}</Text>
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
});
