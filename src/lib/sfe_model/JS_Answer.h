#ifndef JS_ANSWER_H
#define JS_ANSWER_H
#include <stdio.h>
#include <iostream>
#include <cstring>
class JS_Answer
{
public:

    bool res;
    std::string jsonMessage;
    int execute;

    JS_Answer();
    ~JS_Answer();

};

#endif // JS_ANSWER_H
