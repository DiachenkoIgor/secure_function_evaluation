#include "XorGate.h"

XorGate::XorGate()
{
     calculateTable();
    encodeTable();
}

XorGate::~XorGate()
{
}

void XorGate::calculateTable(){
    for(int i = 0; i < 4; i++)
        this -> table[i][2] = this -> table[i][0] ^ this -> table[i][1];
}

