#ifndef GATE_H
#define GATE_H
#include <iostream>

#include "../oblvious_util.hpp"
#include <cstring>
class Gate
{
protected:
     static const int MAC_SIZE = crypto_secretbox_MACBYTES;


    bool table[4][3];

    bool input;
    bool input2;
    bool inputChoice;
    bool output;
    
    void initTable();

    void createLable();
    
    void encodeTable();
    
    virtual void calculateTable();
    
public:

    static const int LABEL_SIZE = crypto_secretbox_KEYBYTES;
    static const int LABEL_SIZE_MAC = crypto_secretbox_KEYBYTES + crypto_secretbox_MACBYTES;
    static const int ENCODE_SIZE = crypto_secretbox_KEYBYTES + crypto_secretbox_MACBYTES + crypto_secretbox_MACBYTES;
    static const int NONCE_SIZE = crypto_secretbox_NONCEBYTES;

    char * encodeResult[4];
    char nonceA[4][NONCE_SIZE];
    char nonceB[4][NONCE_SIZE];
    char lables[6][crypto_secretbox_KEYBYTES];
    
    Gate* next;
    Gate* out;
    
    bool calculated;
    

    Gate();
    virtual ~Gate();
    
    void setInput(bool in){
        this -> input = in;
    }
    
    bool getInput(){
        return this -> input;
    }
    
    void setInput2(bool in){
        this -> input2 = in;
    }
    
    void setInputChoice(bool in){
        this -> inputChoice = in;
    }
    
    void setOutput(bool out){
        this -> output = out;
    }
    
    bool getOutput(){
        return this -> output;
    }
    
    void nextGate(Gate * nextGate){
        this -> next = nextGate;
    }
    
     void outGate(Gate * outGate){
        this -> out = outGate;
    }
    
    void result(){
        if(this -> next == nullptr) return;
        if(this -> inputChoice)
            this -> out -> setInput2(this -> output);
        else
            this -> out -> setInput(this -> output);
    }
    
     void calculate(){
        if(!calculated) return;
        for(int i = 0; i < 4; i++){
            if(this -> table[i][0] == this -> input && this -> table[i][1] == this -> input2)
                this -> output = this -> table[i][2];
        }
    }
    
    
    void getInputLabel(char * dst){
        if(this -> input)
             std::memcpy(dst, this -> lables[1], this -> LABEL_SIZE);
        else
             std::memcpy(dst, this -> lables[0], this -> LABEL_SIZE);
    }
    
   

};

#endif // GATE_H
