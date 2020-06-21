#ifndef XORGATE_H
#define XORGATE_H
#include "Gate.h"
class XorGate : public Gate
{
private:
    void calculateTable();
    
public:
    XorGate();
    ~XorGate();

};

#endif // XORGATE_H
