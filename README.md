C++ Solutions to the Matasano Crypto Challenge
==============================================
For more on the challenge, go [here](http://cryptopals.com/).  Wherever possible,
I've extracted out utilities for solving the challenges into the unit-tested
stand-alone library `cryptolib/`.

To run the unit tests from the main folder,

  mkdir build && cd build
  cmake .. && make
  ./runTests

The unittests require [Google Test](https://github.com/google/googletest/) to be installed.
