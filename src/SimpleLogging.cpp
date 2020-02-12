#include "SimpleLogging.h"

SimpleLogging::SimpleLogging()
{
}

SimpleLogging::~SimpleLogging()
{
}

void SimpleLogging::LOG(std::string data){
    if(SimpleLogging::flag)
        std::cout << data << std::endl;
}

