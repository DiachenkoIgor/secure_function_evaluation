#include "oblvious_util.hpp"

namespace oblivious_util {

    namespace {
        
        void generateRandomByteArray(char * arr,int arrSize){
            randombytes_buf(arr, arrSize);
        }
        
        int bit_size(mpz_t &number){
            return mpz_sizeinbase(number, 2);
        }
        
        void generatePrimeRandomNumber(mpz_t &place, int bytesSize){
            mpz_t tmp;
            mpz_init(tmp);
    
            generateRandomNumber(tmp,bytesSize);
            mpz_nextprime(place,tmp);
    
            mpz_clear(tmp);
        }
    }

    void generateRandomNumber(mpz_t &place, int bytesSize){
        char p [bytesSize]; 
        generateRandomByteArray(p, bytesSize);
        mpz_import (place, sizeof(p), 1, sizeof(p[0]), 0, 0, p);
    }
    
    int exportGmpNumberToByteArray(mpz_t &data, char *& ptr){
        int size = ceil(mpz_sizeinbase(data, 2)/8.0);
        char *c = new char[size];
        mpz_export(c, NULL, 1, sizeof(c[0]), 1, 0, data);
        ptr = c;
        return size;
    }
    
    int importGmpNumberFromByteArray(mpz_t &data, const char * ptr, int size){
        mpz_import (data, size, 1, sizeof(ptr[0]), 1, 0, ptr);
    }
    
    void generate_keys(private_key* ku, public_key* kp){

        char buf[BUFFER_SIZE];
        int i;
        mpz_t phi; mpz_init(phi);
        mpz_t tmp1; mpz_init(tmp1);
        mpz_t tmp2; mpz_init(tmp2);

        srand(time(NULL));

        /* Insetead of selecting e st. gcd(phi, e) = 1; 1 < e < phi, lets choose e
        * first then pick p,q st. gcd(e, p-1) = gcd(e, q-1) = 1 */
        // We'll set e globally.  I've seen suggestions to use primes like 3, 17 or 
        // 65537, as they make coming calculations faster.  Lets use 3.
        mpz_set_ui(ku->e, 65537); 

        /* Select p and q */
        /* Start with p */
        // Set the bits of tmp randomly
        for(i = 0; i < BUFFER_SIZE; i++)
            buf[i] = rand() % 0xFF; 
        // Set the top two bits to 1 to ensure int(tmp) is relatively large
        buf[0] |= 0xC0;
        // Set the bottom bit to 1 to ensure int(tmp) is odd (better for finding primes)
        buf[BUFFER_SIZE - 1] |= 0x01;
        // Interpret this char buffer as an int
        mpz_import(tmp1, BUFFER_SIZE, 1, sizeof(buf[0]), 0, 0, buf);
        // Pick the next prime starting from that random number
        mpz_nextprime(ku->p, tmp1);
        /* Make sure this is a good choice*/
        mpz_mod(tmp2, ku->p, ku->e);        /* If p mod e == 1, gcd(phi, e) != 1 */
        while(!mpz_cmp_ui(tmp2, 1))         
        {
            mpz_nextprime(ku->p, ku->p);    /* so choose the next prime */
            mpz_mod(tmp2, ku->p, ku->e);
        }

        /* Now select q */
        do {
            for(i = 0; i < BUFFER_SIZE; i++)
                buf[i] = rand() % 0xFF; 
            // Set the top two bits to 1 to ensure int(tmp) is relatively large
            buf[0] |= 0xC0;
            // Set the bottom bit to 1 to ensure int(tmp) is odd
            buf[BUFFER_SIZE - 1] |= 0x01;
            // Interpret this char buffer as an int
            mpz_import(tmp1, (BUFFER_SIZE), 1, sizeof(buf[0]), 0, 0, buf);
            // Pick the next prime starting from that random number
            mpz_nextprime(ku->q, tmp1);
            mpz_mod(tmp2, ku->q, ku->e);
            while(!mpz_cmp_ui(tmp2, 1))
            {
                mpz_nextprime(ku->q, ku->q);
                mpz_mod(tmp2, ku->q, ku->e);
            }
        } while(mpz_cmp(ku->p, ku->q) == 0); /* If we have identical primes (unlikely), try again */

        /* Calculate n = p x q */
        mpz_mul(ku->n, ku->p, ku->q);

        /* Compute phi(n) = (p-1)(q-1) */
        mpz_sub_ui(tmp1, ku->p, 1);
        mpz_sub_ui(tmp2, ku->q, 1);
        mpz_mul(phi, tmp1, tmp2);

        /* Calculate d (multiplicative inverse of e mod phi) */
        if(mpz_invert(ku->d, ku->e, phi) == 0)
        {
            mpz_gcd(tmp1, ku->e, phi);
            printf("gcd(e, phi) = [%s]\n", mpz_get_str(NULL, 16, tmp1));
            printf("Invert failed\n");
        }

        /* Set public key */
        mpz_set(kp->e, ku->e);
        mpz_set(kp->n, ku->n);

        return;
    }
    
    void randomize(std::vector<CryptoPairHolder> &table) {
        auto rng = std::default_random_engine {};
      
        std::random_shuffle(table.begin(), table.end());
    }
    
    void generateSecretKey(char * key){
        crypto_secretbox_keygen((unsigned char *)key);
    }

    void generateSecretNonce(char * nonce, int nonceSize){
        randombytes_buf(nonce, nonceSize);
    }

    void secretEncrypt(char * cipher_result, char * message, int messageLength, char * nonce, char * key){
        crypto_secretbox_easy(
            (unsigned char *) cipher_result,
            (unsigned char *) message,
            messageLength,
            (unsigned char *) nonce,(unsigned char *) key);
    }

    int secretDecrypt(char * plain_result, char * cipher_text, int cipher_length, char * nonce, char * key){
        return crypto_secretbox_open_easy(
                            (unsigned char *) plain_result,
                            (unsigned char *) cipher_text,
                            cipher_length,
                            (unsigned char *) nonce,(unsigned char *) key);
    }
    
}