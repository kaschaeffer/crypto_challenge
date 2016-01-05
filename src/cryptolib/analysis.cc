#include "cryptolib/analysis.h"

#include <math.h>

#include <algorithm>
#include <map>

#include "cryptolib/encrypt.h"

namespace {

std::map<std::string, double> getBlockFrequencies(std::string ciphertext,
                                                  int blocksize) {
  std::map<std::string, double> frequencies;
  int size = ciphertext.size();
  for (int i = 0; i < size; i += blocksize) {
    frequencies[ciphertext.substr(i, blocksize)] += (1.0/size);
  }
  return frequencies;
}

// TODO this should be generic
double entropy(std::map<std::string, double> frequencies) {
  double e = 0;
  double f;
  for (auto const &entry : frequencies) {
    f = entry.second;
    e += (-1 * f * log(f));
  }
  return e;
}

}  // namespace


namespace cryptolib {

double GetBlockEntropy(std::string ciphertext, int blocksize) {
  auto frequencies = getBlockFrequencies(ciphertext, blocksize);
  double e = entropy(frequencies);
  return e;
}

// TODO DRY this up (w/ above frequencies implementation)
bool HasRepeatedBlock(const std::vector<unsigned char>& ciphertext, int blocksize) {
  // TODO handle partial blocks properly!
  std::map<std::vector<unsigned char>, int> frequencies;
  int size = ciphertext.size();
  for (int i = 0; i < size; i += blocksize) {
    auto begin = ciphertext.begin() + i;
    auto end = ciphertext.begin() + std::min(i + blocksize, size);

    frequencies[std::vector<unsigned char>(begin, end)]++;
  }
  bool has_repeated_block = false;
  for (auto key_and_count : frequencies) {
    if (key_and_count.second > 1) {
      return true;
    }
  }
  return has_repeated_block;


}

AESMode GetAESBlockMode(const std::vector<unsigned char>& ciphertext) {
  const int BLOCKSIZE = 16;
  if (HasRepeatedBlock(ciphertext, BLOCKSIZE)) {
    return ECB;
  } else {
    return CBC;
  }
}

int GetBlockSize(const cryptolib::Cipher& cipher) {
  const int MAX_SIZE = 10000;
  std::vector<unsigned char>* ciphertext;
  std::vector<unsigned char> plaintext = {'A'};
  int initial_ciphertext_size = cipher.encrypt(plaintext)->size();

  for (int plaintext_size = 1; plaintext_size < MAX_SIZE; plaintext_size++) {
    plaintext.push_back('A');
    ciphertext = cipher.encrypt(plaintext);
    if (ciphertext->size() != initial_ciphertext_size) {
      return ciphertext->size() - initial_ciphertext_size;
    }
  }
  // TODO add error handling
  return 0;
}

}  // namespace cryptolib
