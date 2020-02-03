#include "NaoriPinkasReceiver.h"

NaoriPinkasReceiver::NaoriPinkasReceiver()
{
     mpz_init(this -> key);
}

NaoriPinkasReceiver::~NaoriPinkasReceiver()
{
    mpz_clear(this -> key);
    close(this -> sock);
}

NaoriPinkasReceiver::NaoriPinkasReceiver(int port, std::string address){
    this -> port = port;
    this -> address = address;
    mpz_init(key);
}

std::string NaoriPinkasReceiver::readJSON(){
    std::string result;
    
     int q=0;
        char t[2];
        t[1] = '\0';
        bool stop=true;
        while (stop){
              
            read(this -> sock, t, 1);
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
int NaoriPinkasReceiver::initSocketClient(){

    struct sockaddr_in serv_addr; 

    if ((this -> sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::cout << "Socket creation error" << std::endl;
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(this -> port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, this -> address.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    } 
   
    if (connect(this -> sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        std::cout << "Connection Failed" << std::endl;
        return -1;
    } 
}

void NaoriPinkasReceiver::initialize(){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if(initSocketClient() == -1){
        return;}

    std::string im = readJSON();

    ObliviousUtils::deserializeNPReceiverData(this -> data, im);

    
}
void NaoriPinkasReceiver::receive(int choice, char * result){
    this -> choice = choice;
    step1();
    step2(result);
}
void NaoriPinkasReceiver::step1(){
    mpz_t pkey;
    mpz_init(pkey);
    
    ReceiverPublicKey pk;
    
    generatePublicKey(this -> data, this -> choice, pkey, this -> key);

    mpz_set(pk.key, pkey);
    
    std::string result = ObliviousUtils::serializeNPReceiverPublicKey(pk);
    
   // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    send(this -> sock , result.c_str() , result.size() , 0 );
    

    mpz_clear(pkey);
}

void NaoriPinkasReceiver::step2(char * result){
    mpz_t tmpResult;
    mpz_init(tmpResult);

    std::string json = readJSON();


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    NaoriPinkasTransfer transfer;
    ObliviousUtils::deserializeNPTransfer(transfer, json);
    
    decryptResult(transfer, choice, this -> data.msgByteLength, this -> key ,tmpResult);
    
    char * tmp = nullptr;
    int quantity = ObliviousUtils::exportGmpNumberToByteArray(tmpResult, tmp);
    
    memcpy(result, tmp, quantity);
    
    mpz_clear(tmpResult);

    delete [] tmp;
}

void NaoriPinkasReceiver::generatePublicKey(NaoriPinkasReceiverData& requiredData, int choice, mpz_t& pkey, mpz_t& key){

    mpz_t k, gk, c_over_gk;
    mpz_inits(k, gk, c_over_gk, NULL);
    
    ObliviousUtils::generateRandomNumber(k, 
            ceil(ObliviousUtils::bit_size(requiredData.q)/8.0));
            
    mpz_mod (k, k, requiredData.q);
    
    mpz_powm (gk, requiredData.g, k, requiredData.p);

    mpz_invert (c_over_gk, gk, requiredData.p);

    mpz_mul (c_over_gk, requiredData.C, c_over_gk);

    mpz_mod (c_over_gk, c_over_gk, requiredData.p);

    mpz_powm (key, requiredData.gr, k, requiredData.p);

    if(choice == 1){
        mpz_set(pkey, c_over_gk);
    }else{
        mpz_set(pkey, gk);
    }
    
    mpz_clears(k, gk, c_over_gk, NULL);       
}

void NaoriPinkasReceiver::decryptResult(NaoriPinkasTransfer& requiredData, int choice, int msgByteLength, mpz_t& key, mpz_t& result){
    mpz_t R, msg;
    mpz_inits(R, msg, NULL);
    
    ObliviousUtils::importGmpNumberFromByteArray(R, requiredData.getR(), requiredData.GetRSize());
    
    if(choice==1){
        ObliviousUtils::importGmpNumberFromByteArray(msg, requiredData.getM2(), requiredData.GetM2Size());

        for(int i =0; i < requiredData.GetM2Size(); i++)
            std::cout << (int)requiredData.getM2()[i] << " ";
        ObliviousUtils::decrypt(result, key, R, msg, msgByteLength);
    }else {
        ObliviousUtils::importGmpNumberFromByteArray(msg, requiredData.getM1(), requiredData.GetM1Size());
 
        ObliviousUtils::decrypt(result, key, R, msg, msgByteLength);
    }
    
    mpz_clears(R, msg, NULL);
}


