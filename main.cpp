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
#include "NaoriPinkasReceiverData.h"
#include "NaoriPinkasSender.h"
#include "NaoriPinkasReceiver.h"
#include "./src/SimpleLogging.h"
#include "./src/SocketNPSender.h"
#include "./src/SocketNPReceiver.h"

using json = nlohmann::json;
bool SimpleLogging::flag = true;


void sender(){
    
    SocketNPSender sender {6758, "/home/idiachen/game/keys3.txt"};
    std::cout << "Start server" << std::endl;

     std::cout << "End Server initialize" << std::endl;
     std::string m1 {"Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1123"};
    std::string m2 {"Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2"};
        sender.start(m1.size());
    sender.sendMessage((char *)m1.c_str(), m1.size(),(char *) m2.c_str(), m2.size());
    std::cout << "Server sended" << std::endl;
    
}

void receiver(){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string addr {"127.0.0.1"};
    SocketNPReceiver receiver {6758, addr};
    
    std::cout << "Start client" << std::endl;
    receiver.initialize();
    std::cout << "End client initialize"  << std::endl;

     char result[receiver.getMsgSize()+1];
    result[receiver.getMsgSize()] = '\0';

    receiver.receive(0,result);
    std::cout << "End receiving" << std::endl;
     std::cout<< result <<std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

}

int main(int argc, char **argv)
{

    std::thread sen(sender);
    std::thread rec(receiver);
    

    if (rec.joinable()) {
    rec.join();
    }


        return 0;
   
}
