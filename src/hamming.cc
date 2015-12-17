#include <iostream>
#include <limits>
#include <vector>

int oneBits(unsigned char byte) {
  int number = 0;
  for (int i=0; i < 8; i++) {
    number += (0x01 & (byte >> i));
  }
  return number;
}


int hammingByte(unsigned char a, unsigned char b) {
  return oneBits(a ^ b);
}

int hamming(const std::vector<unsigned char>& a,
            const std::vector<unsigned char>& b) {
  // TODO generalize to handle differing lengths
  int distance = 0;
  for (int i=0; i < a.size(); i++) {
    distance += hammingByte(a[i], b[i]); 
  }
  return distance;
}

int findKeySize(const std::vector<unsigned char>& ciphertext) {
  const int N_TRIALS = 4;
  double bestAvgDistance = std::numeric_limits<double>::max();
  int bestKeysize;

  for (int keysize = 2; keysize < 50; keysize++) {
    auto a = std::vector<unsigned char>(ciphertext.begin(), ciphertext.begin() + keysize);
    double avgDistance = 0.0;
    for (int i = 1; i <= N_TRIALS; i++) {
      auto b = std::vector<unsigned char>(ciphertext.begin() + (keysize * i),
                                          ciphertext.begin() + (keysize * (i + 1)));
      avgDistance += hamming(a, b);
    }
    avgDistance /= (N_TRIALS * keysize);

    if (avgDistance < bestAvgDistance) {
      bestAvgDistance = avgDistance;
      bestKeysize = keysize;
    }
  }
  return bestKeysize;
}

int main() {
  std::cout << "0x00: " << oneBits(0x00) << " expected: 0" << std::endl;
  std::cout << "0x03: " << oneBits(0x03) << " expected: 2" << std::endl;
  std::cout << "0x05: " << oneBits(0x05) << " expected: 2" << std::endl;
  std::cout << "0xff: " << oneBits(0xff) << " expected: 8" << std::endl;

  std::cout << "Hamming distance between 0x0f and 0x4e: " << hammingByte(0xff, 0x4e)
            << " expected: 2" << std::endl;

  std::cout << "Hamming distance between 't' and 'tw': " << hammingByte('t', 'w')
            << " expected: 2" << std::endl;

  std::string testInput1 = "this is a test";
  std::string testInput2 = "wokka wokka!!!";
  int distance = hamming(std::vector<unsigned char>(testInput1.begin(), testInput1.end()),
                         std::vector<unsigned char>(testInput2.begin(), testInput2.end()));
  std::cout << "distance between \"this is a test\" and \"wokka wokka!!!\" is: "
            << distance << " expected: 37" << std::endl;

}
