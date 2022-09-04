import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import { secretboxGenerateKey, secretboxSeal } from 'react-native-nacl-jsi';

export default function App() {
  const secretKey = secretboxGenerateKey();
  const encryptedMessage = secretboxSeal('hello', secretKey);

  return (
    <View style={styles.container}>
      <Text>{secretKey}</Text>
      <Text>{encryptedMessage}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
