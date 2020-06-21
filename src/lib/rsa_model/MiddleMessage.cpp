#include "MiddleMessage.h"

MiddleMessage::MiddleMessage()
{
    mpz_init(v);
}

MiddleMessage::~MiddleMessage()
{
    mpz_clear(v);
}

