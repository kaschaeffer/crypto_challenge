#include <fstream>

#include "stdlib.h"

#include "gtest/gtest.h"

#include "cryptolib/analysis.h"
#include "cryptolib/decrypt.h"
#include "cryptolib/encoding.h"
#include "cryptolib/encrypt.h"

TEST(Set2, Challenge10_CBCMode) {
  std::ifstream ciphertext_file("../resources/ciphertext10.txt");
  std::string ciphertext_base64;
  std::string ciphertext_line;
  while (std::getline(ciphertext_file, ciphertext_line)) {
    ciphertext_base64 += ciphertext_line;
  }
  
  const std::vector<unsigned char> ciphertext = cryptolib::Base64ToBytes(ciphertext_base64);
  std::string string_key = "YELLOW SUBMARINE";
  std::vector<unsigned char>* key = new std::vector<unsigned char>(string_key.begin(), string_key.end());
  std::vector<unsigned char> iv;
  for (int i = 0; i < 16; i++) {
    iv.push_back('\x00');
  }

  std::vector<unsigned char>* plaintext_bytes = cryptolib::DecryptAES128CBC(ciphertext, *key, iv);
  std::string actual_plaintext = std::string(plaintext_bytes->begin(), plaintext_bytes->end());
  std::string expected_plaintext =
    "I'm back and I'm ringin' the bell \n"
    "A rockin' on the mike while the fly girls yell \n"
    "In ecstasy in the back of me \n"
    "Well that's my DJ Deshay cuttin' all them Z's \n"
    "Hittin' hard and the girlies goin' crazy \n"
    "Vanilla's on the mike, man I'm not lazy. \n"
    "\n"
    "I'm lettin' my drug kick in \n"
    "It controls my mouth and I begin \n"
    "To just let it flow, let my concepts go \n"
    "My posse's to the side yellin', Go Vanilla Go! \n"
    "\n"
    "Smooth 'cause that's the way I will be \n"
    "And if you don't give a damn, then \n"
    "Why you starin' at me \n"
    "So get off 'cause I control the stage \n"
    "There's no dissin' allowed \n"
    "I'm in my own phase \n"
    "The girlies sa y they love me and that is ok \n"
    "And I can dance better than any kid n' play \n"
    "\n"
    "Stage 2 -- Yea the one ya' wanna listen to \n"
    "It's off my head so let the beat play through \n"
    "So I can funk it up and make it sound good \n"
    "1-2-3 Yo -- Knock on some wood \n"
    "For good luck, I like my rhymes atrocious \n"
    "Supercalafragilisticexpialidocious \n"
    "I'm an effect and that you can bet \n"
    "I can take a fly girl and make her wet. \n"
    "\n"
    "I'm like Samson -- Samson to Delilah \n"
    "There's no denyin', You can try to hang \n"
    "But you'll keep tryin' to get my style \n"
    "Over and over, practice makes perfect \n"
    "But not if you're a loafer. \n"
    "\n"
    "You'll get nowhere, no place, no time, no girls \n"
    "Soon -- Oh my God, homebody, you probably eat \n"
    "Spaghetti with a spoon! Come on and say it! \n"
    "\n"
    "VIP. Vanilla Ice yep, yep, I'm comin' hard like a rhino \n"
    "Intoxicating so you stagger like a wino \n"
    "So punks stop trying and girl stop cryin' \n"
    "Vanilla Ice is sellin' and you people are buyin' \n"
    "'Cause why the freaks are jockin' like Crazy Glue \n"
    "Movin' and groovin' trying to sing along \n"
    "All through the ghetto groovin' this here song \n"
    "Now you're amazed by the VIP posse. \n"
    "\n"
    "Steppin' so hard like a German Nazi \n"
    "Startled by the bases hittin' ground \n"
    "There's no trippin' on mine, I'm just gettin' down \n"
    "Sparkamatic, I'm hangin' tight like a fanatic \n"
    "You trapped me once and I thought that \n"
    "You might have it \n"
    "So step down and lend me your ear \n"
    "'89 in my time! You, '90 is my year. \n"
    "\n"
    "You're weakenin' fast, YO! and I can tell it \n"
    "Your body's gettin' hot, so, so I can smell it \n"
    "So don't be mad and don't be sad \n"
    "'Cause the lyrics belong to ICE, You can call me Dad \n"
    "You're pitchin' a fit, so step back and endure \n"
    "Let the witch doctor, Ice, do the dance to cure \n"
    "So come up close and don't be square \n"
    "You wanna battle me -- Anytime, anywhere \n"
    "\n"
    "You thought that I was weak, Boy, you're dead wrong \n"
    "So come on, everybody and sing this song \n"
    "\n"
    "Say -- Play that funky music Say, go white boy, go white boy go \n"
    "play that funky music Go white boy, go white boy, go \n"
    "Lay down and boogie and play that funky music till you die. \n"
    "\n"
    "Play that funky music Come on, Come on, let me hear \n"
    "Play that funky music white boy you say it, say it \n"
    "Play that funky music A little louder now \n"
    "Play that funky music, white boy Come on, Come on, Come on \n"
    "Play that funky music \n"
    "\x4\x4\x4\x4";

  EXPECT_EQ(expected_plaintext, actual_plaintext);
}

TEST(Set2, Challenge11_EncryptionModeOracle) {
  std::vector<unsigned char>* ciphertext;
  std::string string_plaintext = "abcdefghijklmno abcdefghijklmno abcdefghijklmno";
  std::vector<unsigned char> plaintext = std::vector<unsigned char>(string_plaintext.begin(), string_plaintext.end());
  const int NUM_TESTS = 1000;
  int number_incorrect = 0;
  srand(87915); // set seed, so test is reproducible

  for (int i = 0; i < NUM_TESTS; i++) {
    std::vector<unsigned char>* key = cryptolib::GetRandom128Key();
    int prefix_length = (rand() % 5) + 5;
    std::vector<unsigned char> random_prefix;
    for (int i = 0; i < prefix_length; i++) {
      random_prefix.push_back(rand() % 256);
    }

    int postfix_length = (rand() % 5) + 5;
    std::vector<unsigned char> random_postfix;
    for (int i = 0; i < postfix_length; i++) {
      random_postfix.push_back(rand() % 256);
    }

    plaintext.insert(plaintext.begin(), random_prefix.begin(), random_prefix.end());
    plaintext.insert(plaintext.end(), random_postfix.begin(), random_postfix.end());

    cryptolib::AESMode mode = static_cast<cryptolib::AESMode>(rand() % 2);
    switch (mode) {
      case cryptolib::ECB: {
        ciphertext = cryptolib::EncryptAES128ECB(plaintext, *key);
        break;
      }
      case cryptolib::CBC: {
        std::vector<unsigned char>* iv = cryptolib::GetRandom128Key();
        ciphertext = cryptolib::EncryptAES128CBC(plaintext, *key, *iv);
        break;
      }
      default: {
        assert(false);
      }
    }
    if (mode != cryptolib::GetAESBlockMode(*ciphertext)) {
      number_incorrect++;
    }
  }
  EXPECT_EQ(0, number_incorrect);
}

TEST(Set2, Challenge12_BasicECBDecryption) {
  std::string base64_plaintext = 
    "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
    "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
    "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
    "YnkK";
  std::vector<unsigned char> plaintext = cryptolib::Base64ToBytes(base64_plaintext);

  // determine block size

}
