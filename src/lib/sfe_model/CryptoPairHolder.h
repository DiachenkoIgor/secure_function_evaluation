#ifndef CRYPTOPAIRHOLDER_H
#define CRYPTOPAIRHOLDER_H
#include <stdio.h>
#include <iostream>
#include <cstring>

class CryptoPairHolder
{


public:
    bool encrypted;

    char* cipher;
    int cipher_size;
    char* hashA;
    int hashA_size;
    char* hashB;
    int hashB_size;

    CryptoPairHolder();
    CryptoPairHolder(const CryptoPairHolder &src);
    ~CryptoPairHolder();
    
    void swap(CryptoPairHolder* src, CryptoPairHolder* dst){
        dst -> init(src -> cipher_size, src -> hashA_size, src -> hashB_size);
        dst -> setCipher(src -> cipher);
        dst -> setHashA(src -> hashA);
        dst -> setHashB(src -> hashB);
    }
    
     CryptoPairHolder& operator=(CryptoPairHolder other)
    {
         swap(&other, this);
         return *this;
    }
    
    void reInitCipher(char * cipher, int cipher_size);
    
    
    void init(int cipher_size, int hashA_size, int hashB_size);
    
    void copy(CryptoPairHolder& dst);

    void getCipher(char* dst)
    {
        std::memcpy(dst, this->cipher, this->cipher_size);
    }
    
     void setCipher(char* src)
    {
        std::memcpy(this->cipher, src, this->cipher_size);
    }

    void getHashA(char* dst)
    {
        std::memcpy(dst, this->hashA, this->hashA_size);
    }
    
    void setHashA(char* src)
    {
        std::memcpy(this->hashA, src, this->hashA_size);
    }

    void getHashB(char* dst)
    {
        std::memcpy(dst, this->hashB, this->hashB_size);
    }
    
     void setHashB(char* src)
    {
        std::memcpy(this->hashB, src, this->hashB_size);
    }

    int GetCipherSize() const
    {
        return cipher_size;
    }
    int GetHashASize() const
    {
        return hashA_size;
    }
    int GetHashBSize() const
    {
        return hashB_size;
    }
};

#endif // CRYPTOPAIRHOLDER_H
