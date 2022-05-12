#include "cipher.h"

/* Cheshire smile implementation.
   It only contains the cipher alphabet
 */
struct Cipher::CipherCheshire {
    string cipher_alpha;
};

/* This function checks the cipher alphabet
   to make sure it's valid
 */
bool is_valid_alpha(string alpha);


// -------------------------------------------------------
// Cipher implementation
/* Default constructor
   This will actually not encrypt the input text
   because it's using the unscrambled alphabet
 */
Cipher::Cipher()
{
    // TODO: Implement this default constructor 
    smile = new struct CipherCheshire;
    smile->cipher_alpha =  "abcdefghijklmnopqrstuvwxyz";
}

/* This constructor initiates the object with a
   input cipher key
 */
Cipher::Cipher(string cipher_alpha)
{
    // TODO: Implement this constructor
    if (is_valid_alpha(cipher_alpha)){ //is_valid also checks that its == ALPABET SIZE
        smile = new struct CipherCheshire;
        smile->cipher_alpha = cipher_alpha;
    }else{
        cerr << "ERROR: Cipher key is not valid: " << cipher_alpha << endl;
        exit(EXIT_FAILURE);
    }
}

/* Destructor
 */
Cipher::~Cipher()
{
    // TODO: Implement this constructor
    delete(smile);
}

/* This member function encrypts the input text 
   using the intialized cipher key
 */
string Cipher::encrypt(string raw)
{
    cout << "Encrypting...";
    string retStr;
    // store alphabet so we can find the position of raw[i] (letter) in the alphabet
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    
    // Loop over the raw string
    // RESORURCE: https://www.cplusplus.com/reference/string/string/length/  string.length()
    //            https://www.cplusplus.com/reference/string/string/at/      string.at()
    for (long unsigned int i = 0; i < raw.length(); i++){ // long unsigned int because raw.length()
        char letter = raw.at(i); // could also use raw[i]
        if (letter != ' '){ // if the letter we are looking at is not a space go into the loop
            int position = find_pos(alphabet, letter);
            if (isupper(letter)){ // letter is uppercase
                retStr += UPPER_CASE(smile->cipher_alpha[position]);
            }else if(islower(letter)){ // ch is lowercase
                retStr += LOWER_CASE(smile->cipher_alpha[position]);
            } // else, letter is not in the alphabet, Do Nothing
        }else{
            retStr += ' '; // There is a space, add it to the return string
        }
    }
    cout << "Done" << endl;
    return retStr;
}


/* This member function decrypts the input text 
   using the intialized cipher key
 */
string Cipher::decrypt(string enc)
{
    cout << "Decrypting...";
    string retStr;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    
    // Loop over the enc string
    for (long unsigned int i = 0; i < enc.length(); i++){
        char letter = enc.at(i); 
        if (letter != ' '){ // if letter is not a space
            int position = find_pos(smile->cipher_alpha, letter); // find enc[i] in the scrambled aplhabet
            if (isupper(letter)){ // ch is uppercase
                retStr += UPPER_CASE(alphabet[position]);
            }else if(islower(letter)){ // ch is lowercase
                retStr += LOWER_CASE(alphabet[position]);
            } // else, ch is not in the alphabet, Do Nothing
        }else{
            retStr += ' '; // letter is a space, add space to retStr
        }
    }

    cout << "Done" << endl;

    return retStr;

}
// -------------------------------------------------------


//  Helper functions 
/* Find the character c's position in the cipher alphabet/key
 */
unsigned int find_pos(string alpha, char c)
{
    unsigned int pos = 0;
    char lower = LOWER_CASE(c);
    for (long unsigned int i = 0; i < alpha.length(); i++){
        if (lower == alpha.at(i)){
            pos = i;
            break;
        }
    }
    return pos;
}

/* Make sure the cipher alphabet is valid - 
   a) it must contain every letter in the alphabet 
   b) it must contain only one of each letter 
   c) it must be all lower case letters 
   ALL of the above conditions must be met for the text to be a valid
   cipher alphabet.
 */
bool is_valid_alpha(string alpha)
{
    bool is_valid = true;
    if(alpha.size() != ALPHABET_SIZE) {
        is_valid = false; 
    } else {
        unsigned int letter_exists[ALPHABET_SIZE];
        for(unsigned int i = 0; i < ALPHABET_SIZE; i++) {
            letter_exists[i] = 0;
        }
        for(unsigned int i = 0; i < alpha.size(); i++) {
            char c = alpha[i];
            if(!((c >= 'a') && (c <= 'z'))) {
                is_valid = false;
            } else {
                letter_exists[(c - 'a')]++;
            }
        }
        for(unsigned int i = 0; i < ALPHABET_SIZE; i++) {
            if(letter_exists[i] != 1) {
                is_valid = false;
            }
        }
    }

    return is_valid;
}
