#ifndef _SFE
#define _SFE
#include "oblvious_util.hpp"
#include "rsa_oblivious.hpp"
#include "./gates/Gate.h"
#include "./sfe_model/CryptoPairHolder.h"
#include "./rsa_model/MiddleMessage.h" 
#include "./rsa_model/StartMessage.h" 
#include "./rsa_model/EndMessage.h" 
#include <typeinfo>
#include "./gates/AndGate.h"
#include "./gates/OrGate.h"
#include "./gates/XorGate.h"
#include "./sfe_model/GarbledTableMessage.h"
#include "./sfe_model/GateResult.h"
#include "./rapidjson/document.h"
#include "./rapidjson/writer.h"
#include "./rapidjson/stringbuffer.h"
namespace sfe{
    
    extern "C" int calculateGate(const char * cmd, char * gateName, bool * input, 
            std::string * startMessageJson, std::string * middleMessageJson, std::string * endMessageJson,
            GateResult * gateResult, bool & res, std::string * jsonMessage);
    
    void convertArraytoJSONvalue(Value * dst, Document * document, char * ptr, int size);
    
    void serializeCryptoPairHolderToJSONValue(Value * src1, Document * document, CryptoPairHolder& data);
    
    std::string serializeGarbledTableMessage(GarbledTableMessage& data);
    
    std::string serializeGateResult(GateResult& data);
    
    void deserializeCryptoPairHolder(CryptoPairHolder& dst, Value * src1);
    
    void deserializeGarbledTableMessage(GarbledTableMessage& dst, std::string& json_string);
     
    void deserializeGateResult(GateResult& dst, std::string& json_string);
    
    int calculateValue(char * cmd, bool * compareValue , std::string * garbledTableJson, std::string * startMessageJson,
                                std::string * endMessageJson, std::string * jsonMessage);

}

#endif