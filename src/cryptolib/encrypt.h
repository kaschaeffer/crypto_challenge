#ifndef CRYPTOLIB_ENCRYPT_H_
#define CRYPTOLIB_ENCRYPT_H_

#include <vector>

namespace cryptolib {

std::vector<unsigned char>* RepeatingKeyXOREncrypt(const std::vector<unsigned char>& plaintext,
                                                   const std::vector<unsigned char>& key);

}

#endif  // CRYPTOLIB_ENCRYPT_H_
