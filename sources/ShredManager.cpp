#include <ShredManager.h>
#include <ThreadManager.h>
#include <MultiHeadQueue.h>
ShredManager::ShredManager () // added this in phase 2
{
}

ShredManager::ShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool truncate)
{
    shred_count = p_shred_count;
    shreds = (Shred ** ) calloc (shred_count,sizeof(Shred*));
    for ( char i = 0 ; i  < shred_count; i++)
    {
        string fname = p_file_name;
        fname.insert(fname.find('.'),1,i+'A');
        cout << fname << endl;
        if (truncate)
            shreds[i] = new Shred(fname.c_str(),p_block_size,truncate);
        else shreds[i] = new Shred(fname.c_str(),(p_block_size+16)&~15,truncate);
    }
}
bool ShredManager::encrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name)
{
    AutoSeededRandomPool prng;
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    ifstream kf;
    kf.open(key_file_name,ios::in);
    if ( kf.is_open())
    {
        kf.read (reinterpret_cast<char*>(key),sizeof(key));
        kf.close();
    }
    prng.GenerateBlock(iv,sizeof(iv));
    Block * block = fileSpooler->getNextBlock();
    for (int i = 0 ;block != NULL; i ++)
    {
        block->encrypt(key,iv);
        *(shreds[i%shred_count]) << *block;
        delete (block);
        block = fileSpooler->getNextBlock();
    }
    ofstream f;
    f.open(iv_file_name,ios::out|ios::trunc);
    if ( f.is_open())
    {
        f.write (reinterpret_cast<const char*>(iv),sizeof(iv));
        f.close();
    }
    return true;
}
bool ShredManager::decrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name)
{
    AutoSeededRandomPool prng;
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    ifstream f;
    f.open(key_file_name,ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(key),sizeof(key));
        f.close();
    }

    f.open(iv_file_name,ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(iv),sizeof(iv));
        f.close();
    }

    Block * block = NULL;
    for (int i = 0 ; i == 0 || block != NULL; i ++)
    {
        block = shreds[i%shred_count]->getNextBlock();
        if ( block == NULL) break;
        block->decrypt(key,iv);
        //block->print();
        fileSpooler->appendBlock(block);
        delete (block);
    }
    return false;
}
ShredManager::~ShredManager()
{
    for ( int i = 0 ; i  < shred_count; i++)
        delete(shreds[i]);
    free(shreds);
}
//*************

MultithreadedShredManager::MultithreadedShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool p_truncate) : ShredManager()
{
    file_name = p_file_name;
    shred_count = p_shred_count;
    block_size = p_block_size;
    truncate = p_truncate;
    shreds = (Shred ** ) calloc (shred_count,sizeof(Shred*));
}
bool MultithreadedShredManager::encrypt (FileSpooler * p_fileSpooler, char * key_file_name,  char * iv_file_name, char * q_file_name)
{
    ThreadManager threadmanager;    //creating an instance of thread manager to control all the threads together 
    Lottery lottery(p_fileSpooler->getBlockCount());  //seeding the lottery schedueler
    MultiHeadQueue<sb_block_index_t> multiHeadQueue;  //initializing the multiheadqueue
    AutoSeededRandomPool prng;              //generting iv and reading key
    CryptoPP::byte* key = new byte[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
	CryptoPP::byte* iv = new byte[ CryptoPP::AES::BLOCKSIZE ];
    prng.GenerateBlock(iv,sizeof(iv));    
	ofstream f;
	f.open(iv_file_name,ios::out);
    if ( f.is_open())
    {
        f.write (reinterpret_cast<const char*>(iv),CryptoPP::AES::BLOCKSIZE);   //casting the iv to const cahr pointer and storing it
        f.close();
    }
   for ( char i =0 ; i<shred_count ; i++){ 
       string fname = file_name;
        fname.insert(fname.find('.'),1,i+'A');   //creating the shreds names
        char pname = ('A'+i);   //shred letter
        if( truncate)                  
     //creating from encrypt shread thread class and passing to it the original file, the key, the iv, the thread name, lottery ticket, queue and the queue index 
     shreds[i]= new EncryptShredThread(p_fileSpooler,key_file_name,iv_file_name,(char*)(fname.c_str()),block_size,&lottery,&multiHeadQueue,pname,truncate);
else      shreds[i]= new EncryptShredThread(p_fileSpooler,(key_file_name),iv_file_name,(char*)(fname.c_str()),(block_size+16)&~15,&lottery,&multiHeadQueue,pname,truncate);
//dynamically casting the shreads into threads and adding them using operator overloading into threadmanager to control them all together
threadmanager += dynamic_cast <Thread *> (shreds[i]);
   }
//starting the threads
threadmanager.start();
//joining the threads 
threadmanager.barrier(); 
//creating the multiheadqueue and encrypting it to be used later
multiHeadQueue.dump(q_file_name,key_file_name,iv_file_name);
    return true;
}

bool MultithreadedShredManager::decrypt (FileSpooler * p_fileSpooler, char * key_file_name,  char * iv_file_name, char * q_file_name)
{
ThreadManager thr;

MultiHeadQueue<sb_block_index_t> multiHeadQueue;
multiHeadQueue.load(q_file_name,key_file_name,iv_file_name);    //loading the block indecies from the multiheaded queue
        //creating the threads, each thread for a shread with the same iv generated above
for(char i=0; i<shred_count; i++){
    string fname = file_name;
    fname.insert(fname.find('.'),1,i+'A');    //everything here is chronocally the same as in the encrypt function
    char pname = ('A'+i);    
    if ( truncate)
            //creating from decrypt shread thread class and passing to it the original file, the key, the iv, the thread name, lottery ticket, queue and the queue index
        shreds[i]= new DecryptShredThread(p_fileSpooler,key_file_name,iv_file_name,(char*)fname.c_str(),(block_size),&multiHeadQueue,pname,truncate) ;
else
    shreds[i]= new DecryptShredThread(p_fileSpooler,key_file_name,iv_file_name,(char*)fname.c_str(),(block_size+16)&~15,&multiHeadQueue,pname,truncate) ;
//dynamically casting the shreads into threads and adding them using operator overloading into threadmanager to control them all together
thr+= dynamic_cast <Thread *> (shreds[i]);

}
//starting and joining the threads
thr.start();
thr.barrier();
    return true;
}

MultithreadedShredManager::~MultithreadedShredManager()
{

}
