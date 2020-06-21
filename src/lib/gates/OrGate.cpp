#include "OrGate.h"

OrGate::OrGate()
{
     calculateTable();
    encodeTable();
}

OrGate::~OrGate()
{
}

void OrGate::calculateTable(){
    for(int i = 0; i < 4; i++)
        this -> table[i][2] = this -> table[i][0] | this -> table[i][1];
}
