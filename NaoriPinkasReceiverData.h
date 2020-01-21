#ifndef NAORIPINKASRECEIVERDATA_H
#define NAORIPINKASRECEIVERDATA_H
#include <stdio.h>
#include <gmp.h>
class NaoriPinkasReceiverData
{
public:

    mpz_t p, q, g, C, gr;

    NaoriPinkasReceiverData();
    ~NaoriPinkasReceiverData();

};

#endif // NAORIPINKASRECEIVERDATA_H
