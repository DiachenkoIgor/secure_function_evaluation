#include "ReceiverPublicKey.h"

ReceiverPublicKey::ReceiverPublicKey()
{
    mpz_init(key);  
}

ReceiverPublicKey::~ReceiverPublicKey()
{
    mpz_clear(key);
}

ReceiverPublicKey::ReceiverPublicKey(mpz_t& value){
    mpz_init(key);
    mpz_set(key, value);
}

