#include "SFEEvaluator.h"

SFEEvaluator::SFEEvaluator()
{

}

SFEEvaluator::~SFEEvaluator()
{
}

void SFEEvaluator::calculate(bool * compareValue, int size){
    
    for(int i = 0; i < size; i++){
        calculateValue(compareValue[i]);
    }

}

void SFEEvaluator::calculateValue(bool compareValue){
        std::string jsonMessage;
    

        std::string garbledTableJson = SFEUtil::readJSON(this -> descriptor);

        sfe::calculateValue("garbled", nullptr, &garbledTableJson, nullptr, nullptr, nullptr);
        
        
    
        std::string startMessageJson = SFEUtil::readJSON(this -> descriptor);
        
        sfe::calculateValue("oblivious_start", &compareValue, nullptr, &startMessageJson, nullptr, &jsonMessage);
        
        SFEUtil::socketSend(this -> descriptor, jsonMessage);
        
        
    
        std::string endMessageJson = SFEUtil::readJSON(this -> descriptor);
        
        sfe::calculateValue("end", &compareValue, nullptr, nullptr, &endMessageJson, &jsonMessage);

        SFEUtil::socketSend(this -> descriptor, jsonMessage);
     
    }

