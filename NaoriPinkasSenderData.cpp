#include "NaoriPinkasSenderData.h"

NaoriPinkasSenderData::NaoriPinkasSenderData() :
    NaoriPinkasSenderData::NaoriPinkasSenderData(50,512,15360)
{
    
}
NaoriPinkasSenderData::NaoriPinkasSenderData(int rLength, 
                                                int qLength, int pLength) :
                                                        rLength {rLength},
                                                        qLength {qLength},
                                                        pLength {pLength}{
        mpz_inits(p, q, g, C, r, Cr, gr, NULL);                                            
}

NaoriPinkasSenderData::~NaoriPinkasSenderData()
{
    mpz_clears(p, q, g, C, r, Cr, gr, NULL); 
}

void NaoriPinkasSenderData::generateInitData(NaoriPinkasSenderData& holder){
    
    mpz_t tmp, tmp2, pdq;
    mpz_inits(tmp,tmp2, pdq, NULL);
    std::cout << "Start generate Data!" << std::endl;
    
    ObliviousUtils::generatePrimeRandomNumber(holder.q, holder.qLength / 8);
    
    int isPrime=0;
    
     do{
        ObliviousUtils::generateRandomNumber(pdq,(holder.pLength - holder.qLength)/8);
        mpz_clrbit(pdq, 0);
        mpz_mul(tmp, holder.q, pdq);
        mpz_add_ui(holder.p, tmp, 1);
        isPrime = mpz_probab_prime_p(holder.p, 20);
    }while(isPrime != 1);
    
     bool res = true;
    do{
        ObliviousUtils::generateRandomNumber(holder.g, (holder.pLength -1)/8);
        mpz_powm(tmp, holder.g, pdq, holder.p);
        mpz_powm(tmp2, holder.g, holder.q, holder.p);
        res =  (mpz_cmp_ui(tmp,1) == 0) || (mpz_cmp_ui(tmp2,1) == 0); 
    }while(res);
    
     ObliviousUtils::generateRandomNumber(holder.r, holder.qLength/8);
     mpz_mod(holder.r, holder.r, holder.q);
     mpz_powm(holder.gr, holder.g, holder.r, holder.p);
     
     ObliviousUtils::generateRandomNumber(holder.C, holder.qLength/8);
     mpz_mod(holder.C, holder.C, holder.q);
     
     
    mpz_powm(holder.Cr, holder.C, holder.r, holder.p);
    
    mpz_clears(tmp, tmp2, pdq, NULL);
    std::cout << "End generate Data!" << std::endl;
}

void NaoriPinkasSenderData::writeInitDataToFile(const std::string &path, NaoriPinkasSenderData& holder){

     if(FILE *fptr = fopen(path.c_str(), "w")){
            
            mpz_out_str(fptr, 10, holder.C);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.p);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.q);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.g);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.gr);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.r);
            fprintf(fptr,"\n");
            
            mpz_out_str(fptr, 10, holder.Cr);
            fprintf(fptr,"\n");
            
            fclose(fptr);
     }else {
         throw std::runtime_error("Could not open file");
     }
}

void NaoriPinkasSenderData::readInitDataFromFile(const std::string &path, NaoriPinkasSenderData& holder){
    
     std::ifstream t("/home/idiachen/test.txt");
    std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
                 
    std::vector<std::string> data;
    
    std::size_t current, previous = 0;
    current = str.find("\n");
    while (current != std::string::npos) {
        std::cout << str.substr(previous, current - previous) << std::endl;
        
        data.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find("\n", previous);
    }
     data.push_back(str.substr(previous,str.size()));
    
    mpz_set_str(holder.C, data.at(0).c_str(), 10);
    mpz_set_str(holder.p, data.at(1).c_str(), 10);
    mpz_set_str(holder.q, data.at(2).c_str(), 10);
    mpz_set_str(holder.g, data.at(3).c_str(), 10);
    mpz_set_str(holder.gr, data.at(4).c_str(), 10);
    mpz_set_str(holder.r, data.at(5).c_str(), 10);
    mpz_set_str(holder.Cr, data.at(6).c_str(), 10);
    
}