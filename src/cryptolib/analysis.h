#ifndef CRYPTOLIB_ANALYSIS_H_
#define CRYPTOLIB_ANALYSIS_H_

#include <string>
#include <vector>

#include "encrypt.h"

namespace cryptolib {

enum AESMode {
  ECB = 0,
  CBC = 1,
};

// TODO should this be string or vec
double GetBlockEntropy(std::string ciphertext, int blocksize);

int GetBlockSize(const cryptolib::Cipher& cipher);
bool HasRepeatedBlock(const std::vector<unsigned char>& ciphertext, int blocksize);

AESMode GetAESBlockMode(const std::vector<unsigned char>& ciphertext);

}


#endif  // CRYPTOLIB_ANALYSIS_H_
