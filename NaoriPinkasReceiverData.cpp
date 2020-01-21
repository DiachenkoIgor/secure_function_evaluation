#include "NaoriPinkasReceiverData.h"

NaoriPinkasReceiverData::NaoriPinkasReceiverData()
{
    mpz_inits(p, q, g, C, gr, NULL);
}

NaoriPinkasReceiverData::~NaoriPinkasReceiverData()
{
    mpz_clears(p, q, g, C, gr, NULL);
}

