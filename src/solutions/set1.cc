#include <iostream>

#include "gtest/gtest.h"

#include "cryptolib/encoding.h"


TEST(Set1, Challenge1HexToBase64) {
  std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  std::string expected_base64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  EXPECT_EQ(expected_base64, cryptolib::BytesToBase64(*cryptolib::HexToBytes(hex)));
}

int main(int argc, char **argv) {
  std::cout << "============================" << std::endl;
  std::cout << "Checking solutions for SET 1" << std::endl;
  std::cout << "============================" << std::endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
