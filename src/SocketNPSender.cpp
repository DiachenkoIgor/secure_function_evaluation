#include "SocketNPSender.h"

SocketNPSender::SocketNPSender(int port, std::string path)
{
    this -> port = port;
    this -> path = path;
    this -> sender = new NaoriPinkasSender(path);
    this -> sender-> initialize();
    
}

SocketNPSender::~SocketNPSender()
{
    delete (this -> sender);
    close(this -> server_fd);
    close(this -> new_socket);
}

void SocketNPSender::initServerAndAccept(){
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

void SocketNPSender::start(int msgByteLength){
    initServerAndAccept();
    
    this -> sender -> setDescriptor(this -> new_socket);

    this -> sender-> handshake(msgByteLength);
    
}

void SocketNPSender::sendMessage(char * m1, int m1Size, char * m2, int m2Size){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    this -> sender -> sendMessages(m1, m1Size, m2, m2Size);
}
