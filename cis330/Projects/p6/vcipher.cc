#include <string>
#include <iostream>
#include <vector>
#include "kcipher.h"
#include "vcipher.h"


// -------------------------------------------------------
// Running Key Cipher implementation
// -------------------------------------------------------
VCipher::VCipher(){

}

// similar to how we did in rot13
VCipher::VCipher(string str) : KCipher(str){

}

VCipher::~VCipher() {}
