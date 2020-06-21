#include "SFEUtil.h"

SFEUtil::SFEUtil()
{
}

SFEUtil::~SFEUtil()
{
}

void SFEUtil::convertArraytoVector(std::vector<char>& holder, char * src, int size){
    holder.insert(holder.begin() , src , src + size) ;    
}

void SFEUtil::convertVectorToArray(std::vector<char>& holder, char * dst){
    std::copy(holder.begin(), holder.end(), dst);    
}

void SFEUtil::socketSend(int descriptor, std::string value){
    send(descriptor , value.c_str() , value.size() , 0 );
}

json SFEUtil::serializeCryptoPairHolder(CryptoPairHolder& data){
    json result;
    std::vector<char> tmpHolder;
    
    convertArraytoVector(tmpHolder, data.cipher, data.cipher_size);
    result["cipher"] = tmpHolder;
    tmpHolder.clear();
    
    convertArraytoVector(tmpHolder, data.hashA, data.hashA_size);
    result["hashA"] = tmpHolder;
    tmpHolder.clear();
    
    convertArraytoVector(tmpHolder, data.hashB, data.hashB_size);
    result["hashB"] = tmpHolder;
    tmpHolder.clear();
    
    return result;
}

std::string SFEUtil::serializeGateResult(GateResult& data){
    json result;
    std::vector<char> tmpHolder;
    
    convertArraytoVector(tmpHolder, data.value, data.value_size);
    result["value"] = tmpHolder;
    tmpHolder.clear();
    
    return result.dump();
}
void SFEUtil::deserializeGateResult(GateResult& dst, std::string& json_string){
    char* tmpArr;
    
    json tmp = json::parse(json_string);
    std::vector<char> tmpHolder;
    
    tmpHolder = tmp["value"].get<std::vector<char>>();
    
    tmpArr = new char[tmpHolder.size()];
    
    convertVectorToArray(tmpHolder, tmpArr);
    
    dst.setValue(tmpArr);
    
    delete [] tmpArr;
}

void SFEUtil::deserializeCryptoPairHolder(CryptoPairHolder& dst, std::string& json_string){
    
    char* tmpArr;
    
    json tmp = json::parse(json_string);
    std::vector<char> tmpHolder;
    
    tmpHolder = tmp["cipher"].get<std::vector<char>>();
    
    tmpArr = new char[tmpHolder.size()];
    
    convertVectorToArray(tmpHolder, tmpArr);
    
    dst.cipher_size = tmpHolder.size();
    
    dst.cipher = tmpArr;

    
    tmpHolder = tmp["hashA"].get<std::vector<char>>();
    
    tmpArr = new char[tmpHolder.size()];
    
    convertVectorToArray(tmpHolder, tmpArr);
    
    dst.hashA_size = tmpHolder.size();
    
    dst.hashA = tmpArr;
    
    
    tmpHolder = tmp["hashB"].get<std::vector<char>>();
    
    tmpArr = new char[tmpHolder.size()];
    
    convertVectorToArray(tmpHolder, tmpArr);
    
    dst.hashB_size = tmpHolder.size();
    
    dst.hashB = tmpArr;
}


std::string SFEUtil::serializeGarbledTableMessage(GarbledTableMessage& data){
    json result;
    std::vector<char> tmpHolder;
    std::vector<json> tmpHolderForObj;
    
    for(int i = 0; i < 4; i++)
        tmpHolderForObj.push_back(serializeCryptoPairHolder(data.holders[i]));
        

    result["holders"] = tmpHolderForObj;
    tmpHolderForObj.clear();
    
    convertArraytoVector(tmpHolder, data.lable, data.LABEL_SIZE);
    result["lable"] = tmpHolder;
    tmpHolder.clear();
    
    return result.dump();
}

void SFEUtil::deserializeGarbledTableMessage(GarbledTableMessage& dst, std::string& json_string){
    
    char* tmpArr;
    std::vector<CryptoPairHolder> holders;
    
    json tmp = json::parse(json_string);
    std::vector<char> tmpHolder;
    
    tmpHolder = tmp["lable"].get<std::vector<char>>();
    
    tmpArr = new char[tmpHolder.size()];
    
    convertVectorToArray(tmpHolder, tmpArr);
    
    dst.setLable(tmpArr);
    
    delete [] tmpArr;
    
    std::vector<json> tmpHolderForObj = tmp["holders"].get<std::vector<json>>();
    
    for(int i = 0; i < 4; i++){
        CryptoPairHolder tmp;
        std::string res = tmpHolderForObj.at(i).dump();
        deserializeCryptoPairHolder(tmp, res);
        holders.push_back(tmp);
    }
    
        
    dst.init(holders);
}

std::string SFEUtil::readJSON(int descriptor){
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
