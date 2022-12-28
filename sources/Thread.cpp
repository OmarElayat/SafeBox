#include <Thread.h>

void Thread::fire (Thread * me)
{
    me->mainThreadBody();
    me->run_once.unlock();
}
Thread::Thread ()
{
    t = NULL;
}

thread * Thread::start ()
{
    run_once.lock();
    t = new thread(Thread::fire,this);
    return t; 
}
void Thread::wait()
{
    if ( t != NULL) t->join();
}

Thread::~Thread()
{
    if ( t != NULL) delete(t);
}
