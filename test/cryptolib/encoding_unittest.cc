#include <map>
#include "stdlib.h"

#include "gtest/gtest.h"
#include "cryptolib/analysis.h"
#include "cryptolib/decrypt.h"
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

TEST(EncryptTest, PadBlock) {
  std::string plaintext = "YELLOW SUBMARINE";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());

  std::vector<unsigned char>* actual_padded_plaintext = cryptolib::PadBlock(plaintext_bytes, 20);
  std::string expected_padded_plaintext = "YELLOW SUBMARINE\x04\x04\x04\x04";
  EXPECT_EQ(expected_padded_plaintext,
            std::string(actual_padded_plaintext->begin(), actual_padded_plaintext->end()));
}

TEST(EncryptTest, PadBlockZeroPadding) {
  std::string plaintext = "YELLOW SUBMARINE";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());

  std::vector<unsigned char>* actual_padded_plaintext = cryptolib::PadBlock(plaintext_bytes, 16);
  std::string expected_padded_plaintext = plaintext;
  EXPECT_EQ(expected_padded_plaintext,
            std::string(actual_padded_plaintext->begin(), actual_padded_plaintext->end()));
}

TEST(EncryptTest, PadBlockBigPadding) {
  std::string plaintext = "ab";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());

  std::string expected_padded_plaintext = "ab";
  for (int i = 0; i < 98; i++) {
    expected_padded_plaintext += 98;
  }
  std::vector<unsigned char>* actual_padded_plaintext = cryptolib::PadBlock(plaintext_bytes, 100);
  EXPECT_EQ(expected_padded_plaintext,
            std::string(actual_padded_plaintext->begin(), actual_padded_plaintext->end()));
}

TEST(EncryptTest, PadText) {
  std::string plaintext = "YELLOW SUBMARINE";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());

  std::vector<unsigned char>* actual_padded_plaintext = cryptolib::PadText(plaintext_bytes, 10);
  std::string expected_padded_plaintext = "YELLOW SUBMARINE\x04\x04\x04\x04";
  EXPECT_EQ(expected_padded_plaintext,
            std::string(actual_padded_plaintext->begin(), actual_padded_plaintext->end()));
}

TEST(EncryptTest, PadTextMultiple) {
  std::string plaintext = "YELLOW SUBMARINE";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());

  std::vector<unsigned char>* actual_padded_plaintext = cryptolib::PadText(plaintext_bytes, 8);
  std::string expected_padded_plaintext = "YELLOW SUBMARINE\x08\x08\x08\x08\x08\x08\x08\x08";
  EXPECT_EQ(expected_padded_plaintext,
            std::string(actual_padded_plaintext->begin(), actual_padded_plaintext->end()));
}

TEST(EncryptTest, AES128CBCEncryptDecrypt) {
  std::string plaintext =
    "This is a test.  Encrypting and then decrypting "
    "this message using the AES-128 block cipher in CBC "
    "mode should give the original message.";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());
  std::string key = "foobar QUUXBAZ--";
  std::string iv = "*Initial Vector*";

  std::vector<unsigned char>* ciphertext = cryptolib::EncryptAES128CBC(std::vector<unsigned char>(plaintext.begin(), plaintext.end()),
                                                                       std::vector<unsigned char>(key.begin(), key.end()),
                                                                       std::vector<unsigned char>(iv.begin(), iv.end()));
  std::vector<unsigned char>* actual_plaintext = cryptolib::DecryptAES128CBC(std::vector<unsigned char>(ciphertext->begin(), ciphertext->end()),
                                                                             std::vector<unsigned char>(key.begin(), key.end()),
                                                                             std::vector<unsigned char>(iv.begin(), iv.end()));
  std::vector<unsigned char>* expected_plaintext = cryptolib::PadText(plaintext_bytes, 16);
  EXPECT_EQ(std::string(expected_plaintext->begin(), expected_plaintext->end()),
            std::string(actual_plaintext->begin(), actual_plaintext->end()));
}

TEST(EncryptTest, AES128ECBEncryptDecrypt) {
  std::string plaintext =
    "This is a test.  Encrypting and then decrypting "
    "this message using the AES-128 block cipher in ECB "
    "mode should give the original message.";
  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(), plaintext.end());
  std::string key = "foobar QUUXBAZ--";

  std::vector<unsigned char>* ciphertext = cryptolib::EncryptAES128ECB(std::vector<unsigned char>(plaintext.begin(), plaintext.end()),
                                                                       std::vector<unsigned char>(key.begin(), key.end()));
  std::vector<unsigned char>* actual_plaintext = cryptolib::DecryptAES128ECB(std::vector<unsigned char>(ciphertext->begin(), ciphertext->end()),
                                                                             std::vector<unsigned char>(key.begin(), key.end()));
  std::vector<unsigned char>* expected_plaintext = cryptolib::PadText(plaintext_bytes, 16);
  EXPECT_EQ(std::string(expected_plaintext->begin(), expected_plaintext->end()),
            std::string(actual_plaintext->begin(), actual_plaintext->end()));
}

TEST(AnalysisTest, HasRepeatedBlock) {
  std::string plaintext =
    "abcdefghijklmno abcdefghijklmno Test to see whether HasRepeatedBlock can properly "
    "identify the existence of a repated block of chars, "
    "such as ABRACADABRAMAGIC and ABRACADABRAMAGIC.";

  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(),
                                                                          plaintext.end());
  /* std::vector<unsigned char> first_block = std::vector<unsigned char>(plaintext_bytes.begin() + 16, */
  /*                                                                     plaintext_bytes.begin() + 32); */
  /* plaintext_bytes.insert(plaintext_bytes.begin() + 128, first_block.begin(), first_block.end()); */

  EXPECT_EQ(true, cryptolib::HasRepeatedBlock(plaintext_bytes, 16));
}

TEST(AnalysisTest, HasNoRepeatedBlock) {
  std::string plaintext =
    "Test to see whether HasRepeatedBlock works properly "
    "identify the existence of a repated block of chars, "
    "such as ABRACADABRAMAGIC and ABRACADABRAMAGIC.";

  std::vector<unsigned char> plaintext_bytes = std::vector<unsigned char>(plaintext.begin(),
                                                                          plaintext.end());

  EXPECT_EQ(false, cryptolib::HasRepeatedBlock(plaintext_bytes, 16));
}

TEST(AnalysisTest, ValidatePadding) {
  std::string text = "ICE ICE BABY\x04\x04\x04\x04";
  std::vector<unsigned char> text_bytes = std::vector<unsigned char>(text.begin(),
                                                                     text.end());
  EXPECT_EQ(true, cryptolib::ValidatePadding(text_bytes));
}

TEST(AnalysisTest, ValidatePaddingInvalid) {
  std::string text = "ICE ICE BABY\x09\x04\x04\x04";
  std::vector<unsigned char> text_bytes = std::vector<unsigned char>(text.begin(),
                                                                     text.end());
  EXPECT_EQ(false, cryptolib::ValidatePadding(text_bytes));
}

TEST(AnalysisTest, ValidatePaddingInvalid2) {
  std::string text = "ICE ICE BABY\x09\x09\x09\x09\x09\x09";
  std::vector<unsigned char> text_bytes = std::vector<unsigned char>(text.begin(),
                                                                     text.end());
  EXPECT_EQ(false, cryptolib::ValidatePadding(text_bytes));
}

TEST(AnalysisTest, ParseKeyValueString) {
  std::string text = "foo=bar&baz=qux&zap=zazzle";
  std::map<std::string, std::string> expected = {{"foo", "bar"},
                                                 {"baz", "qux"},
                                                 {"zap", "zazzle"}};

  EXPECT_EQ(expected, *cryptolib::ParseKeyValueString(text));
}

TEST(AnalysisTest, ParseKeyValueStringEmpty) {
  std::string text = "";
  std::map<std::string, std::string> expected = {};

  EXPECT_EQ(expected, *cryptolib::ParseKeyValueString(text));
}

TEST(AnalysisTest, ParseKeyValueStringOnePair) {
  std::string text = "quux=fuzz";
  std::map<std::string, std::string> expected = {{"quux", "fuzz"}};

  EXPECT_EQ(expected, *cryptolib::ParseKeyValueString(text));
}
