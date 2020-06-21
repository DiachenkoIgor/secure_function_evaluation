#include "EndMessage.h"

EndMessage::EndMessage()
{
    mpz_inits(m0, m1, NULL);
}

EndMessage::~EndMessage()
{
    mpz_clears(m0, m1, NULL);
}

