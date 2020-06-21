#ifndef NAORIPINKASSENDER_H
#define NAORIPINKASSENDER_H
#include "./np_model/NaoriPinkasSenderData.h"
#include "./np_model/NaoriPinkasTransfer.h"
#include "./np_model/NaoriPinkasReceiverData.h"
#include "./np_model/ReceiverPublicKey.h"
#include "./np_util/ObliviousUtils.h"
#include <stdio.h>
#include <gmp.h>
#include <string>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <thread>
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
