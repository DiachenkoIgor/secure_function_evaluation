#ifndef ENDMESSAGE_H
#define ENDMESSAGE_H
#include <stdio.h>
#include <gmp.h>

class EndMessage
{
public:
    mpz_t m0, m1;
    EndMessage();
    ~EndMessage();

};

#endif // ENDMESSAGE_H
