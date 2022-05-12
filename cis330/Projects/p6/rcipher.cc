#include <string>
#include <iostream>
#include "rcipher.h"

// -------------------------------------------------------
// ROT13 Cipher implementation
// -------------------------------------------------------
// CCipher::cipher_shift is private, but will be initialized to with 13 by using its own constructor
RCipher::RCipher() : CCipher(13) {} 

RCipher::~RCipher(){
   // 
}
