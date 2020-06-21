#include "SFEConstructor.h"

SFEConstructor::SFEConstructor()
{
}

SFEConstructor::~SFEConstructor()
{
}



bool SFEConstructor::evaluate(Gate &startGate){
    std::cout << "Start calculate in CONSTR" << std::endl;
    Gate *next = &startGate;
    
    while(next != nullptr){
        if(next -> calculated){
            next -> calculate();
            next -> result();
            
            if(next -> next == nullptr) break;
            
            next = next -> next;
            continue;
        }
        
       
        std::string gateName;
        
        if( typeid( *next ) == typeid( AndGate ) ){
            gateName.assign("and");
        }
        if( typeid( *next ) == typeid( OrGate ) ){
            gateName.assign("or");
        }
        if( typeid( *next ) == typeid( XorGate ) ){
            gateName.assign("xor");
        }
        
       bool result = evaluateGate((char *)gateName.c_str(), next -> getInput());
        
       next -> setOutput(result);
       
        next -> result();
        
        if(next -> next == nullptr) break;
        
        next = next -> next;
    }
    return next -> getOutput();
}

bool SFEConstructor::evaluateGate(char * gate, bool input){
    std::string jsonMessage;
    bool result;
    
    sfe::calculateGate("garbled", gate, &input, nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);

    SFEUtil::socketSend(this -> descriptor, jsonMessage);


    sfe::calculateGate("oblivious_start", nullptr, nullptr,  nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);
     
    SFEUtil::socketSend(this -> descriptor, jsonMessage);
    
    
    auto middleMessageJson = SFEUtil::readJSON(this -> descriptor);
    
    sfe::calculateGate("oblivious_end", nullptr, nullptr,  nullptr, &middleMessageJson, nullptr, nullptr, result, &jsonMessage);
    
    SFEUtil::socketSend(this -> descriptor, jsonMessage);
    
    
    auto gateJSON = SFEUtil::readJSON(this -> descriptor);

    GateResult gateResult(Gate::LABEL_SIZE);

    sfe::deserializeGateResult(gateResult, gateJSON);
    
    sfe::calculateGate("end", nullptr, nullptr, nullptr, nullptr, nullptr, &gateResult, result, &jsonMessage);
    
    return result;
}

