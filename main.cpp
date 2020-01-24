/*#include <stdio.h>
#include <gmp.h>
#include <iostream>
#include <sodium.h>
#include <chrono>
#include <string>
#include "NaoriPinkasSenderData.h"
#include <math.h> */
#include "./src/JSON/json.hpp"
#include <iostream>
#include <thread>
#include "NaoriPinkasSenderData.h"
#include "NaoriPinkasSender.h"
#include "NaoriPinkasReceiver.h"

using json = nlohmann::json;


void sender(){
    std::string path {"/home/idiachen/keys1.txt"};
    NaoriPinkasSender sender {path, 3022};
    
    std::string m1 {"Hello1"};
    std::string m2 {"Hello2"};
    
    sender.messageByteLength = m1.size();
    std::cout << "Start server" << std::endl;
    sender.initialize();
    std::cout << "End Server initialize" << std::endl;
    sender.sendMessages((char *)m1.c_str(), m1.size(),(char *) m2.c_str(), m2.size());
    std::cout << "Server sended" << std::endl;
    
}

void receiver(){
    std::string addr {"127.0.0.1"};
    NaoriPinkasReceiver receiver {3022,addr};
    std::cout << "Start client" << std::endl;
    receiver.initialize();
    std::cout << "End client initialize" << std::endl;
    char result[receiver.getMsgSize()];
    
    receiver.receive(1,result);
    std::cout << "End receiving" << std::endl;
    std::cout << result << std::endl;
}

int main(int argc, char **argv)
{
    std::thread sen(sender);
   std::thread rec(receiver);
    
   // rec.join();
    sen.join();
}
