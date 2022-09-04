import React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import { helloJsi } from 'react-native-nacl-jsi';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>{helloJsi()}</Text>
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
