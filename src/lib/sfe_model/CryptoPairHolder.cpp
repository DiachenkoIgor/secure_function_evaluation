#include "CryptoPairHolder.h"

CryptoPairHolder::CryptoPairHolder()
{
}

CryptoPairHolder::CryptoPairHolder(const CryptoPairHolder& src){
    swap((CryptoPairHolder *) &src, this);
}
CryptoPairHolder::~CryptoPairHolder()
{
    if(this -> cipher == nullptr) return;
    delete (this -> cipher);
    delete (this -> hashA);
    delete (this -> hashB);
}

void CryptoPairHolder::reInitCipher(char * cipher, int cipher_size){
    delete (this -> cipher);
    this -> cipher_size = cipher_size;
    this -> cipher = new char[cipher_size];
    
    this -> setCipher(cipher);
}

void CryptoPairHolder::init(int cipher_size, int hashA_size, int hashB_size){
    this -> cipher_size = cipher_size;
    this -> hashA_size = hashA_size;
    this -> hashB_size = hashB_size;
    
    this -> cipher = new char[cipher_size];
    this -> hashA = new char[hashA_size];
    this -> hashB = new char[hashB_size];
}

void CryptoPairHolder::copy(CryptoPairHolder& dst){
    dst.init(this -> cipher_size, this -> hashA_size, this -> hashB_size);
    dst.setCipher(this -> cipher);
    dst.setHashA(this -> hashA);
    dst.setHashB(this -> hashB);
}

