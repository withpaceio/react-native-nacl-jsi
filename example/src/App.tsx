import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
  boxGenerateKey,
} from 'react-native-nacl-jsi';

export default function App() {
  const secretKey = secretboxGenerateKey();
  const encryptedMessage = secretboxSeal('hello pace!', secretKey);
  const decryptedMessage = secretboxOpen(encryptedMessage, secretKey);

  const keyPair = boxGenerateKey();
  console.debug(keyPair);

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
      <Text>Key pair:</Text>
      <Text>Public: {keyPair.publicKey}</Text>
      <Text>Secret: {keyPair.secretKey}</Text>
      <Text>---</Text>
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
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
