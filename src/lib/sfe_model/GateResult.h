#ifndef GATERESULT_H
#define GATERESULT_H
#include <stdio.h>
#include <iostream>
#include <cstring>
class GateResult
{
public:
    char * value;
    int value_size;

    GateResult(int value_size);
    ~GateResult();
    
    void setValue(char * src){
        std::memcpy(this -> value, src, value_size);
    }
    void getValue(char * dst){
        std::memcpy(dst, this -> value, value_size);
    }

};

#endif // GATERESULT_H
