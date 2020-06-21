#include "NaoriPinkasReceiver.h"

NaoriPinkasReceiver::NaoriPinkasReceiver()
{
     mpz_init(this -> key);
}

NaoriPinkasReceiver::~NaoriPinkasReceiver()
{
    mpz_clear(this -> key);
}

NaoriPinkasReceiver::NaoriPinkasReceiver(int descriptor){
    this -> fDescriptor = descriptor;
    mpz_init(this -> key);
}


void NaoriPinkasReceiver::initialize(){

    std::string im = ObliviousUtils::readJSON(this -> fDescriptor);
   
    ObliviousUtils::deserializeNPReceiverData(this -> data, im);
    
}
void NaoriPinkasReceiver::receive(int choice, char * result){
    this -> choice = choice;
    step1();
    step2(result);
}
void NaoriPinkasReceiver::step1(){
    mpz_t pkey;
    mpz_init(pkey);

    ReceiverPublicKey pk;
    
    generatePublicKey(this -> data, this -> choice, pkey, this -> key);

    mpz_set(pk.key, pkey);
    
    std::string result = ObliviousUtils::serializeNPReceiverPublicKey(pk);
    
    send(this -> fDescriptor , result.c_str() , result.size() , 0 );
    
    mpz_clear(pkey);
}

void NaoriPinkasReceiver::step2(char * result){
    mpz_t tmpResult;
    mpz_init(tmpResult);

    std::string json = ObliviousUtils::readJSON(this -> fDescriptor);
    
    NaoriPinkasTransfer transfer;
    ObliviousUtils::deserializeNPTransfer(transfer, json);

    decryptResult(transfer, choice, this -> data.msgByteLength, this -> key ,tmpResult);

    char * tmp = nullptr;
    int quantity = ObliviousUtils::exportGmpNumberToByteArray(tmpResult, tmp);
    memcpy(result, tmp, quantity);
    mpz_clear(tmpResult);
    delete [] tmp;
}

void NaoriPinkasReceiver::generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& pkey, mpz_t& key){

    mpz_t k, gk, c_over_gk;
    mpz_inits(k, gk, c_over_gk, NULL);
    
    ObliviousUtils::generateRandomNumber(k, 
            ceil(ObliviousUtils::bit_size(requiredData.q)/8.0));
            
    mpz_mod (k, k, requiredData.q);
    
    mpz_powm (gk, requiredData.g, k, requiredData.p);

    mpz_invert (c_over_gk, gk, requiredData.p);

    mpz_mul (c_over_gk, requiredData.C, c_over_gk);

    mpz_mod (c_over_gk, c_over_gk, requiredData.p);

    mpz_powm (key, requiredData.gr, k, requiredData.p);

    if(choice == 1){
        mpz_set(pkey, c_over_gk);
    }else{
        mpz_set(pkey, gk);
    }
    
    mpz_clears(k, gk, c_over_gk, NULL);       
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
    
    mpz_clears(R, msg, NULL);
}


