#ifndef ORGATE_H
#define ORGATE_H
#include "Gate.h"
class OrGate : public Gate
{
private:
    void calculateTable();
public:
    OrGate();
    ~OrGate();

};

#endif // ORGATE_H
