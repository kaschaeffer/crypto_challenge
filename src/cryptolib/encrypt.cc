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

std::vector<unsigned char>* PadBlock(const std::vector<unsigned char>& plaintext,
                                     int blocksize) {
  // TODO add checking
  int n = blocksize - plaintext.size();
  std::vector<unsigned char>* padded_plaintext = new std::vector<unsigned char>(plaintext);

  for (int i = 0; i < n; i++) {
    padded_plaintext->push_back(n);
  }
  return padded_plaintext;
}

}  // namespace cryptolib
