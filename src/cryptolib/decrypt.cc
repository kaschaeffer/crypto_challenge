#include "cryptolib/decrypt.h"

#include <iterator>
#include <vector>

#include <openssl/aes.h>

#include "cryptolib/encrypt.h"

namespace cryptolib {

std::vector<unsigned char>* DecryptAES128ECB(const std::vector<unsigned char>& ciphertext, 
                                               const std::vector<unsigned char>& key) {
  // TODO (verify key size before proceding)

  std::vector<unsigned char>* plaintext = new std::vector<unsigned char>();
  unsigned char plaintext_block[16];

  int size = ciphertext.size();
  AES_KEY* aes_key = new AES_KEY();
  AES_set_decrypt_key(&key[0], 128, aes_key);

  for (int i = 0; i < size; i += 16) {
    AES_decrypt(&ciphertext[i], plaintext_block, aes_key);
    plaintext->insert(std::end(*plaintext), std::begin(plaintext_block), std::end(plaintext_block));
  }
  
  return plaintext;
}

std::vector<unsigned char>* DecryptAES128CBC(const std::vector<unsigned char>& ciphertext, 
                                             const std::vector<unsigned char>& key,
                                             const std::vector<unsigned char>& iv) {
  const int BLOCKSIZE = 16;
  unsigned char previous_ciphertext_block[BLOCKSIZE];
  unsigned char plaintext_block[BLOCKSIZE];
  std::vector<unsigned char>* plaintext_block_vector;
  std::vector<unsigned char>* final_plaintext = new std::vector<unsigned char>();

  AES_KEY* aes_key = new AES_KEY();
  AES_set_decrypt_key(&key[0], 128, aes_key);
  std::copy(std::begin(iv), std::end(iv), std::begin(previous_ciphertext_block));

  for (int i = 0; i < ciphertext.size(); i += BLOCKSIZE) {
    AES_decrypt(&ciphertext[i], plaintext_block, aes_key);
    plaintext_block_vector = cryptolib::FixedXOR(std::vector<unsigned char>(std::begin(plaintext_block),
                                                                            std::end(plaintext_block)),
                                                 std::vector<unsigned char>(std::begin(previous_ciphertext_block),
                                                                            std::end(previous_ciphertext_block)));
    std::copy(std::begin(*plaintext_block_vector),
              std::end(*plaintext_block_vector),
              std::begin(plaintext_block));
    std::copy(ciphertext.begin() + i, ciphertext.begin() + i + 16, std::begin(previous_ciphertext_block));
    final_plaintext->insert(std::end(*final_plaintext),
                            std::begin(plaintext_block),
                            std::end(plaintext_block));
  }
  return final_plaintext;
}



}  // namespace cryptolib
