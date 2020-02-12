#ifndef SOCKETNPSENDER_H
#define SOCKETNPSENDER_H
#include <string>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include "NaoriPinkasSender.h" 
#include <thread>

class SocketNPSender
{
private:
    int port;
    std::string path ;
    
    int server_fd, new_socket;
    
    NaoriPinkasSender* sender;
    
    void initServerAndAccept();
    std::string readJSON();
public:
    SocketNPSender();
    SocketNPSender(int port, std::string path);
    ~SocketNPSender();
    void start(int msgByteLength);
    void sendMessage(char * m1, int m1Size, char * m2, int m2Size);

};

#endif // SOCKETNPSENDER_H
