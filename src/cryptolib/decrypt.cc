#include "cryptolib/decrypt.h"

#include <iterator>
#include <vector>

#include <openssl/aes.h>

namespace cryptolib {

std::vector<unsigned char>* DecryptAES128ECB(const std::vector<unsigned char>& ciphertext, 
                                               const std::vector<unsigned char>& key) {
  // TODO (verify key size before proceding)

  std::vector<unsigned char>* plaintext = new std::vector<unsigned char>();
  unsigned char plaintext_block[16];
  /* = new unsigned char(16); */
  /* unsigned char[16] plaintext_block = new unsigned char(16); */

  int size = ciphertext.size();
  AES_KEY* aes_key = new AES_KEY();
  AES_set_decrypt_key(&key[0], 128, aes_key);

  for (int i = 0; i < size; i += 16) {
    AES_decrypt(&ciphertext[i], plaintext_block, aes_key);
    plaintext->insert(std::end(*plaintext), std::begin(plaintext_block), std::end(plaintext_block));
  }
  
  return plaintext;
}



}  // namespace cryptolib
