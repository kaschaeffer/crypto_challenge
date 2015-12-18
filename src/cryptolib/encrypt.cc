#include "cryptolib/encrypt.h"

#include <vector>

namespace cryptolib {

std::vector<unsigned char>* FixedXOR(const std::vector<unsigned char>& plaintext,
                                     const std::vector<unsigned char>& key) {
  std::vector<unsigned char>* output = new std::vector<unsigned char>();
  for (int i = 0; i < plaintext.size(); i++) {
    output->push_back(plaintext[i] ^ key[i]);
  }
  return output;
}

}  // namespace cryptolib
