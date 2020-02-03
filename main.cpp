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

using json = nlohmann::json;


void sender(){
    std::string path {"/home/idiachen/keys1.txt"};
    NaoriPinkasSender sender {path, 3028};
    
    std::string m1 {"Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1Hello1123"};
    std::string m2 {"Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2Hello2"};
    
    sender.messageByteLength = m1.size();
    std::cout << "Start server" << std::endl;
    sender.initialize();
    std::cout << "End Server initialize" << std::endl;
    sender.sendMessages((char *)m1.c_str(), m1.size(),(char *) m2.c_str(), m2.size());
    std::cout << "Server sended" << std::endl;
    
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
}

void receiver(){
    std::string addr {"127.0.0.1"};
    NaoriPinkasReceiver receiver {3028,addr};
    std::cout << "Start client" << std::endl;
    receiver.initialize();
    std::cout << "End client initialize"  << std::endl;
    char result[receiver.getMsgSize()+1];
    result[receiver.getMsgSize()] = '\0';
    
    receiver.receive(0,result);
   // std::this_thread::sleep_for(std::chrono::milliseconds(20000));
    std::cout << "End receiving" << std::endl;
    std::cout << result << std::endl;
}

int main(int argc, char **argv)
{
    

    std::thread sen(sender);
   std::thread rec(receiver);
    

    if (rec.joinable()) {
    rec.join();
    }
     if (sen.joinable()) {
         
    sen.join();
    }
    
    
    
    return 0;
    


        
        
    /*  std::string path {"/home/idiachen/keys1.txt"};
      NaoriPinkasSenderData data;
      NaoriPinkasReceiverData receiverData;
      NaoriPinkasSenderData::readInitDataFromFile(path, data);
      
      mpz_set(receiverData.C ,data.C);
    mpz_set(receiverData.g , data.g);
    mpz_set(receiverData.gr , data.gr);
    mpz_set(receiverData.p , data.p);
    mpz_set(receiverData.q ,  data.q);
    receiverData.msgByteLength = 11;
    
    auto t = ObliviousUtils::serializeNPReceiverData(receiverData);
      std::cout << t << std:: endl;
      
      mpz_t mpz_tmp;
    mpz_init(mpz_tmp);
    
    json tmp = json::parse(t);
    std::vector<char> tmpHolder;
    
    tmpHolder = tmp["q"].get<std::vector<char>>();
    
    for(int i =0; i < tmpHolder.size(); i ++)
        std::cout << (int)tmpHolder.at(i) << std::endl;
        
         mpz_t test;
    mpz_init(test);
    
    int size = tmpHolder.size();
    
    
    
    ObliviousUtils::importGmpNumberFromByteArray(test, tmpHolder.data(), size);
    
    gmp_printf ("%Zd\n", test);*/
}
