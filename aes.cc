#include <fstream>
#include <iostream>
#include <vector>

#include <openssl/aes.h>

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

std::vector<unsigned char> base64ToBytes(const std::string& base64) {
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

/* CHALLENGE 7 */
int main() {
  const unsigned char key[] = "YELLOW SUBMARINE";

  std::ifstream ciphertextFile("./ciphertext7b.txt");
  std::string ciphertextBase64;
  std::string ciphertextLine;
  while (std::getline(ciphertextFile, ciphertextLine)) {
    ciphertextBase64.append(ciphertextLine);
  }
  ciphertextFile.close();

  std::cout << ciphertextBase64.size();
  std::cout << "ciphertext: " << std::endl
    << ciphertextBase64 << std::endl;

  std::vector<unsigned char> ciphertextVector = base64ToBytes(ciphertextBase64);
  int size = ciphertextVector.size();
  
  unsigned char* plaintextBlock = new unsigned char(16);
  std::string plaintext;
  AES_KEY* aesKey = new AES_KEY();
  AES_set_decrypt_key(key, 128, aesKey);
  for (int i = 0; i < size; i += 16) {
    AES_decrypt(&ciphertextVector[i], plaintextBlock, aesKey);
    /* TODO this cast is ugly! */
    plaintext.append((char *)(plaintextBlock), 16);
  }

  std::cout << "plaintext: " << std::endl
    << plaintext << std::endl;

}
