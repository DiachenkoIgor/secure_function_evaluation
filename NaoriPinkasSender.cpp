#include "NaoriPinkasSender.h"

NaoriPinkasSender::NaoriPinkasSender()
{
}

NaoriPinkasSender::NaoriPinkasSender(std::string path, int port)
{
    this -> path = path;
    this -> port = port;
}

NaoriPinkasSender::~NaoriPinkasSender()
{
    close(this -> server_fd);
    close(this -> new_socket);
}


void NaoriPinkasSender::initServerAndAccept(){
    struct sockaddr_in address;
    int server_fd, new_socket;
    int addrlen = sizeof(address);
    int opt = 1; 
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE); 
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        std::cout << "setsockopt" << std::endl;
        exit(EXIT_FAILURE); 
    } 
    
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( this -> port );

     if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    {   std::cout << "bind failed" << std::endl; 
        exit(EXIT_FAILURE); 
    }
    
    if (listen(server_fd, 3) < 0) 
    { 
        std::cout << "listen" << std::endl;
        exit(EXIT_FAILURE); 
    }
    


     if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        std::cout << "accept" << std::endl;
        exit(EXIT_FAILURE); 
    }
    this -> server_fd = server_fd;
    this -> new_socket = new_socket;
}

void NaoriPinkasSender::initialize(){

    NaoriPinkasSenderData::readInitDataFromFile(this -> path, this -> data);

    initServerAndAccept();

    NaoriPinkasReceiverData receiverData;
    mpz_set(receiverData.C , this -> data.C);
    mpz_set(receiverData.g , this -> data.g);
    mpz_set(receiverData.gr , this -> data.gr);
    mpz_set(receiverData.p , this -> data.p);
    mpz_set(receiverData.q , this -> data.q);
    receiverData.msgByteLength = this -> messageByteLength;
    
    std::string value = ObliviousUtils::serializeNPReceiverData(receiverData);
    
    
    send(this -> new_socket , value.c_str() , value.size() , 0 );
        
}

void NaoriPinkasSender::sendMessages(char * m1, int m1Size, char * m2, int m2Size){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string key = readJSON();

    ReceiverPublicKey pk;
    NaoriPinkasTransfer transfer;
    ObliviousUtils::deserializeNPReceiverPublicKey(pk, key);


    NaoriPinkasSender::encryptData(this -> data, m1, (size_t)m1Size, m2, (size_t)m2Size, pk.key, this -> messageByteLength, transfer);

    std::string value = ObliviousUtils::serializeNPTransfer(transfer);

    send(new_socket , value.c_str() , value.size() , 0 );
}


std::string NaoriPinkasSender::readJSON(){
    std::string result;
    
     int q=0;
        char t[2];
        t[1] = '\0';
        bool stop=true;
        while (stop){
            recv(this -> new_socket, t, 1,0);
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






void NaoriPinkasSender::encryptData(NaoriPinkasSenderData& requiredData, char * m1, size_t m1Size,
                                    char * m2, size_t m2Size, mpz_t& pk, int msgByteLength, NaoriPinkasTransfer& transfer){
    mpz_t msg1, msg2, pk0, pk1, R, encrypt1, encrypt2;
    
    mpz_inits(msg1, msg2, pk0, pk1, R, encrypt1, encrypt2, NULL);
    
    
    ObliviousUtils::importGmpNumberFromByteArray(msg1, (const char *) m1, (int) m1Size);
    ObliviousUtils::importGmpNumberFromByteArray(msg2, (const char *) m2, (int) m2Size);

    
    mpz_powm (pk0, pk, requiredData.r, requiredData.p);

    mpz_invert (pk1, pk0, requiredData.p);

    if(mpz_cmp_ui (pk1, 0) == 0)  throw std::runtime_error("Naori-Pinkas receiver's key hasn't multiplicative inverse!");

    mpz_mul (pk1, pk1, requiredData.Cr);

    mpz_mod (pk1, pk1, requiredData.p);
    

    ObliviousUtils::generateRandomNumber(R, (requiredData.rLength/8) + 1);

    ObliviousUtils::encrypt(encrypt1, pk0, R, msg1, msgByteLength);
    ObliviousUtils::encrypt(encrypt2, pk1, R, msg2, msgByteLength);
    
    char * msg1Ptr = nullptr;
    char * msg2Ptr = nullptr;
    char * RPtr = nullptr;

    int msg1ArraySize = ObliviousUtils::exportGmpNumberToByteArray(encrypt1, msg1Ptr);
    int msg2ArraySize = ObliviousUtils::exportGmpNumberToByteArray(encrypt2, msg2Ptr);
    int RArraySize = ObliviousUtils::exportGmpNumberToByteArray(R, RPtr);
    
    transfer.setM1(msg1Ptr, msg1ArraySize);
    transfer.setM2(msg2Ptr, msg2ArraySize);
    transfer.setR(RPtr, RArraySize);

    delete [] msg1Ptr;
    delete [] msg2Ptr;
    delete [] RPtr;

}

