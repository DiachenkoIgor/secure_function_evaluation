#ifndef NAORIPINKASTRANSFER_H
#define NAORIPINKASTRANSFER_H
#include <cstring>
#include <iostream> 

class NaoriPinkasTransfer
{
private:
    char* m1;
    char* m2;
    char* R;

    int m1Size;
    int m2Size;
    int RSize;

    void checkAndCleanArray(char* ptr);

public:
    NaoriPinkasTransfer();
    NaoriPinkasTransfer(char* m1, int m1Size, char* m2, int m2Size, char* R, int RSize);
    ~NaoriPinkasTransfer();

    void setM1(char* m1, size_t m1Size);
    void setM2(char* m2, size_t m2Size);
    void setR(char* R, size_t RSize);
    
    char const * const getM1(){
        return (char const * const) m1;
    }
    
    char const * const getM2(){
        return (char const * const) m2;
    }
    
    
    char const * const getR(){
        return (char const * const) R;
    }
    

    int GetRSize() const
    {
        return RSize;
    }
    int GetM1Size() const
    {
        return m1Size;
    }
    int GetM2Size() const
    {
        return m2Size;
    }
};

#endif // NAORIPINKASTRANSFER_H
