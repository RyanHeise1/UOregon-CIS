#ifndef KCIPHER_H_
#define KCIPHER_H_
#include "cipher.h"
#include "ccipher.h"

using namespace std;

const unsigned int MAX_LENGTH = 100;

/* A class that implements a
   Running key cipher class. It 
   inherts class Cipher */
class KCipher: public Cipher{
protected: 
   struct RKCipherCheshire;
   RKCipherCheshire *ksmile;
public:
   KCipher();
   KCipher(string str);
   ~KCipher();
   void add_key(string str);
   void set_id(unsigned int book_page);
   virtual string encrypt(string raw);
   virtual string decrypt(string enc);
};

bool isLower(string str);
#endif

