#ifndef SFECONSTRUCTOR_H
#define SFECONSTRUCTOR_H

#include "SFEUtil.h"
#include <iostream>
#include <thread>
#include <vector>
#include "../lib/rsa_model/MiddleMessage.h" 
#include "../lib/rsa_model/StartMessage.h" 
#include "../lib/rsa_model/EndMessage.h" 
#include <typeinfo>
#include "../lib/gates/AndGate.h"
#include "../lib/gates/OrGate.h"
#include "../lib/gates/XorGate.h"
#include "../lib/rsa_oblivious.hpp"
#include "../lib/sfe.hpp" 

class SFEConstructor
{
private:
    void calculateGate(Gate &gate);
    void createGarbledTable(Gate * gate, GarbledTableMessage &garbledTableMessage);
    bool evaluateGate(char * gate, bool input);
    int calculateGate(char * cmd, char * gateName, bool * input, GarbledTableMessage * garbledTableMessage, 
           StartMessage * startMessage, MiddleMessage * middleMessage, EndMessage * endMessage, GateResult * gateResult, bool & res);
public:
    
    int descriptor;


    SFEConstructor();
    ~SFEConstructor();
    
    bool evaluate(Gate &startGate);
    

};

#endif // SFECONSTRUCTOR_H
