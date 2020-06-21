
#ifndef _OBLIVIOUS_UTIL
#define _OBLIVIOUS_UTIL

#include <stdio.h>
#include <gmp.h>
#include <sodium.h>
#include <math.h> 
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <iostream>
#include <sodium.h>
#include "./sfe_model/CryptoPairHolder.h"

namespace oblivious_util {
    
    #define MODULUS_SIZE 1024                   /* This is the number of bits we want in the modulus */
    #define BLOCK_SIZE (MODULUS_SIZE/8)         /* This is the size of a block that gets en/decrypted at once */
    #define BUFFER_SIZE ((MODULUS_SIZE/8) / 2)  /* This is the number of bytes in n and p */
    
    const int MAC_SIZE = crypto_secretbox_MACBYTES;
    
    typedef struct {
        mpz_t n; /* Modulus */
        mpz_t e; /* Public Exponent */
    } public_key;

    typedef struct {
        mpz_t n; /* Modulus */
        mpz_t e; /* Public Exponent */
        mpz_t d; /* Private Exponent */
        mpz_t p; /* Starting prime p */
        mpz_t q; /* Starting prime q */
    } private_key;


    void generateRandomNumber(mpz_t &place, int bytesSize);
    
    int exportGmpNumberToByteArray(mpz_t &data, char *& ptr);
    
    int importGmpNumberFromByteArray(mpz_t &data, const char * ptr, int size);
    
    void generate_keys(private_key* ku, public_key* kp);
    
    void randomize(std::vector<CryptoPairHolder> &table);
    
    void generateSecretKey(char * key);

    void generateSecretNonce(char * nonce, int nonceSize);

    void secretEncrypt(char * cipher_result, char * message, int messageLength, char * nonce, char * key);

    int secretDecrypt(char * plain_result, char * cipher_text, int cipher_length, char * nonce, char * key);

}


#endif

