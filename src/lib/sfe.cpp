#include "sfe.hpp" 
#include <string> 
#include <gmp.h> 
#include <chrono>
#include "rsa_oblivious.hpp"
namespace sfe{
    
    namespace {
        void createGarbledTable(Gate * gate, GarbledTableMessage &garbledTableMessage){
                std::vector<CryptoPairHolder> encodedTable;
    
                for(int i = 0; i < 4; i++ ){
                    CryptoPairHolder tmp;
                    tmp.init(gate -> ENCODE_SIZE, gate -> NONCE_SIZE, gate -> NONCE_SIZE);
                    tmp.setCipher(gate -> encodeResult[i]);
                    tmp.setHashA(gate -> nonceA[i]);
                    tmp.setHashB(gate -> nonceB[i]);
                    encodedTable.push_back(tmp);
                }
    
                oblivious_util::randomize(encodedTable);
                garbledTableMessage.init(encodedTable);
    
                char tmp[gate -> LABEL_SIZE];
                gate -> getInputLabel(tmp);
                garbledTableMessage.setLable(tmp);
        }
        
        void createHalfEncryptTable(GarbledTableMessage * table){
    
            int resultSize = table -> holders[0].cipher_size - oblivious_util::MAC_SIZE;


            char * tmp = new char[resultSize];

    
            for(int i = 0; i < 4; i++){
                if( oblivious_util::secretDecrypt(tmp, table -> holders[i].cipher,
                        table -> holders[i].cipher_size, table -> holders[i].hashA, table -> lable) == 0 ) {
    
                        table -> holders[i].reInitCipher(tmp, resultSize);
                        table -> holders[i].encrypted = true;
                }
                else {
                        table -> holders[i].encrypted = false;
                }
            }
    
        }

        void createResultFromHalfEncrypted(GarbledTableMessage * table, char * bkey, char * dst){
    
            for(int i = 0; i < 4; i++){

                if(!table -> holders[i].encrypted) continue;
        
                int tmp = oblivious_util::secretDecrypt(dst, table -> holders[i].cipher,
                            table -> holders[i].cipher_size, table -> holders[i].hashB, bkey);
                        
                if( tmp == 0 ) {
                    break;
                }
                else {
                    table -> holders[i].encrypted = false;
                }
            }
        }
    }
    
    extern "C" int calculateGate(const char * cmd, char * gateName, bool * input,
            std::string * startMessageJson, std::string * middleMessageJson, std::string * endMessageJson,
            GateResult * gateResult, bool & res, std::string * jsonMessage){
        static Gate * gate = nullptr;
        static StartMessage startMessage;
        static GarbledTableMessage  garbledTableMessage;
        static MiddleMessage middleMessage;
        static EndMessage endMessage;
    
        if(gateName != nullptr){
        
            if(gate != nullptr) delete gate;
        
            if(strcmp(gateName, "and") == 0){
                gate = new AndGate();
            }
            if(strcmp(gateName, "or") == 0){
                gate = new OrGate();
            }
            if(strcmp(gateName, "xor") == 0){
                gate = new XorGate();
            }
        
        }
    
        if(strcmp(cmd, "garbled") == 0){
            gate -> setInput(*input);
            
            createGarbledTable(gate, garbledTableMessage);
            
            jsonMessage -> clear();
            jsonMessage -> assign(sfe::serializeGarbledTableMessage(garbledTableMessage));
            
            return 0;
        }
        if(strcmp(cmd, "oblivious_start") == 0){
        
            char otValue1[gate -> LABEL_SIZE];
            char otValue2[gate -> LABEL_SIZE];
    
            std::memcpy(otValue1, gate -> lables[2], gate -> LABEL_SIZE);
            std::memcpy(otValue2, gate -> lables[3], gate -> LABEL_SIZE);
                    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                        int resultCode = rsa_oblivious::obliviousRSA(otValue1, gate -> LABEL_SIZE, otValue2, gate -> LABEL_SIZE, "start", &startMessage, nullptr, nullptr);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
             std::cout << "calculateGate 'oblivious start' +  " << time << std::endl;

            jsonMessage -> clear();
            jsonMessage -> assign(rsa_oblivious::serializeRSAStartMessage(startMessage));
            
            return resultCode;
    
        }
        if(strcmp(cmd, "oblivious_end") == 0){
            
            if(middleMessageJson == nullptr) return -1;
            
            rsa_oblivious::deserializeRSAMiddleMessage(middleMessage, *middleMessageJson);
         std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                   
            int resultCode = rsa_oblivious::obliviousRSA(nullptr, gate -> LABEL_SIZE, nullptr, gate -> LABEL_SIZE, "end", &startMessage, &middleMessage, &endMessage);
                        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
             std::cout << "calculateGate 'oblivious_end' +  " << time << std::endl;

            jsonMessage -> clear();
            jsonMessage -> assign(rsa_oblivious::serializeRSAEndMessage(endMessage));
            
            return resultCode;
        }

        if(strcmp(cmd, "end") == 0){
            if(std::memcmp(gate -> lables[4], gateResult -> value, gate -> LABEL_SIZE) == 0){
                res = false;
                return 0;
            }
            if(std::memcmp(gate -> lables[5], gateResult -> value, gate -> LABEL_SIZE) == 0){
                res = true;
                return 0;
            }
        }
        return -1;
    
    }
    
    int calculateValue(char * cmd, bool * compareValue , std::string * garbledTableJson, std::string * startMessageJson,
                                std::string * endMessageJson, std::string * jsonMessage){
        static GarbledTableMessage tableMessage;
        static StartMessage startMessage;
        static MiddleMessage middleMessage;
        static EndMessage endMessage;
        static int size;
        
        
        if(strcmp(cmd, "garbled") == 0){
             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             sfe::deserializeGarbledTableMessage(tableMessage, *garbledTableJson);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
             
             begin = std::chrono::steady_clock::now();
            sfe::deserializeGarbledTableMessage(tableMessage, *garbledTableJson);
            end = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "'garbled' step JSON  -  " << time << std::endl;
            
            
            begin = std::chrono::steady_clock::now();
            createHalfEncryptTable(&tableMessage);
            end = std::chrono::steady_clock::now();
            std::cout << "'garbled' step 'createHalfEncryptTable'  -  " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
            return 0;
        }
        
        if(strcmp(cmd, "oblivious_start") == 0){


            char * bKey = nullptr;
            
             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             rsa_oblivious::deserializeRSAStartMessage(startMessage, *startMessageJson);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            
            
            begin = std::chrono::steady_clock::now();
            rsa_oblivious::receive("start", bKey, (int)(*compareValue), size, &startMessage, &middleMessage, &endMessage);
            end = std::chrono::steady_clock::now();
            std::cout << "'oblivious_start' step 'start' -  " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;


            jsonMessage -> clear();
            
            begin = std::chrono::steady_clock::now();
            jsonMessage -> assign(rsa_oblivious::serializeRSAMidleMessage(middleMessage));
            end = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "'oblivious_start' step JSON -  " << time << std::endl;
            
            return 0;
        }
        
        if(strcmp(cmd, "end") == 0){
            
            char * bKey = nullptr;
            char dst[GarbledTableMessage::LABEL_SIZE];
            
             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             rsa_oblivious::deserializeRSAEndMessage(endMessage, *endMessageJson);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             long int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            
            begin = std::chrono::steady_clock::now();
            rsa_oblivious::receive("end", bKey, (int)(*compareValue), size, &startMessage, &middleMessage, &endMessage);
            end = std::chrono::steady_clock::now();
            std::cout << "'end' step 'rsa_oblivious::receive' -  " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
            
            
            begin = std::chrono::steady_clock::now();
            createResultFromHalfEncrypted(&tableMessage, bKey, dst);
            end = std::chrono::steady_clock::now();
            std::cout << "'end' step 'createResultFromHalfEncrypted' -  " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
            
            
            GateResult result(GarbledTableMessage::LABEL_SIZE);
            result.setValue(dst);
            
            delete [] bKey;
            
            jsonMessage -> clear();
            
            begin = std::chrono::steady_clock::now();
            jsonMessage -> assign(sfe::serializeGateResult(result));
            end = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            std::cout << "'end' step  -  " << time << std::endl;
            
            return 0;
        }
    return -1;
    }
    
    void convertArraytoJSONvalue(Value * dst, Document * document, char * ptr, int size){
        
        Document::AllocatorType& allocator = document -> GetAllocator();
    
        for(int i = 0;i < size; i++)
            dst -> PushBack((int)ptr[i], allocator);

    }
    
    void convertJSONValueToArray(Value * src1, char * ptr, int size){
        
        Value& src = *src1;
        
        for (SizeType i = 0; i < src.Size(); i++)
            ptr[i] = src[i].GetInt();

    }
    
    void serializeCryptoPairHolderToJSONValue(Value * src1, Document * document, CryptoPairHolder& data){
        
        Value cipher(kArrayType);
        convertArraytoJSONvalue(&cipher, document, data.cipher, data.cipher_size);
        src1 -> AddMember("cipher", cipher, document -> GetAllocator());
        
        Value hashA(kArrayType);
        convertArraytoJSONvalue(&hashA, document, data.hashA, data.hashA_size);
        src1 -> AddMember("hashA", hashA, document -> GetAllocator());
        
        Value hashB(kArrayType);
        convertArraytoJSONvalue(&hashB, document, data.hashB, data.hashB_size);
        src1 -> AddMember("hashB", hashB, document -> GetAllocator());
      
    }
    
    void deserializeCryptoPairHolder(CryptoPairHolder& dst, Value * src1){
        
        Value& src = *src1;
        
        char* tmpArr;
        int size;
        
        Value& cipher = src["cipher"];
        size = cipher.Size();
        tmpArr = new char[size];
        convertJSONValueToArray(&cipher, tmpArr, size);
        dst.cipher_size = size;
        dst.cipher = tmpArr;
        
        Value& hashA = src["hashA"];
        size = hashA.Size();
        tmpArr = new char[size];
        convertJSONValueToArray(&hashA, tmpArr, size);
        dst.hashA_size = size;
        dst.hashA = tmpArr;
        
        Value& hashB = src["hashB"];
        size = hashB.Size();
        tmpArr = new char[size];
        convertJSONValueToArray(&hashB, tmpArr, size);
        dst.hashB_size = size;
        dst.hashB = tmpArr;

    }
    
    std::string serializeGarbledTableMessage(GarbledTableMessage& data){
        Document d; 
        d.SetObject();
        
        Value holders(kArrayType);
        Document::AllocatorType& allocator = d.GetAllocator();
        
        for(int i = 0; i < 4; i++){
            Value holder(kObjectType);
            serializeCryptoPairHolderToJSONValue(&holder, &d, data.holders[i]);
            holders.PushBack(holder, allocator);
        }
        
        d.AddMember("holders", holders, d.GetAllocator());

        Value lable(kArrayType);
        convertArraytoJSONvalue(&lable, &d, data.lable, data.LABEL_SIZE);
        d.AddMember("lable", lable, d.GetAllocator());
    
          
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
    }
    
    void deserializeGarbledTableMessage(GarbledTableMessage& dst, std::string& json_string){
         
        Document document;
        document.Parse(json_string.c_str());
        
        std::vector<CryptoPairHolder> holdersVector;
        
        Value& holders = document["holders"];
        
        for(int i = 0; i < 4; i++){
            CryptoPairHolder tmp;
            Value tmpHolder = holders[i].GetObject();
            deserializeCryptoPairHolder(tmp, &tmpHolder);
            
            holdersVector.push_back(tmp);
        }
        dst.init(holdersVector);
        
        char* tmpArr;
        int size;
        
        Value& lable = document["lable"];
        size = lable.Size();
        tmpArr = new char[size];
        convertJSONValueToArray(&lable, tmpArr, size);
        dst.setLable(tmpArr);
        
        delete [] tmpArr;
        
    }
    
    std::string serializeGateResult(GateResult& data){
        Document d; 
        d.SetObject();
        
        Value value(kArrayType);
        convertArraytoJSONvalue(&value, &d, data.value, data.value_size);
        d.AddMember("value", value, d.GetAllocator());
        
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        
        return buffer.GetString();
    }
    
    void deserializeGateResult(GateResult& dst, std::string& json_string){
        Document document;
        document.Parse(json_string.c_str());
        
        char* tmpArr;
        int size;
        
        Value& value = document["value"];
        size = value.Size();
        tmpArr = new char[size];
        convertJSONValueToArray(&value, tmpArr, size);
        dst.setValue(tmpArr);
        
        delete [] tmpArr;

    }

}

/*  int main()
{
    mpz_t data;
    mpz_init(data);
    
    char ptr[5] = {120,120,0,4,5};
    
    mpz_import (data, 5, 1, sizeof(ptr[0]), 1, 0, ptr);
    
    gmp_printf("%Zd\n",data);
    StartMessage startMessage;
    std::string test("{\"modulus\":[-48,-56,21,76,-52,-101,58,112,47,65,88,127,-77,9,44,-50,-85,-101,-61,-90,-79,-118,73,-56,44,7,111,-32,-19,5,-124,-63,-28,-42,-115,-108,-57,-18,12,110,109,1,-67,112,-59,41,-36,33,39,-57,-25,31,85,41,116,82,-101,51,40,4,109,-18,-64,99,-52,-5,-82,63,-65,43,4,-32,-59,-70,-47,36,65,-103,120,98,-15,95,40,60,-79,96,-1,-112,108,19,120,79,-83,-111,-120,94,-82,1,20,-28,-110,-75,-94,-125,-31,32,102,-125,78,-103,66,-59,21,26,66,-47,-90,-74,76,-53,42,77,-16,102,69,28,-120,-35],\"publicExponent\":[1,0,1],\"x0\":[102,98],\"x1\":[-9,-40]}");
    rsa_oblivious::deserializeRSAStartMessage(startMessage, test);
    gmp_printf("%Zd\n",startMessage.modulus);
    
  return 0;
}*/

