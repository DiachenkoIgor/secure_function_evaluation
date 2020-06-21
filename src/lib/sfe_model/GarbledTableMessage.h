#ifndef GARBLEDTABLEMESSAGE_H
#define GARBLEDTABLEMESSAGE_H
#include <sodium.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <string.h>
#include "CryptoPairHolder.h"

class GarbledTableMessage
{
    
public:
    const static int LABEL_SIZE = crypto_secretbox_KEYBYTES;; 
    
    CryptoPairHolder holders[4];
    char lable[LABEL_SIZE];

    GarbledTableMessage();
    ~GarbledTableMessage();
    
    
    void init(std::vector<CryptoPairHolder>& source){
        for(int i =0; i < 4; i++)
            source.at(i).copy(this -> holders[i]);
    }
    
    void setLable(char * src){
        std::memcpy(this -> lable, src, LABEL_SIZE);
    }

};

#endif // GARBLEDTABLEMESSAGE_H
