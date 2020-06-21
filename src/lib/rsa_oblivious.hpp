
#ifndef _RSA_OBLIVIOUS
#define _RSA_OBLIVIOUS

#include "./rsa_model/MiddleMessage.h" 
#include "./rsa_model/StartMessage.h" 
#include "./rsa_model/EndMessage.h"
#include "./rapidjson/document.h"
#include "./rapidjson/writer.h"
#include "./rapidjson/stringbuffer.h"
#include "oblvious_util.hpp"
#include <stdio.h>
#include <string.h>
#include <gmp.h>

using namespace rapidjson;

namespace rsa_oblivious{
    
    int obliviousRSA(const char * m1, int m1Size,const char * m2, int m2Size, 
                char * cmd, StartMessage * startMessage, MiddleMessage * middleMessage, EndMessage * endMessage);
    
    int receive( char * cmd, char *& ptr, int choice, int & size,
                StartMessage * startMessage, MiddleMessage * middleMessage, EndMessage * endMessage);
                
    void convertGMPtoJSONvalue(Value * dst, Document * document, mpz_t& data);
    
    void convertJSONValueToGmp(Value * src, mpz_t& data);
    
    std::string serializeRSAStartMessage(StartMessage& data);
    
    void deserializeRSAStartMessage(StartMessage& data, std::string& json_string);
    
    std::string serializeRSAMidleMessage(MiddleMessage& data);
    
    void deserializeRSAMiddleMessage(MiddleMessage& data, std::string& json_string);
    
    std::string serializeRSAEndMessage(EndMessage& data);
    
    void deserializeRSAEndMessage(EndMessage& data, std::string& json_string);
}


#endif