#ifndef ANDGATE_H
#define ANDGATE_H
#include "Gate.h"
class AndGate : public Gate
{
private:

    void calculateTable();

public:
    AndGate();
    ~AndGate();
    
    

};

#endif // ANDGATE_H
