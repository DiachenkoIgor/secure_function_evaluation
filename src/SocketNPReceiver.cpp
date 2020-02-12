#include "SocketNPReceiver.h"

SocketNPReceiver::SocketNPReceiver(int port, std::string address)
{
    this -> port = port;
    this -> address = address;
}

SocketNPReceiver::SocketNPReceiver()
{
    
}
SocketNPReceiver::~SocketNPReceiver()
{
    close(this -> sock);
}


int SocketNPReceiver::initSocketClient(){

    struct sockaddr_in serv_addr; 

    if ((this -> sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::cout << "Socket creation error" << std::endl;
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(this -> port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, this -> address.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    } 
   
    if (connect(this -> sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        std::cout << "Connection Failed" << std::endl;
        return -1;
    } 
}

void SocketNPReceiver::initialize(){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    if(initSocketClient() == -1){
        return;
    }
    
    this -> receiver.setDescriptor(this -> sock);
    
    this -> receiver.initialize();

}

void SocketNPReceiver::receive(int choice, char * result){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    this -> receiver.receive(choice, result);
}

