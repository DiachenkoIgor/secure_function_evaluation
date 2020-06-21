#ifndef RECEIVERPUBLICKEY_H
#define RECEIVERPUBLICKEY_H
#include <stdio.h>
#include <gmp.h>

class ReceiverPublicKey
{
public:
    mpz_t key;
    
    
    ReceiverPublicKey();
    ReceiverPublicKey(mpz_t& value);
    ~ReceiverPublicKey();

};

#endif // RECEIVERPUBLICKEY_H
