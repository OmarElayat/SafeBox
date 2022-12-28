#include <Shred.h>

Shred::Shred(const char * p_file_name,uint16_t block_size,bool truncate){
    fileSpooler = new FileSpooler(p_file_name,block_size,truncate);
}
void Shred::operator << (Block & b){
    fileSpooler->appendBlock(&b);
}
Block * Shred::getNextBlock ()
{
    return fileSpooler->getNextBlock();
}

Shred::~Shred()
{
    delete (fileSpooler);
}
