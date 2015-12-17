#ifndef CRYPTOLIB_ANALYSIS_H_
#define CRYPTOLIB_ANALYSIS_H_

#include <string>

namespace cryptolib {

// TODO should this be string or vec
double GetBlockEntropy(std::string ciphertext, int blocksize);

}


#endif  // CRYPTOLIB_ANALYSIS_H_
