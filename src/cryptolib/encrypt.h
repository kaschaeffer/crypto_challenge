#ifndef CRYPTOLIB_ENCRYPT_H_
#define CRYPTOLIB_ENCRYPT_H_

#include <vector>

namespace cryptolib {

std::vector<unsigned char>* FixedXOR(const std::vector<unsigned char>& plaintext,
                                     const std::vector<unsigned char>& key);

std::vector<unsigned char>* RepeatingKeyXOREncrypt(const std::vector<unsigned char>& plaintext,
                                                   const std::vector<unsigned char>& key);

}  // namespace cryptolib

#endif  // CRYPTOLIB_ENCRYPT_H_
