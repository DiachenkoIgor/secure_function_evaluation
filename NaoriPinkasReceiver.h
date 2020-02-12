#ifndef NAORIPINKASRECEIVER_H
#define NAORIPINKASRECEIVER_H
#include <stdio.h>
#include <string>
#include <iostream>  
#include <gmp.h>
#include "NaoriPinkasReceiverData.h"
#include "ObliviousUtils.h"
#include "NaoriPinkasTransfer.h"
#include "./src/ReceiverPublicKey.h"
#include "./src/SimpleLogging.h"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <thread>
#include <chrono>
class NaoriPinkasReceiver
{
private:
    int fDescriptor, choice;
    NaoriPinkasReceiverData data;
    
    mpz_t key;

public:
    NaoriPinkasReceiver(int descriptor);
    NaoriPinkasReceiver();
    ~NaoriPinkasReceiver();
    
    static void generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& pkey, mpz_t& key);
    static void decryptResult(NaoriPinkasTransfer& requiredData, int choice, int msgByteLength, mpz_t& key, mpz_t& result);

    void initialize();
    void receive(int choice, char * result);
    void step1();
    void step2(char * result);
    
    int getMsgSize(){
        return data.msgByteLength;
    }
    
    void setDescriptor(int descriptor){
        this -> fDescriptor = descriptor;
    }
};

#endif // NAORIPINKASRECEIVER_H
