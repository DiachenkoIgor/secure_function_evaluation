#include "AndGate.h"

AndGate::AndGate()
{
     calculateTable();
    encodeTable();
}

AndGate::~AndGate()
{
}

void AndGate::calculateTable(){
    for(int i = 0; i < 4; i++)
        this -> table[i][2] = this -> table[i][0] & this -> table[i][1];
}

