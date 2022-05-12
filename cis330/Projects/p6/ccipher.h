#ifndef CCIPHER_H_
#define CCIPHER_H_
#include "cipher.h"

using namespace std;

/* A class that implements a
   Caesar cipher class. It 
   inherits the Cipher class */
class CCipher: public Cipher {
protected:
   struct CCipherCheshire;
   CCipherCheshire *caesar;
public:
   CCipher();
   CCipher(int cipher_shift);
   ~CCipher();
};

/* Helper function headers 
 */
void rotate_string(string& in_str, int rot);
#endif

// trying to run caesar cipher and there is no output
// I have the Caesar cipher class inheriting from the cipher class
// encrypt and decrypt not running