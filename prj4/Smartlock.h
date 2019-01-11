#ifndef _SMARTLOCK_H_
#define _SMARTLOCK_H_

#include <pthread.h>

typedef struct ThreadList* TNode;
struct ThreadList{
    pid_t pid;
    TNode next;
};

typedef struct {
    pthread_mutex_t mutex;
    int Sid;
    TNode Athread;     //thread id that assign the source, assume only one thread could use the source at a time
    TNode Rthread;     //thread id that reuire the source, assume many threads could require the source 
    /* Add other variables */
} SmartLock;

typedef struct SmartLockList* SNode;
struct SmartLockList{
    SmartLock *slock;
    SNode next;
};


void init_lock(SmartLock* lock);
int lock(SmartLock* lock);
void unlock(SmartLock* lock);
void cleanup();
int Trace(SmartLock* lock, int Sid);
int DetectCycle(SmartLock* lock);

#endif
