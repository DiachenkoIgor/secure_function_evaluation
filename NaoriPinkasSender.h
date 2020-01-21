#ifndef NAORIPINKASSENDER_H
#define NAORIPINKASSENDER_H
#include "NaoriPinkasSenderData.h"
#include "NaoriPinkasTransfer.h"
#include "ObliviousUtils.h"
#include <stdio.h>
#include <gmp.h>
class NaoriPinkasSender
{
public:
    NaoriPinkasSender();
    ~NaoriPinkasSender();
   static void encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer);

};

#endif // NAORIPINKASSENDER_H
