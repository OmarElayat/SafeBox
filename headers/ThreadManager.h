#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <Thread.h>

using namespace std;
class ThreadManager {

    private:
        vector<Thread *> threads;
    public:
        ThreadManager();
        void operator += (Thread * t);
        void start();
        void barrier ();
        ~ThreadManager();
};


#endif