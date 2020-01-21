#include "NaoriPinkasReceiver.h"

NaoriPinkasReceiver::NaoriPinkasReceiver()
{
}

NaoriPinkasReceiver::~NaoriPinkasReceiver()
{
}

void NaoriPinkasReceiver::generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& key, mpz_t& result){
    mpz_t k, gk, c_over_gk;
    mpz_inits(k, gk, c_over_gk, NULL);
    
    ObliviousUtils::generateRandomNumber(k, 
            ceil(ObliviousUtils::bit_size(requiredData.q)/8.0));
    
    mpz_mod (k, requiredData.q, k);
    
    mpz_powm (requiredData.g, k, requiredData.p, gk);
    
    mpz_invert (c_over_gk, gk, requiredData.p);
    
    mpz_mul (c_over_gk, requiredData.C, c_over_gk);
    
    mpz_mod (c_over_gk, c_over_gk, requiredData.p);
    
    mpz_powm (key, requiredData.gr, k, requiredData.p);
    
    if(choice == 1){
        mpz_set(key, c_over_gk);
    }else{
        mpz_set(key, gk);
    }
}

void NaoriPinkasReceiver::decryptResult(NaoriPinkasTransfer& requiredData, int choice, int msgByteLength, mpz_t& key, mpz_t& result){
    mpz_t R, msg;
    mpz_inits(R, msg, NULL);
    
    ObliviousUtils::importGmpNumberFromByteArray(R, requiredData.getR(), requiredData.GetRSize());
    
    if(choice==1){
        ObliviousUtils::importGmpNumberFromByteArray(msg, requiredData.getM2(), requiredData.GetM2Size());
        
        ObliviousUtils::decrypt(result, key, R, msg, msgByteLength);
    }else {
        ObliviousUtils::importGmpNumberFromByteArray(msg, requiredData.getM1(), requiredData.GetM1Size());
        
        ObliviousUtils::decrypt(result, key, R, msg, msgByteLength);
    }
}


