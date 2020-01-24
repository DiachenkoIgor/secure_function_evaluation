#ifndef NAORIPINKASRECEIVER_H
#define NAORIPINKASRECEIVER_H
#include <stdio.h>
#include <string>
#include <gmp.h>
#include "NaoriPinkasReceiverData.h"
#include "ObliviousUtils.h"
#include "NaoriPinkasTransfer.h"
#include "./src/ReceiverPublicKey.h"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <thread>
#include <chrono>
class NaoriPinkasReceiver
{
private:
    int sock, choice, port;
    std::string address;
    NaoriPinkasReceiverData data;
    
    ReceiverPublicKey pk;
    
    std::string readJSON();
    int initSocketClient();
    
    
    
public:
    NaoriPinkasReceiver(int port, std::string address);
    NaoriPinkasReceiver();
    ~NaoriPinkasReceiver();
    
    static void generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& key);
    static void decryptResult(NaoriPinkasTransfer& requiredData, int choice, int msgByteLength, mpz_t& key, mpz_t& result);

    void initialize();
    void receive(int choice, char * result);
    void step1();
    void step2(char * result);
    
    int getMsgSize(){
        return data.msgByteLength;
    }
};

#endif // NAORIPINKASRECEIVER_H
