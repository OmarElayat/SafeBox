#ifndef SHREDTHREAD_H
#define SHREDTHREAD_H

#include <Thread.h>
#include <Shred.h>
#include <Lottery.h>
#include <MultiHeadQueue.h>

using namespace std;

class ShredThread: public Thread, public Shred {
    protected:
        FileSpooler * srcFileSpooler;
        char * key_file_name;
        string iv_file_name;
        MultiHeadQueue  <sb_block_index_t> * multiHeadQueue;
        char shred_name;
    public:
        ShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name, const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate = false);
        virtual void mainThreadBody();
        virtual ~ShredThread();
};


class EncryptShredThread: public ShredThread {
    private:
        Lottery * lottery;
        
    public:
        EncryptShredThread (FileSpooler * p_fileSpooler, char * p_key_file_name, char * p_iv_file_name,const char * p_file_name,uint16_t block_size,Lottery * p_lottery,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate = false);
        virtual void mainThreadBody();
        virtual ~EncryptShredThread();
};

class DecryptShredThread: public ShredThread {
    private:
    public:
        DecryptShredThread (FileSpooler * p_fileSpooler, char * p_key_file_name, char * p_iv_file_name,const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate = false);
        Block * operator [] (int index);
        virtual void mainThreadBody();
        virtual ~DecryptShredThread();
};

#endif
