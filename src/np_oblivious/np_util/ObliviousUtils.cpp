#include "ObliviousUtils.h"

ObliviousUtils::ObliviousUtils()
{
}

ObliviousUtils::~ObliviousUtils()
{
}

void ObliviousUtils::generateRandomNumber(mpz_t &place, int bytesSize){
    char p [bytesSize]; 
    ObliviousUtils::generateRandomByteArray(p, bytesSize);
    mpz_import (place, sizeof(p), 1, sizeof(p[0]), 0, 0, p);
}
void ObliviousUtils::generateRandomByteArray(char * arr,int arrSize){
    randombytes_buf(arr, arrSize);
}
int ObliviousUtils::bit_size(mpz_t &number){
    return mpz_sizeinbase(number, 2);
}

void ObliviousUtils::generatePrimeRandomNumber(mpz_t &place, int bytesSize){
     mpz_t tmp;
    mpz_init(tmp);
    
    generateRandomNumber(tmp,bytesSize);
    mpz_nextprime(place,tmp);
    
    mpz_clear(tmp);
}

int ObliviousUtils::exportGmpNumberToByteArray(mpz_t &data, char *& ptr){
     int size = ceil(mpz_sizeinbase(data, 2)/8.0);
     char *c = new char[size];
     mpz_export(c, NULL, 1, sizeof(c[0]), 1, 0, data);
     ptr = c;
     return size;
}

void ObliviousUtils::importGmpNumberFromByteArray(mpz_t &data, const char * ptr, int size){
      mpz_import (data, size, 1, sizeof(ptr[0]), 1, 0, ptr);
}

void ObliviousUtils::generateHash(const unsigned char * data[], int quantity, int sizes[], unsigned char * out, int outLength){
    if(outLength < crypto_generichash_BYTES_MIN || outLength > crypto_generichash_BYTES_MAX){
            throw std::runtime_error("Out array size is wrong!");
    }
    
    crypto_generichash_state state;
    
    
    crypto_generichash_init(&state, NULL, 0, outLength);

    for(auto i = 0; i < quantity; i++){
        crypto_generichash_update(&state, data[i], sizes[i]);
    }
    
    crypto_generichash_final(&state, out, outLength);
}

void ObliviousUtils::keyDerevationFunction(unsigned char * out, int outLength, const char * key, int keyLength){
        static int saltFlag = -1;
        static char unsigned salt [crypto_pwhash_SALTBYTES];
        if(saltFlag == -1){
            randombytes_buf(salt, sizeof(salt));
            saltFlag=1;
        }


         int result = crypto_pwhash(out, outLength, key, keyLength, salt,
                  3,
                  crypto_pwhash_MEMLIMIT_MIN, crypto_pwhash_ALG_DEFAULT);

        if(result == -1){
                throw std::runtime_error("Can't derivate function!");
        }
}

void ObliviousUtils::encrypt(mpz_t &result, mpz_t &key, mpz_t &R, mpz_t &msg, int msgLengthBytes){

    char * keyByteData = nullptr;
    int keyDataSize = exportGmpNumberToByteArray(key,keyByteData);
    char * RByteData = nullptr;
    int RDataSize = exportGmpNumberToByteArray(R,RByteData);

    const char * tmp[] = {keyByteData, RByteData};
    int tmpSize[] = {keyDataSize, RDataSize};
    
    int tmpHashSize = 60;
    unsigned char hash[tmpHashSize];
    
    
  
    generateHash((const unsigned char **)tmp, 2, tmpSize, hash, tmpHashSize);

     char* outPtr = nullptr;
    if(msgLengthBytes > tmpHashSize){
        unsigned char out[msgLengthBytes];
        keyDerevationFunction((unsigned char *)out, msgLengthBytes, (const char *)hash, tmpHashSize);
        outPtr = new char[msgLengthBytes];
        memcpy(outPtr, out, sizeof out);
        
    }    
    else{
        outPtr = new char[tmpHashSize];
        memcpy(outPtr, hash, sizeof hash);
    }

    mpz_t data;
    mpz_init(data);
    
    importGmpNumberFromByteArray(data, (const char *) outPtr, msgLengthBytes);
    
    mpz_xor(result, data, msg);

    mpz_clear(data);
    delete [] keyByteData;
    delete [] RByteData;
    delete [] outPtr;
}

void ObliviousUtils::decrypt(mpz_t &result, mpz_t &key, mpz_t &R, mpz_t &cph, int cphLengthBytes){

    char * keyByteData = nullptr;
    int keyDataSize = exportGmpNumberToByteArray(key,keyByteData);
    char * RByteData = nullptr;
    int RDataSize = exportGmpNumberToByteArray(R,RByteData);
   
    const char * tmp[] = {keyByteData, RByteData};
    int tmpSize[] = {keyDataSize, RDataSize};
    
    int tmpHashSize = 60;
    unsigned char hash[tmpHashSize];


    generateHash((const unsigned char **)tmp, 2, tmpSize, hash, tmpHashSize);
    

      char* outPtr = nullptr;
       if(cphLengthBytes > tmpHashSize){
        unsigned char out[cphLengthBytes];
        keyDerevationFunction((unsigned char *)out, cphLengthBytes, (const char *)hash, tmpHashSize);
        outPtr = new char[cphLengthBytes];
        memcpy(outPtr, out, sizeof out);
    
    }    
    else{
        outPtr = new char[tmpHashSize];
        memcpy(outPtr, hash, sizeof hash);
    }

    
    mpz_t data;
    mpz_init(data);
    
    importGmpNumberFromByteArray(data, (const char *) outPtr, cphLengthBytes );

    mpz_xor(result, data, cph);

    
    mpz_clear(data);
    delete [] keyByteData;
    delete [] RByteData;
}

std::string ObliviousUtils::serializeNPReceiverData(NaoriPinkasReceiverData& data){
    json result;
    std::vector<char> tmpHolder;
    
    convertGMPtoVector(tmpHolder, data.p);
    result["p"] = tmpHolder;
    tmpHolder.clear();
    
    convertGMPtoVector(tmpHolder, data.q);
    result["q"] = tmpHolder;
    tmpHolder.clear();
    
    convertGMPtoVector(tmpHolder, data.g);
    result["g"] = tmpHolder;
    tmpHolder.clear();
    
    convertGMPtoVector(tmpHolder, data.C);
    result["C"] = tmpHolder;
    tmpHolder.clear();
    
    convertGMPtoVector(tmpHolder, data.gr);
    result["gr"] = tmpHolder;
    tmpHolder.clear();
    
    result["msgByteLength"] = data.msgByteLength;
    
    return result.dump();
}

void  ObliviousUtils::deserializeNPReceiverData(NaoriPinkasReceiverData& data, std::string& json_string){
    mpz_t mpz_tmp;
    mpz_init(mpz_tmp);
    
    json tmp = json::parse(json_string);
    std::vector<char> tmpHolder;
    
    tmpHolder = tmp["p"].get<std::vector<char>>();

    convertVectorToGmp(tmpHolder, mpz_tmp);

    mpz_set(data.p, mpz_tmp);

    tmpHolder = tmp["q"].get<std::vector<char>>();
    convertVectorToGmp(tmpHolder, mpz_tmp);
    
    mpz_set(data.q, mpz_tmp);
    
    tmpHolder = tmp["g"].get<std::vector<char>>();
    convertVectorToGmp(tmpHolder, mpz_tmp);
    
    mpz_set(data.g, mpz_tmp);
    
    tmpHolder = tmp["C"].get<std::vector<char>>();
    convertVectorToGmp(tmpHolder, mpz_tmp);
    
    mpz_set(data.C, mpz_tmp);
    
    tmpHolder = tmp["gr"].get<std::vector<char>>();
    convertVectorToGmp(tmpHolder, mpz_tmp);
    
    mpz_set(data.gr, mpz_tmp);
    
    data.msgByteLength =  tmp["msgByteLength"].get<int>();
    
    mpz_clear(mpz_tmp);
    
}


std::string ObliviousUtils::serializeNPReceiverPublicKey(ReceiverPublicKey& data){
    json result;
    std::vector<char> tmpHolder;
    
    convertGMPtoVector(tmpHolder, data.key);
    result["key"] = tmpHolder;
    tmpHolder.clear();
    
    return result.dump();
}

void  ObliviousUtils::deserializeNPReceiverPublicKey(ReceiverPublicKey& data, std::string& json_string){
    mpz_t mpz_tmp;
    mpz_init(mpz_tmp);
    
    std::vector<char> tmpHolder;
    
    json tmp = json::parse(json_string);
    
    tmpHolder = tmp["key"].get<std::vector<char>>();
    convertVectorToGmp(tmpHolder, mpz_tmp);
    
    mpz_set(data.key, mpz_tmp);
    
    mpz_clear(mpz_tmp);
}

std::string ObliviousUtils::serializeNPTransfer(NaoriPinkasTransfer& data){
    json result;
    std::vector<char> tmpHolder;
    
    convertArrayToVector(tmpHolder, (char *) data.getM1(), data.GetM1Size());
    result["m1"] = tmpHolder;
    tmpHolder.clear();
    
    convertArrayToVector(tmpHolder, (char *) data.getM2(), data.GetM2Size());
    result["m2"] = tmpHolder;
    tmpHolder.clear();
    
    convertArrayToVector(tmpHolder, (char *) data.getR(), data.GetRSize());
    result["R"] = tmpHolder;
    tmpHolder.clear();
    
    return result.dump();
}

void  ObliviousUtils::deserializeNPTransfer(NaoriPinkasTransfer& data, std::string& json_string){
    json tmp = json::parse(json_string);
    
    std::vector<char> temporaryValue;
    int tmpSize = 0;
    char * tmpArray = nullptr;  
    
    temporaryValue = tmp["m1"].get<std::vector<char>>();
    
    tmpSize = temporaryValue.size();
    
    tmpArray = new char[tmpSize];
    
    convertVectorToArray(temporaryValue, tmpArray, tmpSize);
    
    data.setM1(tmpArray, tmpSize);
    
    delete [] tmpArray;
    
    temporaryValue = tmp["m2"].get<std::vector<char>>();
    
    tmpSize = temporaryValue.size();
    
    tmpArray = new char[tmpSize];
    
    convertVectorToArray(temporaryValue, tmpArray, tmpSize);
    
    data.setM2(tmpArray, tmpSize);
    
    delete [] tmpArray;
    
    temporaryValue = tmp["R"].get<std::vector<char>>();
    
    tmpSize = temporaryValue.size();
    
    tmpArray = new char[tmpSize];
    
    convertVectorToArray(temporaryValue, tmpArray, tmpSize);
    
    data.setR(tmpArray, tmpSize);
    
    delete [] tmpArray;
    
}



void ObliviousUtils::convertArrayToVector(std::vector<char>& holder, char * ptr, int size){
    for(int i = 0;i< size; i++)
        holder.push_back(ptr[i]);
}
void ObliviousUtils::convertVectorToArray(std::vector<char>& holder, char * ptr, int size){
    for(int i = 0;i< size; i++)
        ptr[i] = holder.at(i);
}
void ObliviousUtils::convertGMPtoVector(std::vector<char>& holder, mpz_t& data){
    char * ptr = nullptr;
    int size = ObliviousUtils::exportGmpNumberToByteArray(data, ptr);
    
    for(int i = 0;i< size; i++)
        holder.push_back(ptr[i]);
        
    delete [] ptr;    
}

void ObliviousUtils::convertVectorToGmp(std::vector<char>& holder, mpz_t& data){
    mpz_t tmp;
    mpz_init(tmp);
    
    int size = holder.size();
    
    
    
    ObliviousUtils::importGmpNumberFromByteArray(tmp, holder.data(), size);
    mpz_set(data,tmp);
    mpz_clear(tmp);
}

std::string ObliviousUtils::readJSON(int descriptor){
    std::string result;
    
     int q=0;
        char t[2];
        t[1] = '\0';
        bool stop=true;
        while (stop){
            recv(descriptor, t, 1,0);
            if(*t=='{'){
                q++;
            }
            if(*t=='}'){
                q--;
                if(q==0) stop=false;
            }
            result.append(t);
        }
        return result; 
}

void ObliviousUtils::testAnalyzeSender(char * preffix, mpz_t& value){
    int size = mpz_sizeinbase(value, 10) +2;
     char data[size];
    
    mpz_get_str(data,10,value);
    
    std::string result;
    
    result.append(preffix);
    
    result.append("-");
    result.append(data);
    result.append("\n");
    
    
    std::fstream myFile("/home/idiachen/sender.txt", std::fstream::out | std::fstream::app);
    
    myFile << result;
    
    myFile.close();
}
void ObliviousUtils::testAnalyzeReceiver(char * preffix, mpz_t& value){
    int size = mpz_sizeinbase(value, 10) +2;
     char data[size];
    
    mpz_get_str(data,10,value);
    
    std::string result;
    
    
    result.append(preffix);
    
    result.append("-");
    result.append(data);
    result.append("\n");
    
    std::fstream myFile("/home/idiachen/receiver.txt", std::fstream::out | std::fstream::app);
    
    myFile << result;
    
    myFile.close();
}

