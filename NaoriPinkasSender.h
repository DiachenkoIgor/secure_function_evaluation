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
class NaoriPinkasSender
{
private:

    NaoriPinkasSenderData data;
    NaoriPinkasTransfer transfer;
    std::string path;
    int port;
    
    int server_fd, new_socket;
    
    void initServerAndAccept();
    std::string readJSON();
public:
int messageByteLength;
    NaoriPinkasSender(std::string path, int port);
    NaoriPinkasSender();
    ~NaoriPinkasSender();
   static void encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer);
                                    
    void initialize();
    void sendMessages(char * m1, int m1Size, char * m2, int m2Size);

};

#endif // NAORIPINKASSENDER_H
