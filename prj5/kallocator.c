#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "kallocator.h"
#include "list.h"

struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    blockPtr allocatedM;
    blockPtr freeM;
    // Some other data members you want, 
    // such as lists to record allocated/free memory
};

struct KAllocator kallocator;


void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);
    blockPtr newblock;
    newblock = (blockPtr)malloc(sizeof(struct block));
    newblock->next = NULL;
    newblock->ptr = kallocator.memory;
    newblock->size = kallocator.size;
    kallocator.freeM = newblock;
    kallocator.allocatedM = NULL;
    // Add some other initialization 
}

void destroy_allocator() {
    free(kallocator.memory);
    blockPtr temp, freeptr;
    temp = kallocator.allocatedM;
    while(temp != NULL)
    {
        freeptr = temp;
        temp = temp->next;
        free(freeptr);
    }
    temp = kallocator.freeM;
    while(temp != NULL)
    {
        freeptr = temp;
        temp = temp->next;
        free(freeptr);
    }
    // free other dynamic allocated memory to avoid memory leak
}

void* kalloc(int _size) {
    blockPtr temp, newblock, res = NULL;;
    // Allocate memory from kallocator.memory 
    // ptr = address of allocated memory
    if(kallocator.aalgorithm == FIRST_FIT)
    {
        res = kallocator.freeM;
        while(res != NULL)
        {
            if(res->size > _size)
            {
                DeleteBlock(&kallocator.freeM,res);
                newblock = (blockPtr)malloc(sizeof(struct block));
                newblock->size = res->size - _size;
                newblock->ptr = res->ptr + _size;
                res->size = _size;
                res->next = NULL;
                newblock->next = NULL;
                InsertBlock(&kallocator.freeM,newblock);
                InsertBlock(&kallocator.allocatedM,res);
                return res->ptr;
            }
            res = res->next;
        }
        return NULL;
    }
    else if(kallocator.aalgorithm == BEST_FIT)
    {
        temp = kallocator.freeM;
        while(temp != NULL)
        {
            if(temp->size > _size)
            {
                if(res == NULL)
                    res = temp;
                else if(res->size > temp->size)
                    res = temp;
            }
            temp = temp->next;
        }
        DeleteBlock(&kallocator.freeM,res);
        newblock = (blockPtr)malloc(sizeof(struct block));
        newblock->size = res->size - _size;
        newblock->ptr = res->ptr + _size;
        res->size = _size;
        res->next = NULL;
        newblock->next = NULL;
        InsertBlock(&kallocator.freeM,newblock);
        InsertBlock(&kallocator.allocatedM,res);
        return res->ptr;
    }
    else    //WORST_FIT
    {
        temp = kallocator.freeM;
        while(temp != NULL)
        {
            if(temp->size > _size)
            {
                if(res == NULL)
                    res = temp;
                else if(res->size < temp->size)
                    res = temp;
            }
            temp = temp->next;
        }
        DeleteBlock(&kallocator.freeM,res);
        newblock = (blockPtr)malloc(sizeof(struct block));
        newblock->size = res->size - _size;
        newblock->ptr = res->ptr + _size;
        res->size = _size;
        res->next = NULL;
        newblock->next = NULL;
        InsertBlock(&kallocator.freeM,newblock);
        InsertBlock(&kallocator.allocatedM,res);
        return res->ptr;
    }
}

void kfree(void* _ptr) {
    assert(_ptr != NULL);
    blockPtr temp;
    temp = kallocator.allocatedM;
    while(temp != NULL)
    {
        if(temp->ptr == _ptr)
        {
            DeleteBlock(&kallocator.allocatedM,temp);
            InsertBlock(&kallocator.freeM,temp);
            Mergelist(&kallocator.freeM);
            break;
        }
        temp = temp->next;
    }
    return;
}

int compact_allocation(void** _before, void** _after) {
    int compacted_size = 0;
    int trace_size = 0;
    int count_size = 0;
    int Arrayindex = 0;
    void* temptr;
    blockPtr freeptr, allocatedptr;

    temptr = kallocator.memory;
    freeptr = kallocator.freeM;
    allocatedptr = kallocator.allocatedM;

    while(count_size < kallocator.size)  //get the before array and compact
    {
        if(temptr == freeptr->ptr)
        {
            _before[Arrayindex++] = freeptr->ptr;
            temptr = temptr + freeptr->size;
            count_size += freeptr->size;
            compacted_size += freeptr->size;
            freeptr = freeptr->next;
        }
        else
        {
            _before[Arrayindex++] = allocatedptr->ptr;
            if(compacted_size != 0)
            {
                memcpy(temptr-compacted_size, temptr, allocatedptr->size);
            }    
            temptr = temptr + allocatedptr->size;
            count_size += allocatedptr->size;
            allocatedptr = allocatedptr->next;
        }
    }
    
    trace_size = compacted_size;
    freeptr = kallocator.freeM;
    while(freeptr != NULL)
    {
    	freeptr->ptr = kallocator.memory + count_size - trace_size;
    	trace_size -= freeptr->size;
    	freeptr = freeptr->next;
	}
	Mergelist(&kallocator.freeM);

    temptr = kallocator.memory;
    freeptr = kallocator.freeM;
    allocatedptr = kallocator.allocatedM;
    Arrayindex = 0;

    while(count_size < kallocator.size)  //get the after array and compact
    {
        if(temptr == freeptr->ptr)
        {
            _after[Arrayindex++] = freeptr->ptr;
            temptr = temptr + freeptr->size;
            count_size += freeptr->size;
            freeptr = freeptr->next;
        }
        else
        {
            _after[Arrayindex++] = allocatedptr->ptr;
            temptr = temptr + allocatedptr->size;
            count_size += allocatedptr->size;
            allocatedptr = allocatedptr->next;
        }
    }
    
    // compact allocated memory
    // update _before, _after and compacted_size

    return compacted_size;
}

int available_memory() {
    int available_memory_size = 0;
    blockPtr temp;
    temp = kallocator.freeM;
    while(temp != NULL)
    {
        available_memory_size += temp->size;
        temp = temp->next;
    }
    // Calculate available memory size
    return available_memory_size;
}

void print_statistics() {
    int allocated_size = 0;
    int allocated_chunks = 0;
    int free_size = 0;
    int free_chunks = 0;
    int smallest_free_chunk_size = kallocator.size;
    int largest_free_chunk_size = 0;

    blockPtr temp;
    temp = kallocator.allocatedM;
    while(temp != NULL)
    {
        allocated_size += temp->size;
        allocated_chunks++;
        temp = temp->next;
    }
    temp = kallocator.freeM;
    while(temp != NULL)
    {
        free_size += temp->size;
        free_chunks++;

        if(smallest_free_chunk_size > temp->size)
            smallest_free_chunk_size = temp->size;
        if(largest_free_chunk_size < temp->size)
            largest_free_chunk_size = temp->size;

        temp = temp->next;
    }
    // Calculate the statistics

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}
/*
void ShowList()
{
	blockPtr temp;
	temp = kallocator.freeM;
	printf("Free block: ");
	while(temp != NULL)
	{
		printf("%d ",temp->size);
		temp = temp->next;
	}
	printf("\n");
	printf("Allocated block: ");
	temp = kallocator.allocatedM;
	while(temp != NULL)
	{
		printf("%d ",temp->size);
		temp = temp->next;
	}
	printf("\n------------------------\n");
}
*/

