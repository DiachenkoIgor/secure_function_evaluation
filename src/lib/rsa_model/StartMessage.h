#ifndef STARTMESSAGE_H
#define STARTMESSAGE_H
#include <stdio.h>
#include <gmp.h>
class StartMessage
{
public:
    mpz_t publicExponent, modulus, x0, x1;

    StartMessage();
    ~StartMessage();

};

#endif // STARTMESSAGE_H
