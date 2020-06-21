#ifndef OBLIVIOUSUTILS_H
#define OBLIVIOUSUTILS_H
#include <stdio.h>
#include <gmp.h>
#include <sodium.h>
#include <math.h> 
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "../np_model/NaoriPinkasReceiverData.h"
#include "../np_model/ReceiverPublicKey.h"
#include "../np_model/NaoriPinkasTransfer.h"
#include "./src/JSON/json.hpp"
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 

using json = nlohmann::json;

class NaoriPinkasReceiverData;

class ObliviousUtils
{

public:
    ObliviousUtils();
    ~ObliviousUtils();
    
    static void generateRandomNumber(mpz_t &place, int bytesSize);
    
    static void generateRandomByteArray(char * arr,int arrSize);

    static void generatePrimeRandomNumber(mpz_t &place, int bytesSize);
    
    static int exportGmpNumberToByteArray(mpz_t &data, char *& ptr);
    
    static void importGmpNumberFromByteArray(mpz_t &data, const char * ptr, int size);
    
    static void generateHash(const unsigned char * data[], int quantity, int sizes[], unsigned char * out, int outLength);
    
    static void keyDerevationFunction(unsigned char * out, int outLength, const char * key, int keyLength);
    
    static void encrypt(mpz_t &result, mpz_t &key, mpz_t &R, mpz_t &msg, int msgLengthBytes);
    
    static void decrypt(mpz_t &result, mpz_t &key, mpz_t &R, mpz_t &cph, int cphLengthBytes);

    static int bit_size(mpz_t &number);

    static std::string serializeNPReceiverData(NaoriPinkasReceiverData& data);
    
    static void deserializeNPReceiverData(NaoriPinkasReceiverData& data, std::string& json);
    
    static std::string serializeNPReceiverPublicKey(ReceiverPublicKey& data);
    
    static void deserializeNPReceiverPublicKey(ReceiverPublicKey& data, std::string& json);
    
    static std::string serializeNPTransfer(NaoriPinkasTransfer& data);
    
    static void deserializeNPTransfer(NaoriPinkasTransfer& data, std::string& json);
    
    static std::string readJSON(int descriptor);
    
    static void convertArrayToVector(std::vector<char>& holder, char * ptr, int size);
    
    static void convertVectorToArray(std::vector<char>& holder, char * ptr, int size);
  
    static void convertGMPtoVector(std::vector<char>& holder, mpz_t& data);

    static void convertVectorToGmp(std::vector<char>& holder, mpz_t& data);
    
    static void testAnalyzeSender(char * preffix, mpz_t& value);
    
    static void testAnalyzeReceiver(char * preffix, mpz_t& value);
    
    
    

};

#endif // OBLIVIOUSUTILS_H
