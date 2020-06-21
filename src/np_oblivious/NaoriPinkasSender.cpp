#include "NaoriPinkasSender.h"

NaoriPinkasSender::NaoriPinkasSender()
{
}

NaoriPinkasSender::NaoriPinkasSender(std::string path)
{
    this -> path = path;
}

NaoriPinkasSender::~NaoriPinkasSender()
{
}


void NaoriPinkasSender::initialize(){
    NaoriPinkasSenderData::readInitDataFromFile(this -> path, this -> data);
}

void NaoriPinkasSender::handshake(int msgSize){

    this -> messageByteLength = msgSize;

    NaoriPinkasReceiverData::createReceiverData(this -> receiverData, this -> data, msgSize);
    std::string value = ObliviousUtils::serializeNPReceiverData(receiverData);

    send(this -> fDescriptor , value.c_str() , value.size() , 0 );

}

void NaoriPinkasSender::sendMessages(char * m1, int m1Size, char * m2, int m2Size){
    std::string key = ObliviousUtils::readJSON(this -> fDescriptor);

    ReceiverPublicKey pk;
    NaoriPinkasTransfer transfer;
    ObliviousUtils::deserializeNPReceiverPublicKey(pk, key);

    NaoriPinkasSender::encryptData(this -> data, m1, (size_t)m1Size, m2, (size_t)m2Size, pk.key, this -> messageByteLength, transfer);

    std::string value = ObliviousUtils::serializeNPTransfer(transfer);

    send(this -> fDescriptor , value.c_str() , value.size() , 0 );
    
}

void NaoriPinkasSender::encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer){
    mpz_t msg1, msg2, pk0, pk1, R, encrypt1, encrypt2;
    
    mpz_inits(msg1, msg2, pk0, pk1, R, encrypt1, encrypt2, NULL);
    
    
    ObliviousUtils::importGmpNumberFromByteArray(msg1, (const char *) m1, (int) m1Size);
    ObliviousUtils::importGmpNumberFromByteArray(msg2, (const char *) m2, (int) m2Size);

    
    mpz_powm (pk0, pk, requiredData.r, requiredData.p);

    mpz_invert (pk1, pk0, requiredData.p);

    if(mpz_cmp_ui (pk1, 0) == 0)  throw std::runtime_error("Naori-Pinkas receiver's key hasn't multiplicative inverse!");

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

