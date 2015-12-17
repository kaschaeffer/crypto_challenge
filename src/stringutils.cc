#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <tuple>
#include <vector>

const std::map<std::string, double> ENGLISH_UNIGRAM_FREQUENCIES = {{"a", 0.08167},
                                                                   {"b", 0.01492},
                                                                   {"c", 0.02782},
                                                                   {"d", 0.04253},
                                                                   {"e", 0.12702},
                                                                   {"f", 0.02228},
                                                                   {"g", 0.02015},
                                                                   {"h", 0.06094},
                                                                   {"i", 0.06966},
                                                                   {"j", 0.00153},
                                                                   {"k", 0.00772},
                                                                   {"l", 0.04025},
                                                                   {"m", 0.02406},
                                                                   {"n", 0.06749},
                                                                   {"o", 0.07507},
                                                                   {"p", 0.01929},
                                                                   {"q", 0.00095},
                                                                   {"r", 0.05987},
                                                                   {"s", 0.06327},
                                                                   {"t", 0.09056},
                                                                   {"u", 0.02758},
                                                                   {"v", 0.00978},
                                                                   {"w", 0.02361},
                                                                   {"x", 0.00150},
                                                                   {"y", 0.01974},
                                                                   {"z", 0.00074}};

const std::map<std::string, double> ENGLISH_BIGRAM_FREQUENCIES = {{"th", 0.0356},
                                                                  {"he", 0.0307},
                                                                  {"in", 0.0243},
                                                                  {"er", 0.0205},
                                                                  {"an", 0.0199},
                                                                  {"re", 0.0185},
                                                                  {"on", 0.0176},
                                                                  {"at", 0.0149},
                                                                  {"en", 0.0145},
                                                                  {"nd", 0.0135},
                                                                  {"ti", 0.0134},
                                                                  {"es", 0.0134},
                                                                  {"or", 0.0128},
                                                                  {"te", 0.0120},
                                                                  {"of", 0.0117},
                                                                  {"ed", 0.0117},
                                                                  {"is", 0.0113},
                                                                  {"it", 0.0112},
                                                                  {"al", 0.0109},
                                                                  {"ar", 0.0107},
                                                                  {"st", 0.0105},
                                                                  {"to", 0.0104},
                                                                  {"nt", 0.0104},
                                                                  {"ng", 0.0095},
                                                                  {"se", 0.0093},
                                                                  {"ha", 0.0093},
                                                                  {"as", 0.0087},
                                                                  {"ou", 0.0087},
                                                                  {"io", 0.0083},
                                                                  {"le", 0.0083},
                                                                  {"ve", 0.0083},
                                                                  {"co", 0.0079},
                                                                  {"me", 0.0079},
                                                                  {"de", 0.0076},
                                                                  {"hi", 0.0076},
                                                                  {"ri", 0.0073},
                                                                  {"ro", 0.0073},
                                                                  {"ic", 0.0070},
                                                                  {"ne", 0.0069},
                                                                  {"ea", 0.0069},
                                                                  {"ra", 0.0069},
                                                                  {"ce", 0.0065},
                                                                  {"li", 0.0062},
                                                                  {"ch", 0.0060},
                                                                  {"ll", 0.0058},
                                                                  {"be", 0.0058},
                                                                  {"ma", 0.0057},
                                                                  {"si", 0.0055},
                                                                  {"om", 0.0055},
                                                                  {"ur", 0.0054}};



std::string bytesToHex(const std::vector<unsigned char>& bytes) {
  std::string hex;
  for (auto byte : bytes) {
    hex += intToHex(byte >> 4);
    hex += intToHex(byte & 0x0f);
  }
  return hex;
}

/* actually should this just be a std::string ? */
std::vector<unsigned char>* hexToBytes(const std::string& hex) {
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


std::string bytesToBase64(const std::vector<unsigned char>& raw) {
  std::string base64String;
  int length = raw.size();
  int extraBytes = length % 3;

  int i = 0;
  // for now just chop end of string
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


/* TODO is this the right way to go? maybe just pass back reference? */
std::vector<unsigned char>* fixedXor(const std::vector<unsigned char>& input1,
                                     const std::vector<unsigned char>& input2) {
  std::vector<unsigned char>* output = new std::vector<unsigned char>();
  for (int i = 0; i < input1.size(); i++) {
    output->push_back(input1[i] ^ input2[i]);
  }
  return output;
}

double chiSquared(std::map<std::string, double> expected, std::map<std::string, double> observed, int N) {
  double chiSquaredValue = 0;
  for (auto const &e : expected) {
    chiSquaredValue += ((observed[e.first] - e.second) * (observed[e.first] - e.second) / e.second);
  }    
  chiSquaredValue *= N;
  return chiSquaredValue;
}

/* double squaredError(std::map<std::string, double> expected, std::map<std::string, double> observed, int N) { */
double squaredError(std::map<std::string, double> expected, std::map<std::string, double> observed, int N) {
  double chiSquaredValue = 0;
  for (auto const &e : observed) {
    chiSquaredValue += ((expected[e.first] - e.second) * (expected[e.first] - e.second));
  }    
  return chiSquaredValue;
}

double scorePlaintext(std::vector<char> plaintext) {
  std::map<std::string, double> unigram_frequencies;
  std::map<std::string, double> bigram_frequencies;
  std::string unigram;
  std::string bigram;
  double score;

  for (int i = 0; i < plaintext.size(); i++) {
    unigram = std::string(plaintext.begin() + i, plaintext.begin() + i + 1);
    bigram = std::string(plaintext.begin() + i, plaintext.begin() + i + 2);
    unigram_frequencies[unigram] += (1.0 / plaintext.size());
    bigram_frequencies[bigram] += (1.0 / plaintext.size());
  }
  score = (0.3 * squaredError(ENGLISH_UNIGRAM_FREQUENCIES, unigram_frequencies, plaintext.size())) +
          squaredError(ENGLISH_BIGRAM_FREQUENCIES, bigram_frequencies, plaintext.size());
  return score;
}

bool keysAndScoresComparator(std::pair<char, double> a, std::pair<char, double> b) {
  return (a.second < b.second);
}

/*
 * returns (score, key, plaintext)
 */
std::tuple<double, char, std::string> getPlaintext(const std::vector<unsigned char>& rawBytes) {
  std::vector<std::pair<char, double> > keysAndScores;
  char key;
  double score;
  int numBytes = rawBytes.size();
  std::vector<char> xoredBytes(numBytes);
  xoredBytes[numBytes] = 0;
  std::string plaintext;

  for (int i=0; i < 256; i++) {
    key = i;
    // xor the key
    for (int j = 0; j < numBytes; j++) {
      xoredBytes[j] = rawBytes[j] ^ key;
    }
    /* plaintext = std::string(xoredBytes.begin(), xoredBytes.end()); */
    /* std::cout << "length: " << plaintext.size() << std::endl; */

    std::transform(xoredBytes.begin(), xoredBytes.end(), xoredBytes.begin(), ::tolower);

    /* std::cout << xoredBytes << std::endl; */

    // problem is with zero-terminated strings
    // i.e. when xoring, can accidentally get /000 somewhere!
    score = scorePlaintext(xoredBytes);
    keysAndScores.push_back(std::pair<char, double>(key, score));
  }

  sort(keysAndScores.begin(), keysAndScores.end(), keysAndScoresComparator);
  /* for (int i=0; i < 10; i++) { */
  /*   key = keysAndScores[i].first; */
  /*   // xor the key */
  /*   for (int j=0; j < numBytes; j++) { */
  /*     xoredBytes[j] = rawBytes[j] ^ key; */
  /*   } */
  /*   plaintext = std::string(xoredBytes.begin(), xoredBytes.end()); */
  /*   std::cout << "_____________" << std::endl */
  /*             << "key        = " << keysAndScores[i].first << std::endl */
  /*             << "score      = " << keysAndScores[i].second << std::endl */
  /*             << "plaintext  = " << plaintext << std::endl */
  /*             << std::endl; */
  /* } */
  key = keysAndScores[0].first;
  score = keysAndScores[0].second;
  // xor the key
  for (int j=0; j < numBytes; j++) {
    xoredBytes[j] = rawBytes[j] ^ key;
  }
  plaintext = std::string(xoredBytes.begin(), xoredBytes.end());
  return std::make_tuple(score, key, plaintext);
}

bool isPrintable(std::string input) {
  for (char c : input) {
    if (!std::isprint(c)) {
      return false;
    }
  }
  return true; 
}

std::pair<int, std::string> scoreCipherTexts(std::istream& input) {
  std::string ciphertextLine;
  std::vector<std::tuple<double, char, std::string> > plaintexts;
  std::vector<unsigned char>* plaintextBytes;

  while (std::getline(input, ciphertextLine)) {
    /* std::cout << ciphertextLine << std::endl << ">>>>>>>" << std::endl; */ 
    plaintextBytes = hexToBytes(ciphertextLine);
    plaintexts.push_back(getPlaintext(*plaintextBytes));
  }

  int indexOfBest;
  double bestScore = std::numeric_limits<double>::max();
  std::string bestPlaintext;

  for (int i = 0; i < plaintexts.size(); i++) {
    if (isPrintable(std::get<2>(plaintexts[i]))) {
      std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl
                << "score         " << std::get<0>(plaintexts[i]) << std::endl
                << "plaintext     " << std::get<2>(plaintexts[i]) << std::endl << std::endl;
    }
    if (std::get<0>(plaintexts[i]) < bestScore) {
      indexOfBest = i;
      bestPlaintext = std::get<2>(plaintexts[i]);
    }
  }
  return std::make_pair(indexOfBest, bestPlaintext);
}

std::vector<unsigned char>* repeatingKeyXorEncrypt(const std::vector<unsigned char>& plaintext,
                                                   const std::vector<unsigned char>& key) {
  std::vector<unsigned char>* ciphertext = new std::vector<unsigned char>();
  for (int i = 0; i < plaintext.size(); i++) {
    ciphertext->push_back(plaintext[i] ^ key[(i % key.size())]);
  }
  return ciphertext;
}

int main() {
  std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  const std::vector<unsigned char>* bytes = hexToBytes(hex);

  std::string base64 = bytesToBase64(*bytes);
  std::string expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

  std::cout << "TEST bytesToBase64======" 
            << std::endl;
  std::cout << std::endl
            << "=EXPECTED="
            << std::endl
            << expected
            << std::endl
            << std::endl;
  std::cout << "=ACTUAL==="
            << std::endl
            << base64 
            << std::endl
            << std::endl;
  std::cout << "EQUAL?===="
            << std::endl
            << (base64 == expected ? "True" : "False")
            << std::endl;

  const std::vector<unsigned char>* bytes1 = hexToBytes("1c0111001f010100061a024b53535009181c");
  const std::string hex1 = bytesToHex(*bytes1);
  std::cout << "TEST bytesToHex======" 
            << std::endl;
  std::cout << std::endl
            << "=EXPECTED="
            << std::endl
            << "1c0111001f010100061a024b53535009181c"
            << std::endl
            << std::endl;
  std::cout << "=ACTUAL==="
            << std::endl
            << hex1
            << std::endl
            << std::endl;

  const std::vector<unsigned char>* xorInput1 = hexToBytes("1c0111001f010100061a024b53535009181c");
  const std::vector<unsigned char>* xorInput2 = hexToBytes("686974207468652062756c6c277320657965");
  std::vector<unsigned char>* xorOutput = fixedXor(*xorInput1, *xorInput2);
  expected = "746865206b696420646f6e277420706c6179";

  std::cout << "TEST fixedXor======" 
            << std::endl;
  std::cout << std::endl
            << "=EXPECTED="
            << std::endl
            << expected
            << std::endl
            << std::endl;
  std::cout << "=ACTUAL==="
            << std::endl
            << bytesToHex(*xorOutput)
            << std::endl
            << std::endl;

  const std::vector<unsigned char>* cipherText = hexToBytes("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
  std::tuple<double, char, std::string> result = getPlaintext(*cipherText);
  std::cout << "PLAINTEXT=========="
            << std::get<2>(result)
            << std::endl;
  std::cout << "KEY=========="
            << std::get<1>(result)
            << std::endl
            << std::endl;

  
  std::ifstream ciphertextsFile("ciphertext_xor.txt");
  std::pair<int, std::string> bestPlaintext = scoreCipherTexts(ciphertextsFile);
  /* std::cout << "BEST PLAINTEXT======" << std::endl */
  /*           << bestPlaintext.second << std::endl */
  /*           << "at line # " << bestPlaintext.first << std::endl; */
  ciphertextsFile.close();
  
  

  /* std::string plaintext = "abcBIG,''ffff--Hi"; */
  /* std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::tolower); */
  /* std::cout << plaintext << std::endl; */

  std::cout << "TEST REPEATING KEY CIPHER==========" << std::endl;
  std::string plaintext =
    "Burning 'em, if you ain't quick and nimble\n"
    "I go crazy when I hear a cymbal";
  std::vector<unsigned char> plaintextVector = std::vector<unsigned char>(plaintext.begin(),
                                                                          plaintext.end());
  std::string key = "ICE";
  std::vector<unsigned char> keyVector = std::vector<unsigned char>(key.begin(), key.end());
  std::vector<unsigned char>* ciphertext = repeatingKeyXorEncrypt(plaintextVector, keyVector);
  std::cout << "ciphertext: " << bytesToHex(*ciphertext) << std::endl;
  return 0;
}
