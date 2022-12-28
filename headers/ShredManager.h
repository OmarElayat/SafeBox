#ifndef SHREDMANAGER_H
#define SHREDMANAGER_H

#include <includes.h>
#include <defines.h>
#include <FileSpooler.h>
#include <Shred.h>
#include <ShredThread.h>
#include <Lottery.h>

class ShredManager {
    protected: 
        Shred ** shreds;
        uint16_t shred_count;
    public:
        ShredManager (); 
        ShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool truncate = false);
        
        virtual bool encrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name);
        virtual bool decrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name);
        virtual ~ShredManager(); 
};


class MultithreadedShredManager: public ShredManager {
    private: 
        char * file_name;
        uint16_t block_size;
        bool truncate;

    public:
        MultithreadedShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool p_truncate = false);
        virtual bool encrypt (FileSpooler * p_fileSpooler, char * key_file_name, char * iv_file_name, char * q_file_name);
        virtual bool decrypt (FileSpooler * p_fileSpooler, char * key_file_name, char * iv_file_name, char * q_file_name);
        virtual ~MultithreadedShredManager(); 
};


#endif