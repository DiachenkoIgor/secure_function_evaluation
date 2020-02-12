#ifndef SOCKETNPRECEIVER_H
#define SOCKETNPRECEIVER_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <gmp.h>
#include "NaoriPinkasReceiver.h"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
class SocketNPReceiver
{
private:
    int sock, port;
    std::string address;
    NaoriPinkasReceiver receiver;
    
    int initSocketClient();
public:
    SocketNPReceiver();
    ~SocketNPReceiver();
    SocketNPReceiver(int port, std::string address);
    
    void initialize();

    void receive(int choice, char * result);
    
    int getMsgSize(){
            return this -> receiver.getMsgSize();
    }


};

#endif // SOCKETNPRECEIVER_H
