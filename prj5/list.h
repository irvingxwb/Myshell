#ifndef _myLinklist 
#define _myLinklist

typedef struct block* blockPtr;
struct block{
    int size;
    void* ptr;
    blockPtr next;
};

void InsertBlock(blockPtr* S, blockPtr T);
void DeleteBlock(blockPtr* S, blockPtr T);
void MergeBlock(blockPtr s1, blockPtr s2);
void Mergelist(blockPtr* Head);
void MoveBlock(void* ptr, int movesize, int distance);

#endif
