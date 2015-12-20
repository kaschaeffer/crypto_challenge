#include <fstream>
#include <iostream>

#include "gtest/gtest.h"

#include "cryptolib/encoding.h"
#include "cryptolib/encrypt.h"


TEST(Set1, Challenge1_HexToBase64) {
  std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  std::string expected_base64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  EXPECT_EQ(expected_base64, cryptolib::BytesToBase64(*cryptolib::HexToBytes(hex)));
}

TEST(Set1, Challenge2_FixedXOR) {
  std::string hex1 = "1c0111001f010100061a024b53535009181c";
  std::string hex2 = "686974207468652062756c6c277320657965";
  std::string expected = "746865206b696420646f6e277420706c6179";
  std::string actual = cryptolib::BytesToHex(*cryptolib::FixedXOR(*cryptolib::HexToBytes(hex1),
                                                                  *cryptolib::HexToBytes(hex2)));
  EXPECT_EQ(expected, actual);
}

TEST(Set1, Challenge3_SingleByteXORCipher) {
  std::string hex_ciphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  // TODO
  std::string expected_plaintext = "TODO";
  std::string actual_plaintext = "ALSO_TODO";
  EXPECT_EQ(expected_plaintext, actual_plaintext);
}

TEST(Set1, Challenge4_DetectSingleByteXORCipher) {
  std::ifstream ciphertexts_file("../resources/ciphertext4.txt");
  // TODO
  ciphertexts_file.close();

  int expected_line_number = 0;  // TODO
  int actual_line_number = 1;  // TODO

  std::string expected_plaintext = "TODO";
  std::string actual_plaintext = "TODO_ALSO";

  EXPECT_EQ(expected_line_number, actual_line_number);
  EXPECT_EQ(expected_plaintext, actual_plaintext);
}

TEST(Set1, Challenge5_RepeatingKeyXOR) {
  std::string plaintext =
    "Burning 'em, if you ain't quick and nimble\n"
    "I go crazy when I hear a cymbal";
  std::string key = "ICE";
  std::string expected_ciphertext = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
  std::string actual_ciphertext = "TODO";
  EXPECT_EQ(expected_ciphertext, actual_ciphertext);
}

TEST(Set1, Challenge6_BreakRepeatingKeyXOR) {
  std::ifstream ciphertext_file("../resources/ciphertext6.txt");
  // TODO
  ciphertext_file.close();

  std::string expected_plaintext = "TODO";
  std::string actual_plaintext = "TODO_ALSO";

  EXPECT_EQ(expected_plaintext, actual_plaintext);
}

TEST(Set1, Challenge7_DecryptAESECB) {
  std::string key = "YELLOW SUBMARINE";
  std::ifstream ciphertext_file("../resources/ciphertext7.txt");
  // TODO
  ciphertext_file.close();

  std::string expected_plaintext = "TODO";
  std::string actual_plaintext = "TODO_ALSO";

  EXPECT_EQ(expected_plaintext, actual_plaintext);
}

int main(int argc, char **argv) {
  std::cout << "============================" << std::endl;
  std::cout << "Checking solutions for SET 1" << std::endl;
  std::cout << "============================" << std::endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
