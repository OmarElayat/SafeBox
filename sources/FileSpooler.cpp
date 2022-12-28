#include <FileSpooler.h>
#include<cmath> 
Block * FileSpooler::getBlockLockFree()
{
    Block * b = new Block (block_size);
    if (b->load(f)) return b;
    else {
        delete (b);
        return NULL;
    }
}


FileSpooler::FileSpooler(const char * p_file_name, uint16_t p_block_size, bool truncate)
{
    fsize = 0;
    if ( truncate )
        f.open(p_file_name,ios::in|ios::out|ios::trunc);
    else f.open(p_file_name,ios::in|ios::out);
    if ( f.is_open())
    {
        long cur = f.tellg ();
        f.seekg(0,f.end);
        fsize = f.tellg ();
        f.seekg(cur,f.beg);
    }
    block_size = p_block_size;
}
 Block * FileSpooler::operator [] (long index)
 {
    mtx.lock();
    if ( index*block_size >= fsize) 
    {
        mtx.unlock();
        return NULL;
    }
    long cur = f.tellg ();
    f.seekg(index*block_size,f.beg);
    Block * b  = getBlockLockFree();
    f.seekg(cur,f.beg);
    mtx.unlock();
    return b;
 }
long FileSpooler::getBlockCount () 
{
	// checking if this is the last shred that contains the last block
 
	// in case it is not, we devide the file size over the block size

    if (fsize%block_size ==0 ) return fsize/block_size;
else return ((fsize/block_size)+1); 
	// is case it is, we do the same then add 1, because we account for the last block which is smaller in size 
	// in both cases we return the block count after we calculate it
}


Block * FileSpooler::getNextBlock()
{
    mtx.lock(); 
    Block * b  = getBlockLockFree();
    return b;
    mtx.unlock ();
}
void FileSpooler::appendBlock (Block * b)
{
    mtx.lock();
    b->store(f);
    mtx.unlock();
}
//this function writes the block it has been given as a paremeter in the file
void FileSpooler::writeBlockAt(Block *b,long block_index) 
{
//using the mutex to unlock, to prevent two threads from editing at the same time
mtx.lock();
//the index of this block determines where the block will be written
//pointing the writing pointer of the file to the correct position we need using this index
f.seekp(block_size*block_index,std::ios_base::beg);
b->store(f);
//using the method Block::store to write the contents of the block
mtx.unlock(); 
//using the mutex to unlock, so that other threads can edit now
}
FileSpooler::~FileSpooler()
{
    if ( f.is_open()) f.close();
}
