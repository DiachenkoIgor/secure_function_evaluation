#include "NaoriPinkasReceiverData.h"

NaoriPinkasReceiverData::NaoriPinkasReceiverData()
{
    mpz_inits(p, q, g, C, gr, NULL);

}

NaoriPinkasReceiverData::~NaoriPinkasReceiverData()
{
    mpz_clears(p, q, g, C, gr, NULL);
}
void NaoriPinkasReceiverData::createReceiverData(NaoriPinkasReceiverData &result, NaoriPinkasSenderData &data, int messagesLength){
    mpz_set(result.C , data.C);
    mpz_set(result.g , data.g);
    mpz_set(result.gr , data.gr);
    mpz_set(result.p , data.p);
    mpz_set(result.q , data.q);
    result.msgByteLength = messagesLength;
}
