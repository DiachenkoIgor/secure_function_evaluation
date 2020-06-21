#include "./sfe_lib.hpp"

    extern "C" char * sfe_js::calculateGateJS(char * cmd, char * gateName, int inputV,
                                    char * middleMessageJson, char * endMessageJson, char * gateJSON){
        std::string jsonMessage;
        bool result;
        JS_Answer answer;
        bool in = true;
                
        if(strcmp(cmd, "garbled") == 0){
            
            if(inputV != 0 && inputV != 1) {

                answer.execute = -1;
        
                auto json = serializeJS_Answer(answer);
                char cstr[json.length() + 1];
                sfe_js::stringToCharArr(json, cstr);
            
                return cstr;
            };
        
            bool input = inputV == 1 ? true : false;
            
            int execute = sfe::calculateGate("garbled", gateName, &input, nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);
            
            answer.jsonMessage.assign(jsonMessage);
            answer.execute = execute;
            
            in = false;
        }
        
        if(strcmp(cmd, "oblivious_start") == 0){
        
            int execute = sfe::calculateGate("oblivious_start", nullptr, nullptr,  nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);
            answer.jsonMessage.assign(jsonMessage);
            answer.execute = execute;
            
            in = false;
        }
        
        if(strcmp(cmd, "oblivious_end") == 0){
            
            if(middleMessageJson == nullptr){
                answer.execute = -1;
        
                auto json = serializeJS_Answer(answer);
                char cstr[json.length() + 1];
                sfe_js::stringToCharArr(json, cstr);
            
                return cstr;
            }
            
            std::string middleMessage(middleMessageJson);
            int execute = sfe::calculateGate("oblivious_end", nullptr, nullptr,  nullptr, &middleMessage, nullptr, nullptr, result, &jsonMessage);
        
            answer.jsonMessage.assign(jsonMessage);
            answer.execute = execute;
            
            in = false;
        }
        
        if(strcmp(cmd, "end") == 0){
            GateResult gateResult(Gate::LABEL_SIZE);
            std::string gateMessage(middleMessageJson);
            sfe::deserializeGateResult(gateResult, gateMessage);
            
            int execute = sfe::calculateGate("end", nullptr, nullptr, nullptr, nullptr, nullptr, &gateResult, result, &jsonMessage);
            
            answer.execute = execute;
            answer.res = result;
            in = false;
        }
        if(in == true){
            answer.execute = -1;
        }
        
        auto json = serializeJS_Answer(answer);
        char cstr[json.length() + 1];
        sfe_js::stringToCharArr(json, cstr);
            
        return cstr;

    }
    
    
     extern "C" char * sfe_js::calculateValueJS(char * cmd, int compareValue , char * garbledTableJson, char * startMessageJson,
                                char * endMessageJson){
        std::string jsonMessage;
        static int size;
        bool in = true;
        JS_Answer answer;
        
        if(strcmp(cmd, "garbled") == 0){
            std::string garbledTable(garbledTableJson);
            
            int execute = sfe::calculateValue("garbled", nullptr, &garbledTable, nullptr, nullptr, nullptr);
            
            answer.execute = execute;
            
            in = false;
            
        }
        
        if(strcmp(cmd, "oblivious_start") == 0){

             if(compareValue != 0 && compareValue != 1) {

                answer.execute = -1;
        
                auto json = serializeJS_Answer(answer);
                char cstr[json.length() + 1];
                sfe_js::stringToCharArr(json, cstr);
            
                return cstr;
            };

            bool input = compareValue == 1 ? true : false;
            std::string startMessage(startMessageJson);
            
            int execute = sfe::calculateValue("oblivious_start", &input, nullptr, &startMessage, nullptr, &jsonMessage);

            answer.jsonMessage.assign(jsonMessage);
            answer.execute = execute;
            
            
            in = false;
        }
        
        if(strcmp(cmd, "end") == 0){
            
            if(compareValue != 0 && compareValue != 1) {

                answer.execute = -1;
        
                auto json = serializeJS_Answer(answer);
                char cstr[json.length() + 1];
                sfe_js::stringToCharArr(json, cstr);
            
                return cstr;
            };
            
            bool input = compareValue == 1 ? true : false;
            std::string endMessage(endMessageJson);
            
            int execute = sfe::calculateValue("end", &input, nullptr, nullptr, &endMessage, &jsonMessage);
            
            answer.jsonMessage.assign(jsonMessage);
            answer.execute = execute;
            
            in = false;
        }
        
        if(in == true){
            answer.execute = -1;
        }
        
        auto json = serializeJS_Answer(answer);
        char cstr[json.length() + 1];
        sfe_js::stringToCharArr(json, cstr);
            
        return cstr;
    }
    
    void sfe_js::stringToCharArr(std::string & data, char * ptr){
        strcpy(ptr, data.c_str());
    }
    
     std::string sfe_js::serializeJS_Answer(JS_Answer& data){
        Document d; 
        d.SetObject();
        
        Value res(data.res);
        d.AddMember("res", res, d.GetAllocator());
        
        if(!data.jsonMessage.empty()){
            Value jsonMessage;
            jsonMessage.SetString(data.jsonMessage.c_str(),data.jsonMessage.size() , d.GetAllocator());
            d.AddMember("jsonMessage", jsonMessage, d.GetAllocator());
        }
        
        Value execute(data.execute);
        d.AddMember("execute", execute, d.GetAllocator());
        
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
    }
    