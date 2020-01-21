#ifndef NAORIPINKASRECEIVER_H
#define NAORIPINKASRECEIVER_H
#include <stdio.h>
#include <gmp.h>
#include "NaoriPinkasReceiverData.h"
#include "ObliviousUtils.h"
#include "NaoriPinkasTransfer.h"
class NaoriPinkasReceiver
{
public:
    NaoriPinkasReceiver();
    ~NaoriPinkasReceiver();
    
    static void generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& key, mpz_t& result);
    static void decryptResult(NaoriPinkasTransfer& requiredData, int choice, int msgByteLength, mpz_t& key, mpz_t& result);

};

#endif // NAORIPINKASRECEIVER_H
