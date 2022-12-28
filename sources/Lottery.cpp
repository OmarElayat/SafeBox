#include <Lottery.h>

Lottery::Lottery(unsigned long bit_count)
{
    random_engine = new std::mt19937 (random_device());
    distribution = new std::uniform_int_distribution<std::size_t> (0,(bit_count<2)?0:bit_count-2);
    reserved = 0;
    if ( bit_count != 0 ) bit_count--;
    bitmap = new boost::dynamic_bitset <> (bit_count);
    final_block = false;
}

long Lottery::withdraw ()
{
    mtx.lock();
    if ( final_block)
    {
        mtx.unlock();
        return -1;
    }
    else if (reserved == bitmap->size()) 
    {
        final_block = true;
        mtx.unlock();
        return bitmap->size();
    }
    auto randomNumber =(*distribution)(*random_engine);
    if (!bitmap->test(randomNumber))
    {
        bitmap->set(randomNumber);
        reserved++;
    }
    else {
        auto r = (randomNumber +1)% bitmap->size();
        for ( ; r != randomNumber; r = (r+1) % bitmap->size() )
        {
            if (!bitmap->test(r))
            {
                bitmap->set(r);
                reserved++;
                randomNumber =r;
                break;
            }
        }
    }
    mtx.unlock();
    return randomNumber;
}

Lottery::~Lottery()
{
    delete (distribution);
    delete (random_engine);
    delete(bitmap);
}
