#include <iostream>
#include <string>
#include "../lib/sfe.hpp"
#include "../lib/gates/Gate.h"
#include <typeinfo>
#include "../lib/gates/AndGate.h"
#include "../lib/gates/OrGate.h"
#include "../lib/gates/XorGate.h"
#include "pistache/endpoint.h"
#include "../lib/sfe_lib.hpp"
#include <chrono>
using namespace Pistache;
    
    
class HelloHandler : public Http::Handler {
private:
    std::string jsonMessage;
    
    void setJSONHeader(Http::ResponseWriter &response){
        response.headers()
                .add<Http::Header::ContentType>(Http::Mime::MediaType::fromString("application/json"));
    }
    
    bool calculateAnswer(){
        bool result = results.at(0) == 1 ? true : false;
        for(int i = 1; i < results.size(); i++){
            result = result | (results.at(i) == 1 ? true : false);
        }
        results.clear();
        return result;
    }


public:
    unsigned long int sfe_time= 0;
    bool start = true; 
    int data[106] = {1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1};
    std::vector<int> results;
    int step = 0;

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        bool result;
        JS_Answer answer;
        
         if (request.resource() == "/garbled" && request.method() == Http::Method::Get) {
            
             bool in = data[step] == 0 ? false : true;
             
             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             int execute = sfe::calculateGate("garbled", "xor", &in, nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             sfe_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
             
             answer.jsonMessage.assign(jsonMessage);
             answer.execute = execute;
             setJSONHeader(response);
             
            response.send(Http::Code::Ok, sfe_js::serializeJS_Answer(answer).c_str());
         }
         if (request.resource() == "/oblivious_start" && request.method() == Http::Method::Get) {

             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             int execute = sfe::calculateGate("oblivious_start", nullptr, nullptr,  nullptr, nullptr, nullptr, nullptr, result, &jsonMessage);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             sfe_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
             
             answer.jsonMessage.assign(jsonMessage);
             answer.execute = execute;
             setJSONHeader(response);
            response.send(Http::Code::Ok,  sfe_js::serializeJS_Answer(answer).c_str());
         }
         if (request.resource() == "/oblivious_end" && request.method() == Http::Method::Post) {
             std::string middleMessageJson = request.body();
             
             std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
             sfe::calculateGate("oblivious_end", nullptr, nullptr,  nullptr, &middleMessageJson, nullptr, nullptr, result, &jsonMessage);
             std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
             sfe_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
             
             response.send(Http::Code::Ok);
         }
         if (request.resource() == "/oblivious_end" && request.method() == Http::Method::Get) {
                setJSONHeader(response);
                answer.jsonMessage.assign(jsonMessage);
                response.send(Http::Code::Ok, sfe_js::serializeJS_Answer(answer).c_str());
         }
         if (request.resource() == "/end" && request.method() == Http::Method::Post) {
             std::string gateJSON = request.body();
             
             
            GateResult gateResult(Gate::LABEL_SIZE);

            sfe::deserializeGateResult(gateResult, gateJSON);
            
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            sfe::calculateGate("end", nullptr, nullptr, nullptr, nullptr, nullptr, &gateResult, result, &jsonMessage);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            sfe_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

            results.push_back(result ? 1 : 0);
            step++;
            
            response.send(Http::Code::Ok);
         }
         if (request.resource() == "/result" && request.method() == Http::Method::Get) {
             std::cout << "Time Sfe execution  -  " << sfe_time << std::endl;
             sfe_time = 0;
             step = 0; 
             
            
             response.send(Http::Code::Ok, calculateAnswer() ? "true" : "false");
         }
        if (request.resource() == "/sfe" && request.method() == Http::Method::Get) {
            Http::serveFile(response, "/home/idiachen/Documents/learning/secure-data-evaluation/src/server_test/web/start.html");
        }
        if (request.resource() == "/sfe_lib.js" && request.method() == Http::Method::Get) {
            Http::serveFile(response, "/home/idiachen/Documents/learning/secure-data-evaluation/src/server_test/web/sfe_lib.js");
        }
         if (request.resource() == "/test.js" && request.method() == Http::Method::Get) {
            Http::serveFile(response, "/home/idiachen/Documents/learning/secure-data-evaluation/src/server_test/web/test.js");
        }
        if (request.resource() == "/test.wasm" && request.method() == Http::Method::Get) {
            Http::serveFile(response, "/home/idiachen/Documents/learning/secure-data-evaluation/src/server_test/web/test.wasm");
        }
         
    }
};





/*int main(int argc, char * argv[]){

    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9123));

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();
    
    
    return 0;
}*/