#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void cleanup(void* item)
{
	if(item == NULL)
	{
		printf("Null\n");
	}
	printf("FREEING THE ITEM %d\n",*(int*)(item));
}

bool compare(void*a,void*b)
{
	if(*(int*)a == *(int*)b)
	{
		return true;
	}
	else
		return false;
} 

int main()
{
	List* list1 = List_create();
	//Lets check empty list
	printf("%d\n",List_count(list1));
	if(List_first(list1) != NULL)
		printf("ERROR\n");
	if(List_last(list1) != NULL)
		printf("ERROR\n");
	if(List_next(list1) != NULL)
		printf("ERROR\n");
	if(List_prev(list1) != NULL)
		printf("ERROR\n");
	if(List_curr(list1) != NULL)
		printf("ERROR\n");
	if(List_trim(list1) != NULL)
		printf("ERROR\n");
	if(List_remove(list1) != NULL)
		printf("ERROR\n");

	int list1items[10] = {0,1,2,3,4,5,6,7,8,9};
	for(int i = 0;i < 10; i++)
	{
		List_add(list1,&list1items[i]);
	}
	//Printing the list(should be taking nodes 103 to 94)
	printOneList(list1);
	//Printing current and go back 5 steps and 1 step forward, then remove the item, go back two step and remove again
	printf("Current = %d \n",*(int*)(List_curr(list1)));
	for(int i = 0;i<5;i++)
		printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Current = %d \n",*(int*)(List_next(list1)));
	printf("Removing = %d \n",*(int*)(List_remove(list1)));
	printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Removing = %d \n",*(int*)(List_remove(list1)));
	//Print disjointed list
	printOneList(list1);
	printf("//////////////\n");
	//Add two items to the list , it should be non disjoint again
	List_add(list1,&list1items[3]);
	List_add(list1,&list1items[5]);
	//Print non disjointed list
	printOneList(list1);

	//Create another list
	List* list2 = List_create();
	int* itemArr = malloc(4*sizeof(int));
	for(int i = 0;i<2;i++)
	{
		itemArr[i] = i;
		List_insert(list2,&itemArr[i]);
	}
	//Various functions and outputs
	List_append(list2,&itemArr[2]);
	List_prepend(list2,&itemArr[3]);
	printOneList(list2);
	List_remove(list2);
	printf("AFTER LIST REMOVE list2");
	printOneList(list2);
	List_concat(list1,list2);
	printf("AFTER LIST CONC list1");
	printOneList(list1);
	List_trim(list1);
	printf("AFTER LIST TRIM list1");
	printOneList(list1);

	void(*fptr)(void*) = &cleanup; // defining my cleanup fun pointer
	
	List* list3 = List_create();
	int* itemArr2;
	for(int i = 0;i<78;i++)
	{
		int* itemArr2 = (int*) malloc(sizeof(int));
		*itemArr2 = i;
		List_insert(list3,itemArr2);
	}
	printf("BEFORE LIST FREE LIST3\n");
	printOneList(list3);
	bool(*fcomparePtr)(void*,void*) = &compare;
	int a = 44;
	int b = 79;
	void* returns1;
	void* returns2;
	returns1 = List_search(list3,fcomparePtr,&a);
	returns2 = List_search(list3,fcomparePtr,&b);
	if(returns1 != NULL)
		printf("%d",*(int*)returns1);
	if(returns2 != NULL)
		printf("%d",*(int*)returns2);

	List_free(list3,fptr);

	return 0;
}