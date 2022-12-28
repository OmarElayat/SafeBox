#include <ShredThread.h>

ShredThread::ShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name, const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate):Thread(), Shred(p_file_name,block_size,truncate)
{
        srcFileSpooler = p_fileSpooler;
        key_file_name = p_key_file_name;
        iv_file_name = p_iv_file_name;
        multiHeadQueue = p_multiHeadQueue;
        shred_name = p_shred_name;
}
void ShredThread::mainThreadBody(){}
ShredThread::~ShredThread(){}

EncryptShredThread::EncryptShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name,const char * p_file_name,uint16_t block_size,Lottery * p_lottery,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate) : ShredThread(p_fileSpooler, p_key_file_name, p_iv_file_name,p_file_name,block_size,p_multiHeadQueue,p_shred_name,truncate)
{
    lottery = p_lottery;
}
void EncryptShredThread::mainThreadBody()
{

    CryptoPP::byte* key = new byte[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
	CryptoPP::byte* iv = new byte[ CryptoPP::AES::BLOCKSIZE ];

    
    ifstream kf;
    kf.open(key_file_name,ios::in);
    if ( kf.is_open())
    {
        kf.read (reinterpret_cast<char*>(key), CryptoPP::AES::DEFAULT_KEYLENGTH);
        kf.close();
    }

//    reading the key and the iv

    ifstream f;
	f.open(iv_file_name.c_str(),ios::out|ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(iv),sizeof(iv));
        f.close();
    }
//calling the lottery function to withdraw a ticket
long l = lottery->withdraw();
sb_block_index_t index;  //creating an index for the block
    index.shred= shred_name;  // passing the name of the shread to the name of the block

for ( int i = 0;l!=-1 ;i++)
{Block* bl=(*srcFileSpooler)[l];  // creating an object of type block and loading in it a block whose index is returned by the lotter schedualer
bl->encrypt(key,iv);                 //encrypting the block
*(this) << *bl;                      //storing the block
index.block= l;                           // equating the order of the block returned from the lottery in the index created above
index.shred_block = i;                      
multiHeadQueue->enqueue(index);             //enqueue the block into the queue
cout<<"ok"<<endl;
delete(bl); 
l = lottery->withdraw();              // withdrawing a new tickket
}
}
EncryptShredThread::~EncryptShredThread()
{

}

DecryptShredThread::DecryptShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name,const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate): ShredThread(p_fileSpooler, p_key_file_name, p_iv_file_name,p_file_name,block_size,p_multiHeadQueue,p_shred_name,truncate)
{
    

}
Block * DecryptShredThread::operator [] (int index)
{
    return (*fileSpooler)[index];
}

void DecryptShredThread::mainThreadBody()
{
    AutoSeededRandomPool prng;
    CryptoPP::byte* key = new byte[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
	CryptoPP::byte* iv = new byte[ CryptoPP::AES::BLOCKSIZE ];
// reading the key and the iv 
    ifstream f;
    f.open(key_file_name,ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(key),CryptoPP::AES::DEFAULT_KEYLENGTH);
        f.close();
    }
    f.open(iv_file_name.c_str(),ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(iv),CryptoPP::AES::BLOCKSIZE);
        f.close();
    }
     //   prng.GenerateBlock(iv,sizeof(iv));

sb_block_index_t index;         //creating an index
index.shred=shred_name;     // passing the name of the shread to the name of the block

for(; multiHeadQueue; ){

//while the multihead queue still contains blocks, the file continues to decrypt
//first passing the instance of the block index class to the dequeue
// second, a lampda function is created to check if the shred name of the passed index and the shread name of the dequeued block are the same ( needed in the
// multiheadqueue.h file
if(multiHeadQueue->dequeue(index,[](sb_block_index_t &index1, sb_block_index_t &index2)->bool{
    if(index1.shred == index2.shred) return true; 
    else return false;
})){cout<<"nice"<<endl; break;}
//reading from the passed filespooler the needed block that was returned from the multiheadqueue
Block* bl=(*this)[index.shred_block];
//decrypting the block
bl->decrypt(key,iv);
srcFileSpooler->writeBlockAt(bl,index.block);
//writing the block in the output file
}

}
DecryptShredThread::~DecryptShredThread()
{
    
}
