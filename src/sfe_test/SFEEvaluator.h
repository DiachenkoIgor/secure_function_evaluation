#ifndef SFEEVALUATOR_H
#define SFEEVALUATOR_H
#include "../lib/rsa_oblivious.hpp"
#include "../lib/oblvious_util.hpp"
#include "../lib/sfe.hpp" 
#include "../lib/sfe_model/CryptoPairHolder.h"
#include "../lib/rsa_model/MiddleMessage.h" 
#include "../lib/rsa_model/StartMessage.h" 
#include "../lib/rsa_model/EndMessage.h" 
#include "SFEUtil.h"
#include <iostream>
#include <thread>
#include <vector>

class SFEEvaluator
{
private:
    void calculateValue(bool compareValue);
public:
    SFEEvaluator();
    ~SFEEvaluator();
    int descriptor;

    void calculate(bool * compareValue, int size);
};

#endif // SFEEVALUATOR_H
