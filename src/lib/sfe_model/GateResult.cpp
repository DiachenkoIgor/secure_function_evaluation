#include "GateResult.h"

GateResult::GateResult(int value_size)
{
    this -> value = new char[value_size];
    this -> value_size = value_size;
}

GateResult::~GateResult()
{
    delete [] (this -> value);
}

