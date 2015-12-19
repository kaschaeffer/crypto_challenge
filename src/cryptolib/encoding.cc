#include "cryptolib/encoding.h"

// TODO should this still be private?
namespace {

char intToHex(int i) {
  /* assume i >=0 && i < 16 */
  if (i >= 0 && i <= 9) {
    return '0' + i;
  }
  return 'a' + (i - 10);
}

unsigned char hexToInt(char hex) {
  if (hex >= '0' && hex <= '9') {
    return hex - '0';
  }
  if (hex >= 'A' && hex <= 'F') {
    return hex - 'A' + 10;
  }
  return hex - 'a' + 10;
}

char bitsToBase64(unsigned char c) {
  /* input must be in the range [0, 63)
   * i.e. half a byte */

  // TODO raise exception if not true?

  if (c >= 0 && c < 26) {
    return 'A' + c;
  }
  
  if (c >= 26 && c < 52) {
    return 'a' + (c - 26);
  }
  
  if (c >= 52 && c < 62) {
    return '0' + (c - 52);
  }
  
  if (c == 62) {
    return '+';
  }

  /* c == 63 */
  return '/';
}

unsigned char base64ToBits(char c) {
  unsigned char out = 0;
  if (c >= 'A' && c <= 'Z') {
    out = c - 'A';
  } else if (c >= 'a' && c <= 'z') {
    out = c - 'a' + 26;
  } else if (c >= '0' && c <= '9') {
    out = c - '0' + 52;
  } else if (c == '+') {
    out = 62;
  } else if (c == '/') {
    out = 63;
  }
  return out;
}

}  // namespace


namespace cryptolib {

std::string BytesToHex(const std::vector<unsigned char>& bytes) {
  std::string hex;
  for (auto byte : bytes) {
    hex += intToHex(byte >> 4);
    hex += intToHex(byte & 0x0f);
  }
  return hex;
}

std::vector<unsigned char>* HexToBytes(const std::string& hex) {
  std::vector<unsigned char>* bytes = new std::vector<unsigned char>();
  int i = 0;
  while (hex[i]) {
    if (hex[i+1]) {
      bytes->push_back((hexToInt(hex[i]) << 4) + hexToInt(hex[i+1]));
      i += 2;
    } else {
      // pad byte with zeros
      bytes->push_back(hexToInt(hex[i]) << 4);
      i += 1;
    }
  }

  return bytes;
}

std::string BytesToBase64(const std::vector<unsigned char>& raw) {
  std::string base64String;
  int length = raw.size();
  int extraBytes = length % 3;

  int i = 0;
  while (i + 3 <= length) {
    base64String += bitsToBase64((raw[i] >> 2));
    base64String += bitsToBase64(((raw[i] & 0x03) << 4) + (raw[i+1] >> 4));
    base64String += bitsToBase64(((raw[i+1] & 0x0f) << 2) + (raw[i+2] >> 6));
    base64String += bitsToBase64(raw[i+2] & 0x3f);
    i += 3;
  }
  if (extraBytes == 1) {
    base64String += bitsToBase64((raw[i] >> 2));
    base64String += bitsToBase64((raw[i] & 0x03) << 4);
    base64String += "==";
  }
  if (extraBytes == 2) {
    base64String += bitsToBase64((raw[i] >> 2));
    base64String += bitsToBase64(((raw[i] & 0x03) << 4) + (raw[i+1] >> 4));
    base64String += bitsToBase64((raw[i+1] & 0x0f) << 2);
    base64String += "=";
  }
  return base64String;
}

std::vector<unsigned char> Base64ToBytes(const std::string& base64) {
  auto bytes = new std::vector<unsigned char>();
  unsigned char newByte1 = 0;
  unsigned char newByte2 = 0;
  unsigned char newByte3 = 0;
  unsigned char newByte4 = 0;

  int i = 0;
  while (i + 4 <= base64.size()) {
    if (base64[i+2] == '=' && base64[i+3] == '=') {
      newByte1 = base64ToBits(base64[i++]);
      newByte2 = base64ToBits(base64[i++]);
      bytes->push_back((newByte1 << 2) + (newByte2 >> 4));
    } else if (base64[i+3] == '=') {
      newByte1 = base64ToBits(base64[i++]);
      newByte2 = base64ToBits(base64[i++]);
      newByte3 = base64ToBits(base64[i++]);
      bytes->push_back((newByte1 << 2) + (newByte2 >> 4));
      bytes->push_back(((newByte2 & 0x0f) << 4) + (newByte3 >> 2));
    } else {
      newByte1 = base64ToBits(base64[i++]);
      newByte2 = base64ToBits(base64[i++]);
      newByte3 = base64ToBits(base64[i++]);
      newByte4 = base64ToBits(base64[i++]);

      bytes->push_back((newByte1 << 2) + (newByte2 >> 4));
      bytes->push_back(((newByte2 & 0x0f) << 4) + (newByte3 >> 2));
      bytes->push_back(((newByte3 & 0x03) << 6) + newByte4);
    }
  }

  return *bytes;
}

}  // namespace cryptolib
