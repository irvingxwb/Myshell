#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>
#include "bbuff.h"

void* buffer[BUFFER_SIZE];
int count, in, out;

void bbuff_init(void)
{	
	int i;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		if(buffer[i] != NULL)
		{
			free(buffer[i]);
			buffer[i] = NULL;
		}
	}
	count = 0;
	in = 0;
	out = 0;
}

void bbuff_blocking_insert(void* item)
{
	while(count == BUFFER_SIZE);
	
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	count++;
}

void* bbuff_blocking_extract(void)
{
	void* candy;
	while(count == 0);
	candy = buffer[out];
	out = (out + 1) % BUFFER_SIZE;
	count--;
	return candy;
}

_Bool bbuff_is_empty(void)
{
	if(count == 0)
		return true;
	else
		return false;
}

