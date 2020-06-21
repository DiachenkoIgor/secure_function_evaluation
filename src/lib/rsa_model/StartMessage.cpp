#include "StartMessage.h"

StartMessage::StartMessage()
{
    mpz_inits(publicExponent, modulus, x0, x1, NULL);
}

StartMessage::~StartMessage()
{
    mpz_clears(publicExponent, modulus, x0, x1, NULL);
}

