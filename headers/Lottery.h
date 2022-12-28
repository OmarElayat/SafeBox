#ifndef LOTTERY_H
#define LOTTERY_H

#include <includes.h>
#include <defines.h>
#include <bitset>
#include <random>
#include <thread>
#include <boost/dynamic_bitset.hpp>

using namespace std;

class Lottery {

    private:
        boost::dynamic_bitset<> * bitmap ;
        std::random_device random_device;
        std::mt19937 * random_engine;
        std::uniform_int_distribution<std::size_t> * distribution;
        std::size_t reserved;
        bool final_block;
        mutex mtx;
    public:
        Lottery(unsigned long bit_count);
        long withdraw ();        
        ~Lottery();
};



#endif