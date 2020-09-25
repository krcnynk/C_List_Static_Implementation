#include <stdio.h>
#include "list.h"

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
		List_add(list1,&list1items[i]);
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

	//Try creating 11 Lists
	return 0;
}