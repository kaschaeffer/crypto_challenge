#ifndef CRYPTOLIB_ENCODING_H_
#define CRYPTOLIB_ENCODING_H_

#include <string>
#include <vector>

namespace cryptolib {

std::string BytesToHex(const std::vector<unsigned char>& bytes);
std::vector<unsigned char>* HexToBytes(const std::string& hex);

std::string BytesToBase64(const std::vector<unsigned char>& bytes);
// TODO should this pass pointer back?
std::vector<unsigned char> Base64ToBytes(const std::string& base64);

}  // namespace cryptolib

#endif  // CRYPTOLIB_ENCODING_H_
