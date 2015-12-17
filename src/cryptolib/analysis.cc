#include "cryptolib/analysis.h"

#include <math.h>

#include <map>

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

}  // namespace cryptolib
