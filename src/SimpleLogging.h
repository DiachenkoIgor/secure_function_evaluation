#ifndef SIMPLELOGGING_H
#define SIMPLELOGGING_H
#include <string>
#include <iostream>
class SimpleLogging
{
public:
    static bool flag;
    SimpleLogging();
    ~SimpleLogging();
    
    static void LOG(std::string data);

};

#endif // SIMPLELOGGING_H
