#ifndef DEFINES_H
#define DEFINES_H

typedef unsigned __int128 uint128_t;

#define COMMAND_LINE_ARGUMENT_MAX_SIZE 128
#define ERROR_STRING_MAX_SIZE 128



/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

#define PHASE1     1
#define PHASE2     2

namespace CryptoPP{
    typedef unsigned char byte;
};

typedef struct sb_block_index{ 
    long block;
    long shred_block;
    char shred;
} sb_block_index_t;


#endif
