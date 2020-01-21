#include "NaoriPinkasTransfer.h"

NaoriPinkasTransfer::NaoriPinkasTransfer()
{
}

NaoriPinkasTransfer::~NaoriPinkasTransfer()
{
    delete [] m1;
    delete [] m2;
    delete [] R;
}

NaoriPinkasTransfer::NaoriPinkasTransfer(char* m1, int m1Size,
                        char* m2, int m2Size,
                        char* R, int RSize){
        this -> m1 = new char [m1Size];
        std::memcpy(this -> m1, m1, m1Size * sizeof(char));
        this -> m1Size = m1Size;
        
        this -> m2 = new char [m2Size];
        std::memcpy(this -> m2, m2, m2Size * sizeof(char));
        this -> m2Size = m2Size;
        
        this -> R = new char [RSize];
        std::memcpy(this -> R, R, RSize * sizeof(char));
        this -> RSize = RSize;
        
                            }
                            
void NaoriPinkasTransfer::checkAndCleanArray(char * ptr){
    if(ptr == nullptr) return;
    
    delete [] ptr;
    ptr = nullptr;
}

void NaoriPinkasTransfer::setM1(char* m1, size_t m1Size){
        checkAndCleanArray(this -> m1);
        this -> m1 = new char[m1Size];
        
        std::memcpy(this -> m1, m1, m1Size * sizeof(char));
        
        this -> m1Size = m1Size;
}

void NaoriPinkasTransfer::setM2(char* m2, size_t m2Size){
        checkAndCleanArray(this -> m2);
        this -> m2 = new char[m2Size];
        
        std::memcpy(this -> m2, m2, m2Size * sizeof(char));
        
        this -> m2Size = m2Size;
}

void NaoriPinkasTransfer::setR(char* R, size_t RSize){
        checkAndCleanArray(this -> R);
        this -> R = new char[RSize];
        
        std::memcpy(this -> R, R, RSize * sizeof(char));
        
        this -> RSize = RSize;
}







