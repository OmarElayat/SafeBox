#include <ThreadManager.h>


ThreadManager::ThreadManager(){

}
void ThreadManager::operator += (Thread * t){
    threads.push_back(t);
}
void ThreadManager::start ()
{
//iterating on all the threads and starting them one by one
for ( auto x = threads.begin() ; x!=threads.end() ; x++) 
x[0]->start();
}
void ThreadManager::barrier (){
//iterating on all the threads and joining them one by one
for ( auto x = threads.begin() ; x!=threads.end() ; x++) 
x[0]->wait(); 
}
ThreadManager::~ThreadManager(){
}
