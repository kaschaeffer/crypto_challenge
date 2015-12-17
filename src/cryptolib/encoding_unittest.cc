#include "gtest/gtest.h"
#include "cryptolib/encoding.h"

TEST(EncodingTest, BytesToHex) {
  std::vector<unsigned char> bytes = {0x87, 0xfa, 0x12, 0xad, 0x2e, 0x38, 0x90, 0x0c};
  std::string hex = cryptolib::BytesToHex(bytes);
  EXPECT_EQ("87fa12ad2e38900c", hex);
}

TEST(EncodingTest, HexToBytes) {
  std::vector<unsigned char>* bytes = cryptolib::HexToBytes("87fa12ad2e38900c");
  std::vector<unsigned char> expected_bytes = {0x87, 0xfa, 0x12, 0xad, 0x2e, 0x38, 0x90, 0x0c};
  // TODO correct equality here?
  EXPECT_EQ(expected_bytes, *bytes);
}

// TODO add roundtrip tests for hex <--> bytes
// using random but with deterministic seed


TEST(EncodingTest, BytesToBase64) {
  std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  const std::vector<unsigned char>* bytes = cryptolib::HexToBytes(hex);

  std::string base64 = cryptolib::BytesToBase64(*bytes);
  std::string expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  EXPECT_EQ(expected, base64);
}

TEST(EncodingTest, BytesToBase64) {
  std::string base64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  const std::vector<unsigned char> bytes = cryptolib::Base64ToBytes(base64);

  std::string actual = cryptolib::BytesToHex(bytes);
  std::string expected = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  EXPECT_EQ(expected, actual);
}

// TODO(kschaeffer): add tests for '=' and '==' cases for base64
