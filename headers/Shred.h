#ifndef SHRED_H
#define SHRED_H

#include <includes.h>
#include <defines.h>
#include <stdio.h>
#include <FileSpooler.h>

class Shred {
    protected:  
        FileSpooler * fileSpooler;
    public:
        Shred(const char * p_file_name,uint16_t block_size,bool truncate = false);
        void operator << (Block & b);
        Block * getNextBlock ();
        virtual ~Shred(); 
};


#endif