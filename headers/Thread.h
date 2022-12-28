#ifndef THREAD_H
#define THREAD_H

#include <includes.h>
#include <defines.h>
#include <thread>
#include <mutex>

using namespace std;

class Thread {
    private:
    protected:
        thread * t;
        mutex run_once;
        static void fire (Thread * me);
    public:
        Thread ();
        virtual void mainThreadBody () = 0;
        thread * start ();
        void wait();
        virtual ~Thread();
};

#endif
