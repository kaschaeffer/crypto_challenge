#ifndef CRYPTOLIB_ENCRYPT_H_
#define CRYPTOLIB_ENCRYPT_H_

#include <vector>

namespace cryptolib {

std::vector<unsigned char>* FixedXOR(const std::vector<unsigned char>& plaintext,
                                     const std::vector<unsigned char>& key);

std::vector<unsigned char>* RepeatingKeyXOREncrypt(const std::vector<unsigned char>& plaintext,
                                                   const std::vector<unsigned char>& key);

// Pads the plaintext to make it have length = blocksize, using the PKCS #7 standard
// (http://tools.ietf.org/html/rfc5652#section-6.3).
// The length of plaintext must be less than or equal to blocksize, and the number of padded
// bytes to be added must be less than 256.
//
// Note: in the standard, input that is a multiple of the blocksize will add an
// additional block containing all `blocksize` values.  However, PadBlock does
// *not* add additional padding if the size of plaintext equals blocksize - it
// always returns precisely one block of size blocksize.
std::vector<unsigned char>* PadBlock(const std::vector<unsigned char>& plaintext,
                                     int blocksize);

// Pads the plaintext to make it have length equal to a multiple of blocksize,
// using the PKCS #7 standard
// (http://tools.ietf.org/html/rfc5652#section-6.3).
std::vector<unsigned char>* PadText(const std::vector<unsigned char>& plaintext,
                                     int blocksize);

std::vector<unsigned char>* EncryptAES128CBC(const std::vector<unsigned char>& plaintext, 
                                             const std::vector<unsigned char>& key,
                                             const std::vector<unsigned char>& iv);

std::vector<unsigned char>* EncryptAES128ECB(const std::vector<unsigned char>& plaintext, 
                                             const std::vector<unsigned char>& key);

// Generates a random 128-bit (16-byte) key.
std::vector<unsigned char>* GetRandom128Key();

class Cipher {
 public:
  Cipher();
  virtual ~Cipher();

  virtual std::vector<unsigned char>* encrypt(const std::vector<unsigned char>& plaintext) const = 0;
  virtual std::vector<unsigned char>* decrypt(const std::vector<unsigned char>& ciphertext) const = 0;
};

class AES128ECBCipher : public Cipher {
 public:
  explicit AES128ECBCipher(const std::vector<unsigned char>& key);
  ~AES128ECBCipher();

  std::vector<unsigned char>* encrypt(const std::vector<unsigned char>& plaintext) const override;
  std::vector<unsigned char>* decrypt(const std::vector<unsigned char>& ciphertext) const override;

 private:
  const std::vector<unsigned char> key;
};


}  // namespace cryptolib

#endif  // CRYPTOLIB_ENCRYPT_H_
