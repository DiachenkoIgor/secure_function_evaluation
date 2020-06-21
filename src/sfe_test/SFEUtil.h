#ifndef SFEUTIL_H
#define SFEUTIL_H

#include "../lib/sfe_model/GarbledTableMessage.h"
#include <ctime>
#include <cstdlib>
#include "./src/JSON/json.hpp"
#include <vector>
#include "./src/JSON/json.hpp"
#include <algorithm>
#include <random>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include "../lib/sfe_model/GateResult.h"
using json = nlohmann::json;

class SFEUtil
{
public:
    SFEUtil();
    ~SFEUtil();
    
    static void convertArraytoVector(std::vector<char>& holder, char * src, int size);
    
    static void socketSend(int descriptor, std::string value);
    
    static std::string serializeGateResult(GateResult& data);
    
    static void deserializeGateResult(GateResult& dst, std::string& json_string);
    
    static std::string readJSON(int descriptor);
    
    static void convertVectorToArray(std::vector<char>& holder, char * dst);
    
    static void deserializeGarbledTableMessage(GarbledTableMessage& dst, std::string& json_string);
    
    static void deserializeCryptoPairHolder(CryptoPairHolder& dst, std::string& json_string);
    
    static json serializeCryptoPairHolder(CryptoPairHolder& data);


    static std::string serializeGarbledTableMessage(GarbledTableMessage& data);
    
};

#endif // SFEUTIL_H
