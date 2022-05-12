#include <string>
#include <iostream>
#include <vector>
#include "kcipher.h"

/* Helper function definitions
 */
bool isValid(string str); // make sure string is lowercase

// -------------------------------------------------------
// Running Key Cipher implementation
// -------------------------------------------------------

struct KCipher::RKCipherCheshire{
    vector<string> key;
    int page;
};

struct Cipher::CipherCheshire {
	string cipher_alpha;
};

KCipher::KCipher(){
    // base case
    ksmile = new struct RKCipherCheshire;
    // adds MAX_LENGTH of 'a's to the value fill
    // Resource: https://stackoverflow.com/questions/33397628/c-add-x-amount-of-chars-to-string-array
    string fill (MAX_LENGTH, 'a');
    add_key(fill);
    set_id(0);
}

KCipher::KCipher(string str){
    ksmile = new struct RKCipherCheshire;
    add_key(str);
}

KCipher::~KCipher(){
    delete(ksmile);
}

void KCipher::add_key(string str){ // I think this is messing up everything
    if(str.empty() || !(isValid(str))){
        cerr << "Invalid Running key: " << str << endl;
        exit(EXIT_FAILURE);
    }else{
        ksmile->key.push_back(str);
    }
}

void KCipher::set_id(unsigned int book_page) {
    if (book_page <= ksmile->key.size()){
        ksmile->page = book_page;
    }else {
        cerr << "Warning: invalid id: " << book_page << endl;
        exit(EXIT_FAILURE);
    }
}

string KCipher::encrypt(string raw) {
    cout << "Encrypting...";
    string retStr;
    string book = ksmile->key[ksmile->page];
    size_t book_len = book.length();
    string new_key;

    // Format the key to match the spaces in raw
    // add everything besides spaces to new_key
    for (unsigned int i = 0; i < book_len; i++){
        char letter = book.at(i); 
        if(letter != ' '){
            new_key += book[i];
        }
    }
    // add spaces back to get propper spacing
    for (unsigned int i = 0; i < raw.length(); i++){
        // taking spaces from raw so we can get propper spacing
        char letter = raw.at(i); 
        if (letter == ' '){
            new_key.insert(i, 1, letter);
        }
    }

    // loop over the raw string
    // and create alphabet grid described in class
    for (size_t i = 0; i < raw.length(); i++){
        string alphabet = smile->cipher_alpha;
        char letter = raw.at(i); 
        if (letter != ' '){
            // RESOURCE: http://www.practicalcryptography.com/ciphers/running-key-cipher/
            if (islower(letter)){
                // rotate string (- 'a' takes the letter away from new_key)
                rotate_string(alphabet, (new_key[i] - 'a'));
                retStr += alphabet[letter - 'a'];
            } else{
                rotate_string(alphabet, (new_key[i] - 'a'));
                retStr += alphabet[letter - 'A'] - 32;
            }
        } else{
            retStr += ' ';
        }

    }
    cout << "Done" << endl;
    return retStr;
}

string KCipher::decrypt(string enc){
    cout << "Decrypting...";
    string retStr;
    string book = ksmile->key[ksmile->page];
    string new_key;

    // add everything besides spaces to new_key (same thing we did in encrypt)
    for (unsigned int i = 0; i < book.size(); i++){
        char letter = book.at(i); 
        if(letter != ' '){
            new_key += book[i];
        }
    }

    // add spaces back to get propper spacing
    for (unsigned int i = 0; i < enc.length(); i++){
        char letter = enc.at(i); 
        if (letter == ' '){
            new_key.insert(i, 1, letter);
        }
    }

    // Loop over the enc string
    for (unsigned int i = 0; i < enc.length(); i++){
        string alphabet = smile->cipher_alpha;
        char letter = enc.at(i); 
        if (letter != ' '){
            // RESOURCE: http://www.practicalcryptography.com/ciphers/running-key-cipher/
            if (islower(letter)){ 
                // step 1: rotate alphabet letter places (ie string "hey"; string[0] - 'a' == ey)
                rotate_string(alphabet, new_key[i] - 'a');
                // we then find letter in the alphabet and take that index of the cipher
                retStr += smile->cipher_alpha[alphabet.find(letter)];
            }
            if (isupper(letter)){
                rotate_string(alphabet, new_key[i] - 'a');
                retStr += (smile->cipher_alpha[alphabet.find(letter + 32)]- 32);
            }
        }else{
            retStr += ' ';
        }
    }
    cout << "Done" << endl;
    return retStr;
}


bool isValid(string str){
    for (long unsigned int i = 0; i < str.length(); i++){
        char letter = str.at(i);
        if (letter >= 65 && letter <= 90){ // letter is uppercase (reference: ASCII table)
            return false;
        }else if(letter == ' '){
            continue;
        }
    }
    return true;
}
