#include "stdlib.h"

#include "gtest/gtest.h"
#include "cryptolib/encoding.h"
#include "cryptolib/encrypt.h"

TEST(EncodingTest, BytesToHex) {
  std::vector<unsigned char> bytes = {0x87, 0xfa, 0x12, 0xad, 0x2e, 0x38, 0x90, 0x0c};
  std::string hex = cryptolib::BytesToHex(bytes);
  EXPECT_EQ("87fa12ad2e38900c", hex);
}

TEST(EncodingTest, HexToBytes) {
  std::vector<unsigned char>* bytes = cryptolib::HexToBytes("87fa12ad2e38900c");
  std::vector<unsigned char> expected_bytes = {0x87, 0xfa, 0x12, 0xad, 0x2e, 0x38, 0x90, 0x0c};
  EXPECT_EQ(expected_bytes, *bytes);
}

unsigned char get_random_byte() {
  return (rand() % 256);
}

TEST(EncodingTest, BytesToHexToBytes) {
  srand(10);  // set seed, so test is reproducible
  const int MAX_LENGTH = 100;
  for (int i = 0; i < 5000; i++) {
    int length = (rand() % MAX_LENGTH);
    std::vector<unsigned char> bytes(length);
    for (int j = 0; j < length; j++) {
      bytes[j] = get_random_byte();
    }
    EXPECT_EQ(bytes, *cryptolib::HexToBytes(cryptolib::BytesToHex(bytes)));
  }
}

std::string get_random_hex_byte() {
  std::string hex_byte;
  int rand_hex;
  for (int i = 0; i < 2; i++) {
    rand_hex = rand() % 16;
    if (rand_hex < 10) {
      hex_byte += ('0' + rand_hex);
    } else {
      hex_byte += ('a' + rand_hex - 10);
    }
  }
  return hex_byte;
}

TEST(EncodingTest, HexToBytesToHex) {
  srand(20);  // set seed, so test is reproducible
  const int MAX_LENGTH = 100;
  for (int i = 0; i < 5000; i++) {
    int length = (rand() % MAX_LENGTH);
    std::string hex;
    for (int j = 0; j < length; j++) {
      hex += get_random_hex_byte();
    }
    EXPECT_EQ(hex, cryptolib::BytesToHex(*cryptolib::HexToBytes(hex)));
  }
}

TEST(EncodingTest, BytesToBase64) {
  std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  const std::vector<unsigned char>* bytes = cryptolib::HexToBytes(hex);

  std::string base64 = cryptolib::BytesToBase64(*bytes);
  std::string expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  EXPECT_EQ(expected, base64);
}

TEST(EncodingTest, Base64ToBytes) {
  std::string base64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  const std::vector<unsigned char> bytes = cryptolib::Base64ToBytes(base64);

  std::string actual = cryptolib::BytesToHex(bytes);
  std::string expected = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  EXPECT_EQ(expected, actual);
}

TEST(EncodingTest, BytesToBase64SinglePadding) {
  const std::vector<unsigned char> bytes = {'f', 'o', 'o', 'b', 'a'};
  std::string actual = cryptolib::BytesToBase64(bytes);
  std::string expected = "Zm9vYmE=";
  EXPECT_EQ(expected, actual);
}

TEST(EncodingTest, BytesToBase64DoublePadding) {
  const std::vector<unsigned char> bytes = {'f', 'o', 'o', 'b'};
  std::string actual = cryptolib::BytesToBase64(bytes);
  std::string expected = "Zm9vYg==";
  EXPECT_EQ(expected, actual);
}

TEST(EncodingTest, Base64ToBytesSinglePadding) {
  std::string base64 = "Zm9vYmE=";
  const std::vector<unsigned char> expected = {'f', 'o', 'o', 'b', 'a'};
  std::vector<unsigned char> actual = cryptolib::Base64ToBytes(base64);
  EXPECT_EQ(expected, actual);
}

TEST(EncodingTest, Base64ToBytesDoublePadding) {
  std::string base64 = "Zm9vYg==";
  const std::vector<unsigned char> expected = {'f', 'o', 'o', 'b'};
  std::vector<unsigned char> actual = cryptolib::Base64ToBytes(base64);
  EXPECT_EQ(expected, actual);
}

TEST(EncodingTest, BytesToBase64ToBytes) {
  srand(100);  // set seed, so test is reproducible
  const int MAX_LENGTH = 100;
  for (int i = 0; i < 5000; i++) {
    int length = (rand() % MAX_LENGTH);
    std::vector<unsigned char> bytes(length);
    for (int j = 0; j < length; j++) {
      bytes[j] = get_random_byte();
    }
    EXPECT_EQ(bytes, cryptolib::Base64ToBytes(cryptolib::BytesToBase64(bytes)));
  }
}

TEST(EncryptTest, FixedXOR) {
  std::string plaintext = "1c0111001f010100061a024b53535009";
  std::string key = "686974207468652062756c6c27732065";
  std::string expected_ciphertext = "746865206b696420646f6e277420706c";
  std::string actual_ciphertext = cryptolib::BytesToHex(*cryptolib::FixedXOR(*cryptolib::HexToBytes(plaintext),
                                                                             *cryptolib::HexToBytes(key)));
  EXPECT_EQ(expected_ciphertext, actual_ciphertext);
}

TEST(EncryptTest, FixedXOREmptyString) {
  std::string plaintext = "";
  std::string key = "";
  std::string expected_ciphertext = "";
  std::string actual_ciphertext = cryptolib::BytesToHex(*cryptolib::FixedXOR(*cryptolib::HexToBytes(plaintext),
                                                                             *cryptolib::HexToBytes(key)));
  EXPECT_EQ(expected_ciphertext, actual_ciphertext);
}
