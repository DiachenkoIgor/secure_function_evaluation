#ifndef NaoriPinkasSenderData_H
#define NaoriPinkasSenderData_H
#include <stdio.h>
#include <gmp.h>
#include <sodium.h>
#include <string>
#include "ObliviousUtils.h"
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class NaoriPinkasSenderData
{
public:

         mpz_t p, q, g, C, r;
         mpz_t Cr, gr;
         //Bits
         int rLength;
         int qLength;
         int pLength;
         
         
    NaoriPinkasSenderData();
    NaoriPinkasSenderData(int rLength, 
                            int qLength, int pLength);
    ~NaoriPinkasSenderData();
    
    static void generateInitData(NaoriPinkasSenderData& holder);
    static void writeInitDataToFile(const std::string &path, NaoriPinkasSenderData& holder);
    static void readInitDataFromFile(const std::string &path, NaoriPinkasSenderData& holder);

};

#endif // NaoriPinkasSenderData_H
