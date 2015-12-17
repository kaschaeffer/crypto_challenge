#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <math.h>

std::map<std::string, double> getBlockFrequencies(std::string ciphertext) {
  std::map<std::string, double> frequencies;
  int size = ciphertext.size();
  for (int i = 0; i < size; i += 16) {
    frequencies[ciphertext.substr(i, 16)] += (1.0/size);
  }
  return frequencies;
}

double entropy(std::map<std::string, double> frequencies) {
  double e = 0;
  double f;
  for (auto const &entry : frequencies) {
    f = entry.second;
    e += (-1 * f * log(f));
  }
  return e;
}

/* CHALLENGE 8 */
int main() {
  /* looks for lowest entropy (in terms of block frequencies) */
  std::ifstream ciphertextFile("./ciphertext8.txt");
  std::string ciphertextLine;
  std::vector<std::string> ciphertextsHex;
  std::vector<std::pair<std::string, double> > ciphertextWithEntropy;
  double e;
  int i = 0;
  int best_i;
  double best_e = std::numeric_limits<double>::max();

  while (std::getline(ciphertextFile, ciphertextLine)) {
    e = entropy(getBlockFrequencies(ciphertextLine)); 
    std::cout << "  entropy of line " << i << " ==> " << e << std::endl;
    if (e < best_e) {
      best_i = i;
      best_e = e;
    }
    i++;
  }
  std::cout << "Line that is most likely to be ECB encrypted: " << best_i << std::endl;
  return 0;
}
