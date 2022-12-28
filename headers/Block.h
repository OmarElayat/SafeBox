#ifndef BLOCK_H
#define BLOCK_H

#include <defines.h>
#include <includes.h>
#include <iostream>
#include <fstream>

using namespace std;

class Block {

    private:
        uint32_t block_size;
        uint32_t read_size;
        char * buffer;
    public:
        Block (uint32_t p_block_size);
        bool load(fstream & f);
        bool store(fstream & f);
        void encrypt(CryptoPP::byte * key, CryptoPP::byte * iv);
        void decrypt(CryptoPP::byte * key, CryptoPP::byte * iv);
        void print ();
        ~Block();
};
#endif