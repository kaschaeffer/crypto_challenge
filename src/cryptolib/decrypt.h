#ifndef CRYPTOLIB_DECRYPT_H_
#define CRYPTOLIB_DECRYPT_H_

#include <vector>

namespace cryptolib {
  std::vector<unsigned char>* DecryptAES128ECB(const std::vector<unsigned char>& ciphertext, 
                                               const std::vector<unsigned char>& key);


}  // namespace cryptolib

#endif  // CRYPTOLIB_DECRYPT_H_
