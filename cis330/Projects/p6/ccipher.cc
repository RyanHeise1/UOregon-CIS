#include <string>
#include <iostream>
#include <algorithm>
#include "ccipher.h"

struct Cipher::CipherCheshire {
    string cipher_alpha;
};

struct CCipher::CCipherCheshire {
    int cipher_shift;
};

// -------------------------------------------------------
// Caesar Cipher implementation
// -------------------------------------------------------
CCipher::CCipher(){
    // default shift is 0
    caesar = new struct CCipherCheshire;
    caesar->cipher_shift = 0;
}

CCipher::CCipher(int cipher_shift){
    if (cipher_shift >= 0){
        caesar = new struct CCipherCheshire;
        caesar->cipher_shift = cipher_shift;
        rotate_string(smile->cipher_alpha, caesar->cipher_shift);
    } else{
        cerr << "Error: Caesar cipher is less than 0" << endl;
        exit(EXIT_FAILURE);
    }
}

CCipher::~CCipher(){
    delete(caesar);
}

// Rotates the input string in_str by rot positions
void rotate_string(string& in_str, int rot)
{
    if (rot > 26){
        rot = rot % 26;
    }
    // RESORCE: http://www.cplusplus.com/reference/algorithm/rotate/   rotate()
    rotate(in_str.begin(), in_str.begin() + rot, in_str.end());
}
