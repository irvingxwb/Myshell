/* CMPT Assign 5 lib
 * Author: 	Irving
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>


void InsertBlock(blockPtr* S, blockPtr T) 
{
	blockPtr head, temp, trace;
	head = *S;
	if(head == NULL)
	{
		*S = T;
		return;
	}
	if(head->next == NULL)
	{
		if(T->ptr < head->ptr)
		{
			temp = *S;
			*S = T;
			T->next = temp;
		}
		else
		{
			head->next = T;
		}
	}
	else
	{
		temp = *S;
		while(temp != NULL && temp->ptr < T->ptr)
			temp = temp->next;
		if(temp == *S)
		{
			*S = T;
			(*S)->next = temp;
		}
		else
		{
			trace = *S;
			while(trace->next != temp)
				trace = trace->next;
			trace->next = T;
			T->next = temp;
		}
	}
	return;
}

void DeleteBlock(blockPtr* S, blockPtr T)
{
	blockPtr temp, tmp;
	temp = tmp = *S;

	if(temp == T)
	{
		*S = (*S)->next;
		return;
	}

	while(temp != NULL)
	{
		if(temp == T)
		{
			while(tmp->next != temp)
				tmp = tmp->next;
			tmp->next = temp->next;
			return;
		}
		temp = temp->next;
	}
}

void MergeBlock(blockPtr s1, blockPtr s2)
{
	s1->size += s2->size;
	s1->next = s2->next;
	free(s2);
}

void Mergelist(blockPtr* head)
{
	blockPtr first, second;
	first = *head;
	if(first->next == NULL)
		return;

	second = first->next;
	while(second != NULL)
	{
		if(first->ptr + first->size == second->ptr)
		{
			MergeBlock(first, second);
			if(first->next == NULL)
				break;
			else
				second = first->next;
		}
		else
		{
			first = first->next;
			second = second->next;
		}
	}
	return;
}



