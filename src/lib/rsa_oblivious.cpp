#include "rsa_oblivious.hpp"
#include <chrono>
namespace rsa_oblivious{
    
    int obliviousRSA(const char * m1, int m1Size,const char * m2, int m2Size, 
                char * cmd, StartMessage * startMessage, MiddleMessage * middleMessage, EndMessage * endMessage){
        //Convert messages to GMP numbers
        
        static mpz_t msg1, msg2;
        static oblivious_util::private_key ku;
        static oblivious_util::public_key kp;
        static int sizeOfRandomMessageBytes = 2;
    
        if(strcmp(cmd, "start") == 0){
            mpz_inits(msg1, msg2, NULL);
        
            mpz_init(kp.n);
            mpz_init(kp.e); 
            // Initialize private key
            mpz_init(ku.n); 
            mpz_init(ku.e); 
            mpz_init(ku.d); 
            mpz_init(ku.p); 
            mpz_init(ku.q);
        
            oblivious_util::importGmpNumberFromByteArray(msg1, m1, m1Size);
            oblivious_util::importGmpNumberFromByteArray(msg2, m2, m2Size);
            
            oblivious_util::generate_keys(&ku, &kp);
        
            oblivious_util::generateRandomNumber(startMessage -> x0, sizeOfRandomMessageBytes);
            oblivious_util::generateRandomNumber(startMessage -> x1, sizeOfRandomMessageBytes);
    
            mpz_set(startMessage -> modulus, kp.n);
            mpz_set(startMessage -> publicExponent, kp.e);
            return 0;
        }
    
        if(strcmp(cmd, "end") == 0){
            mpz_sub(endMessage -> m0, middleMessage -> v, startMessage -> x0);
            mpz_powm(endMessage -> m0, endMessage -> m0, ku.d, kp.n);
            mpz_add(endMessage -> m0, msg1, endMessage -> m0);
    
    
            mpz_sub(endMessage -> m1, middleMessage -> v, startMessage -> x1);
            mpz_powm(endMessage -> m1, endMessage -> m1, ku.d, kp.n);
            mpz_add(endMessage -> m1, msg2, endMessage -> m1);
    
            mpz_clears(kp.n, kp.e, ku.n, ku.e, ku.d, ku.p, ku.q, msg1, msg2, NULL);
            return 0;
        }
    
   
        return -1;
    }
    
    int receive( char * cmd, char *& ptr, int choice, int & size,
                StartMessage * startMessage, MiddleMessage * middleMessage, EndMessage * endMessage){
        static mpz_t k, v, result;
        static int sizeOfK = 8;
    
        if(strcmp(cmd, "start") == 0){
            mpz_inits(k, v, result, NULL);
        
            oblivious_util::generateRandomNumber(k, sizeOfK);
            
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             mpz_pow_ui(v, k, mpz_get_ui(startMessage -> publicExponent));
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "mpz_pow_ui  -  " << time << std::endl;
            
            
            begin = std::chrono::steady_clock::now();
            if(choice == 0)
                mpz_add(v, v, startMessage -> x0);
            else
                mpz_add(v, v, startMessage -> x1);
            end = std::chrono::steady_clock::now();
            time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "mpz_add  -  " << time << std::endl;
            
            begin = std::chrono::steady_clock::now();
            mpz_mod(v, v, startMessage -> modulus);
            end = std::chrono::steady_clock::now();
            time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "mpz_mod  -  " << time << std::endl;
            
            mpz_set(middleMessage -> v, v);
        
            return 0;
        
        }
    
        if(strcmp(cmd, "end") == 0){
            if(choice == 0)
                mpz_sub(result, endMessage -> m0, k);
            else
                mpz_sub(result, endMessage -> m1, k);
        
    
            size = oblivious_util::exportGmpNumberToByteArray(result, ptr);
     
            mpz_clears(k, v, result, NULL);
    
            return 0;
        }
    
        return -1;
    
    }
    
    void convertGMPtoJSONvalue(Value * dst, Document * document, mpz_t& data){
        char * ptr = nullptr;
        int size = oblivious_util::exportGmpNumberToByteArray(data, ptr);

        Document::AllocatorType& allocator = document -> GetAllocator();
    
        for(int i = 0;i < size; i++)
            dst -> PushBack((int)ptr[i], allocator);
        
        delete [] ptr;    
    }
    
    void convertJSONValueToGmp(Value * src1, mpz_t& data){
        mpz_t tmp;
        mpz_init(tmp);
        
        Value& src = *src1;
        int size = src.Size();
        
        char chrTmp[size];
        
        for (SizeType i = 0; i < src.Size(); i++)
            chrTmp[i] = src[i].GetInt();
            
       // importGmpNumberFromByteArray(tmp, chrTmp, size);
       // oblivious_util::importGmpNumberFromByteArray(tmp, chrTmp, size);
        mpz_import (tmp, size, 1, sizeof(chrTmp[0]), 1, 0, chrTmp);
        mpz_set(data,tmp);
        mpz_clear(tmp);
    }
    
    
    std::string serializeRSAStartMessage(StartMessage& data){
        Document d; 
        d.SetObject();
        
        Value modulus(kArrayType);
        convertGMPtoJSONvalue(&modulus, &d, data.modulus);
        d.AddMember("modulus", modulus, d.GetAllocator());
        
        Value publicExponent(kArrayType);
        convertGMPtoJSONvalue(&publicExponent, &d, data.publicExponent);
        d.AddMember("publicExponent", publicExponent, d.GetAllocator());
        
        Value x0(kArrayType);
        convertGMPtoJSONvalue(&x0, &d, data.x0);
        d.AddMember("x0", x0, d.GetAllocator());
        
        Value x1(kArrayType);
        convertGMPtoJSONvalue(&x1, &d, data.x1);
        d.AddMember("x1", x1, d.GetAllocator());
        
        
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
    }
    
    void deserializeRSAStartMessage(StartMessage& data, std::string& json_string){
        mpz_t mpz_tmp;
        mpz_init(mpz_tmp);
        
        Document document;
        document.Parse(json_string.c_str());

        Value& modulus = document["modulus"];
        convertJSONValueToGmp(&modulus, mpz_tmp);
        mpz_set(data.modulus, mpz_tmp);

        Value& publicExponent = document["publicExponent"];
        convertJSONValueToGmp(&publicExponent, mpz_tmp);
        mpz_set(data.publicExponent, mpz_tmp);

        Value& x0 = document["x0"];
        convertJSONValueToGmp(&x0, mpz_tmp);
        mpz_set(data.x0, mpz_tmp);

        Value& x1 = document["x1"];
        convertJSONValueToGmp(&x1, mpz_tmp);
        mpz_set(data.x1, mpz_tmp);
        
        mpz_clear(mpz_tmp);

    }
    
    std::string serializeRSAMidleMessage(MiddleMessage& data){
        Document d; 
        d.SetObject();
        
        Value v(kArrayType);
        convertGMPtoJSONvalue(&v, &d, data.v);
        d.AddMember("v", v, d.GetAllocator());
        
         
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
       
    }
    
    void deserializeRSAMiddleMessage(MiddleMessage& data, std::string& json_string){
        
        mpz_t mpz_tmp;
        mpz_init(mpz_tmp);
        
        Document document;
        document.Parse(json_string.c_str());
        
        Value& v = document["v"];
        convertJSONValueToGmp(&v, mpz_tmp);
        mpz_set(data.v, mpz_tmp);
        
        mpz_clear(mpz_tmp);
        
    }
    
    std::string serializeRSAEndMessage(EndMessage& data){
        
        Document d; 
        d.SetObject();
        
        Value m0(kArrayType);
        convertGMPtoJSONvalue(&m0, &d, data.m0);
        d.AddMember("m0", m0, d.GetAllocator());
        
        Value m1(kArrayType);
        convertGMPtoJSONvalue(&m1, &d, data.m1);
        d.AddMember("m1", m1, d.GetAllocator());
        
         
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
        
    }
    
    void deserializeRSAEndMessage(EndMessage& data, std::string& json_string){
        mpz_t mpz_tmp;
        mpz_init(mpz_tmp);
        
        Document document;
        document.Parse(json_string.c_str());
        
        Value& m0 = document["m0"];
        convertJSONValueToGmp(&m0, mpz_tmp);
        mpz_set(data.m0, mpz_tmp);
        
        Value& m1 = document["m1"];
        convertJSONValueToGmp(&m1, mpz_tmp);
        mpz_set(data.m1, mpz_tmp);
        
        mpz_clear(mpz_tmp);
        
    }
    
}