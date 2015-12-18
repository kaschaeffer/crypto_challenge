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

int main(int argc, char **argv) {
  std::cout << "============================" << std::endl;
  std::cout << "Checking solutions for SET 1" << std::endl;
  std::cout << "============================" << std::endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
