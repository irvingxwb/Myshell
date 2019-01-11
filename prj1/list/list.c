/* CMPT Assign 1
 * Author: 	Irving
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int item)
{
	Npointer newNode;
	
	newNode = (Npointer)malloc(sizeof(struct nodeStruct));
	
	newNode->item = item;
	newNode->next = NULL;
	
	return newNode;
}

/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node)
{
	Npointer temp;
	
	temp = *(headRef);
	*(headRef) = node;
	node->next = temp;
}

/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node)
{
	Npointer List, temp;
	
	List = *(headRef);
	temp = List;
	
	while(temp->next != NULL)
		temp = temp->next;
		
	temp->next = node;
}

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head)
{
	int sum;
	Npointer temp;
	
	temp = head;
	sum = 0;
	
	while(temp != NULL)
	{
		sum += 1;
		temp = temp->next;
	}
	
	return sum;
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item)
{
	Npointer temp;
	
	temp = head;
	
	while(temp->item != item && temp != NULL)
		temp = temp->next;
	
	//if item is not found, then null will be returned
	return temp;
}

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set to a valid node 
 * in the list. For example, the client code may have found it by calling 
 * List_findNode(). If the list contains only one node, the head of the list 
 * should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node)
{
	Npointer Dnode, Rnode;
	
	Dnode = *headRef;
	
	if(Dnode->item == node->item)
	{
		*headRef = (*headRef)->next;
		free(Dnode);
	}
	else
	{
		Rnode = Dnode;
		Dnode = Dnode->next;
		
		while(Dnode->item != node->item)
			if(Dnode != NULL)
			{
				Dnode = Dnode->next;
				Rnode = Rnode->next;
			}
			else
				break;
				
		if(Dnode != NULL)
		{
			Rnode->next = Dnode->next;
			free(Dnode);
		}
	}
} 

/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef)
{
	int n, i, j, t, *array;
	Npointer Pcount, temp;
	
	Pcount = *headRef;
	temp = *headRef;
	n = 0;
	while(Pcount != NULL)
	{
		n += 1;
		Pcount = Pcount->next;
	} 
	
	array = (int*)malloc(sizeof(int) * n);
	
	for(i=0;i<n;i++)
	{
		array[i] = temp->item;
		temp = temp->next;
	}
	
	for(i=0;i<n-1;i++)
		for(j=i+1;j<n;j++)
		{
			if(array[j] < array[i])
			{
				t = array[i];
				array[i] = array[j];
				array[j] = t;
			}
		}
		
	temp = *headRef;
	
	for(i=0;i<n;i++)
	{
		temp->item = array[i];
		temp = temp->next;
	}
	
}

