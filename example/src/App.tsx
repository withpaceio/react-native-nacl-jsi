import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import {
  secretboxGenerateKey,
  secretboxSeal,
  secretboxOpen,
} from 'react-native-nacl-jsi';

export default function App() {
  const secretKey = secretboxGenerateKey();
  const encryptedMessage = secretboxSeal('hello!', secretKey);
  const decryptedMessage = secretboxOpen(encryptedMessage, secretKey);
  console.debug(decryptedMessage);

  return (
    <View style={styles.container}>
      <Text>secret key: {secretKey}</Text>
      <Text>---</Text>
      <Text>cipher text: {encryptedMessage}</Text>
      <Text>---</Text>
      <Text>clear text: {decryptedMessage}</Text>
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
