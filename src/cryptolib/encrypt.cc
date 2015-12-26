#include "cryptolib/encrypt.h"

#include "stdlib.h"

#include <iterator>
#include <vector>

#include <openssl/aes.h>

namespace cryptolib {

std::vector<unsigned char>* FixedXOR(const std::vector<unsigned char>& plaintext,
                                     const std::vector<unsigned char>& key) {
  std::vector<unsigned char>* output = new std::vector<unsigned char>();
  for (int i = 0; i < plaintext.size(); i++) {
    output->push_back(plaintext[i] ^ key[i]);
  }
  return output;
}

std::vector<unsigned char>* PadBlock(const std::vector<unsigned char>& plaintext,
                                     int blocksize) {
  // TODO add checking
  int n = blocksize - plaintext.size();
  std::vector<unsigned char>* padded_plaintext = new std::vector<unsigned char>(plaintext);

  for (int i = 0; i < n; i++) {
    padded_plaintext->push_back(n);
  }
  return padded_plaintext;
}

std::vector<unsigned char>* PadText(const std::vector<unsigned char>& plaintext,
                                     int blocksize) {
  int n = blocksize - (plaintext.size() % blocksize);
  std::vector<unsigned char>* padded_plaintext = new std::vector<unsigned char>(plaintext);

  for (int i = 0; i < n; i++) {
    padded_plaintext->push_back(n);
  }
  return padded_plaintext;
}

// TODO refactor this to use the existing Encrypt/DecryptAES128ECB
std::vector<unsigned char>* EncryptAES128CBC(const std::vector<unsigned char>& plaintext, 
                                             const std::vector<unsigned char>& key,
                                             const std::vector<unsigned char>& iv) {
  const int BLOCKSIZE = 16;
  unsigned char previous_ciphertext_block[BLOCKSIZE];
  std::vector<unsigned char>* final_ciphertext = new std::vector<unsigned char>();

  AES_KEY* aes_key = new AES_KEY();
  AES_set_encrypt_key(&key[0], 128, aes_key);
  std::vector<unsigned char>* padded_plaintext = PadText(plaintext, BLOCKSIZE);

  std::copy(std::begin(iv), std::end(iv), std::begin(previous_ciphertext_block));


  for (int i = 0; i < padded_plaintext->size(); i += BLOCKSIZE) {
    auto xored_plaintext = FixedXOR(std::vector<unsigned char>(padded_plaintext->begin() + i,
                                                               padded_plaintext->begin() + i + 16),
                                    std::vector<unsigned char>(std::begin(previous_ciphertext_block),
                                                               std::end(previous_ciphertext_block)));
    AES_encrypt(&((*xored_plaintext)[0]), previous_ciphertext_block, aes_key);
    final_ciphertext->insert(std::end(*final_ciphertext),
                             std::begin(previous_ciphertext_block),
                             std::end(previous_ciphertext_block));
  }
  return final_ciphertext;
}

std::vector<unsigned char>* EncryptAES128ECB(const std::vector<unsigned char>& plaintext, 
                                             const std::vector<unsigned char>& key) {
  const int BLOCKSIZE = 16;
  unsigned char ciphertext_block[BLOCKSIZE];
  std::vector<unsigned char>* final_ciphertext = new std::vector<unsigned char>();

  AES_KEY* aes_key = new AES_KEY();
  AES_set_encrypt_key(&key[0], 128, aes_key);
  std::vector<unsigned char>* padded_plaintext = PadText(plaintext, BLOCKSIZE);


  for (int i = 0; i < padded_plaintext->size(); i += BLOCKSIZE) {
    AES_encrypt(&((*padded_plaintext)[i]), ciphertext_block, aes_key);
    final_ciphertext->insert(std::end(*final_ciphertext),
                             std::begin(ciphertext_block),
                             std::end(ciphertext_block));
  }
  return final_ciphertext;
}

std::vector<unsigned char>* GetRandom128Key() {
  std::vector<unsigned char>* key = new std::vector<unsigned char>();
  for (int i = 0; i < 16; i++) {
    key->push_back(rand() % 256);
  }
  return key;  
}       

}  // namespace cryptolib
