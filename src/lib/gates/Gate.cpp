#include "Gate.h"
void Gate::initTable(){
    this -> table[0][0] = false;
    this -> table[0][1] = false;
    this -> table[0][2] = false;
    this -> table[1][0] = false;
    this -> table[1][1] = true;
    this -> table[1][2] = false;
    this -> table[2][0] = true;
    this -> table[2][1] = false;
    this -> table[2][2] = false;
    this -> table[3][0] = true;
    this -> table[3][1] = true;
    this -> table[3][2] = false;
}

void Gate::createLable(){
    oblivious_util::generateSecretKey(this -> lables[0]);
    oblivious_util::generateSecretKey(this -> lables[1]);
    oblivious_util::generateSecretKey(this -> lables[2]);
    oblivious_util::generateSecretKey(this -> lables[3]);
    oblivious_util::generateSecretKey(this -> lables[4]);
    oblivious_util::generateSecretKey(this -> lables[5]);
}

void Gate::encodeTable(){
    
    char nonceTmp[this -> NONCE_SIZE];
    

    for(int i = 0; i < 4; i++){
        
        this -> encodeResult[i] = new char[this -> LABEL_SIZE];
        
        if(this -> table[i][2]){
            std::memcpy(this -> encodeResult[i], this -> lables[5], this -> LABEL_SIZE);
        }
        else{
            std::memcpy(this -> encodeResult[i], this -> lables[4], this -> LABEL_SIZE);
        }
        
            
    }

    for(int i = 0; i < 4; i++){
        char * key = this -> table[i][1] ? this -> lables[3] : this -> lables[2];

        oblivious_util::generateSecretNonce(nonceTmp, this -> NONCE_SIZE);
        
        std::memcpy(this -> nonceB[i], nonceTmp, this -> NONCE_SIZE);
        
        char tmp[this -> LABEL_SIZE];
        std::memcpy(tmp, this -> encodeResult[i], this -> LABEL_SIZE);
        
        delete (this -> encodeResult[i]);
        this -> encodeResult[i] = new char [this -> LABEL_SIZE_MAC];
        
        oblivious_util::secretEncrypt(this -> encodeResult[i], tmp, this -> LABEL_SIZE, nonceB[i], key);

    }

      for(int i = 0; i < 4; i++){
        char * key = this -> table[i][0] ? this -> lables[1] : this -> lables[0];
        
        oblivious_util::generateSecretNonce(nonceTmp, this -> NONCE_SIZE);
        
        std::memcpy(this -> nonceA[i], nonceTmp, this -> NONCE_SIZE);
        
        char tmp[this -> LABEL_SIZE_MAC];
        std::memcpy(tmp, this -> encodeResult[i], this -> LABEL_SIZE_MAC);
        
        delete (this -> encodeResult[i]);
        this -> encodeResult[i] = new char [this -> LABEL_SIZE_MAC + this -> MAC_SIZE];
        
        oblivious_util::secretEncrypt(this -> encodeResult[i], tmp, this -> LABEL_SIZE_MAC, nonceA[i], key);
    }
    
    
}
Gate::Gate()
{
    initTable();
    createLable();
}



Gate::~Gate()
{
    for(int i = 0; i < 4; i++){
        if(this -> encodeResult[i] != nullptr);
        delete [] (this -> encodeResult[i]);
        
    }
}

void Gate::calculateTable(){}
