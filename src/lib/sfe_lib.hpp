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
#include "./sfe_model/JS_Answer.h"
#include "./rapidjson/document.h"
#include "./rapidjson/writer.h"
#include "./rapidjson/stringbuffer.h"
#include "./sfe.hpp"

namespace sfe_js{
    
    extern "C" char * calculateGateJS(char * cmd, char * gateName, int inputV,
                                    char * middleMessageJson, char * endMessageJson, char * gateJSON);
    
    void stringToCharArr(std::string & data, char * ptr);
    
    std::string serializeJS_Answer(JS_Answer& data);
    
    extern "C" char * calculateValueJS(char * cmd, int compareValue , char * garbledTableJson, char * startMessageJson,
                                char * endMessageJson);
    
}