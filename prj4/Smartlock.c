#include <stdio.h>
#include <stdlib.h>
#include "Smartlock.h"

int count=0;
SNode Head, Tail;
                                                                                                     
void init_lock(SmartLock* lock) 
{
    if(count == 0)
    {
        Head = (SNode)malloc(sizeof(SNode));
        Tail = Head;
        Head->slock = lock;
        Head->next = NULL;
        lock->Sid = count++;
        lock->Athread = NULL;
        lock->Rthread = NULL;

        pthread_mutex_init(&(lock->mutex), NULL);
    }
    else
    {
        SNode new;
        new = (SNode)malloc(sizeof(SNode));
        new->slock = lock;
        new->next = NULL;
        lock->Sid = count++;
        lock->Athread = NULL;
        lock->Rthread = NULL;

        pthread_mutex_init(&(lock->mutex), NULL);
        Tail->next = new;
        Tail = Tail->next;
    }
}

SmartLock* FindSource(pid_t pid) //find next source that the thread is in its require list
{                                //trace the cycle through the way the same with arrows' direction
    int flag = 0;
    SNode stmp;
    TNode ttmp;
    SmartLock* tmplock;
    stmp = Head;
    while(stmp != NULL)
    {
        tmplock = stmp->slock;
        ttmp = tmplock->Rthread;
        while(ttmp != NULL)
        {
            if(ttmp->pid == pid)
            {
                flag = 1;
                break;
            }
            else
                ttmp = ttmp->next;
        }
        
        if(flag == 1)
            break;
        
        stmp = stmp->next;
    }
    
    if(stmp != NULL)
        return stmp->slock;     //return next lock(source)
    else 
        return NULL;
}


int Trace(SmartLock* lock, int Sid)
{
    pid_t pid;
    SmartLock* nextS;
    if(lock->Athread == NULL)
        return 0;
    else
    {
        pid = lock->Athread->pid;
        nextS = FindSource(pid);        //trace back 
        if(nextS == NULL)               //NULL means trace end is NULL, no cycle
            return 0;

        if(Sid == nextS->Sid)           //find cycle
            return 1;
        else
            return Trace(nextS, Sid);   //else continue trace, Sid is the origin source id
    }
}

int DetectCycle(SmartLock* lock)
{
    if(Trace(lock,lock->Sid))       //function for iterative call trace
        return 1;
    else 
        return 0;
}
    

int lock(SmartLock* lock)
{
    int isRhead = 0, isAssign = 0;
    pid_t npid;
    TNode T, temp;
    npid = pthread_self();

    T = (TNode)malloc(sizeof(TNode));
    T->pid = npid;
    T->next = NULL;
    
    if(lock->Athread == NULL)
    {
        lock->Athread = T;
        if(DetectCycle(lock) == 1)
        {
            lock->Athread = NULL;
            free(T);
            return 0;
        }    
        isAssign = 1;
    }
    else
    {
        temp = lock->Rthread;
        if(temp == NULL)            //considering mutiple threads require a same source
        {
            lock->Rthread = T;
            isRhead = 1;
        }
        else
        {
            while(temp != NULL)     //find the tail of require thread list in the lock
            {
                if(temp->next == NULL)
                {
                    temp->next = T;
                    break;
                }
                temp = temp->next;
            }
            isRhead = 0;
        }
       // printf("%d thread require %d source\n",npid,lock->Sid);
        if(DetectCycle(lock) == 1)
        {
           // printf("cycle detected %d\n",lock->Sid);
            if(isRhead == 1)            //detcet cyle and if find, delete the node in Rrquire thread list
            {                           //deal with node in list head or not
                free(lock->Rthread);
                lock->Rthread = NULL;
                return 0;
            }
            else
            {
                free(temp->next);
                temp->next = NULL;
                return 0;
            }
        }
    }

    pthread_mutex_lock(&(lock->mutex)); //if the thread is require source, it will block until being assigned the source
    //printf("%d thread lock source %d\n",npid,lock->Sid);

    if(isAssign == 0)   //move the thread to assign list if the node is not assigned yet
    {
        lock->Athread = T;
        temp = lock->Rthread;
        if(temp->pid == npid)
            lock->Rthread = NULL;
        else
        {
            while(temp->next->pid != npid)
                temp = temp->next;
            temp->next = NULL;
        }
       // printf("move thread %d from R to A\n",npid);
    }

    return 1;
}

void unlock(SmartLock* lock)
{
    free(lock->Athread);        //unlock the smartlock and delete the aquire list
    lock->Athread = NULL;
    pthread_mutex_unlock(&(lock->mutex));
   // printf("unlock %d\n",lock->Sid);
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() 
{
    SNode tempS, freeS;
    // TNode tempT, freeT;

    tempS = Head;
    while(tempS != NULL)  //free the malloc memory in hea
    {
        freeS = tempS;
        free(freeS);
        tempS = tempS->next;
    }
}