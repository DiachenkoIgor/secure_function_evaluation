
#include "./src/JSON/json.hpp"
#include "./src/lib/gates/Gate.h"
#include "./src/lib/gates/XorGate.h"
#include "./src/lib/gates/OrGate.h"
#include "./src/lib/gates/AndGate.h"
#include "./src/sfe_test/SFEEvaluator.h"
#include "./src/sfe_test/SFEConstructor.h"
#include "./src/JSON/json.hpp"
#include <iostream>
#include <thread>
#include <string.h>
#include <unistd.h> 
#include <vector> 
#include <stdio.h>  
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 


using json = nlohmann::json;



void initServerAndAccept(int &server, int &socketVar, int port){
    struct sockaddr_in address;
    int server_fd, new_socket;
    int addrlen = sizeof(address);
    int opt = 1; 
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE); 
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        std::cout << "setsockopt" << std::endl;
        exit(EXIT_FAILURE); 
    } 
    
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port );

     if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    {   std::cout << "bind failed" << std::endl; 
        exit(EXIT_FAILURE); 
    }
    
    if (listen(server_fd, 3) < 0) 
    { 
        std::cout << "listen" << std::endl;
        exit(EXIT_FAILURE); 
    }
    


     if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        std::cout << "accept" << std::endl;
        exit(EXIT_FAILURE); 
    }
    server = server_fd;
    socketVar = new_socket;
}

int initSocketClient(int & sock, int port, std::string address){

    struct sockaddr_in serv_addr; 

    if (( sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::cout << "Socket creation error" << std::endl;
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    } 
   
    if (connect( sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        std::cout << "Connection Failed" << std::endl;
        return -1;
    } 
}


void SFEConsTest(){

    
    int server=0;
    int socket = 0;
    
    initServerAndAccept(server, socket, 11113);
    
    int byt[8] = {1,0,1,1,1,0,0,1};
    
        XorGate xor1;
    xor1.setInput(byt[0] == 1 ? true : false);
    xor1.setInputChoice(true);
    xor1.calculated = false;
    XorGate xor2;
    xor2.setInput(byt[1] == 1 ? true : false);
    xor2.setInputChoice(false);
    xor2.calculated = false;
    
    OrGate startOr;
    startOr.calculated = true;
    startOr.setInputChoice(false);
    
    
    xor1.nextGate(&xor2);
    xor1.outGate(&startOr);
    xor2.nextGate(&startOr);
    xor2.outGate(&startOr);
    
    OrGate * next = &startOr;

    std::vector<Gate*> holder;
    
    for(int i =2; i< 8; i++ ){
        XorGate * xorTmp = new XorGate;
    xorTmp -> setInput(byt[i] == 1 ? true : false);
    xorTmp -> setInputChoice(true);
    
    
    OrGate * orTmp = new OrGate;
    orTmp -> calculated = true;
    orTmp -> setInputChoice(false);
    
    xorTmp -> nextGate(orTmp);
    xorTmp -> outGate(orTmp);
    
    holder.push_back(xorTmp);
    holder.push_back(orTmp);
    
    next -> nextGate(xorTmp);
    next -> outGate(orTmp);
    
    
    next = orTmp;
    }
    
    SFEConstructor constructor;
    constructor.descriptor = socket;
    
    constructor.evaluate(xor1);
    
    close(socket);
    close(server);
    
    for(int i =0; i< holder.size(); i++){
        delete holder.at(i);
    }
    
    std::cout << "SFEConsTest   -   " << next -> getOutput() << std::endl;
}

void SFEEvalTest(){
    int port = 11113;
    int sock =0;
    std::string adr = {"127.0.0.1"};
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    initSocketClient(sock, port, adr);
    
    int byt[8] = {1,0,1,1,1,0,0,1};
    
    
    
    bool arr[8];
    
    for(int i =0; i< 8; i++){
        arr[i] = byt[i] == 1 ? true : false;
    }
    
    SFEEvaluator eval;
    eval.descriptor = sock;
    
     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            
    eval.calculate(arr, 8);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "SFE time  " << time << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    close(sock);
}

int  SFETest(){
    
    std::thread sen(SFEConsTest);
    std::thread rec(SFEEvalTest);
    

    if (rec.joinable()) {
    rec.join();
    }


        return 0;
    
}

int main(int argc, char **argv){

    SFETest();

    return 0;

}

