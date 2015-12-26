#ifndef CRYPTOLIB_ANALYSIS_H_
#define CRYPTOLIB_ANALYSIS_H_

#include <string>
#include <vector>

namespace cryptolib {

enum AESMode {
  ECB = 0,
  CBC = 1,
};

// TODO should this be string or vec
double GetBlockEntropy(std::string ciphertext, int blocksize);

bool HasRepeatedBlock(std::vector<unsigned char>& ciphertext, int blocksize);

AESMode GetAESBlockMode(const std::vector<unsigned char>& ciphertext);

}


#endif  // CRYPTOLIB_ANALYSIS_H_
