#ifndef RCIPHER_H_ 
#define RCIPHER_H_
#include "ccipher.h"

using namespace std;

/* A class that implements a
   ROT13 cipher class. It 
   inherts class CCipher */
class RCipher: public CCipher{
public:
   // we only need the default constructor and destructor becuase ROT13 always shifts by 13
   RCipher();
   ~RCipher();
};
#endif

