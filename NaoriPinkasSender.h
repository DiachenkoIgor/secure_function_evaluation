#ifndef NAORIPINKASSENDER_H
#define NAORIPINKASSENDER_H
#include "NaoriPinkasSenderData.h"
#include "NaoriPinkasTransfer.h"
#include "NaoriPinkasReceiverData.h"
#include "./src/ReceiverPublicKey.h"
#include "ObliviousUtils.h"
#include <stdio.h>
#include <gmp.h>
#include <string>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <thread>
#include "./src/SimpleLogging.h"
class NaoriPinkasSender
{
private:

    NaoriPinkasSenderData data;
    NaoriPinkasReceiverData receiverData;
    std::string path;
    int fDescriptor;
    
    void initServerAndAccept();
public:

    int messageByteLength;
    
    NaoriPinkasSender(std::string path);
    NaoriPinkasSender();
    ~NaoriPinkasSender();
    
    static void encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer);
                                    
    void initialize();
    void handshake(int msgSize);
    void sendMessages(char * m1, int m1Size, char * m2, int m2Size);
    
    void setDescriptor(int descriptor){
        this -> fDescriptor = descriptor;
    }

};

#endif // NAORIPINKASSENDER_H
