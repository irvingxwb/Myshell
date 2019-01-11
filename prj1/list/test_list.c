#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void PrintList(Npointer L)
{
	while(L!= NULL)
	{
		printf("%d ",L->item);
		L = L->next;
	}
	printf("\n");
}

int main()
{
	Npointer a, b, c, d, e, temp;
	int num;
	
	a = List_createNode(1);
	b = List_createNode(2);
	c = List_createNode(3);
	d = List_createNode(4);
	e = List_createNode(5);

	printf("Origin: a:%d b:%d\n",a->item,b->item);
	
	List_insertHead(&a,b);
	
	printf("After headinsert b to a: %d %d\n",a->item, a->next->item);	 
	
	List_insertTail(&a,c);
	
	printf("After tailinsert : %d %d %d\n",a->item, a->next->item, a->next->next->item);	 
	
	num = List_countNodes(a);
	printf("Count : %d \n",num);
	
	List_insertHead(&a,d);
	List_insertTail(&a,e);
	
	PrintList(a);
	
	temp = List_findNode(a, 4);
	printf("Find : %d \n",temp->item);
	
	List_deleteNode(&a,temp);
	printf("Delete the node that found : ");
	PrintList(a);
	
	List_sort(&a);
	printf("After sort : \n");
	PrintList(a);

	printf("Test Successful!\n");

	return 0;
	
}
