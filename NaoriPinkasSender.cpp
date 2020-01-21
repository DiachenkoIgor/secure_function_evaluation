#include "NaoriPinkasSender.h"

NaoriPinkasSender::NaoriPinkasSender()
{
}

NaoriPinkasSender::~NaoriPinkasSender()
{
}

void NaoriPinkasSender::encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer){
    mpz_t msg1, msg2, pk0, pk1, R, encrypt1, encrypt2;
    
    mpz_inits(msg1, msg2, pk0, NULL);
    
    
    ObliviousUtils::importGmpNumberFromByteArray(msg1, (const char *) m1, (int) m1Size);
    ObliviousUtils::importGmpNumberFromByteArray(msg2, (const char *) m2, (int) m2Size);
    
    mpz_powm (pk0, pk, requiredData.r, requiredData.p);
    
    mpz_invert (pk1, pk0, requiredData.p);
    
    if(mpz_cmp_ui (pk1, 0))  throw std::runtime_error("Naori-Pinkas receiver's key hasn't multiplicative inverse!");

    mpz_mul (pk1, pk1, requiredData.Cr);
    
    mpz_mod (pk1, pk1, requiredData.p);
    
    ObliviousUtils::generateRandomNumber(R, (requiredData.rLength/8) + 1);
    
    ObliviousUtils::encrypt(encrypt1, pk0, R, msg1, msgByteLength);
    ObliviousUtils::encrypt(encrypt2, pk1, R, msg2, msgByteLength);
    
    char * msg1Ptr = nullptr;
    char * msg2Ptr = nullptr;
    char * RPtr = nullptr;
    
    int msg1ArraySize = ObliviousUtils::exportGmpNumberToByteArray(encrypt1, msg1Ptr);
    int msg2ArraySize = ObliviousUtils::exportGmpNumberToByteArray(encrypt2, msg2Ptr);
    int RArraySize = ObliviousUtils::exportGmpNumberToByteArray(R, RPtr);
    
    transfer.setM1(msg1Ptr, msg1ArraySize);
    transfer.setM2(msg2Ptr, msg2ArraySize);
    transfer.setR(RPtr, RArraySize);
    
    delete [] msg1Ptr;
    delete [] msg2Ptr;
    delete [] RPtr;
}

