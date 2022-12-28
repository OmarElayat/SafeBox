#ifndef FILESPOOLER_H
#define FILESPOOLER_H

#include <includes.h>
#include <defines.h>
#include <Block.h>
#include<cmath>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;


class FileSpooler{
    private:
        uint16_t block_size;
        fstream f;
        long fsize; 
        mutex mtx;
        Block * getBlockLockFree();
    public:
        FileSpooler(const char * p_file_name, uint16_t p_block_size, bool truncate =false);
        long getBlockCount (); 
        Block * operator [] (long index);
        Block * getNextBlock();  
        void appendBlock (Block * b);   
        void writeBlockAt(Block *b,long block_index);  
        ~FileSpooler();
};


#endif
