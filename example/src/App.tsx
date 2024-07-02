import React from 'react';
import { SafeAreaView, ScrollView, StyleSheet, Text, View } from 'react-native';
import {
  boxGenerateKey,
  boxOpen,
  boxSeal,
  decodeUtf8,
  encodeBase64,
  encodeUtf8,
  secretboxGenerateKey,
  secretboxOpen,
  secretboxSeal,
  signDetached,
  signGenerateKey,
  signVerifyDetached,
} from 'react-native-nacl-jsi';

export default function App() {
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
