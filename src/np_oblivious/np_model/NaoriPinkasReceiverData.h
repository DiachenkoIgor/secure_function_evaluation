#ifndef NAORIPINKASRECEIVERDATA_H
#define NAORIPINKASRECEIVERDATA_H
#include <stdio.h>
#include <gmp.h>
#include "NaoriPinkasSenderData.h"

class NaoriPinkasSenderData;

class NaoriPinkasReceiverData
{
public:

    mpz_t p, q, g, C, gr;
    int msgByteLength;

    NaoriPinkasReceiverData();
    ~NaoriPinkasReceiverData();
    
    static void createReceiverData(NaoriPinkasReceiverData& result, NaoriPinkasSenderData& data, int messagesLength);

};

#endif // NAORIPINKASRECEIVERDATA_H
